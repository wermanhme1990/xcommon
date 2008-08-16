//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// BCGPVisualManager.cpp: implementation of the CBCGPPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "multimon.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManager.h"
#include "BCGPToolbarButton.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPOutlookButton.h"
#include "BCGGlobals.h"
#include "BCGPDockingControlBar.h"
#include "BCGPBaseControlBar.h"
#include "BCGPToolBar.h"
#include "BCGPTabWnd.h"
#include "BCGPDrawManager.h"
#include "BCGPShowAllButton.h"
#include "BCGPButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPCaptionBar.h"
#include "BCGPOutlookButton.h"
#include "BCGPTasksPane.h"
#include "BCGPSlider.h"
#include "BCGPCalendarBar.h"
#include "MenuImages.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPSpinButtonCtrl.h"
#include "BCGPDockManager.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPAutoHideButton.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPPlannerManagerCtrl.h"
#include "BCGPPropList.h"
#include "BCGPStatusBar.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonLabel.h"
#include "BCGPGridCtrl.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPToolTipCtrl.h"
#include "BCGPTooltipManager.h"

#ifndef BCGP_EXCLUDE_PLANNER
#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewMonth.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE (CBCGPVisualManager, CObject)

extern CObList	gAllToolbars;
extern CBCGPTooltipManager*	g_pTooltipManager;

CBCGPVisualManager*	CBCGPVisualManager::m_pVisManager = NULL;
CRuntimeClass*		CBCGPVisualManager::m_pRTIDefault = NULL;

