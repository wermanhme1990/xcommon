// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0E43D1CC_5903_4633_A715_0E8FD092E860__INCLUDED_)
#define AFX_MAINFRM_H__0E43D1CC_5903_4633_A715_0E8FD092E860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CBCGPFrameWnd
{
	friend class CToolBoxDemoView;

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	CBCGPToolBoxPage* GetToolBoxPage (int nPage);
	void SetToolBoxLayout (int nLayout);
	void SetToolBoxMode (BOOL bIsExt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPToolBar			m_wndToolBar;

	CBCGPDockingControlBar*	m_pWndToolBoxBar;
	BOOL					m_bIsToolBoxEx;
	int						m_nPageLayout;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnToolBox();
	DECLARE_MESSAGE_MAP()

	BOOL CreateToolBox ();
	UINT	m_nAppLook;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0E43D1CC_5903_4633_A715_0E8FD092E860__INCLUDED_)
