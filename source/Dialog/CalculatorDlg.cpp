
// CalculatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "VariableRegisterForm.h"

#include <fstream>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHAR_BUFFER_MAX 2048

static const char* s_variableListFileName = "variable.bin";

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalculatorDlg dialog



CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALCULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_outputText = m_processor.GetTextC();
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OUTPUT, m_outputText);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_listVariable);
}

BOOL CCalculatorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccel)
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		{
			return(TRUE);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NUM_1, &CCalculatorDlg::OnBnClickedNum1)
	ON_BN_CLICKED(IDC_NUM_2, &CCalculatorDlg::OnBnClickedNum2)
	ON_BN_CLICKED(IDC_NUM_3, &CCalculatorDlg::OnBnClickedNum3)
	ON_BN_CLICKED(IDC_NUM_4, &CCalculatorDlg::OnBnClickedNum4)
	ON_BN_CLICKED(IDC_NUM_5, &CCalculatorDlg::OnBnClickedNum5)
	ON_BN_CLICKED(IDC_NUM_6, &CCalculatorDlg::OnBnClickedNum6)
	ON_BN_CLICKED(IDC_NUM_7, &CCalculatorDlg::OnBnClickedNum7)
	ON_BN_CLICKED(IDC_NUM_8, &CCalculatorDlg::OnBnClickedNum8)
	ON_BN_CLICKED(IDC_NUM_9, &CCalculatorDlg::OnBnClickedNum9)
	ON_BN_CLICKED(IDC_NUM_0, &CCalculatorDlg::OnBnClickedNum0)
	ON_BN_CLICKED(IDC_NUM_000, &CCalculatorDlg::OnBnClickedNum000)
	ON_BN_CLICKED(IDC_POINT, &CCalculatorDlg::OnBnClickedPoint)
	ON_BN_CLICKED(IDC_SIGN, &CCalculatorDlg::OnBnClickedSign)
	ON_BN_CLICKED(IDC_CLEAR, &CCalculatorDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_DELETE, &CCalculatorDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_MULTIPLICATION, &CCalculatorDlg::OnBnClickedMultiplication)
	ON_BN_CLICKED(IDC_DIVISION, &CCalculatorDlg::OnBnClickedDivision)
	ON_BN_CLICKED(IDC_ADDITION, &CCalculatorDlg::OnBnClickedAddition)
	ON_BN_CLICKED(IDC_SUBTRACTION, &CCalculatorDlg::OnBnClickedSubtraction)
	ON_BN_CLICKED(IDC_RESULT, &CCalculatorDlg::OnBnClickedResult)
	ON_BN_CLICKED(IDC_BTN_VAR_ADD, &CCalculatorDlg::OnBnClickedBtnVarAdd)
	ON_BN_CLICKED(IDC_BTN_VAR_DELETE, &CCalculatorDlg::OnBnClickedBtnVarDelete)
	ON_BN_CLICKED(IDC_BTN_VAR_EDIT, &CCalculatorDlg::OnBnClickedBtnVarEdit)
	ON_LBN_SELCHANGE(IDC_LIST_VARIABLE, &CCalculatorDlg::OnLbnSelchangeListVariable)
	ON_LBN_DBLCLK(IDC_LIST_VARIABLE, &CCalculatorDlg::OnLbnDblclkListVariable)
END_MESSAGE_MAP()


// CCalculatorDlg message handlers

BOOL CCalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	CString appTitle, tmp;
	ASSERT(tmp.LoadString(IDS_APP_NAME));
	
	appTitle.Append(tmp);
	appTitle.Append(L" - ver ");
	ASSERT(tmp.LoadString(IDS_APP_VERSION));
	
	appTitle.Append(tmp);
	SetWindowText(appTitle);

	LoadVariable();
	UpdateButton();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCalculatorDlg::SaveVariable()
{
	fstream file;
	file.open(s_variableListFileName, fstream::out | fstream::trunc | fstream::binary);

	if (file.is_open())
	{
		CString _str;
		string _sstr;

		char* buffer;
		int vcount = m_listVariable.GetCount();
		int sz, pos = 0;

		buffer = new char[CHAR_BUFFER_MAX];
		
		sz = sizeof(int);
		memcpy(&buffer[pos], &vcount, sz);
		pos += sz;

		for (usize i = 0; i < vcount; i++)
		{
			m_listVariable.GetText(i, _str);
			_sstr = CW2A(_str.GetString());
			sz = _str.GetLength();

			memcpy(&buffer[pos], &sz, sizeof(int));
			pos += sizeof(int);

			memcpy(&buffer[pos], _sstr.c_str(), sz);
			pos += sz;
		}

		file.write(buffer, pos);

		delete[] buffer;
	}

	file.close();
}

