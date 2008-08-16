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
// BCGPVisualManager2007.cpp: implementation of the CBCGPVisualManager2007 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManager2007.h"
#include "BCGPToolBar.h"
#include "BCGPDrawManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPMenuBar.h"
#include "bcgglobals.h"
#include "BCGPToolbarMenuButton.h"
#include "CustomizeButton.h"
#include "MenuImages.h"
#include "BCGPCaptionBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "BCGPTabWnd.h"
#include "BCGPTasksPane.h"
#include "BCGPStatusBar.h"
#include "BCGPAutoHideButton.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPReBar.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPCalendarBar.h"
#include "BCGPDropDown.h"
#include "BCGPTagManager.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPOutlookWnd.h"
#include "BCGPGridCtrl.h"
#include "BCGPToolbarComboBoxButton.h"

#ifndef BCGP_EXCULDE_RIBBON
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonProgressBar.h"
#endif

#include "Styles/Style_res.h"

#ifndef BCGP_EXCLUDE_PLANNER
#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewMonth.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_

const CBCGPVisualManager2007::Style c_StyleDefault = 
	CBCGPVisualManager2007::VS2007_LunaBlue;

HINSTANCE CBCGPVisualManager2007::m_hinstRes = NULL;
BOOL CBCGPVisualManager2007::m_bAutoFreeRes = FALSE;
CBCGPVisualManager2007::Style CBCGPVisualManager2007::m_Style = c_StyleDefault;

#endif

CBitmapCache::CBitmapCacheItem::CBitmapCacheItem()
{
}

CBitmapCache::CBitmapCacheItem::~CBitmapCacheItem()
{
}

void CBitmapCache::CBitmapCacheItem::AddImage (HBITMAP hBmp)
{
	m_Images.AddImage (hBmp, TRUE);
}

void CBitmapCache::CBitmapCacheItem::Cache (const CSize& size, CBCGPControlRenderer& renderer)
{
	m_Images.Clear ();

	const int nCount = renderer.GetImageCount ();

	if (nCount > 0)
	{
		m_Images.SetImageSize (size);
		m_Images.SetTransparentColor ((COLORREF)-1);

		for (int i = 0; i < nCount; i++)
		{
			BITMAPINFO bi;
			memset(&bi, 0, sizeof(BITMAPINFO));

			// Fill in the BITMAPINFOHEADER
			bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth       = size.cx;
			bi.bmiHeader.biHeight      = size.cy;
			bi.bmiHeader.biPlanes      = 1;
			bi.bmiHeader.biBitCount    = 32;
			bi.bmiHeader.biCompression = BI_RGB;
			bi.bmiHeader.biSizeImage   = size.cy * size.cx * 4;

			LPBYTE pBits = NULL;
			HBITMAP hDib = ::CreateDIBSection (
				NULL, &bi, DIB_RGB_COLORS, (void **)&pBits,
				NULL, NULL);

			if (hDib == NULL || pBits == NULL)
			{
				ASSERT (FALSE);
				break;
			}

			CDC dc;
			dc.CreateCompatibleDC (NULL);

			HBITMAP hOldDib = (HBITMAP)::SelectObject (dc.GetSafeHdc (), hDib);

			renderer.Draw (&dc, CRect (0, 0, size.cx, size.cy), i);

			::SelectObject (dc.GetSafeHdc (), hOldDib);

			AddImage (hDib);

			::DeleteObject (hDib);
		}
	}
}

void CBitmapCache::CBitmapCacheItem::Draw (CDC* pDC, CRect rect, int iImageIndex/* = 0*/,
										   BYTE alphaSrc/* = 255*/)
{
	m_Images.DrawEx (pDC, CRect (rect.TopLeft (), m_Images.GetImageSize ()), 
		iImageIndex, CBCGPToolBarImages::ImageAlignHorzLeft, 
		CBCGPToolBarImages::ImageAlignVertTop, CRect (0, 0, 0, 0), alphaSrc);
}

void CBitmapCache::CBitmapCacheItem::DrawY (CDC* pDC, CRect rect, CSize sides, 
											int iImageIndex/* = 0*/, BYTE alphaSrc/* = 255*/)
{
	CRect rectImage (CPoint (0, 0), m_Images.GetImageSize ());

	ASSERT (rect.Height () == rectImage.Height ());

	if (sides.cx > 0)
	{
		CRect rt (rectImage);
		rt.right = rt.left + sides.cx;

		rectImage.left = rt.right;

		m_Images.DrawEx (pDC, rect, iImageIndex, CBCGPToolBarImages::ImageAlignHorzLeft,
			CBCGPToolBarImages::ImageAlignVertTop, rt, alphaSrc);
	}

	if (sides.cy > 0)
	{
		CRect rt (rectImage);
		rt.left = rt.right - sides.cy;

		rectImage.right = rt.left;

		m_Images.DrawEx (pDC, rect, iImageIndex, CBCGPToolBarImages::ImageAlignHorzRight,
			CBCGPToolBarImages::ImageAlignVertTop, rt, alphaSrc);
	}

	if (rectImage.Width () > 0)
	{
		rect.DeflateRect (sides.cx, 0, sides.cy, 0);
		m_Images.DrawEx (pDC, rect, iImageIndex, CBCGPToolBarImages::ImageAlignHorzStretch,
			CBCGPToolBarImages::ImageAlignVertTop, rectImage, alphaSrc);
	}
}

CBitmapCache::CBitmapCache()
{
}

CBitmapCache::~CBitmapCache()
{
	Clear ();
}

void CBitmapCache::Clear ()
{
	for (int i = 0; i < m_Cache.GetSize (); i++)
	{
		if (m_Cache[i] != NULL)
		{
			delete m_Cache[i];
		}
	}

	m_Cache.RemoveAll ();
	m_Sizes.RemoveAll ();
}

int CBitmapCache::Cache (const CSize& size, CBCGPControlRenderer& renderer)
{
	if (FindIndex (size) != -1)
	{
		ASSERT (FALSE);
		return -1;
	}

	CBitmapCacheItem* pItem = new CBitmapCacheItem;
	pItem->Cache (size, renderer);

	int nCache = (int) m_Cache.Add (pItem);
	int nSize  = (int) m_Sizes.Add (size);

	ASSERT (nCache == nSize);

	return nCache;
}

int CBitmapCache::CacheY (int height, CBCGPControlRenderer& renderer)
{
	CSize size (renderer.GetParams ().m_rectImage.Width (), height);

	return Cache (size, renderer);
}

BOOL CBitmapCache::IsCached (const CSize& size) const
{
	return FindIndex (size) != -1;
}

int CBitmapCache::FindIndex (const CSize& size) const
{
	int nRes = -1;
	for (int i = 0; i < m_Sizes.GetSize (); i++)
	{
		if (size == m_Sizes[i])
		{
			nRes = i;
			break;
		}
	}

	return nRes;
}

CBitmapCache::CBitmapCacheItem* CBitmapCache::Get (const CSize& size)
{
	int nIndex = FindIndex (size);

	if (nIndex != -1)
	{
		return m_Cache[nIndex];
	}

	return NULL;
}

CBitmapCache::CBitmapCacheItem* CBitmapCache::Get (int nIndex)
{
	if (0 <= nIndex && nIndex < m_Cache.GetSize ())
	{
		return m_Cache[nIndex];
	}

	return NULL;
}


IMPLEMENT_DYNCREATE(CBCGPVisualManager2007, CBCGPVisualManager2003)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManager2007::CBCGPVisualManager2007()
	: m_bNcTextCenter   (FALSE)
	, m_bLoaded         (FALSE)
	, m_bPlannerBlack          (FALSE)
{
	m_szNcBtnSize[0]    = CSize (0, 0);
	m_szNcBtnSize[1]    = CSize (0, 0);
}
//*****************************************************************************
CBCGPVisualManager2007::~CBCGPVisualManager2007()
{
	CMenuImages::SetColor (CMenuImages::ImageBlack, (COLORREF)-1);
}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_

BOOL CBCGPVisualManager2007::SetStyle (Style style, LPCTSTR lpszPath)
{
	if (m_Style == style && m_hinstRes > (HINSTANCE) 32)
	{
		return TRUE;
	}

	CString strStyleDLLName;
	CString strStyleDLLPath;

	switch (style)
	{
	case VS2007_LunaBlue:
		strStyleDLLName = _T("Luna.dll");
		break;

	case VS2007_ObsidianBlack:
		strStyleDLLName = _T("Obsidian.dll");
		break;

	case VS2007_Aqua:
		strStyleDLLName = _T("Aqua.dll");
		break;
		
	case VS2007_Silver:
		strStyleDLLName = _T("Silver.dll");
		break;

	default:
		ASSERT (FALSE);
		return FALSE;
	}

	strStyleDLLName = _T("BCGPStyle2007") + strStyleDLLName;

	if (lpszPath != NULL && lstrlen (lpszPath) > 0)
	{
		strStyleDLLPath = lpszPath;

		if (strStyleDLLPath [strStyleDLLPath.GetLength () - 1] != _T('\\'))
		{
			strStyleDLLPath += _T('\\');
		}

		strStyleDLLPath += strStyleDLLName;
	}
	else
	{
		strStyleDLLPath = strStyleDLLName;
	}

	Style oldStyle = m_Style;

	CleanStyle ();

	HINSTANCE hinstRes = LoadLibrary (strStyleDLLPath);

	if (hinstRes <= (HINSTANCE) 32)
	{
		m_Style = oldStyle;

		TRACE(_T("Cannot load Style DLL: %s\r\n"), strStyleDLLPath);
		ASSERT (FALSE);
		return FALSE;
	}

	m_Style = style;

	SetResourceHandle (hinstRes);
	m_bAutoFreeRes = TRUE;

	return TRUE;
}
//*****************************************************************************
CBCGPVisualManager2007::Style CBCGPVisualManager2007::GetStyle ()
{
	return m_Style;
}
//*****************************************************************************
void CBCGPVisualManager2007::SetResourceHandle (HINSTANCE hinstRes)
{
	m_bAutoFreeRes = FALSE;

	if (m_hinstRes != hinstRes)
	{
		m_hinstRes = hinstRes;

		if (CBCGPVisualManager::GetInstance ()->IsKindOf (
			RUNTIME_CLASS (CBCGPVisualManager2007)))
		{
			CBCGPVisualManager::GetInstance ()->OnUpdateSystemColors ();
		}
	}
}

#endif

