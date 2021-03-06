#include "pch.h"
#include "Processor.h"

Processor::Processor()
{
	Clear();
}

Processor::~Processor()
{
	m_PACallback = nullptr;
	m_PACaller = nullptr;
}

void Processor::AssignNumber(si16 number)
{
	AssignValueInternal(number, 0);
}

void Processor::AssignValue(int value)
{
	AssignValueInternal(abs(value), 0);

	if (value < 0)
	{
		AssignOperator(Inputs::Op_Inverse);
	}
}

void Processor::AssignValue(float value, int decimalDigit)
{
	AssignValueInternal(fabs(value), decimalDigit);

	m_arg->nType |= ARG_NUMBER_DECIMAL;
	ChangeArgumentsToDecimal(true);

	if (value < FLT_EPSILON)
	{
		AssignOperator(Inputs::Op_Inverse);
	}
}

template <typename _Type>
void Processor::AssignValueInternal(_Type value, int decimalDigit)
{
	bool isDecimal = (decimalDigit > 0);

	// initial state or not decimal result argument
	if ((IsFirstArg() && !m_arg->isInitialized())
		|| (m_arg->nType & ARG_NUMBER_RESULT && m_arg->svali() != 0))
	{
		m_arg->reset();
		m_text.clear();
		m_lastError = Error::OPERATION_SUCESSFUL;
		m_bIsDecimal = false;
	}
	// stream value is 0 and not a decimal number
	else if (m_arg->isInitialized()
		&& m_arg->svali() == 0
		&& !(m_arg->nType & ARG_NUMBER_DECIMAL))
	{
		if ((isDecimal && equals(value, 0.f))
			|| (!isDecimal && value == 0))
		{
			return;
		}
		else
		{
			m_arg->reset();
			m_text.pop_back();
		}
	}

	if (m_arg->nType & ARG_NUMBER_RESULT)
	{
		m_arg->nType ^= ARG_NUMBER_RESULT;
	}
	
	m_arg->nType |= ARG_NUMBER;
	(*m_arg) << value;

	if (isDecimal)
	{
		// avoiding text has different decimal digit with it's value
		const int BUFF_MAX = 2048;
		char tmp[BUFF_MAX];
		string arg;
		arg += "%.";
		arg += to_string(decimalDigit);
		arg += 'f';

		sprintf_s(tmp, BUFF_MAX, arg.c_str(), value);

		m_text += tmp;
	}
	else
	{
		m_text += to_string(value);
	}
}


void Processor::AssignOperator(char c)
{
	AssignOperator(Inputs::ConvertToOperator(c));
}