UINT BCGM_CHANGEVISUALMANAGER = ::RegisterWindowMessage (_T("BCGM_CHANGEVISUALMANAGER"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManager::CBCGPVisualManager(BOOL bIsTemporary)
{
	m_bAutoDestroy = FALSE;
	m_bIsTemporary = bIsTemporary;

	if (!bIsTemporary)
	{
		if (m_pVisManager != NULL)
		{
			ASSERT (FALSE);
		}
		else
		{
			m_pVisManager = this;
		}
	}

	m_bLook2000 = FALSE;
	m_bMenuFlatLook = FALSE;
	m_nMenuShadowDepth = 6;
	m_bShadowHighlightedImage = FALSE;
	m_bEmbossDisabledImage = TRUE;
	m_bFadeInactiveImage = FALSE;
	m_bEnableToolbarButtonFill = TRUE;

	m_nVertMargin = 12;
	m_nHorzMargin = 12;
	m_nGroupVertOffset = 15;
	m_nGroupCaptionHeight = 25;
	m_nGroupCaptionHorzOffset = 13;
	m_nGroupCaptionVertOffset = 7;
	m_nTasksHorzOffset = 12;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = TRUE;

	m_bOfficeXPStyleMenus = FALSE;
	m_nMenuBorderSize = 2;

	m_b3DTabWideBorder = TRUE;
	m_bAlwaysFillTab = FALSE;
	m_bFrameMenuCheckedItems = FALSE;
	m_clrMenuShadowBase = (COLORREF)-1;	// Used in derived classes

	if (!bIsTemporary)
	{
		CBCGPDockManager::m_bSDParamsModified = TRUE;
		CBCGPDockManager::EnableDockBarMenu (FALSE);

		CBCGPAutoHideButton::m_bOverlappingTabs = TRUE;

		globalData.UpdateSysColors ();
	}

	m_bPlannerBackItemToday         = FALSE;
	m_bPlannerBackItemSelected      = FALSE;
	m_bPlannerCaptionBackItemHeader = FALSE;

	OnUpdateSystemColors ();
}
//*************************************************************************************
CBCGPVisualManager::~CBCGPVisualManager()
{
	if (!m_bIsTemporary)
	{
		m_pVisManager = NULL;
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnUpdateSystemColors ()
{
	m_clrPlannerWork      = RGB (255, 255, 0);
	m_clrPalennerLine     = RGB (128, 128, 128);
}
//*************************************************************************************
void CBCGPVisualManager::SetDefaultManager (CRuntimeClass* pRTI)
{
	if (pRTI != NULL &&
		!pRTI->IsDerivedFrom (RUNTIME_CLASS (CBCGPVisualManager)))
	{
		ASSERT (FALSE);
		return;
	}

	m_pRTIDefault = pRTI;

	if (m_pVisManager != NULL)
	{
		ASSERT_VALID (m_pVisManager);

		delete m_pVisManager;
		m_pVisManager = NULL;
	}

	globalData.UpdateSysColors ();

	CBCGPDockManager::SetDockMode (DT_STANDARD);
	CBCGPTabbedControlBar::ResetTabs ();

	AdjustFrames ();
	AdjustToolbars ();

	RedrawAll ();

	if (g_pTooltipManager != NULL)
	{
		g_pTooltipManager->UpdateTooltips ();
	}
}
//*************************************************************************************
void CBCGPVisualManager::RedrawAll ()
{
	CWnd* pMainWnd = AfxGetMainWnd ();
	BOOL bIsMainWndRedrawn = FALSE;

	const CList<CFrameWnd*, CFrameWnd*>& lstFrames = 
		CBCGPFrameImpl::GetFrameList ();

	for (POSITION pos = lstFrames.GetHeadPosition (); pos != NULL;)
	{
		CFrameWnd* pFrame = lstFrames.GetNext (pos);

		if (CWnd::FromHandlePermanent (pFrame->m_hWnd) != NULL)
		{
			ASSERT_VALID (pFrame);

			pFrame->RedrawWindow (NULL, NULL,
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN | RDW_FRAME);

			if (pFrame->GetSafeHwnd () == pMainWnd->GetSafeHwnd ())
			{
				bIsMainWndRedrawn = FALSE;
			}
		}
	}

	if (!bIsMainWndRedrawn && pMainWnd->GetSafeHwnd () != NULL &&
		CWnd::FromHandlePermanent (pMainWnd->m_hWnd) != NULL)
	{
		pMainWnd->RedrawWindow (NULL, NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN | RDW_FRAME);
	}

	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPControlBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPControlBar, gAllToolbars.GetNext (posTlb));
		if (pToolBar != NULL)
		{
			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID (pToolBar);
				
				pToolBar->RedrawWindow (NULL, NULL,
					RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
			}
		}
	}

	CBCGPMiniFrameWnd::RedrawAll ();
}
//*************************************************************************************
void CBCGPVisualManager::AdjustToolbars ()
{
	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, gAllToolbars.GetNext (posTlb));
		if (pToolBar != NULL)
		{
			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID (pToolBar);
				pToolBar->OnChangeVisualManager ();
			}
		}
	}
}
//*************************************************************************************
void CBCGPVisualManager::AdjustFrames ()
{
	const CList<CFrameWnd*, CFrameWnd*>& lstFrames = 
		CBCGPFrameImpl::GetFrameList ();

	for (POSITION pos = lstFrames.GetHeadPosition (); pos != NULL;)
	{
		CFrameWnd* pFrame = lstFrames.GetNext (pos);

		if (CWnd::FromHandlePermanent (pFrame->m_hWnd) != NULL)
		{
			ASSERT_VALID (pFrame);
			pFrame->SendMessage (BCGM_CHANGEVISUALMANAGER);
		}
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	const COLORREF clrHilite = pBar != NULL && pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar != NULL && pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	const BOOL bSingleGripper = m_bLook2000;

	const int iGripperSize = 3;
	const int iGripperOffset = bSingleGripper ? 0 : 1;
	const int iLinesNum = bSingleGripper ? 1 : 2;

	if (bHorz)
	{
		//-----------------
		// Gripper at left:
		//-----------------
		rectGripper.DeflateRect (0, bSingleGripper ? 3 : 2);

		// ET: Center the grippers
		rectGripper.left = iGripperOffset + rectGripper.CenterPoint().x - 
			( iLinesNum*iGripperSize + (iLinesNum-1)*iGripperOffset) / 2;

		rectGripper.right = rectGripper.left + iGripperSize;

 
		for (int i = 0; i < iLinesNum; i ++)
		{
			pDC->Draw3dRect (rectGripper, 
							clrHilite,
							clrShadow);

			// ET: not used for NewFlat look
			if(! bSingleGripper ) {
				//-----------------------------------
				// To look same as MS Office Gripper!
				//-----------------------------------
				pDC->SetPixel (CPoint (rectGripper.left, rectGripper.bottom - 1),
								clrHilite);
			}

			rectGripper.OffsetRect (iGripperSize+1, 0);
		}
	} 
	else 
	{
		//----------------
		// Gripper at top:
		//----------------
		rectGripper.top += iGripperOffset;
		rectGripper.DeflateRect (bSingleGripper ? 3 : 2, 0);

		// ET: Center the grippers
		rectGripper.top = iGripperOffset + rectGripper.CenterPoint().y - 
			( iLinesNum*iGripperSize + (iLinesNum-1)) / 2;

		rectGripper.bottom = rectGripper.top + iGripperSize;

		for (int i = 0; i < iLinesNum; i ++)
		{
			pDC->Draw3dRect (rectGripper,
							clrHilite,
							clrShadow);

			// ET: not used for NewFlat look
			if(! bSingleGripper ) {
				//-----------------------------------
				// To look same as MS Office Gripper!
				//-----------------------------------
				pDC->SetPixel (CPoint (rectGripper.right - 1, rectGripper.top),
								clrHilite);
			}

			rectGripper.OffsetRect (0, iGripperSize+1);
		}
	}
}
//*************************************************************************************
void CBCGPVisualManager::SetLook2000 (BOOL bLook2000)
{
	m_bLook2000 = bLook2000;

	if (AfxGetMainWnd () != NULL)
	{
		AfxGetMainWnd()->RedrawWindow (NULL, NULL,
			RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
	}

	RedrawAll ();
}
//*************************************************************************************
void CBCGPVisualManager::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL /*bNCArea*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	if (DYNAMIC_DOWNCAST (CReBar, pBar) != NULL ||
		DYNAMIC_DOWNCAST (CReBar, pBar->GetParent ()))
	{
		FillRebarPane (pDC, pBar, rectClient);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPOutlookBarPane)))
	{
		((CBCGPOutlookBarPane*) pBar)->OnEraseWorkArea (pDC, rectClient);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPCaptionBar)))
	{
		CBCGPCaptionBar* pCaptionBar = (CBCGPCaptionBar*) pBar;

		pDC->FillSolidRect	(rectClip, pCaptionBar->m_clrBarBackground == -1 ? 
			globalData.clrBarShadow : pCaptionBar->m_clrBarBackground);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*) pBar;

		if (pMenuBar->IsDropDownListMode ())
		{
			pDC->FillRect (rectClip, &globalData.brWindow);
			return;
		}
	}

	// By default, control bar background is filled by 
	// the system 3d background color

	pDC->FillRect (rectClip.IsRectEmpty () ? rectClient : rectClip, 
				pBar->IsDialogControl () ?
					&globalData.brBtnFace : &globalData.brBarFace);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsFloating ())
	{
		return;
	}

	DWORD dwStyle = pBar->GetBarStyle  ();
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	COLORREF clrBckOld = pDC->GetBkColor ();	// FillSolidRect changes it

	// Copyright (C) 1997,'98 by Joerg Koenig
	// All rights reserved
	//

	const COLORREF clrHilite = pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	COLORREF clr = clrHilite;

	if(dwStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, clr);
	if(dwStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, clr);
	if(dwStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.right, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, clrShadow);
	if(dwStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, clrShadow);

	// if undockable toolbar at top of frame, apply special formatting to mesh
	// properly with frame menu
	if(!pBar->CanFloat ()) 
	{
		pDC->FillSolidRect(0,0,rect.Width(),1,clrShadow);
		pDC->FillSolidRect(0,1,rect.Width(),1,clrHilite);
	}

	if (dwStyle & CBRS_BORDER_LEFT)
		++rect.left;
	if (dwStyle & CBRS_BORDER_TOP)
		++rect.top;
	if (dwStyle & CBRS_BORDER_RIGHT)
		--rect.right;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		--rect.bottom;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* /*pMenu*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBarLight, globalData.clrBarDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuShadow (CDC* pPaintDC, const CRect& rectClient, const CRect& /*rectExclude*/,
								int nDepth,  int iMinBrightness,  int iMaxBrightness,  
								CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight,
								BOOL bRTL)
{
	ASSERT_VALID (pPaintDC);
	ASSERT_VALID (pBmpSaveBottom);
	ASSERT_VALID (pBmpSaveRight);

	//------------------------------------------------------
	// Simple draw the shadow, ignore rectExclude parameter:
	//------------------------------------------------------
	CBCGPDrawManager dm (*pPaintDC);
	dm.DrawShadow (rectClient, nDepth, iMinBrightness, iMaxBrightness,
				pBmpSaveBottom, pBmpSaveRight, (COLORREF)-1, !bRTL);
}
//************************************************************************************
void CBCGPVisualManager::OnFillButtonInterior (CDC* pDC,
				CBCGPToolbarButton* pButton, CRect rect,
				CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPShowAllButton)))
	{
		if (state == ButtonsIsHighlighted)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rect);
		}

		return;
	}

	if (!m_bEnableToolbarButtonFill)
	{
		BOOL bIsPopupMenu = FALSE;

		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL)
		{
			bIsPopupMenu = pMenuButton->GetParentWnd () != NULL &&
				pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));
		}

		if (!bIsPopupMenu)
		{
			return;
		}
	}

	if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)) &&
		!CBCGPToolBar::IsCustomizeMode () && state != ButtonsIsHighlighted &&
		(pButton->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
	{
		CRect rectDither = rect;
		rectDither.InflateRect (-afxData.cxBorder2, -afxData.cyBorder2);

		CBCGPToolBarImages::FillDitheredRect (pDC, rectDither);
	}
}
//************************************************************************************
COLORREF CBCGPVisualManager::GetToolbarHighlightColor ()
{
	return globalData.clrBarFace;
}
//************************************************************************************
COLORREF CBCGPVisualManager::GetToolbarDisabledTextColor ()
{
	return globalData.clrGrayedText;
}
//************************************************************************************
void CBCGPVisualManager::OnHighlightMenuItem (CDC*pDC, CBCGPToolbarMenuButton* /*pButton*/,
											CRect rect, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brHilite);
}
//************************************************************************************
COLORREF CBCGPVisualManager::GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton)
{
	ASSERT_VALID (pButton);

	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		return globalData.clrGrayedText;
	}

	return globalData.clrTextHilite;
}
//************************************************************************************
void CBCGPVisualManager::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	dm.HighlightRect (rectRarelyUsed);

	pDC->Draw3dRect (rectRarelyUsed, globalData.clrBarShadow, globalData.clrBarHilite);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton,
		CRect rectCheck, BOOL /*bHighlight*/, BOOL bIsRadio)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	int iImage = bIsRadio ? CMenuImages::IdRadio : CMenuImages::IdCheck;
	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, 
		rectCheck,
		(pButton->m_nStyle & TBBS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuItemButton (CDC* pDC, CBCGPToolbarMenuButton* /*pButton*/,
				CRect rectButton, BOOL bHighlight, BOOL /*bDisabled*/)
{
	ASSERT_VALID (pDC);

	CRect rect = rectButton;
	rect.right = rect.left + 1;
	rect.left--;
	rect.DeflateRect (0, bHighlight ? 1 : 4);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	BOOL bIsOutlookButton = pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton));
	COLORREF clrDark = bIsOutlookButton ? 
					   globalData.clrBarDkShadow : globalData.clrBarShadow;

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->Draw3dRect (&rect, clrDark, globalData.clrBarHilite);
		return;

	case ButtonsIsHighlighted:
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, clrDark);
		return;
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL /*bHorz*/)
{
	ASSERT_VALID (pButton);

	if (!m_bMenuFlatLook || !pButton->IsDroppedDown ())
	{
		OnDrawButtonBorder (pDC, pButton, rect, state);
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pBar);
	ASSERT_VALID (pDC);

	CRect rectSeparator = rect;

	if (bHorz)
	{
		rectSeparator.left += rectSeparator.Width () / 2 - 1;
		rectSeparator.right = rectSeparator.left + 2;
	}
	else
	{
		rectSeparator.top += rectSeparator.Height () / 2 - 1;
		rectSeparator.bottom = rectSeparator.top + 2;
	}

	const COLORREF clrHilite = pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	pDC->Draw3dRect (rectSeparator, clrShadow,
									clrHilite);
}
//***************************************************************************************
COLORREF CBCGPVisualManager::OnDrawMenuLabel (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);

	CRect rectSeparator = rect;
	rectSeparator.top = rectSeparator.bottom - 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBtnShadow,
									globalData.clrBtnHilite);

	return globalData.clrBtnText;
}
//***************************************************************************************
COLORREF CBCGPVisualManager::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* /*pBar*/, 
			BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
	ASSERT_VALID (pDC);

	CBrush br (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

    // get the text color
	return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawCaptionButton (
						CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive,
						BOOL bHorz, BOOL bMaximized, BOOL bDisabled, 
						int nImageID /*= -1*/)
{
	ASSERT_VALID (pDC);
    CRect rc = pButton->GetRect ();

	CMenuImages::IMAGES_IDS id = (CMenuImages::IMAGES_IDS)-1;
	
	if (nImageID != -1)
	{
		id = (CMenuImages::IMAGES_IDS)nImageID;
	}
	else
	{
		id = pButton->GetIconID (bHorz, bMaximized);
	}

	CRect rectImage = rc;

	if (pButton->m_bPushed && (pButton->m_bFocused || pButton->m_bDroppedDown))
	{
		rectImage.OffsetRect (1, 1);
	}

	CMenuImages::IMAGE_STATE imageState;
	
	if (bDisabled)
	{
		imageState = CMenuImages::ImageGray;
	}
	else if (pButton->m_clrForeground == (COLORREF)-1)
	{
		imageState = bActive ? CMenuImages::ImageWhite : CMenuImages::ImageBlack;
	}
	else
	{
		if (GetRValue (pButton->m_clrForeground) > 192 &&
			GetGValue (pButton->m_clrForeground) > 192 &&
			GetBValue (pButton->m_clrForeground) > 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}

	CMenuImages::Draw (pDC, id, rectImage, imageState);

	if (!bDisabled)
	{
		if (pButton->m_bPushed && (pButton->m_bFocused || pButton->m_bDroppedDown))
		{
			pDC->Draw3dRect (rc, globalData.clrBarDkShadow, globalData.clrBarLight);
			rc.DeflateRect (1, 1);
			pDC->Draw3dRect (rc, globalData.clrBarDkShadow, globalData.clrBarHilite);
		}
		else if (!m_bLook2000)
		{
			pDC->Draw3dRect (rc, globalData.clrBarLight, globalData.clrBarDkShadow);
			rc.DeflateRect (1, 1);
			pDC->Draw3dRect (rc, globalData.clrBarHilite, globalData.clrBarShadow);
		}
		else if (pButton->m_bFocused || pButton->m_bPushed || pButton->m_bDroppedDown)
		{
			pDC->Draw3dRect (rc, globalData.clrBarHilite, globalData.clrBarShadow);
		}
	}
}
//***********************************************************************************
void CBCGPVisualManager::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* /*pTabWnd*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	COLORREF clrTab = pTabWnd->GetTabBkColor (iTab);

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	if (pTabWnd->IsFlatTab ())
	{
		//----------------
		// Draw tab edges:
		//----------------
		#define FLAT_POINTS_NUM	4
		POINT pts [FLAT_POINTS_NUM];

		const int nHalfHeight = pTabWnd->GetTabsHeight () / 2;

		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			rectTab.bottom --;

			pts [0].x = rectTab.left;
			pts [0].y = rectTab.top;

			pts [1].x = rectTab.left + nHalfHeight;
			pts [1].y = rectTab.bottom;

			pts [2].x = rectTab.right - nHalfHeight;
			pts [2].y = rectTab.bottom;

			pts [3].x = rectTab.right;
			pts [3].y = rectTab.top;
		}
		else
		{
			rectTab.top ++;

			pts [0].x = rectTab.left + nHalfHeight;
			pts [0].y = rectTab.top;

			pts [1].x = rectTab.left;
			pts [1].y = rectTab.bottom;

			pts [2].x = rectTab.right;
			pts [2].y = rectTab.bottom;

			pts [3].x = rectTab.right - nHalfHeight;
			pts [3].y = rectTab.top;

			rectTab.left += 2;
		}

		CBrush* pOldBrush = NULL;
		CBrush br (clrTab);

		if (!bIsActive && clrTab != (COLORREF)-1)
		{
			pOldBrush = pDC->SelectObject (&br);
		}

		pDC->Polygon (pts, FLAT_POINTS_NUM);

		if (pOldBrush != NULL)
		{
			pDC->SelectObject (pOldBrush);
		}
	}
	else if (pTabWnd->IsLeftRightRounded ())
	{
		CList<POINT, POINT> pts;

		POSITION posLeft = pts.AddHead (CPoint (rectTab.left, rectTab.top));
		posLeft = pts.InsertAfter (posLeft, CPoint (rectTab.left, rectTab.top + 2));

		POSITION posRight = pts.AddTail (CPoint (rectTab.right, rectTab.top));
		posRight = pts.InsertBefore (posRight, CPoint (rectTab.right, rectTab.top + 2));

		int xLeft = rectTab.left + 1;
		int xRight = rectTab.right - 1;

		int y = 0;

		for (y = rectTab.top + 2; y < rectTab.bottom - 4; y += 2)
		{
			posLeft = pts.InsertAfter (posLeft, CPoint (xLeft, y));
			posLeft = pts.InsertAfter (posLeft, CPoint (xLeft, y + 2));

			posRight = pts.InsertBefore (posRight, CPoint (xRight, y));
			posRight = pts.InsertBefore (posRight, CPoint (xRight, y + 2));

			xLeft++;
			xRight--;
		}

		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_TOP)
		{
			xLeft--;
			xRight++;
		}

		const int nTabLeft = xLeft - 1;
		const int nTabRight = xRight + 1;

		for (;y < rectTab.bottom - 1; y++)
		{
			posLeft = pts.InsertAfter (posLeft, CPoint (xLeft, y));
			posLeft = pts.InsertAfter (posLeft, CPoint (xLeft + 1, y + 1));

			posRight = pts.InsertBefore (posRight, CPoint (xRight, y));
			posRight = pts.InsertBefore (posRight, CPoint (xRight - 1, y + 1));

			if (y == rectTab.bottom - 2)
			{
				posLeft = pts.InsertAfter (posLeft, CPoint (xLeft + 1, y + 1));
				posLeft = pts.InsertAfter (posLeft, CPoint (xLeft + 3, y + 1));

				posRight = pts.InsertBefore (posRight, CPoint (xRight, y + 1));
				posRight = pts.InsertBefore (posRight, CPoint (xRight - 2, y + 1));
			}

			xLeft++;
			xRight--;
		}

		posLeft = pts.InsertAfter (posLeft, CPoint (xLeft + 2, rectTab.bottom));
		posRight = pts.InsertBefore (posRight, CPoint (xRight - 2, rectTab.bottom));

		LPPOINT points = new POINT [pts.GetCount ()];

		int i = 0;

		for (POSITION pos = pts.GetHeadPosition (); pos != NULL; i++)
		{
			points [i] = pts.GetNext (pos);

			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_TOP)
			{
				points [i].y = rectTab.bottom - (points [i].y - rectTab.top);
			}
		}

		CRgn rgnClip;
		rgnClip.CreatePolygonRgn (points, (int) pts.GetCount (), WINDING);

		pDC->SelectClipRgn (&rgnClip);

		CBrush br (clrTab == (COLORREF)-1 ? globalData.clrBtnFace : clrTab);
		OnFillTab (pDC, rectTab, &br, iTab, bIsActive, pTabWnd);

		pDC->SelectClipRgn (NULL);

		CPen pen (PS_SOLID, 1, globalData.clrBarShadow);
		CPen* pOLdPen = pDC->SelectObject (&pen);

		for (i = 0; i < pts.GetCount (); i++)
		{
			if ((i % 2) != 0)
			{
				int x1 = points [i - 1].x;
				int y1 = points [i - 1].y;

				int x2 = points [i].x;
				int y2 = points [i].y;

				if (x1 > rectTab.CenterPoint ().x && x2 > rectTab.CenterPoint ().x)
				{
					x1--;
					x2--;
				}

				if (y2 >= y1)
				{
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x2, y2);
				}
				else
				{
					pDC->MoveTo (x2, y2);
					pDC->LineTo (x1, y1);
				}
			}
		}

		delete [] points;
		pDC->SelectObject (pOLdPen);

		rectTab.left = nTabLeft;
		rectTab.right = nTabRight;
	}
	else	// 3D Tab
	{
		CRgn rgnClip;

		CRect rectClip;
		pTabWnd->GetTabsRect (rectClip);

		BOOL bIsCutted = FALSE;

		const BOOL bIsOneNote = pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ();

		const int nExtra = bIsOneNote ?
			((iTab == 0 || bIsActive || pTabWnd->IsVS2005Style ()) ? 
			0 : rectTab.Height ()) : 0;

		if (rectTab.left + nExtra + 10 > rectClip.right ||
			rectTab.right - 10 <= rectClip.left)
		{
			return;
		}

		const int iVertOffset = 2;
		const int iHorzOffset = 2;
		const BOOL bIs2005 = pTabWnd->IsVS2005Style ();

		#define POINTS_NUM	8
		POINT pts [POINTS_NUM];

		if (!bIsActive || bIsOneNote || clrTab != (COLORREF)-1 || m_bAlwaysFillTab)
		{
			if (clrTab != (COLORREF)-1 || bIsOneNote || m_bAlwaysFillTab)
			{
				CRgn rgn;
				CBrush br (clrTab == (COLORREF)-1 ? globalData.clrBtnFace : clrTab);

				CRect rectFill = rectTab;

				if (bIsOneNote)
				{
					CRect rectFill = rectTab;

					const int nHeight = rectFill.Height ();

					pts [0].x = rectFill.left;
					pts [0].y = rectFill.bottom;

					pts [1].x = bIs2005 ? rectFill.left : rectFill.left + 2;
					pts [1].y = bIs2005 ? rectFill.bottom : rectFill.bottom - 1;

					pts [2].x = bIs2005 ? rectFill.left + 2 : rectFill.left + 4;
					pts [2].y = bIs2005 ? rectFill.bottom : rectFill.bottom - 2;
					
					pts [3].x = rectFill.left + nHeight;
					pts [3].y = rectFill.top + 2;
					
					pts [4].x = rectFill.left + nHeight + 4;
					pts [4].y = rectFill.top;
					
					pts [5].x = rectFill.right - 2;
					pts [5].y = rectFill.top;
					
					pts [6].x = rectFill.right;
					pts [6].y = rectFill.top + 2;

					pts [7].x = rectFill.right;
					pts [7].y = rectFill.bottom;

					for (int i = 0; i < POINTS_NUM; i++)
					{
						if (pts [i].x > rectClip.right)
						{
							pts [i].x = rectClip.right;
							bIsCutted = TRUE;
						}

						if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
						{
							pts [i].y = rectFill.bottom - pts [i].y + rectFill.top - 1;
						}
					}

					rgn.CreatePolygonRgn (pts, POINTS_NUM, WINDING);
					pDC->SelectClipRgn (&rgn);
				}
				else
				{
					rectFill.DeflateRect (1, 0);

					if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
					{
						rectFill.bottom--;
					}
					else
					{
						rectFill.top++;
					}

					rectFill.right = min (rectFill.right, rectClip.right);
				}

				OnFillTab (pDC, rectFill, &br, iTab, bIsActive, pTabWnd);
				pDC->SelectClipRgn (NULL);

				if (bIsOneNote)
				{
					CRect rectLeft;
					pTabWnd->GetClientRect (rectLeft);
					rectLeft.right = rectClip.left - 1;

					pDC->ExcludeClipRect (rectLeft);

					if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum ())
					{
						CRect rectLeftTab = rectClip;
						rectLeftTab.right = rectFill.left + rectFill.Height () - 10;

						const int nVertOffset = bIs2005 ? 2 : 1;

						if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
						{
							rectLeftTab.top -= nVertOffset;
						}
						else
						{
							rectLeftTab.bottom += nVertOffset;
						}

						pDC->ExcludeClipRect (rectLeftTab);
					}

					pDC->Polyline (pts, POINTS_NUM);

					if (bIsCutted)
					{
						pDC->MoveTo (rectClip.right, rectTab.top);
						pDC->LineTo (rectClip.right, rectTab.bottom);
					}

					CRect rectRight = rectClip;
					rectRight.left = rectFill.right;

					pDC->ExcludeClipRect (rectRight);
				}
			}
		}

		CPen penLight (PS_SOLID, 1, globalData.clrBarHilite);
		CPen penShadow (PS_SOLID, 1, globalData.clrBarShadow);
		CPen penDark (PS_SOLID, 1, globalData.clrBarDkShadow);

		CPen* pOldPen = NULL;

		if (bIsOneNote)
		{
			pOldPen = (CPen*) pDC->SelectObject (&penLight);
			ASSERT(pOldPen != NULL);

			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
			{
				if (!bIsCutted)
				{
					int yTop = bIsActive ? pts [7].y - 1 : pts [7].y;

					pDC->MoveTo (pts [6].x - 1, pts [6].y);
					pDC->LineTo (pts [7].x - 1, yTop);
				}
			}
			else
			{
				pDC->MoveTo (pts [2].x - 1, pts [2].y + 1);
				pDC->LineTo (pts [2].x, pts [2].y + 1);

				pDC->MoveTo (pts [2].x, pts [2].y + 1);
				pDC->LineTo (pts [3].x + 1, pts [3].y);

				pDC->MoveTo (pts [3].x + 1, pts [3].y);
				pDC->LineTo (pts [3].x + 2, pts [3].y);

				pDC->MoveTo (pts [3].x + 2, pts [3].y);
				pDC->LineTo (pts [3].x + 3, pts [3].y);

				pDC->MoveTo (pts [4].x - 1, pts [4].y + 1);
				pDC->LineTo (pts [5].x + 1, pts [5].y + 1);

				if (!bIsActive && !bIsCutted && m_b3DTabWideBorder)
				{
					pDC->SelectObject (&penShadow);

					pDC->MoveTo (pts [6].x - 2, pts [6].y - 1);
					pDC->LineTo (pts [6].x - 1, pts [6].y - 1);
				}

				pDC->MoveTo (pts [6].x - 1, pts [6].y);
				pDC->LineTo (pts [7].x - 1, pts [7].y);
			}
		}
		else
		{
			if (rectTab.right > rectClip.right)
			{
				CRect rectTabClip = rectTab;
				rectTabClip.right = rectClip.right;

				rgnClip.CreateRectRgnIndirect (&rectTabClip);
				pDC->SelectClipRgn (&rgnClip);
			}

			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
			{
				pOldPen = (CPen*) pDC->SelectObject (&penLight);
				ASSERT(pOldPen != NULL);

				if (!m_b3DTabWideBorder)
				{
					pDC->SelectObject(&penShadow);
				}

				pDC->MoveTo (rectTab.left, rectTab.top);
				pDC->LineTo (rectTab.left, rectTab.bottom - iVertOffset);

				if (m_b3DTabWideBorder)
				{
					pDC->SelectObject (&penDark);
				}

				pDC->LineTo (rectTab.left + iHorzOffset, rectTab.bottom);
				pDC->LineTo (rectTab.right - iHorzOffset, rectTab.bottom);
				pDC->LineTo (rectTab.right, rectTab.bottom - iVertOffset);
				pDC->LineTo (rectTab.right, rectTab.top - 1);

				pDC->SelectObject(&penShadow);

				if (m_b3DTabWideBorder)
				{
					pDC->MoveTo (rectTab.left + iHorzOffset + 1, rectTab.bottom - 1);
					pDC->LineTo (rectTab.right - iHorzOffset, rectTab.bottom - 1);
					pDC->LineTo (rectTab.right - 1, rectTab.bottom - iVertOffset);
					pDC->LineTo (rectTab.right - 1, rectTab.top - 1);
				}
			}
			else
			{
				pOldPen = pDC->SelectObject (
					m_b3DTabWideBorder ? &penDark : &penShadow);

				ASSERT(pOldPen != NULL);

				pDC->MoveTo (rectTab.right, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
				pDC->LineTo (rectTab.right, rectTab.top + iVertOffset);
				pDC->LineTo (rectTab.right - iHorzOffset, rectTab.top);
				
				if (m_b3DTabWideBorder)
				{
					pDC->SelectObject (&penLight);
				}
				
				pDC->LineTo (rectTab.left + iHorzOffset, rectTab.top);
				pDC->LineTo (rectTab.left, rectTab.top + iVertOffset);

				pDC->LineTo (rectTab.left, rectTab.bottom);

				if (m_b3DTabWideBorder)
				{
					pDC->SelectObject (&penShadow);
					
					pDC->MoveTo (rectTab.right - 1, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
					pDC->LineTo (rectTab.right - 1, rectTab.top + iVertOffset - 1);
				}
 			}
		}

		if (bIsActive)
		{
			const int iBarHeight = 1;
			const int y = (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM) ? 
				(rectTab.top - iBarHeight - 1) : (rectTab.bottom);

			CRect rectFill (CPoint (rectTab.left, y), 
							CSize (rectTab.Width (), iBarHeight + 1));

			COLORREF clrActiveTab = pTabWnd->GetTabBkColor (iTab);

			if (bIsOneNote)
			{
				if (bIs2005)
				{
					rectFill.left += 3;
				}
				else
				{
					rectFill.OffsetRect (1, 0);
					rectFill.left++;
				}

				if (clrActiveTab == (COLORREF)-1)
				{
					clrActiveTab = globalData.clrWindow;
				}
			}

			if (clrActiveTab != (COLORREF)-1)
			{
				CBrush br (clrActiveTab);
				pDC->FillRect (rectFill, &br);
			}
			else
			{
				pDC->FillRect (rectFill, &globalData.brBarFace);
			}
		}

		pDC->SelectObject (pOldPen);

		if (bIsOneNote)
		{
			const int nLeftMargin = pTabWnd->IsVS2005Style () && bIsActive ?
				rectTab.Height () * 3 / 4 : rectTab.Height ();

			const int nRightMargin = pTabWnd->IsVS2005Style () && bIsActive ?
				CBCGPBaseTabWnd::TAB_IMAGE_MARGIN * 3 / 4 : CBCGPBaseTabWnd::TAB_IMAGE_MARGIN;

			rectTab.left += nLeftMargin;
			rectTab.right -= nRightMargin;

			if (pTabWnd->IsVS2005Style () && bIsActive && pTabWnd->HasImage (iTab))
			{
				rectTab.OffsetRect (CBCGPBaseTabWnd::TAB_IMAGE_MARGIN, 0);
			}
		}

		pDC->SelectClipRgn (NULL);
	}

	COLORREF clrText = pTabWnd->GetTabTextColor (iTab);
	
	COLORREF cltTextOld = (COLORREF)-1;
	if (!bIsActive && clrText != (COLORREF)-1)
	{
		cltTextOld = pDC->SetTextColor (clrText);
	}

	if (pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ())
	{
		CRect rectClip;
		pTabWnd->GetTabsRect (rectClip);

		rectTab.right = min (rectTab.right, rectClip.right - 2);
	}

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rectClip);

	pDC->SelectClipRgn (&rgn);

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, (COLORREF)-1);

	if (cltTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (cltTextOld);
	}

	pDC->SelectClipRgn (NULL);
}
//*********************************************************************************
void CBCGPVisualManager::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pbrFill);
	ASSERT_VALID (pTabWnd);

	if (bIsActive && !globalData.IsHighContastMode () &&
		(pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style () || pTabWnd->IsLeftRightRounded ()) &&
		pTabWnd->GetTabBkColor (iTab) == (COLORREF)-1)
	{
		pDC->FillRect (rectFill, &globalData.brWindow);
	}
	else
	{
		pDC->FillRect (rectFill, pbrFill);
	}
}
//*********************************************************************************
BOOL CBCGPVisualManager::OnEraseTabsFrame (CDC* pDC, CRect rect, 
										   const CBCGPBaseTabWnd* pTabWnd) 
{	
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (clrActiveTab == (COLORREF)-1)
	{
		return FALSE;
	}

	pDC->FillSolidRect (rect, clrActiveTab);
	return TRUE;
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawTabContent (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
						COLORREF clrText)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	CString strText;
	pTabWnd->GetTabLabel (iTab, strText);

	if (pTabWnd->IsFlatTab ())
	{
		//---------------
		// Draw tab text:
		//---------------
		UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
		if (pTabWnd->IsDrawNoPrefix ())
		{
			nFormat |= DT_NOPREFIX;
		}

		pDC->DrawText (strText, rectTab, nFormat);
	}
	else
	{
		CSize sizeImage = pTabWnd->GetImageSize ();
		UINT uiIcon = pTabWnd->GetTabIcon (iTab);
		HICON hIcon = pTabWnd->GetTabHicon (iTab);

		if (uiIcon == (UINT)-1 && hIcon == NULL)
		{
			sizeImage.cx = 0;
		}

		if (sizeImage.cx + 2 * CBCGPBaseTabWnd::TAB_IMAGE_MARGIN <= rectTab.Width ())
		{
			if (hIcon != NULL)
			{
				//---------------------
				// Draw the tab's icon:
				//---------------------
				CRect rectImage = rectTab;

				rectImage.top += (rectTab.Height () - sizeImage.cy) / 2;
				rectImage.bottom = rectImage.top + sizeImage.cy;

				rectImage.left += IMAGE_MARGIN;
				rectImage.right = rectImage.left + sizeImage.cx;

				pDC->DrawState (rectImage.TopLeft (), rectImage.Size (), 
					hIcon, DSS_NORMAL, (HBRUSH) NULL);
			}
			else
			{
				const CImageList* pImageList = pTabWnd->GetImageList ();
				if (pImageList != NULL && uiIcon != (UINT)-1)
				{
					//----------------------
					// Draw the tab's image:
					//----------------------
					CRect rectImage = rectTab;

					rectImage.top += (rectTab.Height () - sizeImage.cy) / 2;
					rectImage.bottom = rectImage.top + sizeImage.cy;

					rectImage.left += IMAGE_MARGIN;
					rectImage.right = rectImage.left + sizeImage.cx;

					ASSERT_VALID (pImageList);
					((CImageList*) pImageList)->Draw (pDC, uiIcon, rectImage.TopLeft (), ILD_TRANSPARENT);
				}
			}

			//------------------------------
			// Finally, draw the tab's text:
			//------------------------------
			CRect rcText = rectTab;
			rcText.left += sizeImage.cx + 2 * TEXT_MARGIN;

			if (rcText.Width () < sizeImage.cx * 2 && 
				!pTabWnd->IsLeftRightRounded ())
			{
				rcText.right -= TEXT_MARGIN;
			}
			
			if (clrText == (COLORREF)-1)
			{
				clrText = GetTabTextColor (pTabWnd, iTab, bIsActive);
			}

			if (clrText != (COLORREF)-1)
			{
				pDC->SetTextColor (clrText);
			}

			CString strText;
			pTabWnd->GetTabLabel (iTab, strText);

			UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
			if (pTabWnd->IsDrawNoPrefix ())
			{
				nFormat |= DT_NOPREFIX;
			}

			if (pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ())
			{
				nFormat |= DT_CENTER;
			}
			else
			{
				nFormat |= DT_LEFT;
			}

			pDC->DrawText (strText, rcText, nFormat);
		}
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnEraseTabsButton (CDC* pDC, CRect rect, 
											CBCGPButton* /*pButton*/,
											CBCGPBaseTabWnd* /*pWndTab*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT uiState,
												 CBCGPBaseTabWnd* /*pWndTab*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsPressed () || (uiState & ODS_SELECTED))
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarHilite);

		rect.left += 2;
		rect.top += 2;
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarDkShadow);
	}

	rect.DeflateRect (2, 2);
}
//**********************************************************************************
COLORREF CBCGPVisualManager::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brHilite);

		const int nFrameSize = 1;

		rect.DeflateRect (1, 1);
		rect.right--;
		rect.bottom--;

		pDC->PatBlt (rect.left, rect.top + nFrameSize, nFrameSize, rect.Height (), PATINVERT);
		pDC->PatBlt (rect.left, rect.top, rect.Width (), nFrameSize, PATINVERT);
		pDC->PatBlt (rect.right, rect.top, nFrameSize, rect.Height (), PATINVERT);
		pDC->PatBlt (rect.left + nFrameSize, rect.bottom, rect.Width (), nFrameSize, PATINVERT);

		return globalData.clrTextHilite;
	}

	pDC->FillRect (rect, &globalData.brBarFace);

	return globalData.clrBarText;
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawMenuArrowOnCustomizeList (CDC* pDC, 
	CRect rectCommand, BOOL bSelected)
{
	CRect rectTriangle = rectCommand;
	rectTriangle.left = rectTriangle.right - CMenuImages::Size ().cx;

	CMenuImages::Draw (pDC, CMenuImages::IdArowRightLarge, rectTriangle,
		bSelected ? CMenuImages::ImageWhite : CMenuImages::ImageBlack);

	CRect rectLine = rectCommand;
	rectLine.right = rectTriangle.left - 1;
	rectLine.left = rectLine.right - 2;
	rectLine.DeflateRect (0, 2);

	pDC->Draw3dRect (&rectLine, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
//*********************************************************************************
CBCGPVisualManager* CBCGPVisualManager::CreateVisualManager (
					CRuntimeClass* pVisualManager)
{
	if (pVisualManager == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPVisualManager* pVisManagerOld = m_pVisManager;
	
	CObject* pObj = pVisualManager->CreateObject ();
	if (pObj == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT_VALID (pObj);
	
	if (pVisManagerOld != NULL)
	{
		ASSERT_VALID (pVisManagerOld);
		delete pVisManagerOld;
	}
	
	m_pVisManager = (CBCGPVisualManager*) pObj;
	m_pVisManager->m_bAutoDestroy = TRUE;

	return m_pVisManager;
}
//*************************************************************************************
void CBCGPVisualManager::DestroyInstance (BOOL bAutoDestroyOnly)
{
	if (m_pVisManager == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pVisManager);

	if (bAutoDestroyOnly && !m_pVisManager->m_bAutoDestroy)
	{
		return;
	}

	delete m_pVisManager;
	m_pVisManager = NULL;
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	const int iBorderSize = 2;

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
	
	rect.DeflateRect (iBorderSize, 1);

	pDC->FillSolidRect (rect, 
		bIsActive ? 
		globalData.clrActiveCaption :
		globalData.clrInactiveCaption);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawMenuResizeBar (CDC* pDC, CRect rect, int /*nResizeFlags*/)
{
	ASSERT_VALID (pDC);

	pDC->FillSolidRect (rect, globalData.clrInactiveCaption);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawMenuSystemButton (CDC* pDC, CRect rect, 
												UINT uiSystemCommand, 
												UINT nStyle, BOOL /*bHighlight*/)
{
	ASSERT_VALID (pDC);

	UINT uiState = 0;

	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		uiState |= DFCS_CAPTIONCLOSE;
		break;

	case SC_MINIMIZE:
		uiState |= DFCS_CAPTIONMIN;
		break;

	case SC_RESTORE:
		uiState |= DFCS_CAPTIONRESTORE;
		break;

	default:
		return;
	}

	if (nStyle & TBBS_PRESSED)
	{
		uiState |= DFCS_PUSHED;
	}
	
	if (nStyle & TBBS_DISABLED) // Jan Vasina: Add support for disabled buttons
	{
		uiState |= DFCS_INACTIVE;
	}

	pDC->DrawFrameControl (rect, DFC_CAPTION, uiState);
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawComboDropButton (CDC* pDC, CRect rect,
											    BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	COLORREF clrText = pDC->GetTextColor ();

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, globalData.clrBarFace, globalData.clrBarHilite);

		if (bIsDropped)
		{
			rect.OffsetRect (1, 1);
			dm.DrawRect (rect, (COLORREF)-1, globalData.clrBarShadow);
		}
		else if (bIsHighlighted)
		{
			dm.DrawRect (rect, (COLORREF)-1, globalData.clrBarShadow);
		}
	}
	else
	{
		pDC->FillRect (rect, &globalData.brBarFace);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);

		if (bIsDropped)
		{
			rect.OffsetRect (1, 1);
			pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
		}
		else if (bIsHighlighted)
		{
			pDC->Draw3dRect (&rect, globalData.clrBarHilite, globalData.clrBarShadow);
		}
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rect,
		bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);

	pDC->SetTextColor (clrText);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted || bIsDropped)
	{
		if (m_bMenuFlatLook)
		{
			CRect rectBorder = rect;
			rectBorder.DeflateRect (1, 1);

			pDC->Draw3dRect (&rectBorder, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
		}
		else
		{
			pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
		}
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* /*pBar*/,
					CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	if (!(nStyle & SBPS_NOBORDERS))
	{
		// draw the borders
		COLORREF clrHilite;
		COLORREF clrShadow;

		if (nStyle & SBPS_POPOUT)
		{
			// reverse colors
			clrHilite = globalData.clrBarShadow;
			clrShadow = globalData.clrBarHilite;
		}
		else
		{
			// normal colors
			clrHilite = globalData.clrBarHilite;
			clrShadow = globalData.clrBarShadow;
		}

		pDC->Draw3dRect (rectPane, clrShadow, clrHilite);
	}
}
//*********************************************************************************
COLORREF CBCGPVisualManager::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd, BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = TRUE;
	}

	CBrush br (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

    // get the text color
	return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);
	
	BCGP_PREDOCK_STATE preDockState = pFrameWnd->GetPreDockState ();

	if (preDockState == PDS_DOCK_REGULAR)
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, RGB (127, 0, 0), globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, RGB (127, 0, 0));
	}
	else if (preDockState == PDS_DOCK_TO_TAB)
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, RGB (0, 0, 127), globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, RGB (0, 0, 127));
	}
	else
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
	}	

	// draw the inner egde
	rectBorder.DeflateRect (rectBorderSize.right - 2, rectBorderSize.top - 2);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
	rectBorder.InflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/, 
												CRect rectBorder, CRect /*rectBorderSize*/)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
	rectBorder.DeflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
	rectBorder.DeflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//**************************************************************************************
