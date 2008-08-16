// BCGPGridExampleView.h : interface of the CBCGPGridExampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPGRIDEXAMPLEVIEW_H__F77E2428_8CF3_4411_945C_5B0D3CF831FB__INCLUDED_)
#define AFX_BCGPGRIDEXAMPLEVIEW_H__F77E2428_8CF3_4411_945C_5B0D3CF831FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CViewToolBar : public CBCGPToolBar
{
public: 
	// All commands should be routed via owner window, not via frame:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) 
	{ 
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
	}

	// Toolbar should't appear on the customization list
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}
};

class CBCGPGridExampleView : public CBCGPGridView
{
protected: // create from serialization only
	CBCGPGridExampleView();
	DECLARE_DYNCREATE(CBCGPGridExampleView)

// Attributes
public:
	CBCGPGridExampleDoc* GetDocument();

protected:
	CBCGPGridCtrl*							m_pCurrGrid;
	CArray<CBCGPGridCtrl*, CBCGPGridCtrl*>	m_arGrids;
	int										m_nCurrType;
	CViewToolBar							m_wndToolBar;
	BOOL									m_bIsToolbar;

// Operations
public:
	void SetViewType (int nType);
	void UpdateGrid ();
	void AdjustLayout ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPGridExampleView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPGridExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPGridExampleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnFieldChooser();
	afx_msg void OnUpdateFieldChooser(CCmdUI* pCmdUI);
	afx_msg void OnGroupByBox();
	afx_msg void OnUpdateGroupByBox(CCmdUI* pCmdUI);
	afx_msg void OnColorThemeCombo();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnHideColumnChooser (WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BCGPGridExampleView.cpp
inline CBCGPGridExampleDoc* CBCGPGridExampleView::GetDocument()
   { return (CBCGPGridExampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPGRIDEXAMPLEVIEW_H__F77E2428_8CF3_4411_945C_5B0D3CF831FB__INCLUDED_)
