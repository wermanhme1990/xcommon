// DynamicMenuView.h : interface of the CDynamicMenuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNAMICMENUVIEW_H__CA1CF6DF_40B1_41AF_9C51_9AB70D8677CE__INCLUDED_)
#define AFX_DYNAMICMENUVIEW_H__CA1CF6DF_40B1_41AF_9C51_9AB70D8677CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynamicMenuView : public CView
{
protected: // create from serialization only
	CDynamicMenuView();
	DECLARE_DYNCREATE(CDynamicMenuView)

// Attributes
public:
	CDynamicMenuDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicMenuView)
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
	virtual ~CDynamicMenuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDynamicMenuView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DynamicMenuView.cpp
inline CDynamicMenuDoc* CDynamicMenuView::GetDocument()
   { return (CDynamicMenuDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICMENUVIEW_H__CA1CF6DF_40B1_41AF_9C51_9AB70D8677CE__INCLUDED_)
