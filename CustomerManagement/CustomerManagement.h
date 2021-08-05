
// CustomerManagement.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "afxcmn.h"
#include "resource.h"		// main symbols


// CustomerManagement:
// See CustomerManagement.cpp for the implementation of this class
//

class CustomerManagement : public CWinApp
{
public:
	CustomerManagement();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CustomerManagement theApp;
