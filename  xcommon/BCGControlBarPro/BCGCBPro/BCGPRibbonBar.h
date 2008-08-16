//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
// BCGPRibbonBar.h : header file
//

#if !defined(AFX_BCGPRIBBONBAR_H__B3C69A4D_9CDE_406F_9837_2347D306920A__INCLUDED_)
#define AFX_BCGPRIBBONBAR_H__B3C69A4D_9CDE_406F_9837_2347D306920A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AFX_IDW_RIBBON_BAR		0xE806  // CBCGPRibbonBar
#define RIBBON_CAPTION_BUTTONS	3	

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPControlBar.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"

class CBCGPRibbonCategory;
class CBCGPRibbonBar;
class CBCGPRibbonMainButton;
class CBCGPToolbarMenuButton;
class CBCGPRibbonMainPanel;
class CBCGPRibbonEditCtrl;

/////////////////////////////////////////////////////////////////////////////
// BCGPRibbonCategoryColor

enum BCGPRibbonCategoryColor
{
	BCGPCategoryColor_None,
	BCGPCategoryColor_Red,
	BCGPCategoryColor_Orange,
	BCGPCategoryColor_Yellow,
	BCGPCategoryColor_Green,
	BCGPCategoryColor_Blue,
	BCGPCategoryColor_Indigo,
	BCGPCategoryColor_Violet
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCaptionButton

class BCGCBPRODLLEXPORT CBCGPRibbonCaptionButton : public CBCGPRibbonButton
{
	DECLARE_DYNCREATE (CBCGPRibbonCaptionButton)

	friend class CBCGPRibbonBar;

public:
	BOOL IsMDIChildButton () const
	{
		return m_hwndMDIChild != NULL;
	}

protected:
	CBCGPRibbonCaptionButton (UINT uiCmd = 0, HWND hwndMDIChild = NULL);

	virtual void OnDraw (CDC* pDC);
	virtual void OnLButtonUp (CPoint point);

	virtual CSize GetRegularSize (CDC* pDC);

	virtual CSize GetCompactSize (CDC* pDC)
	{
		return GetRegularSize (pDC);
	}

	HWND	m_hwndMDIChild;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonContextCaption

class BCGCBPRODLLEXPORT CBCGPRibbonContextCaption : public CBCGPRibbonButton
{
	DECLARE_DYNCREATE (CBCGPRibbonContextCaption)

	friend class CBCGPRibbonBar;

public:
	BCGPRibbonCategoryColor	GetColor () const
	{
		return m_Color;
	}

protected:
	CBCGPRibbonContextCaption (LPCTSTR lpszName, UINT uiID, BCGPRibbonCategoryColor clrContext);
	CBCGPRibbonContextCaption ();

	virtual void OnDraw (CDC* pDC);
	virtual void OnLButtonUp (CPoint point);

	UINT					m_uiID;
	BCGPRibbonCategoryColor	m_Color;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonBar window

#define BCGPRIBBONBAR_HIDE_ELEMENTS	0x0001
#define BCGPRIBBONBAR_HIDE_ALL		0x0002

class BCGCBPRODLLEXPORT CBCGPRibbonBar : public CBCGPControlBar
{
	friend class CBCGPRibbonCategory;
	friend class CBCGPRibbonCmdUI;
	friend class CBCGPBaseRibbonElement;
	friend class CBCGPRibbonCustomizePage;
	friend class CBCGPPrintPreviewView;

	DECLARE_DYNAMIC(CBCGPRibbonBar)

// Construction
public:
	CBCGPRibbonBar(BOOL bReplaceFrameCaption = TRUE);

	BOOL Create(CWnd* pParentWnd,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
			UINT nID = AFX_IDW_RIBBON_BAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_RIBBON_BAR);

// Operations
public:
	virtual void RecalcLayout ();

	//----------------------------------------------------------------------
	// Enable/disable ribbon main button (large rounded button on top left):
	//----------------------------------------------------------------------
	void SetMainButton (CBCGPRibbonMainButton* pButton, CSize sizeButton);

