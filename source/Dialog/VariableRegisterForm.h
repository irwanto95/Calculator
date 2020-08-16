#pragma once


struct Variable
{
	void reset()
	{
		nName = "";
		nValue = "";
		nIsDecimal = false;
	}

	CString nName;
	CString	nValue;
	bool	nIsDecimal;
};

// CVariableRegisterForm dialog

class CVariableRegisterForm : public CDialogEx
{
	DECLARE_DYNAMIC(CVariableRegisterForm)

public:
	CVariableRegisterForm(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVariableRegisterForm();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLE_REGISTER };
#endif

	const Variable& GetVariable() { return m_variable; }

	afx_msg void OnBnClickedOk();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	bool ValidateValue();

private:
	Variable	m_variable;
	CEdit		m_inVarName;
	CEdit		m_inVarValue;
};
