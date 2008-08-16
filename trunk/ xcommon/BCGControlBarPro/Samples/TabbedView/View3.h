#if !defined(AFX_VIEW3_H__1C86C69A_BE4F_42BD_ACE1_1FEC44869C74__INCLUDED_)
#define AFX_VIEW3_H__1C86C69A_BE4F_42BD_ACE1_1FEC44869C74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView3 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CView3 : public CFormView
{
protected:
	CView3();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView3)

// Form Data
public:
	//{{AFX_DATA(CView3)
	enum { IDD = IDD_FORMVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CView3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CView3)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3_H__1C86C69A_BE4F_42BD_ACE1_1FEC44869C74__INCLUDED_)
