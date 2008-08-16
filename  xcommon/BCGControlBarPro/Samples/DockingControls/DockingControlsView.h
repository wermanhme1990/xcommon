// DockingControlsView.h : interface of the CDockingControlsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKINGCONTROLSVIEW_H__BA15CE41_6E26_4DB6_94E4_7D8CE36E520D__INCLUDED_)
#define AFX_DOCKINGCONTROLSVIEW_H__BA15CE41_6E26_4DB6_94E4_7D8CE36E520D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDockingControlsView : public CView
{
protected: // create from serialization only
	CDockingControlsView();
	DECLARE_DYNCREATE(CDockingControlsView)

// Attributes
public:
	CDockingControlsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingControlsView)
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
	virtual ~CDockingControlsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDockingControlsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DockingControlsView.cpp
inline CDockingControlsDoc* CDockingControlsView::GetDocument()
   { return (CDockingControlsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGCONTROLSVIEW_H__BA15CE41_6E26_4DB6_94E4_7D8CE36E520D__INCLUDED_)
