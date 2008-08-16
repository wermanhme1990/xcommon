// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3F9B386A_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_MAINFRM_H__3F9B386A_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolsPalette.h"

class CMainFrame : public CBCGPFrameWnd
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
	CBCGPMenuBar	m_wndMenuBar;
	CBCGPStatusBar	m_wndStatusBar;
	CBCGPToolBar	m_wndToolBar;
	CToolsPalette	m_wndPalette;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewToolsPalette();
	afx_msg void OnUpdateViewToolsPalette(CCmdUI* pCmdUI);
	afx_msg void OnTwoColumns();
	afx_msg void OnUpdateTwoColumns(CCmdUI* pCmdUI);
	afx_msg void OnThreeColumns();
	afx_msg void OnUpdateThreeColumns(CCmdUI* pCmdUI);
	afx_msg void OnFourColumns();
	afx_msg void OnUpdateFourColumns(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewCustomize();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3F9B386A_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
