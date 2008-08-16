// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3D253DE4_0FD1_4C9B_9ED3_A105F8B4857A__INCLUDED_)
#define AFX_MAINFRM_H__3D253DE4_0FD1_4C9B_9ED3_A105F8B4857A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------- Example --------------------//
// here is the overriden version of of CBCGPMenuBar that will dinamically modify 
// the top level menu bar
#include "MyMenuBar.h"
//-----------------------------------------//

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
	//------------- Example --------------------//
	CMyMenuBar		m_wndMenuBar;	// the wizard generated CBCGPMenuBar is changed to CMyMenuBar
	//------------------------------------------//
	CBCGPStatusBar		m_wndStatusBar;
	CBCGPToolBar		m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDynamicItem1();
	afx_msg void OnDynamicItem2();
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg LRESULT OnResetMenu(WPARAM,LPARAM);
	afx_msg void OnEditMyItem1 ();
	afx_msg void OnEditMyItem2 ();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	//------------- Example --------------------//
	virtual BOOL OnDrawMenuImage (	CDC* pDC, 
									const CBCGPToolbarMenuButton* pMenuButton, 
									const CRect& rectImage);
	//------------------------------------------//
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3D253DE4_0FD1_4C9B_9ED3_A105F8B4857A__INCLUDED_)
