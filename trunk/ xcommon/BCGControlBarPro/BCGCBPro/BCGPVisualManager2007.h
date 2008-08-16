//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPVisualManager2007.h: interface for the CBCGPVisualManager2007 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGER2007_H__4950864A_0CAE_4A54_8717_59381C1B05B8__INCLUDED_)
#define AFX_BCGPVISUALMANAGER2007_H__4950864A_0CAE_4A54_8717_59381C1B05B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPVisualManager2003.h"
#include "BCGPToolBarImages.h"
#include "BCGPControlRenderer.h"
#include "BCGPToolTipCtrl.h"
#include "bcgglobals.h"

#define BCGPRibbonCategoryColorCount 7

class CBitmapCache
{
public:
	class CBitmapCacheItem
	{
	public:

		CBitmapCacheItem();
		virtual ~CBitmapCacheItem();

		void AddImage (HBITMAP hBmp);
		void Cache (const CSize& size, CBCGPControlRenderer& renderer);

		void Draw (CDC* pDC, CRect rect, int iImageIndex = 0, BYTE alphaSrc = 255);
		void DrawY (CDC* pDC, CRect rect, CSize sides, int iImageIndex = 0, BYTE alphaSrc = 255);

	protected:
		CBCGPToolBarImages m_Images;
	};

	typedef CArray<CBitmapCacheItem*, CBitmapCacheItem*> XCache;
	typedef CArray<CSize, CSize> XSizes;

public:

	CBitmapCache();
	virtual ~CBitmapCache();

	void Clear ();

	int Cache (const CSize& size, CBCGPControlRenderer& renderer);
	int CacheY (int height, CBCGPControlRenderer& renderer);
	BOOL IsCached (const CSize& size) const;

	int FindIndex (const CSize& size) const;

	CBitmapCacheItem* Get (const CSize& size);
	CBitmapCacheItem* Get (int nIndex);

protected:
	XCache	m_Cache;
	XSizes  m_Sizes;
};

class BCGCBPRODLLEXPORT CBCGPVisualManager2007 : public CBCGPVisualManager2003  
{
	DECLARE_DYNCREATE(CBCGPVisualManager2007)

protected:
	CBCGPVisualManager2007();
	virtual ~CBCGPVisualManager2007();

public:

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
	enum Style
	{
		VS2007_LunaBlue,
		VS2007_ObsidianBlack,
		VS2007_Aqua,
		VS2007_Silver
	};

	static BOOL SetStyle (Style style, LPCTSTR lpszPath = NULL);
	static Style GetStyle ();
	static void SetResourceHandle (HINSTANCE hinstRes);

#endif

	static void CleanStyle ();

	virtual void OnUpdateSystemColors ();

	BOOL IsRibbonPresent (CWnd* pWnd) const;
	CBCGPRibbonBar*	GetRibbonBar (CWnd* pWnd) const;

	virtual BOOL IsOwnerDrawCaption ()	{	return CanDrawImage () && !globalData.DwmIsCompositionEnabled ();	}
	virtual BOOL OnNcActivate (CWnd* pWnd, BOOL bActive);
	virtual BOOL OnNcPaint (CWnd* pWnd, const CObList& lstSysButtons, CRect rectRedraw);
	virtual BOOL OnSetWindowRegion (CWnd* pWnd, CSize sizeWindow);

	virtual CSize GetNcBtnSize (BOOL bSmall) const;

public:
	virtual BOOL IsOwnerDrawMenuCheck ();
	virtual BOOL IsHighlightWholeMenuItem ();

	virtual COLORREF GetStatusBarPaneTextColor (CBCGPStatusBar* pStatusBar, 
									CBCGStatusBarPaneInfo* pPane);

	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual COLORREF GetMenuItemTextColor (CBCGPToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled);

	virtual COLORREF GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton);	

