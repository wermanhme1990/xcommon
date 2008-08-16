// $$workspace_hfile$$.h : interface of the $$WORKSPACEBAR_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class $$WORKSPACEBAR_CLASS$$ : public CBCGPDockingControlBar
{
public:
	$$WORKSPACEBAR_CLASS$$();

// Attributes
protected:
	CTreeCtrl	m_wndTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$WORKSPACEBAR_CLASS$$)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~$$WORKSPACEBAR_CLASS$$();

// Generated message map functions
protected:
	//{{AFX_MSG($$WORKSPACEBAR_CLASS$$)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