COLORREF CBCGPVisualManager::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												  CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
		(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
	{
		if (globalData.IsHighContastMode ())
		{
			return bDisabled ? globalData.clrGrayedText : globalData.clrWindowText;
		}

		return bDisabled ? globalData.clrBtnFace : globalData.clrWindow;
	}

	return	(bDisabled ? globalData.clrGrayedText : 
			(state == ButtonsIsHighlighted) ? 
				CBCGPToolBar::GetHotTextColor () : globalData.clrBarText);
}
//***************************************************************************************
void CBCGPVisualManager::OnFillOutlookPageButton (CDC* pDC, 
												  const CRect& rect,
												  BOOL /*bIsHighlighted*/, BOOL /*bIsPressed*/,
												  COLORREF& clrText)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
	clrText = globalData.clrBarText;
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawOutlookPageButtonBorder (
	CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted && bIsPressed)
	{
		pDC->Draw3dRect (rectBtn, globalData.clrBarDkShadow, globalData.clrBarFace);
		rectBtn.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBtn, globalData.clrBarShadow, globalData.clrBarHilite);
	}
	else
	{
		if (bIsHighlighted || bIsPressed)
		{
			pDC->Draw3dRect (rectBtn, globalData.clrBarFace, globalData.clrBarDkShadow);
			rectBtn.DeflateRect (1, 1);
		}

		pDC->Draw3dRect (rectBtn, globalData.clrBarHilite, globalData.clrBarShadow);
	}

	rectBtn.DeflateRect (1, 1);
}
//**********************************************************************************
COLORREF CBCGPVisualManager::GetCaptionBarTextColor (CBCGPCaptionBar* /*pBar*/)
{
	return globalData.clrWindow;
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawCaptionBarBorder (CDC* pDC, 
	CBCGPCaptionBar* /*pBar*/, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder)
{
	ASSERT_VALID (pDC);

	if (clrBarBorder == (COLORREF) -1)
	{
		pDC->FillRect (rect, &globalData.brBarFace);
	}
	else
	{
		CBrush brBorder;
		brBorder.CreateSolidBrush (clrBarBorder);
		pDC->FillRect (rect, &brBorder);
	}

	if (!bFlatBorder)
	{
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
			BOOL bProgressText)
{
	ASSERT_VALID (pDC);

	if (nProgressTotal == 0)
	{
		return;
	}

	CRect rectComplete = rectProgress;
	rectComplete.right = rectComplete.left + 
		nProgressCurr * rectComplete.Width () / nProgressTotal;

	if (clrProgressBarDest == (COLORREF)-1)
	{
		// one-color bar
		CBrush br (clrBar);
		pDC->FillRect (rectComplete, &br);
	}
	else
	{
		// gradient bar:
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectComplete, clrBar, clrProgressBarDest, FALSE);
	}

	if (bProgressText)
	{
		CString strText;
		strText.Format (_T("%d%%"), nProgressCurr * 100 / nProgressTotal);

		COLORREF clrText = pDC->SetTextColor (globalData.clrBarText);

		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

		CRgn rgn;
		rgn.CreateRectRgnIndirect (rectComplete);
		pDC->SelectClipRgn (&rgn);

		pDC->SetTextColor (clrProgressText == (COLORREF)-1 ?
			globalData.clrTextHilite : clrProgressText);
		
		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		pDC->SelectClipRgn (NULL);
		pDC->SetTextColor (clrText);
	}
}
//****************************************************************************************
void CBCGPVisualManager::OnFillHeaderCtrlBackground (CBCGPHeaderCtrl* pCtrl,
													 CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, 
		pCtrl->IsDialogControl () ? &globalData.brBtnFace : &globalData.brBarFace);
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
		CRect& rect, BOOL bIsPressed, BOOL /*bIsHighlighted*/)
{
	ASSERT_VALID (pDC);

	if (bIsPressed)
	{
		if (pCtrl->IsDialogControl ())
		{
			pDC->Draw3dRect (rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
		}

		rect.left++;
		rect.top++;
	}
	else
	{
		if (pCtrl->IsDialogControl ())
		{
			pDC->Draw3dRect (rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
		}
	}
}
//*****************************************************************************************
void CBCGPVisualManager::OnDrawHeaderCtrlSortArrow (CBCGPHeaderCtrl* pCtrl, 
												   CDC* pDC,
												   CRect& rectArrow, BOOL bIsUp)
{
	DoDrawHeaderSortArrow (pDC, rectArrow, bIsUp, pCtrl->IsDialogControl ());
}
//*****************************************************************************************
void CBCGPVisualManager::OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* /*pStatBar*/,
			CRect rectSizeBox)
{
	ASSERT_VALID (pDC);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontMarlett);
	ASSERT (pOldFont != NULL);

	const CString strSizeBox (_T("o"));	// Char of the sizing box in "Marlett" font

	UINT nTextAlign = pDC->SetTextAlign (TA_RIGHT | TA_BOTTOM);
	COLORREF clrText = pDC->SetTextColor (globalData.clrBarShadow);

	pDC->ExtTextOut (rectSizeBox.right, rectSizeBox.bottom,
		ETO_CLIPPED, &rectSizeBox, strSizeBox, NULL);

	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrText);
	pDC->SetTextAlign (nTextAlign);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted)
	{
		pDC->DrawEdge (rect, EDGE_SUNKEN, BF_RECT);
	}
}

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectWorkArea, &globalData.brWindow);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
						BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);
	ASSERT_VALID (pGroup->m_pPage);