public:
	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush,
									CBCGPToolbarButton* pButton);
	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);
	virtual COLORREF OnDrawMenuLabel (CDC* pDC, CRect rect);
	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);

	virtual int GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault);
	virtual void OnDrawShowAllMenuItems (CDC* pDC, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle);
	virtual void OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
				CRect rectSizeBox);

	virtual void OnDrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawComboBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawEditBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsHighlighted,
										CBCGPToolbarEditBoxButton* pButton);
	virtual void OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive);
	virtual void OnDrawMenuResizeBar (CDC* pDC, CRect rect, int nResizeFlags);

	virtual COLORREF OnDrawPropSheetListItem (CDC* pDC, CBCGPPropertySheet* pParent, 
		CRect rect, BOOL bIsHighlihted, BOOL bIsSelected);

	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL bHorz);

	virtual void OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText);
	virtual void OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed);
	virtual void OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton, 
		CRect rect, BOOL bHighlight, BOOL bIsRadio);
	
	virtual void OnDrawMenuItemButton (CDC* pDC, CBCGPToolbarMenuButton* pButton,
				CRect rectButton, BOOL bHighlight, BOOL bDisabled);

	// MiniFrame:
	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd,
											BOOL bActive);
	virtual void OnDrawMiniFrameBorder (CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize);
	virtual void OnDrawFloatingToolbarBorder (	CDC* pDC, CBCGPBaseToolBar* pToolBar, 
												CRect rectBorder, CRect rectBorderSize);

	// tabs
	virtual void GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack);	
	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual BOOL OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnEraseTabsButton (CDC* pDC, CRect rect, CBCGPButton* pButton,
									CBCGPBaseTabWnd* pWndTab);
	virtual void OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
										CBCGPButton* pButton, UINT uiState, CBCGPBaseTabWnd* pWndTab);
	virtual void OnDrawTab (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual BOOL AlwaysHighlight3DTabs () const		{	return CanDrawImage () ? TRUE : CBCGPVisualManager2003::AlwaysHighlight3DTabs ();	}
	virtual COLORREF GetTabTextColor (const CBCGPBaseTabWnd* /*pTabWnd*/, int /*iTab*/, BOOL bIsActive)
	{
		return bIsActive ? m_clrTabTextActive : m_clrTabTextInactive;
	}

	virtual COLORREF GetCaptionBarTextColor (CBCGPCaptionBar* pBar);
	
	// Outlook bar:
	virtual void OnFillOutlookPageButton (CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed);
	virtual void OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter);
	virtual void OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText);

	virtual void GetCalendarColors (const CBCGPCalendar* pCalendar,
				   CBCGPCalendarColors& colors);

	virtual COLORREF GetToolbarDisabledTextColor ()
	{
		return m_clrToolBarBtnTextDisabled;
	}

#ifndef BCGP_EXCLUDE_TASK_PANE
	virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
										BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, 
										BOOL bCanCollapse = FALSE);
	virtual void OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
							BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);

	virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited);
#endif // BCGP_EXCLUDE_TASK_PANE

	virtual void OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
			CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);

	// Grid control:
#ifndef BCGP_EXCLUDE_GRID_CTRL
	virtual void OnFillGridHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual BOOL OnDrawGridHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
	virtual COLORREF OnFillGridGroupByBoxBackground (CDC* pDC, CRect rect);	
	virtual void OnDrawGridGroupByBoxItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual COLORREF GetGridLeftOffsetColor (CBCGPGridCtrl* pCtrl);
	virtual COLORREF OnFillGridRowBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill, BOOL bSelected);
#endif

	virtual void OnDrawCheckBoxEx (CDC *pDC, CRect rect, 
										 int nState,
										 BOOL bHighlighted, 
										 BOOL bPressed,
										 BOOL bEnabled);
	
	// Ribbon control:
