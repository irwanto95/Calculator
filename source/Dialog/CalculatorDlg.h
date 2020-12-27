#pragma once

#include "Core/Processor.h"

struct Variable;

// CCalculatorDlg dialog
class CCalculatorDlg : public CDialogEx
{
// Construction
public:
	CCalculatorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCULATOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedNum1();
	afx_msg void OnBnClickedNum2();
	afx_msg void OnBnClickedNum3();
	afx_msg void OnBnClickedNum4();
	afx_msg void OnBnClickedNum5();
	afx_msg void OnBnClickedNum6();
	afx_msg void OnBnClickedNum7();
	afx_msg void OnBnClickedNum8();
	afx_msg void OnBnClickedNum9();
	afx_msg void OnBnClickedNum0();
	afx_msg void OnBnClickedNum000();
	afx_msg void OnBnClickedPoint();
	afx_msg void OnBnClickedSign();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedMultiplication();
	afx_msg void OnBnClickedDivision();
	afx_msg void OnBnClickedAddition();
	afx_msg void OnBnClickedSubtraction();
	afx_msg void OnBnClickedResult();
	afx_msg void OnBnClickedBtnVarAdd();
	afx_msg void OnBnClickedBtnVarDelete();
	afx_msg void OnBnClickedBtnVarEdit();
	afx_msg void OnLbnDblclkListVariable();

// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	static void	OnProcessorCallback(int type, void * data, void * caller);
	
	void	OnInitialize();
	void	OnUpdate();

	// Generated message map functions
	DECLARE_MESSAGE_MAP()

private:
	void	InitButton();
	void	SaveVariable();
	void	LoadVariable();
	void	UpdateVariable();


protected:
	HICON	m_hIcon;
	HACCEL	m_hAccel;

private:
	Processor	m_processor;
	CListBox	m_listVariable;
	CListBox	m_listHistory;
	CString		m_outputText;
};
