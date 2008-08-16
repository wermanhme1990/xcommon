// BCGPExplorerView.h : interface of the CBCGPExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPEXPLORERVIEW_H__56FB6332_9EAD_4007_8B43_B21B9F050E42__INCLUDED_)
#define AFX_BCGPEXPLORERVIEW_H__56FB6332_9EAD_4007_8B43_B21B9F050E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPExplorerView : public CView
{
protected: // create from serialization only
	CBCGPExplorerView();
	DECLARE_DYNCREATE(CBCGPExplorerView)

// Attributes
public:
	CBCGPExplorerDoc* GetDocument();
	CBCGPShellList	m_wndList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPExplorerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPExplorerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewLargeicon();
	afx_msg void OnUpdateViewLargeicon(CCmdUI* pCmdUI);
	afx_msg void OnViewDetails();
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnViewSmallicon();
	afx_msg void OnUpdateViewSmallicon(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFolderUp();
	afx_msg void OnUpdateFolderUp(CCmdUI* pCmdUI);
	afx_msg void OnCopyTo();
	afx_msg void OnMoveTo();
	afx_msg void OnViewRefresh();
	//}}AFX_MSG
	afx_msg LRESULT OnChangeFolder(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BCGPExplorerView.cpp
inline CBCGPExplorerDoc* CBCGPExplorerView::GetDocument()
   { return (CBCGPExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPEXPLORERVIEW_H__56FB6332_9EAD_4007_8B43_B21B9F050E42__INCLUDED_)