#ifndef BCGP_EXCLUDE_RIBBON
	virtual COLORREF OnDrawRibbonTabsFrame (
					CDC* pDC, 
					CBCGPRibbonBar* pWndRibbonBar, 
					CRect rectTab);
	
	virtual void OnDrawRibbonMainButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);

	virtual void OnDrawRibbonCategory (
					CDC* pDC, 
					CBCGPRibbonCategory* pCategory, 
					CRect rectCategory);

	virtual COLORREF OnDrawRibbonCategoryTab (
					CDC* pDC, 
					CBCGPRibbonTab* pTab, 
					BOOL bIsActive);

	virtual COLORREF OnDrawRibbonPanel (
					CDC* pDC,
					CBCGPRibbonPanel* pPanel, 
					CRect rectPanel,
					CRect rectCaption);

	virtual void OnDrawRibbonPanelCaption (
					CDC* pDC,
					CBCGPRibbonPanel* pPanel, 
					CRect rectCaption);

	virtual void OnDrawRibbonLaunchButton (
					CDC* pDC,
					CBCGPRibbonLaunchButton* pButton,
					CBCGPRibbonPanel* pPanel);

	virtual COLORREF OnFillRibbonButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);

	virtual void OnFillRibbonEdit (
					CDC* pDC, 
					CBCGPRibbonEditCtrl* pEdit,
					CRect rect,
					BOOL bIsHighlighted,
					BOOL bIsPaneHighlighted,
					BOOL bIsDisabled,
					COLORREF& clrText, COLORREF& clrSelBackground, COLORREF& clrSelText);

	virtual void OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);

	virtual void OnDrawRibbonDefaultPaneButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);

	virtual void OnDrawRibbonDefaultPaneButtonIndicator (
					CDC* pDC, 
					CBCGPRibbonButton* pButton,
					CRect rect, 
					BOOL bIsSelected, 
					BOOL bHighlighted);

	virtual void OnDrawRibbonCaption (
					CDC* pDC, CBCGPRibbonBar* pBar, CRect rectCaption,
					CRect rectText);

	virtual int GetRibbonQATChevronOffset ()	{	return IsBeta1 () ? 0 : 10;	}

	virtual int GetRibbonQATRightMargin ();

	virtual void OnDrawRibbonCaptionButton (
					CDC* pDC, CBCGPRibbonCaptionButton* pButton);

	virtual COLORREF OnDrawRibbonButtonsGroup (
					CDC* pDC,
					CBCGPRibbonButtonsGroup* pGroup,
					CRect rectGroup);

	virtual void OnDrawDefaultRibbonImage (
					CDC* pDC, 
					CRect rectImage,
					BOOL bIsDisabled = FALSE,
					BOOL bIsPressed = FALSE,
					BOOL bIsHighlighted = FALSE);

	virtual void OnDrawRibbonMainPanelFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel, 
					CRect rect);

	virtual void OnFillRibbonMenuFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel, 
					CRect rect);

	virtual void OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel, 
					CRect rect);

	virtual void OnDrawRibbonLabel (
					CDC* pDC, 
					CBCGPRibbonLabel* pLabel,
					CRect rect);

	virtual COLORREF OnFillRibbonMainPanelButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);

	virtual void OnDrawRibbonMainPanelButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton);
	
	virtual void OnDrawRibbonPaletteButton (
					CDC* pDC,
					CBCGPRibbonPaletteIcon* pButton);

	virtual void OnDrawRibbonPaletteBorder (
					CDC* pDC, 
					CBCGPRibbonPaletteButton* pButton, 
					CRect rectBorder);
	
	virtual COLORREF OnDrawRibbonCategoryCaption (
					CDC* pDC, 
					CBCGPRibbonContextCaption* pContextCaption);

	virtual COLORREF OnDrawRibbonStatusBarPane (
					CDC* pDC, 
					CBCGPRibbonStatusBar* pBar,
					CBCGPRibbonStatusBarPane* pPane);
	
	virtual void OnDrawRibbonSliderZoomButton (
					CDC* pDC, CBCGPRibbonSlider* pSlider, 
					CRect rect, BOOL bIsZoomOut, 
					BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);

	virtual void OnDrawRibbonSliderChannel (
					CDC* pDC, CBCGPRibbonSlider* pSlider, 
					CRect rect);

	virtual void OnDrawRibbonSliderThumb (
					CDC* pDC, CBCGPRibbonSlider* pSlider, 
					CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);

	virtual void OnDrawRibbonProgressBar (
					CDC* pDC, CBCGPRibbonProgressBar* pProgress, 
					CRect rectProgress, CRect rectChunk, BOOL bInfiniteMode);
	
	virtual void OnFillRibbonQATPopup (
				CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar, CRect rect);

	virtual int GetRibbonPopupBorderSize (const CBCGPRibbonPanelMenu* pPopup) const;

