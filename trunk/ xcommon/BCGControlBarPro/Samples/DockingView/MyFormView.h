#if !defined(AFX_MYFORMVIEW_H__0365F7A2_9484_429E_8546_0EEFB452AD54__INCLUDED_)
#define AFX_MYFORMVIEW_H__0365F7A2_9484_429E_8546_0EEFB452AD54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMyFormView : public CFormView
{
protected:
	CMyFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFormView)

// Form Data
public:
	//{{AFX_DATA(CMyFormView)
	enum { IDD = IDD_FORMVIEW };
	CString	m_strCount;
	//}}AFX_DATA

	int	m_nCount;

// Attributes
public:

// Operations
public:
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFormView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMyFormView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFORMVIEW_H__0365F7A2_9484_429E_8546_0EEFB452AD54__INCLUDED_)