void CCalculatorDlg::LoadVariable()
{
	fstream file;
	file.open(s_variableListFileName, fstream::in | fstream::binary);

	if (file.is_open())
	{
		file.seekg(0, file.end);
		usize buffsize = file.tellg();
		file.seekg(0, file.beg);

		char* buffer = new char[buffsize];

		int vcount, sz, pos = 0;

		file.read(buffer, buffsize);

		sz = sizeof(int);
		memcpy(&vcount, &buffer[pos], sz);
		pos += sz;

		for (usize i = 0; i < vcount; i++)
		{
			memcpy(&sz, &buffer[pos], sizeof(int));
			pos += sizeof(int);

			string _sstr;
			_sstr.resize(sz);

			memcpy(&_sstr[0], &buffer[pos], sz);
			pos += sz;

			m_listVariable.AddString(utils::MFCUtils::ToLPCTSTR(_sstr));
		}

		delete[] buffer;
	}

	file.close();
}

void CCalculatorDlg::UpdateButton()
{
	if (m_listVariable.GetCurSel() != LB_ERR)
	{
		GetDlgItem(IDC_BTN_VAR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_VAR_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_VAR_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_VAR_DELETE)->EnableWindow(FALSE);
	}
}

void CCalculatorDlg::OnBnClickedNum1()
{
	m_processor.AssignNumber(Inputs::Number_1);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum2()
{
	m_processor.AssignNumber(Inputs::Number_2);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum3()
{
	m_processor.AssignNumber(Inputs::Number_3);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum4()
{
	m_processor.AssignNumber(Inputs::Number_4);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum5()
{
	m_processor.AssignNumber(Inputs::Number_5);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum6()
{
	m_processor.AssignNumber(Inputs::Number_6);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum7()
{
	m_processor.AssignNumber(Inputs::Number_7);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum8()
{
	m_processor.AssignNumber(Inputs::Number_8);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum9()
{
	m_processor.AssignNumber(Inputs::Number_9);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum0()
{
	m_processor.AssignNumber(Inputs::Number_0);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedNum000()
{
	m_processor.AssignNumber(Inputs::Number_0);
	m_processor.AssignNumber(Inputs::Number_0);
	m_processor.AssignNumber(Inputs::Number_0);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedPoint()
{
	m_processor.AssignOperator(Inputs::Op_Point);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedSign()
{
	m_processor.AssignOperator(Inputs::Op_Inverse);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedClear()
{
	m_processor.Clear();
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedDelete()
{
	m_processor.EraseBack();
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedMultiplication()
{
	m_processor.AssignOperator(Inputs::Op_Multiplication);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedDivision()
{
	m_processor.AssignOperator(Inputs::Op_Division);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedAddition()
{
	m_processor.AssignOperator(Inputs::Op_Addition);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedSubtraction()
{
	m_processor.AssignOperator(Inputs::Op_Subtraction);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedResult()
{
	m_processor.AssignOperator(Inputs::Op_Result);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}


void CCalculatorDlg::OnBnClickedBtnVarAdd()
{
	CVariableRegisterForm varReg;
	varReg.DoModal();

	const Variable& var = varReg.GetVariable();

	if (!var.nName.IsEmpty())
	{
		m_listVariable.AddString(CVariableRegisterForm::VariableToString(varReg.GetVariable()));

		SaveVariable();
		UpdateButton();
	}
}


void CCalculatorDlg::OnBnClickedBtnVarDelete()
{
	si16 idx = m_listVariable.GetCurSel();
	if (idx < 0)
		return;

	m_listVariable.DeleteString(idx);

	if (idx < m_listVariable.GetCount())
	{
		m_listVariable.SetCurSel(idx);
	}
	else if (m_listVariable.GetCount() > 0)
	{
		m_listVariable.SetCurSel(idx - 1);
	}

	SaveVariable();
	UpdateButton();
}


void CCalculatorDlg::OnBnClickedBtnVarEdit()
{
	si16 idx = m_listVariable.GetCurSel();
	if (idx < 0)
		return;

	CString selectedStr;
	m_listVariable.GetText(idx, selectedStr);

	CVariableRegisterForm varReg(selectedStr);
	varReg.DoModal();

	const Variable& var = varReg.GetVariable();
	if (!var.nName.IsEmpty())
	{
		m_listVariable.DeleteString(idx);
		m_listVariable.InsertString(idx, CVariableRegisterForm::VariableToString(varReg.GetVariable()));
		m_listVariable.SetCurSel(idx);

		SaveVariable();
		UpdateButton();
	}
}


void CCalculatorDlg::OnLbnSelchangeListVariable()
{
	UpdateButton();
}


void CCalculatorDlg::OnLbnDblclkListVariable()
{
	si16 idx = m_listVariable.GetCurSel();
	if (idx < 0)
		return;

	CString selectedStr;
	m_listVariable.GetText(idx, selectedStr);

	m_processor.AssignOperator(Inputs::Op_Addition);
	
	Variable var = CVariableRegisterForm::StringToVariable(selectedStr);
	std::string strVal = CW2A(var.nValue.GetString());

	if (var.nDecimalDigit > 0)
	{
		float v = atof(strVal.c_str());
		m_processor.AssignValue(v, var.nDecimalDigit);
	}
	else
	{
		int v = atoi(strVal.c_str());
		m_processor.AssignValue(v);
	}
	
	m_processor.AssignOperator(Inputs::Op_Addition);
	m_outputText = m_processor.GetTextC();
	UpdateData(FALSE);
}
