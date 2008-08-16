// TabControlDlg.h : header file
//

#if !defined(AFX_TABCONTROLDLG_H__E5632AB3_2BD7_4045_AD3B_9191770EC7EC__INCLUDED_)
#define AFX_TABCONTROLDLG_H__E5632AB3_2BD7_4045_AD3B_9191770EC7EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTabControlDlg dialog

class CTabControlDlg : public CBCGPDialog
{
// Construction
public:
	CTabControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTabControlDlg)
	enum { IDD = IDD_TABCONTROL_DIALOG };
	CBCGPColorButton	m_wndColor;
	CButton	m_wndDefaultColors;
	CEdit	m_wndTabsArea;
	BOOL	m_bAutoColors;
	BOOL	m_bDefaultColors;
	int		m_nTab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTabControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDefaultColors();
	afx_msg void OnEnableAutoColor();
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBCGPTabWnd	m_wndTabs;

	CEdit	m_wnd1;
	CEdit	m_wnd2;
	CEdit	m_wnd3;
	CEdit	m_wnd4;
	CEdit	m_wnd5;
	CEdit	m_wnd6;

	void SetTabColors ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCONTROLDLG_H__E5632AB3_2BD7_4045_AD3B_9191770EC7EC__INCLUDED_)