//*****************************************************************************
void CBCGPVisualManager2007::CleanStyle ()
{
#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_

	if (m_bAutoFreeRes && m_hinstRes > (HINSTANCE) 32)
	{
		::FreeLibrary (m_hinstRes);
		m_hinstRes = NULL;
	}

	m_Style = c_StyleDefault;

#endif
}
//*****************************************************************************
void CBCGPVisualManager2007::CleanUp ()
{
	m_clrEditBorder                = (COLORREF)(-1);
	m_clrEditBorderDisabled        = (COLORREF)(-1);
	m_clrEditBorderHighlighted     = (COLORREF)(-1);
	m_clrEditSelection             = (COLORREF)(-1);
	m_clrComboBorder               = (COLORREF)(-1);
	m_clrComboBorderDisabled       = (COLORREF)(-1);
    m_clrComboBorderPressed        = (COLORREF)(-1);
	m_clrComboBorderHighlighted    = (COLORREF)(-1);
	m_clrComboBtnStart             = (COLORREF)(-1);
	m_clrComboBtnFinish            = (COLORREF)(-1);
	m_clrComboBtnBorder            = (COLORREF)(-1);
	m_clrComboBtnDisabledStart     = (COLORREF)(-1);
	m_clrComboBtnDisabledFinish    = (COLORREF)(-1);
	m_clrComboBtnBorderDisabled    = (COLORREF)(-1);
	m_clrComboBtnPressedStart      = (COLORREF)(-1);
	m_clrComboBtnPressedFinish     = (COLORREF)(-1);
	m_clrComboBtnBorderPressed     = (COLORREF)(-1);
	m_clrComboBtnHighlightedStart  = (COLORREF)(-1);
	m_clrComboBtnHighlightedFinish = (COLORREF)(-1);
	m_clrComboBtnBorderHighlighted = (COLORREF)(-1);
	m_clrComboSelection            = (COLORREF)(-1);
	m_ctrlComboBoxBtn.CleanUp ();

    m_ToolBarGripper.Clear ();
    m_ToolBarTear.Clear ();
	m_ctrlToolBarBorder.CleanUp ();

	m_ctrlStatusBarBack.CleanUp ();
	m_ctrlStatusBarBack_Ext.CleanUp ();
    m_StatusBarPaneBorder.Clear ();
    m_StatusBarSizeBox.Clear ();

	m_SysBtnBack[0].CleanUp ();
	m_SysBtnBack[1].CleanUp ();
	m_SysBtnClose[0].Clear ();
	m_SysBtnClose[1].Clear ();
	m_SysBtnRestore[0].Clear ();
	m_SysBtnRestore[1].Clear ();
	m_SysBtnMaximize[0].Clear ();
	m_SysBtnMaximize[1].Clear ();
	m_SysBtnMinimize[0].Clear ();
	m_SysBtnMinimize[1].Clear ();

	m_brMainClientArea.DeleteObject ();

	m_AppCaptionFont.DeleteObject ();
	m_penSeparator2.DeleteObject ();

	m_penSeparatorDark.DeleteObject ();

	m_ctrlMainBorder.CleanUp ();
	m_ctrlMainBorderCaption.CleanUp ();
	m_ctrlPopupBorder.CleanUp ();
	m_ctrlPopupResizeBar.CleanUp ();
	m_PopupResizeBar_HV.Clear ();
	m_PopupResizeBar_V.Clear ();

	m_ctrlMenuBarBtn.CleanUp ();

	m_ctrlMenuItemBack.CleanUp ();
    m_MenuItemMarkerC.Clear ();
    m_MenuItemMarkerR.Clear ();
	m_ctrlMenuItemShowAll.CleanUp ();
	m_ctrlMenuHighlighted[0].CleanUp ();
	m_ctrlMenuHighlighted[1].CleanUp ();

	m_ctrlToolBarBtn.CleanUp ();
	
#ifndef BCGP_EXCLUDE_TASK_PANE
	m_ctrlTaskScrollBtn.CleanUp ();
#endif

	m_ctrlTab3D[0].CleanUp ();
	m_ctrlTab3D[1].CleanUp ();
	m_ctrlTabFlat[0].CleanUp ();
	m_ctrlTabFlat[1].CleanUp ();
	m_clrTabFlatBlack = CLR_DEFAULT;
	m_clrTabTextActive = CLR_DEFAULT;
	m_clrTabTextInactive = CLR_DEFAULT;
	m_clrTabFlatHighlight = CLR_DEFAULT;
	m_penTabFlatInner[0].DeleteObject ();
	m_penTabFlatInner[1].DeleteObject ();
	m_penTabFlatOuter[0].DeleteObject ();
	m_penTabFlatOuter[1].DeleteObject ();

	m_ctrlOutlookWndBar.CleanUp ();
	m_ctrlOutlookWndPageBtn.CleanUp ();

	m_ctrlRibbonCaptionQA.CleanUp ();
	m_ctrlRibbonCaptionQA_Glass.CleanUp ();
	m_ctrlRibbonCategoryBack.CleanUp ();
	m_ctrlRibbonCategoryTab.CleanUp ();
	m_ctrlRibbonCategoryTabSep.CleanUp ();
	m_ctrlRibbonPanelBack_T.CleanUp ();
	m_ctrlRibbonPanelBack_B.CleanUp ();
	m_RibbonPanelSeparator.Clear ();
	m_ctrlRibbonPanelQAT.CleanUp ();
	m_ctrlRibbonMainPanel.CleanUp ();
	m_ctrlRibbonMainPanelBorder.CleanUp ();
	m_ctrlRibbonBtnMainPanel.CleanUp ();

	m_ctrlRibbonBtnGroup_S.CleanUp ();
	m_ctrlRibbonBtnGroup_F.CleanUp ();
	m_ctrlRibbonBtnGroup_M.CleanUp ();
	m_ctrlRibbonBtnGroup_L.CleanUp ();
	m_ctrlRibbonBtnGroupMenu_F[0].CleanUp ();
	m_ctrlRibbonBtnGroupMenu_F[1].CleanUp ();
	m_ctrlRibbonBtnGroupMenu_M[0].CleanUp ();
	m_ctrlRibbonBtnGroupMenu_M[1].CleanUp ();
	m_ctrlRibbonBtnGroupMenu_L[0].CleanUp ();
	m_ctrlRibbonBtnGroupMenu_L[1].CleanUp ();
	m_ctrlRibbonBtn[0].CleanUp ();
	m_ctrlRibbonBtn[1].CleanUp ();
	m_ctrlRibbonBtnMenuH[0].CleanUp ();
	m_ctrlRibbonBtnMenuH[1].CleanUp ();
	m_ctrlRibbonBtnMenuV[0].CleanUp ();
	m_ctrlRibbonBtnMenuV[1].CleanUp ();
	m_ctrlRibbonBtnLaunch.CleanUp ();
	m_RibbonBtnLaunchIcon.Clear ();
	m_RibbonBtnMain.CleanUp ();
	m_ctrlRibbonBtnDefault.CleanUp ();
	m_ctrlRibbonBtnDefaultIcon.CleanUp ();
	m_RibbonBtnDefaultImage.Clear ();
	m_ctrlRibbonBtnDefaultQATIcon.CleanUp ();
	m_ctrlRibbonBtnDefaultQAT.CleanUp ();
	m_ctrlRibbonBtnCheck.CleanUp ();
	m_ctrlRibbonBtnPalette[0].CleanUp ();
	m_ctrlRibbonBtnPalette[1].CleanUp ();
	m_ctrlRibbonBtnPalette[2].CleanUp ();
	m_ctrlRibbonBtnStatusPane.CleanUp ();
	m_ctrlRibbonSliderThumb.CleanUp ();
	m_ctrlRibbonSliderBtnPlus.CleanUp ();
	m_ctrlRibbonSliderBtnMinus.CleanUp ();
	m_ctrlRibbonProgressBack.CleanUp ();
	m_ctrlRibbonProgressNormal.CleanUp ();
	m_ctrlRibbonProgressNormalExt.CleanUp ();
	m_ctrlRibbonProgressInfinity.CleanUp ();
	m_ctrlRibbonBorder_QAT.CleanUp ();
	m_ctrlRibbonBorder_Floaty.CleanUp ();

	m_ctrlRibbonComboBoxBtn.CleanUp ();

	m_cacheRibbonCategoryBack.Clear ();
	m_cacheRibbonPanelBack_T.Clear ();
	m_cacheRibbonPanelBack_B.Clear ();
	m_cacheRibbonBtnDefault.Clear ();

	m_cacheRibbonBtnGroup_S.Clear ();
	m_cacheRibbonBtnGroup_F.Clear ();
	m_cacheRibbonBtnGroup_M.Clear ();
	m_cacheRibbonBtnGroup_L.Clear ();
	m_cacheRibbonBtnGroupMenu_F[0].Clear ();
	m_cacheRibbonBtnGroupMenu_M[0].Clear ();
	m_cacheRibbonBtnGroupMenu_L[0].Clear ();
	m_cacheRibbonBtnGroupMenu_F[1].Clear ();
	m_cacheRibbonBtnGroupMenu_M[1].Clear ();
	m_cacheRibbonBtnGroupMenu_L[1].Clear ();

	m_ctrlRibbonContextPanelBack_T.CleanUp ();
	m_ctrlRibbonContextPanelBack_B.CleanUp ();
	m_cacheRibbonContextPanelBack_T.Clear ();
	m_cacheRibbonContextPanelBack_B.Clear ();
	m_ctrlRibbonContextSeparator.CleanUp ();

	for (int i = 0; i < BCGPRibbonCategoryColorCount; i++)
	{
		m_ctrlRibbonContextCategory[i].CleanUp ();
	}

	m_clrPlannerTodayCaption[0] = RGB (247, 208, 112);
	m_clrPlannerTodayCaption[1] = RGB (251, 230, 148);
	m_clrPlannerTodayCaption[2] = RGB (239, 155,  30);
	m_clrPlannerTodayCaption[3] = RGB (250, 224, 139);
	m_clrPlannerTodayBorder     = RGB (238, 147,  17);
	m_clrPlannerNcArea          = globalData.clrBtnFace;
	m_clrPlannerNcLine          = globalData.clrBtnShadow;
	m_clrPlannerNcText          = globalData.clrBtnText;

	m_clrCaptionBarText         = globalData.clrWindow;

	m_penGridSeparator.DeleteObject ();
	m_penGridSeparator.CreatePen (PS_SOLID, 1, globalData.clrBarShadow);

	m_clrGridLeftOffset = globalData.clrBarFace;

	m_bToolTipParams = FALSE;
	CBCGPToolTipParams dummy;
	m_ToolTipParams = dummy;

	m_ActivateFlag.RemoveAll ();

	m_bPlannerBlack = FALSE;

	m_bLoaded = FALSE;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnUpdateSystemColors ()
{
	CleanUp ();

	CBCGPVisualManager2003::OnUpdateSystemColors ();

	if (globalData.bIsWindows9x)
	{
		return;
	}

	if (!globalData.bIsOSAlphaBlendingSupport ||
		globalData.IsHighContastMode () ||
		globalData.m_nBitsPerPixel <= 8)
	{
		return;
	}
	
	m_nMenuBorderSize = 1;

	HINSTANCE hinstResOld = NULL;

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
	
	if (m_hinstRes == NULL)
	{
		SetStyle (c_StyleDefault);
	}

	if (m_hinstRes != NULL)
	{
		hinstResOld = AfxGetResourceHandle ();
		AfxSetResourceHandle (m_hinstRes);
	}

#endif

	CBCGPTagManager tm;

	if (!tm.LoadFromResource (IDX_OFFICE2007_STYLE, _T("STYLE_XML")))
	{

#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
		TRACE(_T("\r\nImportant: to enable Office 2007 look in static link, you need:\r\n"));
		TRACE(_T("1. Open \"Resource Includes\" dialog and add style.rc file:\r\n"));
		TRACE(_T("<BCGCBPro-Path>\\Styles\\Office 2007 (luna - blue)\\Style.rc\r\n"));
		TRACE(_T("2. Add path to this folder to \"Additional Resource Include Directories\"\r\n\r\n"));
		ASSERT (FALSE);
#endif
		return;
	}

	{
		CString strStyle;
		tm.ExcludeTag (_T("STYLE"), strStyle);
		tm.SetBuffer (strStyle);
	}

	CString strItem;

	m_nType = 2;
	
	if (!tm.IsEmpty ())
	{
		int nVersion = 0;

		if (tm.ExcludeTag (_T("VERSION"), strItem))
		{
			CBCGPTagManager tmItem (strItem);

			tmItem.ReadInt (_T("NUMBER"), nVersion);

			if (nVersion == 2007)
			{
				tmItem.ReadInt (_T("TYPE"), m_nType);

				m_bLoaded = TRUE;
			}
		}
	}

	if (!m_bLoaded)
	{
		if (hinstResOld != NULL)
		{
			::AfxSetResourceHandle (hinstResOld);
		}

		return;
	}

	// globals
	if (tm.ExcludeTag (_T("GLOBALS"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("BarText"), globalData.clrBarText);

		if (tmItem.ReadColor (_T("BarFace"), globalData.clrBarFace))
		{
			globalData.brBarFace.DeleteObject ();
			globalData.brBarFace.CreateSolidBrush (globalData.clrBarFace);
			m_clrMenuShadowBase = globalData.clrBarFace;
		}
		if (tmItem.ReadColor (_T("ActiveCaption"), globalData.clrActiveCaption))
		{
			globalData.clrInactiveCaption     = globalData.clrActiveCaption;
			globalData.brActiveCaption.DeleteObject ();
			globalData.brActiveCaption.CreateSolidBrush (globalData.clrActiveCaption);
		}
		if (tmItem.ReadColor (_T("CaptionText"), globalData.clrCaptionText))
		{
			globalData.clrInactiveCaptionText = globalData.clrCaptionText;
		}

		tmItem.ReadColor (_T("InactiveCaption"), globalData.clrInactiveCaption);
		globalData.brInactiveCaption.DeleteObject ();
		globalData.brInactiveCaption.CreateSolidBrush (globalData.clrInactiveCaption);
		tmItem.ReadColor (_T("InactiveCaptionText"), globalData.clrInactiveCaptionText);

		tmItem.ReadColor (_T("BarShadow"), globalData.clrBarShadow);
		tmItem.ReadColor (_T("BarDkShadow"), globalData.clrBarDkShadow);
		tmItem.ReadColor (_T("BarLight"), globalData.clrBarLight);

		
		COLORREF clrFloatToolBarBorder;
		tmItem.ReadColor (_T("FloatToolBarBorder"), clrFloatToolBarBorder);
		m_brFloatToolBarBorder.DeleteObject ();
		m_brFloatToolBarBorder.CreateSolidBrush (clrFloatToolBarBorder);

		tmItem.ReadColor (_T("HighlightGradientDark"), m_clrHighlightGradientDark);
		tmItem.ReadColor (_T("HighlightGradientLight"), m_clrHighlightGradientLight);

		m_clrHighlightDnGradientDark = m_clrHighlightGradientLight;
		m_clrHighlightDnGradientLight = m_clrHighlightGradientDark;
		tmItem.ReadColor (_T("HighlightDnGradientDark"), m_clrHighlightDnGradientDark);
		tmItem.ReadColor (_T("HighlightDnGradientLight"), m_clrHighlightDnGradientLight);

		m_clrHighlightCheckedGradientDark = m_clrHighlightDnGradientLight;
		m_clrHighlightCheckedGradientLight = m_clrHighlightDnGradientDark;
		tmItem.ReadColor (_T("HighlightCheckedGradientDark"), m_clrHighlightCheckedGradientDark);
		tmItem.ReadColor (_T("HighlightCheckedGradientLight"), m_clrHighlightCheckedGradientLight);

		tmItem.ReadColor (_T("PressedButtonBorder"), m_clrPressedButtonBorder);

		COLORREF clrHB = globalData.clrHilite;
		COLORREF clrHT = globalData.clrTextHilite;
		if (tmItem.ReadColor (_T("Highlight"), clrHB) &&
			tmItem.ReadColor (_T("HighlightText"), clrHT))
		{
			globalData.clrHilite = clrHB;

			globalData.brHilite.DeleteObject ();
			globalData.brHilite.CreateSolidBrush (clrHB);

			globalData.clrTextHilite = clrHT;
		}

		tmItem.ReadColor (_T("MenuShadowColor"), m_clrMenuShadowBase);

		// ToolTipParams
		m_bToolTipParams = tmItem.ReadToolTipParams (_T("TOOLTIP"), m_ToolTipParams);
	}

	// mainwnd
	if (tm.ExcludeTag (_T("MAINWND"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		// caption
		CString strCaption;
		if (tmItem.ExcludeTag (_T("CAPTION"), strCaption))
		{
			CBCGPTagManager tmCaption (strCaption);

			NONCLIENTMETRICS ncm;
			ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
			ncm.cbSize = sizeof(NONCLIENTMETRICS);

			if (::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0))
			{
				tmCaption.ReadFont (_T("FONT"), ncm.lfCaptionFont);
				m_AppCaptionFont.DeleteObject ();
				m_AppCaptionFont.CreateFontIndirect (&ncm.lfCaptionFont);
			}

			tmCaption.ReadColor (_T("ActiveStart"), m_clrAppCaptionActiveStart);
			tmCaption.ReadColor (_T("ActiveFinish"), m_clrAppCaptionActiveFinish);
			tmCaption.ReadColor (_T("InactiveStart"), m_clrAppCaptionInactiveStart);
			tmCaption.ReadColor (_T("InactiveFinish"), m_clrAppCaptionInactiveFinish);
			tmCaption.ReadColor (_T("ActiveText"), m_clrAppCaptionActiveText);
			tmCaption.ReadColor (_T("InactiveText"), m_clrAppCaptionInactiveText);
			tmCaption.ReadColor (_T("ActiveTitleText"), m_clrAppCaptionActiveTitleText);
			tmCaption.ReadColor (_T("InactiveTitleText"), m_clrAppCaptionInactiveTitleText);

			tmCaption.ReadBool (_T("TextCenter"), m_bNcTextCenter);

			tmCaption.ReadControlRenderer (_T("BORDER"), m_ctrlMainBorderCaption, IDB_OFFICE2007_MAINBORDER_CAPTION);

			m_szNcBtnSize[0] = CSize (::GetSystemMetrics (SM_CXSIZE),
									  ::GetSystemMetrics (SM_CYSIZE));
			m_szNcBtnSize[1] = CSize (::GetSystemMetrics (SM_CXSMSIZE),
									  ::GetSystemMetrics (SM_CYSMSIZE));

			// buttons
			CString strButtons;
			if (tmCaption.ExcludeTag (_T("BUTTONS"), strButtons))
			{
				CBCGPTagManager tmButtons (strButtons);

				for (int i = 0; i < 2; i++)
				{
					CString str;
					if (tmButtons.ExcludeTag (i == 0 ? _T("NORMAL") : _T("SMALL"), str))
					{
						CBCGPTagManager tmBtn (str);

						tmBtn.ReadSize (_T("ConstSize"), m_szNcBtnSize[i]);

						CSize sizeIcon (0, 0);
						if (tmBtn.ReadSize (_T("IconSize"), sizeIcon))
						{
							m_SysBtnClose[i].Clear ();
							m_SysBtnClose[i].SetImageSize (sizeIcon);
							m_SysBtnClose[i].Load (IDB_OFFICE2007_SYS_BTN_CLOSE + i);
							
							m_SysBtnRestore[i].Clear ();
							m_SysBtnRestore[i].SetImageSize (sizeIcon);
							m_SysBtnRestore[i].Load (IDB_OFFICE2007_SYS_BTN_RESTORE + i);

							m_SysBtnMaximize[i].Clear ();
							m_SysBtnMaximize[i].SetImageSize (sizeIcon);
							m_SysBtnMaximize[i].Load (IDB_OFFICE2007_SYS_BTN_MAXIMIZE + i);

							m_SysBtnMinimize[i].Clear ();
							m_SysBtnMinimize[i].SetImageSize (sizeIcon);
							m_SysBtnMinimize[i].Load (IDB_OFFICE2007_SYS_BTN_MINIMIZE + i);
						}

						CBCGPTagManager::ParseControlRenderer (tmBtn.GetBuffer (), 
							m_SysBtnBack[i], IDB_OFFICE2007_SYS_BTN_BACK);
					}
				}
			}
		}

		// border
		tmItem.ReadControlRenderer (_T("BORDER"), m_ctrlMainBorder, IDB_OFFICE2007_MAINBORDER);
		
		if (tmItem.ReadColor (_T("MainClientArea"), m_clrMainClientArea))
		{
			m_brMainClientArea.DeleteObject ();
			m_brMainClientArea.CreateSolidBrush (m_clrMainClientArea);
		}
	}

	// menu
	if (tm.ExcludeTag (_T("MENU"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		if (tmItem.ReadColor (_T("Light"), m_clrMenuLight))
		{
			m_brMenuLight.DeleteObject ();
			m_brMenuLight.CreateSolidBrush (m_clrMenuLight);
		}

		m_clrMenuRarelyUsed = CLR_DEFAULT;
		tmItem.ReadColor (_T("Rarely"), m_clrMenuRarelyUsed);

		if (tmItem.ReadColor (_T("Separator1"), m_clrSeparator1))
		{
			m_penSeparator.DeleteObject ();
			m_penSeparator.CreatePen (PS_SOLID, 1, m_clrSeparator1);
		}

		if (tmItem.ReadColor (_T("Separator2"), m_clrSeparator2))
		{
			m_penSeparator2.DeleteObject ();
			m_penSeparator2.CreatePen (PS_SOLID, 1, m_clrSeparator2);
		}

		if (tmItem.ReadColor (_T("ItemBorder"), m_clrMenuItemBorder))
		{
			m_penMenuItemBorder.DeleteObject ();
			m_penMenuItemBorder.CreatePen (PS_SOLID, 1, m_clrMenuItemBorder);
		}

		tmItem.ReadInt (_T("BorderSize"), m_nMenuBorderSize);

		tmItem.ReadControlRenderer (_T("ItemBack"), m_ctrlMenuItemBack, IDB_OFFICE2007_MENU_ITEM_BACK);
		tmItem.ReadToolBarImages (_T("ItemCheck"), m_MenuItemMarkerC, IDB_OFFICE2007_MENU_ITEM_MARKER_C);
		tmItem.ReadToolBarImages (_T("ItemRadio"), m_MenuItemMarkerR, IDB_OFFICE2007_MENU_ITEM_MARKER_R);
		tmItem.ReadControlRenderer (_T("ItemShowAll"), m_ctrlMenuItemShowAll, IDB_OFFICE2007_MENU_ITEM_SHOWALL);
		tmItem.ReadControlRenderer (_T("Highlighted"), m_ctrlMenuHighlighted[0], IDB_OFFICE2007_MENU_BTN);
		tmItem.ReadControlRenderer (_T("HighlightedDisabled"), m_ctrlMenuHighlighted[1], IDB_OFFICE2007_MENU_BTN_DISABLED);
		tmItem.ReadControlRenderer (_T("ButtonBorder"), m_ctrlMenuButtonBorder, IDB_OFFICE2007_MENU_BTN_VERT_SEPARATOR);

		tmItem.ReadColor (_T("TextNormal"), m_clrMenuText);
		tmItem.ReadColor (_T("TextHighlighted"), m_clrMenuTextHighlighted);
		tmItem.ReadColor (_T("TextDisabled"), m_clrMenuTextDisabled);

		COLORREF clrImages = m_clrMenuText;
		tmItem.ReadColor (_T("ImagesColor"), clrImages);

		CMenuImages::SetColor (CMenuImages::ImageBlack, clrImages);
	}

	// bars
	if (tm.ExcludeTag (_T("BARS"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		CString strBar;
		if (tmItem.ExcludeTag (_T("DEFAULT"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			if (tmBar.ReadColor (_T("Bkgnd"), m_clrBarBkgnd))
			{
				m_brBarBkgnd.DeleteObject ();
				m_brBarBkgnd.CreateSolidBrush  (m_clrBarBkgnd);
			}

			tmBar.ReadColor (_T("GradientLight"), m_clrBarGradientLight);
			m_clrBarGradientDark = m_clrBarGradientLight;
			tmBar.ReadColor (_T("GradientDark"), m_clrBarGradientDark);
		}

		if (tmItem.ExcludeTag (_T("TOOLBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			m_clrToolBarGradientLight = m_clrBarGradientLight;
			m_clrToolBarGradientDark  = m_clrBarGradientDark;

			m_clrToolbarDisabled = CBCGPDrawManager::SmartMixColors (
				m_clrToolBarGradientDark, m_clrToolBarGradientLight);

			tmBar.ReadColor (_T("GradientLight"), m_clrToolBarGradientLight);
			tmBar.ReadColor (_T("GradientDark"), m_clrToolBarGradientDark);

			m_clrToolBarGradientVertLight = m_clrToolBarGradientLight;
			m_clrToolBarGradientVertDark  = m_clrToolBarGradientDark;

			tmBar.ReadColor (_T("GradientVertLight"), m_clrToolBarGradientVertLight);
			tmBar.ReadColor (_T("GradientVertDark"), m_clrToolBarGradientVertDark);

			tmBar.ReadColor (_T("CustomizeButtonGradientLight"), m_clrCustomizeButtonGradientLight);
			tmBar.ReadColor (_T("CustomizeButtonGradientDark"), m_clrCustomizeButtonGradientDark);

			tmBar.ReadToolBarImages (_T("GRIPPER"), m_ToolBarGripper, IDB_OFFICE2007_GRIPPER);
			tmBar.ReadToolBarImages (_T("TEAR"), m_ToolBarTear, IDB_OFFICE2007_TEAR);

			tmBar.ReadControlRenderer (_T("BUTTON"), m_ctrlToolBarBtn, IDB_OFFICE2007_TOOLBAR_BTN);
			tmBar.ReadControlRenderer (_T("BORDER"), m_ctrlToolBarBorder, IDB_OFFICE2007_TOOLBAR_BORDER);

			m_clrToolBarBtnText            = globalData.clrBarText;
			m_clrToolBarBtnTextHighlighted = m_clrToolBarBtnText;
			tmBar.ReadColor (_T("TextNormal"), m_clrToolBarBtnText);
			tmBar.ReadColor (_T("TextHighlighted"), m_clrToolBarBtnTextHighlighted);
			tmBar.ReadColor (_T("TextDisabled"), m_clrToolBarBtnTextDisabled);

			if (tmBar.ReadColor (_T("BottomLineColor"), m_clrToolBarBottomLine))
			{
				m_penBottomLine.DeleteObject ();
				m_penBottomLine.CreatePen (PS_SOLID, 1, m_clrToolBarBottomLine);
			}

			m_penSeparatorDark.DeleteObject ();
			m_penSeparatorDark.CreatePen (PS_SOLID, 1, 
				CBCGPDrawManager::PixelAlpha (m_clrToolBarBottomLine, RGB (255, 255, 255), 95));

			m_penSeparatorLight.DeleteObject ();
			m_penSeparatorLight.CreatePen (PS_SOLID, 1, RGB (255, 255, 255));
		}

		if (tmItem.ExcludeTag (_T("MENUBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			m_clrMenuBarGradientLight = m_clrToolBarGradientLight;
			m_clrMenuBarGradientDark  = m_clrToolBarGradientDark;

			tmBar.ReadColor (_T("GradientLight"), m_clrMenuBarGradientLight);
			tmBar.ReadColor (_T("GradientDark"), m_clrMenuBarGradientDark);

			m_clrMenuBarGradientVertLight = m_clrMenuBarGradientLight;
			m_clrMenuBarGradientVertDark  = m_clrMenuBarGradientDark;

			tmBar.ReadColor (_T("GradientVertLight"), m_clrMenuBarGradientVertLight);
			tmBar.ReadColor (_T("GradientVertDark"), m_clrMenuBarGradientVertDark);

			m_clrMenuBarBtnText            = m_clrToolBarBtnText;
			m_clrMenuBarBtnTextHighlighted = m_clrToolBarBtnTextHighlighted;
			m_clrMenuBarBtnTextDisabled    = m_clrToolBarBtnTextDisabled;
			tmBar.ReadColor (_T("TextNormal"), m_clrMenuBarBtnText);
			tmBar.ReadColor (_T("TextHighlighted"), m_clrMenuBarBtnTextHighlighted);
			tmBar.ReadColor (_T("TextDisabled"), m_clrMenuBarBtnTextDisabled);

			tmBar.ReadControlRenderer (_T("BUTTON"), m_ctrlMenuBarBtn, IDB_OFFICE2007_MENUBAR_BTN);
		}

		if (tmItem.ExcludeTag (_T("POPUPBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);
			tmBar.ReadControlRenderer (_T("BORDER"), m_ctrlPopupBorder, IDB_OFFICE2007_POPUPMENU_BORDER);

			CString strResize;
			if (tmBar.ExcludeTag (_T("RESIZEBAR"), strResize))
			{
				CBCGPTagManager tmResize (strResize);
				tmResize.ReadControlRenderer (_T("BACK"), m_ctrlPopupResizeBar, IDB_OFFICE2007_POPUPMENU_RESIZEBAR);
				tmResize.ReadToolBarImages (_T("ICON_HV"), m_PopupResizeBar_HV, IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_HV);
				tmResize.ReadToolBarImages (_T("ICON_V"), m_PopupResizeBar_V, IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_V);
			}
		}

		if (tmItem.ExcludeTag (_T("STATUSBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			tmBar.ReadControlRenderer (_T("BACK"), m_ctrlStatusBarBack, IDB_OFFICE2007_STATUSBAR_BACK);
			tmBar.ReadControlRenderer (_T("BACK_EXT"), m_ctrlStatusBarBack_Ext, IDB_OFFICE2007_STATUSBAR_BACK_EXT);

			tmBar.ReadToolBarImages (_T("PANEBORDER"), m_StatusBarPaneBorder, IDB_OFFICE2007_STATUSBAR_PANEBORDER);
			tmBar.ReadToolBarImages (_T("SIZEBOX"), m_StatusBarSizeBox, IDB_OFFICE2007_STATUSBAR_SIZEBOX);

			m_clrStatusBarText         = m_clrMenuBarBtnText;
			m_clrStatusBarTextDisabled = m_clrMenuBarBtnTextDisabled;
			m_clrExtenedStatusBarTextDisabled = m_clrMenuBarBtnTextDisabled;

			tmBar.ReadColor (_T("TextNormal"), m_clrStatusBarText);
			tmBar.ReadColor (_T("TextDisabled"), m_clrStatusBarTextDisabled);
			tmBar.ReadColor (_T("TextExtendedDisabled"), m_clrExtenedStatusBarTextDisabled);
		}
		
		if (tmItem.ExcludeTag (_T("CAPTIONBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			tmBar.ReadColor (_T("GradientLight"), m_clrCaptionBarGradientLight);
			tmBar.ReadColor (_T("GradientDark"), m_clrCaptionBarGradientDark);
			tmBar.ReadColor (_T("TextNormal"), m_clrCaptionBarText);
		}
	}

	if (m_clrMenuRarelyUsed == CLR_DEFAULT)
	{
		m_clrMenuRarelyUsed = m_clrBarBkgnd;
	}

	m_brMenuRarelyUsed.DeleteObject ();
	m_brMenuRarelyUsed.CreateSolidBrush (m_clrMenuRarelyUsed);

	m_clrEditBorder            = globalData.clrWindow;
	m_clrEditBorderDisabled    = globalData.clrBtnShadow;
	m_clrEditBorderHighlighted = m_clrMenuItemBorder;
	m_clrEditSelection         = globalData.clrHilite;

	// edit
	if (tm.ExcludeTag (_T("EDIT"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("BorderNormal"), m_clrEditBorder);
		tmItem.ReadColor (_T("BorderHighlighted"), m_clrEditBorderHighlighted);
		tmItem.ReadColor (_T("BorderDisabled"), m_clrEditBorderDisabled);
		tmItem.ReadColor (_T("Selection"), m_clrEditSelection);
	}

	m_clrComboBorder               = globalData.clrWindow;
	m_clrComboBorderDisabled       = globalData.clrBtnShadow;
	m_clrComboBorderHighlighted    = m_clrMenuItemBorder;
	m_clrComboBorderPressed        = m_clrComboBorderHighlighted;
	m_clrComboBtnBorder            = m_clrComboBorder;
	m_clrComboBtnBorderHighlighted = m_clrComboBorderHighlighted;
	m_clrComboBtnBorderPressed     = m_clrComboBorderHighlighted;
	m_clrComboSelection            = globalData.clrHilite;
	m_clrComboBtnStart             = m_clrToolBarGradientDark;
	m_clrComboBtnFinish            = m_clrToolBarGradientLight;
	m_clrComboBtnDisabledStart     = globalData.clrBtnFace;
	m_clrComboBtnDisabledFinish    = m_clrComboBtnDisabledStart;
	m_clrComboBtnHighlightedStart  = m_clrHighlightGradientDark;
	m_clrComboBtnHighlightedFinish = m_clrHighlightGradientLight;
	m_clrComboBtnPressedStart      = m_clrHighlightDnGradientDark;
	m_clrComboBtnPressedFinish     = m_clrHighlightDnGradientLight;

	// combobox
	if (tm.ExcludeTag (_T("COMBO"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("BorderNormal"), m_clrComboBorder);
		tmItem.ReadColor (_T("BorderHighlighted"), m_clrComboBorderHighlighted);
		tmItem.ReadColor (_T("BorderDisabled"), m_clrComboBorderDisabled);

		m_clrComboBorderPressed = m_clrComboBorderHighlighted;
		tmItem.ReadColor (_T("BorderPressed"), m_clrComboBorderPressed);

		tmItem.ReadColor (_T("Selection"), m_clrComboSelection);

		CString strButton;
		if (tmItem.ExcludeTag (_T("BUTTON"), strButton))
		{
			CBCGPTagManager tmButton (strButton);

			tmButton.ReadColor (_T("GradientStartNormal"), m_clrComboBtnStart);
			tmButton.ReadColor (_T("GradientFinishNormal"), m_clrComboBtnFinish);
			tmButton.ReadColor (_T("BtnBorderNormal"), m_clrComboBtnBorder);

			if (!tmButton.ReadControlRenderer (_T("IMAGE"), m_ctrlComboBoxBtn, IDB_OFFICE2007_COMBOBOX_BTN))
			{
				tmButton.ReadColor (_T("GradientStartHighlighted"), m_clrComboBtnHighlightedStart);
				tmButton.ReadColor (_T("GradientFinishHighlighted"), m_clrComboBtnHighlightedFinish);
				tmButton.ReadColor (_T("GradientStartDisabled"), m_clrComboBtnDisabledStart);
				tmButton.ReadColor (_T("GradientFinishDisabled"), m_clrComboBtnDisabledFinish);
				tmButton.ReadColor (_T("GradientStartPressed"), m_clrComboBtnPressedStart);
				tmButton.ReadColor (_T("GradientFinishPressed"), m_clrComboBtnPressedFinish);

				tmButton.ReadColor (_T("BtnBorderHighlighted"), m_clrComboBtnBorderHighlighted);
				tmButton.ReadColor (_T("BtnBorderDisabled"), m_clrComboBtnBorderDisabled);

				m_clrComboBtnBorderPressed = m_clrComboBtnBorderHighlighted;
				tmButton.ReadColor (_T("BtnBorderPressed"), m_clrComboBtnBorderPressed);
			}
		}
	}

	m_clrRibbonEditBorder            = m_clrEditBorder;
	m_clrRibbonEditBorderDisabled    = m_clrEditBorderDisabled;
	m_clrRibbonEditBorderHighlighted = m_clrEditBorderHighlighted;
	m_clrRibbonEditBorderPressed     = m_clrRibbonEditBorderHighlighted;
	m_clrRibbonEditSelection         = m_clrEditSelection;

	m_clrRibbonComboBtnBorder            = m_clrComboBtnBorder;
	m_clrRibbonComboBtnBorderHighlighted = m_clrComboBtnBorderHighlighted;
	m_clrRibbonComboBtnBorderPressed     = m_clrComboBtnBorderPressed;
	m_clrRibbonComboBtnStart             = m_clrComboBtnStart;
	m_clrRibbonComboBtnFinish            = m_clrComboBtnFinish;
	m_clrRibbonComboBtnDisabledStart     = m_clrComboBtnDisabledStart;
	m_clrRibbonComboBtnDisabledFinish    = m_clrComboBtnDisabledFinish;
	m_clrRibbonComboBtnHighlightedStart  = m_clrComboBtnHighlightedStart;
	m_clrRibbonComboBtnHighlightedFinish = m_clrComboBtnHighlightedFinish;
	m_clrRibbonComboBtnPressedStart      = m_clrComboBtnPressedStart;
	m_clrRibbonComboBtnPressedFinish     = m_clrComboBtnPressedFinish;

	// task pane
#ifndef BCGP_EXCLUDE_TASK_PANE
	m_clrTaskPaneGradientDark       = m_clrBarGradientLight;
	m_clrTaskPaneGradientLight      = m_clrTaskPaneGradientDark;
	
	if (tm.ExcludeTag (_T("TASK"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("GradientDark"), m_clrTaskPaneGradientDark);
		tmItem.ReadColor (_T("GradientLight"), m_clrTaskPaneGradientLight);

		CString strGroup;
		if (tmItem.ExcludeTag (_T("GROUP"), strGroup))
		{
			CBCGPTagManager tmGroup (strGroup);

			CString strState;
			if (tmGroup.ExcludeTag (_T("NORMAL"), strState))
			{
				CBCGPTagManager tmState (strState);

				CString str;

				if (tmState.ExcludeTag (_T("CAPTION"), str))
				{
					CBCGPTagManager tmCaption (str);

					tmCaption.ReadColor (_T("DarkNormal"), m_clrTaskPaneGroupCaptionDark);
					tmCaption.ReadColor (_T("LightNormal"), m_clrTaskPaneGroupCaptionLight);
					tmCaption.ReadColor (_T("DarkHighlighted"), m_clrTaskPaneGroupCaptionHighDark);
					tmCaption.ReadColor (_T("LightHighlighted"), m_clrTaskPaneGroupCaptionHighLight);
					tmCaption.ReadColor (_T("TextNormal"), m_clrTaskPaneGroupCaptionText);
					tmCaption.ReadColor (_T("TextHighlighted"), m_clrTaskPaneGroupCaptionTextHigh);
				}

				if (tmState.ExcludeTag (_T("AREA"), str))
				{
					CBCGPTagManager tmArea (str);

					tmArea.ReadColor (_T("DarkNormal"), m_clrTaskPaneGroupAreaDark);
					tmArea.ReadColor (_T("LightNormal"), m_clrTaskPaneGroupAreaLight);
				}
			}

			if (tmGroup.ExcludeTag (_T("SPECIAL"), strState))
			{
				CBCGPTagManager tmState (strState);

				CString str;
				if (tmState.ExcludeTag (_T("CAPTION"), str))
				{
					CBCGPTagManager tmCaption (str);

					tmCaption.ReadColor (_T("DarkNormal"), m_clrTaskPaneGroupCaptionSpecDark);
					tmCaption.ReadColor (_T("LightNormal"), m_clrTaskPaneGroupCaptionSpecLight);
					tmCaption.ReadColor (_T("DarkHighlighted"), m_clrTaskPaneGroupCaptionHighSpecDark);
					tmCaption.ReadColor (_T("LightHighlighted"), m_clrTaskPaneGroupCaptionHighSpecLight);
					tmCaption.ReadColor (_T("TextNormal"), m_clrTaskPaneGroupCaptionTextSpec);
					tmCaption.ReadColor (_T("TextHighlighted"), m_clrTaskPaneGroupCaptionTextHighSpec);
				}

				if (tmState.ExcludeTag (_T("AREA"), str))
				{
					CBCGPTagManager tmArea (str);

					tmArea.ReadColor (_T("DarkNormal"), m_clrTaskPaneGroupAreaSpecDark);
					tmArea.ReadColor (_T("LightNormal"), m_clrTaskPaneGroupAreaSpecLight);
				}
			}

			if (tmGroup.ReadColor (_T("BORDER"), m_clrTaskPaneGroupBorder))
			{
				m_penTaskPaneGroupBorder.DeleteObject ();
				m_penTaskPaneGroupBorder.CreatePen (PS_SOLID, 1, m_clrTaskPaneGroupBorder);
			}
		}

		tmItem.ReadControlRenderer (_T("SCROLL_BUTTON"), m_ctrlTaskScrollBtn, IDB_OFFICE2007_TASKPANE_SCROLL_BTN);
	}
#endif

	if (tm.ExcludeTag (_T("TABS"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("TextColorActive"), m_clrTabTextActive);
		tmItem.ReadColor (_T("TextColorInactive"), m_clrTabTextInactive);

		CString strTab;
		if (tmItem.ExcludeTag (_T("3D"), strTab))
		{
			CBCGPTagManager tmTab (strTab);

			CString strBtn;
			if (tmTab.ExcludeTag (_T("BUTTON"), strBtn))
			{
				CBCGPControlRendererParams params (IDB_OFFICE2007_TAB_3D, CRect (0, 0, 0, 0), CRect (0, 0, 0, 0));
				if (CBCGPTagManager::ParseControlRendererParams (strBtn, params))
				{
					m_ctrlTab3D[0].Create (params);
					m_ctrlTab3D[1].Create (params, TRUE);
				}
			}
		}

		if (tmItem.ExcludeTag (_T("FLAT"), strTab))
		{
			CBCGPTagManager tmTab (strTab);

			CString strBtn;
			if (tmTab.ExcludeTag (_T("BUTTON"), strBtn))
			{
				CBCGPControlRendererParams params (IDB_OFFICE2007_TAB_FLAT, CRect (0, 0, 0, 0), CRect (0, 0, 0, 0));
				if (CBCGPTagManager::ParseControlRendererParams (strBtn, params))
				{
					m_ctrlTabFlat[0].Create (params);
					m_ctrlTabFlat[1].Create (params, TRUE);
				}
			}

			tmTab.ReadColor (_T("Black"), m_clrTabFlatBlack);
			tmTab.ReadColor (_T("Highlight"), m_clrTabFlatHighlight);

			COLORREF clr;
			if (tmTab.ReadColor (_T("BorderInnerNormal"), clr))
			{
				m_penTabFlatInner[0].DeleteObject ();
				m_penTabFlatInner[0].CreatePen (PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor (_T("BorderInnerActive"), clr))
			{
				m_penTabFlatInner[1].DeleteObject ();
				m_penTabFlatInner[1].CreatePen (PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor (_T("BorderOuterNormal"), clr))
			{
				m_penTabFlatOuter[0].DeleteObject ();
				m_penTabFlatOuter[0].CreatePen (PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor (_T("BorderOuterActive"), clr))
			{
				m_penTabFlatOuter[1].DeleteObject ();
				m_penTabFlatOuter[1].CreatePen (PS_SOLID, 1, clr);
			}
		}
	}

	if (tm.ExcludeTag (_T("HEADER"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("NormalStart"), m_clrHeaderNormalStart);
		tmItem.ReadColor (_T("NormalFinish"), m_clrHeaderNormalFinish);
		tmItem.ReadColor (_T("NormalBorder"), m_clrHeaderNormalBorder);
		tmItem.ReadColor (_T("HighlightedStart"), m_clrHeaderHighlightedStart);
		tmItem.ReadColor (_T("HighlightedFinish"), m_clrHeaderHighlightedFinish);
		tmItem.ReadColor (_T("HighlightedBorder"), m_clrHeaderHighlightedBorder);
		tmItem.ReadColor (_T("PressedStart"), m_clrHeaderPressedStart);
		tmItem.ReadColor (_T("PressedFinish"), m_clrHeaderPressedFinish);
		tmItem.ReadColor (_T("PressedBorder"), m_clrHeaderPressedBorder);
	}	

	m_clrRibbonCategoryText                = m_clrMenuBarBtnText;
	m_clrRibbonCategoryTextHighlighted     = m_clrMenuBarBtnTextHighlighted;
	m_clrRibbonPanelText                   = m_clrToolBarBtnText;
	m_clrRibbonPanelTextHighlighted        = m_clrToolBarBtnTextHighlighted;
	m_clrRibbonPanelCaptionText            = m_clrRibbonPanelText;
	m_clrRibbonPanelCaptionTextHighlighted = m_clrRibbonPanelTextHighlighted;

	m_clrRibbonEdit                        = globalData.clrBarLight;
	m_clrRibbonEditHighlighted             = globalData.clrWindow;
	m_clrRibbonEditPressed                 = m_clrRibbonEditHighlighted;
	m_clrRibbonEditDisabled                = globalData.clrBtnFace;

	if (tm.ExcludeTag (_T("RIBBON"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		CString str;

		if (tmItem.ExcludeTag (_T("CATEGORY"), str))
		{
			CBCGPTagManager tmCategory (str);
			tmCategory.ReadControlRenderer(_T("BACK"), m_ctrlRibbonCategoryBack, IDB_OFFICE2007_RIBBON_CATEGORY_BACK);

			CString strTab;
			if (tmCategory.ExcludeTag (_T("TAB"), strTab))
			{
				CBCGPTagManager tmTab (strTab);
				tmTab.ReadControlRenderer(_T("BUTTON"), m_ctrlRibbonCategoryTab, IDB_OFFICE2007_RIBBON_CATEGORY_TAB);

				tmTab.ReadColor (_T("TextNormal"), m_clrRibbonCategoryText);
				tmTab.ReadColor (_T("TextHighlighted"), m_clrRibbonCategoryTextHighlighted);
			}

			tmCategory.ReadControlRenderer (_T("TAB_SEPARATOR"), m_ctrlRibbonCategoryTabSep, IDB_OFFICE2007_RIBBON_CATEGORY_TAB_SEP);
		}

		if (tmItem.ExcludeTag (_T("PANEL"), str))
		{
			CBCGPTagManager tmPanel (str);

			{
				CString strBack;
				if (tmPanel.ExcludeTag (_T("BACK"), strBack))
				{
					CBCGPTagManager tmBack (strBack);

					tmBack.ReadControlRenderer(_T("TOP"), m_ctrlRibbonPanelBack_T, IDB_OFFICE2007_RIBBON_PANEL_BACK_T);
					tmBack.ReadControlRenderer(_T("BOTTOM"), m_ctrlRibbonPanelBack_B, IDB_OFFICE2007_RIBBON_PANEL_BACK_B);
				}
			}

			{
				CString strCaption;
				if (tmPanel.ExcludeTag (_T("CAPTION"), strCaption))
				{
					CBCGPTagManager tmCaption (strCaption);

					tmCaption.ReadControlRenderer (_T("LAUNCH_BTN"), m_ctrlRibbonBtnLaunch, IDB_OFFICE2007_RIBBON_BTN_LAUNCH);
					tmCaption.ReadToolBarImages (_T("LAUNCH_ICON"), m_RibbonBtnLaunchIcon, IDB_OFFICE2007_RIBBON_BTN_LAUNCH_ICON);
					tmCaption.ReadColor (_T("TextNormal"), m_clrRibbonPanelCaptionText);
					tmCaption.ReadColor (_T("TextHighlighted"), m_clrRibbonPanelCaptionTextHighlighted);
				}
			}

			m_RibbonPanelSeparator.SetPreMultiplyAutoCheck ();
			tmPanel.ReadToolBarImages (_T("SEPARATOR"), m_RibbonPanelSeparator, IDB_OFFICE2007_RIBBON_PANEL_SEPARATOR);

			tmPanel.ReadControlRenderer (_T("QAT"), m_ctrlRibbonPanelQAT, IDB_OFFICE2007_RIBBON_PANEL_QAT);

			{
				CString strButtons;
				if (tmPanel.ExcludeTag (_T("BUTTONS"), strButtons))
				{
					CBCGPTagManager tmButtons (strButtons);

					tmButtons.ReadControlRenderer (_T("BUTTON_GROUP_F"), m_ctrlRibbonBtnGroup_F, IDB_OFFICE2007_RIBBON_BTN_GROUP_F);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUP_M"), m_ctrlRibbonBtnGroup_M, IDB_OFFICE2007_RIBBON_BTN_GROUP_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUP_L"), m_ctrlRibbonBtnGroup_L, IDB_OFFICE2007_RIBBON_BTN_GROUP_L);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUP_S"), m_ctrlRibbonBtnGroup_S, IDB_OFFICE2007_RIBBON_BTN_GROUP_S);

					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_F_C"), m_ctrlRibbonBtnGroupMenu_F[0], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_C);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_F_M"), m_ctrlRibbonBtnGroupMenu_F[1], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_M_C"), m_ctrlRibbonBtnGroupMenu_M[0], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_M_C);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_M_M"), m_ctrlRibbonBtnGroupMenu_M[1], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_M_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_L_C"), m_ctrlRibbonBtnGroupMenu_L[0], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_C);
					tmButtons.ReadControlRenderer (_T("BUTTON_GROUPMENU_L_M"), m_ctrlRibbonBtnGroupMenu_L[1], IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_M);

					tmButtons.ReadControlRenderer (_T("BUTTON_NORMAL_S"), m_ctrlRibbonBtn[0], IDB_OFFICE2007_RIBBON_BTN_NORMAL_S);
					tmButtons.ReadControlRenderer (_T("BUTTON_NORMAL_B"), m_ctrlRibbonBtn[1], IDB_OFFICE2007_RIBBON_BTN_NORMAL_B);

					tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT"), m_ctrlRibbonBtnDefault, IDB_OFFICE2007_RIBBON_BTN_DEFAULT);
					tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT_ICON"), m_ctrlRibbonBtnDefaultIcon, IDB_OFFICE2007_RIBBON_BTN_DEFAULT_ICON);
					m_RibbonBtnDefaultImage.SetPreMultiplyAutoCheck ();
					tmButtons.ReadToolBarImages (_T("BUTTON_DEFAULT_IMAGE"), m_RibbonBtnDefaultImage, IDB_OFFICE2007_RIBBON_BTN_DEFAULT_IMAGE);
					tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT_QAT"), m_ctrlRibbonBtnDefaultQAT, IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT);

					if (!m_ctrlRibbonBtnDefaultQAT.IsValid ())
					{
						tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT_QAT_ICON"), m_ctrlRibbonBtnDefaultQATIcon, IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT_ICON);
					}

					tmButtons.ReadControlRenderer (_T("BUTTON_MENU_H_C"), m_ctrlRibbonBtnMenuH[0], IDB_OFFICE2007_RIBBON_BTN_MENU_H_C);
					tmButtons.ReadControlRenderer (_T("BUTTON_MENU_H_M"), m_ctrlRibbonBtnMenuH[1], IDB_OFFICE2007_RIBBON_BTN_MENU_H_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_MENU_V_C"), m_ctrlRibbonBtnMenuV[0], IDB_OFFICE2007_RIBBON_BTN_MENU_V_C);
					tmButtons.ReadControlRenderer (_T("BUTTON_MENU_V_M"), m_ctrlRibbonBtnMenuV[1], IDB_OFFICE2007_RIBBON_BTN_MENU_V_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_CHECK"), m_ctrlRibbonBtnCheck, IDB_OFFICE2007_RIBBON_BTN_CHECK);

					tmButtons.ReadControlRenderer (_T("BUTTON_PNL_T"), m_ctrlRibbonBtnPalette[0], IDB_OFFICE2007_RIBBON_BTN_PALETTE_T);
					tmButtons.ReadControlRenderer (_T("BUTTON_PNL_M"), m_ctrlRibbonBtnPalette[1], IDB_OFFICE2007_RIBBON_BTN_PALETTE_M);
					tmButtons.ReadControlRenderer (_T("BUTTON_PNL_B"), m_ctrlRibbonBtnPalette[2], IDB_OFFICE2007_RIBBON_BTN_PALETTE_B);
				}
			}

			{
				CString strEdit;
				if (tmPanel.ExcludeTag (_T("EDIT"), strEdit))
				{
					CBCGPTagManager tmEdit (strEdit);

					tmEdit.ReadColor (_T("Normal"), m_clrRibbonEdit);
					tmEdit.ReadColor (_T("Highlighted"), m_clrRibbonEditHighlighted);
					tmEdit.ReadColor (_T("Disabled"), m_clrRibbonEditDisabled);
					tmEdit.ReadColor (_T("Pressed"), m_clrRibbonEditPressed);

					tmEdit.ReadColor (_T("BorderNormal"), m_clrRibbonEditBorder);
					tmEdit.ReadColor (_T("BorderHighlighted"), m_clrRibbonEditBorderHighlighted);
					tmEdit.ReadColor (_T("BorderDisabled"), m_clrRibbonEditBorderDisabled);
					tmEdit.ReadColor (_T("BorderPressed"), m_clrRibbonEditBorderPressed);
					tmEdit.ReadColor (_T("Selection"), m_clrRibbonEditSelection);

					CString strButton;
					if (tmEdit.ExcludeTag (_T("BUTTON"), strButton))
					{
						CBCGPTagManager tmButton (strButton);

						tmButton.ReadColor (_T("GradientStartNormal"), m_clrRibbonComboBtnStart);
						tmButton.ReadColor (_T("GradientFinishNormal"), m_clrRibbonComboBtnFinish);
						tmButton.ReadColor (_T("BtnBorderNormal"), m_clrRibbonComboBtnBorder);

						if (!tmButton.ReadControlRenderer (_T("IMAGE"), m_ctrlRibbonComboBoxBtn, IDB_OFFICE2007_COMBOBOX_BTN))
						{
							tmButton.ReadColor (_T("GradientStartHighlighted"), m_clrRibbonComboBtnHighlightedStart);
							tmButton.ReadColor (_T("GradientFinishHighlighted"), m_clrRibbonComboBtnHighlightedFinish);
							tmButton.ReadColor (_T("GradientStartDisabled"), m_clrRibbonComboBtnDisabledStart);
							tmButton.ReadColor (_T("GradientFinishDisabled"), m_clrRibbonComboBtnDisabledFinish);
							tmButton.ReadColor (_T("GradientStartPressed"), m_clrRibbonComboBtnPressedStart);
							tmButton.ReadColor (_T("GradientFinishPressed"), m_clrRibbonComboBtnPressedFinish);

							tmButton.ReadColor (_T("BtnBorderHighlighted"), m_clrRibbonComboBtnBorderHighlighted);
							tmButton.ReadColor (_T("BtnBorderDisabled"), m_clrRibbonComboBtnBorderDisabled);

							m_clrRibbonComboBtnBorderPressed = m_clrRibbonComboBtnBorderHighlighted;
							tmButton.ReadColor (_T("BtnBorderPressed"), m_clrRibbonComboBtnBorderPressed);
						}
					}
				}
			}

			tmPanel.ReadColor (_T("TextNormal"), m_clrRibbonPanelText);
			tmPanel.ReadColor (_T("TextHighlighted"), m_clrRibbonPanelTextHighlighted);
		}

		if (tmItem.ExcludeTag (_T("CONTEXT"), str))
		{
			CBCGPTagManager tmContext (str);

			CString strCategory;
			if (tmContext.ExcludeTag (_T("CATEGORY"), strCategory))
			{
				CBCGPTagManager tmCategory (strCategory);

				CBCGPControlRendererParams prBack;
				CBCGPControlRendererParams prCaption;
				CBCGPControlRendererParams prTab;
				CBCGPControlRendererParams prDefault;
				COLORREF clrText = m_clrRibbonCategoryText;
				COLORREF clrTextHighlighted = m_clrRibbonCategoryTextHighlighted;
				COLORREF clrCaptionText = clrText;

				tmCategory.ReadControlRendererParams (_T("BACK"), prBack);

				CString strTab;
				if (tmCategory.ExcludeTag (_T("TAB"), strTab))
				{
					CBCGPTagManager tmTab (strTab);

					tmTab.ReadControlRendererParams(_T("BUTTON"), prTab);
					tmTab.ReadColor (_T("TextNormal"), clrText);
					tmTab.ReadColor (_T("TextHighlighted"), clrTextHighlighted);
				}

				CString strCaption;
				if (tmCategory.ExcludeTag (_T("CAPTION"), strCaption))
				{
					CBCGPTagManager tmCaption (strCaption);

					tmCaption.ReadControlRendererParams(_T("BACK"), prCaption);
					tmCaption.ReadColor (_T("TextNormal"), clrCaptionText);
				}

				tmCategory.ReadControlRendererParams(_T("BUTTON_DEFAULT"), prDefault);

				UINT nID[BCGPRibbonCategoryColorCount] = 
					{
						IDB_OFFICE2007_RIBBON_CONTEXT_R_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_O_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_Y_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_G_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_B_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_I_BTN_DEFAULT,
						IDB_OFFICE2007_RIBBON_CONTEXT_V_BTN_DEFAULT
					};

				for (int i = 0; i < BCGPRibbonCategoryColorCount; i++)
				{
					XRibbonContextCategory& cat = m_ctrlRibbonContextCategory[i];

					prDefault.m_uiBmpResID = nID[i] + 0;
					prTab.m_uiBmpResID     = nID[i] + 1;
					prCaption.m_uiBmpResID = nID[i] + 2;
					prBack.m_uiBmpResID    = nID[i] + 3;

					cat.m_ctrlBtnDefault.Create (prDefault);
					cat.m_ctrlCaption.Create (prCaption);
					cat.m_ctrlTab.Create (prTab);
					cat.m_ctrlBack.Create (prBack);
					cat.m_clrText            = clrText;
					cat.m_clrTextHighlighted = clrTextHighlighted;
					cat.m_clrCaptionText     = clrCaptionText;
				}
			}

			CString strPanel;
			if (tmContext.ExcludeTag (_T("PANEL"), strPanel))
			{
				CBCGPTagManager tmPanel (strPanel);

				CString strBack;
				if (tmPanel.ExcludeTag (_T("BACK"), strBack))
				{
					CBCGPTagManager tmBack (strBack);

					tmBack.ReadControlRenderer(_T("TOP"), m_ctrlRibbonContextPanelBack_T, IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_T);
					tmBack.ReadControlRenderer(_T("BOTTOM"), m_ctrlRibbonContextPanelBack_B, IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_B);
				}

				CString strCaption;
				if (tmPanel.ExcludeTag (_T("CAPTION"), strCaption))
				{
					CBCGPTagManager tmCaption (strCaption);

					tmCaption.ReadColor (_T("TextNormal"), m_clrRibbonContextPanelCaptionText);
					tmCaption.ReadColor (_T("TextHighlighted"), m_clrRibbonContextPanelCaptionTextHighlighted);
				}

				tmPanel.ReadColor (_T("TextNormal"), m_clrRibbonContextPanelText);
				tmPanel.ReadColor (_T("TextHighlighted"), m_clrRibbonContextPanelTextHighlighted);
			}

			tmContext.ReadControlRenderer (_T("SEPARATOR"), m_ctrlRibbonContextSeparator, IDB_OFFICE2007_RIBBON_CONTEXT_SEPARATOR);
		}

		tmItem.ReadControlRenderer (_T("MAIN_BUTTON"), m_RibbonBtnMain, IDB_OFFICE2007_RIBBON_BTN_MAIN);

		if (tmItem.ExcludeTag (_T("MAIN"), str))
		{
			CBCGPTagManager tmMain (str);
			tmMain.ReadControlRenderer (_T("BACK"), m_ctrlRibbonMainPanel, IDB_OFFICE2007_RIBBON_PANEL_MAIN);
			tmMain.ReadControlRenderer (_T("BORDER"), m_ctrlRibbonMainPanelBorder, IDB_OFFICE2007_RIBBON_PANEL_MAIN_BORDER);
			tmMain.ReadControlRenderer (_T("BUTTON"), m_ctrlRibbonBtnMainPanel, IDB_OFFICE2007_RIBBON_BTN_PANEL_MAIN);
		}

		if (tmItem.ExcludeTag (_T("CAPTION"), str))
		{
			CBCGPTagManager tmCaption (str);
			tmCaption.ReadControlRenderer(_T("QA"), m_ctrlRibbonCaptionQA, IDB_OFFICE2007_RIBBON_CAPTION_QA);
			tmCaption.ReadControlRenderer(_T("QA_GLASS"), m_ctrlRibbonCaptionQA_Glass, IDB_OFFICE2007_RIBBON_CAPTION_QA_GLASS);
		}

		if (tmItem.ExcludeTag (_T("STATUS"), str))
		{
			CBCGPTagManager tmStatus (str);
			tmStatus.ReadControlRenderer(_T("PANE_BUTTON"), m_ctrlRibbonBtnStatusPane, IDB_OFFICE2007_RIBBON_BTN_STATUS_PANE);

			CString strSlider;
			if (tmStatus.ExcludeTag (_T("SLIDER"), strSlider))
			{
				CBCGPTagManager tmSlider (strSlider);

				tmSlider.ReadControlRenderer(_T("THUMB"), m_ctrlRibbonSliderThumb, IDB_OFFICE2007_RIBBON_SLIDER_THUMB);
				tmSlider.ReadControlRenderer(_T("PLUS"), m_ctrlRibbonSliderBtnPlus, IDB_OFFICE2007_RIBBON_SLIDER_BTN_PLUS);
				tmSlider.ReadControlRenderer(_T("MINUS"), m_ctrlRibbonSliderBtnMinus, IDB_OFFICE2007_RIBBON_SLIDER_BTN_MINUS);
			}

			CString strProgress;
			if (tmStatus.ExcludeTag (_T("PROGRESS"), strProgress))
			{
				CBCGPTagManager tmProgress (strProgress);

				tmProgress.ReadControlRenderer(_T("BACK"), m_ctrlRibbonProgressBack, IDB_OFFICE2007_RIBBON_PROGRESS_BACK);
				tmProgress.ReadControlRenderer(_T("NORMAL"), m_ctrlRibbonProgressNormal, IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL);
				tmProgress.ReadControlRenderer(_T("NORMAL_EXT"), m_ctrlRibbonProgressNormalExt, IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL_EXT);
				tmProgress.ReadControlRenderer(_T("INFINITY"), m_ctrlRibbonProgressInfinity, IDB_OFFICE2007_RIBBON_PROGRESS_INFINITY);
			}
		}

		if (tmItem.ExcludeTag (_T("BORDERS"), str))
		{
			CBCGPTagManager tmBorders (str);

			tmBorders.ReadControlRenderer (_T("QAT"), m_ctrlRibbonBorder_QAT, IDB_OFFICE2007_RIBBON_BORDER_QAT);
			tmBorders.ReadControlRenderer (_T("FLOATY"), m_ctrlRibbonBorder_Floaty, IDB_OFFICE2007_RIBBON_BORDER_FLOATY);
		}
	}


	if (tm.ExcludeTag (_T("PLANNER"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadBool (_T("BLACK"), m_bPlannerBlack);

		if (tmItem.ReadColor (_T("WorkColor"), m_clrPlannerWork))
		{
			m_brPlanner.DeleteObject ();
			m_brPlanner.CreateSolidBrush (m_clrPlannerWork);
		}

		tmItem.ReadColor (_T("NcAreaColor"), m_clrPlannerNcArea);
		tmItem.ReadColor (_T("NcLineColor"), m_clrPlannerNcLine);
		tmItem.ReadColor (_T("NcTextColor"), m_clrPlannerNcText);

		CString str;
		if (tmItem.ExcludeTag (_T("TODAY"), str))
		{
			CBCGPTagManager tmToday (str);

			tmToday.ReadColor (_T("Caption1Start") , m_clrPlannerTodayCaption[0]);
			tmToday.ReadColor (_T("Caption1Finish"), m_clrPlannerTodayCaption[1]);
			tmToday.ReadColor (_T("Caption2Start") , m_clrPlannerTodayCaption[2]);
			tmToday.ReadColor (_T("Caption2Finish"), m_clrPlannerTodayCaption[3]);
			tmToday.ReadColor (_T("BorderColor")   , m_clrPlannerTodayBorder);
		}
	}


	m_clrGridLeftOffset          = globalData.clrBarFace;
	m_clrGridGroupLine			 = globalData.clrBarShadow;
	m_clrGridHeaderNormalStart   = m_clrHeaderNormalStart;
	m_clrGridHeaderNormalFinish  = m_clrHeaderNormalFinish;
	m_clrGridHeaderNormalBorder  = m_clrHeaderNormalBorder;
	m_clrGridHeaderPressedStart  = m_clrHeaderPressedStart;
	m_clrGridHeaderPressedFinish = m_clrHeaderPressedFinish;
	m_clrGridHeaderPressedBorder = m_clrHeaderPressedBorder;

	if (tm.ExcludeTag (_T("GRID"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		CString strHeader;
		if (tmItem.ExcludeTag (_T("HEADER"), strHeader))
		{
			CBCGPTagManager tmHeader (strHeader);

			tmHeader.ReadColor (_T("NormalStart"), m_clrGridHeaderNormalStart);
			tmHeader.ReadColor (_T("NormalFinish"), m_clrGridHeaderNormalFinish);
			tmHeader.ReadColor (_T("NormalBorder"), m_clrGridHeaderNormalBorder);

			m_clrGridHeaderPressedStart  = m_clrGridHeaderNormalFinish;
			m_clrGridHeaderPressedFinish = m_clrGridHeaderNormalStart;
			m_clrGridHeaderPressedBorder = m_clrGridHeaderNormalBorder;

			tmHeader.ReadColor (_T("PressedStart"), m_clrGridHeaderPressedStart);
			tmHeader.ReadColor (_T("PressedFinish"), m_clrGridHeaderPressedFinish);
			tmHeader.ReadColor (_T("PressedBorder"), m_clrGridHeaderPressedBorder);

			COLORREF clr;
			if (tmHeader.ReadColor (_T("Separator"), clr))
			{
				m_penGridSeparator.DeleteObject ();
				m_penGridSeparator.CreatePen (PS_SOLID, 1, clr);
			}
		}

		COLORREF clr = globalData.clrBtnLight;
		if (tmItem.ReadColor (_T("ExpandBoxLight"), clr))
		{
			m_penGridExpandBoxLight.DeleteObject ();
			m_penGridExpandBoxLight.CreatePen (PS_SOLID, 1, clr);
		}

		clr = globalData.clrBtnShadow;
		if (tmItem.ReadColor (_T("ExpandBoxDark"), clr))
		{
			m_penGridExpandBoxDark.DeleteObject ();
			m_penGridExpandBoxDark.CreatePen (PS_SOLID, 1, clr);
		}

		tmItem.ReadColor (_T("LeftOffset"), m_clrGridLeftOffset);

		tmItem.ReadColor (_T("GroupLine"), m_clrGridGroupLine);
		m_clrGridGroupSubLine = m_clrGridGroupLine;
		tmItem.ReadColor (_T("GroupSubLine"), m_clrGridGroupSubLine);
	}

	m_clrOutlookCaptionTextNormal   = m_clrCaptionBarText;
	m_clrOutlookPageTextNormal      = m_clrOutlookCaptionTextNormal;
	m_clrOutlookPageTextHighlighted = m_clrOutlookPageTextNormal;
	m_clrOutlookPageTextPressed     = m_clrOutlookPageTextNormal;

	if (tm.ExcludeTag (_T("OUTLOOK"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		CString str;
		if (tmItem.ExcludeTag (_T("CAPTION"), str))
		{
			CBCGPTagManager tmCaption (str);

			tmCaption.ReadColor (_T("TextNormal"), m_clrOutlookCaptionTextNormal);
		}
		
		if (tmItem.ExcludeTag (_T("PAGEBUTTON"), str))
		{
			CBCGPTagManager tmPage (str);

			tmPage.ReadControlRenderer (_T("BACK"), m_ctrlOutlookWndPageBtn, IDB_OFFICE2007_OUTLOOK_BTN_PAGE);

			tmPage.ReadColor (_T("TextNormal"), m_clrOutlookPageTextNormal);
			tmPage.ReadColor (_T("TextHighlighted"), m_clrOutlookPageTextHighlighted);
			tmPage.ReadColor (_T("TextPressed"), m_clrOutlookPageTextPressed);
		}

		if (tmItem.ExcludeTag (_T("BAR"), str))
		{
			CBCGPTagManager tmBar (str);

			tmBar.ReadControlRenderer (_T("BACK"), m_ctrlOutlookWndBar, IDB_OFFICE2007_OUTLOOK_BAR_BACK);
		}
	}

	// Popup Window:
	m_clrPopupGradientLight = m_clrBarGradientLight;
	m_clrPopupGradientDark = m_clrBarGradientDark;

	if (tm.ExcludeTag (_T("POPUP"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		tmItem.ReadColor (_T("GradientFillLight"), m_clrPopupGradientLight);
		tmItem.ReadColor (_T("GradientFillDark"), m_clrPopupGradientDark);
	}

	if (hinstResOld != NULL)
	{
		AfxSetResourceHandle (hinstResOld);
	}
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::IsWindowActive (CWnd* pWnd) const
{
	BOOL bActive = FALSE;

	HWND hWnd = pWnd->GetSafeHwnd ();

	if (hWnd != NULL)
	{
		if(!m_ActivateFlag.Lookup (pWnd->GetSafeHwnd (), bActive))
		{
			//ASSERT(FALSE);
			bActive = TRUE;
		}
	}

	return bActive;
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::OnNcActivate (CWnd* pWnd, BOOL bActive)
{
	if (globalData.DwmIsCompositionEnabled ())
	{
		return FALSE;
	}

	HWND hWnd = pWnd->GetSafeHwnd ();

	ASSERT_VALID (pWnd);

	if (hWnd == NULL)
	{
		return FALSE;
	}

	// stay active if WF_STAYACTIVE bit is on
	if (pWnd->m_nFlags & WF_STAYACTIVE)
	{
		bActive = TRUE;
	}

	// but do not stay active if the window is disabled
	if (!pWnd->IsWindowEnabled())
	{
		bActive = FALSE;
	}

	m_ActivateFlag[pWnd->GetSafeHwnd ()] = bActive;
	pWnd->SendMessage (WM_NCPAINT, 0, 0);

	return TRUE;
}
//*****************************************************************************
void CBCGPVisualManager2007::DrawNcBtn (CDC* pDC, const CRect& rect, UINT nButton, 
										BCGBUTTON_STATE state, BOOL bSmall, 
										BOOL bActive, BOOL bMDI/* = FALSE*/)
{
	ASSERT_VALID (pDC);

	CBCGPToolBarImages* pImage = NULL;

	int nIndex = bSmall ? 1 : 0;

	if (nButton == SC_CLOSE)
	{
		pImage = &m_SysBtnClose[nIndex];
	}
	else if (nButton == SC_MINIMIZE)
	{
		pImage = &m_SysBtnMinimize[nIndex];
	}
	else if (nButton == SC_MAXIMIZE)
	{
		pImage = &m_SysBtnMaximize[nIndex];
	}
	else if (nButton == SC_RESTORE)
	{
		pImage = &m_SysBtnRestore[nIndex];
	}
	else
	{
		return;
	}

	CBCGPToolBarImages::ImageAlignHorz horz = CBCGPToolBarImages::ImageAlignHorzCenter;
	CBCGPToolBarImages::ImageAlignVert vert = CBCGPToolBarImages::ImageAlignVertCenter;

	CRect rtBtnImage (CPoint (0, 0), pImage->GetImageSize ());

	if (!bActive)
	{
		rtBtnImage.OffsetRect (0, pImage->GetImageSize ().cy * 3);
	}
	else
	{
		if (state != ButtonsIsRegular)
		{
			if (!IsBeta1 () && bMDI)
			{
				m_ctrlRibbonBtn[0].Draw (pDC, rect, state == ButtonsIsHighlighted ? 0 : 1);
			}
			else
			{
				m_SysBtnBack[nIndex].Draw (pDC, rect, state == ButtonsIsHighlighted ? 0 : 1);
			}

			rtBtnImage.OffsetRect (0, pImage->GetImageSize ().cy * 
				(state == ButtonsIsHighlighted ? 1 : 2));
		}
	}

	pImage->DrawEx (pDC, rect, 0, horz, vert, rtBtnImage);
}
//*****************************************************************************
void CBCGPVisualManager2007::DrawNcText (CDC* pDC, CRect& rect, 
										 const CString& strTitle, 
										 const CString& strDocument, 
										 BOOL bPrefix, BOOL bActive, BOOL bIsRTL, 
										 BOOL bTextCenter)
{
	if ((strTitle.IsEmpty () && strDocument.IsEmpty ()) || 
		rect.right <= rect.left)
	{
		return;
	}

	ASSERT_VALID (pDC);

	int nOldMode = pDC->SetBkMode    (TRANSPARENT);
	COLORREF clrOldText = pDC->GetTextColor ();

	DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
		(bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);

	if (strDocument.IsEmpty ())
	{
		COLORREF clrText = bActive 
			? m_clrAppCaptionActiveTitleText
			: m_clrAppCaptionInactiveTitleText;

		int widthFull = rect.Width ();
		int width = pDC->GetTextExtent (strTitle).cx;

		if (bTextCenter && width < widthFull)
		{
			rect.left += (widthFull - width) / 2;
		}

		rect.right = min (rect.left + width, rect.right);

		if (rect.right > rect.left)
		{
			pDC->SetTextColor (clrText);
			pDC->DrawText (strTitle, rect, dwTextStyle);
		}
	}
	else
	{
		const CString& str1 = bPrefix ? strDocument : strTitle;
		const CString& str2 = bPrefix ? strTitle : strDocument;

		COLORREF clrText1 = bActive 
			? m_clrAppCaptionActiveText
			: m_clrAppCaptionInactiveText;
		COLORREF clrText2 = bActive 
			? m_clrAppCaptionActiveTitleText
			: m_clrAppCaptionInactiveTitleText;

		if (!bPrefix)
		{
			COLORREF clr = clrText1;
			clrText1 = clrText2;
			clrText2 = clr;
		}

		int widthFull = rect.Width ();
		CSize sz1 = pDC->GetTextExtent (str1);
		CSize sz2 = pDC->GetTextExtent (str2);
		int width = sz1.cx + sz2.cx;
		int left = rect.left;

		if (bTextCenter && width < widthFull)
		{
			rect.left += (widthFull - width) / 2;
		}

		rect.right = min (rect.left + width, rect.right);

		if (bIsRTL)
		{
			if (width <= rect.Width ())
			{
				rect.left += sz2.cx;
			}
			else
			{
				if (sz1.cx < rect.Width ())
				{
					rect.left += max (0, sz2.cx + (rect.Width () - width));
				}
			}
		}

		pDC->SetTextColor (clrText1);
		pDC->DrawText (str1, rect, dwTextStyle);

		if (bIsRTL)
		{
			rect.left = left;
			if (width <= rect.Width ())
			{
				rect.right = rect.left + sz2.cx;
			}
			else
			{
				rect.right -= sz1.cx;
			}
		}
		else
		{
			rect.left += sz1.cx;
		}

		if (rect.right > rect.left)
		{
			pDC->SetTextColor (clrText2);
			pDC->DrawText (str2, rect, dwTextStyle);
		}
	}

	pDC->SetBkMode    (nOldMode);
	pDC->SetTextColor (clrOldText);
}
//*****************************************************************************
void CBCGPVisualManager2007::DrawNcCaption (CDC* pDC, CRect rectCaption, 
											   DWORD dwStyle, DWORD dwStyleEx,
											   const CString& strTitle, const CString& strDocument,
											   HICON hIcon, BOOL bPrefix, BOOL bActive, 
											   BOOL bTextCenter,
											   const CObList& lstSysButtons)
{
	const BOOL bIsRTL           = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
	const int nSysCaptionHeight = ::GetSystemMetrics (SM_CYCAPTION);
	CSize szSysBorder (GetSystemBorders (FALSE));

    CDC memDC;
    memDC.CreateCompatibleDC (pDC);
    CBitmap memBmp;
    memBmp.CreateCompatibleBitmap (pDC, rectCaption.Width (), rectCaption.Height ());
    CBitmap* pBmpOld = memDC.SelectObject (&memBmp);
	memDC.BitBlt (0, 0, rectCaption.Width (), rectCaption.Height (), pDC, 0, 0, SRCCOPY);

	BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;

    {
        if (IsBeta1 ())
        {
			COLORREF clr1  = bActive 
								? m_clrAppCaptionActiveStart 
								: m_clrAppCaptionInactiveStart;
			COLORREF clr2  = bActive 
								? m_clrAppCaptionActiveFinish 
								: m_clrAppCaptionInactiveFinish;

			CRect rectCaption1 (rectCaption);
			CRect rectBorder (m_ctrlMainBorderCaption.GetParams ().m_rectSides);
			
			rectCaption1.DeflateRect (rectBorder.left, rectBorder.top, 
				rectBorder.right, rectBorder.bottom);

			{
				CBCGPDrawManager dm (memDC);
				dm.Fill4ColorsGradient (rectCaption1, clr1, clr2, clr2, clr1, FALSE);
			}

			m_ctrlMainBorderCaption.DrawFrame (&memDC, rectCaption, bActive ? 0 : 1);
        }
		else
		{
			m_ctrlMainBorderCaption.Draw (&memDC, rectCaption, bActive ? 0 : 1);
		}
    }

	CRect rect (rectCaption);
	rect.DeflateRect (szSysBorder.cx, bMaximized ? szSysBorder.cy : 0, szSysBorder.cx, 0);

	// Draw icon:
	if (hIcon != NULL)
	{
		CSize szIcon (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON));

		CRect rectIcon (rect.TopLeft (), CSize (nSysCaptionHeight, rect.Height ()));

		long x = rect.left + max (0, (rectIcon.Width ()  - szIcon.cx) / 2);
		long y = rect.top  + max (0, (rectIcon.Height () - szIcon.cy) / 2);

		::DrawIconEx (memDC.GetSafeHdc (), x, y, hIcon, szIcon.cx, szIcon.cy,
			0, NULL, DI_NORMAL);

		rect.left += rectIcon.Width ();
	}

	// Draw system buttons:
	int xButtonsRight = rect.right;

	for (POSITION pos = lstSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			lstSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		BCGBUTTON_STATE state = ButtonsIsRegular;

		if (pButton->m_bPushed && pButton->m_bFocused)
		{
			state = ButtonsIsPressed;
		}
		else if (pButton->m_bFocused)
		{
			state = ButtonsIsHighlighted;
		}

		UINT uiHit = pButton->GetHit ();
		UINT nButton = 0;

		switch (uiHit)
		{
		case HTCLOSE_BCG:
			nButton = SC_CLOSE;
			break;

		case HTMAXBUTTON_BCG:
			nButton = 
				(dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE;
			break;

		case HTMINBUTTON_BCG:
			nButton = 
				(dwStyle & WS_MINIMIZE) == WS_MINIMIZE ? SC_RESTORE : SC_MINIMIZE;
			break;
		}

		CRect rectBtn (pButton->GetRect ());
		if (bMaximized)
		{
			rectBtn.OffsetRect (szSysBorder.cx, szSysBorder.cy);
		}

		DrawNcBtn (&memDC, rectBtn, nButton, state, FALSE, bActive, FALSE);

		xButtonsRight = min (xButtonsRight, pButton->GetRect ().left);
	}

	// Draw text:
	if ((!strTitle.IsEmpty () || !strDocument.IsEmpty ()) && 
		rect.left < rect.right)
	{
		CFont* pOldFont = (CFont*)memDC.SelectObject (&m_AppCaptionFont);

		CRect rectText = rect;
		rectText.right = xButtonsRight - 1;

		DrawNcText (&memDC, rectText, strTitle, strDocument, bPrefix, bActive, bIsRTL, bTextCenter);

		memDC.SelectObject (pOldFont);
	}

    pDC->BitBlt (rectCaption.left, rectCaption.top, rectCaption.Width (), rectCaption.Height (),
        &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject (pBmpOld);
}

#ifndef BCGP_EXCLUDE_RIBBON

CBCGPRibbonBar*	CBCGPVisualManager2007::GetRibbonBar (CWnd* pWnd) const
{
	CBCGPRibbonBar* pBar = NULL;

	if (pWnd == NULL)
	{
		pWnd = AfxGetMainWnd ();
	}

	if (pWnd->GetSafeHwnd () == NULL)
	{
		return NULL;
	}

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		pBar = ((CBCGPFrameWnd*) pWnd)->GetRibbonBar ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		pBar = ((CBCGPMDIFrameWnd*) pWnd)->GetRibbonBar ();
	}

	return pBar;
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::IsRibbonPresent (CWnd* pWnd) const
{
	CBCGPRibbonBar* pBar = GetRibbonBar (pWnd);

	return pBar != NULL && pBar->IsWindowVisible ();
}

#endif

BOOL CBCGPVisualManager2007::OnNcPaint (CWnd* pWnd, const CObList& lstSysButtons, CRect rectRedraw)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnNcPaint (pWnd, lstSysButtons, rectRedraw);
	}

	ASSERT_VALID (pWnd);

	if (pWnd->GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	CWindowDC dc (pWnd);

	if (dc.GetSafeHdc () != NULL)
	{
		CRgn rgn;
		if (!rectRedraw.IsRectEmpty ())
		{
			rgn.CreateRectRgnIndirect (rectRedraw);
			dc.SelectClipRgn (&rgn);
		}

#ifndef BCGP_EXCLUDE_RIBBON
		CBCGPRibbonBar* pBar = GetRibbonBar (pWnd);
		BOOL bRibbonCaption  = pBar != NULL && 
							   pBar->IsWindowVisible () &&
							   pBar->IsReplaceFrameCaption ();
#else
		BOOL bRibbonCaption = FALSE;
#endif

		CRect rtWindow;
		pWnd->GetWindowRect (rtWindow);
		pWnd->ScreenToClient (rtWindow);

		CRect rtClient;
		pWnd->GetClientRect (rtClient);

		rtClient.OffsetRect (-rtWindow.TopLeft ());
		dc.ExcludeClipRect (rtClient);

		rtWindow.OffsetRect (-rtWindow.TopLeft ());

        BOOL bActive = IsWindowActive (pWnd);

		CRect rectCaption (rtWindow);
		CSize szSysBorder (GetSystemBorders (bRibbonCaption));

		rectCaption.bottom = rectCaption.top + szSysBorder.cy;

		const DWORD dwStyle = pWnd->GetStyle ();
		BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;

		if (!bRibbonCaption)
		{
			const int nSysCaptionHeight = ::GetSystemMetrics (SM_CYCAPTION);
			rectCaption.bottom += nSysCaptionHeight;

			const DWORD dwStyleEx = pWnd->GetExStyle ();

			HICON hIcon = globalUtils.GetWndIcon (pWnd);

			CString strText;
			pWnd->GetWindowText (strText);

			CString strTitle (strText);
			CString strDocument;

			BOOL bPrefix = FALSE;
			if ((dwStyle & FWS_ADDTOTITLE) == FWS_ADDTOTITLE)
			{
				bPrefix = (dwStyle & FWS_PREFIXTITLE) == FWS_PREFIXTITLE;
				CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);

				if (pFrameWnd != NULL)
				{
					strTitle = pFrameWnd->GetTitle();

					if (!strTitle.IsEmpty ())
					{
						int pos = strText.Find (strTitle);

						if (pos != -1)
						{
							if (strText.GetLength () > strTitle.GetLength ())
							{
								if (pos == 0)
								{
									bPrefix = FALSE; // avoid exception
									strTitle = strText.Left (strTitle.GetLength () + 3);
									strDocument = strText.Right (strText.GetLength () - strTitle.GetLength ());
								}
								else
								{
									strTitle = strText.Right (strTitle.GetLength () + 3);
									strDocument = strText.Left (strText.GetLength () - strTitle.GetLength ());
								}
							}
						}
					}
					else
					{
						strDocument = strText;
					}
				}
			}

			if (bMaximized)
			{
				rectCaption.InflateRect (szSysBorder.cx, szSysBorder.cy, szSysBorder.cx, 0);
			}

			DrawNcCaption (&dc, rectCaption, dwStyle, dwStyleEx, 
							strTitle, strDocument, hIcon, bPrefix, bActive, m_bNcTextCenter,
							lstSysButtons);

			if (bMaximized)
			{
				return TRUE;
			}
		}
#ifndef BCGP_EXCLUDE_RIBBON
		else
		{
			if (bMaximized)
			{
				return TRUE;
			}

			int nExtraWidth = 0;
			BOOL bExtra = ((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ALL) == 0 &&
						pBar->IsQuickAccessToolbarOnTop ()) && 
						(!pBar->IsQATEmpty() || IsBeta1 ());

			rectCaption.bottom += pBar->GetCaptionHeight ();

			CRect rectBorder (m_ctrlMainBorderCaption.GetParams ().m_rectSides);
			CRect rectQA (rectCaption);
			rectQA.right = rectQA.left;

			if (bExtra)
			{
				nExtraWidth = GetRibbonQATRightMargin ();
				rectQA.right += pBar->GetQuickAccessToolbarLocation ().right;
			}
			else
			{
				rectQA.right += rectBorder.left;
			}

			rectQA.right += nExtraWidth;

			if (IsBeta1 ())
			{
				COLORREF clr1  = bActive 
									? m_clrAppCaptionActiveStart 
									: m_clrAppCaptionInactiveStart;
				COLORREF clr2  = bActive 
									? m_clrAppCaptionActiveFinish 
									: m_clrAppCaptionInactiveFinish;

				CRect rectCaption2 (rectCaption);
				rectCaption2.DeflateRect (rectBorder.left, rectBorder.top, 
					rectBorder.right, rectBorder.bottom);

				{
					CBCGPDrawManager dm (dc);
					dm.Fill4ColorsGradient (rectCaption2, clr1, clr2, clr2, clr1, FALSE);
				}

				m_ctrlMainBorderCaption.DrawFrame (&dc, rectCaption, bActive ? 0 : 1);
			}
			else
			{
				m_ctrlMainBorderCaption.Draw (&dc, rectCaption, bActive ? 0 : 1);
			}

			if (bExtra)
			{
				if (!IsBeta1 ())
				{
					rectQA.right = pBar->GetQATCommandsLocation ().right + nExtraWidth;
				}

				if (!globalData.bIsWindowsVista)
				{
					m_ctrlRibbonCaptionQA.Draw (&dc, rectQA, bActive ? 0 : 1);
				}
			}
		}
#endif // BCGP_EXCLUDE_RIBBON

		rtWindow.top = rectCaption.bottom;

		dc.ExcludeClipRect (rectCaption);
		m_ctrlMainBorder.DrawFrame (&dc, rtWindow, bActive ? 0 : 1);

		//-------------------------------
		// Find status bar extended area:
		//-------------------------------
		CRect rectExt (0, 0, 0, 0);
		BOOL bExtended = FALSE;
		BOOL bIsStatusBar = FALSE;

		CWnd* pStatusBar = pWnd->GetDescendantWindow (AFX_IDW_STATUS_BAR, TRUE);

		if (pStatusBar->GetSafeHwnd () != NULL && pStatusBar->IsWindowVisible ())
		{
			CBCGPStatusBar* pClassicStatusBar = DYNAMIC_DOWNCAST (
				CBCGPStatusBar, pStatusBar);
			if (pClassicStatusBar != NULL)
			{
				bExtended = pClassicStatusBar->GetExtendedArea (rectExt);
				bIsStatusBar = TRUE;
			}
#ifndef BCGP_EXCLUDE_RIBBON
			else
			{
				CBCGPRibbonStatusBar* pRibbonStatusBar = DYNAMIC_DOWNCAST (
					CBCGPRibbonStatusBar, pStatusBar);
				if (pRibbonStatusBar != NULL)
				{
					bExtended = pRibbonStatusBar->GetExtendedArea (rectExt);
					bIsStatusBar = TRUE;
				}
			}
#endif // BCGP_EXCLUDE_RIBBON
		}

		if (bIsStatusBar)
		{
			CRect rectStatus;
			pStatusBar->GetClientRect (rectStatus);

			int nHeight = rectStatus.Height ();
			rectStatus.bottom = rtWindow.bottom;
			rectStatus.top    = rectStatus.bottom - nHeight - szSysBorder.cy;
			rectStatus.left   = rtWindow.left;
			rectStatus.right  = rtWindow.right;

			if (bExtended)
			{
				rectExt.left   = rectStatus.right - rectExt.Width () - szSysBorder.cx;
				rectExt.top    = rectStatus.top;
				rectExt.bottom = rectStatus.bottom;
				rectExt.right  = rtWindow.right;
			}

			m_ctrlStatusBarBack.Draw (&dc, rectStatus, bActive ? 0 : 1);

			if (bExtended)
			{
				rectExt.left -= m_ctrlStatusBarBack_Ext.GetParams ().m_rectCorners.left;

				m_ctrlStatusBarBack_Ext.Draw (&dc, rectExt, bActive ? 0 : 1);
			}
		}

		dc.SelectClipRgn (NULL);

		return TRUE;
	}

	return CBCGPVisualManager2003::OnNcPaint (pWnd, lstSysButtons, rectRedraw);
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::OnSetWindowRegion (CWnd* pWnd, CSize sizeWindow)
{
	if (!CanDrawImage () || globalData.DwmIsCompositionEnabled ())
	{
		return FALSE;
	}

	ASSERT_VALID (pWnd);

	if (pWnd->GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

    CSize sz (0, 0);

	BOOL bMainWnd = FALSE;

	if (DYNAMIC_DOWNCAST (CBCGPPopupMenu, pWnd) != NULL)
	{
		sz  = CSize (3, 3);
	}
#ifndef BCGP_EXCLUDE_RIBBON
	else if (DYNAMIC_DOWNCAST (CBCGPRibbonBar, pWnd) != NULL)
	{
		return FALSE;
	}
#endif
	else
	{
		if ((pWnd->GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE)
		{
			pWnd->SetWindowRgn (NULL, TRUE);
			return TRUE;
		}

		sz  = CSize (9, 9);

		bMainWnd = TRUE;
	}

	if (sz != CSize (0, 0))
	{
        CRgn rgn;
		BOOL bCreated = FALSE;

		bCreated = rgn.CreateRoundRectRgn (0, 0, sizeWindow.cx + 1, sizeWindow.cy + 1, sz.cx, sz.cy);

		if (bCreated)
		{
			pWnd->SetWindowRgn ((HRGN)rgn.Detach (), TRUE);
			return TRUE;
		}
	}

	return FALSE;
}
//*****************************************************************************
CSize CBCGPVisualManager2007::GetNcBtnSize (BOOL bSmall) const
{
	return m_szNcBtnSize[bSmall ? 1 : 0];
}
//*****************************************************************************
void CBCGPVisualManager2007::DrawSeparator (CDC* pDC, const CRect& rect, BOOL bHorz)
{
    DrawSeparator (pDC, rect, m_penSeparator, m_penSeparator2, bHorz);
}
//*****************************************************************************
void CBCGPVisualManager2007::DrawSeparator (CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, BOOL bHorz)
{
    CRect rect1 (rect);
    CRect rect2;

    if (bHorz)
    {
        rect1.top += rect.Height () / 2 - 1;
        rect1.bottom = rect1.top;
        rect2 = rect1;
        rect2.OffsetRect (0, 1);
    }
    else
    {
        rect1.left += rect.Width () / 2 - 1;
        rect1.right = rect1.left;
        rect2 = rect1;
        rect2.OffsetRect (1, 0);
    }

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);

		LOGPEN logpen;

		pen1.GetLogPen (&logpen);
		dm.DrawLine (rect1.left, rect1.top, rect1.right, rect1.bottom, logpen.lopnColor);

		pen2.GetLogPen (&logpen);
		dm.DrawLine (rect2.left, rect2.top, rect2.right, rect2.bottom, logpen.lopnColor);
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject (&pen1);
		pDC->MoveTo (rect1.TopLeft ());
		pDC->LineTo (rect1.BottomRight ());

		pDC->SelectObject (&pen2);
		pDC->MoveTo (rect2.TopLeft ());
		pDC->LineTo (rect2.BottomRight ());

		pDC->SelectObject (pOldPen);
	}
}
//**********************************************************************************
COLORREF CBCGPVisualManager2007::GetCaptionBarTextColor (CBCGPCaptionBar* pBar)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetCaptionBarTextColor (pBar);
	}

	return m_clrCaptionBarText;
}
//***************************************************************************************
void CBCGPVisualManager2007::OnFillOutlookPageButton (CDC* pDC, 
												  const CRect& rect,
												  BOOL bIsHighlighted, BOOL bIsPressed,
												  COLORREF& clrText)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillOutlookPageButton (pDC, rect, bIsHighlighted, bIsPressed, clrText);
		return;
	}

	ASSERT_VALID (pDC);

	CRect rt (rect);

	if (m_ctrlOutlookWndPageBtn.IsValid ())
	{
		int index = 0;

		if (bIsPressed)
		{
			index = 2;

			if (bIsHighlighted)
			{
				index = 3;
			}

			clrText = m_clrOutlookPageTextPressed;
		}
		else if (bIsHighlighted)
		{
			index = 1;

			clrText = m_clrOutlookPageTextHighlighted;
		}

		m_ctrlOutlookWndPageBtn.Draw (pDC, rt, index);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		if (bIsPressed)
		{
			dm.FillGradient (rt,	m_clrHighlightDnGradientDark,
									m_clrHighlightDnGradientLight,
									TRUE);
		}
		else if (bIsHighlighted)
		{
			dm.FillGradient (rt,	m_clrHighlightGradientDark,
									m_clrHighlightGradientLight,
									TRUE);
		}
		else
		{
			dm.FillGradient (rt,	m_clrToolBarGradientDark,
									m_clrToolBarGradientLight,
									TRUE);
		}
	}

	clrText = m_clrOutlookPageTextNormal;

	if (bIsPressed)
	{
		clrText = m_clrOutlookPageTextPressed;
	}
	else if (bIsHighlighted)
	{
		clrText = m_clrOutlookPageTextHighlighted;
	}
}
//****************************************************************************************
void CBCGPVisualManager2007::OnDrawOutlookPageButtonBorder (
	CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawOutlookPageButtonBorder (pDC, rectBtn, bIsHighlighted, bIsPressed);
		return;
	}

	pDC->Draw3dRect (rectBtn, globalData.clrBtnHilite, m_clrToolBarBottomLine);
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter)
{
	ASSERT_VALID (pDC);

	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawOutlookBarSplitter (pDC, rectSplitter);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient (rectSplitter,
					m_clrCaptionBarGradientDark,
					m_clrCaptionBarGradientLight,
					TRUE);

	rectSplitter.OffsetRect (0, 1);
	m_ToolBarTear.DrawEx (pDC, rectSplitter, 0, CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
	rectSplitter.OffsetRect (0, -1);

	CPen* pOldPen = pDC->SelectObject (&m_penBottomLine);

	pDC->MoveTo (rectSplitter.left, rectSplitter.top);
	pDC->LineTo (rectSplitter.right, rectSplitter.top);

	pDC->MoveTo (rectSplitter.left, rectSplitter.bottom - 1);
	pDC->LineTo (rectSplitter.right, rectSplitter.bottom - 1);

	pDC->SelectObject (pOldPen);
}
//*******************************************************************************
void CBCGPVisualManager2007::OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText)
{
	CBCGPVisualManager2003::OnFillOutlookBarCaption (pDC, rectCaption, clrText);

	if (CanDrawImage ())
	{
		clrText = m_clrOutlookCaptionTextNormal;
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
						CRect rectClient, CRect rectClip,
						BOOL bNCArea/* = FALSE*/)
{
    CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (!CanDrawImage () ||
		pBar->IsDialogControl () ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPColorBar)) ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalculator)) ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalendarBar)))
	{
		CBCGPVisualManager2003::OnFillBarBackground (pDC, pBar, rectClient, rectClip, bNCArea);
		return;
	}

    if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
    {
	    BOOL bIsHorz = (pBar->GetBarStyle () & CBRS_ORIENT_HORZ);
	    COLORREF clr1 = bIsHorz ? m_clrMenuBarGradientDark : m_clrMenuBarGradientVertLight;
	    COLORREF clr2 = bIsHorz ? m_clrMenuBarGradientLight : m_clrMenuBarGradientVertDark;

        CBCGPDrawManager dm (*pDC);
        dm.Fill4ColorsGradient (rectClient, clr1, clr2, clr2, clr1, !bIsHorz);
        return;
    }
    else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
    {
		pDC->FillRect (rectClip, &m_brMenuLight);

		BOOL bQuickMode = FALSE;

		CBCGPPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode)
		{
			CWnd* pWnd = pMenuBar->GetParent();

			if(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CBCGPPopupMenu)))
			{
				CBCGPPopupMenu* pMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, pWnd);

				if(pMenu->IsCustomizePane())
				{
					bQuickMode = TRUE;
				}
			}

			CRect rectImages = rectClient;
			//rectImages.DeflateRect (0, 1);

			if (bQuickMode)
			{
				rectImages.right = rectImages.left + 2 * CBCGPToolBar::GetMenuImageSize ().cx + 
							4 * GetMenuImageMargin () + 4;
			}
			else
			{
				rectImages.right = rectImages.left + CBCGPToolBar::GetMenuImageSize ().cx + 
							2 * GetMenuImageMargin () + 2;
			}

            pDC->FillRect (rectImages, &m_brBarBkgnd);

            CRect rect (rectImages);
            rectImages.left = rectImages.right;
            rectImages.right += 2;
            DrawSeparator (pDC, rectImages, FALSE);
		}

        return;
    }
    else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPStatusBar)))
    {
		CSize szSysBorder (GetSystemBorders (TRUE));

		CRect rect (rectClient);
		CRect rectExt (0, 0, 0, 0);
		BOOL bExtended = ((CBCGPStatusBar*)pBar)->GetExtendedArea (rectExt);

		//CRect rectBorders = m_ctrlStatusBarBack.GetParams ().m_rectSides;

		if (bExtended)
		{
			rect.right = rectExt.left;
		}

		CWnd* pWnd = ((CBCGPStatusBar*)pBar)->GetParent ();
		ASSERT_VALID (pWnd);

		BOOL bActive = IsWindowActive (pWnd);

		rect.InflateRect (szSysBorder.cx, 0, szSysBorder.cx, szSysBorder.cy);
		m_ctrlStatusBarBack.Draw (pDC, rect, bActive ? 0 : 1);

		if (bExtended)
		{
			rectExt.InflateRect (0, 0, szSysBorder.cx, szSysBorder.cy);
			rectExt.left -= m_ctrlStatusBarBack_Ext.GetParams ().m_rectCorners.left;
			m_ctrlStatusBarBack_Ext.Draw (pDC, rectExt, bActive ? 0 : 1);
		}

        return;
    }
#ifndef BCGP_EXCLUDE_RIBBON
    else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonStatusBar)))
    {
		CSize szSysBorder (GetSystemBorders (TRUE));

		CRect rect (rectClient);
		CRect rectExt (0, 0, 0, 0);
		BOOL bExtended = ((CBCGPRibbonStatusBar*)pBar)->GetExtendedArea (rectExt);

		//CRect rectBorders = m_ctrlStatusBarBack.GetParams ().m_rectSides;

		if (bExtended)
		{
			rect.right = rectExt.left;
		}

		CWnd* pWnd = pBar->GetParent ();
		ASSERT_VALID (pWnd);

		BOOL bActive = IsWindowActive (pWnd);

		rect.InflateRect (szSysBorder.cx, 0, szSysBorder.cx, szSysBorder.cy);
		m_ctrlStatusBarBack.Draw (pDC, rect, bActive ? 0 : 1);

		if (bExtended)
		{
			rectExt.InflateRect (0, 0, szSysBorder.cx, szSysBorder.cy);
			rectExt.left -= m_ctrlStatusBarBack_Ext.GetParams ().m_rectCorners.left;
			m_ctrlStatusBarBack_Ext.Draw (pDC, rectExt, bActive ? 0 : 1);
		}

        return;
    }
#endif
	else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPOutlookBarToolBar)))
	{
		if (m_ctrlOutlookWndBar.IsValid ())
		{
			m_ctrlOutlookWndBar.Draw (pDC, rectClient);
		}
		else
		{
			CBCGPDrawManager dm (*pDC);
			dm.FillGradient (rectClient,	m_clrToolBarGradientDark,
											m_clrToolBarGradientLight,
											TRUE);
		}

		return;
	}

#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox =	pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxPage)) ||
						pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBox)) ||
						pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx));

	if (bIsToolBox)
	{
			CBCGPDrawManager dm (*pDC);
			dm.FillGradient (rectClient,	m_clrToolBarGradientLight,
											m_clrToolBarGradientDark,
											FALSE);
		return;
	}

#endif

	CBCGPVisualManager2003::OnFillBarBackground (pDC, pBar, rectClient, rectClip, bNCArea);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnFillHighlightedArea (CDC* pDC, CRect rect, 
							CBrush* pBrush, CBCGPToolbarButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	BOOL bIsHorz = TRUE;
//	BOOL bIsPopupMenu = FALSE;

	COLORREF clr1 = (COLORREF)-1;
	COLORREF clr2 = (COLORREF)-1;

	if (pButton != NULL)
	{
		ASSERT_VALID (pButton);

		bIsHorz = pButton->IsHorizontal ();

		CBCGPToolbarMenuButton* pCustButton = 
			DYNAMIC_DOWNCAST (CCustomizeButton, pButton);

		if (pCustButton != NULL)
		{
			if (pButton->IsDroppedDown ())
			{
				clr1 = m_clrHighlightDnGradientDark;
				clr2 = m_clrHighlightDnGradientLight;
			}
		}
	}

	if (pBrush == &m_brHighlight)
	{
		clr1 = m_clrHighlightGradientDark;
		clr2 = m_clrHighlightGradientLight;//bIsPopupMenu ? clr1 : m_clrHighlightGradientLight;
	}
	else if (pBrush == &m_brHighlightDn)
	{
		clr1 = m_clrHighlightDnGradientDark;//bIsPopupMenu ? m_clrHighlightDnGradientLight : m_clrHighlightDnGradientDark;
		clr2 = m_clrHighlightDnGradientLight;
	}
	else if (pBrush == &m_brHighlightChecked)
	{
		clr1 = m_clrHighlightCheckedGradientDark;//bIsPopupMenu ? m_clrHighlightCheckedGradientLight : m_clrHighlightCheckedGradientDark;
		clr2 = m_clrHighlightCheckedGradientLight;
	}

	if (clr1 == (COLORREF)-1 || clr2 == (COLORREF)-1)
	{
		CBCGPVisualManager2003::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, clr1, clr2, bIsHorz);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawMenuBorder (pDC, pMenu, rect);
		return;
	}

#ifndef BCGP_EXCLUDE_RIBBON
	//DR:
	if (pMenu != NULL /*&& !IsBeta1 ()*/)
	{
		CBCGPRibbonPanelMenuBar* pRibbonMenuBar = 
			DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenuBar, pMenu->GetMenuBar ());

		if (pRibbonMenuBar != NULL)
		{
			ASSERT_VALID (pRibbonMenuBar);

			if (pRibbonMenuBar->IsMainPanel ())
			{
				if (m_ctrlRibbonMainPanel.IsValid ())
				{
					m_ctrlRibbonMainPanel.DrawFrame (pDC, rect);
				}
				else
				{
					m_ctrlPopupBorder.DrawFrame (pDC, rect);
				}

				return;
			}

			if (!pRibbonMenuBar->IsMenuMode ())
			{
				if (pRibbonMenuBar->IsQATPopup () &&
					m_ctrlRibbonBorder_QAT.IsValid ())
				{
					m_ctrlRibbonBorder_QAT.DrawFrame (pDC, rect);
					return;
				}
				else if (pRibbonMenuBar->IsCategoryPopup ())
				{
					if (IsBeta1 ())
					{
						m_ctrlRibbonCategoryBack.DrawFrame (pDC, rect);
					}

					return;
				}
				else if (pRibbonMenuBar->IsFloaty () &&
						 m_ctrlRibbonBorder_Floaty.IsValid ())
				{
					m_ctrlRibbonBorder_Floaty.DrawFrame (pDC, rect);
					return;
				}
				else
				{
					if (pRibbonMenuBar->GetPanel () != NULL)
					{
						if (IsBeta1 ())
						{
							m_ctrlRibbonCategoryBack.DrawFrame (pDC, rect);
						}

						return;
					}

					// draw standard
				}
			}
		}
	}
#endif

	m_ctrlPopupBorder.DrawFrame (pDC, rect);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar)
{
	if (!CanDrawImage () ||
		(pBar != NULL && pBar->IsDialogControl ()) ||
        m_ToolBarGripper.GetCount () == 0)
	{
		CBCGPVisualManager2003::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

    CSize szBox (m_ToolBarGripper.GetImageSize ());

    if (szBox != CSize (0, 0))
    {
		if (bHorz)
		{
			rectGripper.left = rectGripper.right - szBox.cx;
		}
		else
		{
			rectGripper.top = rectGripper.bottom - szBox.cy;
		}

		CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pBar);
		if (pToolBar != NULL)
		{
			if (bHorz)
			{
				const int nHeight = CBCGPToolBar::IsLargeIcons () ? 
					pToolBar->GetRowHeight () : pToolBar->GetButtonSize ().cy;

				const int nDelta = max (0, (nHeight - pToolBar->GetImageSize ().cy) / 2);
				rectGripper.DeflateRect (0, nDelta);
			}
			else
			{
				const int nWidth = CBCGPToolBar::IsLargeIcons () ? 
					pToolBar->GetColumnWidth () : pToolBar->GetButtonSize ().cx;

				const int nDelta = max (0, (nWidth - pToolBar->GetImageSize ().cx) / 2);
				rectGripper.DeflateRect (nDelta, 0);
			}
		}

		const int nBoxesNumber = bHorz ?
			(rectGripper.Height () - szBox.cy) / szBox.cy : 
			(rectGripper.Width () - szBox.cx) / szBox.cx;

		int nOffset = bHorz ? 
			(rectGripper.Height () - nBoxesNumber * szBox.cy) / 2 :
			(rectGripper.Width () - nBoxesNumber * szBox.cx) / 2;

		for (int nBox = 0; nBox < nBoxesNumber; nBox++)
		{
			int x = bHorz ? 
				rectGripper.left : 
				rectGripper.left + nOffset;

			int y = bHorz ? 
				rectGripper.top + nOffset : 
				rectGripper.top;

            m_ToolBarGripper.DrawEx (pDC, CRect (CPoint (x, y), szBox), 0);

			nOffset += bHorz ? szBox.cy : szBox.cx;
		}
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);

	if (!CanDrawImage () || pBar == NULL || pBar->IsDialogControl ())
	{
		CBCGPVisualManager2003::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	ASSERT_VALID (pBar);

	CRect rectSeparator (rect);

#ifndef BCGP_EXCLUDE_RIBBON
	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonStatusBar)))
	{
		rect.InflateRect (1, 5);

		m_StatusBarPaneBorder.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzRight, 
			CBCGPToolBarImages::ImageAlignVertStretch);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonBar)) ||
		(bHorz && pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPanelMenuBar))))
	{
		if (rect.Width () < m_RibbonPanelSeparator.GetImageSize ().cx)
		{
			rect.left = rect.right - m_RibbonPanelSeparator.GetImageSize ().cx;
		}
		
		m_RibbonPanelSeparator.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzCenter,
			CBCGPToolBarImages::ImageAlignVertCenter);
		return;
	}
#endif

	BOOL bPopupMenu = FALSE;

	if (!bHorz)
	{
		BOOL bIsRibbon = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
		bIsRibbon = pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPanelMenuBar));
#endif
		bPopupMenu = pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

		if (bPopupMenu &&
			!bIsRibbon &&
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPColorBar)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalculator)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalendarBar)))
		{
			rectSeparator.left = rect.left + CBCGPToolBar::GetMenuImageSize ().cx + 
						GetMenuImageMargin () + 1;

			CRect rectBar;
			pBar->GetClientRect (rectBar);

			if (rectBar.right - rectSeparator.right < 50) // Last item in row
			{
				rectSeparator.right = rectBar.right;
			}

			if (((CBCGPPopupMenuBar*) pBar)->m_bDisableSideBarInXPMode)
			{
				rectSeparator.left = 0;
			}

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (bPopupMenu)
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenu)))
				{
					CBCGPPopupMenu* pMenu = (CBCGPPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						rectSeparator.left = rect.left + 2 * CBCGPToolBar::GetMenuImageSize ().cx + 
								3 * GetMenuImageMargin () + 2;
					}
				}
			}
		}
	}

	if (bPopupMenu)
	{
		DrawSeparator (pDC, rectSeparator, !bHorz);
	}
	else
	{
		if (bHorz)
		{
			int nHeight = rectSeparator.Height () / 5;
			rectSeparator.top    += nHeight;
			rectSeparator.bottom -= nHeight;
		}
		else
		{
			int nWidth = rectSeparator.Width () / 5;
			rectSeparator.left  += nWidth;
			rectSeparator.right -= nWidth;
		}

		DrawSeparator (pDC, rectSeparator, m_penSeparatorDark, m_penSeparatorLight, !bHorz);
	}
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons)
{
	if (!CanDrawImage () || pBar == NULL || pBar->IsDialogControl ())
	{
		return CBCGPVisualManager2003::OnDrawControlBarCaption (pDC, pBar, bActive, rectCaption, rectButtons);
	}

	ASSERT_VALID (pDC);

	CPen pen (PS_SOLID, 1, globalData.clrBarFace);
	CPen* pOldPen = pDC->SelectObject (&pen);

	rectCaption.bottom += 2;

	pDC->MoveTo (rectCaption.left, rectCaption.bottom);
	pDC->LineTo (rectCaption.left, rectCaption.top);

	pDC->MoveTo (rectCaption.left  + 1, rectCaption.top);
	pDC->LineTo (rectCaption.right - 1, rectCaption.top);

	pDC->MoveTo (rectCaption.right - 1, rectCaption.top + 1);
	pDC->LineTo (rectCaption.right - 1, rectCaption.bottom);

	pDC->SelectObject (pOldPen);

	rectCaption.DeflateRect (1, 1, 1, 0);
	pDC->FillRect (rectCaption, bActive ? &globalData.brActiveCaption : &globalData.brInactiveCaption);

	return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawStatusBarPaneBorder (pDC, pBar, rectPane, 
								uiID, nStyle);
		return;
	}	

	BOOL bExtended = pBar->GetDrawExtendedArea ();
	if (!bExtended || ((nStyle & SBPS_STRETCH) == 0 && bExtended))
	{
		rectPane.OffsetRect (1, 0);
		m_StatusBarPaneBorder.DrawEx (pDC, rectPane, 0, CBCGPToolBarImages::ImageAlignHorzRight, 
			CBCGPToolBarImages::ImageAlignVertStretch);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
			CRect rectSizeBox)
{
	if (!CanDrawImage () || 
        m_StatusBarSizeBox.GetCount () == 0)
	{
		CBCGPVisualManager2003::OnDrawStatusBarSizeBox (pDC, pStatBar, rectSizeBox);
		return;
	}

    m_StatusBarSizeBox.DrawEx (pDC, rectSizeBox, 0, CBCGPToolBarImages::ImageAlignHorzRight, CBCGPToolBarImages::ImageAlignVertBottom);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawComboDropButton (CDC* pDC, CRect rect,
							BOOL bDisabled,
							BOOL bIsDropped,
							BOOL bIsHighlighted,
							CBCGPToolbarComboBoxButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawComboDropButton (pDC, rect,
												bDisabled, bIsDropped,
												bIsHighlighted, pButton);
		return;
	}

	BOOL bRibbon = pButton != NULL && pButton->IsRibbonButton ();

	BOOL bActive = bIsHighlighted || bIsDropped;

	CBCGPControlRenderer* pRenderer = bRibbon 
		? &m_ctrlRibbonComboBoxBtn
		: &m_ctrlComboBoxBtn;

	if (!pRenderer->IsValid ())
	{
		COLORREF color1 = bRibbon 
							? m_clrRibbonComboBtnStart
							: m_clrComboBtnStart;
		COLORREF color2 = bRibbon 
							? m_clrRibbonComboBtnFinish
							: m_clrComboBtnFinish;
		COLORREF colorBorder = bRibbon 
							? m_clrRibbonComboBtnBorder
							: m_clrComboBtnBorder;
		if (bDisabled)
		{
			color1 = bRibbon 
							? m_clrRibbonComboBtnDisabledStart
							: m_clrComboBtnDisabledStart;
			color2 = bRibbon 
							? m_clrRibbonComboBtnDisabledFinish
							: m_clrComboBtnDisabledFinish;
			colorBorder = bRibbon 
							? m_clrRibbonComboBtnBorderDisabled
							: m_clrComboBtnBorderDisabled;
		}
		else if (bActive)
		{
			if (bIsDropped)
			{
				color1 = bRibbon 
								? m_clrRibbonComboBtnPressedStart
								: m_clrComboBtnPressedStart;
				color2 = bRibbon 
								? m_clrRibbonComboBtnPressedFinish
								: m_clrComboBtnPressedFinish;
				colorBorder = bRibbon 
								? m_clrRibbonComboBtnBorderPressed
								: m_clrComboBtnBorderPressed;
			}
			else
			{
				color1 = bRibbon 
								? m_clrRibbonComboBtnHighlightedStart
								: m_clrComboBtnHighlightedStart;
				color2 = bRibbon 
								? m_clrRibbonComboBtnHighlightedFinish
								: m_clrComboBtnHighlightedFinish;
				colorBorder = bRibbon 
								? m_clrRibbonComboBtnBorderHighlighted
								: m_clrComboBtnBorderHighlighted;
			}
		}

		if (bRibbon || 
			!bDisabled || 
			(bDisabled && colorBorder != (COLORREF)(-1)))
		{
			if (!bDisabled)
			{
				rect.InflateRect (0, 1, 1, 1);
			}

			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				dm.DrawRect (rect, (COLORREF)-1, colorBorder);
			}
			else
			{
				pDC->Draw3dRect (rect, colorBorder, colorBorder);
			}

			if (!bDisabled)
			{
				rect.DeflateRect (0, 1, 1, 1);
			}
		}

		if (bDisabled)
		{
			rect.DeflateRect (0, 1, 1, 1);
		}
		else if (bActive)
		{
			rect.DeflateRect (1, 0, 0, 0);
		}

		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, color1, color2, TRUE);

		if (bDisabled)
		{
			rect.InflateRect (0, 1, 1, 1);
		}
		else if (bActive)
		{
			rect.InflateRect (1, 0, 0, 0);
		}
	}
	else
	{
		rect.InflateRect (0, 1, 1, 1);

		int nIndex = 0;
		if (bDisabled)
		{
			nIndex = 3;
		}
		else
		{
			if (bIsDropped)
			{
				nIndex = 2;
			}
			else if (bIsHighlighted)
			{
				nIndex = 1;
			}
		}

		pRenderer->Draw (pDC, rect, nIndex);

		rect.DeflateRect (0, 1, 1, 1);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rect,
		bDisabled 
		? CMenuImages::ImageGray 
		: CMenuImages::ImageBlack);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawComboBorder (CDC* pDC, CRect rect,
							BOOL bDisabled,
							BOOL bIsDropped,
							BOOL bIsHighlighted,
							CBCGPToolbarComboBoxButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawComboBorder (pDC, rect,
												bDisabled,
												bIsDropped,
												bIsHighlighted,
												pButton);
		return;
	}

	rect.DeflateRect (1, 1);

    COLORREF colorBorder = m_clrComboBorder;

	if (bDisabled)
	{
		colorBorder = m_clrComboBorderDisabled;
	}
	else if (bIsHighlighted || bIsDropped)
	{
		colorBorder = bIsDropped
			? m_clrComboBorderPressed
			: m_clrComboBorderHighlighted;
	}

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, (COLORREF)-1, colorBorder);
	}
	else
	{
		pDC->Draw3dRect (&rect, colorBorder, colorBorder);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawEditBorder (CDC* pDC, CRect rect,
							BOOL bDisabled,
							BOOL bIsHighlighted,
							CBCGPToolbarEditBoxButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawEditBorder (pDC, rect,
												bDisabled,
												bIsHighlighted,
												pButton);
		return;
	}

	rect.DeflateRect (1, 1);

    COLORREF colorBorder = m_clrEditBorder;

	if (bDisabled)
	{
		colorBorder = m_clrEditBorderDisabled;
	}
	else if (bIsHighlighted)
	{
		colorBorder = m_clrEditBorderHighlighted;
	}

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, (COLORREF)-1, colorBorder);
	}
	else
	{
		pDC->Draw3dRect (&rect, colorBorder, colorBorder);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	if (!CanDrawImage () || 
        m_ToolBarTear.GetCount () == 0)
	{
		CBCGPVisualManager2003::OnDrawTearOffCaption (pDC, rect, bIsActive);
		return;
	}

    pDC->FillRect (rect, &m_brBarBkgnd);	
    if (bIsActive)
    {
		m_ctrlMenuHighlighted[0].Draw (pDC, rect);
    }

    m_ToolBarTear.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawMenuResizeBar (CDC* pDC, CRect rect, int nResizeFlags)
{
	CBCGPToolBarImages& images =
		(nResizeFlags == (int) CBCGPPopupMenu::MENU_RESIZE_BOTTOM_RIGHT) ?
	m_PopupResizeBar_HV :
	m_PopupResizeBar_V;	// TODO - vertical resize

	if (!CanDrawImage () ||
		!m_ctrlPopupResizeBar.IsValid () ||
		!images.IsValid ())
	{
		CBCGPVisualManager2003::OnDrawMenuResizeBar (pDC, rect, nResizeFlags);
		return;
	}

	ASSERT_VALID (pDC);

	m_ctrlPopupResizeBar.Draw (pDC, rect);

	if (nResizeFlags == (int) CBCGPPopupMenu::MENU_RESIZE_BOTTOM_RIGHT)
	{
		images.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzRight,
			CBCGPToolBarImages::ImageAlignVertBottom);
	}
	else
	{
		images.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzCenter,
			CBCGPToolBarImages::ImageAlignVertCenter);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST (CCustomizeButton, pButton);

	if (pCustButton == NULL)
	{
		if (CBCGPToolBar::IsCustomizeMode () && 
			!CBCGPToolBar::IsAltCustomizeMode () && !pButton->IsLocked ())
		{
			return;
		}

		CBCGPControlRenderer* pRenderer = NULL;
		int index = 0;

		BOOL bDisabled = (pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED;
		BOOL bPressed  = (pButton->m_nStyle & TBBS_PRESSED ) == TBBS_PRESSED;
		BOOL bChecked  = (pButton->m_nStyle & TBBS_CHECKED ) == TBBS_CHECKED;
		BOOL bHandled  = FALSE;

		CBCGPBaseControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar, pButton->GetParentWnd ());

		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL)
		{
			if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
			{
				if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
				{
					if (pMenuButton->IsDroppedDown ())
					{
						index = 1;
					}

					pRenderer = &m_ctrlMenuBarBtn;

					bHandled = TRUE;
				}
				else
				{
					return;
				}

				bHandled = TRUE;
			}
			else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
			{
				if (bChecked)
				{
					pRenderer = &m_ctrlMenuItemBack;

					if (bDisabled)
					{
						index = 1;
					}

					rect.InflateRect (0, 0, 0, 1);
					bHandled = TRUE;
				}
				else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
				{
					pRenderer = &m_ctrlMenuHighlighted[bDisabled ? 1 : 0];
					bHandled = TRUE;
				}
				else
				{
					return;
				}
			}
		}
		else if (pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPColorBar)))
		{
			if (bChecked)
			{
				pRenderer = &m_ctrlMenuItemBack;

				if (bDisabled)
				{
					index = 1;
				}
			}


			if (!bDisabled)
			{
				if (state == ButtonsIsHighlighted)
				{
					pRenderer = &m_ctrlMenuHighlighted[0];
					index = 0;
				}
			}

			bHandled = TRUE;
		}
		else if (pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPOutlookBarToolBar)))
		{
			bHandled = TRUE;
		}

		if (!bHandled)
		{
			index = -1;

			if (bChecked)
			{
				if (bDisabled)
				{
					index = 0;
				}
				else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
				{
					index = 3;
				}
			}

			if (!bDisabled)
			{
				if (bPressed)
				{
					index = 2;
				}
				else if (state == ButtonsIsHighlighted)
				{
					if (index == -1)
					{
						index = 0;
					}

					index++;
				}
			}

			if (index == -1)
			{
				return;
			}

			pRenderer = &m_ctrlToolBarBtn;
		}

		if (pRenderer != NULL)
		{
			pRenderer->Draw (pDC, rect, index);
			return;
		}
	}

	CBCGPVisualManager2003::OnFillButtonInterior (pDC, pButton, rect, state);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawButtonBorder (pDC, pButton, rect, state);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL bHorz)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawButtonSeparator (pDC, pButton, rect, state, bHorz);
		return;
	}

	CPen* pPen = &m_penMenuItemBorder;

	CPen* pOldPen = pDC->SelectObject (pPen);
	ASSERT (pOldPen != NULL);

	if (bHorz)
	{
		pDC->MoveTo (rect.left, rect.top + 2);
		pDC->LineTo (rect.left, rect.bottom - 2);
	}
	else
	{
		pDC->MoveTo (rect.left  + 2, rect.top);
		pDC->LineTo (rect.right - 2, rect.top);
	}

	pDC->SelectObject (pOldPen);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnHighlightMenuItem (pDC, pButton, rect, clrText);
		return;
	}

	m_ctrlMenuHighlighted[(pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED ? 1 : 0].Draw (pDC, rect);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnHighlightRarelyUsedMenuItems (pDC, rectRarelyUsed);
	}

	rectRarelyUsed.left --;
	rectRarelyUsed.right = rectRarelyUsed.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * GetMenuImageMargin () + 2;

	pDC->FillRect (rectRarelyUsed, &m_brMenuRarelyUsed);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton, 
		CRect rect, BOOL bHighlight, BOOL bIsRadio)
{
	ASSERT_VALID (pButton);

    CBCGPToolBarImages& img = bIsRadio ? m_MenuItemMarkerR : m_MenuItemMarkerC;

	if (!CanDrawImage () || 
        img.GetCount () == 0)
	{
		CBCGPVisualManager2003::OnDrawMenuCheck (pDC, pButton, rect, bHighlight, bIsRadio);
		return;
	}

    CSize size (img.GetImageSize ());
    CRect rectImage (0, 0, size.cx, size.cy);

    if ((pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED)
    {
        rectImage.OffsetRect (0, size.cy);
    }

    img.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter, rectImage);
}
//************************************************************************************
void CBCGPVisualManager2007::OnDrawMenuItemButton (CDC* pDC, CBCGPToolbarMenuButton* pButton,
				CRect rectButton, BOOL bHighlight, BOOL bDisabled)
{
	if (!CanDrawImage () || !m_ctrlMenuButtonBorder.IsValid ())
	{
		CBCGPVisualManager2003::OnDrawMenuItemButton (pDC, pButton,
				rectButton, bHighlight, bDisabled);
		return;
	}

	ASSERT_VALID (pDC);

	CRect rect = rectButton;
	rect.right = rect.left + 1;
	rect.left--;
	rect.DeflateRect (0, 1);

	if (bHighlight)
	{
		m_ctrlMenuButtonBorder.Draw (pDC, rect);
	}
	else
	{
		CBrush br (globalData.clrBtnShadow);

		rect.DeflateRect (0, 3);
		rect.right--;
		pDC->FillRect (rect, &br);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawShowAllMenuItems (CDC* pDC, CRect rect, 
												 CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage () || !m_ctrlMenuItemShowAll.IsValid ())
	{
		CBCGPVisualManager2003::OnDrawShowAllMenuItems (pDC, rect, state);
		return;
	}

	m_ctrlMenuItemShowAll.FillInterior (pDC, rect, CBCGPToolBarImages::ImageAlignHorzCenter,
		CBCGPToolBarImages::ImageAlignVertCenter, state == ButtonsIsHighlighted ? 1 : 0);
}
//*****************************************************************************
int CBCGPVisualManager2007::GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault)
{
	return (CanDrawImage () && m_ctrlMenuItemShowAll.IsValid ())
		? m_ctrlMenuItemShowAll.GetParams ().m_rectImage.Size ().cy + 2 * TEXT_MARGIN
		: CBCGPVisualManager2003::GetShowAllMenuItemsHeight (pDC, sizeDefault);
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd, BOOL bActive)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillMiniFrameCaption (pDC, 
								rectCaption, pFrameWnd, bActive);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	BOOL bIsToolBar = FALSE;

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = FALSE;
		bIsToolBar = TRUE;
	}

	pDC->FillRect (rectCaption, 
		bActive ? &globalData.brActiveCaption : &globalData.brInactiveCaption);

    // get the text color
	return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawMiniFrameBorder (
										pDC, pFrameWnd,
										rectBorder, rectBorderSize);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

