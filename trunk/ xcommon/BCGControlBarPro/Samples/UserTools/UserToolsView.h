// UserToolsView.h : interface of the CUserToolsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTOOLSVIEW_H__D2CFB54D_C030_4887_8905_0D40B5E2379A__INCLUDED_)
#define AFX_USERTOOLSVIEW_H__D2CFB54D_C030_4887_8905_0D40B5E2379A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUserToolsView : public CView
{
protected: // create from serialization only
	CUserToolsView();
	DECLARE_DYNCREATE(CUserToolsView)

// Attributes
public:
	CUserToolsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserToolsView)
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
	virtual ~CUserToolsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUserToolsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UserToolsView.cpp
inline CUserToolsDoc* CUserToolsView::GetDocument()
   { return (CUserToolsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERTOOLSVIEW_H__D2CFB54D_C030_4887_8905_0D40B5E2379A__INCLUDED_)