	//--------------------------
	// Ribbon categories (tabs):
	//--------------------------
	CBCGPRibbonMainPanel* AddMainCategory (
		LPCTSTR		lpszName,
		UINT		uiSmallImagesResID,
		UINT		uiLargeImagesResID,
		CSize		sizeSmallImage = CSize (16, 16),
		CSize		sizeLargeImage = CSize (32, 32));

	CBCGPRibbonCategory* AddCategory (
		LPCTSTR		lpszName,
		UINT		uiSmallImagesResID,
		UINT		uiLargeImagesResID,
		CSize		sizeSmallImage = CSize (16, 16),
		CSize		sizeLargeImage = CSize (32, 32),
		int			nInsertAt = -1);

	CBCGPRibbonCategory* AddContextCategory (
		LPCTSTR					lpszName,
		LPCTSTR					lpszContextName,
		UINT					uiContextID,
		BCGPRibbonCategoryColor	clrContext,
		UINT					uiSmallImagesResID,
		UINT					uiLargeImagesResID,
		CSize					sizeSmallImage = CSize (16, 16),
		CSize					sizeLargeImage = CSize (32, 32));

	CBCGPRibbonCategory* AddQATOnlyCategory (
		LPCTSTR					lpszName,
		UINT					uiSmallImagesResID,
		CSize					sizeSmallImage = CSize (16, 16));

	CBCGPRibbonCategory* AddPrintPreviewCategory ();

	int GetCategoryCount () const;
	int GetVisibleCategoryCount () const;
	CBCGPRibbonCategory* GetCategory (int nIndex) const;

	void ShowCategory (int nIndex, BOOL bShow = TRUE);
	void ShowContextCategories (UINT uiContextID, BOOL bShow = TRUE);
	BOOL HideAllContextCategories ();
	BOOL ActivateContextCategory (UINT uiContextID);

	BOOL RemoveCategory (int nIndex);
	void RemoveAllCategories ();

	BOOL SetActiveCategory (CBCGPRibbonCategory* pCategory, BOOL bForceRestore = FALSE);
	CBCGPRibbonCategory* GetActiveCategory () const
	{
		return m_pActiveCategory;
	}

	int FindCategoryIndexByData (DWORD dwData) const;

	BOOL GetContextName (UINT uiContextID, CString& strName) const;

	//-------------------------------
	// Ribbon elements direct access:
	//-------------------------------
	CBCGPBaseRibbonElement* FindByID (UINT uiCmdID, BOOL bVisibleOnly = TRUE) const;
	CBCGPBaseRibbonElement* FindByData (DWORD_PTR dwData, BOOL bVisibleOnly = TRUE) const;

	void GetElementsByID (UINT uiCmdID, 
		CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons);

	void SetQuickAccessDefaultState (const CBCGPRibbonQATDefaultState& state);
	void SetQuickAccessCommands (const CList<UINT,UINT>& lstCommands, BOOL bRecalcLayout = TRUE);
	void GetQuickAccessCommands (CList<UINT,UINT>& lstCommands);

	//--------------------------------------------------
	// Additional elements located on the right of tabs:
	//--------------------------------------------------
	void AddToTabs (CBCGPBaseRibbonElement* pElement);
	void RemoveAllFromTabs ();

	//------------------
	// Tooltips support:
	//------------------
	void EnableToolTips (BOOL bEnable = TRUE, BOOL bEnableDescr = TRUE);
	
	BOOL IsToolTipEnabled () const
	{
		return m_bToolTip;
	}

	BOOL IsToolTipDescrEnabled () const
	{
		return m_bToolTipDescr;
	}

	void GetItemIDsList (CList<UINT,UINT>& lstItems, BOOL bHiddenOnly = FALSE) const;

// Attributes
public:

	int GetCaptionHeight () const
	{
		return m_nCaptionHeight;
	}

	int GetCategoryHeight () const
	{
		return m_nCategoryHeight;
	}

	BOOL IsReplaceFrameCaption () const
	{
		return m_bReplaceFrameCaption;
	}

	CBCGPRibbonMainButton* GetMainButton () const
	{
		return m_pMainButton;
	}