void Processor::AssignOperator(si32 op)
{
	if (op == Inputs::Op_Result)
	{
		if (m_arg->nType & ARG_NUMBER_RESULT)
		{
			return;
		}

		if (!m_arg->isInitialized() && m_prevArg && m_prevArg->nType & ARG_OPERATOR)
		{
			PrevArgument(); // set to operator
			MF_ASSERT(m_arg->nType & ARG_OPERATOR);

			PrevArgument(); // set to number
			MF_ASSERT(m_arg->nType & ARG_NUMBER);

			m_text.pop_back();
		}
		else
		{
			m_arg->applyStream(m_bIsDecimal);
		}

		m_lastError = ProcessResult(m_PACallback, m_PACaller);
		m_prevArg = NULL;
		MF_ASSERT_MSG(m_argIdx == OS_FIRST_ARG, "Something wrong on ProcessResult !");
	}
	else if (op == Inputs::Op_Point)
	{
		MF_ASSERT(!(m_arg->nType & ARG_OPERATOR));

		if (!(m_arg->nType & ARG_NUMBER_DECIMAL))
		{
			if (!m_arg->isInitialized() || m_arg->nType & ARG_NUMBER_RESULT)
			{
				AssignNumber(Inputs::Number_0);
			}

			m_arg->nStream << k_op_point;
			m_text += k_op_point;

			ChangeArgumentsToDecimal(true);

			m_arg->nType |= ARG_NUMBER_DECIMAL;
		}
	}
	else if (op == Inputs::Op_Inverse)
	{
		if (!m_arg->isInitialized()
			|| (m_arg->nType & ARG_NUMBER_DECIMAL && equals(m_arg->svalf(), 0.f))
			|| m_arg->svali() == 0)
		{
			return;
		}

		if (m_arguments[OS_FIRST_NUM].nType & ARG_NUMBER_RESULT)
		{
			m_arguments[OS_FIRST_NUM].nType ^= ARG_NUMBER_RESULT;
		}

		stringstream operators;
		string originalStr = m_arg->nStream.str();
		m_arg->resetStream();

		if (m_arg->nType & ARG_NUMBER_INVERSE)
		{
			originalStr.erase(0, 1);
			m_arg->nStream << originalStr;

			operators << k_open_parenthesis;

			size_t opPos = m_text.find_last_of(operators.str());
			MF_ASSERT(opPos != string::npos);

			m_text.erase(opPos);			
			m_text += m_arg->nStream.str();
		}
		else
		{
			m_arg->nStream << k_op_subtraction << originalStr;

			operators << k_op_math_operation;
			
			size_t opPos = m_text.find_last_of(operators.str());
			if (opPos != string::npos)
			{
				m_text.erase(opPos + 1);
			}
			else
			{
				m_text.clear();
			}
			
			m_text += k_open_parenthesis;
			m_text += m_arg->nStream.str();
		}

		m_arg->nType ^= ARG_NUMBER_INVERSE;
	}
	else
	{
		if (m_arguments[OS_FIRST_NUM].nType & ARG_NUMBER_RESULT)
		{
			m_arguments[OS_FIRST_NUM].nType ^= ARG_NUMBER_RESULT;
		}

		if (!m_arg->isInitialized() && m_prevArg && m_prevArg->nType & ARG_OPERATOR)
		{
			m_text.pop_back();
			m_prevArg->applyOperator(op);
		}
		else
		{
			if (!m_arg->isInitialized())
			{
				AssignNumber(Inputs::Number_0);
			}

			NextArgument();
			MF_ASSERT(m_prevArg);

			ValidateStreamAndText(m_prevArg, &m_text);

			if (m_lastError != Error::OPERATION_SUCESSFUL)
			{
				m_text = m_arg->str();
				m_lastError = Error::OPERATION_SUCESSFUL;
			}

			m_prevArg->applyStream(m_bIsDecimal);
			m_arg->applyOperator(op);
			
			if (m_argIdx >= OS_MAX_ARG_INDEX)
			{
				m_lastError = ProcessResult(m_PACallback, m_PACaller);
				MF_ASSERT_MSG(m_argIdx == OS_FIRST_OP, "Something wrong on ProcessResult !");
			}
			
			NextArgument();
		}

		switch (op)
		{
		case Inputs::Op_Multiplication:	m_text += k_op_mulplication; break;
		case Inputs::Op_Division:		m_text += k_op_division; break;
		case Inputs::Op_Addition:		m_text += k_op_addition; break;
		case Inputs::Op_Subtraction:	m_text += k_op_subtraction; break;
		default:
			break;
		}
	}
}

void Processor::Clear()
{
	m_arguments.clear();
	m_arguments.resize(OS_MAX_ARG_COUNT);

	m_prevArg = NULL;
	m_arg = &m_arguments[OS_FIRST_ARG];

	m_argIdx = OS_FIRST_ARG;
	m_bIsDecimal = false;

	(*m_arg) << k_number_0;
	m_text = m_arg->str();
}

void Processor::EraseBack()
{
	if (m_text.size() <= 1 || m_lastError != OPERATION_SUCESSFUL)
	{
		Clear();
		return;
	}

	if (!m_arg->isInitialized())
	{
		PrevArgument();

		MF_ASSERT(m_arg->nType & ARG_OPERATOR);
		PrevArgument();

		m_text.pop_back();

		if (m_arg->nType & ARG_NUMBER_INVERSE)
		{
			m_text.pop_back(); // remove ')'
		}

		return;
	}

	MF_ASSERT(m_arg->nStream.tellp() > 0);

	string originalStr = m_arg->nStream.str();
	m_arg->resetStream();

	char c = originalStr.back();
	if (c == k_op_point)
	{
		m_arg->nType ^= ARG_NUMBER_DECIMAL;
	}

	originalStr.pop_back();
	m_text.pop_back();

	if (originalStr.size() > 0)
	{
		c = originalStr.back();
		if (c == k_op_subtraction)
		{
			m_arg->reset();

			MF_ASSERT(m_text.size() >= 2);
			m_text.pop_back(); // remove '-'
			m_text.pop_back(); // remove '('

			if (m_text.size() <= 0)
			{
				Clear();
			}

			return;
		}
		else
		{
			m_arg->nStream << originalStr;	
		}
	}
	else
	{
		m_arg->reset();
	}

	if (m_arg->nType & ARG_NUMBER_RESULT)
	{
		m_arg->nType ^= ARG_NUMBER_RESULT;
	}
}

void Processor::SetAssignCallback(ProcessorAssignCallback func, void* caller)
{
	m_PACallback = func;
	m_PACaller = caller;
}