#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox = pGroup->m_pPage->m_pTaskPane != NULL &&
		(pGroup->m_pPage->m_pTaskPane->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)));
#else
	BOOL bIsToolBox = FALSE;
#endif

	CRect rectGroup = pGroup->m_rect;

	if (bIsToolBox)
	{
		CRect rectFill = rectGroup;
		rectFill.DeflateRect (1, 0, 1, 1);

		CBrush brFill (globalData.clrBarShadow);
		pDC->FillRect (rectFill, &brFill);

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

			pDC->FillRect (rectButton, &globalData.brBarFace);

			OnDrawExpandingBox (pDC, rectButton, !pGroup->m_bIsCollapsed, 
				globalData.clrBarText);

			rectGroup.left = rectButton.right + nBoxOffset;
			bCanCollapse = FALSE;
		}
	}
	else
	{
		// ---------------------------------
		// Draw caption background (Windows)
		// ---------------------------------
		COLORREF clrBckOld = pDC->GetBkColor ();
		pDC->FillSolidRect(rectGroup, 
			(pGroup->m_bIsSpecial ? globalData.clrHilite : globalData.clrBarFace)); 
		pDC->SetBkColor (clrBckOld);
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
		pDC->SetTextColor (globalData.clrWindow);

	}
	else
	{
		if (bCanCollapse && bIsHighlighted)
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ?
				(pGroup->m_bIsSpecial ? globalData.clrWindow : globalData.clrWindowText) :
				pGroup->m_clrTextHot);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ?
				(pGroup->m_bIsSpecial ? globalData.clrWindow : globalData.clrWindowText) :
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

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);

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
			rectButton.right - (rectButton.Height()+1)/2 - (sizeButton.cx+1)/2);
		rectButton.top = max(rectButton.top, 
			rectButton.bottom - (rectButton.Height()+1)/2 - (sizeButton.cy+1)/2);
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

			if (pGroup->m_bIsSpecial)
			{
				if (!pGroup->m_bIsCollapsed)
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowUp, rectButton.TopLeft());
				}
				else
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowDown, rectButton.TopLeft());
				}
			}
			else
			{
				if (!pGroup->m_bIsCollapsed)
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowUp, rectButton.TopLeft());
				}
				else
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowDown, rectButton.TopLeft());
				}
			}
			}
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTasksGroupIcon(CDC* pDC, CBCGPTasksGroup* pGroup,
		int nIconHOffset, BOOL /*bIsHighlighted*/, BOOL /*bIsSelected*/, BOOL /*bCanCollapse*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pGroup);

	if (pGroup->m_hIcon == NULL)
	{
		return;
	}

	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();

	CRect rectImage = pGroup->m_rect;
	rectImage.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectImage.right = rectImage.left + pGroup->m_sizeIcon.cx + nIconHOffset;

	int x = max (0, (rectImage.Width () - pGroup->m_sizeIcon.cx) / 2);
	int y = max (0, (rectImage.Height () - pGroup->m_sizeIcon.cy) / 2);

	::DrawIconEx (pDC->GetSafeHdc (),
		rectImage.left + x, rectImage.bottom - y - pGroup->m_sizeIcon.cy,
		pGroup->m_hIcon, pGroup->m_sizeIcon.cx, pGroup->m_sizeIcon.cy,
		0, NULL, DI_NORMAL);
}
//**********************************************************************************
void CBCGPVisualManager::OnFillTasksGroupInterior(CDC* /*pDC*/, CRect /*rect*/, BOOL /*bSpecial*/)
{
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial, 
												   BOOL bNoTitle)
{
	ASSERT_VALID(pDC);

	// Draw caption background:
	CPen* pPenOld = (CPen*) pDC->SelectObject (bSpecial ? &globalData.penHilite : &globalData.penBarFace);

	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.left, rect.bottom-1);
	pDC->LineTo (rect.right-1, rect.bottom-1);
	pDC->LineTo (rect.right-1, rect.top);
	if (bNoTitle)
	{
		pDC->LineTo (rect.left, rect.top);
	}
	else
	{
		pDC->LineTo (rect.right-1, rect.top-1);
	}
	pDC->SelectObject (pPenOld);
	
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
								   BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	CRect rectText = pTask->m_rect;

	if (pTask->m_bIsSeparator)
	{
		CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarFace);

		pDC->MoveTo (rectText.left, rectText.CenterPoint ().y);
		pDC->LineTo (rectText.right, rectText.CenterPoint ().y);

		pDC->SelectObject (pPenOld);
		return;
	}

	// ---------
	// Draw icon
	// ---------
	CSize sizeIcon(0, 0);
	::ImageList_GetIconSize (pIcons->m_hImageList, (int*) &sizeIcon.cx, (int*) &sizeIcon.cy);
	if (pTask->m_nIcon >= 0 && sizeIcon.cx > 0)
	{
		pIcons->Draw (pDC, pTask->m_nIcon, rectText.TopLeft (), ILD_TRANSPARENT);
	}
	int nTaskPaneOffset = pTask->m_pGroup->m_pPage->m_pTaskPane->GetTasksIconHorzOffset();
	rectText.left += sizeIcon.cx + (nTaskPaneOffset != -1 ? nTaskPaneOffset : m_nTasksIconHorzOffset);

	// ---------
	// Draw text
	// ---------
	BOOL bIsLabel = (pTask->m_uiCommandID == 0);

	CFont* pFontOld = NULL;
	COLORREF clrTextOld = pDC->GetTextColor ();
	if (bIsLabel)
	{
		pFontOld = pDC->SelectObject (
			pTask->m_bIsBold ? &globalData.fontBold : &globalData.fontRegular);
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
	}
	else if (!pTask->m_bEnabled)
	{
		pDC->SetTextColor (globalData.clrGrayedText);
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
	}
	else if (bIsHighlighted)
	{
		pFontOld = pDC->SelectObject (&globalData.fontUnderline);
		pDC->SetTextColor (pTask->m_clrTextHot == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrTextHot);
	}
	else
	{
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
	}

	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	CBCGPTasksPane* pTaskPane = pTask->m_pGroup->m_pPage->m_pTaskPane;
	ASSERT_VALID (pTaskPane);

	BOOL bMultiline = bIsLabel ? 
		pTaskPane->IsWrapLabelsEnabled () : pTaskPane->IsWrapTasksEnabled ();

	if (bMultiline)
	{
		pDC->DrawText (pTask->m_strName, rectText, DT_WORDBREAK);
	}
	else
	{
		pDC->DrawText (pTask->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	}
	
	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectImage (CPoint (0, 0), CMenuImages::Size ());

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brBarFace);

	if (bHilited)
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rect);

		pDC->Draw3dRect (rect,
			globalData.clrBarHilite,
			globalData.clrBarDkShadow);
	}

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, rect);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawToolBoxFrame (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (pDC);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}

#endif // BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManager::OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pSlider);

	CRect rectScreen = globalData.m_rectVirtual;
	pSlider->ScreenToClient (&rectScreen);

	CRect rectFill = rect;
	rectFill.left = min (rectFill.left, rectScreen.left);

	OnFillBarBackground (pDC, pSlider, rectFill, rect);

	if (bAutoHideMode)
	{
		// draw outer edge;

		DWORD dwAlgn = pSlider->GetCurrentAlignment ();
		CRect rectBorder = rect;

		COLORREF clrBorder = globalData.clrBarDkShadow;
		
		if (dwAlgn & CBRS_ALIGN_LEFT)
		{
			rectBorder.left = rectBorder.right;
		}
		else if (dwAlgn & CBRS_ALIGN_RIGHT)
		{
			rectBorder.right = rectBorder.left;
			clrBorder = globalData.clrBarHilite;
		}
		else if (dwAlgn & CBRS_ALIGN_TOP)
		{
			rectBorder.top = rectBorder.bottom;
		}
		else if (dwAlgn & CBRS_ALIGN_BOTTOM)
		{
			rectBorder.bottom = rectBorder.top;
			clrBorder = globalData.clrBarHilite;
		}
		else
		{
			ASSERT(FALSE);
			return;
		}

		pDC->Draw3dRect (rectBorder, clrBorder, clrBorder);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarShadow);
}
//********************************************************************************
void CBCGPVisualManager::OnFillSplitterBackground (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);
	pDC->FillSolidRect (rect, globalData.clrBarFace);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawCheckBox (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 BOOL bChecked,
										 BOOL bEnabled)
{
	OnDrawCheckBoxEx (pDC, rect, bChecked ? 1 : 0, bHighlighted, FALSE, bEnabled);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawCheckBoxEx (CDC *pDC, CRect rect, 
											int nState,
											BOOL bHighlighted, 
											BOOL /*bPressed*/,
											BOOL bEnabled)
{
	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);

		rect.DeflateRect (1, 1);

		dm.DrawRect (rect, 
			bEnabled ? globalData.clrWindow : globalData.clrBarFace, 
			globalData.clrBarShadow);

		if (nState == 1)
		{
			CMenuImages::Draw (pDC, CMenuImages::IdCheck, rect,
								CMenuImages::ImageBlack);
		}

		return;
	}

	if (bHighlighted)
	{
		pDC->DrawFocusRect (rect);
	}

	rect.DeflateRect (1, 1);
	pDC->FillSolidRect (&rect, bEnabled ? globalData.clrWindow :
		globalData.clrBarFace);

	pDC->Draw3dRect (&rect, 
					globalData.clrBarDkShadow,
					globalData.clrBarHilite);
	
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (&rect,
					globalData.clrBarShadow,
					globalData.clrBarLight);

	if (nState == 1)
	{
		CMenuImages::Draw (pDC, CMenuImages::IdCheck, rect,
							CMenuImages::ImageBlack);
	}
	else if (nState == 2)
	{
		rect.DeflateRect (1, 1);
		
		CBrush br;
		br.CreateHatchBrush (HS_DIAGCROSS, globalData.clrBtnText);

		pDC->FillRect (rect, &br);
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawSpinButtons (CDC* pDC, CRect rectSpin, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	ASSERT_VALID (pSpinCtrl);

	rectSpin.DeflateRect (1, 1);

	CRect rect[2];

	rect[0] = rect[1] = rectSpin;

	if (!bOrientation)
	{
		rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
		rect[1].top = rect[0].bottom + 1;
	}
	else
	{
		rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
		rect[1].left = rect[0].right + 1;
	}

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rectSpin, globalData.clrBarFace, globalData.clrBarHilite);
	}
	else
	{
		pDC->FillRect (rectSpin, &globalData.brBarFace);
		pDC->Draw3dRect (rectSpin, globalData.clrBarHilite, globalData.clrBarHilite);
	}

	CMenuImages::IMAGES_IDS id[2][2] = {{CMenuImages::IdArowUp, CMenuImages::IdArowDown}, {CMenuImages::IdArowLeft, CMenuImages::IdArowRight}};

	int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
	BOOL bDisabled = nState & SPIN_DISABLED;

	for (int i = 0; i < 2; i ++)
	{
		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);

			if (idxPressed == i)
			{
				dm.DrawRect (rect[i], (COLORREF)-1, globalData.clrBarShadow);
			}
			else
			{
				dm.DrawRect (rect[i], (COLORREF)-1, globalData.clrBarHilite);
			}
		}
		else
		{
			if (idxPressed == i)
			{
				pDC->Draw3dRect (&rect[i], globalData.clrBarShadow, globalData.clrBarHilite);
			}
			else
			{
				pDC->Draw3dRect (&rect[i], globalData.clrBarHilite, globalData.clrBarShadow);
			}
		}

		CMenuImages::Draw (pDC, id[bOrientation][i], rect[i],
			bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
	}
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened,
											COLORREF colorBox)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, colorBox, colorBox);

	rect.DeflateRect (2, 2);

	CPen penLine (PS_SOLID, 1, globalData.clrBarText);
	CPen* pOldPen = pDC->SelectObject (&penLine);

	CPoint ptCenter = rect.CenterPoint ();

	pDC->MoveTo (rect.left, ptCenter.y);
	pDC->LineTo (rect.right, ptCenter.y);

	if (!bIsOpened)
	{
		pDC->MoveTo (ptCenter.x, rect.top);
		pDC->LineTo (ptCenter.x, rect.bottom);
	}

	pDC->SelectObject (pOldPen);
}
//***********************************************************************************
COLORREF CBCGPVisualManager::OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBrush br (globalData.clrInactiveCaption);
	pDC->FillRect (rect, &br);

	pDC->Draw3dRect (rect, globalData.clrInactiveBorder, globalData.clrInactiveBorder);
	return globalData.clrInactiveCaptionText;
}
//*************************************************************************************
void CBCGPVisualManager::GetCalendarColors (const CBCGPCalendar* /*pCalendar*/,
				   CBCGPCalendarColors& colors)
{
	colors.clrCaption = globalData.clrBtnFace;
	colors.clrCaptionText = globalData.clrBtnText;
	colors.clrSelected = globalData.clrBtnFace;
	colors.clrSelectedText = globalData.clrBtnText;
	colors.clrTodayBorder = RGB (187, 85, 3);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawControlBorder (CWnd* pWndCtrl)
{
	ASSERT_VALID (pWndCtrl);

	CWindowDC dc (pWndCtrl);

	CRect rect;
	pWndCtrl->GetWindowRect (rect);

	rect.bottom -= rect.top;
	rect.right -= rect.left;
	rect.left = rect.top = 0;

	if (pWndCtrl->GetStyle () & WS_POPUP)
	{
		dc.Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	}
	else
	{
		dc.Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarHilite);
	}

	rect.DeflateRect (1, 1);
	dc.Draw3dRect (rect, globalData.clrWindow, globalData.clrWindow);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawShowAllMenuItems (CDC* pDC, CRect rect, 
												 CBCGPVisualManager::BCGBUTTON_STATE /*state*/)
{
	ASSERT_VALID (pDC);
	CMenuImages::Draw (pDC, CMenuImages::IdArowShowAll, rect);
}
//************************************************************************************
int CBCGPVisualManager::GetShowAllMenuItemsHeight (CDC* /*pDC*/, const CSize& /*sizeDefault*/)
{
	return CMenuImages::Size ().cy + 2 * TEXT_MARGIN;
}
//**********************************************************************************
void CBCGPVisualManager::GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack)
{
	ASSERT_VALID (pTabWnd);

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (pTabWnd->IsOneNoteStyle () && clrActiveTab != (COLORREF)-1)
	{
		clrFace = clrActiveTab;
	}
	else if (pTabWnd->IsDialogControl ())
	{
		clrFace = globalData.clrBtnFace;
	}
	else
	{
		clrFace = globalData.clrBarFace;
	}

	if (pTabWnd->IsDialogControl ())
	{
		clrDark = globalData.clrBtnShadow;
		clrBlack = globalData.clrBtnText;
		clrHighlight = pTabWnd->IsVS2005Style () ? globalData.clrBtnShadow : globalData.clrBtnHilite;
		clrDarkShadow = globalData.clrBtnDkShadow;
		clrLight = globalData.clrBtnLight;

		pbrFace = &globalData.brBtnFace;
	}
	else
	{
		clrDark = globalData.clrBarShadow;
		clrBlack = globalData.clrBarText;
		clrHighlight = pTabWnd->IsVS2005Style () ? globalData.clrBarShadow : globalData.clrBarHilite;
		clrDarkShadow = globalData.clrBarDkShadow;
		clrLight = globalData.clrBarLight;

		pbrFace = &globalData.brBarFace;
	}

	pbrBlack = &globalData.brBlack;
}
//*********************************************************************************
void CBCGPVisualManager::OnFillAutoHideButtonBackground (CDC* pDC, CRect rect, CBCGPAutoHideButton* /*pButton*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

	COLORREF clr = globalData.clrBarShadow;

	COLORREF clrText = pDC->GetTextColor ();

	if (rectBorderSize.left > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.top, 
							rectBounds.left + rectBorderSize.left, 
							rectBounds.bottom, clr);
	}
	if (rectBorderSize.top > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.top, 
							rectBounds.right, 
							rectBounds.top + rectBorderSize.top, clr);
	}
	if (rectBorderSize.right > 0)
	{
		pDC->FillSolidRect (rectBounds.right - rectBorderSize.right, rectBounds.top, 
							rectBounds.right, 
							rectBounds.bottom, clr);
	}
	if (rectBorderSize.bottom > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.bottom - rectBorderSize.bottom,  
							rectBounds.right, 
							rectBounds.bottom, clr);
	}

	pDC->SetTextColor (clrText);
}
//*******************************************************************************
COLORREF CBCGPVisualManager::GetAutoHideButtonTextColor (CBCGPAutoHideButton* /*pButton*/)
{
	return globalData.clrBarText;
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectSplitter, &globalData.brBarFace);
	pDC->Draw3dRect (rectSplitter, globalData.clrBarHilite, globalData.clrBarShadow);
}
//********************************************************************************
void CBCGPVisualManager::OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, 
												  COLORREF& clrText)
{

	pDC->FillSolidRect	(rectCaption, globalData.clrBarShadow);
	clrText = globalData.clrBarHilite;
}
//********************************************************************************
BOOL CBCGPVisualManager::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* pButton, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &globalData.brLight);
		pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &globalData.brLight);

	default:
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManager::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);

	return TRUE;
}
//*********************************************************************************
BOOL CBCGPVisualManager::OnDrawBrowseButton (CDC* pDC, CRect rect, 
											 CBCGPEdit* /*pEdit*/, 
											 CBCGPVisualManager::BCGBUTTON_STATE state,
											 COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (&rect, &globalData.brBtnFace);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	pDC->Draw3dRect (rectFrame, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);

	rectFrame.DeflateRect (1, 1);
	pDC->DrawEdge (rectFrame, state == ButtonsIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);

	return TRUE;
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawAppBarBorder (CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
									CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&globalData.brBtnFace);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brLight);

	pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawAppBarCaption (CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/, 
											CRect rectCaption, CString strCaption)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectCaption, &globalData.brBarFace);

	// Paint caption text:
	int nOldMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->SetTextColor (globalData.clrBarText);
	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
	ASSERT_VALID (pOldFont);

	CRect rectText = rectCaption;
	rectText.DeflateRect (2, 0);
	pDC->DrawText (strCaption, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
	pDC->SetBkMode (nOldMode);
	pDC->SetTextColor (clrOldText);
}
//******************************************************************************
void CBCGPVisualManager::GetSmartDockingBaseMarkerColors (
		COLORREF& clrBaseGroupBackground,
		COLORREF& clrBaseGroupBorder)
{
	clrBaseGroupBackground = globalData.clrBarFace;
	clrBaseGroupBorder = globalData.clrBarShadow;
}
//******************************************************************************
COLORREF CBCGPVisualManager::GetSmartDockingMarkerToneColor ()
{
	return globalData.clrActiveCaption;
}

