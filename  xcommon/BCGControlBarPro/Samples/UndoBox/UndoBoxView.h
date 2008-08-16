// UndoBoxView.h : interface of the CUndoBoxView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOBOXVIEW_H__90FE94CB_02EA_49DB_9992_2A21B0D35222__INCLUDED_)
#define AFX_UNDOBOXVIEW_H__90FE94CB_02EA_49DB_9992_2A21B0D35222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUndoBoxView : public CView
{
protected: // create from serialization only
	CUndoBoxView();
	DECLARE_DYNCREATE(CUndoBoxView)

// Attributes
public:
	CUndoBoxDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoBoxView)
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
	virtual ~CUndoBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUndoBoxView)
	afx_msg void OnEditUndo();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UndoBoxView.cpp
inline CUndoBoxDoc* CUndoBoxView::GetDocument()
   { return (CUndoBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOBOXVIEW_H__90FE94CB_02EA_49DB_9992_2A21B0D35222__INCLUDED_)