#ifndef BCGP_EXCLUDE_TASK_PANE
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
#else
	BOOL bIsTasksPane = FALSE;
#endif

	if (bIsTasksPane)
	{
		CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
		ASSERT (pOldBrush != NULL);

		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

		rectBorderSize.DeflateRect (2, 2);
		rectBorder.DeflateRect (2, 2);

		pDC->SelectObject (&globalData.brBarFace);

		pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 1, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 1, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

		pDC->SelectObject (pOldBrush);
	}
	else
	{
		CBCGPVisualManager2003::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* pToolBar, 
												CRect rectBorder, CRect rectBorderSize)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawFloatingToolbarBorder (pDC, pToolBar, 
												rectBorder, rectBorderSize);
		return;
	}

	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brBarFace);

	pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 1, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 1, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::IsOwnerDrawMenuCheck ()
{
	return CanDrawImage () ? FALSE : CBCGPVisualManager2003::IsOwnerDrawMenuCheck ();
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::IsHighlightWholeMenuItem ()
{
	return CanDrawImage () ? TRUE : CBCGPVisualManager2003::IsHighlightWholeMenuItem ();
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::GetStatusBarPaneTextColor (CBCGPStatusBar* pStatusBar, 
									CBCGStatusBarPaneInfo* pPane)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetStatusBarPaneTextColor (pStatusBar, pPane);
	}

	ASSERT (pPane != NULL);

	return (pPane->nStyle & SBPS_DISABLED) ? m_clrStatusBarTextDisabled : 
			pPane->clrText == (COLORREF)-1 ? m_clrStatusBarText : pPane->clrText;
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::GetToolbarButtonTextColor (
	CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetToolbarButtonTextColor (pButton, state);
	}

	ASSERT_VALID (pButton);

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
		(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	if (pButton->GetParentWnd () != NULL && 
		pButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
	{
		return bDisabled
					? m_clrMenuBarBtnTextDisabled
					: ((state == ButtonsIsHighlighted || state == ButtonsIsPressed ||
						pButton->IsDroppedDown ())
						? m_clrMenuBarBtnTextHighlighted
						: m_clrMenuBarBtnText);
	}

	return bDisabled
				? m_clrToolBarBtnTextDisabled
				: ((state == ButtonsIsHighlighted || state == ButtonsIsPressed)
					? m_clrToolBarBtnTextHighlighted
					: m_clrToolBarBtnText);
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::GetMenuItemTextColor (
	CBCGPToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetMenuItemTextColor (pButton, 
			bHighlighted, bDisabled);
	}

	return bDisabled ? m_clrMenuTextDisabled : m_clrMenuText;
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetHighlightedMenuItemTextColor (pButton);
	}

	return m_clrMenuTextHighlighted;
}
//**********************************************************************************
void CBCGPVisualManager2007::GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::GetTabFrameColors (pTabWnd,
				   clrDark, clrBlack, clrHighlight, clrFace,
				   clrDarkShadow, clrLight, pbrFace, pbrBlack);
		return;
	}

	ASSERT_VALID (pTabWnd);

	CBCGPVisualManager2003::GetTabFrameColors (pTabWnd,
				   clrDark,
				   clrBlack,
				   clrHighlight,
				   clrFace,
				   clrDarkShadow,
				   clrLight,
				   pbrFace,
				   pbrBlack);

	if (pTabWnd->IsFlatTab () && !pTabWnd->IsDialogControl ())
	{
		if (m_clrTabFlatBlack != CLR_DEFAULT)
		{
			clrBlack = m_clrTabFlatBlack;
		}

		if (m_clrTabFlatHighlight != CLR_DEFAULT)
		{
			clrHighlight = m_clrTabFlatHighlight;
		}
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (!CanDrawImage () || pTabWnd->IsDialogControl ())
	{
		CBCGPVisualManager2003::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	if(pTabWnd->IsOneNoteStyle () ||
	   pTabWnd->IsColored () ||
	   pTabWnd->IsVS2005Style () ||
	   pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManager2003::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	const BOOL bBottom = pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM;

	if (pTabWnd->IsFlatTab ())
	{
		m_ctrlTabFlat[bBottom ? 1 : 0].Draw (pDC, rect);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		COLORREF clr1 = m_clrBarGradientDark;
		COLORREF clr2 = m_clrBarGradientLight;

		if (bBottom)
		{
			dm.FillGradient (rect, clr1, clr2, TRUE);
		}
		else
		{
			dm.FillGradient (rect, clr2, clr1, TRUE);
		}
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	if(pTabWnd->IsOneNoteStyle () ||
	   pTabWnd->IsColored () ||
	   pTabWnd->IsVS2005Style () ||
	   pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManager2003::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	const BOOL bBottom = pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM;
	const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab ();

	COLORREF clrText = pTabWnd->GetTabTextColor (iTab);
	
	if (pTabWnd->IsFlatTab ())
	{
		int nImage = (bIsActive || bIsHighlight) ? 2 : 1;

		CRgn rgn;

		POINT pts[4];

		if (bBottom)
		{
			rectTab.bottom++;

			pts[0].x = rectTab.left;
			pts[0].y = rectTab.bottom + 1;
			pts[1].x = rectTab.left;
			pts[1].y = rectTab.top;
			pts[2].x = rectTab.right + 1;
			pts[2].y = rectTab.top;
			pts[3].x = rectTab.right - rectTab.Height () + 1;
			pts[3].y = rectTab.bottom + 1;

			rectTab.top++;
		}
		else
		{
			pts[0].x = rectTab.left;
			pts[0].y = rectTab.bottom + 1;
			pts[1].x = rectTab.left;
			pts[1].y = rectTab.top;
			pts[2].x = rectTab.right - rectTab.Height () + 1;
			pts[2].y = rectTab.top;
			pts[3].x = rectTab.right + 1;
			pts[3].y = rectTab.bottom + 1;
		}

		rgn.CreatePolygonRgn (pts, 4, WINDING);

		int isave = pDC->SaveDC ();

		pDC->SelectClipRgn (&rgn, RGN_AND);

		m_ctrlTabFlat[bBottom ? 1 : 0].Draw (pDC, rectTab, nImage);

		CPen* pOldPen = pDC->SelectObject (&m_penTabFlatOuter[bIsActive ? 1 : 0]);

		if (bBottom)
		{
			pDC->MoveTo (pts[2].x, pts[2].y);
			pDC->LineTo (pts[3].x, pts[3].y - 1);
		}
		else
		{
			pDC->MoveTo (pts[2].x - 1, pts[2].y);
			pDC->LineTo (pts[3].x - 1, pts[3].y - 1);
		}

		pDC->SelectObject (&m_penTabFlatInner[bIsActive ? 1 : 0]);

		if (bBottom)
		{
			pDC->MoveTo (pts[2].x - 2, pts[2].y + 1);
			pDC->LineTo (pts[3].x, pts[3].y - 2);
		}
		else
		{
			pDC->MoveTo (pts[2].x - 1, pts[2].y + 1);
			pDC->LineTo (pts[3].x - 2, pts[3].y - 1);
		}

		pDC->SelectObject (pOldPen);

		pDC->SelectClipRgn (NULL);
		
		clrText = globalData.clrBarText;
		pDC->RestoreDC (isave);
	}
	else
	{
		if(clrText == (COLORREF)-1)
		{
			clrText = bIsActive
							? m_clrMenuBarBtnTextHighlighted
							: m_clrMenuBarBtnText;
		}

		int nImage = bIsActive ? 3 : 0;
		if(bIsHighlight)
		{
			nImage += 1;
		}

		m_ctrlTab3D[bBottom ? 1 : 0].Draw (pDC, rectTab, nImage);

		if (pTabWnd->IsDialogControl ())
		{
			clrText = globalData.clrBtnText;
		}
	}

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);

	if (!CanDrawImage () || pTabWnd->IsDialogControl ())
	{
		CBCGPVisualManager2003::OnFillTab (pDC, rectFill, pbrFill,
									 iTab, bIsActive, pTabWnd);
		return;
	}

	if(pTabWnd->IsFlatTab () ||
	   pTabWnd->IsOneNoteStyle () ||
	   pTabWnd->IsColored () ||
	   pTabWnd->IsVS2005Style () ||
	   pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManager2003::OnFillTab (pDC, rectFill, pbrFill,
									 iTab, bIsActive, pTabWnd);
		return;
	}

	ASSERT_VALID (pDC);

	const BOOL bBottom = pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM;
	const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab ();

	BOOL bIsBeta1 = IsBeta1 ();
	if (!bIsBeta1 || (bIsActive || bIsHighlight))
	{
		int nImage = bIsActive 
						? (bIsBeta1 ? 2 : 3)
						: (bIsBeta1 ? -1 : 0);
		if(bIsHighlight)
		{
			nImage += 1;
		}
		
		m_ctrlTab3D[bBottom ? 1 : 0].Draw (pDC, rectFill, nImage);
	}
}
//*****************************************************************************
BOOL CBCGPVisualManager2007::OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd)
{	
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (!CanDrawImage () || pTabWnd->IsDialogControl ())
	{
		return CBCGPVisualManager2003::OnEraseTabsFrame (pDC, rect, pTabWnd);
	}

	if(pTabWnd->IsOneNoteStyle () ||
	   pTabWnd->IsColored () ||
	   pTabWnd->IsVS2005Style () ||
	   pTabWnd->IsLeftRightRounded ())
	{
		return CBCGPVisualManager2003::OnEraseTabsFrame (pDC, rect, pTabWnd);
	}

	if (pTabWnd->IsFlatTab ())
	{
		pDC->FillRect (rect, &globalData.brWindow);

		if (pTabWnd->GetLocation () != CBCGPTabWnd::LOCATION_BOTTOM)
		{
			CPen pen (PS_SOLID, 1, m_clrTabFlatBlack);
			CPen* pOldPen = pDC->SelectObject (&pen);

			pDC->MoveTo (rect.left, rect.top + pTabWnd->GetTabsHeight () + 1);
			pDC->LineTo (rect.right, rect.top + pTabWnd->GetTabsHeight () + 1);

			pDC->SelectObject (pOldPen);
		}

		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pBaseTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pBaseTab);

	CBCGPTabWnd* pWndTab = DYNAMIC_DOWNCAST (CBCGPTabWnd, pBaseTab);

	if (!CanDrawImage () || 
		pWndTab == NULL || 
		pBaseTab->IsDialogControl ())
	{
		CBCGPVisualManager2003::OnEraseTabsButton (pDC, rect, pButton, pBaseTab);
		return;
	}

	if(pBaseTab->IsFlatTab () || 
	   pBaseTab->IsOneNoteStyle () ||
	   pBaseTab->IsColored () ||
	   pBaseTab->IsVS2005Style () ||
	   pBaseTab->IsLeftRightRounded () ||
	   (!pButton->IsPressed () && !pButton->IsHighlighted ()))
	{
		CBCGPVisualManager2003::OnEraseTabsButton (pDC, rect, pButton, pBaseTab);
		return;
	}

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rect);

	pDC->SelectClipRgn (&rgn);

	CRect rectTabs;
	pWndTab->GetClientRect (&rectTabs);

	CRect rectTabArea;
	pWndTab->GetTabsRect (rectTabArea);

	if (pWndTab->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		rectTabs.top = rectTabArea.top;
	}
	else
	{
		rectTabs.bottom = rectTabArea.bottom;
	}

	pWndTab->MapWindowPoints (pButton, rectTabs);
	OnEraseTabsArea (pDC, rectTabs, pWndTab);

	pDC->SelectClipRgn (NULL);

	int index = pButton->IsPressed () ? 2 : 1;
	m_ctrlToolBarBtn.Draw (pDC, rect, index);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT uiState,
												 CBCGPBaseTabWnd* pWndTab)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawTabsButtonBorder(pDC, rect, 
												 pButton, uiState,
												 pWndTab);
	}
}


