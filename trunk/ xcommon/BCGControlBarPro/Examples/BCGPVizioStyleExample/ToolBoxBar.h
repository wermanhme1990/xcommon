// ToolBoxBar.h: interface for the CToolBoxBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBOXBAR_H__3B91F262_7A9A_43ED_8107_6EE797D92B22__INCLUDED_)
#define AFX_TOOLBOXBAR_H__3B91F262_7A9A_43ED_8107_6EE797D92B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CToolBoxList window

class CToolBoxList : public CListCtrl
{
// Construction
public:
	CToolBoxList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBoxList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxList)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CToolBoxBar : public CBCGPDockingControlBar  
{
public:
	CToolBoxBar();

// Attributes
protected:
	CToolBoxList	m_wndList;
	CImageList	m_images;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBoxBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TOOLBOXBAR_H__3B91F262_7A9A_43ED_8107_6EE797D92B22__INCLUDED_)