#ifndef BCGP_EXCLUDE_TOOLBOX

BOOL CBCGPVisualManager::OnEraseToolBoxButton (CDC* pDC, CRect rect, 
											CBCGPToolBoxButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->GetCheck ())
	{
		rect.DeflateRect (1, 1);
		CBCGPToolBarImages::FillDitheredRect (pDC, rect);
	}

	return TRUE;
}
//**********************************************************************************
BOOL CBCGPVisualManager::OnDrawToolBoxButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPToolBoxButton* pButton, UINT /*uiState*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->GetCheck ())
	{
		pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
	}
	else if (pButton->IsHighlighted ())
	{
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}

	return TRUE;
}

#endif // BCGP_EXCLUDE_TOOLBOX

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManager::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBarLight, globalData.clrBarDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
}
//**********************************************************************************
COLORREF  CBCGPVisualManager::OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* /*pPopupWnd*/)
{
	ASSERT_VALID (pDC);

	BOOL bActive = TRUE;	// TODO

	CBrush br (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

    // get the text color
	return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//**********************************************************************************
void CBCGPVisualManager::OnErasePopupWindowButton (CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsCaptionButton ())
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
		return;
	}

	CRect rectParent;
	pButton->GetParent ()->GetClientRect (rectParent);

	pButton->GetParent ()->MapWindowPoints (pButton, rectParent);
	OnFillPopupWindowBackground (pDC, rectParent);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
	if (pButton->IsPressed ())
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarLight);
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarLight, globalData.clrBarDkShadow);
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW

#ifndef BCGP_EXCLUDE_PLANNER

COLORREF CBCGPVisualManager::OnFillPlannerCaption (CDC* pDC,
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected,
		BOOL bNoBorder/* = FALSE*/)
{
	ASSERT_VALID (pDC);

	COLORREF clrText = globalData.clrBtnText;

	const BOOL bMonth = DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL;
	if (bMonth && !bIsToday && !bIsSelected)
	{
		return clrText;
	}

	rect.DeflateRect (1, 1);

	if (bIsToday)
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
		CBCGPToolBarImages::FillDitheredRect (pDC, rect);
	}
	else if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brHilite);
		clrText = globalData.clrTextHilite;
	}
	else
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
	}

	if (bIsToday || !bNoBorder)
	{
		CPen pen (PS_SOLID, 1, bIsToday ? globalData.clrBarShadow : globalData.clrBtnShadow);
		CPen* pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rect.left, rect.bottom);
		pDC->LineTo (rect.right, rect.bottom);

		pDC->SelectObject (pOldPen);
	}

	return clrText;
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawPlannerCaptionText (CDC* pDC, 
		CBCGPPlannerView* /*pView*/, CRect rect, const CString& strText, 
		COLORREF clrText, int nAlign, BOOL bHighlight)
{
	const int nTextMargin = 2;

	if (bHighlight)
	{
		const int nTextLen = min (rect.Width (), 
			pDC->GetTextExtent (strText).cx + 2 * nTextMargin);

		CRect rectHighlight = rect;
		rectHighlight.DeflateRect (1, 1);

		switch (nAlign)
		{
		case DT_LEFT:
			rectHighlight.right = rectHighlight.left + nTextLen;
			break;

		case DT_RIGHT:
			rectHighlight.left = rectHighlight.right - nTextLen;
			break;

		case DT_CENTER:
			rectHighlight.left = rectHighlight.CenterPoint ().x - nTextLen / 2;
			rectHighlight.right = rectHighlight.left + nTextLen;
			break;
		}

		pDC->FillRect (rectHighlight, &globalData.brHilite);

		clrText = globalData.clrTextHilite;
	}

	rect.DeflateRect (nTextMargin, 0);

	COLORREF clrOld = pDC->SetTextColor (clrText);
	pDC->DrawText (strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);

	pDC->SetTextColor (clrOld);
}
void CBCGPVisualManager::GetPlannerAppointmentColors (CBCGPPlannerView* pView,
		BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags, 
		COLORREF& clrBack1, COLORREF& clrBack2,
		COLORREF& clrFrame1, COLORREF& clrFrame2, COLORREF& clrText)
{
	ASSERT_VALID (pView);

	const BOOL bIsGradientFill = 
		dwDrawFlags & BCGP_PLANNER_DRAW_APP_GRADIENT_FILL &&
		globalData.m_nBitsPerPixel > 8 && 
		!(globalData.m_bIsBlackHighContrast || globalData.m_bIsWhiteHighContrast);
	const BOOL bIsOverrideSelection = dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION;

	const BOOL bSelect = bSelected && !bIsOverrideSelection;

	const BOOL bDayView = pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewDay));

	if (bSelect)
	{
		if (bDayView)
		{
			if (!bSimple)
			{
				clrBack1 = globalData.clrBtnFace;
				clrText  = globalData.clrBtnText;
			}
		}
		else
		{
			clrBack1 = globalData.clrHilite;
			clrText  = globalData.clrTextHilite;
		}
	}

	BOOL bDefault = FALSE;

	if (clrBack1 == CLR_DEFAULT)
	{
		if (bIsGradientFill)
		{
			clrBack1 = pView->GetPlanner ()->GetBackgroundColor ();
			if (clrBack1 == CLR_DEFAULT)
			{
				bDefault = TRUE;
				clrBack1 = m_clrPlannerWork;
			}
		}
		else
		{
			clrBack1 = (bDayView || !bSimple)
						? globalData.clrWindow
						: CLR_DEFAULT;
		}
	}

	clrBack2 = clrBack1;
	
	if (clrText == CLR_DEFAULT)
	{
		clrText = globalData.clrWindowText;
	}
	
	clrFrame1 = globalData.clrWindowFrame;
	clrFrame2 = clrFrame1;

	if (bIsGradientFill && clrBack1 != CLR_DEFAULT)
	{
		if (!bSelect)
		{
			if (bDefault)
			{
				clrBack2 = RGB (255, 255, 255);
			}
			else
			{
				clrBack2 = CBCGPDrawManager::PixelAlpha (clrBack1, RGB (255, 255, 255), 25);
			}
		}

		clrFrame1 = CBCGPDrawManager::PixelAlpha (clrBack1, RGB (0, 0, 0), 75);

		if (!bSelected)
		{
			clrFrame2 = clrFrame1;
		}
	}
}
//*******************************************************************************
COLORREF CBCGPVisualManager::GetPlannerHourLineColor (CBCGPPlannerView* /*pView*/,
		BOOL /*bWorkingHours*/, BOOL /*bHour*/)
{
	return m_clrPalennerLine;
}
//*******************************************************************************
COLORREF CBCGPVisualManager::GetPlannerSelectionColor (CBCGPPlannerView* /*pView*/)
{
	return globalData.clrHilite;
}
//*******************************************************************************
COLORREF CBCGPVisualManager::GetPlannerSeparatorColor (CBCGPPlannerView* /*pView*/)
{
	return globalData.clrWindowFrame;
}
//*******************************************************************************
void CBCGPVisualManager::OnFillPlanner (CDC* pDC, CBCGPPlannerView* pView, 
		CRect rect, BOOL bWorkingArea)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pView);

	if (m_brPlanner.GetSafeHandle () == NULL)
	{
		WORD HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };

		CBitmap bmp;
		bmp.CreateBitmap (8, 8, 1, 1, HatchBits);

		CBrush brLight;
		m_brPlanner.CreatePatternBrush (&bmp);
	}

	COLORREF colorFill = globalData.IsHighContastMode () ?
		CLR_DEFAULT : pView->GetPlanner ()->GetBackgroundColor ();
	BOOL bIsDefaultColor = colorFill == CLR_DEFAULT;

	if (bIsDefaultColor)
	{
		colorFill = m_clrPlannerWork;	// Use default color
	}
		
	switch (pView->GetPlanner ()->GetType ())
	{
	case CBCGPPlannerManagerCtrl::BCGP_PLANNER_TYPE_DAY:
	case CBCGPPlannerManagerCtrl::BCGP_PLANNER_TYPE_WORK_WEEK:
		{
			COLORREF clrTextOld = pDC->SetTextColor (colorFill);

			COLORREF clrBkOld = pDC->SetBkColor (bWorkingArea ? 
				RGB (255, 255, 255) : RGB (128, 128, 128));

			pDC->FillRect (rect, &m_brPlanner);

			pDC->SetTextColor (clrTextOld);
			pDC->SetBkColor (clrBkOld);
		}
		break;

	default:
		if (bIsDefaultColor || !bWorkingArea)
		{
			pDC->FillRect (rect, bWorkingArea ? 
				&globalData.brWindow : &globalData.brBtnFace);
		}
		else
		{
			CBrush br (colorFill);
			pDC->FillRect (rect, &br);
		}
	}
}
//*******************************************************************************
COLORREF CBCGPVisualManager::OnFillPlannerTimeBar (CDC* pDC, CBCGPPlannerView* /*pView*/, 
											   CRect rect, COLORREF& clrLine)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);

	clrLine = globalData.clrBtnShadow;

	return globalData.clrBtnText;
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawPlannerTimeLine (CDC* pDC, CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brActiveCaption);

	CPen* pOldPen = pDC->SelectObject (&globalData.penBarShadow);

	pDC->MoveTo (rect.left, rect.bottom);
	pDC->LineTo (rect.right, rect.bottom);

	pDC->SelectObject (pOldPen);
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawPlannerHeader (CDC* pDC, 
	CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);
	pDC->Draw3dRect (rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawPlannerHeaderPane (CDC* pDC, 
	CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->Draw3dRect (rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
//*******************************************************************************
void CBCGPVisualManager::OnFillPlannerHeaderAllDay (CDC* pDC, 
		CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);
	
	CBrush br (globalData.clrBtnShadow);
	pDC->FillRect (rect, &br);
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawPlannerHeaderAllDayItem (CDC* pDC, 
		CBCGPPlannerView* /*pView*/, CRect rect, BOOL /*bIsToday*/, BOOL bIsSelected)
{
	if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brWindow);
	}
}
//*******************************************************************************
void CBCGPVisualManager::PreparePlannerBackItem (BOOL /*bIsToday*/, BOOL /*bIsSelected*/)
{
	m_bPlannerBackItemToday    = FALSE;
	m_bPlannerBackItemSelected = FALSE;
}
//*******************************************************************************
void CBCGPVisualManager::PreparePlannerCaptionBackItem (BOOL bIsHeader)
{
	m_bPlannerCaptionBackItemHeader = bIsHeader;
}
#endif // BCGP_EXCLUDE_PLANNER

//********************************************************************************
void CBCGPVisualManager::DoDrawHeaderSortArrow (CDC* pDC, CRect rectArrow, BOOL bIsUp, BOOL bDlgCtrl)
{
	CPen penLight (1, PS_SOLID, 
		bDlgCtrl ? globalData.clrBtnHilite : globalData.clrBarHilite);
	CPen penDark (1, PS_SOLID, 
		bDlgCtrl ? globalData.clrBtnDkShadow : globalData.clrBarDkShadow);

	CPen* pPenOld = pDC->SelectObject (&penLight);
	ASSERT_VALID (pPenOld);

	if (!bIsUp)
	{
		pDC->MoveTo (rectArrow.right, rectArrow.top);
		pDC->LineTo (rectArrow.CenterPoint ().x, rectArrow.bottom);

		pDC->SelectObject (&penDark);
		pDC->LineTo (rectArrow.left, rectArrow.top);
		pDC->LineTo (rectArrow.right, rectArrow.top);
	}
	else
	{
		pDC->MoveTo (rectArrow.left, rectArrow.bottom);
		pDC->LineTo (rectArrow.right, rectArrow.bottom);
		pDC->LineTo (rectArrow.CenterPoint ().x, rectArrow.top);

		pDC->SelectObject (&penDark);
		pDC->LineTo (rectArrow.left, rectArrow.bottom);
	}

	pDC->SelectObject (pPenOld);
}

#ifndef BCGP_EXCLUDE_GRID_CTRL

void CBCGPVisualManager::OnDrawGridSortArrow (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC,
											  CRect rectArrow, BOOL bIsUp)
{
	DoDrawHeaderSortArrow (pDC, rectArrow, bIsUp, TRUE/*!pCtrl->IsControlBarColors ()*/);
}
//********************************************************************************
COLORREF CBCGPVisualManager::OnFillGridGroupByBoxBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBrush br (globalData.clrBtnShadow);
	pDC->FillRect (rect, &br);

	return globalData.clrBarText;
}
//********************************************************************************
COLORREF CBCGPVisualManager::GetGridGroupByBoxLineColor () const
{
	return globalData.clrBarText;
}
//********************************************************************************
void CBCGPVisualManager::OnDrawGridGroupByBoxItemBorder (CBCGPGridCtrl* /*pCtrl*/,
														 CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBtnFace);

	pDC->Draw3dRect (rect, globalData.clrBarWindow, globalData.clrBtnText);
	rect.DeflateRect (0, 0, 1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarWindow, globalData.clrBtnShadow);
}
//********************************************************************************
COLORREF CBCGPVisualManager::GetGridLeftOffsetColor (CBCGPGridCtrl* pCtrl)
{
	ASSERT_VALID (pCtrl);

	COLORREF clrGray = (COLORREF)-1;

	if (globalData.m_nBitsPerPixel <= 8)
	{
		clrGray = pCtrl->IsControlBarColors () ?
			globalData.clrBarShadow : globalData.clrBtnShadow;
	}
	else
	{
		clrGray = CBCGPDrawManager::PixelAlpha (
			pCtrl->IsControlBarColors () ? globalData.clrBarFace : globalData.clrBtnFace, 94);
	}

	return clrGray;
}
//********************************************************************************
COLORREF CBCGPVisualManager::GetGridItemSortedColor (CBCGPGridCtrl* pCtrl)
{
	ASSERT_VALID (pCtrl);

	COLORREF clrSortedColumn = (COLORREF)-1;
	if (globalData.m_nBitsPerPixel <= 8)
	{
		clrSortedColumn = pCtrl->GetBkColor ();
	}
	else
	{
		clrSortedColumn = CBCGPDrawManager::PixelAlpha (
				pCtrl->GetBkColor (), .97, .97, .97);
	}

	return clrSortedColumn;
}
//********************************************************************************
void CBCGPVisualManager::OnFillGridGroupBackground (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, 
												   CRect rectFill)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rectFill, &globalData.brWindow);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawGridGroupUnderline (CBCGPGridCtrl* pCtrl, CDC* pDC, 
												   CRect rectFill)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCtrl);

	COLORREF clrOld = pDC->GetBkColor ();
	pDC->FillSolidRect (rectFill, 
		pCtrl->IsControlBarColors () ? globalData.clrBarShadow : globalData.clrBtnShadow);
	pDC->SetBkColor (clrOld);
}
//********************************************************************************
COLORREF CBCGPVisualManager::OnFillGridRowBackground (CBCGPGridCtrl* pCtrl, 
												  CDC* pDC, CRect rectFill, BOOL bSelected)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCtrl);

	// Fill area:
	if (!pCtrl->IsFocused ())
	{
		pDC->FillRect (rectFill, 
			pCtrl->IsControlBarColors () ? &globalData.brBarFace : &globalData.brBtnFace);
	}
	else
	{
		pDC->FillRect (rectFill, &globalData.brHilite);
	}

	// Return text color:
	if (!pCtrl->IsHighlightGroups () && bSelected)
	{
		return (!pCtrl->IsFocused ()) ? globalData.clrHotText : globalData.clrTextHilite;
	}

	return pCtrl->IsHighlightGroups () ? 
		(pCtrl->IsControlBarColors () ? globalData.clrBarShadow : globalData.clrBtnShadow) :
		globalData.clrWindowText;
}
//********************************************************************************
COLORREF CBCGPVisualManager::OnFillGridItem (CBCGPGridCtrl* pCtrl, 
											CDC* pDC, CRect rectFill,
											BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCtrl);

	// Fill area:
	if (bSelected && !bActiveItem)
	{
		if (!pCtrl->IsFocused ())
		{
			pDC->FillRect (rectFill,
				pCtrl->IsControlBarColors () ? &globalData.brBarFace : &globalData.brBtnFace);
			return globalData.clrBtnText;
		}
		else
		{
			pDC->FillRect (rectFill, &globalData.brHilite);
			return globalData.clrTextHilite;
		}
	}
	else
	{
		if (bActiveItem)
		{
			pDC->FillRect (rectFill, &globalData.brWindow);
		}
		else if (bSortedColumn)
		{
			CBrush br (pCtrl->GetSortedColor ());
			pDC->FillRect (rectFill, &br);
		}
		else
		{
			// no painting
		}
	}

	return (COLORREF)-1;
}
//********************************************************************************
COLORREF CBCGPVisualManager::OnFillReportCtrlRowBackground (CBCGPGridCtrl* pCtrl, 
												  CDC* pDC, CRect rectFill,
												  BOOL bSelected, BOOL bGroup)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCtrl);

	// Fill area:
	COLORREF clrText = (COLORREF)-1;
	if (bSelected)
	{
		if (!pCtrl->IsFocused ())
		{
			pDC->FillRect (rectFill, 
				pCtrl->IsControlBarColors () ? &globalData.brBarFace : &globalData.brBtnFace);
			clrText = globalData.clrHilite;
		}
		else
		{
			pDC->FillRect (rectFill, &globalData.brHilite);
			clrText = globalData.clrTextHilite;
		}
	}
	else
	{
		if (bGroup)
		{
			// no painting
			clrText = globalData.clrHilite;
		}
	}

	// Return text color:
	return clrText;
}
//********************************************************************************
COLORREF CBCGPVisualManager::GetReportCtrlGroupBackgoundColor ()
{
	return globalData.clrBtnLight;
}
//********************************************************************************
void CBCGPVisualManager::OnFillGridBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brWindow);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawGridExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox)
{
	OnDrawExpandingBox (pDC, rect, bIsOpened, colorBox);
}
//********************************************************************************
void CBCGPVisualManager::OnFillGridHeaderBackground (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);
}
//********************************************************************************
BOOL CBCGPVisualManager::OnDrawGridHeaderItemBorder (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, BOOL /*bPressed*/)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
	return TRUE;
}