bool Processor::IsHighPriority(sbit16 op)
{
	return op & (ARG_OPERATOR_MULTIPLICATION | ARG_OPERATOR_DIVISION);
}

int Processor::ProcessResult(ProcessorAssignCallback callback, void* caller)
{
#define FARGUMENT_OPERATION(op)	m_arguments[leftArg].nfArg op##= m_arguments[rightArg].nfArg
#define IARGUMENT_OPERATION(op)	m_arguments[leftArg].niArg op##= m_arguments[rightArg].niArg

	if (callback)
	{
		ValidateStreamAndText(m_arg, &m_text);
		callback(Inputs::Op_Result, (void*)&m_text, caller);
	}

	int leftArg, rightArg ,opArg;

	while (m_argIdx >= OS_SECOND_NUM)
	{
		if (!IsHighPriority(m_arguments[OS_SECOND_OP].nType) 
			|| IsHighPriority(m_arguments[OS_FIRST_OP].nType) 
			|| m_argIdx == OS_SECOND_NUM)
		{
			leftArg = OS_FIRST_NUM;
			rightArg = OS_SECOND_NUM;
			opArg = OS_FIRST_OP;
		}
		else
		{
			leftArg = OS_SECOND_NUM;
			rightArg = OS_THIRD_NUM;
			opArg = OS_SECOND_OP;
		}

		try
		{
			switch (m_arguments[opArg].nType)
			{
			case ARG_OPERATOR_MULTIPLICATION:
			{
				if (m_bIsDecimal)	FARGUMENT_OPERATION(*);
				else				IARGUMENT_OPERATION(*);
				break;
			}
			case ARG_OPERATOR_DIVISION:
			{
				if (m_arguments[rightArg].niArg == 0)
				{
					throw m_arguments[leftArg].niArg == 0 
						? Error::OPERATION_EXCEPTION_DIV_ZERO_INF 
						: Error::OPERATION_EXCEPTION_DIV_ZERO;
				}

				if (m_bIsDecimal)
				{
					FARGUMENT_OPERATION(/);
				}
				else
				{
					if ((m_arguments[leftArg].niArg % m_arguments[rightArg].niArg) != 0
						&& !m_bIsDecimal)
					{
						ChangeArgumentsToDecimal(false);
						return ProcessResult(nullptr, nullptr);
					}

					IARGUMENT_OPERATION(/);
				}

				break;
			}
			case ARG_OPERATOR_ADDITION:
			{
				if (m_bIsDecimal)	FARGUMENT_OPERATION(+);
				else				IARGUMENT_OPERATION(+);
				break;
			}
			case ARG_OPERATOR_SUBTRACTION:
			{
				if (m_bIsDecimal)	FARGUMENT_OPERATION(-);
				else				IARGUMENT_OPERATION(-);
				break;
			}
			default:
				break;
			}
		}
		catch (Error e)
		{
			Clear();

			switch (e)
			{
			case Processor::OPERATION_EXCEPTION_DIV_ZERO:
				m_text = k_exceptionMsgDivZero;
			case Processor::OPERATION_EXCEPTION_DIV_ZERO_INF:
				m_text = k_exceptionMsgDivZeroInf;
			}

			return e;
		}

		m_arguments.erase(m_arguments.begin() + rightArg);
		m_arguments.erase(m_arguments.begin() + opArg);

		m_argIdx -= OS_OPERATOR_SHIFT;
	}

	// number or operator
	m_arg = &m_arguments[m_argIdx];
	m_arguments[OS_FIRST_NUM].nType = ARG_NUMBER | ARG_NUMBER_RESULT;

	// to do : apply new decimal & inverse
	if (m_bIsDecimal)
	{
		m_arguments[OS_FIRST_NUM].nType |= ARG_NUMBER_DECIMAL;

		if (m_arguments[OS_FIRST_NUM].nfArg < -FLT_EPSILON)
		{			
			m_arguments[OS_FIRST_NUM].nType |= ARG_NUMBER_INVERSE;
		}
		else if (m_arguments[OS_FIRST_NUM].nfArg < FLT_EPSILON)
		{			
			m_bIsDecimal = false;
			m_arguments[OS_FIRST_NUM].nfArg = NULL;
			m_arguments[OS_FIRST_NUM].nType ^= ARG_NUMBER_DECIMAL;
		}
	}
	else
	{
		if (m_arguments[OS_FIRST_NUM].niArg < 0)
		{
			m_arguments[OS_FIRST_NUM].nType |= ARG_NUMBER_INVERSE;
		}
	}

	m_arguments[OS_FIRST_NUM].applyValue();

	// fill text with remaining argument(s)
	m_text.clear();

	if (m_arguments[OS_FIRST_NUM].nType & ARG_NUMBER_INVERSE)
	{
		m_text += k_open_parenthesis;
	}

	m_text += m_arguments[OS_FIRST_NUM].str();
	m_arguments.resize(OS_MAX_ARG_COUNT);

	ValidateStreamAndText(&m_arguments[OS_FIRST_NUM], &m_text);

	return Error::OPERATION_SUCESSFUL;
}

