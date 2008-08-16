// ToolbarLabelView.h : interface of the CToolbarLabelView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARLABELVIEW_H__EE74625F_2F5E_4F98_93D7_B4F0F55B8937__INCLUDED_)
#define AFX_TOOLBARLABELVIEW_H__EE74625F_2F5E_4F98_93D7_B4F0F55B8937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolbarLabelView : public CView
{
protected: // create from serialization only
	CToolbarLabelView();
	DECLARE_DYNCREATE(CToolbarLabelView)

// Attributes
public:
	CToolbarLabelDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarLabelView)
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
	virtual ~CToolbarLabelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarLabelView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ToolbarLabelView.cpp
inline CToolbarLabelDoc* CToolbarLabelView::GetDocument()
   { return (CToolbarLabelDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARLABELVIEW_H__EE74625F_2F5E_4F98_93D7_B4F0F55B8937__INCLUDED_)
