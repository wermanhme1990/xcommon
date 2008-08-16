// CustomPagesView.h : interface of the CCustomPagesView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMPAGESVIEW_H__4396A901_90B8_4F64_8500_6989DBC32728__INCLUDED_)
#define AFX_CUSTOMPAGESVIEW_H__4396A901_90B8_4F64_8500_6989DBC32728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomPagesView : public CView
{
protected: // create from serialization only
	CCustomPagesView();
	DECLARE_DYNCREATE(CCustomPagesView)

// Attributes
public:
	CCustomPagesDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomPagesView)
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
	virtual ~CCustomPagesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomPagesView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CustomPagesView.cpp
inline CCustomPagesDoc* CCustomPagesView::GetDocument()
   { return (CCustomPagesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMPAGESVIEW_H__4396A901_90B8_4F64_8500_6989DBC32728__INCLUDED_)
