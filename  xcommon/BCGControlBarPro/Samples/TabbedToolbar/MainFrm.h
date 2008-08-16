// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3F6E55C6_0B33_4F55_98A0_D060AFA7DFB1__INCLUDED_)
#define AFX_MAINFRM_H__3F6E55C6_0B33_4F55_98A0_D060AFA7DFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CFrameWnd CBCGPFrameWnd

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

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
	CBCGPMenuBar		m_wndMenuBar;
	CBCGPStatusBar		m_wndStatusBar;
	CBCGPToolBar		m_wndToolBarStandard;
	CBCGPToolBar		m_wndToolBarAdditional;
	CBCGPToolBar		m_wndToolBarWin32;
	CBCGPToolBar		m_wndToolBarSystem;
	CBCGPToolBar		m_wndToolBarInternet;
	CBCGPToolBar		m_wndToolBarDataAccess;
	CBCGPToolBar		m_wndToolBarDataControls;
	CBCGPToolBar		m_wndToolBarQReports;
	CBCGPToolBar		m_wndToolBarDialogs;
	CBCGPToolBar		m_wndToolBarActiveX;
	CBCGPTabbedToolbar	m_wndTabbedToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolsButton(UINT uiCmd);
	afx_msg void OnUpdateToolsButton(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3F6E55C6_0B33_4F55_98A0_D060AFA7DFB1__INCLUDED_)
