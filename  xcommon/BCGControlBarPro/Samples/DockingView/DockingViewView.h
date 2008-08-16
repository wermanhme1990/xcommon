// DockingViewView.h : interface of the CDockingViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKINGVIEWVIEW_H__109F3FED_AE97_4C45_B08B_51EE19602794__INCLUDED_)
#define AFX_DOCKINGVIEWVIEW_H__109F3FED_AE97_4C45_B08B_51EE19602794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDockingViewView : public CView
{
protected: // create from serialization only
	CDockingViewView();
	DECLARE_DYNCREATE(CDockingViewView)

// Attributes
public:
	CDockingViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingViewView)
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
	virtual ~CDockingViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDockingViewView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DockingViewView.cpp
inline CDockingViewDoc* CDockingViewView::GetDocument()
   { return (CDockingViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGVIEWVIEW_H__109F3FED_AE97_4C45_B08B_51EE19602794__INCLUDED_)