#ifndef BCGP_EXCLUDE_TASK_PANE
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawTasksGroupCaption(
										CDC* pDC, CBCGPTasksGroup* pGroup, 
										BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/, 
										BOOL bCanCollapse /*= FALSE*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (pGroup);
	ASSERT_VALID (pGroup->m_pPage);

	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawTasksGroupCaption(
										pDC, pGroup, 
										bIsHighlighted, bIsSelected, bCanCollapse);
		return;
	}

#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox = pGroup->m_pPage->m_pTaskPane != NULL &&
		(pGroup->m_pPage->m_pTaskPane->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)));
#else
	BOOL bIsToolBox = FALSE;
#endif

	CRect rectGroup = pGroup->m_rect;

	// -----------------------
	// Draw caption background
	// -----------------------
	if (bIsToolBox)
	{
		CRect rectFill = rectGroup;
		rectFill.DeflateRect (1, 1);
		rectFill.bottom--;

		COLORREF clrGrdaient1 = CBCGPDrawManager::PixelAlpha (
			m_clrToolBarGradientDark, 105);
		COLORREF clrGrdaient2 = CBCGPDrawManager::PixelAlpha (
			m_clrToolBarGradientDark, 120);

		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectFill, clrGrdaient1, clrGrdaient2, TRUE);

		CBrush brFillBottom (CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientDark, 120));
		
		CRect rectFillBottom = rectGroup;
		rectFillBottom.DeflateRect (1, 0);
		rectFillBottom.top = rectFillBottom.bottom - 1;

		pDC->FillRect (rectFillBottom, &brFillBottom);

		if (bCanCollapse)
		{
			//--------------------
			// Draw expanding box:
			//--------------------
			const int nBoxSize = 9;
			const int nBoxOffset = 6;

			CRect rectButton = rectFill;
			
			rectButton.left += nBoxOffset;
			rectButton.right = rectButton.left + nBoxSize;
			rectButton.top = rectButton.CenterPoint ().y - nBoxSize / 2;
			rectButton.bottom = rectButton.top + nBoxSize;

			OnDrawExpandingBox (pDC, rectButton, !pGroup->m_bIsCollapsed, 
				globalData.clrBarText);

			rectGroup.left = rectButton.right + nBoxOffset;
			bCanCollapse = FALSE;
		}
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		if (pGroup->m_bIsSpecial)
		{
			if (IsBeta1 ())
			{
				dm.FillGradient (pGroup->m_rect, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight,
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark : m_clrTaskPaneGroupCaptionSpecDark,
					TRUE);
			}
			else
			{
				dm.Fill4ColorsGradient (pGroup->m_rect, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark  : m_clrTaskPaneGroupCaptionSpecDark, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark  : m_clrTaskPaneGroupCaptionSpecDark,
					FALSE);
			}
		}
		else
		{
			if (IsBeta1 ())
			{
				dm.FillGradient (pGroup->m_rect, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark : m_clrTaskPaneGroupCaptionDark, 
					TRUE);
			}
			else
			{
				dm.Fill4ColorsGradient (pGroup->m_rect, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark  : m_clrTaskPaneGroupCaptionDark, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight, 
					bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark  : m_clrTaskPaneGroupCaptionDark,
					FALSE);
			}
		}
	}

	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL 
		&& pGroup->m_sizeIcon.cx < rectGroup.Width () - rectGroup.Height());
	if (bShowIcon)
	{
		OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
	}

	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject (&globalData.fontBold);
	COLORREF clrTextOld = pDC->GetTextColor();

	if (bIsToolBox)
	{
		pDC->SetTextColor (globalData.clrBarText);
	}
	else
	{
		if (bCanCollapse && bIsHighlighted)
		{
			pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ?
                (pGroup->m_bIsSpecial ? m_clrTaskPaneGroupCaptionTextHighSpec : m_clrTaskPaneGroupCaptionTextHigh) :
				pGroup->m_clrTextHot);
		}
		else
		{
			pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ?
				(pGroup->m_bIsSpecial ? m_clrTaskPaneGroupCaptionTextSpec : m_clrTaskPaneGroupCaptionText) :
				pGroup->m_clrText);
		}
	}

	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);

	CRect rectText = rectGroup;
	rectText.left += (bShowIcon ? pGroup->m_sizeIcon.cx	+ 5: nCaptionHOffset);
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, 
						rectText.right - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CMenuImages::Size();
		CRect rectButton = rectGroup;
		rectButton.left = max(rectButton.left, 
			rectButton.right - (rectButton.Height() + 1) / 2 - (sizeButton.cx + 1) / 2);
		rectButton.top = max(rectButton.top, 
			rectButton.bottom - (rectButton.Height() + 1) / 2 - (sizeButton.cy + 1) / 2);
		rectButton.right = rectButton.left + sizeButton.cx;
		rectButton.bottom = rectButton.top + sizeButton.cy;

		if (rectButton.right <= rectGroup.right && rectButton.bottom <= rectGroup.bottom)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject (&globalData.brBarFace);
				COLORREF clrBckOld = pDC->GetBkColor ();

				pDC->Draw3dRect(&rectButton, globalData.clrWindow, globalData.clrBarShadow);

				pDC->SetBkColor (clrBckOld);
				pDC->SelectObject (pBrushOld);
			}

			CMenuImages::Draw(pDC, 
				pGroup->m_bIsCollapsed 
					? CMenuImages::IdArowDown
					: CMenuImages::IdArowUp, 
				rectButton.TopLeft(),
				CMenuImages::ImageBlack);
		}
	}
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
							BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/)
{
	ASSERT_VALID (pTask);

	if (CanDrawImage () && pTask->m_bIsSeparator)
	{
        DrawSeparator (pDC, pTask->m_rect, TRUE);
        return;
	}

	CBCGPVisualManager2003::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, 
		bIsSelected);
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawScrollButtons(pDC, rect, nBorderSize, iImage, bHilited);
		return;
	}

	CRect rt (rect);
	rt.top--;
	m_ctrlTaskScrollBtn.Draw (pDC, rt, bHilited ? 1 : 0);

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, rect);
}

#endif	// BCGP_EXCLUDE_TASK_PANE

//****************************************************************************************
void CBCGPVisualManager2007::OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
		CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawHeaderCtrlBorder (pCtrl, pDC, rect, bIsPressed, bIsHighlighted);
		return;
	}

	COLORREF clrStart  = m_clrHeaderNormalStart;
	COLORREF clrFinish = m_clrHeaderNormalFinish;
	COLORREF clrBorder = m_clrHeaderNormalBorder;

	if (bIsPressed)
	{
		clrStart  = m_clrHeaderPressedStart;
		clrFinish = m_clrHeaderPressedFinish;
		clrBorder = m_clrHeaderPressedBorder;
	}
	else if (bIsHighlighted)
	{
		clrStart  = m_clrHeaderHighlightedStart;
		clrFinish = m_clrHeaderHighlightedFinish;
		clrBorder = m_clrHeaderHighlightedBorder;
	}

	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, clrFinish, clrStart);
	}

	CPen pen (PS_SOLID, 0, clrBorder);
	CPen* pOldPen = pDC->SelectObject (&pen);
	
	if (bIsPressed || bIsHighlighted)
	{
		pDC->MoveTo (rect.right - 1, rect.top);
		pDC->LineTo (rect.right - 1, rect.bottom - 1);
		pDC->LineTo (rect.left, rect.bottom - 1);
		pDC->LineTo (rect.left, rect.top - 1);
	}
	else
	{
		pDC->MoveTo (rect.right - 1, rect.top);
		pDC->LineTo (rect.right - 1, rect.bottom - 1);
		pDC->LineTo (rect.left - 1, rect.bottom - 1);
	}

	pDC->SelectObject (pOldPen);
}

#ifndef BCGP_EXCLUDE_GRID_CTRL

void CBCGPVisualManager2007::OnFillGridHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillGridHeaderBackground (pCtrl, pDC, rect);
		return;
	}

	ASSERT_VALID(pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
}
//********************************************************************************
BOOL CBCGPVisualManager2007::OnDrawGridHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawGridHeaderItemBorder (pCtrl, pDC, rect, bPressed);
	}

	ASSERT_VALID(pDC);

	COLORREF clrStart  = bPressed ? m_clrGridHeaderPressedStart : m_clrGridHeaderNormalStart;
	COLORREF clrFinish = bPressed ? m_clrGridHeaderPressedFinish : m_clrGridHeaderNormalFinish;
	COLORREF clrBorder = bPressed ? m_clrGridHeaderPressedBorder : m_clrGridHeaderNormalBorder;

	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, clrFinish, clrStart);
	}

	CPen* pOldPen = pDC->SelectObject (&m_penGridSeparator);

	int nHeight = rect.Height () / 5;

	pDC->MoveTo (rect.right - 1, rect.top + nHeight);
	pDC->LineTo (rect.right - 1, rect.bottom - nHeight);

	pDC->SelectObject (pOldPen);

	{
		CPen pen (PS_SOLID, 1, clrBorder);
		pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rect.left, rect.top);
		pDC->LineTo (rect.right, rect.top);

		pDC->MoveTo (rect.left, rect.bottom - 1);
		pDC->LineTo (rect.right, rect.bottom - 1);

		pDC->SelectObject (pOldPen);
	}

	return FALSE;
}
//********************************************************************************
COLORREF CBCGPVisualManager2007::OnFillGridGroupByBoxBackground (CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillGridGroupByBoxBackground (pDC, rect);
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);

	return globalData.clrBarText;
}
//********************************************************************************
void CBCGPVisualManager2007::OnDrawGridGroupByBoxItemBorder (CBCGPGridCtrl* pCtrl,
														 CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawGridGroupByBoxItemBorder (pCtrl,
														 pDC, rect);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clrStart  = m_clrGridHeaderNormalStart;
	COLORREF clrFinish = m_clrGridHeaderNormalFinish;
	COLORREF clrBorder = m_clrGridHeaderNormalBorder;

	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, clrFinish, clrStart);
	}

	pDC->Draw3dRect (rect, clrBorder, clrBorder);
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::GetGridLeftOffsetColor (CBCGPGridCtrl* pCtrl)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetGridLeftOffsetColor (pCtrl);
	}

	return m_clrGridLeftOffset;
}
//********************************************************************************
COLORREF CBCGPVisualManager2007::OnFillGridRowBackground (CBCGPGridCtrl* pCtrl, 
												  CDC* pDC, CRect rectFill, BOOL bSelected)
{
	COLORREF clr = CBCGPVisualManager2003::OnFillGridRowBackground (pCtrl, 
											  pDC, rectFill, bSelected);

	if (CanDrawImage ())
	{
		clr = pCtrl->IsHighlightGroups ()
				? (pCtrl->IsControlBarColors ()
					? globalData.clrBarText
					: globalData.clrBtnShadow)
				: globalData.clrWindowText;
	}

	return clr;
}

#endif // BCGP_EXCLUDE_GRID_CTRL

void CBCGPVisualManager2007::OnDrawCheckBoxEx (CDC *pDC, CRect rect, 
										 int nState,
										 BOOL bHighlighted, 
										 BOOL bPressed,
										 BOOL bEnabled)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawCheckBoxEx (pDC, rect, nState, bHighlighted, bPressed, bEnabled);
		return;
	}

	int index = nState * 4;

	if (!bEnabled)
	{
		index += 3;
	}
	else if (bPressed)
	{
		if (bHighlighted)
		{
			index += 2;
		}
	}
	else if (bHighlighted)
	{
		index += 1;
	}

	m_ctrlRibbonBtnCheck.FillInterior (pDC, rect, 
		CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter,
		index);
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManager2007::OnDrawRibbonCaption (CDC* pDC, CBCGPRibbonBar* pBar,
											  CRect rectCaption, CRect rectText)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonCaption (pDC, pBar, rectCaption, rectText);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	CWnd* pWnd = pBar->GetParent ();
	ASSERT_VALID (pWnd);

	const DWORD dwStyle   = pWnd->GetStyle ();
	const DWORD dwStyleEx = pWnd->GetExStyle ();

	const BOOL bIsRTL     = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
	const BOOL bActive    = IsWindowActive (pWnd);
	const BOOL bGlass	  = pBar->IsTransparentCaption ();

	int nExtraWidth = 0;

    {
		CSize szSysBorder (GetSystemBorders (TRUE));
		CRect rectCaption1 (rectCaption);
		CRect rectBorder (m_ctrlMainBorderCaption.GetParams ().m_rectSides);
		CRect rectQAT = pBar->GetQuickAccessToolbarLocation ();

		if (rectQAT.left > rectQAT.right)
		{
			rectText.left = rectQAT.left + 1;
		}

		rectCaption1.InflateRect (szSysBorder.cx, szSysBorder.cy, szSysBorder.cx, 0);

		CRect rectQA (rectCaption1);
		rectQA.right = rectQA.left;

		if (!bGlass)
		{
			rectQA.top = rectCaption.top - 4;
		}

		BOOL bHide  = (pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ALL) != 0;
		BOOL bExtra = (!bHide && pBar->IsQuickAccessToolbarOnTop () &&
					rectQAT.left < rectQAT.right) && 
					(!pBar->IsQATEmpty() || IsBeta1 ());

		if (bExtra)
		{
			nExtraWidth = GetRibbonQATRightMargin ();
			rectQA.right += rectQAT.right;
		}
		else
		{
			rectQA.right += rectBorder.left;

			if (!bHide)
			{
				rectQA.right += m_RibbonBtnMain.GetParams ().m_rectImage.Width ();
			}
		}

		rectQA.right += nExtraWidth;

		if (IsBeta1 ())
		{
			COLORREF clr1  = bActive 
								? m_clrAppCaptionActiveStart 
								: m_clrAppCaptionInactiveStart;
			COLORREF clr2  = bActive 
								? m_clrAppCaptionActiveFinish 
								: m_clrAppCaptionInactiveFinish;

			CRect rectCaption2 (rectCaption1);
			rectCaption2.DeflateRect (rectBorder.left, rectBorder.top, 
				rectBorder.right, rectBorder.bottom);

			if (!bGlass)
			{
				CBCGPDrawManager dm (*pDC);
				dm.Fill4ColorsGradient (rectCaption2, clr1, clr2, clr2, clr1, FALSE);
			}

			m_ctrlMainBorderCaption.DrawFrame (pDC, rectCaption1, bActive ? 0 : 1);
		}
        else if (!bGlass)
        {
			m_ctrlMainBorderCaption.Draw (pDC, rectCaption1, bActive ? 0 : 1);
        }

		if (bExtra)
		{
			if (!IsBeta1 ())
			{
				rectQA.right = pBar->GetQATCommandsLocation ().right + nExtraWidth;
			}

			CBCGPControlRenderer* pCaptionQA = bGlass 
				? &m_ctrlRibbonCaptionQA_Glass
				: &m_ctrlRibbonCaptionQA;

			if (pCaptionQA->IsValid ())
			{
				CRect rectQAFrame = rectQA;

				if (bGlass)
				{
					const int dxFrame = GetSystemMetrics (SM_CXSIZEFRAME) / 2;
					const int dyFrame = GetSystemMetrics (SM_CYSIZEFRAME) / 2;

					rectQAFrame.DeflateRect (dxFrame, dyFrame + 1, dxFrame, 0);
				}

				pCaptionQA->Draw (pDC, rectQAFrame, bActive ? 0 : 1);
			}
		}
		else if (bHide)
		{
			HICON hIcon = globalUtils.GetWndIcon (pWnd);

			if (hIcon != NULL)
			{
				CSize szIcon (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON));

				CRect rectIcon (rectCaption.TopLeft (), 
					CSize (min(::GetSystemMetrics (SM_CYCAPTION), rectCaption.Height ()), rectCaption.Height ()));

				long x = rectCaption.left + max (0, (rectIcon.Width ()  - szIcon.cx) / 2);
				long y = rectCaption.top  + max (0, (rectIcon.Height () - szIcon.cy) / 2);

				::DrawIconEx (pDC->GetSafeHdc (), x, y, hIcon, szIcon.cx, szIcon.cy,
					0, NULL, DI_NORMAL);

				if (rectText.left < rectIcon.right)
				{
					rectText.left = rectIcon.right;
				}
			}
		}
    }

	CString strText;
	pWnd->GetWindowText (strText);

	CFont* pOldFont = (CFont*)pDC->SelectObject (&m_AppCaptionFont);
	ASSERT (pOldFont != NULL);

	if (bGlass)
	{
		DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_CENTER |
			(bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);

		DrawTextOnGlass (pDC, strText, rectText, dwTextStyle, 10);
	}
	else
	{
		CString strTitle (strText);
		CString strDocument;

		BOOL bPrefix = FALSE;
		if ((dwStyle & FWS_ADDTOTITLE) == FWS_ADDTOTITLE)
		{
			bPrefix = (dwStyle & FWS_PREFIXTITLE) == FWS_PREFIXTITLE;
			CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);

			if (pFrameWnd != NULL)
			{
				strTitle = pFrameWnd->GetTitle();

				if (!strTitle.IsEmpty ())
				{
					int pos = strText.Find (strTitle);

					if (pos != -1)
					{
						if (strText.GetLength () > strTitle.GetLength ())
						{
							if (pos == 0)
							{
								bPrefix = FALSE; // avoid exception
								strTitle = strText.Left (strTitle.GetLength () + 3);
								strDocument = strText.Right (strText.GetLength () - strTitle.GetLength ());
							}
							else
							{
								strTitle = strText.Right (strTitle.GetLength () + 3);
								strDocument = strText.Left (strText.GetLength () - strTitle.GetLength ());
							}
						}
					}
				}
				else
				{
					strDocument = strText;
				}
			}
		}

		DrawNcText (pDC, rectText, strTitle, strDocument, bPrefix, bActive, bIsRTL, m_bNcTextCenter);
	}

	pDC->SelectObject (pOldFont);
}
//*****************************************************************************
int CBCGPVisualManager2007::GetRibbonQATRightMargin ()
{
	if (!CanDrawImage () ||
		!m_ctrlRibbonCaptionQA.IsValid ())
	{
		return CBCGPVisualManager2003::GetRibbonQATRightMargin ();
	}

	return m_ctrlRibbonCaptionQA.GetParams ().m_rectSides.right;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonCaptionButton (
					CDC* pDC, CBCGPRibbonCaptionButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonCaptionButton (pDC, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	const BOOL bHighlighted = pButton->IsHighlighted ();
	const BOOL bPressed = pButton->IsPressed ();

	BCGBUTTON_STATE state = ButtonsIsRegular;
	if (bPressed)
	{
		if (bHighlighted)
		{
			state = ButtonsIsPressed;
		}
	}
	else if (bHighlighted)
	{
		state = ButtonsIsHighlighted;
	}

	const BOOL bMDI = pButton->IsMDIChildButton ();
	BOOL bActive = TRUE;

	if (!bMDI)
	{
		CBCGPRibbonBar* pBar = pButton->GetParentRibbonBar ();
		if (pBar->GetSafeHwnd () != NULL)
		{
			CWnd* pWnd = pBar->GetParent ();
			ASSERT_VALID (pWnd);

			bActive = IsWindowActive (pWnd);
		}
	}

	DrawNcBtn (pDC, pButton->GetRect (), pButton->GetID (), state, FALSE, bActive, pButton->IsMDIChildButton ());
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonButtonsGroup (
					CDC* pDC, CBCGPRibbonButtonsGroup* pGroup,
					CRect rectGroup)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawRibbonButtonsGroup (pDC, pGroup, rectGroup);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pGroup);

	if (pGroup->IsKindOf (RUNTIME_CLASS(CBCGPRibbonQuickAccessToolbar)) &&
		m_ctrlRibbonPanelQAT.IsValid ())
	{
		CBCGPRibbonBar* pBar = pGroup->GetParentRibbonBar ();

		if (pBar != NULL &&
			(pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ALL) == 0 &&
			!pBar->IsQuickAccessToolbarOnTop ())
		{
			m_ctrlRibbonPanelQAT.Draw (pDC, rectGroup);
		}
	}

	return (COLORREF)-1;
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawDefaultRibbonImage (CDC* pDC, CRect rectImage,
					BOOL bIsDisabled/* = FALSE*/,
					BOOL bIsPressed/* = FALSE*/,
					BOOL bIsHighlighted/* = FALSE*/)
{
	if (!CanDrawImage () || !m_RibbonBtnDefaultImage.IsValid ())
	{
		CBCGPVisualManager2003::OnDrawDefaultRibbonImage (pDC, rectImage, 
			bIsDisabled, bIsPressed, bIsHighlighted);
		return;
	}

	m_RibbonBtnDefaultImage.DrawEx (pDC, rectImage, bIsDisabled ? 1 : 0,
		CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
}

//*******************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonMainButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonMainButton (pDC, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	BOOL bHighlighted = pButton->IsHighlighted ();
	BOOL bPressed = pButton->IsPressed ();

	if (pButton->IsDroppedDown ())
	{
		bPressed = TRUE;
		bHighlighted = TRUE;
	}

	CRect rect = pButton->GetRect ();

	int index = 0;
	if (bPressed)
	{
		if (bHighlighted)
		{
			index = 2;
		}
	}
	else if (bHighlighted)
	{
		index = 1;
	}

	CRect rectImage (m_RibbonBtnMain.GetParams ().m_rectImage);

	CBCGPToolBarImages::ImageAlignHorz horz = CBCGPToolBarImages::ImageAlignHorzStretch;
	CBCGPToolBarImages::ImageAlignVert vert = CBCGPToolBarImages::ImageAlignVertStretch;

	if (rect.Width () >= rectImage.Width () &&
		rect.Height () >= rectImage.Height ())
	{
		horz = CBCGPToolBarImages::ImageAlignHorzCenter;
		vert = CBCGPToolBarImages::ImageAlignVertCenter;
	}

	m_RibbonBtnMain.FillInterior (pDC, rect, horz, vert, index);
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonTabsFrame (
					CDC* pDC, 
					CBCGPRibbonBar* pWndRibbonBar, 
					CRect rectTab)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawRibbonTabsFrame (pDC, 
											pWndRibbonBar, rectTab);
	}