#endif // BCGP_EXCLUDE_GRID_CTRL

#ifndef BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManager::GetPropListGroupColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	return pPropList->DrawControlBarColors () ?
		globalData.clrBarFace : globalData.clrBtnFace;
}
//********************************************************************************
COLORREF CBCGPVisualManager::GetPropListGroupTextColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	return pPropList->DrawControlBarColors () ?
		globalData.clrBarDkShadow : globalData.clrBtnDkShadow;
}

#endif // BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManager::GetMenuItemTextColor (
	CBCGPToolbarMenuButton* /*pButton*/, BOOL bHighlighted, BOOL bDisabled)
{
	if (bHighlighted)
	{
		return bDisabled ? globalData.clrBtnFace : globalData.clrTextHilite;
	}

	return bDisabled ? globalData.clrGrayedText : globalData.clrWindowText;
}
//*****************************************************************************
COLORREF CBCGPVisualManager::GetStatusBarPaneTextColor (CBCGPStatusBar* /*pStatusBar*/, 
									CBCGStatusBarPaneInfo* pPane)
{
	ASSERT (pPane != NULL);

	return (pPane->nStyle & SBPS_DISABLED) ? globalData.clrGrayedText : 
			pPane->clrText == (COLORREF)-1 ? globalData.clrBtnText : pPane->clrText;
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManager::OnDrawRibbonCaption (CDC* pDC, CBCGPRibbonBar* pBar,
											  CRect rect, CRect rectText)
{
	ASSERT_VALID (pBar);

	CWnd* pWnd = pBar->GetParent ();
	ASSERT_VALID (pWnd);

	const BOOL bGlass	   = pBar->IsTransparentCaption ();
	const DWORD dwStyleEx  = pWnd->GetExStyle ();
	const BOOL bIsRTL      = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
	BOOL bTextCenter = TRUE;

	ASSERT_VALID (pDC);

	if ((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ALL) == BCGPRIBBONBAR_HIDE_ALL)
	{
		HICON hIcon = globalUtils.GetWndIcon (pWnd);

		if (hIcon != NULL)
		{
			CSize szIcon (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON));

			CRect rectIcon (rect.TopLeft (), 
				CSize (min(::GetSystemMetrics (SM_CYCAPTION), rect.Height ()), rect.Height ()));

			long x = rect.left + max (0, (rectIcon.Width ()  - szIcon.cx) / 2);
			long y = rect.top  + max (0, (rectIcon.Height () - szIcon.cy) / 2);

			::DrawIconEx (pDC->GetSafeHdc (), x, y, hIcon, szIcon.cx, szIcon.cy,
				0, NULL, DI_NORMAL);

			if (rectText.left < rectIcon.right)
			{
				rectText.left = rectIcon.right;
			}
		}

		bTextCenter = TRUE;
	}


	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
	ASSERT (pOldFont != NULL);
	
	int nOldMode = pDC->SetBkMode    (TRANSPARENT);

	CString strCaption;
	pWnd->GetWindowText (strCaption);

	DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
		(bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);

	COLORREF clrText = RGB (0, 0, 0);

	int widthFull = rectText.Width ();
	int width = pDC->GetTextExtent (strCaption).cx;

	if (bTextCenter && width < widthFull)
	{
		rectText.left += (widthFull - width) / 2;
	}

	rectText.right = min (rectText.left + width, rectText.right);

	if (rectText.right > rectText.left)
	{
		if (bGlass)
		{
			DrawTextOnGlass (pDC, strCaption, rectText, dwTextStyle, 10);
		}
		else
		{
			COLORREF clrOldText = pDC->SetTextColor (clrText);
			pDC->DrawText (strCaption, rectText, dwTextStyle);
			pDC->SetTextColor (clrOldText);
		}
	}

	pDC->SetBkMode    (nOldMode);
	pDC->SelectObject (pOldFont);
}
//*****************************************************************************
void CBCGPVisualManager::OnDrawRibbonCaptionButton (
					CDC* pDC, CBCGPRibbonCaptionButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	OnFillRibbonButton (pDC, pButton);

	CMenuImages::IMAGES_IDS imageID;

	switch (pButton->GetID ())
	{
	case SC_CLOSE:
		imageID = CMenuImages::IdClose;
		break;

	case SC_MINIMIZE:
		imageID = CMenuImages::IdMinimize;
		break;

	case SC_MAXIMIZE:
		imageID = CMenuImages::IdMaximize;
		break;

	case SC_RESTORE:
		imageID = CMenuImages::IdRestore;
		break;

	default:
		return;
	}

	CMenuImages::Draw (pDC, imageID, pButton->GetRect (),
		pButton->IsDisabled () ? 
		CMenuImages::ImageGray : CMenuImages::ImageBlack);

	OnDrawRibbonButtonBorder (pDC, pButton);
}
//*******************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonButtonsGroup (
					CDC* /*pDC*/, CBCGPRibbonButtonsGroup* /*pGroup*/,
					CRect /*rectGroup*/)
{
	return (COLORREF)-1;
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawDefaultRibbonImage (
					CDC* pDC, CRect rectImage,
					BOOL bIsDisabled,
					BOOL /*bIsPressed*/,
					BOOL /*bIsHighlighted*/)
{
	ASSERT_VALID (pDC);

	CRect rectBullet (rectImage.CenterPoint (), CSize (1, 1));
	rectBullet.InflateRect (5, 5);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		globalData.bIsWindows9x)
	{
		CBrush br (bIsDisabled ? globalData.clrGrayedText : RGB (0, 127, 0));

		CBrush* pOldBrush = (CBrush*) pDC->SelectObject (&br);
		CPen* pOldPen = (CPen*) pDC->SelectStockObject (NULL_PEN);

		pDC->Ellipse (rectBullet);

		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldPen);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		dm.DrawEllipse (rectBullet,
			bIsDisabled ? globalData.clrGrayedText : RGB (160, 208, 128),
			bIsDisabled ? globalData.clrBtnShadow : RGB (71, 117, 44));
	}
}
//*******************************************************************************
void CBCGPVisualManager::OnDrawRibbonMainButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	const BOOL bIsHighlighted = pButton->IsHighlighted ();
	const BOOL bIsPressed = pButton->IsPressed () || pButton->IsDroppedDown ();

	CRect rect = pButton->GetRect ();
	rect.DeflateRect (2, 2);

	CBCGPDrawManager dm (*pDC);

	dm.DrawEllipse (rect,
		bIsPressed ? globalData.clrBarLight : globalData.clrBarFace,
		bIsHighlighted ? globalData.clrBarDkShadow : globalData.clrBarShadow);
}
//*****************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonTabsFrame (
					CDC* pDC, 
					CBCGPRibbonBar* /*pWndRibbonBar*/, 
					CRect rectTab)
{
	ASSERT_VALID (pDC);

	CPen pen (PS_SOLID, 1, globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rectTab.left, rectTab.top);
	pDC->LineTo (rectTab.right, rectTab.top);

	pDC->SelectObject (pOldPen);

	return (COLORREF)-1;
}
//*****************************************************************************
void CBCGPVisualManager::OnDrawRibbonCategory (
		CDC* pDC, 
		CBCGPRibbonCategory* pCategory,
		CRect rectCategory)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCategory);

	const int nShadowSize = 2;

	rectCategory.right -= nShadowSize;
	rectCategory.bottom -= nShadowSize;

	pDC->FillRect (rectCategory, &globalData.brBarFace);

	CRect rectActiveTab = pCategory->GetTabRect ();

	CPen pen (PS_SOLID, 1, globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rectCategory.left, rectCategory.top);
	pDC->LineTo (rectActiveTab.left + 1, rectCategory.top);

	pDC->MoveTo (rectActiveTab.right - 2, rectCategory.top);
	pDC->LineTo (rectCategory.right, rectCategory.top);
	pDC->LineTo (rectCategory.right, rectCategory.bottom);
	pDC->LineTo (rectCategory.left, rectCategory.bottom);
	pDC->LineTo (rectCategory.left, rectCategory.top);

	pDC->SelectObject (pOldPen);

	CBCGPDrawManager dm (*pDC);
	dm.DrawShadow (rectCategory, nShadowSize, 100, 75, NULL, NULL,
		m_clrMenuShadowBase);
}
//*****************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonCategoryTab (
					CDC* pDC, 
					CBCGPRibbonTab* pTab,
					BOOL bIsActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTab);

	CBCGPRibbonCategory* pCategory = pTab->GetParentCategory ();
	ASSERT_VALID (pCategory);
	CBCGPRibbonBar* pBar = pCategory->GetParentRibbonBar ();
	ASSERT_VALID (pBar);

	bIsActive = bIsActive && 
		((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown () != NULL);

	const BOOL bIsHighlighted = pTab->IsHighlighted () && !pTab->IsDroppedDown ();

	CPen pen (PS_SOLID, 1, globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);
	ASSERT (pOldPen != NULL);

	CRect rectTab = pTab->GetRect ();

	rectTab.top += 3;

	const int nTrancateRatio =
		pTab->GetParentCategory ()->GetParentRibbonBar ()->GetTabTrancateRatio ();

	if (nTrancateRatio > 0)
	{
		const int nPercent = max (10, 100 - nTrancateRatio / 2);

		COLORREF color = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, nPercent);

		CPen pen (PS_SOLID, 1, color);
		CPen* pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rectTab.right - 1, rectTab.top);
		pDC->LineTo (rectTab.right - 1, rectTab.bottom);

		pDC->SelectObject (pOldPen);
	}

	if (!bIsActive && !bIsHighlighted)
	{
		return globalData.clrBarText;
	}

	rectTab.right -= 2;

	#define POINTS_NUM	8
	POINT pts [POINTS_NUM];

	pts [0] = CPoint (rectTab.left, rectTab.bottom);
	pts [1] = CPoint (rectTab.left + 1, rectTab.bottom - 1);
	pts [2] = CPoint (rectTab.left + 1, rectTab.top + 2);
	pts [3] = CPoint (rectTab.left + 3, rectTab.top);
	pts [4] = CPoint (rectTab.right - 3, rectTab.top);
	pts [5] = CPoint (rectTab.right - 1, rectTab.top + 2);
	pts [6] = CPoint (rectTab.right - 1, rectTab.bottom - 1);
	pts [7] = CPoint (rectTab.right, rectTab.bottom);

	CRgn rgnClip;
	rgnClip.CreatePolygonRgn (pts, POINTS_NUM, WINDING);

	if (bIsActive)
	{
		pDC->SelectClipRgn (&rgnClip);

		COLORREF clrFill = RibbonCategoryColorToRGB (
			pTab->GetParentCategory ()->GetTabColor ());
		
		if (clrFill != (COLORREF)-1)
		{
			CBrush br (clrFill);
			pDC->FillRect (rectTab, &br);
		}
		else
		{
			pDC->FillRect (rectTab, 
				bIsHighlighted ? 
				&globalData.brWindow : &globalData.brBarFace);
		}

		pDC->SelectClipRgn (NULL);
	}

	pDC->Polyline (pts, POINTS_NUM);

	pDC->SelectObject (pOldPen);

	return globalData.clrBarText;
}
//****************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonPanel (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel, 
		CRect rectPanel,
		CRect /*rectCaption*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPanel);

	if (pPanel->IsHighlighted ())
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rectPanel);
	}

	pDC->Draw3dRect (rectPanel, globalData.clrBarHilite, globalData.clrBarHilite);
	rectPanel.OffsetRect (-1, -1);
	pDC->Draw3dRect (rectPanel, globalData.clrBarShadow, globalData.clrBarShadow);

	return globalData.clrBarText;
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonPanelCaption (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel,
		CRect rectCaption)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPanel);

	COLORREF clrTextOld = pDC->SetTextColor (pPanel->IsHighlighted () ? 
		globalData.clrCaptionText : globalData.clrInactiveCaptionText);

	rectCaption.DeflateRect (1, 1);

	CBrush br (pPanel->IsHighlighted () ? 
		globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

	CString str = pPanel->GetName ();

	if (pPanel->GetLaunchButton ().GetID () > 0)
	{
		rectCaption.right = pPanel->GetLaunchButton ().GetRect ().left;
	}

	rectCaption.DeflateRect (4, 0);

	pDC->DrawText (	str, rectCaption, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

	pDC->SetTextColor (clrTextOld);
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonLaunchButton (
					CDC* pDC,
					CBCGPRibbonLaunchButton* pButton,
					CBCGPRibbonPanel* pPanel)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pPanel);

	OnFillRibbonButton (pDC, pButton);

	COLORREF clrText = pPanel->IsHighlighted () ? 
		globalData.clrCaptionText : globalData.clrInactiveCaptionText;
	
	CMenuImages::IMAGE_STATE imageState = CMenuImages::ImageBlack;
	
	if (pButton->IsDisabled ())
	{
		imageState = CMenuImages::ImageGray;
	}
	else if (!pButton->IsHighlighted ())
	{
		if (GetRValue (clrText) > 192 &&
			GetGValue (clrText) > 192 &&
			GetBValue (clrText) > 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}

	CMenuImages::Draw (pDC, CMenuImages::IdLaunchArrow, 
		pButton->GetRect (), imageState);

	OnDrawRibbonButtonBorder (pDC, pButton);
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonDefaultPaneButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CRect rectMenuArrow = pButton->GetRect ();;

	if (pButton->IsQATMode ())
	{
		OnFillRibbonButton (pDC, pButton);

		CRect rectImage = pButton->GetRect ();
		rectImage.bottom = rectImage.top + globalData.m_sizeSmallIcon.cy;

		pButton->DrawImage (pDC, CBCGPRibbonButton::RibbonImageSmall, rectImage);

		OnDrawRibbonButtonBorder (pDC, pButton);

		rectMenuArrow.top = rectMenuArrow.bottom - 4;
	}
	else
	{
		CRect rectImage = pButton->GetRect ();
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

	CRect rectWhite = rectMenuArrow;
	rectWhite.OffsetRect (0, 1);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectWhite,
		CMenuImages::ImageWhite);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectMenuArrow,
		CMenuImages::ImageBlack);
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonDefaultPaneButtonIndicator (
					CDC* pDC, 
					CBCGPRibbonButton* pButton,
					CRect rect, 
					BOOL /*bIsSelected*/, 
					BOOL /*bHighlighted*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	rect.left = rect.right - rect.Height ();
	rect.DeflateRect (1, 1);

	pDC->FillRect (rect, &globalData.brBarFace);
	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);

	CRect rectWhite = rect;
	rectWhite.OffsetRect (0, 1);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rectWhite,
		CMenuImages::ImageWhite);

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rect,
		CMenuImages::ImageBlack);
}
//****************************************************************************
COLORREF CBCGPVisualManager::OnFillRibbonButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		COLORREF clrBorder = globalData.clrBarShadow;
		CRect rectCommand = pButton->GetCommandRect ();

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (pButton->GetRect (), globalData.clrWindow, clrBorder);
		}
		else
		{
			if (pButton->IsDroppedDown () || pButton->IsHighlighted ())
			{
				pDC->FillRect (rectCommand, &globalData.brWindow);
			}
			else
			{
				CBCGPDrawManager dm (*pDC);
				dm.HighlightRect (rectCommand);
			}

			pDC->Draw3dRect (pButton->GetRect (), clrBorder, clrBorder);
		}

		return (COLORREF)-1;
	}

	if (pButton->IsMenuMode ())
	{
		if (pButton->IsHighlighted ())
		{
			pDC->FillRect (pButton->GetRect (), &globalData.brHilite);
			return globalData.clrTextHilite;
		}
	}
	else 
	{
		if (pButton->IsChecked () && !pButton->IsHighlighted ())
		{
			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				dm.DrawRect (pButton->GetRect (), globalData.clrWindow, (COLORREF)-1);
			}
			else
			{
				CBCGPToolBarImages::FillDitheredRect (pDC, pButton->GetRect ());
			}
		}
	}

	return (COLORREF)-1;
}
//****************************************************************************
COLORREF CBCGPVisualManager::OnFillRibbonMainPanelButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	return OnFillRibbonButton (pDC, pButton);
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonMainPanelButtonBorder (
		CDC* pDC, CBCGPRibbonButton* pButton)
{
	OnDrawRibbonButtonBorder (pDC, pButton);
}
//****************************************************************************
void CBCGPVisualManager::OnFillRibbonEdit (
					CDC* pDC, 
					CBCGPRibbonEditCtrl* /*pEdit*/,
					CRect rect,
					BOOL bIsHighlighted,
					BOOL /*bIsPaneHighlighted*/,
					BOOL bIsDisabled,
					COLORREF& /*clrText*/, COLORREF& /*clrSelBackground*/, COLORREF& /*clrSelText*/)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted && !bIsDisabled)
	{
		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, globalData.clrWindow, (COLORREF)-1);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brWindow);
		}
	}
	else
	{
		CBCGPDrawManager dm (*pDC);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			dm.DrawRect (rect, globalData.clrBarFace, (COLORREF)-1);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBarFace);
			dm.HighlightRect (rect);
		}
	}
}
//****************************************************************************
void CBCGPVisualManager::OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		return;
	}

	CRect rect = pButton->GetRect ();

	if (pButton->IsMenuMode () &&
		pButton->IsChecked () && !pButton->IsHighlighted ())
	{
		return;
	}

	if (pButton->IsHighlighted () || pButton->IsChecked () ||
		pButton->IsDroppedDown ())
	{
		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, globalData.clrBarShadow);
		}
		else
		{
			if (pButton->IsPressed () || pButton->IsChecked () ||
				pButton->IsDroppedDown ())
			{
				pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
			}
			else 
			{
				pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
			}
		}

		CRect rectMenu = pButton->GetMenuRect ();
		
		if (!rectMenu.IsRectEmpty ())
		{
			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				
				if (pButton->IsMenuOnBottom ())
				{
					dm.DrawLine (rectMenu.left, rectMenu.top, rectMenu.right, rectMenu.top, globalData.clrBarShadow);
				}
				else
				{
					dm.DrawLine (rectMenu.left, rectMenu.top, rectMenu.left, rectMenu.bottom, globalData.clrBarShadow);
				}
			}
			else
			{
				CPen* pOldPen = pDC->SelectObject (&globalData.penBarShadow);
				ASSERT (pOldPen != NULL);

				if (pButton->IsMenuOnBottom ())
				{
					pDC->MoveTo (rectMenu.left, rectMenu.top);
					pDC->LineTo (rectMenu.right, rectMenu.top);
				}
				else
				{
					pDC->MoveTo (rectMenu.left, rectMenu.top);
					pDC->LineTo (rectMenu.left, rectMenu.bottom);
				}

				pDC->SelectObject (pOldPen);
			}
		}
	}
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonMenuCheckFrame (
					CDC* pDC,
					CBCGPRibbonButton* /*pButton*/, 
					CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);

	pDC->Draw3dRect (rect, globalData.clrBtnShadow,
							globalData.clrBtnHilite);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonMainPanelFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	rect.InflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
}
//***********************************************************************************
void CBCGPVisualManager::OnFillRibbonMenuFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brWindow);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBtnFace);

	CRect rectSeparator = rect;
	rectSeparator.right = rectSeparator.left + 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBtnShadow,
									globalData.clrBtnHilite);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonLabel (
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
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonPaletteButton (
					CDC* pDC, 
					CBCGPRibbonPaletteIcon* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	OnFillRibbonButton (pDC, pButton);
	OnDrawRibbonButtonBorder (pDC, pButton);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawRibbonPaletteBorder (
				CDC* pDC, 
				CBCGPRibbonPaletteButton* /*pButton*/, 
				CRect rectBorder)
{
	ASSERT_VALID (pDC);
	pDC->Draw3dRect (rectBorder, globalData.clrBarShadow, globalData.clrBarShadow);
}
//***********************************************************************************
COLORREF CBCGPVisualManager::RibbonCategoryColorToRGB (BCGPRibbonCategoryColor color)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		switch (color)
		{
		case BCGPCategoryColor_Red:
			return RGB (255, 0, 0);

		case BCGPCategoryColor_Orange:
			return RGB (255, 128, 0);

		case BCGPCategoryColor_Yellow:
			return RGB (255, 255, 0);

		case BCGPCategoryColor_Green:
			return RGB (0, 255, 0);

		case BCGPCategoryColor_Blue:
			return RGB (0, 0, 255);

		case BCGPCategoryColor_Indigo:
			return RGB (0, 0, 128);

		case BCGPCategoryColor_Violet:
			return RGB (255, 0, 255);
		}

		return (COLORREF)-1;
	}

	switch (color)
	{
	case BCGPCategoryColor_Red:
		return RGB (255, 160, 160);

	case BCGPCategoryColor_Orange:
		return RGB (239, 189, 55);

	case BCGPCategoryColor_Yellow:
		return RGB (253, 229, 27);

	case BCGPCategoryColor_Green:
		return RGB (113, 190, 89);

	case BCGPCategoryColor_Blue:
		return RGB (128, 181, 196);

	case BCGPCategoryColor_Indigo:
		return RGB (114, 163, 224);

	case BCGPCategoryColor_Violet:
		return RGB (214, 178, 209);
	}

	return (COLORREF)-1;
}
//***********************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonCategoryCaption (
					CDC* pDC, 
					CBCGPRibbonContextCaption* pContextCaption)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pContextCaption);

	COLORREF clrFill = RibbonCategoryColorToRGB (pContextCaption->GetColor ());
	CRect rect = pContextCaption->GetRect ();
	
	if (clrFill != (COLORREF)-1)
	{
		CBrush br (clrFill);
		pDC->FillRect (rect, &br);
	}

	return globalData.clrBarText;
}
//********************************************************************************
COLORREF CBCGPVisualManager::OnDrawRibbonStatusBarPane (CDC* pDC, CBCGPRibbonStatusBar* /*pBar*/,
					CBCGPRibbonStatusBarPane* pPane)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPane);

	CRect rect = pPane->GetRect ();

	if (pPane->IsHighlighted ())
	{
		CRect rectButton = rect;
		rectButton.DeflateRect (1, 1);

		pDC->Draw3dRect (rectButton, 
			pPane->IsPressed () ? globalData.clrBarShadow : globalData.clrBarHilite,
			pPane->IsPressed () ? globalData.clrBarHilite : globalData.clrBarShadow);
	}

	return (COLORREF)-1;
}
//********************************************************************************
void CBCGPVisualManager::GetRibbonSliderColors (CBCGPRibbonSlider* /*pSlider*/,
							BOOL bIsHighlighted, 
							BOOL bIsPressed,
							BOOL bIsDisabled,
							COLORREF& clrLine,
							COLORREF& clrFill)
{
	clrLine = bIsDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow;

	clrFill = bIsPressed && bIsHighlighted ?
		globalData.clrBarShadow :
		bIsHighlighted ? globalData.clrBarHilite : globalData.clrBarFace;
}
//********************************************************************************
void CBCGPVisualManager::OnDrawRibbonSliderZoomButton (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect, BOOL bIsZoomOut, 
			BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	ASSERT_VALID (pDC);

	COLORREF clrLine;
	COLORREF clrFill;

	GetRibbonSliderColors (pSlider, bIsHighlighted, bIsPressed, bIsDisabled,
		clrLine, clrFill);

	CPoint ptCenter = rect.CenterPoint ();

	CRect rectCircle (CPoint (ptCenter.x - 7, ptCenter.y - 7), CSize (15, 15));

	CBCGPDrawManager dm (*pDC);

	dm.DrawEllipse (rectCircle, clrFill, clrLine);

	// Draw +/- sign:
	CRect rectSign (CPoint (ptCenter.x - 3, ptCenter.y - 3), CSize (7, 7));

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		dm.DrawLine (rectSign.left, ptCenter.y, rectSign.right, ptCenter.y, clrLine);

		if (!bIsZoomOut)
		{
			dm.DrawLine (ptCenter.x, rectSign.top, ptCenter.x, rectSign.bottom, clrLine);
		}
	}
	else
	{
		CPen penLine (PS_SOLID, 1, clrLine);
		CPen* pOldPen = pDC->SelectObject (&penLine);

		pDC->MoveTo (rectSign.left, ptCenter.y);
		pDC->LineTo (rectSign.right, ptCenter.y);

		if (!bIsZoomOut)
		{
			pDC->MoveTo (ptCenter.x, rectSign.top);
			pDC->LineTo (ptCenter.x, rectSign.bottom);
		}

		pDC->SelectObject (pOldPen);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawRibbonSliderChannel (
			CDC* pDC, CBCGPRibbonSlider* /*pSlider*/, 
			CRect rect)
{
	ASSERT_VALID (pDC);

	rect.InflateRect (0, 1);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, (COLORREF)-1, globalData.clrBarShadow);
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawRibbonSliderThumb (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	ASSERT_VALID (pDC);

	COLORREF clrLine;
	COLORREF clrFill;

	GetRibbonSliderColors (pSlider, bIsHighlighted, bIsPressed, bIsDisabled,
		clrLine, clrFill);

	rect.DeflateRect (1, 2);

	rect.top = rect.CenterPoint ().y - rect.Width ();
	rect.bottom = rect.top + 2 * rect.Width ();

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, clrFill, clrLine);
	}
	else
	{
		CPen penLine (PS_SOLID, 1, clrLine);
		CPen* pOldPen = pDC->SelectObject (&penLine);

		CBrush br (clrFill);
		CBrush* pOldBrush = pDC->SelectObject (&br);

		POINT pts [5] =
		{
			{	rect.left, rect.top	},
			{	rect.left, rect.bottom - rect.Width () / 2	},
			{	rect.left + rect.Width () / 2, rect.bottom	},
			{	rect.right, rect.bottom - rect.Width () / 2	},
			{	rect.right, rect.top						},
		};

		pDC->Polygon (pts, 5);

		pDC->SelectObject (pOldPen);
		pDC->SelectObject (pOldBrush);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawRibbonProgressBar (CDC* pDC, 
												  CBCGPRibbonProgressBar* /*pProgress*/, 
												  CRect rectProgress, CRect rectChunk,
												  BOOL /*bInfiniteMode*/)
{
	ASSERT_VALID (pDC);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		
		if (!rectChunk.IsRectEmpty ())
		{
			dm.DrawRect (rectChunk, globalData.clrHilite, (COLORREF)-1);
		}

		dm.DrawRect (rectProgress, (COLORREF)-1, globalData.clrBarShadow);
	}
	else
	{
		if (!rectChunk.IsRectEmpty ())
		{
			pDC->FillRect (rectChunk, &globalData.brHilite);
		}

		pDC->Draw3dRect (rectProgress, globalData.clrBarShadow, globalData.clrBarHilite);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnFillRibbonQATPopup (
				CDC* pDC, CBCGPRibbonPanelMenuBar* /*pMenuBar*/, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawRibbonQATSeparator (CDC* pDC, 
												   CBCGPRibbonSeparator* /*pSeparator*/, CRect rect)
{
	ASSERT_VALID (pDC);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, (COLORREF)-1, globalData.clrBtnShadow);
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
	}
}

#endif // BCGP_EXCLUDE_RIBBON

BOOL CBCGPVisualManager::OnSetWindowRegion (CWnd* pWnd, CSize sizeWindow)
{
#ifdef BCGP_EXCLUDE_RIBBON
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(sizeWindow);
	return FALSE;
#else
	if (globalData.DwmIsCompositionEnabled ())
	{
		return FALSE;
	}

	ASSERT_VALID (pWnd);

	CBCGPRibbonBar* pRibbonBar = NULL;

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		pRibbonBar = ((CBCGPFrameWnd*) pWnd)->GetRibbonBar ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		pRibbonBar = ((CBCGPMDIFrameWnd*) pWnd)->GetRibbonBar ();
	}

	if (pRibbonBar == NULL || 
		!pRibbonBar->IsWindowVisible () ||
		!pRibbonBar->IsReplaceFrameCaption ())
	{
		return FALSE;
	}

	const int nLeftRadius  = 11;
	const int nRightRadius = 11;

	CRgn rgnWnd;
	rgnWnd.CreateRectRgn (0, 0, sizeWindow.cx, sizeWindow.cy);

	CRgn rgnTemp;

	rgnTemp.CreateRectRgn (0, 0, nLeftRadius / 2, nLeftRadius / 2);
	rgnWnd.CombineRgn (&rgnTemp, &rgnWnd, RGN_XOR);

	rgnTemp.DeleteObject ();
	rgnTemp.CreateEllipticRgn (0, 0, nLeftRadius, nLeftRadius);
	rgnWnd.CombineRgn (&rgnTemp, &rgnWnd, RGN_OR);

	rgnTemp.DeleteObject ();
	rgnTemp.CreateRectRgn (sizeWindow.cx - nRightRadius / 2, 0, sizeWindow.cx, nRightRadius / 2);
	rgnWnd.CombineRgn (&rgnTemp, &rgnWnd, RGN_XOR);

	rgnTemp.DeleteObject ();
	rgnTemp.CreateEllipticRgn (sizeWindow.cx - nRightRadius + 1, 0, sizeWindow.cx + 1, nRightRadius);
	rgnWnd.CombineRgn (&rgnTemp, &rgnWnd, RGN_OR);
	
	pWnd->SetWindowRgn ((HRGN)rgnWnd.Detach (), TRUE);
	return TRUE;
#endif
}
//*****************************************************************************
BOOL CBCGPVisualManager::OnNcPaint (CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, CRect /*rectRedraw*/)
{
	return FALSE;
}
//*****************************************************************************
void CBCGPVisualManager::OnActivateApp (CWnd* /*pWnd*/, BOOL /*bActive*/)
{
}
//*****************************************************************************
BOOL CBCGPVisualManager::OnNcActivate (CWnd* /*pWnd*/, BOOL /*bActive*/)
{
	return FALSE;
}
//*****************************************************************************
CSize CBCGPVisualManager::GetNcBtnSize (BOOL /*bSmall*/) const
{
	return CSize (0, 0);
}
//*****************************************************************************
BOOL CBCGPVisualManager::OnEraseMDIClientArea (CDC* /*pDC*/, CRect /*rectClient*/)
{
	return FALSE;
}
//*******************************************************************************
BOOL CBCGPVisualManager::GetToolTipParams (CBCGPToolTipParams& params, 
										   UINT /*nType*/ /*= (UINT)(-1)*/)
{
	CBCGPToolTipParams dummy;
	params = dummy;

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPVisualManager::DrawTextOnGlass (CDC* pDC, CString strText, CRect rect, 
										  DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
	ASSERT_VALID (pDC);

	COLORREF clrOldText = pDC->GetTextColor ();
	pDC->SetTextColor (RGB (0, 0, 0));	// TODO

	BOOL bRes = globalData.DrawTextOnGlass (m_hThemeButton, pDC, 0, 0, strText, rect, 
		dwFlags, nGlowSize, clrText);

	pDC->SetTextColor (clrOldText);

	return bRes;
}
//*******************************************************************************
COLORREF CBCGPVisualManager::OnDrawPropSheetListItem (CDC* pDC, CBCGPPropertySheet* /*pParent*/, 
		CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	ASSERT_VALID (pDC);

	COLORREF clrText = (COLORREF)-1;

	if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brHilite);
		clrText = globalData.clrTextHilite;
	}

	if (bIsHighlihted)
	{
		pDC->DrawFocusRect (rect);
	}

	return clrText;
}

