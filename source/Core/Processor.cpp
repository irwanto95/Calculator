#include "pch.h"
#include "Processor.h"

#define prev	m_argIdx - 1
#define cur		m_argIdx



Processor::Processor()
	: m_lastError(0)
	, m_lastInput(Status::INPUT_NONE)
{
	Clear();
}

Processor::~Processor()
{
}

void Processor::AssignNumber(unum num)
{
	if (m_valueStr.tellp() > 0 && atoi(m_valueStr.str().c_str()) == 0)
	{
		if (num == 0)
		{
			return;
		}
		else if (!(m_lastInput & Status::NUMBER_DECIMAL))
		{
			ResetStream();
			m_text.pop_back();
		}
	}

	if (m_lastInput == Status::INPUT_NONE)
	{
		ResetStream(true);
		m_lastError = Error::OPERATION_SUCESSFUL;
		m_bIsDecimal = false;
	}

	m_valueStr << num;
	m_text += to_string(num);

	if (m_lastInput & Status::NUMBER_EXTEND)
	{
		m_lastInput |= Status::INPUT_NUMBER;
	}
	else
	{
		m_lastInput = Status::INPUT_NUMBER;
	}
}

void Processor::AssignOperator(snum op)
{
	if (op == Inputs::Op_Result)
	{
		if (m_lastInput & Status::INPUT_OPERATOR)
		{
			m_text.pop_back();
			m_operator[prev] = Inputs::Op_None;
		}
		else
		{
			AssignStreamToValue();
		}

		if (m_bIsDecimal)
		{
			m_lastError = ProcessResult(m_fArg);
		}
		else
		{
			m_lastError = ProcessResult(m_iArg);
		}

		if (m_lastError == Error::OPERATION_SUCESSFUL)
		{
			AssignValueToStream();
			m_lastInput = Status::INPUT_NONE;
		}
		else
		{
			switch (m_lastError)
			{
			case Processor::OPERATION_EXCEPTION_DIV_ZERO:
				Clear();
				m_text = k_exceptionMsgDivZero;
				return;
			case Processor::OPERATION_EXCEPTION_DIV_ZERO_INF:
				Clear();
				m_text = k_exceptionMsgDivZeroInf;
				return;
			}
		}
	}
	else if (op == Inputs::Op_Point)
	{
		if (!(m_lastInput & Status::NUMBER_DECIMAL))
		{
			if (m_valueStr.tellp() == 0)
			{
				AssignNumber(Inputs::Number_0);
			}

			m_valueStr << k_op_point;
			m_text += k_op_point;

			// floating for the first time
			if (!m_bIsDecimal && m_argIdx >= OS_SECOND_ARG)
			{
				int arg = m_argIdx;

				while (arg-- > OS_FIRST_ARG)
				{
					m_fArg[arg] = m_iArg[arg];
				}
			}

			m_lastInput |= Status::NUMBER_DECIMAL;
			m_bIsDecimal = true;
		}
	}
	else if (op == Inputs::Op_Inverse)
	{
		if (m_valueStr.tellp() == 0 || atoi(m_valueStr.str().c_str()) == 0)
		{
			return;
		}

		int value;
		m_valueStr >> value;

		ResetStream();

		value *= -1;
		m_valueStr << value;

		stringstream operators;

		if (m_lastInput & Status::NUMBER_INVERSE)
		{
			operators << k_open_parenthesis;

			size_t opPos = m_text.find_last_of(operators.str());
			if (opPos != string::npos)
			{
				m_text.erase(opPos);
			}
			else
			{
				m_text.clear();
			}

			m_text += m_valueStr.str();
			m_lastInput ^= Status::NUMBER_INVERSE;
		}
		else
		{
			operators << k_op_mulplication << k_op_division << k_op_addition << k_op_substraction;
			
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
			m_text += m_valueStr.str();
			m_lastInput |= Status::NUMBER_INVERSE;
		}
	}
	else
	{
		if (m_lastInput & Status::INPUT_OPERATOR)
		{
			m_text.pop_back();
			m_operator[prev] = op;
			m_argIdx--;
		}
		else
		{
			if (m_lastInput & Status::NUMBER_INVERSE)
			{
				m_text += k_close_parenthesis;
			}

			if (m_lastError != Error::OPERATION_SUCESSFUL)
			{
				m_text = m_valueStr.str();
				m_lastError = Error::OPERATION_SUCESSFUL;
			}

			AssignStreamToValue();

			m_operator[cur] = op;

			if (m_argIdx >= OS_MAX_ARG_INDEX)
			{
				if (m_bIsDecimal)
				{
					m_lastError = ProcessResult(m_fArg);
				}
				else
				{
					m_lastError = ProcessResult(m_iArg);
				}

				if (m_lastError == Error::OPERATION_SUCESSFUL)
				{
					AssignValueToStream();
					ResetStream();
				}
				else
				{
					switch (m_lastError)
					{
					case Processor::OPERATION_EXCEPTION_DIV_ZERO:
						Clear();
						m_text = k_exceptionMsgDivZero;
						return;
					case Processor::OPERATION_EXCEPTION_DIV_ZERO_INF:
						Clear();
						m_text = k_exceptionMsgDivZeroInf;
						return;
					}
				}
			}
		}

		switch (op)
		{
		case Inputs::Op_Multiplication:
			m_text += k_op_mulplication;
			break;
		case Inputs::Op_Division:
			m_text += k_op_division;
			break;
		case Inputs::Op_Addition:
			m_text += k_op_addition;
			break;
		case Inputs::Op_Subtraction:
			m_text += k_op_substraction;
			break;
		default:
			break;
		}

		m_lastInput = Status::INPUT_OPERATOR;
		m_argIdx++;
	}
}

void Processor::Clear()
{
	m_argIdx = 0;
	m_lastInput = Status::INPUT_NONE;
	m_bIsDecimal = false;
	
	ResetStream(true);
	m_valueStr << k_number_0;
	m_text = m_valueStr.str();

	for (int i = 0; i < OS_MAX_ARG_COUNT; i++)
	{
		m_iArg[i] = 0;
	}

	for (int i = 0; i < OS_MAX_ARG_COUNT; i++)
	{
		m_operator[i] = Inputs::Op_None;
	}
}

bool Processor::IsHighPriority(snum op)
{
	return op == Inputs::Op_Multiplication
		|| op == Inputs::Op_Division;
}

void Processor::AssignStreamToValue()
{
	if (m_bIsDecimal)
	{
		m_valueStr >> m_fArg[cur];
	}
	else
	{
		m_valueStr >> m_iArg[cur];
	}

	ResetStream();
}

void Processor::AssignValueToStream()
{
	if (m_bIsDecimal)
	{
		m_valueStr << m_fArg[0];
	}
	else
	{
		m_valueStr << m_iArg[0];
	}

	m_text = m_valueStr.str();
}

void Processor::ResetStream(bool clearText)
{
	m_valueStr.str("");
	m_valueStr.clear();

	if (clearText)
	{
		m_text.clear();
	}
}