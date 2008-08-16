// BCGPMSMoneyDemoView.h : interface of the CBCGPMSMoneyDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPMSMoneyDemoVIEW_H__F83C8E13_F33E_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_BCGPMSMoneyDemoVIEW_H__F83C8E13_F33E_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCGPMSMoneyDemoView : public CView
{
protected: // create from serialization only
	CBCGPMSMoneyDemoView();
	DECLARE_DYNCREATE(CBCGPMSMoneyDemoView)

// Attributes
public:
	CBCGPMSMoneyDemoDoc* GetDocument();

protected:
	CBitmap	m_Image;
	CSize	m_sizeImage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMSMoneyDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPMSMoneyDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPMSMoneyDemoView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BCGPMSMoneyDemoView.cpp
inline CBCGPMSMoneyDemoDoc* CBCGPMSMoneyDemoView::GetDocument()
{
	return (CBCGPMSMoneyDemoDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMSMoneyDemoVIEW_H__F83C8E13_F33E_11D2_A713_0090274409AC__INCLUDED_)