/////////////////////////////////////////////////////////////////////////////////////
// CBCGPWinXPThemeManager

CBCGPWinXPThemeManager::CBCGPWinXPThemeManager ()
{
	m_hThemeWindow = NULL;
	m_hThemeToolBar = NULL;
	m_hThemeButton = NULL;
	m_hThemeStatusBar = NULL;
	m_hThemeRebar = NULL;
	m_hThemeComboBox = NULL;
	m_hThemeProgress = NULL;
	m_hThemeHeader = NULL;
	m_hThemeScrollBar = NULL;
	m_hThemeExplorerBar = NULL;
	m_hThemeTree = NULL;
	m_hThemeStartPanel = NULL;
	m_hThemeTaskBand = NULL;
	m_hThemeTaskBar = NULL;
	m_hThemeSpin = NULL;
	m_hThemeTab = NULL;
	m_hThemeTrack = NULL;

	m_hinstUXDLL = LoadLibrary (_T("UxTheme.dll"));

	if (m_hinstUXDLL != NULL)
	{
		m_pfOpenThemeData = (OPENTHEMEDATA)::GetProcAddress (m_hinstUXDLL, "OpenThemeData");
		m_pfCloseThemeData = (CLOSETHEMEDATA)::GetProcAddress (m_hinstUXDLL, "CloseThemeData");
		m_pfDrawThemeBackground = (DRAWTHEMEBACKGROUND)::GetProcAddress (m_hinstUXDLL, "DrawThemeBackground");
		m_pfGetThemeColor = (GETTHEMECOLOR)::GetProcAddress (m_hinstUXDLL, "GetThemeColor");
		m_pfGetThemeSysColor = (GETTHEMESYSCOLOR)::GetProcAddress (m_hinstUXDLL, "GetThemeSysColor");
		m_pfGetCurrentThemeName = (GETCURRENTTHEMENAME)::GetProcAddress (m_hinstUXDLL, "GetCurrentThemeName");
		m_pfGetWindowTheme = (GETWINDOWTHEME)::GetProcAddress (m_hinstUXDLL, "GetWindowTheme");

		UpdateSystemColors ();
	}
	else
	{
		m_pfOpenThemeData = NULL;
		m_pfCloseThemeData = NULL;
		m_pfDrawThemeBackground = NULL;
		m_pfGetThemeColor = NULL;
		m_pfGetThemeSysColor = NULL;
		m_pfGetCurrentThemeName = NULL;
		m_pfGetWindowTheme = NULL;
	}
}
//*************************************************************************************
CBCGPWinXPThemeManager::~CBCGPWinXPThemeManager ()
{
	if (m_hinstUXDLL != NULL)
	{
		CleanUpThemes ();
		::FreeLibrary (m_hinstUXDLL);
	}
}
//*************************************************************************************
void CBCGPWinXPThemeManager::UpdateSystemColors ()
{
	if (m_hinstUXDLL != NULL)
	{
		CleanUpThemes ();

		if (m_pfOpenThemeData == NULL ||
			m_pfCloseThemeData == NULL ||
			m_pfDrawThemeBackground == NULL)
		{
			ASSERT (FALSE);
		}
		else
		{
			m_hThemeWindow = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"WINDOW");
			m_hThemeToolBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TOOLBAR");
			m_hThemeButton = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"BUTTON");
			m_hThemeStatusBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"STATUS");
			m_hThemeRebar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"REBAR");
			m_hThemeComboBox = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"COMBOBOX");
			m_hThemeProgress = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"PROGRESS");
			m_hThemeHeader = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"HEADER");
			m_hThemeScrollBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"SCROLLBAR");
			m_hThemeExplorerBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"EXPLORERBAR");
			m_hThemeTree = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TREEVIEW");
			m_hThemeStartPanel = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"STARTPANEL");
			m_hThemeTaskBand = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TASKBAND");
			m_hThemeTaskBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TASKBAR");
			m_hThemeSpin = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"SPIN");
			m_hThemeTab = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TAB");
			m_hThemeTrack = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TRACKBAR");
		}
	}
}
//************************************************************************************
void CBCGPWinXPThemeManager::CleanUpThemes ()
{
	if (m_pfCloseThemeData == NULL)
	{
		return;
	}

	if (m_hThemeWindow != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeWindow);
	}

	if (m_hThemeToolBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeToolBar);
	}

	if (m_hThemeRebar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeRebar);
	}

	if (m_hThemeStatusBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeStatusBar);
	}

	if (m_hThemeButton != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeButton);
	}

	if (m_hThemeComboBox != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeComboBox);
	}

	if (m_hThemeProgress != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeProgress);
	}

	if (m_hThemeHeader != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeHeader);
	}

	if (m_hThemeScrollBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeScrollBar);
	}

	if (m_hThemeExplorerBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeExplorerBar);
	}

	if (m_hThemeTree != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTree);
	}

	if (m_hThemeStartPanel != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeStartPanel);
	}

	if (m_hThemeTaskBand != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTaskBand);
	}

	if (m_hThemeTaskBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTaskBar);
	}

	if (m_hThemeSpin != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeSpin);
	}

	if (m_hThemeTab != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTab);
	}

	if (m_hThemeTrack != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTrack);
	}
}
//**********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawPushButton (CDC* pDC, CRect rect, 
											 CBCGPButton* pButton, UINT /*uiState*/)
{
	#define BP_PUSHBUTTON		1

	#define PBS_NORMAL			1
	#define PBS_HOT				2
	#define PBS_PRESSED			3
	#define PBS_DISABLED		4
	#define PBS_DEFAULTED		5

	if (m_hThemeButton == NULL)
	{
		return FALSE;
	}

	int nState = PBS_NORMAL;

	if (!pButton->IsWindowEnabled ())
	{
		nState = PBS_DISABLED;
	}
	else if (pButton->IsPressed () || pButton->GetCheck ())
	{
		nState = PBS_PRESSED;
	}
	else if (pButton->IsHighlighted ())
	{
		nState = PBS_HOT;
	}
	else if (CWnd::GetFocus () == pButton)
	{
		nState = PBS_DEFAULTED;
	}

	globalData.DrawParentBackground (pButton, pDC, rect);

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON, 
		nState, &rect, 0);

	return TRUE;
}
//**************************************************************************************
BOOL CBCGPWinXPThemeManager::DrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF /*clrBar*/, COLORREF /*clrProgressBarDest*/, COLORREF /*clrProgressText*/,
			BOOL bProgressText)
{
	#define	PP_BAR				1
	#define	PP_CHUNK			3

	if (m_hThemeProgress == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pDC);

	(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(), 
								PP_BAR, 0, &rectProgress, 0);

	if (nProgressTotal == 0)
	{
		return TRUE;
	}

	CRect rectComplete = rectProgress;
	rectComplete.DeflateRect (3, 3);

	rectComplete.right = rectComplete.left + 
		nProgressCurr * rectComplete.Width () / nProgressTotal;

	(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(), 
								PP_CHUNK, 0, &rectComplete, 0);

	if (bProgressText)
	{
		CString strText;
		strText.Format (_T("%d%%"), nProgressCurr * 100 / nProgressTotal);

		COLORREF clrText = pDC->SetTextColor (globalData.clrBtnText);
		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor (clrText);
	}

	return TRUE;
}
//********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
{
	#define CP_DROPDOWNBUTTON	1

	#define CBXS_NORMAL			1
	#define CBXS_HOT			2
	#define CBXS_PRESSED		3
	#define CBXS_DISABLED		4

	if (m_hThemeComboBox == NULL)
	{
		return FALSE;
	}

	int nState = bDisabled ? CBXS_DISABLED : bIsDropped ? CBXS_PRESSED : bIsHighlighted ? CBXS_HOT : CBXS_NORMAL;

	(*m_pfDrawThemeBackground) (m_hThemeComboBox, pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, 
		nState, &rect, 0);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawComboBorder (CDC* pDC, CRect rect,
										BOOL /*bDisabled*/,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
{
	ASSERT_VALID (pDC);

	if (m_hThemeWindow == NULL)
	{
		return FALSE;
	}

	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (&rect,  globalData.clrHilite, globalData.clrHilite);
	}

	return TRUE;
}
//********************************************************************************
void CBCGPWinXPThemeManager::FillRebarPane (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rectClient)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (m_pfDrawThemeBackground == NULL || m_hThemeRebar == NULL)
	{
		pDC->FillRect (rectClient, &globalData.brBarFace);
		return;
	}

	CWnd* pWndParent = BCGPGetParentFrame (pBar);
	if (pWndParent->GetSafeHwnd () == NULL)
	{
		pWndParent = pBar->GetParent ();
	}

	ASSERT_VALID (pWndParent);

	CRect rectParent;
	pWndParent->GetWindowRect (rectParent);

	pBar->ScreenToClient (&rectParent);

	rectClient.right = max (rectClient.right, rectParent.right);
	rectClient.bottom = max (rectClient.bottom, rectParent.bottom);

	if (!pBar->IsFloating () && pBar->GetParentMiniFrame () == NULL)
	{
		rectClient.left = rectParent.left;
		rectClient.top = rectParent.top;
	}

	(*m_pfDrawThemeBackground) (m_hThemeRebar, pDC->GetSafeHdc(),
		0, 0, &rectClient, 0);
}