#endif

#ifndef BCGP_EXCLUDE_PLANNER
	virtual void OnFillPlanner (CDC* pDC, CBCGPPlannerView* pView, 
		CRect rect, BOOL bWorkingArea);

	virtual COLORREF OnFillPlannerCaption (CDC* pDC, CBCGPPlannerView* pView,
		CRect rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder = FALSE);

	virtual void OnDrawPlannerCaptionText (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, const CString& strText, 
		COLORREF clrText, int nAlign, BOOL bHighlight);

	virtual COLORREF GetPlannerViewBackgroundColor (CBCGPPlannerView* pView);	
	virtual COLORREF GetPlannerHourLineColor (CBCGPPlannerView* pView,
		BOOL bWorkingHours, BOOL bHour);

	virtual COLORREF GetPlannerViewWorkingColor (CBCGPPlannerView* pView);
	virtual COLORREF GetPlannerViewNonWorkingColor (CBCGPPlannerView* pView);

	virtual COLORREF OnFillPlannerTimeBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, COLORREF& clrLine);

	virtual void OnDrawPlannerHeader (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);

	virtual void OnDrawPlannerHeaderPane (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);

	virtual void OnFillPlannerHeaderAllDay (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);
	virtual void OnDrawPlannerHeaderAllDayItem (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected);

	virtual DWORD GetPlannerDrawFlags () const;

	virtual COLORREF GetPlannerSelectionColor (CBCGPPlannerView* pView);
	virtual COLORREF GetPlannerSeparatorColor (CBCGPPlannerView* pView);

	virtual void PreparePlannerBackItem (BOOL bIsToday, BOOL bIsSelected);

#endif // BCGP_EXCLUDE_PLANNER

#ifndef BCGP_EXCLUDE_POPUP_WINDOW
	virtual void OnFillPopupWindowBackground (CDC* pDC, CRect rect);
#endif

	virtual BOOL OnEraseMDIClientArea (CDC* pDC, CRect rectClient);

	// ToolTip
	virtual BOOL GetToolTipParams (CBCGPToolTipParams& params, UINT nType = (UINT)(-1));

