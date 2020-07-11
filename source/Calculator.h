
// Calculator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CalculatorApp:
// See Calculator.cpp for the implementation of this class
//

class CalculatorApp : public CWinApp
{
public:
	CalculatorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CalculatorApp theApp;
