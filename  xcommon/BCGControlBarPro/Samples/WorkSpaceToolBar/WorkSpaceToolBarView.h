// WorkSpaceToolBarView.h : interface of the CWorkSpaceToolBarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACETOOLBARVIEW_H__0BE25353_6B5A_41B8_906D_E6F57CE92077__INCLUDED_)
#define AFX_WORKSPACETOOLBARVIEW_H__0BE25353_6B5A_41B8_906D_E6F57CE92077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWorkSpaceToolBarView : public CView
{
protected: // create from serialization only
	CWorkSpaceToolBarView();
	DECLARE_DYNCREATE(CWorkSpaceToolBarView)

// Attributes
public:
	CWorkSpaceToolBarDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceToolBarView)
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
	virtual ~CWorkSpaceToolBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceToolBarView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WorkSpaceToolBarView.cpp
inline CWorkSpaceToolBarDoc* CWorkSpaceToolBarView::GetDocument()
   { return (CWorkSpaceToolBarDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACETOOLBARVIEW_H__0BE25353_6B5A_41B8_906D_E6F57CE92077__INCLUDED_)
