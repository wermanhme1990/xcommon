// TrayMenuDlg.h : header file
//

#if !defined(AFX_TRAYMENUDLG_H__4D261DAB_3ED5_4DB0_A774_4EB954A43E10__INCLUDED_)
#define AFX_TRAYMENUDLG_H__4D261DAB_3ED5_4DB0_A774_4EB954A43E10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTrayMenuDlg dialog

#define CDialog CBCGPDialog

class CTrayMenuDlg : public CDialog
{
// Construction
public:
	CTrayMenuDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTrayMenuDlg();

// Dialog Data
	//{{AFX_DATA(CTrayMenuDlg)
	enum { IDD = IDD_TRAYMENU_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayMenuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTrayMenuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnAppOpen();
	afx_msg void OnItem1();
	afx_msg void OnItem2();
	afx_msg void OnItem3();
	afx_msg void OnItem4();
	afx_msg void OnUpdateItem4(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	NOTIFYICONDATA	m_nid;			// struct for Shell_NotifyIcon args

	void OnTrayContextMenu ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYMENUDLG_H__4D261DAB_3ED5_4DB0_A774_4EB954A43E10__INCLUDED_)
