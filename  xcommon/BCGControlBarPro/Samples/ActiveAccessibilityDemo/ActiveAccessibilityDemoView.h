// ActiveAccessibilityDemoView.h : interface of the CActiveAccessibilityDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEACCESSIBILITYDEMOVIEW_H__B762D7F8_4F79_41D8_96F3_2728D3C11B08__INCLUDED_)
#define AFX_ACTIVEACCESSIBILITYDEMOVIEW_H__B762D7F8_4F79_41D8_96F3_2728D3C11B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CActiveAccessibilityDemoView : public CView
{
protected: // create from serialization only
	CActiveAccessibilityDemoView();
	DECLARE_DYNCREATE(CActiveAccessibilityDemoView)

// Attributes
public:
	CActiveAccessibilityDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveAccessibilityDemoView)
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
	virtual ~CActiveAccessibilityDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CActiveAccessibilityDemoView)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ActiveAccessibilityDemoView.cpp
inline CActiveAccessibilityDemoDoc* CActiveAccessibilityDemoView::GetDocument()
   { return (CActiveAccessibilityDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEACCESSIBILITYDEMOVIEW_H__B762D7F8_4F79_41D8_96F3_2728D3C11B08__INCLUDED_)
