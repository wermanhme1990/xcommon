// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F83C8E0F_F33E_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_MAINFRM_H__F83C8E0F_F33E_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CFrameWnd CBCGPFrameWnd

#include "LinksBar.h"
#include "ExplorerBar.h"

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMenu								m_menuFavotites;

protected:
	CBitmap								m_bmpBack;
	CBitmap								m_bmpStatus;
	CImageList							m_imlStatusAnimation;

// Operations
public:
	void SetAddress(LPCTSTR lpszUrl);
	void StartAnimation();
	BOOL IsFavoritesMenu (const CBCGPToolbarMenuButton* pMenuButton);

	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual BOOL OnDrawMenuImage (CDC* pDC,
									const CBCGPToolbarMenuButton* pMenuButton, 
									const CRect& rectImage);

	virtual BOOL OnMenuButtonToolHitTest (CBCGPToolbarButton* pButton, TOOLINFO* pTI);
	virtual BOOL GetToolbarButtonToolTipText (CBCGPToolbarButton* pButton, CString& strTTText);

	void LoadBackImage ();
	void SetBackImage ();

	void SetProgress (long nCurr, long nTotal);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPMenuBar	m_wndMenuBar;
	CBCGPStatusBar	m_wndStatusBar;
	CBCGPToolBar	m_wndToolBar;
	CLinksBar		m_wndLinksBar;
	CBCGPReBar		m_wndReBar;
	CBCGPAnimCtrl	m_wndAnimate;
	CComboBoxEx		m_wndAddress;
	CExplorerBar	m_wndExplorerBar;

	BOOL			m_bMainToolbarMenu;
	BOOL			m_bIsHighColor;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLink1();
	afx_msg void OnViewAddressBar();
	afx_msg void OnUpdateViewAddressBar(CCmdUI* pCmdUI);
	afx_msg void OnViewLinksBar();
	afx_msg void OnUpdateViewLinksBar(CCmdUI* pCmdUI);
	afx_msg void OnViewTextlabels();
	afx_msg void OnUpdateViewTextlabels(CCmdUI* pCmdUI);
	afx_msg void OnViewBackground();
	afx_msg void OnUpdateViewBackground(CCmdUI* pCmdUI);
	afx_msg void OnSysColorChange();
	afx_msg void OnViewExplorerbar();
	afx_msg void OnUpdateViewExplorerbar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
	afx_msg void OnNewAddress();
	afx_msg void OnNewAddressEnter();
	afx_msg void OnFavorite(UINT nID);
	afx_msg void OnHistory(UINT nID);
	afx_msg void OnLinkBestCompany();
	afx_msg void OnLinkBestCar();
	afx_msg void OnLinkBestRockband();
	afx_msg void OnLinkBestCity();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F83C8E0F_F33E_11D2_A713_0090274409AC__INCLUDED_)
