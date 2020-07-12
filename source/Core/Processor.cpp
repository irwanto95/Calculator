#include "pch.h"
#include "Processor.h"

template<typename _Ty>
void ShiftLeft(_Ty* data, int start, int size, _Ty default);

Processor::Processor()
{
	Clear();
}

Processor::~Processor()
{
}

void Processor::AssignNumber(unum num)
{
	char firstArg;
	m_text >> firstArg;

	if (firstArg == k_number_0)
	{
		m_text.str("");
	}

	m_text << num;
}

void Processor::AssignOperator(snum op)
{
	if (op == Inputs::Op_Result)
	{
		ProcessResult();

		m_text.str("");

		if (m_bIsFloatingNumber)
		{
			m_text << m_fArg[0];
		}
		else
		{
			m_text << m_iArg[0];
		}

		m_bLastInputIsOperator = false;
	}
	else if (op == Inputs::Op_Point)
	{
		if (!m_bIsFloatingNumber)
		{
			m_text << k_op_point;
			m_bIsFloatingNumber = true;
		}
	}
	else if (op == Inputs::Op_Inverse)
	{
	}
	else
	{
		if (m_bLastInputIsOperator)
		{
			std::string _s = m_text.str();
			_s.pop_back();

			m_text.str("");
			m_text << _s;
		}

		if (m_bIsFloatingNumber)
		{
			m_text >> m_fArg[m_argIdx];
		}
		else
		{
			m_text >> m_iArg[m_argIdx];
		}

		m_operator[m_argIdx] = op;

		if (m_argIdx >= OS_MAX_ARG_COUNT)
		{
			AssignOperator(Inputs::Op_Result);
		}

		switch (op)
		{
		case Inputs::Op_Multiplication:
			m_text << k_op_mulplication;
			break;
		case Inputs::Op_Division:
			m_text << k_op_division;
			break;
		case Inputs::Op_Addition:
			m_text << k_op_addition;
			break;
		case Inputs::Op_Substraction:
			m_text << k_op_substraction;
			break;
		default:
			break;
		}

		char unused;
		m_text >> unused;

		m_argIdx++;

		m_bLastInputIsOperator = true;
	}
}

void Processor::Clear()
{
	m_argIdx = 0;
	m_text.str("");
	m_text << k_number_0;

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
}

bool Processor::IsHighPriority(snum op)
{
	return op == Inputs::Op_Multiplication
		|| op == Inputs::Op_Division;
}

void Processor::ProcessResult()
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

			switch (m_operator[leftArg])
			{
			case Inputs::Op_Multiplication:
				m_fArg[leftArg] *= m_fArg[rightArg];
				break;
			case Inputs::Op_Division:
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

			switch (m_operator[leftArg])
			{
			case Inputs::Op_Multiplication:
				m_iArg[leftArg] *= m_iArg[rightArg];
				break;
			case Inputs::Op_Division:
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

			ShiftLeft(m_iArg, rightArg, OS_MAX_ARG_COUNT, 0);
			ShiftLeft(m_operator, leftArg, OS_MAX_ARG_COUNT, (snum)Inputs::Op_None);

			m_argIdx--;
		}
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