/*
	if (IsBeta1 ())
	{
		CBCGPDrawManager dm (*pDC);
		dm.Fill4ColorsGradient (rectTab, m_clrAppCaptionActiveStart, m_clrAppCaptionActiveFinish, 
			m_clrAppCaptionActiveFinish, m_clrAppCaptionActiveStart, FALSE);
	}
*/
	return m_clrRibbonCategoryText;
}
//*****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonCategory (
		CDC* pDC, 
		CBCGPRibbonCategory* pCategory,
		CRect rectCategory)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonCategory (pDC, pCategory, rectCategory);
		return;
	}

	CBCGPControlRenderer* pRenderer = &m_ctrlRibbonCategoryBack;
	CBitmapCache* pCache = &m_cacheRibbonCategoryBack;

	CBCGPBaseRibbonElement* pParentButton = pCategory->GetParentButton ();

	if (pCategory->GetTabColor () != BCGPCategoryColor_None &&
		(pParentButton == NULL || !pParentButton->IsQATMode ()))
	{
		XRibbonContextCategory& context = 
			m_ctrlRibbonContextCategory[pCategory->GetTabColor () - 1];

		pRenderer = &context.m_ctrlBack;
		pCache    = &context.m_cacheBack;
	}

	const CBCGPControlRendererParams& params = pRenderer->GetParams ();

	CBCGPRibbonPanelMenuBar* pMenuBar = pCategory->GetParentMenuBar ();
	if (pMenuBar != NULL)
	{
		if (pMenuBar->GetCategory () != NULL)
		{
			if (rectCategory.left < 0 || rectCategory.top < 0)
			{
				CBCGPDrawManager dm (*pDC);
				dm.FillGradient (rectCategory, m_clrBarGradientDark, m_clrBarGradientLight, TRUE);

				return;
			}
		}
		else if (pMenuBar->GetPanel () != NULL)
		{
			if (IsBeta1 ())
			{
				pRenderer->FillInterior (pDC, rectCategory);
				return;
			}
		}
	}

	CSize size (params.m_rectImage.Width (), 
		rectCategory.Height ());
	int nCacheIndex = pCache->FindIndex (size);
	if (nCacheIndex == -1)
	{
		nCacheIndex = pCache->CacheY (size.cy, *pRenderer);
	}

	if (nCacheIndex != -1)
	{
		pCache->Get(nCacheIndex)->DrawY (pDC, rectCategory, 
			CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right));
	}
	else
	{
		pRenderer->Draw (pDC, rectCategory);
	}
}
//*****************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonCategoryTab (
					CDC* pDC, 
					CBCGPRibbonTab* pTab,
					BOOL bIsActive)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawRibbonCategoryTab (pDC, 
											pTab, bIsActive);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pTab);

	CBCGPRibbonCategory* pCategory = pTab->GetParentCategory ();
	ASSERT_VALID (pCategory);
	CBCGPRibbonBar* pBar = pCategory->GetParentRibbonBar ();
	ASSERT_VALID (pBar);

	bIsActive = bIsActive && 
		((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown () != NULL);

	const BOOL bPressed     = pTab->IsPressed ();
	const BOOL bIsHighlight = pTab->IsHighlighted () && !pTab->IsDroppedDown ();

	CRect rectTab (pTab->GetRect ());
	rectTab.bottom++;

	int ratio = 0;
	if (!IsBeta1 () && m_ctrlRibbonCategoryTabSep.IsValid ())
	{
		ratio = pBar->GetTabTrancateRatio ();
	}

	if (ratio > 0)
	{
		rectTab.left++;
	}

	int nImage = bIsActive ? 3 : 0;

	if (bPressed)
	{
		if (bIsHighlight)
		{
			nImage = bIsActive ? 2 : 1;
		}
	}
	
	if(bIsHighlight)
	{
		nImage += 1;
	}

	CBCGPControlRenderer* pRenderer = &m_ctrlRibbonCategoryTab;
	COLORREF clrText = m_clrRibbonCategoryText;
	COLORREF clrTextHighlighted = m_clrRibbonCategoryTextHighlighted;

	if (pCategory->GetTabColor () != BCGPCategoryColor_None)
	{
		XRibbonContextCategory& context = 
			m_ctrlRibbonContextCategory[pCategory->GetTabColor () - 1];

		pRenderer = &context.m_ctrlTab;
		clrText  = context.m_clrText;
		clrTextHighlighted = context.m_clrTextHighlighted;
	}

	pRenderer->Draw (pDC, rectTab, nImage);

	if (ratio > 0)
	{
		CRect rectSep (rectTab);
		rectSep.left = rectSep.right;
		rectSep.right++;
		rectSep.bottom--;

		m_ctrlRibbonCategoryTabSep.Draw (pDC, rectSep, 0, (BYTE)min(ratio * 255 / 100, 255));
	}

	return bIsActive
			? clrTextHighlighted
			: clrText;
}
//****************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonPanel (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel, 
		CRect rectPanel,
		CRect rectCaption)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawRibbonPanel (pDC, 
											pPanel, rectPanel, rectCaption);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pPanel);

	COLORREF clrText = m_clrRibbonPanelText;

	if (pPanel->IsKindOf (RUNTIME_CLASS(CBCGPRibbonMainPanel)))
	{
		const int nBorderSize = GetPopupMenuBorderSize ();
		rectPanel.InflateRect (nBorderSize, nBorderSize);
		
		m_ctrlRibbonMainPanel.Draw (pDC, rectPanel);
	}
	else
	{
		BOOL bHighlighted = pPanel->IsHighlighted ();

		if (bHighlighted)
		{
			clrText = m_clrRibbonPanelTextHighlighted;
		}

		CBCGPControlRenderer* pRendererB = &m_ctrlRibbonPanelBack_B;
		CBCGPControlRenderer* pRendererT = &m_ctrlRibbonPanelBack_T;
		CBitmapCache* pCacheB = &m_cacheRibbonPanelBack_B;
		CBitmapCache* pCacheT = &m_cacheRibbonPanelBack_T;

		CBCGPRibbonCategory* pCategory = pPanel->GetParentCategory ();
		ASSERT_VALID (pCategory);

		CBCGPBaseRibbonElement* pParentButton = pPanel->GetParentButton ();

		if (pCategory->GetTabColor () != BCGPCategoryColor_None &&
			(pParentButton == NULL || !pParentButton->IsQATMode ()))
		{
			pRendererB = &m_ctrlRibbonContextPanelBack_B;
			pRendererT = &m_ctrlRibbonContextPanelBack_T;
			pCacheB = &m_cacheRibbonContextPanelBack_B;
			pCacheT = &m_cacheRibbonContextPanelBack_T;

			clrText = bHighlighted 
						? m_clrRibbonContextPanelTextHighlighted
						: m_clrRibbonContextPanelText;
		}

		if (!pPanel->IsCollapsed ())
		{
			CRect rect (rectPanel);

			BOOL bDrawCaption = rectCaption.Height () > 0 && pRendererT->IsValid ();

			if (bDrawCaption)
			{
				BOOL bBottomEnabled = pRendererB->IsValid ();

				if (bBottomEnabled)
				{
					rect.bottom -= rectCaption.Height () == 0
							? pRendererB->GetParams ().m_rectImage.Height ()
							: rectCaption.Height ();
				}

				{
					const CBCGPControlRendererParams& params = pRendererT->GetParams ();

					CSize size (params.m_rectImage.Width (), 
						rect.Height ());
					int nCacheIndex = pCacheT->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheT->CacheY (size.cy, *pRendererT);
					}

					if (nCacheIndex != -1)
					{
						pCacheT->Get(nCacheIndex)->DrawY (pDC, rect, 
							CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right),
							bHighlighted ? 1 : 0);
					}
					else
					{
						pRendererT->Draw (pDC, rect, bHighlighted ? 1 : 0);
					}
				}	

				if (bBottomEnabled)
				{
					rect.top = rect.bottom;
					rect.bottom = rectPanel.bottom;

					const CBCGPControlRendererParams& params = pRendererB->GetParams ();

					CSize size (params.m_rectImage.Width (), 
						rect.Height ());
					int nCacheIndex = pCacheB->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheB->CacheY (size.cy, *pRendererB);
					}

					if (nCacheIndex != -1)
					{
						pCacheB->Get(nCacheIndex)->DrawY (pDC, rect, 
							CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right),
							bHighlighted ? 1 : 0);
					}
					else
					{
						pRendererB->Draw (pDC, rect, bHighlighted ? 1 : 0);
					}
				}
			}
		}
	}

	return clrText;
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonPanelCaption (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel, 
		CRect rectCaption)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonPanelCaption (pDC, 
											pPanel, rectCaption);
		return;
	}

	if (pPanel->IsKindOf (RUNTIME_CLASS(CBCGPRibbonMainPanel)))
	{
		return;
	}

	CString str = pPanel->GetName ();

	if (!str.IsEmpty ())
	{
		if (pPanel->GetLaunchButton ().GetID () > 0)
		{
			rectCaption.right = pPanel->GetLaunchButton ().GetRect ().left;
		}

		COLORREF clrTextOld = pDC->SetTextColor (pPanel->IsHighlighted () ? 
			m_clrRibbonPanelCaptionTextHighlighted : m_clrRibbonPanelCaptionText);

		rectCaption.DeflateRect (5, 1);
		rectCaption.OffsetRect (0, -1);

		pDC->DrawText (	str, rectCaption, 
						DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

		pDC->SetTextColor (clrTextOld);
	}
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonLaunchButton (
					CDC* pDC,
					CBCGPRibbonLaunchButton* pButton,
					CBCGPRibbonPanel* pPanel)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonLaunchButton (pDC, 
											pButton, pPanel);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pPanel);

	CRect rect (pButton->GetRect ());

	if (!IsBeta1 ())
	{
		rect.right--;
		rect.bottom--;
	}

	BOOL bHighlighted = pButton->IsHighlighted ();

	int index = 0;

	if (!IsBeta1 ())
	{
		if (m_ctrlRibbonBtnLaunch.IsValid ())
		{
			int index = 0;
			if (pButton->IsDisabled ())
			{
				index = 3;
			}
			else if (pButton->IsPressed ())
			{
				if (bHighlighted)
				{
					index = 2;
				}
			}
			else if (bHighlighted)
			{
				index = 1;
			}

			m_ctrlRibbonBtnLaunch.Draw (pDC, rect, index);
		}
	}
	else
	{
		if (pButton->IsPressed ())
		{
			if (bHighlighted)
			{
				index = 2;
			}
		}
		else if (bHighlighted)
		{
			index = 1;
		}
	}

	if (m_RibbonBtnLaunchIcon.IsValid ())
	{
		m_RibbonBtnLaunchIcon.DrawEx (pDC, rect, index, 
			CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
	}
}
//****************************************************************************
COLORREF CBCGPVisualManager2007::OnFillRibbonButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillRibbonButton (pDC, pButton);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	const BOOL bIsMenuMode = pButton->IsMenuMode ();

	CRect rect (pButton->GetRect ());

	CBCGPControlRenderer* pRenderer = NULL;
	CBitmapCache* pCache = NULL;
	int index = 0;

	BOOL bDisabled    = pButton->IsDisabled ();
	BOOL bFocused     = pButton->IsFocused ();
	BOOL bPressed     = pButton->IsPressed () && !bIsMenuMode;
	BOOL bChecked     = pButton->IsChecked ();
	BOOL bHighlighted = pButton->IsHighlighted ();

	BOOL bDefaultPanelButton = pButton->IsDefaultPanelButton () && !pButton->IsQATMode ();

	if (pButton->IsDroppedDown () && !bIsMenuMode)
	{
		bChecked     = TRUE;
		bPressed     = FALSE;
		bHighlighted = FALSE;
	}

	CBCGPBaseRibbonElement::RibbonElementLocation location = 
		pButton->GetLocationInGroup ();

	if (pButton->IsKindOf (RUNTIME_CLASS(CBCGPRibbonEdit)))
	{
		COLORREF color1 = m_clrRibbonEdit;
		if (bDisabled)
		{
			color1 = m_clrRibbonEditDisabled;
		}
		else if (bChecked || bHighlighted)
		{
			color1 = m_clrRibbonEditHighlighted;
		}

		COLORREF color2 = color1;

		{
			CBCGPDrawManager dm (*pDC);
			dm.FillGradient (rect, color1, color2, TRUE);
		}

		return (COLORREF)-1;
	}

	if (bChecked && bIsMenuMode)
	{
		bChecked = FALSE;
	}

	if (location != CBCGPBaseRibbonElement::RibbonElementNotInGroup &&
		pButton->IsShowGroupBorder ())
	{
		if (!pButton->GetMenuRect().IsRectEmpty ())
		{
			CRect rectC = pButton->GetCommandRect();
			CRect rectM = pButton->GetMenuRect();

			CBCGPControlRenderer* pRendererC = NULL;
			CBCGPControlRenderer* pRendererM = NULL;

			CBitmapCache* pCacheC = NULL;
			CBitmapCache* pCacheM = NULL;

			if (location == CBCGPBaseRibbonElement::RibbonElementSingleInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_F[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_L[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_F[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_L[1];
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementFirstInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_F[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_F[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_F[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_F[1];
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementLastInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_L[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_L[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_L[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_L[1];
			}
			else
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_M[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_M[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_M[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_M[1];
			}

			int indexC = 0;
			int indexM = 0;

			BOOL bHighlightedC = pButton->IsCommandAreaHighlighted ();
			BOOL bHighlightedM = pButton->IsMenuAreaHighlighted ();

			if (IsBeta1 ())
			{
				if (bChecked)
				{
					indexC = 3;
				}

				if (bDisabled)
				{
					indexC = 0;
				}
				else
				{
					if (pButton->IsDroppedDown () && !bIsMenuMode)
					{
						indexC = pButton->IsChecked () ? 3 : 0;
						indexM = 3;
					}
					else
					{
						if (bPressed)
						{
							if (bHighlightedC)
							{
								indexM = 1;
								indexC = 2;
							}
						}
						else if (bHighlighted)
						{
							indexC++;
							indexM = 1;
						}
					}
				}
			}
			else
			{
				if (bChecked)
				{
					indexC = 3;
					indexM = 5;
				}

				if (bDisabled)
				{
					if (bChecked)
					{
						indexC = 5;
						indexM = 4;
					}
				}
				else
				{
					if (pButton->IsDroppedDown () && !bIsMenuMode)
					{
						indexC = pButton->IsChecked () ? 3 : 6;
						indexM = 3;
					}
					else
					{
						if (bHighlightedC || bHighlightedM)
						{
							if (bChecked)
							{
								indexC = bHighlightedC ? 4 : 3;
							}
							else
							{
								indexC = bHighlightedC ? 1 : 6;
							}

							indexM = bHighlightedM ? 1 : 5;
						}

						if (bPressed)
						{
							if (bHighlightedC)
							{
								indexC = 2;
							}
						}
					}
				}
			}

			if (indexC != -1 && indexM != -1)
			{
				if (pCacheC != NULL && pCacheM != NULL)
				{
					CSize size (rectC.Size ());
					int nCacheIndex = pCacheC->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheC->Cache (size, *pRendererC);
					}

					pCacheC->Get (nCacheIndex)->Draw (pDC, rectC, indexC);

					size = rectM.Size ();
					nCacheIndex = pCacheM->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheM->Cache (size, *pRendererM);
					}

					pCacheM->Get (nCacheIndex)->Draw (pDC, rectM, indexM);
				}
				else
				{
					pRendererC->Draw (pDC, rectC, indexC);
					pRendererM->Draw (pDC, rectM, indexM);
				}
			}

			return (COLORREF)-1;
		}
		else
		{
			if (location == CBCGPBaseRibbonElement::RibbonElementSingleInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_S;
				pCache    = &m_cacheRibbonBtnGroup_S;
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementFirstInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_F;
				pCache    = &m_cacheRibbonBtnGroup_F;
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementLastInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_L;
				pCache    = &m_cacheRibbonBtnGroup_L;
			}
			else
			{
				pRenderer = &m_ctrlRibbonBtnGroup_M;
				pCache    = &m_cacheRibbonBtnGroup_M;
			}

			if (bChecked)
			{
				index = 3;
			}

			if (bDisabled)
			{
				index = 0;
			}
			else
			{
				if (bPressed)
				{
					if (bHighlighted)
					{
						index = 2;
					}
				}
				else if (bHighlighted)
				{
					index++;
				}
			}
		}
	}
	else if (bDefaultPanelButton)
	{
		if (bPressed)
		{
			if (bHighlighted)
			{
				index = 2;
			}
		}
		else if (bHighlighted)
		{
			index = 1;
		}
		else if (bChecked)
		{
			index = 2;
		}

		if (index != -1)
		{
			pRenderer = &m_ctrlRibbonBtnDefault;
			CBitmapCache* pCache = &m_cacheRibbonBtnDefault;

			CBCGPRibbonCategory* pCategory = pButton->GetParentCategory ();
			ASSERT_VALID (pCategory);

			if (pCategory->GetTabColor () != BCGPCategoryColor_None)
			{
				XRibbonContextCategory& context = 
					m_ctrlRibbonContextCategory[pCategory->GetTabColor () - 1];

				pRenderer = &context.m_ctrlBtnDefault;
				pCache    = &context.m_cacheBtnDefault;
			}

			const CBCGPControlRendererParams& params = pRenderer->GetParams ();

			CSize size (params.m_rectImage.Width (), 
				rect.Height ());
			int nCacheIndex = pCache->FindIndex (size);
			if (nCacheIndex == -1)
			{
				nCacheIndex = pCache->CacheY (size.cy, *pRenderer);
			}

			if (nCacheIndex != -1)
			{
				pCache->Get(nCacheIndex)->DrawY (pDC, rect, 
					CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right),
					index);

				return m_clrToolBarBtnTextHighlighted;
			}
		}
	}
	else if ((!bDisabled && (bPressed || bChecked || bHighlighted)) || 
		    (bDisabled && bFocused))
	{
		if (!pButton->GetMenuRect().IsRectEmpty ()/* &&
			(pButton->IsHighlighted () || bChecked)*/)
		{
			CRect rectC = pButton->GetCommandRect();
			CRect rectM = pButton->GetMenuRect();

			CBCGPControlRenderer* pRendererC = pButton->IsMenuOnBottom () 
				? &m_ctrlRibbonBtnMenuV[0]
				: &m_ctrlRibbonBtnMenuH[0];
			CBCGPControlRenderer* pRendererM = pButton->IsMenuOnBottom () 
				? &m_ctrlRibbonBtnMenuV[1]
				: &m_ctrlRibbonBtnMenuH[1];

			int indexC = -1;
			int indexM = -1;

			BOOL bHighlightedC = pButton->IsCommandAreaHighlighted ();
			BOOL bHighlightedM = pButton->IsMenuAreaHighlighted ();

			if (IsBeta1 ())
			{
				if (bChecked)
				{
					indexC = 2;
					indexM = 2;
				}

				if (bDisabled)
				{
					if (bChecked)
					{
						// TODO
					}
				}
				else
				{
					if (pButton->IsDroppedDown () && !bIsMenuMode)
					{
						indexC = pButton->IsChecked () ? 2 : 4;
						indexM = 2;
					}
					else
					{
						if (bPressed)
						{
							if (bHighlighted)
							{
								if (bHighlightedC)
								{
									indexC = 1;
								}
								else
								{
									indexC = pButton->IsChecked () ? indexC : 0;
								}

								indexM = pButton->IsChecked () ? indexM : 0;
							}
						}
						else if (bHighlighted)
						{
							indexC++;
							indexM++;
						}
					}
				}
			}
			else
			{
				if (bChecked)
				{
					indexC = 2;
					indexM = 4;
				}

				if (bDisabled)
				{
					if (bChecked)
					{
						indexC = 4;
						indexM = 3;
					}
					else if (bFocused)
					{
						indexC = 3;
						indexM = 3;
					}
				}
				else
				{
					if (pButton->IsDroppedDown () && !bIsMenuMode)
					{
						indexC = pButton->IsChecked () ? 2 : 5;
						indexM = 2;
					}
					else
					{
						if (bHighlightedC || bHighlightedM)
						{
							if (bChecked)
							{
								indexC = bHighlightedC ? 3 : 2;
							}
							else
							{
								indexC = bHighlightedC ? 0 : 5;
							}

							indexM = bHighlightedM ? 0 : 4;
						}

						if (bPressed)
						{
							if (bHighlightedC)
							{
								indexC = 1;
							}
						}
					}
				}
			}

			if (indexC != -1)// && indexM != -1)
			{
				pRendererC->Draw (pDC, rectC, indexC);
				pRendererM->Draw (pDC, rectM, indexM);
			}

			return (COLORREF)-1;
		}
		else
		{
			index = -1;

			pRenderer = &m_ctrlRibbonBtn[0];
			if (rect.Height () > pRenderer->GetParams ().m_rectImage.Height () * 1.5 &&
				m_ctrlRibbonBtn[1].IsValid ())
			{
				pRenderer = &m_ctrlRibbonBtn[1];
			}

			if (bDisabled && bFocused)
			{
				if (pRenderer->GetImageCount () > 4)
				{
					index = 4;
				}
				else
				{
					index = 0;
				}
			}
			
			if (!bDisabled)
			{
				if (bChecked)
				{
					index = 2;
				}

				if (bPressed)
				{
					if (bHighlighted)
					{
						index = 1;
					}
				}
				else if (bHighlighted)
				{
					index++;
				}
			}
		}
	}

	COLORREF clrText = bDisabled
		? m_clrToolBarBtnTextDisabled
		: COLORREF (-1);

	if (pRenderer != NULL)
	{
		if (index != -1)
		{
			if (pCache != NULL)
			{
				CSize size (rect.Size ());
				int nCacheIndex = pCache->FindIndex (size);
				if (nCacheIndex == -1)
				{
					nCacheIndex = pCache->Cache (size, *pRenderer);
				}

				pCache->Get (nCacheIndex)->Draw (pDC, rect, index);
			}
			else
			{
				pRenderer->Draw (pDC, rect, index);
			}

			if (!bDisabled)
			{
				clrText = m_clrToolBarBtnTextHighlighted;
			}
		}
	}

	return clrText;
//	return CBCGPVisualManager2003::OnFillRibbonButton (pDC, pButton);
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonButtonBorder (pDC, pButton);
	}

	if (pButton->IsKindOf (RUNTIME_CLASS(CBCGPRibbonEdit)))
	{
		CRect rect (pButton->GetRect ());

		COLORREF colorBorder = m_clrRibbonEditBorder;

		if (pButton->IsDisabled ())
		{
			colorBorder = m_clrRibbonEditBorderDisabled;
		}
		else if (pButton->IsHighlighted () || pButton->IsDroppedDown ())
		{
			colorBorder = pButton->IsDroppedDown ()
				? m_clrRibbonEditBorderPressed
				: m_clrRibbonEditBorderHighlighted;
		}

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, colorBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, colorBorder, colorBorder);
		}
	}
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonDefaultPaneButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonDefaultPaneButton (pDC, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	OnFillRibbonButton (pDC, pButton);

	BOOL bIsQATMode = pButton->IsQATMode ();

	CRect rectFrame (pButton->GetRect ());

	if (!bIsQATMode)
	{
		if (m_ctrlRibbonBtnDefaultIcon.IsValid ())
		{
			rectFrame.bottom = pButton->GetRect ().CenterPoint ().y;
			CPoint ptCenter (rectFrame.CenterPoint ());

			CRect rectImage  (m_ctrlRibbonBtnDefaultIcon.GetParams ().m_rectImage);
			CRect rectInter  (m_ctrlRibbonBtnDefaultIcon.GetParams ().m_rectInter);
			CPoint point (rectInter.CenterPoint ());

			rectFrame = CRect (rectImage);
			rectFrame.OffsetRect (-rectFrame.TopLeft ());
			rectFrame.OffsetRect (ptCenter - rectFrame.CenterPoint ());
			rectFrame.OffsetRect (rectImage.CenterPoint () - rectInter.CenterPoint ());

			m_ctrlRibbonBtnDefaultIcon.Draw (pDC, rectFrame);
		}
	}
	else
	{
		if (m_ctrlRibbonBtnDefaultQAT.IsValid ())
		{
			int index = 0;
			if (pButton->IsDroppedDown ())
			{
				index = 2;
			}
			else if (pButton->IsPressed ())
			{
				if (pButton->IsHighlighted ())
				{
					index = 2;
				}
			}
			else if (pButton->IsHighlighted ())
			{
				index = 1;
			}

			m_ctrlRibbonBtnDefaultQAT.Draw (pDC, rectFrame, index);
		}
		else if (m_ctrlRibbonBtnDefaultQATIcon.IsValid ())
		{
			int nHeight = m_ctrlRibbonBtnDefaultQATIcon.GetParams ().m_rectImage.Height ();

			if (rectFrame.Height () < nHeight)
			{
				nHeight = rectFrame.Height () / 2;
			}

			rectFrame.DeflateRect (1, 0);
			rectFrame.top = rectFrame.bottom - nHeight;

			m_ctrlRibbonBtnDefaultQATIcon.Draw (pDC, rectFrame);
		}
	}

	CRect rectImage (pButton->GetRect ());
	CRect rectMenuArrow (rectImage);

	if (bIsQATMode)
	{
		if (!m_ctrlRibbonBtnDefaultQAT.IsValid ())
		{
			rectImage.bottom = rectImage.top + globalData.m_sizeSmallIcon.cy;
		}

		pButton->DrawImage (pDC, CBCGPRibbonButton::RibbonImageSmall, rectImage);

		OnDrawRibbonButtonBorder (pDC, pButton);

		rectMenuArrow.top = rectMenuArrow.bottom - 4;
	}
	else
	{
		rectImage.bottom = rectImage.CenterPoint ().y;

		pButton->DrawImage (pDC, CBCGPRibbonButton::RibbonImageSmall, rectImage);

		// Draw text:
		CString strTect = pButton->GetText ();

		CRect rectText = pButton->GetRect ();
		rectText.top = rectImage.bottom;

		int nTextHeight = pDC->DrawText (strTect, rectText, 
			DT_WORDBREAK | DT_CENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

		if (pButton->IsSingleLineText ())
		{
			//----------------------------------------
			// Single line text, draw arrow on bottom:
			//----------------------------------------
			rectMenuArrow.top = rectText.top + nTextHeight;
		}
		else
		{
			//-------------------------------------
			// Multi-line text, draw arrow on right:
			//-------------------------------------
			rectMenuArrow.top = rectText.top + pButton->GetTextRowHeight ();
			rectMenuArrow.bottom = rectMenuArrow.top + CMenuImages::Size ().cy;
			rectMenuArrow.left = rectMenuArrow.right - CMenuImages::Size ().cx - 1;
		}
	}

	if (bIsQATMode && m_ctrlRibbonBtnDefaultQAT.IsValid ())
	{
		return;
	}

	COLORREF clrTextOld = pDC->SetTextColor (pButton->IsHighlighted () ?
			m_clrRibbonPanelTextHighlighted : m_clrRibbonPanelText);

	CRect rectWhite = rectMenuArrow;
	rectWhite.OffsetRect (0, 1);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectWhite,
		CMenuImages::ImageWhite);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectMenuArrow,
		CMenuImages::ImageBlack);

	pDC->SetTextColor (clrTextOld);
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonDefaultPaneButtonIndicator (
					CDC* pDC, 
					CBCGPRibbonButton* pButton,
					CRect rect, 
					BOOL bIsSelected, 
					BOOL bHighlighted)
{
	if (!CanDrawImage () || !m_ctrlRibbonBtnDefaultIcon.IsValid ())
	{
		CBCGPVisualManager2003::OnDrawRibbonDefaultPaneButtonIndicator (
					pDC, pButton, rect, bIsSelected, bHighlighted);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	rect.left = rect.right - m_ctrlRibbonBtnDefaultIcon.GetParams ().m_rectImage.Width ();
	m_ctrlRibbonBtnDefaultIcon.Draw (pDC, rect);

	CRect rectWhite = rect;
	rectWhite.OffsetRect (0, 1);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectWhite,
		CMenuImages::ImageWhite);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rect,
		CMenuImages::ImageBlack);
}
//****************************************************************************
void CBCGPVisualManager2007::OnFillRibbonEdit (
					CDC* pDC, 
					CBCGPRibbonEditCtrl* pEdit,
					CRect rect,
					BOOL bIsHighlighted,
					BOOL bIsPaneHighlighted,
					BOOL bIsDisabled,
					COLORREF& clrText, COLORREF& clrSelBackground, COLORREF& clrSelText)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillRibbonEdit (pDC, 
			pEdit, rect, bIsHighlighted, bIsPaneHighlighted, bIsDisabled,
			clrText, clrSelBackground, clrSelText);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF color1 = m_clrRibbonEdit;

	if (bIsDisabled)
	{
		color1 = m_clrRibbonEditDisabled;
	}
	else
	{
		if (bIsHighlighted)
		{
			color1 = m_clrRibbonEditHighlighted;
		}
	}

	COLORREF color2 = color1;

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, color1, color2, TRUE);

	if (bIsDisabled)
	{
		clrText = globalData.clrGrayedText;
	}
	else
	{
		clrText = m_clrMenuText;	// TODO
		clrSelText = m_clrMenuText;	// TODO
		clrSelBackground = m_clrRibbonEditSelection;
	}
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonMainPanelFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel,
					CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonMainPanelFrame (pDC, 
					pPanel, rect);
		return;
	}

	if (!IsBeta1 ())
	{
		ASSERT_VALID (pDC);

		rect.right += 2; //TODO
		m_ctrlRibbonMainPanelBorder.DrawFrame (pDC, rect);
	}
}
//***********************************************************************************
void CBCGPVisualManager2007::OnFillRibbonMenuFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel,
					CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillRibbonMenuFrame (pDC, 
					pPanel, rect);
		return;
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel,
					CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonRecentFilesFrame (pDC, 
					pPanel, rect);
		return;
	}

	ASSERT_VALID (pDC);

	rect.right += 2; //TODO
	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.right = rectSeparator.left + 2;

	DrawSeparator (pDC, rectSeparator, FALSE);
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonLabel (
					CDC* pDC, 
					CBCGPRibbonLabel* pLabel,
					CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pLabel);

	if (pLabel->IsMenuMode ())
	{
		OnDrawMenuLabel (pDC, rect);
	}
}

