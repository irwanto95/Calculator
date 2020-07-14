#include "pch.h"
#include "Processor.h"

#define prev	m_argIdx - 1
#define cur		m_argIdx

template<typename _Ty>
void ShiftLeft(_Ty* data, int start, int size, _Ty default);

Processor::Processor()
	: m_lastError(0)
{
	Clear();
}

Processor::~Processor()
{
}

void Processor::AssignNumber(unum num)
{
	if (num == 0 
		&& m_valueStr.tellp() > 0
		&& atoi(m_valueStr.str().c_str()) == 0)
	{
		return;
	}

	if (m_bNeedToInputFirstNumber)
	{
		ResetStream();
		m_text.clear();
		m_bNeedToInputFirstNumber = false;
		m_lastError = Error::OPERATION_SUCESSFUL;
	}

	m_valueStr << num;
	m_text += to_string(num);

	m_bLastInputIsOperator = false;
}

void Processor::AssignOperator(snum op)
{
	if (op == Inputs::Op_Result)
	{
		if (m_bLastInputIsOperator)
		{
			m_text.pop_back();
			m_operator[prev] = Inputs::Op_None;
		}
		else
		{
			AssignStreamToValue();
		}

		m_lastError = ProcessResult();

		if (m_lastError == Error::OPERATION_SUCESSFUL)
		{
			AssignValueToStream();

			m_bLastInputIsOperator = false;
			m_bNeedToInputFirstNumber = true;
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
			default:
				return;
			}
		}
	}
	else if (op == Inputs::Op_Point)
	{
		if (!m_bIsFloatingNumber)
		{
			m_valueStr << k_op_point;
			m_bIsFloatingNumber = true;
		}
	}
	else if (op == Inputs::Op_Inverse)
	{
		if (m_bIsFloatingNumber)
		{
			m_fArg[prev] *= -1.f;
		}
		else
		{
			m_iArg[prev] *= -1;
		}
	}
	else
	{
		m_bNeedToInputFirstNumber = false;

		if (m_bLastInputIsOperator)
		{
			m_text.pop_back();
			m_operator[prev] = op;
			m_argIdx--;
		}
		else
		{
			if (m_lastError != Error::OPERATION_SUCESSFUL)
			{
				m_text = m_valueStr.str();
				m_lastError = Error::OPERATION_SUCESSFUL;
			}

			AssignStreamToValue();

			m_operator[cur] = op;

			if (m_argIdx >= OS_MAX_ARG_INDEX)
			{
				m_lastError = ProcessResult();

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
					default:
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
		case Inputs::Op_Substraction:
			m_text += k_op_substraction;
			break;
		default:
			break;
		}

		m_bLastInputIsOperator = true;
		m_argIdx++;
	}
}

void Processor::Clear()
{
	m_argIdx = 0;
	
	ResetStream(true);
	m_text = m_valueStr.str();

	for (int i = 0; i < OS_MAX_ARG_COUNT; i++)
	{
		m_iArg[i] = 0;
	}

	for (int i = 0; i < OS_MAX_ARG_COUNT; i++)
	{
		m_operator[i] = Inputs::Op_None;
	}

	m_bIsFloatingNumber = false;
	m_bLastInputIsOperator = false;
	m_bNeedToInputFirstNumber = true;
}

bool Processor::IsHighPriority(snum op)
{
	return op == Inputs::Op_Multiplication
		|| op == Inputs::Op_Division;
}

int Processor::ProcessResult()
{
	int leftArg, rightArg;

	if (m_bIsFloatingNumber)
	{
		while (m_argIdx >= OS_SECOND_ARG)
		{
			if (!IsHighPriority(m_operator[OS_SECOND_ARG]) || IsHighPriority(m_operator[OS_FIRST_ARG]) || m_argIdx == OS_SECOND_ARG)
			{
				leftArg = OS_FIRST_ARG;
				rightArg = OS_SECOND_ARG;
			}
			else
			{
				leftArg = OS_SECOND_ARG;
				rightArg = OS_THIRD_ARG;
			}

			try
			{
				switch (m_operator[leftArg])
				{
				case Inputs::Op_Multiplication:
					m_fArg[leftArg] *= m_fArg[rightArg];
					break;
				case Inputs::Op_Division:
					if (m_fArg[rightArg] == 0)
						throw m_fArg[leftArg] == 0 ? Error::OPERATION_EXCEPTION_DIV_ZERO_INF : Error::OPERATION_EXCEPTION_DIV_ZERO;

					m_fArg[leftArg] /= m_fArg[rightArg];
					break;
				case Inputs::Op_Addition:
					m_fArg[leftArg] += m_fArg[rightArg];
					break;
				case Inputs::Op_Substraction:
					m_fArg[leftArg] -= m_fArg[rightArg];
					break;
				default:
					break;
				}
			}
			catch (Error e)
			{
				return e;
			}

			ShiftLeft(m_fArg, rightArg, OS_MAX_ARG_COUNT, 0.f);
			ShiftLeft(m_operator, leftArg, OS_MAX_ARG_COUNT, (snum)Inputs::Op_None);

			m_argIdx--;
		}
	}
	else
	{
		while (m_argIdx >= OS_SECOND_ARG)
		{
			if (!IsHighPriority(m_operator[OS_SECOND_ARG]) || IsHighPriority(m_operator[OS_FIRST_ARG]) || m_argIdx == OS_SECOND_ARG)
			{
				leftArg = OS_FIRST_ARG;
				rightArg = OS_SECOND_ARG;
			}
			else
			{
				leftArg = OS_SECOND_ARG;
				rightArg = OS_THIRD_ARG;
			}

			try
			{
				switch (m_operator[leftArg])
				{
				case Inputs::Op_Multiplication:
					m_iArg[leftArg] *= m_iArg[rightArg];
					break;
				case Inputs::Op_Division:
					if (m_iArg[rightArg] == 0)
						throw m_fArg[leftArg] == 0 ? Error::OPERATION_EXCEPTION_DIV_ZERO_INF : Error::OPERATION_EXCEPTION_DIV_ZERO;

					m_iArg[leftArg] /= m_iArg[rightArg];
					break;
				case Inputs::Op_Addition:
					m_iArg[leftArg] += m_iArg[rightArg];
					break;
				case Inputs::Op_Substraction:
					m_iArg[leftArg] -= m_iArg[rightArg];
					break;
				default:
					break;
				}
			}
			catch (Error e)
			{
				return e;
			}

			ShiftLeft(m_iArg, rightArg, OS_MAX_ARG_COUNT, 0);
			ShiftLeft(m_operator, leftArg, OS_MAX_ARG_COUNT, (snum)Inputs::Op_None);

			m_argIdx--;
		}
	}

	return Error::OPERATION_SUCESSFUL;
}

void Processor::AssignStreamToValue()
{
	if (m_bIsFloatingNumber)
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
	if (m_bIsFloatingNumber)
	{
		m_valueStr << m_fArg[0];
	}
	else
	{
		m_valueStr << m_iArg[0];
	}

	m_text = m_valueStr.str();
}

void Processor::ResetStream(bool addDefaultValue)
{
	m_valueStr.str("");
	m_valueStr.clear();

	if (addDefaultValue)
	{
		m_valueStr << k_number_0;
	}
}


template<typename _Ty>
void ShiftLeft(_Ty* data, int start, int size, _Ty default)
{
	for (int i = start; i < size - 1; i++)
	{
		data[i] = data[i + 1];
	}

	data[size - 1] = default;
}