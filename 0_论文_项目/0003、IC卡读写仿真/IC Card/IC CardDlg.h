// IC CardDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_ICCARDDLG_H__EDAACB4C_2A35_4D13_8C54_2B1D8EA3187F__INCLUDED_)
#define AFX_ICCARDDLG_H__EDAACB4C_2A35_4D13_8C54_2B1D8EA3187F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CICCardDlg dialog

class CICCardDlg : public CDialog
{
// Construction
public:
	char Num2Char(unsigned char ch);
	BYTE rxdata[100];
	void DealWithData(void);
	CString m_getstr;
	char ConvertHexChar(char ch);
	BOOL m_bOpenCom;
	CICCardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CICCardDlg)
	enum { IDD = IDD_ICCARD_DIALOG };
	CStatic	m_LED;
	CButton	m_OpenCom;
	CComboBox	m_ComNum;
	CMSComm	m_comm;
	CString	m_id;
	float	m_money;
	float	m_money2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CICCardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HICON m_hIcon1;
	HICON m_hIcon2;

	// Generated message map functions
	//{{AFX_MSG(CICCardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnCommMscomm();
	afx_msg void OnOpencom();
	afx_msg void OnWr();
	afx_msg void OnRd();
	afx_msg void OnAdd();
	afx_msg void OnDec();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICCARDDLG_H__EDAACB4C_2A35_4D13_8C54_2B1D8EA3187F__INCLUDED_)
