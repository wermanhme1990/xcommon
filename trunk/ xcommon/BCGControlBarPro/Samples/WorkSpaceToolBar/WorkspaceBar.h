// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__BB90CA5E_A36A_4B42_A0A4_97916997D2A2__INCLUDED_)
#define AFX_WORKSPACEBAR_H__BB90CA5E_A36A_4B42_A0A4_97916997D2A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkspaceToolBar : public CBCGPToolBar
{
public: 
	// All commands should be routed via owner window, not via frame:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) 
	{ 
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
	}

	// Toolbar should't appear on the customization list
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}
	virtual void AdjustLayout ();
};


class CWorkspaceBar : public CBCGPDockingControlBar
{
public:
	CWorkspaceBar();

// Attributes
protected:
	CWorkspaceToolBar	m_wndToolBar;
	CBCGPTabWnd			m_wndTabs;

	CTreeCtrl			m_wndTree1;
	CTreeCtrl			m_wndTree2;

// Operations
public:
	void AdjustLayout ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCommand1();
	afx_msg void OnCommand2();
	afx_msg void OnCommand3();
	afx_msg void OnUpdateCommand3(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__BB90CA5E_A36A_4B42_A0A4_97916997D2A2__INCLUDED_)
