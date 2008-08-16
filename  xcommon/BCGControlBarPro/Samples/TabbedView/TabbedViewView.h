// TabbedViewView.h : interface of the CTabbedViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDVIEWVIEW_H__8D08E849_7B7E_4C16_9000_D249F93A439F__INCLUDED_)
#define AFX_TABBEDVIEWVIEW_H__8D08E849_7B7E_4C16_9000_D249F93A439F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabbedViewView : public CBCGPTabView
{
protected: // create from serialization only
	CTabbedViewView();
	DECLARE_DYNCREATE(CTabbedViewView)

// Attributes
public:
	CTabbedViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	BOOL IsScrollBar () const
	{
		return TRUE;
	}

// Implementation
public:
	virtual ~CTabbedViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedViewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TabbedViewView.cpp
inline CTabbedViewDoc* CTabbedViewView::GetDocument()
   { return (CTabbedViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDVIEWVIEW_H__8D08E849_7B7E_4C16_9000_D249F93A439F__INCLUDED_)
