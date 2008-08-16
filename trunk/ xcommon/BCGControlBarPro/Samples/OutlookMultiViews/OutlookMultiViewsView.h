// OutlookMultiViewsView.h : interface of the COutlookMultiViewsView class
//
/////////////////////////////////////////////////////////////////////////////
//
// This sample is based on Vswap32 sample:
// http://support.microsoft.com/default.aspx?scid=kb;en-us;141334

#if !defined(AFX_OUTLOOKMULTIVIEWSVIEW_H__BF301EB0_24ED_4FB6_BADB_A0102E7A26AB__INCLUDED_)
#define AFX_OUTLOOKMULTIVIEWSVIEW_H__BF301EB0_24ED_4FB6_BADB_A0102E7A26AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COutlookMultiViewsView : public CView
{
protected: // create from serialization only
	COutlookMultiViewsView();
	DECLARE_DYNCREATE(COutlookMultiViewsView)

// Attributes
public:
	COutlookMultiViewsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookMultiViewsView)
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
	virtual ~COutlookMultiViewsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COutlookMultiViewsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OutlookMultiViewsView.cpp
inline COutlookMultiViewsDoc* COutlookMultiViewsView::GetDocument()
   { return (COutlookMultiViewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLOOKMULTIVIEWSVIEW_H__BF301EB0_24ED_4FB6_BADB_A0102E7A26AB__INCLUDED_)
