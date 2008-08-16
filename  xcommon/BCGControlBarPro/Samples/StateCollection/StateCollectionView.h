// StateCollectionView.h : interface of the CStateCollectionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATECOLLECTIONVIEW_H__36FFDF17_2981_4CFC_AF93_5AEB82BFC0D0__INCLUDED_)
#define AFX_STATECOLLECTIONVIEW_H__36FFDF17_2981_4CFC_AF93_5AEB82BFC0D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStateCollectionView : public CView
{
protected: // create from serialization only
	CStateCollectionView();
	DECLARE_DYNCREATE(CStateCollectionView)

// Attributes
public:
	CStateCollectionDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateCollectionView)
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
	virtual ~CStateCollectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStateCollectionView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in StateCollectionView.cpp
inline CStateCollectionDoc* CStateCollectionView::GetDocument()
   { return (CStateCollectionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATECOLLECTIONVIEW_H__36FFDF17_2981_4CFC_AF93_5AEB82BFC0D0__INCLUDED_)
