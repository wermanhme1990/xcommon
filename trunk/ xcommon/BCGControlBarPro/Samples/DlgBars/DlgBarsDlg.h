// DlgBarsDlg.h : header file
//

#if !defined(AFX_DLGBARSDLG_H__83F01224_470C_4BE5_B3ED_6AA844B62AFC__INCLUDED_)
#define AFX_DLGBARSDLG_H__83F01224_470C_4BE5_B3ED_6AA844B62AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBarsDlg dialog

#define CDialog CBCGPDialog

class CMyMenuBar : public CBCGPMenuBar
{
	virtual BOOL LoadState (LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1)	{	return TRUE;	}
	virtual BOOL SaveState (LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1)	{	return TRUE;	}
};

class CDlgBarsDlg : public CDialog
{
// Construction
public:
	CDlgBarsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBarsDlg)
	enum { IDD = IDD_DLGBARS_DIALOG };
	CStatic	m_wndMenuBarLocation;
	CStatic	m_wndStatusBarLocation;
	CStatic	m_wndToolbarLocation;
	CStatic	m_wndOutlookBarLocation;
	CStatic	m_wndCaptionLocation;
	//}}AFX_DATA

	CBCGPOutlookBar		m_wndOutlookBar;
	CBCGPOutlookBarPane	m_wndPane1;
	CBCGPOutlookBarPane	m_wndPane2;

	CMyMenuBar			m_wndMenuBar;
	CBCGPCaptionBar		m_wndCaptionBar;
	CBCGPToolBar		m_wndToolBar;
	CBCGPStatusBar		m_wndStatusBar;

	CBitmap				m_bmpCaption;

	CCmdFrame*			m_pMenuFrame;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDlgBarsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFilePrint();
	afx_msg void OnFileSave();
	afx_msg void OnAppAbout();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnShortcut(UINT uiCmdID);
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBARSDLG_H__83F01224_470C_4BE5_B3ED_6AA844B62AFC__INCLUDED_)
