// D:\Projects\Apps_Win\Calculator\trunk\source\Dialog\VariableRegisterForm.cpp : implementation file
//

#include "pch.h"
#include "VariableRegisterForm.h"
#include "afxdialogex.h"


// CVariableRegisterForm dialog

IMPLEMENT_DYNAMIC(CVariableRegisterForm, CDialogEx)

CVariableRegisterForm::CVariableRegisterForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VARIABLE_REGISTER, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}

CVariableRegisterForm::CVariableRegisterForm(const Variable& var, CWnd* pParent)
	: CVariableRegisterForm(pParent)
{
	m_variable = var;
}

CVariableRegisterForm::CVariableRegisterForm(const CString& str, CWnd* pParent)
	: CVariableRegisterForm(pParent)
{
	m_variable = StringToVariable(str);
}

CVariableRegisterForm::~CVariableRegisterForm()
{
}

Variable CVariableRegisterForm::StringToVariable(const CString& str)
{
	Variable result;

	si16 nameSeparator = str.Find(_T(':'));
	if (nameSeparator > -1)
	{
		result.nName = str.Left(nameSeparator);
		
		si16 digitSeparator = str.Find(_T(':'), ++nameSeparator);
		if (digitSeparator > -1)
		{
			string strDig = CW2A(str.Mid(nameSeparator, digitSeparator - nameSeparator));
			result.nDecimalDigit = atoi(strDig.c_str());
			result.nValue = str.Right(str.GetLength() - ++digitSeparator);
		}
		else
		{
			MF_ASSERT(digitSeparator > -1);
		}
	}
	else
	{
		MF_ASSERT(nameSeparator > -1);
	}

	return result;
}

CString CVariableRegisterForm::VariableToString(const Variable& var)
{
	CString result;
	result.Append(var.nName);
	result.Append(CString(":"));
	result.Append(utils::MFCUtils::ToLPCTSTR(std::to_string(var.nDecimalDigit)));
	result.Append(CString(":"));
	result.Append(var.nValue);

	return result;
}

BOOL CVariableRegisterForm::OnInitDialog()
{
	CString str;
	MF_ASSERT(str.LoadString(IDS_STR_VAR_NAME));
	GetDlgItem(IDC_TXT_VAR_NAME)->SetWindowTextW(str);

	MF_ASSERT(str.LoadString(IDS_STR_VAR_VALUE));
	GetDlgItem(IDC_TXT_VAR_VALUE)->SetWindowTextW(str);

	GetDlgItem(IDC_INPUT_VAR_NAME)->SetWindowTextW(m_variable.nName);
	GetDlgItem(IDC_INPUT_VAR_VALUE)->SetWindowTextW(m_variable.nValue);

	return CDialogEx::OnInitDialog();
}

void CVariableRegisterForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_VAR_NAME, m_inVarName);
	DDX_Control(pDX, IDC_INPUT_VAR_VALUE, m_inVarValue);
}


BEGIN_MESSAGE_MAP(CVariableRegisterForm, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVariableRegisterForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVariableRegisterForm::OnBnClickedCancel)
END_MESSAGE_MAP()


// CVariableRegisterForm message handlers


void CVariableRegisterForm::OnBnClickedOk()
{
	m_variable.reset();

	m_inVarName.GetWindowTextW(m_variable.nName);
	m_inVarValue.GetWindowTextW(m_variable.nValue);

	std::string _strName = CW2A(m_variable.nName.GetString());
	std::string _strVal = CW2A(m_variable.nValue.GetString());

	bool hasWhiteSpaceInFrontOrBack = !_strName.empty();
	while (hasWhiteSpaceInFrontOrBack)
	{
		hasWhiteSpaceInFrontOrBack = false;

		if (_strName[0] == ' ')
		{
			_strName.erase(0, 1);
			hasWhiteSpaceInFrontOrBack = true;
		}

		if (_strName.length() > 1 && _strName.back() == ' ')
		{
			_strName.pop_back();
			hasWhiteSpaceInFrontOrBack = true;
		}
	}

	m_variable.nName = utils::MFCUtils::ToLPCTSTR(_strName);

	bool isValid = !_strName.empty()
		&& !_strVal.empty()
		&& (_strVal.find_first_not_of("-.0123456789") == std::string::npos);
	
	si8 repetitions = 2;

	for (si8 i = 0; isValid && i < repetitions; i++)
	{
		switch (i)
		{
		case 0:
		{
			int minusIdx = _strVal.find(k_op_subtraction);
			isValid = (minusIdx <= 0);
			if (isValid)
			{
				// looking for the secont sign
				isValid = _strVal.find(k_op_subtraction, minusIdx + 1) == string::npos;
			}

			break;
		}
		case 1:
		{
			int pointIdx = _strVal.find(k_op_point);
			if (pointIdx > -1)
			{
				// looking for the secont point
				isValid = _strVal.find(k_op_point, ++pointIdx) == string::npos;
				if (isValid)
				{
					m_variable.nDecimalDigit = _strVal.size() - pointIdx;
				}
			}
			else
			{
				m_variable.nDecimalDigit = 0;
			}

			break;
		}
		default:
			break;
		}
	}

	if (isValid)
	{
		CDialogEx::OnOK();
	}
	else
	{
		m_inVarName.SetWindowTextW(L"");
		m_inVarValue.SetWindowTextW(L"");

		// show error popup
		CString msg;
		MF_ASSERT(msg.LoadString(IDS_STR_INVALID_VARIABLE));
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
}

void CVariableRegisterForm::OnBnClickedCancel()
{
	m_variable.reset();

	CDialogEx::OnCancel();
}