protected:
	BOOL CanDrawImage () const
	{
		return globalData.m_nBitsPerPixel > 8 && 
			!globalData.IsHighContastMode () &&
			m_bLoaded;
	}
	
	virtual void CleanUp ();

    void DrawSeparator (CDC* pDC, const CRect& rect, BOOL bHorz);
    void DrawSeparator (CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, BOOL bHorz);
	void DrawNcBtn (CDC* pDC, const CRect& rect, UINT nButton, 
							BCGBUTTON_STATE state, BOOL bSmall, 
							BOOL bActive, BOOL bMDI = FALSE);
	void DrawNcText (CDC* pDC, CRect& rect, const CString& strTitle, 
							const CString& strDocument, BOOL bPrefix, 
							BOOL bActive, BOOL bIsRTL, BOOL bTextCenter);
	void DrawNcCaption (CDC* pDC, CRect rectCaption, 
						   DWORD dwStyle, DWORD dwStyleEx,
						   const CString& strTitle, const CString& strDocument,
						   HICON hIcon, BOOL bPrefix, BOOL bActive, BOOL bTextCenter,
						   const CObList& lstSysButtons);

	BOOL					m_bLoaded;
	
    CSize                   m_szNcBtnSize[2];
	BOOL                    m_bNcTextCenter;

    CBCGPToolBarImages      m_ToolBarGripper;
    CBCGPToolBarImages      m_ToolBarTear;
	CBCGPControlRenderer    m_ctrlToolBarBorder;

    CBCGPControlRenderer    m_ctrlStatusBarBack;
	CBCGPControlRenderer    m_ctrlStatusBarBack_Ext;
    CBCGPToolBarImages      m_StatusBarPaneBorder;
    CBCGPToolBarImages      m_StatusBarSizeBox;

	CBCGPControlRenderer    m_SysBtnBack[2];
	CBCGPToolBarImages      m_SysBtnClose[2];
	CBCGPToolBarImages      m_SysBtnRestore[2];
	CBCGPToolBarImages      m_SysBtnMaximize[2];
	CBCGPToolBarImages      m_SysBtnMinimize[2];

    CFont m_AppCaptionFont;

	COLORREF m_clrAppCaptionActiveStart;
	COLORREF m_clrAppCaptionActiveFinish;
	COLORREF m_clrAppCaptionInactiveStart;
	COLORREF m_clrAppCaptionInactiveFinish;
	COLORREF m_clrAppCaptionActiveText;
	COLORREF m_clrAppCaptionInactiveText;
	COLORREF m_clrAppCaptionActiveTitleText;
	COLORREF m_clrAppCaptionInactiveTitleText;

	COLORREF m_clrMainClientArea;
	CBrush	 m_brMainClientArea;

    COLORREF m_clrMenuBarGradientLight;
    COLORREF m_clrMenuBarGradientDark;
    COLORREF m_clrMenuBarGradientVertLight;
    COLORREF m_clrMenuBarGradientVertDark;

    COLORREF m_clrMenuBarBtnText;
	COLORREF m_clrMenuBarBtnTextHighlighted;
    COLORREF m_clrMenuBarBtnTextDisabled;
    COLORREF m_clrToolBarBtnText;
	COLORREF m_clrToolBarBtnTextHighlighted;
    COLORREF m_clrToolBarBtnTextDisabled;
    COLORREF m_clrMenuText;
    COLORREF m_clrMenuTextHighlighted;
    COLORREF m_clrMenuTextDisabled;

    COLORREF m_clrStatusBarText;
    COLORREF m_clrStatusBarTextDisabled;
	COLORREF m_clrExtenedStatusBarTextDisabled;

	COLORREF m_clrEditBorder;
	COLORREF m_clrEditBorderDisabled;
	COLORREF m_clrEditBorderHighlighted;
	COLORREF m_clrEditSelection;
	COLORREF m_clrComboBorder;
	COLORREF m_clrComboBorderDisabled;
    COLORREF m_clrComboBorderPressed;
	COLORREF m_clrComboBorderHighlighted;
	COLORREF m_clrComboBtnStart;
	COLORREF m_clrComboBtnFinish;
	COLORREF m_clrComboBtnBorder;
	COLORREF m_clrComboBtnDisabledStart;
	COLORREF m_clrComboBtnDisabledFinish;
	COLORREF m_clrComboBtnBorderDisabled;
	COLORREF m_clrComboBtnPressedStart;
	COLORREF m_clrComboBtnPressedFinish;
	COLORREF m_clrComboBtnBorderPressed;
	COLORREF m_clrComboBtnHighlightedStart;
	COLORREF m_clrComboBtnHighlightedFinish;
	COLORREF m_clrComboBtnBorderHighlighted;
	COLORREF m_clrComboSelection;
	CBCGPControlRenderer m_ctrlComboBoxBtn;

	COLORREF m_clrHeaderNormalStart;
	COLORREF m_clrHeaderNormalFinish;
	COLORREF m_clrHeaderNormalBorder;
	COLORREF m_clrHeaderHighlightedStart;
	COLORREF m_clrHeaderHighlightedFinish;
	COLORREF m_clrHeaderHighlightedBorder;
	COLORREF m_clrHeaderPressedStart;
	COLORREF m_clrHeaderPressedFinish;
	COLORREF m_clrHeaderPressedBorder;

    COLORREF m_clrBarCaption;
    COLORREF m_clrMiniFrameCaption;

    COLORREF m_clrSeparator1;
    COLORREF m_clrSeparator2;
    CPen     m_penSeparator2;
	CPen	 m_penSeparatorDark;

	COLORREF m_clrCaptionBarText;

	CBCGPControlRenderer m_ctrlMainBorder;
	CBCGPControlRenderer m_ctrlMainBorderCaption;
	CBCGPControlRenderer m_ctrlPopupBorder;
	CBCGPControlRenderer m_ctrlPopupResizeBar;
	CBCGPToolBarImages   m_PopupResizeBar_HV;
	CBCGPToolBarImages   m_PopupResizeBar_V;
	
	CBCGPControlRenderer m_ctrlMenuBarBtn;

	CBCGPControlRenderer m_ctrlMenuItemBack;
    CBCGPToolBarImages   m_MenuItemMarkerC;
    CBCGPToolBarImages   m_MenuItemMarkerR;
	CBCGPControlRenderer m_ctrlMenuItemShowAll;
	CBCGPControlRenderer m_ctrlMenuHighlighted[2];
	CBCGPControlRenderer m_ctrlMenuButtonBorder;

	CBCGPControlRenderer m_ctrlToolBarBtn;