BOOL CBCGPWinXPThemeManager::DrawCheckBox (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 int nState,
										 BOOL bEnabled,
										 BOOL bPressed)
{
	#define CBS_UNCHECKEDNORMAL	1
	#define CBS_UNCHECKEDHOT	2
	#define CBS_UNCHECKEDPRESSED	3
	#define CBS_UNCHECKEDDISABLED	4
	#define CBS_CHECKEDNORMAL	5
	#define CBS_CHECKEDHOT		6
	#define CBS_CHECKEDPRESSED	7
	#define CBS_CHECKEDDISABLED	8
	#define CBS_MIXEDNORMAL		9
	#define CBS_MIXEDHOT		10
	#define CBS_MIXEDPRESSED	11
	#define CBS_MIXEDDISABLED	12

	#define BP_CHECKBOX			3

	if (m_hThemeButton == NULL)
	{
		return FALSE;
	}

	nState = max (0, nState);
	nState = min (2, nState);

	ASSERT_VALID (pDC);

	int nDrawState = nState == 1 ? CBS_CHECKEDNORMAL : nState == 2 ? CBS_MIXEDNORMAL : CBS_UNCHECKEDNORMAL;

	if (!bEnabled)
	{
		nDrawState = nState == 1 ? CBS_CHECKEDDISABLED : nState == 2 ? CBS_MIXEDDISABLED : CBS_UNCHECKEDDISABLED;
	}
	else if (bPressed)
	{
		nDrawState = nState == 1 ? CBS_CHECKEDPRESSED : nState == 2 ? CBS_MIXEDPRESSED : CBS_UNCHECKEDPRESSED;
	}
	else if (bHighlighted)
	{
		nDrawState = nState == 1 ? CBS_CHECKEDHOT : nState == 2 ? CBS_MIXEDHOT : CBS_UNCHECKEDHOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_CHECKBOX,
		nDrawState, &rect, 0);

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPWinXPThemeManager::DrawRadioButton (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 BOOL bChecked,
										 BOOL bEnabled,
										 BOOL bPressed)
{
	#define RBS_UNCHECKEDNORMAL		1
    #define RBS_UNCHECKEDHOT		2
    #define RBS_UNCHECKEDPRESSED	3
    #define RBS_UNCHECKEDDISABLED	4
    #define RBS_CHECKEDNORMAL		5
    #define RBS_CHECKEDHOT			6
    #define RBS_CHECKEDPRESSED		7
    #define RBS_CHECKEDDISABLED		8

	#define BP_RADIOBUTTON			2

	if (m_hThemeButton == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pDC);

	int nDrawState = bChecked ? RBS_CHECKEDNORMAL : RBS_UNCHECKEDNORMAL;

	if (!bEnabled)
	{
		nDrawState = bChecked ? RBS_CHECKEDDISABLED : RBS_UNCHECKEDDISABLED;
	}
	else if (bPressed)
	{
		nDrawState = bChecked ? RBS_CHECKEDPRESSED : RBS_UNCHECKEDPRESSED;
	}
	else if (bHighlighted)
	{
		nDrawState = bChecked ? RBS_CHECKEDHOT : RBS_UNCHECKEDHOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_RADIOBUTTON,
		nDrawState, &rect, 0);

	return TRUE;
}
//*******************************************************************************
CBCGPWinXPThemeManager::WinXpTheme CBCGPWinXPThemeManager::GetStandardWinXPTheme ()
{
	WCHAR szName [256];
	WCHAR szColor [256];

	if (m_pfGetCurrentThemeName == NULL ||
		(*m_pfGetCurrentThemeName) (szName, 255, szColor, 255, NULL, 0) != S_OK)
	{
		return WinXpTheme_None;
	}

	CString strThemeName = szName;
	CString strWinXPThemeColor = szColor;

	TCHAR fname[_MAX_FNAME];   
	_tsplitpath (strThemeName, NULL, NULL, fname, NULL);

	strThemeName = fname;

	if (strThemeName.CompareNoCase (_T("Luna")) != 0)
	{
		return WinXpTheme_NonStandard;
	}

	// Check for 3-d party visual managers:
	if (m_pfGetThemeColor != NULL && m_hThemeButton != NULL)
	{
		COLORREF clrTest = 0;
		if ((*m_pfGetThemeColor) (m_hThemeButton, 1, 0, 3823, &clrTest) !=
			S_OK || clrTest == 1)
		{
			return WinXpTheme_NonStandard;
		}
	}

	if (strWinXPThemeColor.CompareNoCase (_T("normalcolor")) == 0)
	{
		return WinXpTheme_Blue;
	}

	if (strWinXPThemeColor.CompareNoCase (_T("homestead")) == 0)
	{
		return WinXpTheme_Olive;
	}

	if (strWinXPThemeColor.CompareNoCase (_T("metallic")) == 0)
	{
		return WinXpTheme_Silver;
	}

	return WinXpTheme_NonStandard;
}