//****************************************************************************
COLORREF CBCGPVisualManager2007::OnFillRibbonMainPanelButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillRibbonMainPanelButton (pDC, pButton);
	}

	BOOL bHighlighted = pButton->IsHighlighted ();

	COLORREF clrText = bHighlighted
						? m_clrMenuTextHighlighted
						: pButton->IsDisabled () 
							? m_clrMenuTextDisabled
							: m_clrMenuText;

	int index = 0;

	if (pButton->IsPressed ())
	{
		if (bHighlighted)
		{
			index = 2;
		}
	}
	else if (bHighlighted)
	{
		index = 1;
	}

	m_ctrlRibbonBtnMainPanel.Draw (pDC, pButton->GetRect (), index);

	return clrText;
}
//****************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonMainPanelButtonBorder (
		CDC* pDC, CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonMainPanelButtonBorder (pDC, pButton);
		return;
	}
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonPaletteButton (
					CDC* pDC, 
					CBCGPRibbonPaletteIcon* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonPaletteButton (pDC, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	int index = 0;
	if (pButton->IsDisabled ())
	{
		index = 3;
	}
	else
	{
		if (pButton->IsPressed ())
		{
			if (pButton->IsHighlighted ())
			{
				index = 2;
			}
		}
		else if (pButton->IsHighlighted ())
		{
			index = 1;
		}
	}

	int nBtn = 1;
	if (pButton->IsLast ())
	{
		nBtn = 2;
	}
	else if (pButton->IsFirst ())
	{
		nBtn = 0;
	}

	m_ctrlRibbonBtnPalette[nBtn].Draw (pDC, pButton->GetRect (), index);
}
//***********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonPaletteBorder (
				CDC* pDC, 
				CBCGPRibbonPaletteButton* pButton, 
				CRect rectBorder)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonPaletteBorder (pDC, pButton, rectBorder);
		return;
	}

	rectBorder.right -= 5;

	ASSERT_VALID (pDC);
	pDC->Draw3dRect (rectBorder, globalData.clrBarShadow, globalData.clrBarShadow);
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonCategoryCaption (
				CDC* pDC, 
				CBCGPRibbonContextCaption* pContextCaption)
{
	if (!CanDrawImage () || pContextCaption->GetColor () == BCGPCategoryColor_None)
	{
		return CBCGPVisualManager2003::OnDrawRibbonCategoryCaption (pDC, pContextCaption);
	}

	XRibbonContextCategory& context = 
		m_ctrlRibbonContextCategory[pContextCaption->GetColor () - 1];

	CRect rect (pContextCaption->GetRect ());
	context.m_ctrlCaption.Draw (pDC, rect);

	CRect rectTab (pContextCaption->GetParentRibbonBar ()->GetActiveCategory ()->GetTabRect ());
	rect.top = rectTab.top;
	rect.bottom = rectTab.bottom;
	m_ctrlRibbonContextSeparator.DrawFrame (pDC, rect);

	return context.m_clrCaptionText;
}
//********************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawRibbonStatusBarPane (CDC* pDC, CBCGPRibbonStatusBar* pBar,
					CBCGPRibbonStatusBarPane* pPane)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnDrawRibbonStatusBarPane (pDC, pBar, pPane);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);
	ASSERT_VALID (pPane);

	CRect rectPane = pPane->GetRect ();

	const BOOL bHighlighted = pPane->IsHighlighted ();
	const BOOL bChecked     = pPane->IsChecked ();
	const BOOL bExtended	= pPane->IsExtended ();

	if (bHighlighted || bChecked)
	{
		CRect rectButton = rectPane;
		rectButton.DeflateRect (1, 1);

		int index = 0;
		if (pPane->IsPressed ())
		{
			if (bHighlighted)
			{
				index = 1;
			}
		}
		else if (bChecked)
		{
			if (bHighlighted)
			{
				index = 0;
			}
			else
			{
				index = 1;
			}
		}

		m_ctrlRibbonBtnStatusPane.Draw (pDC, rectButton, index);
	}

	if (pPane->IsDisabled ())
	{
		return bExtended ? m_clrExtenedStatusBarTextDisabled : m_clrStatusBarTextDisabled;
	}

	return bHighlighted ? 
		m_clrToolBarBtnTextHighlighted :
		m_clrStatusBarText;
}
//********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonSliderZoomButton (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect, BOOL bIsZoomOut, 
			BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonSliderZoomButton (
			pDC, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPControlRenderer* pRenderer = bIsZoomOut 
		? &m_ctrlRibbonSliderBtnMinus
		: &m_ctrlRibbonSliderBtnPlus;

	int index = 0;
	if (bIsPressed)
	{
		if (bIsHighlighted)
		{
			index = 2;
		}
	}
	else if (bIsHighlighted)
	{
		index = 1;
	}

	pRenderer->FillInterior (pDC, rect, 
		CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter,
		index);
}
//********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonSliderChannel (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonSliderChannel (
			pDC, pSlider, rect);
		return;
	}

	ASSERT_VALID (pDC);

	DrawSeparator (pDC, rect, m_penSeparatorDark, m_penSeparator2, TRUE);

	rect.left += rect.Width () / 2 - 1;
	rect.right = rect.left + 2;
	rect.top -= 2;
	rect.bottom += 2;
	DrawSeparator (pDC, rect, m_penSeparatorDark, m_penSeparator2, FALSE);
}
//********************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonSliderThumb (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonSliderThumb (
			pDC, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
		return;
	}

	ASSERT_VALID (pDC);

	int index = 0;
	if (bIsPressed)
	{
		index = 2;
	}
	else if (bIsHighlighted)
	{
		index = 1;
	}

	m_ctrlRibbonSliderThumb.FillInterior (pDC, rect, 
		CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter,
		index);
}
//**************************************************************************************
void CBCGPVisualManager2007::OnDrawRibbonProgressBar (CDC* pDC, 
												  CBCGPRibbonProgressBar* pProgress, 
												  CRect rectProgress, CRect rectChunk,
												  BOOL bInfiniteMode)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawRibbonProgressBar (pDC, pProgress, 
												  rectProgress, rectChunk, bInfiniteMode);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pProgress);

	m_ctrlRibbonProgressBack.Draw (pDC, rectProgress);

	CRect rectInternal (rectProgress);

	CRect rectBorders (m_ctrlRibbonProgressBack.GetParams ().m_rectCorners);

	rectInternal.DeflateRect (rectBorders.left, rectBorders.top, rectBorders.right, rectBorders.bottom);

	if (!bInfiniteMode)
	{
		// normal
		rectChunk.IntersectRect (rectChunk, rectInternal);

		if (!rectChunk.IsRectEmpty () || pProgress->GetPos () != pProgress->GetRangeMin ())
		{
			CRgn rgn;
			rgn.CreateRectRgnIndirect (rectInternal);
			pDC->SelectClipRgn (&rgn);

			if (!rectChunk.IsRectEmpty ())
			{
				rectChunk.left = rectChunk.right - rectInternal.Width ();
				m_ctrlRibbonProgressNormal.Draw (pDC, rectChunk);
			}
			else
			{
				rectChunk = rectInternal;
				rectChunk.right  = rectInternal.left;
			}

			if (rectChunk.right != rectInternal.right)
			{
				rectChunk.left = rectChunk.right;
				rectChunk.right += m_ctrlRibbonProgressNormalExt.GetParams ().m_rectImage.Width ();

				m_ctrlRibbonProgressNormalExt.Draw (pDC, rectChunk);
			}

			pDC->SelectClipRgn (NULL);
		}
	}
	else if (pProgress->GetPos () != pProgress->GetRangeMin ())
	{
		int index = (pProgress->GetPos () - pProgress->GetRangeMin ()) % 
			m_ctrlRibbonProgressInfinity.GetImageCount ();

		m_ctrlRibbonProgressInfinity.Draw (pDC, rectInternal, index);
	}
}
//*******************************************************************************
void CBCGPVisualManager2007::OnFillRibbonQATPopup (
				CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillRibbonQATPopup (pDC, pMenuBar, rect);
		return;
	}

	ASSERT_VALID (pDC);

	if (m_ctrlRibbonBorder_QAT.IsValid ())
	{
		m_ctrlRibbonBorder_QAT.FillInterior (pDC, rect);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, m_clrBarGradientDark, m_clrBarGradientLight, TRUE);
	}
}
//*******************************************************************************
int CBCGPVisualManager2007::GetRibbonPopupBorderSize (const CBCGPRibbonPanelMenu* pPopup) const
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetRibbonPopupBorderSize (pPopup);
	}

	//DR:
	if (pPopup != NULL/* && !IsBeta1 ()*/)
	{
		ASSERT_VALID (pPopup);

		CBCGPRibbonPanelMenuBar* pRibbonMenuBar = 
			DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenuBar, 
				(const_cast<CBCGPRibbonPanelMenu*>(pPopup))->GetMenuBar ());

		if (pRibbonMenuBar != NULL)
		{
			if (pRibbonMenuBar->IsMainPanel ())
			{
				return (int)GetPopupMenuBorderSize ();
			}

			if (!pRibbonMenuBar->IsMenuMode ())
			{
				if (pRibbonMenuBar->IsQATPopup ())
				{
					if (m_ctrlRibbonBorder_QAT.IsValid ())
					{
						return m_ctrlRibbonBorder_QAT.GetParams ().m_rectSides.left;
					}
				}
				else if (pRibbonMenuBar->IsCategoryPopup ())
				{
					return 0;
				}
				else if (pRibbonMenuBar->IsFloaty ())
				{
					if (m_ctrlRibbonBorder_Floaty.IsValid ())
					{
						return m_ctrlRibbonBorder_Floaty.GetParams ().m_rectSides.left;
					}
				}
				else
				{
					if (pRibbonMenuBar->GetPanel () != NULL)
					{
						if (!IsBeta1 ())
						{
							return 0;
						}
					}

					// standard size
				}
			}
		}
	}

	return (int)GetPopupMenuBorderSize ();
}
#endif // BCGP_EXCLUDE_RIBBON