#ifndef BCGP_EXCLUDE_TASK_PANE
	COLORREF	m_clrTaskPaneGroupCaptionHighDark;
	COLORREF	m_clrTaskPaneGroupCaptionHighLight;
	COLORREF	m_clrTaskPaneGroupCaptionHighSpecDark;
	COLORREF	m_clrTaskPaneGroupCaptionHighSpecLight;

    COLORREF	m_clrTaskPaneGroupCaptionTextSpec;
	COLORREF	m_clrTaskPaneGroupCaptionTextHighSpec;
	COLORREF	m_clrTaskPaneGroupCaptionText;
	COLORREF	m_clrTaskPaneGroupCaptionTextHigh;

	CBCGPControlRenderer m_ctrlTaskScrollBtn;
#endif

	CBCGPControlRenderer	m_ctrlTab3D[2];
	CBCGPControlRenderer	m_ctrlTabFlat[2];
	COLORREF				m_clrTabFlatBlack;
	COLORREF				m_clrTabFlatHighlight;
	CPen					m_penTabFlatInner[2];
	CPen					m_penTabFlatOuter[2];
	COLORREF				m_clrTabTextActive;
	COLORREF				m_clrTabTextInactive;

	CBCGPControlRenderer	m_ctrlOutlookWndBar;
	CBCGPControlRenderer	m_ctrlOutlookWndPageBtn;
	COLORREF				m_clrOutlookPageTextNormal;
	COLORREF				m_clrOutlookPageTextHighlighted;
	COLORREF				m_clrOutlookPageTextPressed;
	COLORREF				m_clrOutlookCaptionTextNormal;


	CBCGPControlRenderer	m_ctrlRibbonCaptionQA;
	CBCGPControlRenderer	m_ctrlRibbonCaptionQA_Glass;
    COLORREF				m_clrRibbonCategoryText;
    COLORREF				m_clrRibbonCategoryTextHighlighted;
	CBCGPControlRenderer	m_ctrlRibbonCategoryBack;
	CBCGPControlRenderer	m_ctrlRibbonCategoryTab;
	CBCGPControlRenderer	m_ctrlRibbonCategoryTabSep;
    COLORREF				m_clrRibbonPanelText;
    COLORREF				m_clrRibbonPanelTextHighlighted;
    COLORREF				m_clrRibbonPanelCaptionText;
    COLORREF				m_clrRibbonPanelCaptionTextHighlighted;
	CBCGPControlRenderer	m_ctrlRibbonPanelBack_T;
	CBCGPControlRenderer	m_ctrlRibbonPanelBack_B;
	CBCGPToolBarImages		m_RibbonPanelSeparator;
	CBCGPControlRenderer	m_ctrlRibbonPanelQAT;
	CBCGPControlRenderer	m_ctrlRibbonMainPanel;
	CBCGPControlRenderer	m_ctrlRibbonMainPanelBorder;
	CBCGPControlRenderer	m_ctrlRibbonBtnMainPanel;

	CBCGPControlRenderer	m_ctrlRibbonBtnGroup_S;
	CBCGPControlRenderer	m_ctrlRibbonBtnGroup_F;
	CBCGPControlRenderer	m_ctrlRibbonBtnGroup_M;
	CBCGPControlRenderer	m_ctrlRibbonBtnGroup_L;
	CBCGPControlRenderer	m_ctrlRibbonBtnGroupMenu_F[2];
	CBCGPControlRenderer	m_ctrlRibbonBtnGroupMenu_M[2];
	CBCGPControlRenderer	m_ctrlRibbonBtnGroupMenu_L[2];
	CBCGPControlRenderer	m_ctrlRibbonBtn[2];
	CBCGPControlRenderer	m_ctrlRibbonBtnMenuH[2];
	CBCGPControlRenderer	m_ctrlRibbonBtnMenuV[2];
	CBCGPControlRenderer	m_ctrlRibbonBtnLaunch;
	CBCGPToolBarImages		m_RibbonBtnLaunchIcon;
	CBCGPControlRenderer	m_RibbonBtnMain;
	CBCGPControlRenderer	m_ctrlRibbonBtnDefault;
	CBCGPControlRenderer	m_ctrlRibbonBtnDefaultIcon;
	CBCGPToolBarImages		m_RibbonBtnDefaultImage;
	CBCGPControlRenderer	m_ctrlRibbonBtnDefaultQATIcon;
	CBCGPControlRenderer	m_ctrlRibbonBtnDefaultQAT;
	CBCGPControlRenderer	m_ctrlRibbonBtnCheck;
	CBCGPControlRenderer	m_ctrlRibbonBtnPalette[3];
	CBCGPControlRenderer	m_ctrlRibbonBtnStatusPane;
	CBCGPControlRenderer	m_ctrlRibbonSliderThumb;
	CBCGPControlRenderer	m_ctrlRibbonSliderBtnPlus;
	CBCGPControlRenderer	m_ctrlRibbonSliderBtnMinus;
	CBCGPControlRenderer	m_ctrlRibbonProgressBack;
	CBCGPControlRenderer	m_ctrlRibbonProgressNormal;
	CBCGPControlRenderer	m_ctrlRibbonProgressNormalExt;
	CBCGPControlRenderer	m_ctrlRibbonProgressInfinity;

	CBCGPControlRenderer	m_ctrlRibbonBorder_QAT;
	CBCGPControlRenderer	m_ctrlRibbonBorder_Floaty;

	COLORREF m_clrRibbonEdit;
	COLORREF m_clrRibbonEditDisabled;
	COLORREF m_clrRibbonEditHighlighted;
	COLORREF m_clrRibbonEditPressed;
	COLORREF m_clrRibbonEditBorder;
	COLORREF m_clrRibbonEditBorderDisabled;
	COLORREF m_clrRibbonEditBorderHighlighted;
	COLORREF m_clrRibbonEditBorderPressed;
	COLORREF m_clrRibbonEditSelection;
	COLORREF m_clrRibbonComboBtnStart;
	COLORREF m_clrRibbonComboBtnFinish;
	COLORREF m_clrRibbonComboBtnBorder;
	COLORREF m_clrRibbonComboBtnDisabledStart;
	COLORREF m_clrRibbonComboBtnDisabledFinish;
	COLORREF m_clrRibbonComboBtnBorderDisabled;
	COLORREF m_clrRibbonComboBtnPressedStart;
	COLORREF m_clrRibbonComboBtnPressedFinish;
	COLORREF m_clrRibbonComboBtnBorderPressed;
	COLORREF m_clrRibbonComboBtnHighlightedStart;
	COLORREF m_clrRibbonComboBtnHighlightedFinish;
	COLORREF m_clrRibbonComboBtnBorderHighlighted;
	CBCGPControlRenderer m_ctrlRibbonComboBoxBtn;

	struct XRibbonContextCategory
	{
		CBCGPControlRenderer	m_ctrlCaption;
		CBCGPControlRenderer	m_ctrlBack;
		CBCGPControlRenderer	m_ctrlTab;
		CBCGPControlRenderer	m_ctrlBtnDefault;
		COLORREF				m_clrCaptionText;
		COLORREF				m_clrText;
		COLORREF				m_clrTextHighlighted;
		CBitmapCache			m_cacheBack;
		CBitmapCache			m_cacheBtnDefault;

		void CleanUp ()
		{
			m_ctrlCaption.CleanUp ();
			m_ctrlBack.CleanUp ();
			m_ctrlTab.CleanUp ();
			m_ctrlBtnDefault.CleanUp ();
			m_cacheBack.Clear ();
			m_cacheBtnDefault.Clear ();
		}
	};

	CBCGPControlRenderer	m_ctrlRibbonContextPanelBack_T;
	CBCGPControlRenderer	m_ctrlRibbonContextPanelBack_B;
    COLORREF				m_clrRibbonContextPanelText;
    COLORREF				m_clrRibbonContextPanelTextHighlighted;
    COLORREF				m_clrRibbonContextPanelCaptionText;
    COLORREF				m_clrRibbonContextPanelCaptionTextHighlighted;
	CBitmapCache			m_cacheRibbonContextPanelBack_T;
	CBitmapCache			m_cacheRibbonContextPanelBack_B;
	CBCGPControlRenderer	m_ctrlRibbonContextSeparator;
	XRibbonContextCategory  m_ctrlRibbonContextCategory[BCGPRibbonCategoryColorCount];

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_

	static HINSTANCE		m_hinstRes;
	static BOOL				m_bAutoFreeRes;
	static Style			m_Style;

