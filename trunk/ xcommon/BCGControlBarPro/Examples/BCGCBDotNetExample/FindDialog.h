#if !defined(AFX_FINDDIALOG_H__C7A71BDB_746B_4BC0_9663_9091F33B820F__INCLUDED_)
#define AFX_FINDDIALOG_H__C7A71BDB_746B_4BC0_9663_9091F33B820F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindDialog.h : header file
//

#include "EnhMenuButton.h"
#include "FindComboBox.h"

class CBCGCBDotNetExampleView;

/////////////////////////////////////////////////////////////////////////////
// CFindDialog dialog

class CFindDialog : public CDialog
{
// Construction
public:
	CFindDialog(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CFindDialog)
	enum { IDD = IDD_FINDDIALOG };
	CBCGCBDotNetExampleView* m_pDocView;
	CFindComboBox	m_wndCombo;
	CEnhMenuButton	m_btnMenu;
	BOOL			m_bMatchCase;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMenu m_menu;

	// Generated message map functions
	//{{AFX_MSG(CFindDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnMarkAll();
	afx_msg void OnDestroy();
	afx_msg void OnMatchwholeChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMatchcaseChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
	afx_msg void OnRegularChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSearchallChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDIALOG_H__C7A71BDB_746B_4BC0_9663_9091F33B820F__INCLUDED_)