	CBCGPRibbonCategory* GetMainCategory () const
	{
		return m_pMainCategory;
	}

	virtual CBCGPBaseRibbonElement* HitTest (CPoint point, 
		BOOL bCheckActiveCategory = FALSE,
		BOOL bCheckPanelCaption = FALSE);

	//---------------------------------
	// Quick access toolbar attributes:
	//---------------------------------
	void SetQuickAccessToolbarOnTop (BOOL bOnTop);
	BOOL IsQuickAccessToolbarOnTop () const
	{
		return m_bQuickAccessToolbarOnTop && m_bReplaceFrameCaption;
	}

	CRect GetQuickAccessToolbarLocation () const
	{
		return m_QAToolbar.GetRect ();
	}

	CRect GetQATCommandsLocation () const
	{
		return m_QAToolbar.GetCommandsRect ();
	}

	BOOL IsQATEmpty () const
	{
		return (int) m_QAToolbar.m_arButtons.GetSize () <= 1;
	}

	DWORD GetHideFlags () const
	{
		return m_dwHideFlags;
	}

	int GetTabTrancateRatio () const
	{
		return m_nTabTrancateRatio;
	}

	void SetMaximizeMode (BOOL bMax, CWnd* pWnd = NULL);
	void SetActiveMDIChild (CWnd* pWnd);

	virtual CBCGPBaseRibbonElement* GetDroppedDown ();

	BOOL IsTransparentCaption () const
	{
		return m_bIsTransparentCaption;
	}

protected:
	int						m_nTabsHeight;
	int						m_nCategoryHeight;
	int						m_nCategoryMinWidth;
	BOOL					m_bRecalcCategoryHeight;
	BOOL					m_bRecalcCategoryWidth;
	HFONT					m_hFont;
	CBCGPRibbonCategory*	m_pActiveCategory;
	CBCGPRibbonCategory*	m_pActiveCategorySaved;
	int						m_nHighlightedTab;
	
	CArray<CBCGPRibbonCategory*,CBCGPRibbonCategory*> m_arCategories;
	CArray<int,int>			m_arVisibleCategoriesSaved;

	int						m_nCaptionHeight;
	CRect					m_rectCaption;
	CRect					m_rectCaptionText;
	CBCGPRibbonMainButton*	m_pMainButton;
	CBCGPRibbonCategory*	m_pMainCategory;
	CBCGPRibbonCategory*	m_pPrintPreviewCategory;
	CSize					m_sizeMainButton;
	CBCGPBaseRibbonElement*	m_pHighlighted;
	CBCGPBaseRibbonElement*	m_pPressed;
	BOOL					m_bTracked;
	const BOOL				m_bReplaceFrameCaption;
	BOOL					m_bQuickAccessToolbarOnTop;
	DWORD					m_dwHideFlags;
	int						m_nTabTrancateRatio;
	CBCGPRibbonButtonsGroup	m_TabElements;
	CToolTipCtrl*			m_pToolTip;
	BOOL					m_bForceRedraw;
	int						m_nSystemButtonsNum;
	BOOL					m_bMaximizeMode;
	BOOL					m_bAutoCommandTimer;
	BOOL					m_bPrintPreviewMode;
	BOOL					m_bIsTransparentCaption;
	CRect					m_rectSysButtons;
	BOOL					m_bIsMaximized;
	BOOL					m_bToolTip;
	BOOL					m_bToolTipDescr;
	BOOL					m_bIsCustomizeMenu;

	CBCGPRibbonCaptionButton m_CaptionButtons [RIBBON_CAPTION_BUTTONS];

	CArray<CBCGPRibbonContextCaption*, CBCGPRibbonContextCaption*>
							m_arContextCaptions;

	CBCGPRibbonQuickAccessToolbar	m_QAToolbar;

// Overrides
public:
	virtual void OnClickButton (CBCGPRibbonButton* pButton, CPoint point);
	virtual BOOL IsMainRibbonBar () const	{	return TRUE;	}
	virtual BOOL IsShowGroupBorder (CBCGPRibbonButtonsGroup* /*pGroup*/) const	{	return FALSE;	}
	virtual void OnEditContextMenu (CBCGPRibbonEditCtrl* pEdit, CPoint point);