#endif

	CBitmapCache			m_cacheRibbonCategoryBack;
	CBitmapCache			m_cacheRibbonPanelBack_T;
	CBitmapCache			m_cacheRibbonPanelBack_B;
	CBitmapCache			m_cacheRibbonBtnDefault;

	CBitmapCache			m_cacheRibbonBtnGroup_S;
	CBitmapCache			m_cacheRibbonBtnGroup_F;
	CBitmapCache			m_cacheRibbonBtnGroup_M;
	CBitmapCache			m_cacheRibbonBtnGroup_L;
	CBitmapCache			m_cacheRibbonBtnGroupMenu_F[2];
	CBitmapCache			m_cacheRibbonBtnGroupMenu_M[2];
	CBitmapCache			m_cacheRibbonBtnGroupMenu_L[2];

	int 					m_nType;
	BOOL					m_bPlannerBlack;

	COLORREF				m_clrPlannerTodayCaption[4];
	COLORREF				m_clrPlannerTodayBorder;
	COLORREF				m_clrPlannerNcArea;
	COLORREF				m_clrPlannerNcLine;
	COLORREF				m_clrPlannerNcText;

	COLORREF				m_clrGridHeaderNormalStart;
	COLORREF				m_clrGridHeaderNormalFinish;
	COLORREF				m_clrGridHeaderNormalBorder;
	COLORREF				m_clrGridHeaderPressedStart;
	COLORREF				m_clrGridHeaderPressedFinish;
	COLORREF				m_clrGridHeaderPressedBorder;
	COLORREF				m_clrGridSeparator;
	CPen					m_penGridSeparator;
	COLORREF				m_clrGridLeftOffset;
	COLORREF				m_clrGridGroupLine;
	COLORREF				m_clrGridGroupSubLine;

	COLORREF				m_clrPopupGradientLight;
	COLORREF				m_clrPopupGradientDark;

	CBCGPToolTipParams		m_ToolTipParams;
	BOOL					m_bToolTipParams;

	BOOL					IsBeta1 () const {	return m_nType == 1;	}

	CMap<HWND, HWND, BOOL, BOOL>
							m_ActivateFlag;
	
	BOOL	IsWindowActive (CWnd* pWnd) const;
	CSize	GetSystemBorders (BOOL bRibbonPresent) const;
};

#endif // !defined(AFX_BCGPVISUALMANAGER2007_H__4950864A_0CAE_4A54_8717_59381C1B05B8__INCLUDED_)
