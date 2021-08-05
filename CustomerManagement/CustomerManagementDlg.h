
// CustomerManagementDlg.h : header file
//

#pragma once
#include "tinyxml2.h"
#include "XMLParse.h"

// CCustomerManagementDlg dialog
class CCustomerManagementDlg : public CDialog
{
// Construction
public:
	CCustomerManagementDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMERMANAGEMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int newfile = 0;
	XMLParse xmlParse;
	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSave();
};