	virtual void DWMCompositionChanged ();

	virtual BOOL OnShowRibbonQATMenu (CWnd* pWnd, int x, int y, CBCGPBaseRibbonElement* pHit);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPRibbonBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void OnUpdateCmdUI (CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual void OnControlBarContextMenu (CWnd* pParentFrame, CPoint point);

protected:
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual BOOL AllowShowOnControlMenu () const			
	{	
		return FALSE;	
	}

	virtual void OnFillBackground (CDC* pDC, CRect rectClient);

	virtual BOOL LoadState (LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1);
	virtual BOOL SaveState (LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1);

	virtual BOOL HideInPrintPreviewMode () const			{	return FALSE;	}

// Implementation
public:
	virtual ~CBCGPRibbonBar();

	void PopTooltip ();
	BOOL DrawMenuImage (CDC* pDC, const CBCGPToolbarMenuButton* pMenuItem, 
						const CRect& rectImage);

	BOOL OnShowRibbonContextMenu (CWnd* pWnd, int x, int y, CBCGPBaseRibbonElement* pHit);

	void ForceRelalcLayout ();

protected:
	void ShowSysMenu (const CPoint& point);

	void SetPrintPreviewMode (BOOL bSet = TRUE);

	CBCGPRibbonContextCaption* FindContextCaption (UINT uiID) const;

	void UpdateToolTipsRect ();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPRibbonBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont (WPARAM, LPARAM);
	afx_msg LRESULT OnGetFont (WPARAM, LPARAM);
	afx_msg LRESULT OnMouseLeave (WPARAM, LPARAM);
	afx_msg LRESULT OnBCGUpdateToolTips (WPARAM, LPARAM);
	afx_msg BOOL OnNeedTipText(UINT id, NMHDR* pNMH, LRESULT* pResult);
	afx_msg LRESULT OnPostRecalcLayout (WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonBar idle update through CBCGPRibbonCmdUI class

class CBCGPRibbonCmdUI : public CCmdUI
{
public:
	CBCGPRibbonCmdUI ();

	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
	virtual void SetRadio(BOOL bOn = TRUE);

	CBCGPBaseRibbonElement*	m_pUpdated;
};

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonMainButton

class BCGCBPRODLLEXPORT CBCGPRibbonMainButton : public CBCGPRibbonButton
{
public:
	CBCGPRibbonMainButton ()
	{
	}

	CBCGPRibbonMainButton (UINT uiBmpResID)
	{
		SetImage (uiBmpResID);
	}

	CBCGPRibbonMainButton (HBITMAP hBmp)
	{
		SetImage (hBmp);
	}

	void SetImage (UINT uiBmpResID);
	void SetImage (HBITMAP hBmp);

protected:
	virtual BOOL IsMainRibbonButton () const
	{
		return TRUE;
	}

	virtual BOOL CanBeAddedToQAT () const
	{
		return FALSE;
	}

	virtual CSize GetImageSize (RibbonImageType /*type*/) const
	{
		ASSERT_VALID (this);
		return m_Image.GetImageSize ();
	}

	virtual void DrawImage (CDC* pDC, RibbonImageType /*type*/, CRect rectImage)
	{
		ASSERT_VALID (this);
		ASSERT_VALID (pDC);

		m_Image.SetTransparentColor (globalData.clrBtnFace);
		m_Image.DrawEx (pDC, rectImage, 0);
	}

	virtual void OnLButtonDown (CPoint point);
	virtual void OnLButtonDblClk (CPoint point);

	CBCGPToolBarImages		m_Image;
};

extern BCGCBPRODLLEXPORT UINT BCGM_ON_CHANGE_RIBBON_CATEGORY;
extern BCGCBPRODLLEXPORT UINT BCGM_ON_RIBBON_CUSTOMIZE;

#endif // BCGP_EXCLUDE_RIBBON

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPRIBBONBAR_H__B3C69A4D_9CDE_406F_9837_2347D306920A__INCLUDED_)