#ifndef BCGP_EXCLUDE_PLANNER
//*******************************************************************************
void CBCGPVisualManager2007::OnFillPlanner (CDC* pDC, CBCGPPlannerView* pView, 
		CRect rect, BOOL bWorkingArea)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pView);

	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillPlanner (pDC, pView, rect, bWorkingArea);
		return;
	}

	if (m_bPlannerBackItemSelected)
	{
		CBrush br (m_bPlannerBlack ? RGB (232, 234, 236) : RGB (230, 237, 247));
		pDC->FillRect (rect, &br);
	}
	else
	{
		CBCGPVisualManager2003::OnFillPlanner (pDC, pView, rect, bWorkingArea);
	}

	if (m_bPlannerBackItemToday && DYNAMIC_DOWNCAST (CBCGPPlannerViewDay, pView) == NULL)
	{
		rect.right--;
		pDC->Draw3dRect (rect, m_clrPlannerTodayBorder, m_clrPlannerTodayBorder);

		rect.left--;
		rect.right++;
		rect.bottom++;
		pDC->Draw3dRect (rect, m_clrPlannerTodayBorder, m_clrPlannerTodayBorder);
	}
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::OnFillPlannerCaption (CDC* pDC,
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected,
		BOOL bNoBorder/* = FALSE*/)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillPlannerCaption (pDC,
			pView, rect, bIsToday, bIsSelected, bNoBorder);
	}

	const BOOL bMonth = DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL;

	if (bMonth && m_bPlannerCaptionBackItemHeader)
	{
		return m_clrPlannerNcText;
	}

	ASSERT_VALID (pDC);

	const BOOL bDay = pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewDay));

	if (bDay)
	{
		if (!bIsToday)
		{
			rect.DeflateRect (1, 1);
		}
	}
	else if(bMonth)
	{
		if (!bIsToday)
		{
			rect.bottom--;
		}
	}

	COLORREF clrText   = RGB (0, 0, 0);
	COLORREF clrBorder = CLR_DEFAULT;

	if (bIsToday)
	{
		CRect rectHalf (rect);
		rectHalf.bottom = rectHalf.top + rectHalf.Height () / 2;

		CBCGPDrawManager dm (*pDC);
		dm.Fill4ColorsGradient (rectHalf,
			m_clrPlannerTodayCaption[0], m_clrPlannerTodayCaption[1],
			m_clrPlannerTodayCaption[1], m_clrPlannerTodayCaption[0], FALSE);
		
		rectHalf.top = rectHalf.bottom;
		rectHalf.bottom = rect.bottom;
		dm.Fill4ColorsGradient (rectHalf,
			m_clrPlannerTodayCaption[2], m_clrPlannerTodayCaption[3],
			m_clrPlannerTodayCaption[3], m_clrPlannerTodayCaption[2], FALSE);

		clrBorder = m_clrPlannerTodayBorder;
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		if (m_bPlannerBlack)
		{
			dm.Fill4ColorsGradient (rect,
				RGB (211, 214, 218), RGB (220, 223, 226),
				RGB (203, 206, 212), RGB (180, 186, 193), TRUE);
		}
		else
		{
			dm.Fill4ColorsGradient (rect,
				RGB (214, 226, 241), RGB (228, 236, 246),
				RGB (208, 222, 239), RGB (194, 212, 235), TRUE);
		}

		if (!bDay)
		{
			clrBorder = GetPlannerViewBackgroundColor (pView);
		}
	}

	if (clrBorder != CLR_DEFAULT)
	{
		if (!bDay)
		{
			rect.InflateRect (1, 0);
		}

		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}

	return clrText;
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawPlannerCaptionText (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, const CString& strText, 
		COLORREF clrText, int nAlign, BOOL bHighlight)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawPlannerCaptionText (pDC,
			pView, rect, strText, clrText, nAlign, bHighlight);
		return;
	}

	const int nTextMargin = 2;

	rect.DeflateRect (nTextMargin, 0);

	COLORREF clrOld = pDC->SetTextColor (clrText);

	pDC->DrawText (strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);

	pDC->SetTextColor (clrOld);
}
//*******************************************************************************
// in the future versions use base function
COLORREF CBCGPVisualManager2007::GetPlannerViewBackgroundColor (CBCGPPlannerView* pView)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerViewBackgroundColor (pView);
	}

	return m_clrPlannerWork;
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::GetPlannerHourLineColor (CBCGPPlannerView* pView,
		BOOL bWorkingHours, BOOL bHour)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerHourLineColor (pView, 
			bWorkingHours, bHour);
	}

	ASSERT_VALID (pView);

	COLORREF colorFill = m_bPlannerBlack ? RGB (199, 203, 209) : RGB (165, 191, 225);
		
	if (!bHour)
	{
		if (bWorkingHours)
		{
			colorFill = m_bPlannerBlack ? RGB (232, 234, 236) : RGB (230, 237, 247);
		}
		else
		{
			colorFill = m_bPlannerBlack ? RGB (221, 224, 227) : RGB (213, 225, 241);
		}
	}

	return colorFill;
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::GetPlannerViewWorkingColor (CBCGPPlannerView* pView)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerViewWorkingColor (pView);
	}

	return RGB (255, 255, 255);
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::GetPlannerViewNonWorkingColor (CBCGPPlannerView* pView)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerViewNonWorkingColor (pView);
	}

	ASSERT_VALID (pView);

	COLORREF colorFill = GetPlannerViewBackgroundColor (pView);

	if (pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewDay)))
	{
		colorFill = m_bPlannerBlack ? RGB (232, 234, 236) : RGB (230, 237, 247);
	}
	else
	{
		colorFill = m_bPlannerBlack ? RGB (199, 203, 209) : RGB (165, 191, 225);
	}

	return colorFill;
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::GetPlannerSelectionColor (CBCGPPlannerView* pView)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerSelectionColor (pView);
	}

	COLORREF colorFill = GetPlannerViewBackgroundColor (pView);
	colorFill = m_bPlannerBlack ? RGB (76, 83, 92) : RGB (41, 76, 122);

	return colorFill;
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::GetPlannerSeparatorColor (CBCGPPlannerView* pView)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerSeparatorColor (pView);
	}

	COLORREF colorFill = GetPlannerViewBackgroundColor (pView);
	colorFill = m_bPlannerBlack ? RGB (145, 153, 164) : RGB (93, 140, 201);

	return colorFill;
}
//*******************************************************************************
COLORREF CBCGPVisualManager2007::OnFillPlannerTimeBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, COLORREF& clrLine)
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::OnFillPlannerTimeBar (pDC, pView, rect, clrLine);
	}

	ASSERT_VALID (pDC);

	CBrush br (m_clrPlannerNcArea);
	pDC->FillRect (rect, &br);
	
	clrLine = m_clrPlannerNcLine;

	return m_clrPlannerNcText;
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawPlannerHeader (CDC* pDC, 
	CBCGPPlannerView* pView, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawPlannerHeader (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clr = GetPlannerViewBackgroundColor (pView);

	if (DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL)
	{
		clr = m_clrPlannerNcArea;
	}

	CBrush br (clr);
	pDC->FillRect (rect, &br);
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawPlannerHeaderPane (CDC* pDC, 
	CBCGPPlannerView* pView, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawPlannerHeaderPane (pDC, pView, rect);
		return;
	}
}
//*******************************************************************************
void CBCGPVisualManager2007::OnFillPlannerHeaderAllDay (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillPlannerHeaderAllDay (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);
	
	CBrush br (GetPlannerHourLineColor (pView, FALSE, TRUE));
	pDC->FillRect (rect, &br);
}
//*******************************************************************************
void CBCGPVisualManager2007::OnDrawPlannerHeaderAllDayItem (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnDrawPlannerHeaderAllDayItem (pDC, pView, rect, 
			bIsToday, bIsSelected);
		return;
	}

	ASSERT_VALID (pDC);

	rect.left++;
	if (bIsSelected)
	{
		CBrush br (GetPlannerSelectionColor (pView));
		pDC->FillRect (rect, &br);
	}

	if (bIsToday)
	{
		rect.top--;
		rect.right--;
		pDC->Draw3dRect (rect, m_clrPlannerTodayBorder, m_clrPlannerTodayBorder);

		rect.left--;
		rect.right++;
		rect.bottom++;
		pDC->Draw3dRect (rect, m_clrPlannerTodayBorder, m_clrPlannerTodayBorder);
	}
}
//*******************************************************************************
void CBCGPVisualManager2007::PreparePlannerBackItem (BOOL bIsToday, BOOL bIsSelected)
{
	m_bPlannerBackItemToday    = bIsToday;
	m_bPlannerBackItemSelected = bIsSelected;
}
//*******************************************************************************
DWORD CBCGPVisualManager2007::GetPlannerDrawFlags () const
{
	if (!CanDrawImage ())
	{
		return CBCGPVisualManager2003::GetPlannerDrawFlags ();
	}

	return BCGP_PLANNER_DRAW_APP_GRADIENT_FILL | 
		   BCGP_PLANNER_DRAW_APP_ROUNDED_CORNERS | 
		   BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION;
}

