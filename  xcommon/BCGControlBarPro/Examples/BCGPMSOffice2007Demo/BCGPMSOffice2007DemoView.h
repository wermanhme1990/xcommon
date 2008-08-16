// BCGPMSOffice2007DemoView.h : interface of the CBCGPMSOffice2007DemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPMSOFFICE2007DEMOVIEW_H__C6C975F4_635B_4202_B288_F5A93EF3E7B4__INCLUDED_)
#define AFX_BCGPMSOFFICE2007DEMOVIEW_H__C6C975F4_635B_4202_B288_F5A93EF3E7B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPMSOffice2007DemoCntrItem;
struct CCharFormat;

class CBCGPMSOffice2007DemoView : public CRichEditView
{
protected: // create from serialization only
	CBCGPMSOffice2007DemoView();
	DECLARE_DYNCREATE(CBCGPMSOffice2007DemoView)

// Attributes
public:
	CBCGPMSOffice2007DemoDoc* GetDocument();
	static BOOL m_bForceReloadBmps;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMSOffice2007DemoView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	virtual void SyncFont ();

// Implementation
public:
	virtual ~CBCGPMSOffice2007DemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPMSOffice2007DemoView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClipboardDialog();
	afx_msg void OnFontDialog();
	afx_msg void OnParagraphDialog();
	afx_msg void OnStyleDialog();
	afx_msg void OnPagesetupDialog();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFontColor();
	afx_msg void OnFontname();
	afx_msg void OnFontsize();
	afx_msg void OnEditChange();
	afx_msg void OnDummy(UINT id);
	afx_msg void OnInsertPicture();
	afx_msg void OnInsertChart();
	afx_msg void OnInsertTable();
	afx_msg void OnZoom();
	afx_msg void OnZoomSlider();
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdatePrintLayout(CCmdUI* pCmdUI);
	afx_msg void OnIndentLeft();
	afx_msg void OnIndentRight();
	afx_msg void OnSpaceAfter();
	afx_msg void OnSpaceBefore();
	afx_msg void OnViewRuler();
	afx_msg void OnViewGridLines();
	afx_msg void OnViewProps();
	afx_msg void OnViewDocMap();
	afx_msg void OnViewThumb();
	afx_msg void OnViewActionBar();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGridLines(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewProps(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDocMap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewThumb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewActionBar(CCmdUI* pCmdUI);
	afx_msg void OnInsertBulletFromPalette();
	afx_msg void OnInsertNumberFromPalette();
	DECLARE_MESSAGE_MAP()

	BOOL ShowContextMenu (CPoint pt);

	void GetDefaultFont(CCharFormat& cf);
	BOOL InsertBitmap (UINT uiBmpResID);

protected:
	BOOL m_bIsEndOfPrint;

	BOOL m_bViewRuler;
	BOOL m_bViewGridLines;
	BOOL m_bViewProps;
	BOOL m_bViewDocMap;
	BOOL m_bViewThumb;
	BOOL m_bViewActionBar;
};

#ifndef _DEBUG  // debug version in BCGPMSOffice2007DemoView.cpp
inline CBCGPMSOffice2007DemoDoc* CBCGPMSOffice2007DemoView::GetDocument()
   { return (CBCGPMSOffice2007DemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMSOFFICE2007DEMOVIEW_H__C6C975F4_635B_4202_B288_F5A93EF3E7B4__INCLUDED_)
