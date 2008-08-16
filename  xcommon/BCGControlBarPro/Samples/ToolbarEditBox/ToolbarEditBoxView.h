// ToolbarEditBoxView.h : interface of the CToolbarEditBoxView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBAREDITBOXVIEW_H__AE4B3DF1_5634_4359_84F2_BFCB1004DF9B__INCLUDED_)
#define AFX_TOOLBAREDITBOXVIEW_H__AE4B3DF1_5634_4359_84F2_BFCB1004DF9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolbarEditBoxView : public CView
{
protected: // create from serialization only
	CToolbarEditBoxView();
	DECLARE_DYNCREATE(CToolbarEditBoxView)

// Attributes
public:
	CToolbarEditBoxDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarEditBoxView)
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
	virtual ~CToolbarEditBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarEditBoxView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ToolbarEditBoxView.cpp
inline CToolbarEditBoxDoc* CToolbarEditBoxView::GetDocument()
   { return (CToolbarEditBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREDITBOXVIEW_H__AE4B3DF1_5634_4359_84F2_BFCB1004DF9B__INCLUDED_)
