// $$outputbar_hfile$$.h : interface of the $$OUTPUTBAR_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class $$OUTPUTBAR_CLASS$$ : public CBCGPDockingControlBar
{
public:
	$$OUTPUTBAR_CLASS$$();

// Attributes
protected:
	CBCGPTabWnd	m_wndTabs;

	CListCtrl	m_wndList1;
	CListCtrl	m_wndList2;
	CListCtrl	m_wndList3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$OUTPUTBAR_CLASS$$)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~$$OUTPUTBAR_CLASS$$();

// Generated message map functions
protected:
	//{{AFX_MSG($$OUTPUTBAR_CLASS$$)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
