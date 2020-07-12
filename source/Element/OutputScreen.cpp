#include "pch.h"
#include "OutputScreen.h"

#include "InputFields.h"

template <typename _Ty>
void ShiftLeft(_Ty* array, int start, int size, _Ty default);

OutputScreen::OutputScreen()
	: m_text("")
	, m_bIsFloatingNumber(false)
{
	Clear();
}

OutputScreen::~OutputScreen()
{
	Clear();
}

void OutputScreen::Clear()
{
	for (int i = 0; i < OS_STORED_ARG_COUNT; i++)
	{
		m_iValue[i] = 0;
		m_operator[i] = InputFields::Op_None;
	}

	m_argIndex = OS_FIRST_ARG;
	m_orde = 0;
	m_text = k_number_0;
	m_bIsFloatingNumber = false;
}

void OutputScreen::AssignNumber(uint8_t _num)
{
	if (m_orde > UCHAR_MAX)
	{
		return;
	}
	
	if (!m_bIsFloatingNumber)
	{
		m_iValue[m_argIndex] *= pow(10, m_orde++);
		m_iValue[m_argIndex] += _num;
	}
	else
	{
	}

	if (m_text == k_number_0)
	{
		m_text = *std::to_string(_num).c_str();
	}
	else
	{
		m_text += *std::to_string(_num).c_str();
	}
}

void OutputScreen::AssignOperator(uint8_t _op)
{
	if (_op == InputFields::Op_Inverse)
	{
	}
	else if (_op == InputFields::Op_Point)
	{
		m_bIsFloatingNumber = true;
	}
	else if (_op == InputFields::Op_Result)
	{
		while (m_argIndex > OS_FIRST_ARG)
		{
			ProcessResult(OS_FIRST_ARG);
		}

		if (m_bIsFloatingNumber)
		{
			m_text = std::to_string(m_fValue[OS_FIRST_ARG]).c_str();
		}
		else
		{
			m_text = std::to_string(m_iValue[OS_FIRST_ARG]).c_str();
		}
	}
	else
	{
		m_operator[m_argIndex] = _op;
		
		if (m_argIndex > OS_FIRST_ARG)
		{
			if (!IsNeedPriority(_op) || IsNeedPriority(m_operator[m_argIndex - 1]))
			{
				ProcessResult(m_argIndex - 1);
			}
		}		

		switch (_op)
		{
		case InputFields::Op_Multiplication:
			m_text += k_sign_mulplication;
			break;
		case InputFields::Op_Division:
			m_text += k_sign_division;
			break;
		case InputFields::Op_Addition:
			m_text += k_sign_addition;
			break;
		case InputFields::Op_Substraction:
			m_text += k_sign_substraction;
			break;
		}

		m_argIndex++;
		m_orde = 0;
	}
 }

void OutputScreen::ProcessResult(uint8_t leftArg)
{
	int rightArg = leftArg + 1;

	if (!m_bIsFloatingNumber)
	{
		switch (m_operator[leftArg])
		{
		case InputFields::Op_Multiplication:
			m_iValue[leftArg] *= m_iValue[rightArg];
			break;
		case InputFields::Op_Division:
			m_iValue[leftArg] /= m_iValue[rightArg];
			break;
		case InputFields::Op_Addition:
			m_iValue[leftArg] += m_iValue[rightArg];
			break;
		case InputFields::Op_Substraction:
			m_iValue[leftArg] -= m_iValue[rightArg];
			break;
		default:
			break;
		}

		ShiftLeft(m_iValue, rightArg, OS_STORED_ARG_COUNT, 0);
		ShiftLeft(m_operator, (int)leftArg, OS_STORED_ARG_COUNT, (uint8_t)InputFields::Op_None);
	}
	else
	{
	}

	m_argIndex--;
}

inline bool OutputScreen::IsNeedPriority(uint8_t op)
{
	return op == InputFields::Op_Multiplication 
		|| op == InputFields::Op_Division;
}


template <typename _Ty>
void ShiftLeft(_Ty* array, int start, int size, _Ty default)
{
	for (int i = start; i < size - 1; i++)
	{
		array[i] = array[i + 1];
	}

	array[size - 1] = default;
}