#endif // BCGP_EXCLUDE_PLANNER

CSize CBCGPVisualManager2007::GetSystemBorders (BOOL bRibbonPresent) const
{
	CSize size (::GetSystemMetrics (SM_CYSIZEFRAME), 
			    ::GetSystemMetrics (SM_CXSIZEFRAME));

	if (bRibbonPresent)
	{
		size.cx--;
		size.cy--;
	}

	return size;
}
//***************************************************************************
BOOL CBCGPVisualManager2007::OnEraseMDIClientArea (CDC* pDC, CRect rectClient)
{
	if (!CanDrawImage () ||
		m_brMainClientArea.GetSafeHandle () == NULL)
	{
		return CBCGPVisualManager2003::OnEraseMDIClientArea (pDC, rectClient);
	}

	pDC->FillRect (rectClient, &m_brMainClientArea);
	return TRUE;
}
//*******************************************************************************
BOOL CBCGPVisualManager2007::GetToolTipParams (CBCGPToolTipParams& params, 
											   UINT /*nType*/ /*= (UINT)(-1)*/)
{
	if (!CanDrawImage () ||
		!m_bToolTipParams)
	{
		return CBCGPVisualManager2003::GetToolTipParams (params);
	}

	params = m_ToolTipParams;
	return TRUE;
}
//*************************************************************************************
void CBCGPVisualManager2007::GetCalendarColors (const CBCGPCalendar* pCalendar,
				   CBCGPCalendarColors& colors)
{
	CBCGPVisualManager2003::GetCalendarColors (pCalendar, colors);

	if (!CanDrawImage ())
	{
		return;
	}

	colors.clrCaption = m_clrCaptionBarGradientDark;
	colors.clrCaptionText = m_clrCaptionBarText;

	colors.clrSelected = m_clrHighlightGradientDark;
	colors.clrSelectedText = m_clrOutlookPageTextHighlighted;
}

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManager2007::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPVisualManager2003::OnFillPopupWindowBackground (pDC, rect);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, m_clrPopupGradientDark, m_clrPopupGradientLight);
}

#endif

COLORREF CBCGPVisualManager2007::OnDrawPropSheetListItem (CDC* pDC, CBCGPPropertySheet* pParent, 
		CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	if (!CanDrawImage () || !m_ctrlRibbonBtn[0].IsValid ())
	{
		return CBCGPVisualManager2003::OnDrawPropSheetListItem (pDC, pParent, rect, bIsHighlihted, bIsSelected);
	}

	rect.DeflateRect (2, 1);

	int nIndex = 0;

	if (bIsSelected)
	{
		nIndex = bIsHighlihted ? 1 : 2;
	}

	m_ctrlRibbonBtn [0].Draw (pDC, rect, nIndex);
	return m_clrToolBarBtnTextHighlighted;
}
//***************************************************************************************
COLORREF CBCGPVisualManager2007::OnDrawMenuLabel (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.top = rectSeparator.bottom - 2;
	rectSeparator.right += 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarHilite);

	return globalData.clrBarText;
}
