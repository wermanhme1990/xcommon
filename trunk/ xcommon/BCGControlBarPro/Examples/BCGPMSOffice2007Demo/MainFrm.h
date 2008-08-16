// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF400043_4CD2_41E4_80C1_5580326AEBA7__INCLUDED_)
#define AFX_MAINFRM_H__CF400043_4CD2_41E4_80C1_5580326AEBA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TaskPane.h"

class CMainFrame : public CBCGPFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CBCGPRibbonBar* GetRibbonBar ()
	{
		return &m_wndRibbonBar;
	}

// Operations
public:
	CBCGPToolBarImages* GetTooltipImage (UINT uiID);
	
	void SetRibbonContextCategory (UINT uiCategoryID);
	void ActivateRibbonContextCategory (UINT uiCategoryID);

	void OnZoomSlider();

protected:
	void AddMainCategory ();

	void AddTab_Home ();
	void AddTab_Insert ();
	void AddTab_PageLayout ();
	void AddTab_References ();
	void AddTab_Mailings ();
	void AddTab_View ();
	void AddTab_Developer ();

	void AddContextTab_Picture ();
	void AddContextTab_Chart ();
	void AddContextTab_Table ();
	
	CBCGPRibbonPanel* AddPanelArrange (CBCGPRibbonCategory* pCategory, 
					UINT imageP, UINT imageS, UINT imageL);

	void InitBulletPalette (CBCGPRibbonPaletteButton* pPalette);
	void InitNumberingPalette (CBCGPRibbonPaletteButton* pPalette);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

	virtual void AdjustClientArea ();
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPRibbonBar			m_wndRibbonBar;	// Ribbon control bar
	CBCGPRibbonMainButton	m_MainButton;	// Ribbon main button
	CBCGPRibbonStatusBar	m_wndStatusBar;

	CBCGPToolBarImages		m_PanelImages;	// Ribbon panel images
											// (appear when pane is collapsed)

	CMap<UINT,UINT,CBCGPToolBarImages*,CBCGPToolBarImages*>
							m_TooltipImages;

	void CreateTextPalette (CPalette& pal);

	void ShowProgress (BOOL bShow = TRUE);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg void OnRefresh();
	afx_msg void OnSpell();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnToolsOptions ();
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateRibbonBar ();
	BOOL CreateStatusBar ();

	void ShowOptions (int nPage);

	CBCGPControlRenderer m_Pat[4];

	CRect		m_rectFill;
    CBitmap		m_MemBitmap;
	CRect		m_rectSizing;

	int			m_nProgressValue;
	BOOL		m_bInfiniteProgressMode;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF400043_4CD2_41E4_80C1_5580326AEBA7__INCLUDED_)
