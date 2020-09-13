#pragma once


struct Variable
{
	void reset()
	{
		nName = "";
		nValue = "";
		nDecimalDigit = 0;
	}

	CString			nName;
	CString			nValue;
	unsigned int	nDecimalDigit;
};

// CVariableRegisterForm dialog

class CVariableRegisterForm : public CDialogEx
{
	DECLARE_DYNAMIC(CVariableRegisterForm)

public:
	CVariableRegisterForm(CWnd* pParent = nullptr);   // standard constructor
	CVariableRegisterForm(const Variable& var, CWnd* pParent = nullptr);
	CVariableRegisterForm(const CString& str, CWnd* pParent = nullptr);
	virtual ~CVariableRegisterForm();

	static Variable StringToVariable(const CString& str);
	static CString	VariableToString(const Variable& var);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLE_REGISTER };
#endif

	const Variable& GetVariable() { return m_variable; }

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	Variable	m_variable;
	CEdit		m_inVarName;
	CEdit		m_inVarValue;
};
