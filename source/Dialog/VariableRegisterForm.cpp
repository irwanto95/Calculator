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

CVariableRegisterForm::~CVariableRegisterForm()
{
}

BOOL CVariableRegisterForm::OnInitDialog()
{
	CString str;
	MF_ASSERT(str.LoadString(IDS_STR_VAR_NAME));
	GetDlgItem(IDC_TXT_VAR_NAME)->SetWindowTextW(str);

	MF_ASSERT(str.LoadString(IDS_STR_VAR_VALUE));
	GetDlgItem(IDC_TXT_VAR_VALUE)->SetWindowTextW(str);

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
END_MESSAGE_MAP()


// CVariableRegisterForm message handlers


void CVariableRegisterForm::OnBnClickedOk()
{
	m_variable.reset();

	m_inVarName.GetWindowTextW(m_variable.nName);
	m_inVarValue.GetWindowTextW(m_variable.nValue);

	std::string _str = CW2A(m_variable.nValue.GetString());

	bool isValid = (m_variable.nName != "")
		&& (m_variable.nValue != "")
		&& (_str.find_first_not_of(".0123456789") == std::string::npos);
	
	if (isValid)
	{
		int pointIdx = _str.find('.');
		if (pointIdx > -1)
		{
			// looking for the secont point
			isValid = _str.find('.', pointIdx + 1) == string::npos;
			if (isValid)
			{
				m_variable.nIsDecimal = true;
			}
		}
		else
		{
			m_variable.nIsDecimal = false;
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