void Processor::ChangeArgumentsToDecimal(bool skipCurrent)
{
	// floating for the first time
	if (!m_bIsDecimal && m_argIdx >= OS_SECOND_NUM)
	{
		int idx = m_argIdx;

		while (idx >= OS_FIRST_NUM)
		{
			if (!skipCurrent)
			{
				m_arguments[idx].nfArg = m_arguments[idx].niArg;
				m_arguments[idx].nType |= ARG_NUMBER_DECIMAL;
			}
			else
			{
				skipCurrent = false;
			}

			idx -= OS_OPERATOR_SHIFT;
		}
	}

	m_bIsDecimal = true;
}

void Processor::NextArgument()
{
	m_prevArg = m_arg;
	m_arg = &m_arguments[++m_argIdx];
}

void Processor::PrevArgument()
{
	m_argIdx--;
	m_arg->reset();
	m_arg = m_prevArg;
	m_prevArg = m_argIdx > OS_FIRST_ARG ? &m_arguments[m_argIdx - 1] : NULL;
}

void Processor::ValidateStreamAndText(Argument* pArg, string* text)
{
	MF_ASSERT(pArg && text);

	if (pArg->nType & ARG_NUMBER_DECIMAL)
	{
		size_t opPos = pArg->nStream.str().find_last_of(k_op_point);
		if (opPos == string::npos)
		{
			pArg->nStream << k_op_point << k_number_0;
			*text += k_op_point;
			*text += k_number_0;
		}
		else if (opPos == pArg->nStream.str().size() - 1)
		{
			pArg->nStream << k_number_0;	
			*text += k_number_0;
		}
	}

	if (pArg->nType & ARG_NUMBER_INVERSE)
	{
		size_t opPos;
		
		opPos = text->find_last_of(k_open_parenthesis);
		MF_ASSERT(opPos != string::npos);

		opPos = text->find_first_of(k_close_parenthesis, opPos);
		if (opPos == string::npos)
		{
			*text += k_close_parenthesis;
		}
	}
}

Processor::Argument::Argument()
	: nfArg(0.f)
	, niArg(0)
	, nType(ARG_UNDEFINED)
{
}

Processor::Argument::Argument(const Argument& pArg)
{
	*this = pArg;
}

Processor::Argument::~Argument()
{
	reset();
}

void Processor::Argument::reset(Argument* pArg)
{
	if (pArg)
	{
		*this = *pArg;
	}
	else
	{
		nfArg = NULL;
		nType = ARG_UNDEFINED;
		resetStream();
	}
}

void Processor::Argument::resetStream()
{
	nStream.str("");
	nStream.clear();
}

void Processor::Argument::applyStream(bool isDecimal)
{
	if (isDecimal)
	{
		nStream >> nfArg;
	}
	else
	{
		nStream >> niArg;
	}

	nType |= ARG_NUMBER;
}

void Processor::Argument::applyValue()
{
	resetStream();

	if (nType & ARG_NUMBER_DECIMAL)
	{
		nStream << nfArg;
	}
	else
	{
		nStream << niArg;
	}
}

void Processor::Argument::applyOperator(si16 op)
{
	resetStream();

	switch (op)
	{
	case Inputs::Op_Multiplication:
		nType = ARG_OPERATOR_MULTIPLICATION;
		nStream << k_op_mulplication;
		break;
	case Inputs::Op_Division:
		nType = ARG_OPERATOR_DIVISION;
		nStream << k_op_division;
		break;
	case Inputs::Op_Addition:
		nType = ARG_OPERATOR_ADDITION;
		nStream << k_op_addition;
		break;
	case Inputs::Op_Subtraction:
		nType = ARG_OPERATOR_SUBTRACTION;
		nStream << k_op_subtraction;
		break;
	default:
		nType = ARG_UNDEFINED;
		break;
	}
}

Processor::Argument* Processor::Argument::operator=(const Argument& oth)
{
	nfArg = oth.nfArg;
	niArg = oth.niArg;
	nType = oth.nType;

	resetStream();
	nStream << oth.nStream.str();

	return this;
}