#include "pch.h"
#include "History.h"
#include "Core/Inputs.h"


History::History()
{
}

History::~History()
{
}

void History::Clear()
{
	m_board.ResetContent();
}

void History::Add(const string & text)
{
	m_board.AddString(MFCUtils::ToLPCTSTR(text));
}

void History::Add(LPCTSTR wtext)
{
	m_board.AddString(wtext);
}

void History::SetUsage(si64 idc_idx)
{
	switch (idc_idx)
	{
	case IDC_HISTORY_USE_ADD:		m_usage = EUsage::HISTORY_ADD; break;
	case IDC_HISTORY_USE_REPLACE:	m_usage = EUsage::HISTORY_REPLACE; break;
	default:						m_usage = EUsage::HISTORY_UNKNOWN; break;
	}
}

ArgumentPacks History::GetSelected()
{
	si16 idx = m_board.GetCurSel();
	if (idx < 0)
		return ArgumentPacks();

	return Get(idx);
}

ArgumentPacks History::Get(ui16 idx)
{
	ArgumentPacks v;
	
	CString selectedStr;
	m_board.GetText(idx, selectedStr);

	string _substr, _strVal = CW2A(selectedStr);
	int operatorIdx{ 0 }, findOff{ 0 }, findIdx{ 0 }, decimal{ 0 };
	bool isNegative;

	while (findIdx < _strVal.length())
	{
		isNegative = _strVal[findIdx] == k_open_parenthesis;
		if (isNegative)
		{
			findOff = _strVal.find(k_close_parenthesis, findIdx);
			operatorIdx = _strVal.find_first_of(k_op_math_operation, findOff);
			findIdx++;
		}
		else
		{
			operatorIdx = findOff = _strVal.find_first_of(k_op_math_operation, findIdx);
		}

		_substr = _strVal.substr(findIdx, findOff - findIdx);
		decimal = _substr.find(k_op_point);
		if (decimal != string::npos)
		{
			decimal = _substr.size() - decimal - 1;
		}
		
		v.push_back(make_tuple(false, _substr, decimal != string::npos, decimal));
		if (operatorIdx != string::npos)
		{
			_substr = _strVal[operatorIdx];
			v.push_back(make_tuple(true, _substr, false, 0));
		}
		else
		{
			break;
		}

		findIdx = isNegative ? findOff + 2 : findOff + 1;
	}

	return v;
}
