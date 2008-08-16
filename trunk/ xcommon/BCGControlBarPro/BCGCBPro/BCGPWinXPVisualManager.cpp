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
//
// BCGWinXPVisualManager.cpp: implementation of the CBCGPWinXPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "multimon.h"
#include "BCGPWinXPVisualManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPButton.h"
#include "BCGPDrawManager.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPTasksPane.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPOutlookButton.h"
#include "BCGPStatusBar.h"
#include "BCGPCaptionBar.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPCalendarBar.h"
#include "BCGPDockBar.h"
#include "BCGPRibbonStatusBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if (defined(SCHEMA_STRINGS)) || (! defined(TMSCHEMA_H))

#define RP_GRIPPER			1
#define RP_GRIPPERVERT		2
#define RP_BAND				3

#define TP_BUTTON			1
#define TP_SEPARATOR		5
#define TP_SEPARATORVERT	6

#define WP_SMALLCAPTION		2
#define WP_MINCAPTION		3
#define WP_MDIMINBUTTON		16
#define WP_CLOSEBUTTON		18
#define	WP_SMALLCLOSEBUTTON	19
#define WP_MDICLOSEBUTTON	20
#define WP_MDIRESTOREBUTTON	22

#define BP_PUSHBUTTON		1
#define BP_RADIOBUTTON		2
#define BP_CHECKBOX			3
#define BP_GROUPBOX			4

#define SP_PANE				1

#define CP_DROPDOWNBUTTON	1

#define HP_HEADERITEM		1
#define HP_HEADERSORTARROW	4

#define SBP_SIZEBOX			10

#define TS_NORMAL			1
#define TS_HOT				2
#define TS_PRESSED			3
#define TS_DISABLED			4
#define TS_CHECKED			5
#define TS_HOTCHECKED		6

#define PBS_NORMAL			1
#define PBS_HOT				2
#define PBS_PRESSED			3
#define PBS_DISABLED		4
#define PBS_DEFAULTED		5

#define CBS_NORMAL			1
#define CBS_HOT				2
#define CBS_PUSHED			3
#define	CBS_DISABLED		4

#define CBXS_NORMAL			1
#define CBXS_HOT			2
#define CBXS_PRESSED		3
#define CBXS_DISABLED		4

#define MNCS_ACTIVE			1
#define MNCS_INACTIVE		2

#define TIBES_NORMAL		1
#define TIBES_HOT			2
#define TIBES_SELECTED		3
#define TIBES_DISABLED		4
#define TIBES_FOCUSED		5

#define HIS_NORMAL			1
#define HIS_HOT				2
#define HIS_PRESSED			3

#define HILS_NORMAL			1
#define HILS_HOT			2
#define HILS_PRESSED		3

#define HIRS_NORMAL			1
#define HIRS_HOT			2
#define HIRS_PRESSED		3

#define HSAS_SORTEDUP		1
#define HSAS_SORTEDDOWN		2

#define SZB_RIGHTALIGN		1
#define SZB_LEFTALIGN		2

#define EBP_HEADERBACKGROUND		1
#define EBP_HEADERCLOSE				2
#define EBP_HEADERPIN				3
#define EBP_IEBARMENU				4
#define EBP_NORMALGROUPBACKGROUND	5
#define EBP_NORMALGROUPCOLLAPSE		6
#define EBP_NORMALGROUPEXPAND		7
#define EBP_NORMALGROUPHEAD			8
#define EBP_SPECIALGROUPBACKGROUND	9
#define EBP_SPECIALGROUPCOLLAPSE	10
#define EBP_SPECIALGROUPEXPAND		11
#define EBP_SPECIALGROUPHEAD		12

#define EBNGC_NORMAL	1
#define EBNGC_HOT		2
#define EBNGC_PRESSED	3

#define EBNGE_NORMAL	1
#define EBNGE_HOT		2
#define EBNGE_PRESSED	3

#define EBSGC_NORMAL	1
#define EBSGC_HOT		2
#define EBSGC_PRESSED	3

#define EBSGE_NORMAL	1
#define EBSGE_HOT		2
#define EBSGE_PRESSED	3

#define TVP_GLYPH		2
#define GLPS_CLOSED		1
#define GLPS_OPENED		2

#define SPNP_UP			1
#define SPNP_DOWN		2
#define SPNP_UPHORZ		3
#define SPNP_DOWNHORZ	4

#define UPS_NORMAL		1
#define UPS_HOT			2
#define UPS_PRESSED		3
#define UPS_DISABLED	4

#define TABP_TABITEM				1
#define TABP_TABITEMLEFTEDGE		2
#define TABP_TABITEMRIGHTEDGE		3
#define TABP_TABITEMBOTHEDGE		4
#define TABP_TOPTABITEM				5
#define TABP_TOPTABITEMLEFTEDGE		6
#define TABP_TOPTABITEMRIGHTEDGE	7
#define TABP_TOPTABITEMBOTHEDGE		8
#define TABP_PANE					9
#define TABP_BODY					10

#define TIS_NORMAL		1
#define TIS_HOT			2
#define TIS_SELECTED	3
#define TIS_DISABLED	4
#define TIS_FOCUSED		5

#define TMT_FILLCOLOR			3802
#define	TMT_TEXTCOLOR			3803
#define TMT_EDGELIGHTCOLOR		3804
#define TMT_EDGEHIGHLIGHTCOLOR	3805
#define TMT_EDGESHADOWCOLOR		3806
#define TMT_EDGEDKSHADOWCOLOR	3807
#define TMT_FILLCOLORHINT		3821

#endif

IMPLEMENT_DYNCREATE(CBCGPWinXPVisualManager, CBCGPVisualManagerXP)

BOOL CBCGPWinXPVisualManager::m_b3DTabsXPTheme = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPWinXPVisualManager::CBCGPWinXPVisualManager(BOOL bIsTemporary) :
	CBCGPVisualManagerXP (bIsTemporary)
{
	m_bShadowHighlightedImage = FALSE;

	m_bOfficeStyleMenus = FALSE;

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

	globalData.UpdateSysColors ();
	OnUpdateSystemColors ();
}

CBCGPWinXPVisualManager::~CBCGPWinXPVisualManager()
{
}

BOOL CBCGPWinXPVisualManager::IsWinXPThemeAvailible ()
{
	CBCGPWinXPVisualManager* pWinXPManager =
		DYNAMIC_DOWNCAST (CBCGPWinXPVisualManager, m_pVisManager);
	if (pWinXPManager != NULL)
	{
		return pWinXPManager->m_hThemeWindow != NULL;
	}

	// Create a temporary manager and check it:
	CBCGPWinXPVisualManager winXPManager (TRUE /* Temporary */);
	return winXPManager.m_hThemeWindow != NULL;
}

void CBCGPWinXPVisualManager::SetOfficeStyleMenus (BOOL bOn)
{
	m_bOfficeStyleMenus = bOn;
}

void CBCGPWinXPVisualManager::OnUpdateSystemColors ()
{
	CBCGPVisualManagerXP::OnUpdateSystemColors ();

	m_bShadowHighlightedImage = TRUE;

	CBCGPWinXPThemeManager::UpdateSystemColors ();

	if (m_hThemeWindow != NULL)
	{
		m_bShadowHighlightedImage = FALSE;
	}

	m_clrCalendarCaption = (COLORREF)-1;
	m_clrCalendarCaptionText = (COLORREF)-1;

	if (m_pfGetThemeColor != NULL)
	{
		if (m_hThemeToolBar != NULL)
		{
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_TEXTCOLOR, &globalData.clrBarText);
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_FILLCOLOR, &globalData.clrBarFace);
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGELIGHTCOLOR, &globalData.clrBarLight);
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGEHIGHLIGHTCOLOR, &globalData.clrBarHilite);
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGESHADOWCOLOR, &globalData.clrBarShadow);
			(*m_pfGetThemeColor) (m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGEDKSHADOWCOLOR, &globalData.clrBarDkShadow);
		}

		if (m_hThemeButton != NULL)
		{
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_TEXTCOLOR, &globalData.clrBtnText);
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_FILLCOLOR, &globalData.clrBtnFace);
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGELIGHTCOLOR, &globalData.clrBtnLight);
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGEHIGHLIGHTCOLOR, &globalData.clrBtnHilite);
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGESHADOWCOLOR, &globalData.clrBtnShadow);
			(*m_pfGetThemeColor) (m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGEDKSHADOWCOLOR, &globalData.clrBtnDkShadow);
		}

		if (m_hThemeExplorerBar != NULL)
		{
			(*m_pfGetThemeColor) (m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_FILLCOLOR, &m_clrCalendarCaption);
			(*m_pfGetThemeColor) (m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_TEXTCOLOR, &m_clrCalendarCaptionText);
		}
	}
}

void CBCGPWinXPVisualManager::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	if (m_hThemeRebar == NULL || pBar->GetSafeHwnd () == NULL)
	{
		CBCGPVisualManagerXP::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	BOOL bSideBar = pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar));

	CRect rectFill = rectGripper;

	if (bSideBar)
	{
		bHorz = !bHorz;
	}

	COLORREF clrTextOld = pDC->SetTextColor (globalData.clrBtnShadow);
	COLORREF clrBkOld = pDC->SetBkColor (globalData.clrBtnFace);

	CRect rectGripperTheme = rectGripper;
	const int nGripperOffset = 2;

	if (bHorz)
	{
		rectGripperTheme.DeflateRect (0, nGripperOffset);
		rectGripperTheme.OffsetRect (nGripperOffset, 0);
		rectGripperTheme.right = rectGripperTheme.left + 3 * nGripperOffset;
	}
	else
	{
		rectGripperTheme.DeflateRect (nGripperOffset, 0);
		rectGripperTheme.OffsetRect (0, nGripperOffset);
		rectGripperTheme.bottom = rectGripperTheme.top + 3 * nGripperOffset;
	}

	(*m_pfDrawThemeBackground) (m_hThemeRebar, pDC->GetSafeHdc(),
		bHorz ? RP_GRIPPER : RP_GRIPPERVERT, 0, &rectGripperTheme, 0);

	if (bSideBar)
	{
		//------------------
		// Draw bar caption:
		//------------------
		int nOldBkMode = pDC->SetBkMode (OPAQUE);
		pDC->SetTextColor (globalData.clrBtnText);

		const CFont& font = CBCGPMenuBar::GetMenuFont (bHorz);

		CFont* pOldFont = pDC->SelectObject ((CFont*) &font);

		CString strCaption;
		pBar->GetWindowText (strCaption);
		strCaption = _T(" ") + strCaption + _T(" ");

		CRect rectText = rectGripper;
		UINT uiTextFormat = 0;

		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		CPoint ptTextOffset (0, 0);
		if (bHorz)
		{
			ptTextOffset.y = (rectGripper.Height () - tm.tmHeight - 1) / 2;
		}
		else
		{
			ptTextOffset.x = (rectGripper.Width () - tm.tmHeight + 1) / 2;
		}

		if (bHorz)
		{
			rectText.top += ptTextOffset.y;
			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}
		else
		{
			rectText.left = rectText.right - ptTextOffset.x;
			rectText.top = rectGripper.top + ptTextOffset.y;
			rectText.bottom = rectGripper.top + 3 * ptTextOffset.y;

			uiTextFormat |= DT_NOCLIP;

			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}

		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldBkMode);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea)
{
	ASSERT_VALID (pBar);
	ASSERT_VALID (pDC);

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPStatusBar)) &&
		m_hThemeStatusBar != NULL)
	{
		(*m_pfDrawThemeBackground) (m_hThemeStatusBar, 
			pDC->GetSafeHdc (),
			0, 0, &rectClient, 0);
		return;
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonStatusBar)))
	{
		if (m_hThemeStatusBar != NULL)
		{
			(*m_pfDrawThemeBackground) (m_hThemeStatusBar, 
				pDC->GetSafeHdc (),
				0, 0, &rectClient, 0);
			return;
		}
	}
#endif	// BCGP_EXCLUDE_RIBBON

	if (m_pfDrawThemeBackground == NULL || 
		m_hThemeRebar == NULL || 
		pBar->IsDialogControl () ||
		pBar->IsKindOf (RUNTIME_CLASS (CBCGPCaptionBar)))
	{
		CBCGPVisualManagerXP::OnFillBarBackground (pDC, pBar,rectClient, rectClip, bNCArea);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		if (m_bOfficeStyleMenus)
		{
			CBCGPVisualManagerXP::OnFillBarBackground (pDC, pBar,rectClient, rectClip, bNCArea);
		}
		else
		{
			::FillRect (pDC->GetSafeHdc (), rectClient, ::GetSysColorBrush (COLOR_MENU));
		}

		return;
	}

	FillRebarPane (pDC, pBar, rectClient);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	CBCGPVisualManager::OnDrawBarBorder (pDC, pBar, rect);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnFillButtonInterior (CDC* pDC,
				CBCGPToolbarButton* pButton, CRect rect,
				CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (m_hThemeToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	BOOL bIsMenuBar = FALSE;
	BOOL bIsPopupMenu = FALSE;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	if (pMenuButton != NULL)
	{
		bIsMenuBar = pMenuButton->GetParentWnd () != NULL &&
			pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

		bIsPopupMenu = pMenuButton->GetParentWnd () != NULL &&
			pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));
	}

	if (m_bOfficeStyleMenus && (bIsPopupMenu || bIsMenuBar))
	{
		CBCGPVisualManagerXP::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	if (bIsPopupMenu && state != ButtonsIsHighlighted &&
		state != ButtonsIsPressed)
	{
		return;
	}

	int nState = TS_NORMAL;
	
	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		nState = TS_DISABLED;
	}
	else if ((pButton->m_nStyle & TBBS_PRESSED) && state == ButtonsIsHighlighted)
	{
		nState = TS_PRESSED;
	}
	else if (pButton->m_nStyle & TBBS_CHECKED)
	{
		nState = (state == ButtonsIsHighlighted) ? TS_HOTCHECKED : TS_CHECKED;
	}
	else if (state == ButtonsIsHighlighted)
	{
		nState = TS_HOT;

		if (pMenuButton != NULL && pMenuButton->IsDroppedDown ())
		{
			nState = TS_PRESSED;
			rect.bottom--;
		}
	}

	(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), TP_BUTTON, nState, &rect, 0);
}
//**************************************************************************************
COLORREF CBCGPWinXPVisualManager::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, 
														CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	if (m_hThemeToolBar == NULL ||
		pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
	{
		return CBCGPVisualManagerXP::GetToolbarButtonTextColor (pButton, state);
	}

	return CBCGPVisualManager::GetToolbarButtonTextColor (pButton, state);
}
//************************************************************************************
void CBCGPWinXPVisualManager::OnHighlightMenuItem (CDC*pDC, CBCGPToolbarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	if (m_hThemeWindow == NULL || m_bOfficeStyleMenus)
	{
		CBCGPVisualManagerXP::OnHighlightMenuItem (pDC, pButton,	rect, clrText);
	}
	else
	{
		CBCGPVisualManager::OnHighlightMenuItem (pDC, pButton,	rect, clrText);
	}
}
//************************************************************************************
COLORREF CBCGPWinXPVisualManager::GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton)
{
	if (m_hThemeWindow == NULL || m_bOfficeStyleMenus)
	{
		return CBCGPVisualManagerXP::GetHighlightedMenuItemTextColor (pButton);
	}
	else
	{
		return CBCGPVisualManager::GetHighlightedMenuItemTextColor (pButton);
	}
}
//************************************************************************************
void CBCGPWinXPVisualManager::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	if (m_hThemeWindow == NULL || m_bOfficeStyleMenus)
	{
		CBCGPVisualManagerXP::OnHighlightRarelyUsedMenuItems  (pDC, rectRarelyUsed);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	rectRarelyUsed.left --;
	rectRarelyUsed.right = rectRarelyUsed.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * GetMenuImageMargin () + 2;
	dm.HighlightRect (rectRarelyUsed, 94);
}
//************************************************************************************
void CBCGPWinXPVisualManager::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	if (m_hThemeToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawButtonBorder (pDC, pButton, rect, state);
		return;
	}

	if (m_bOfficeStyleMenus)
	{
		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL)
		{
			BOOL bIsMenuBar = pMenuButton->GetParentWnd () != NULL &&
				pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

			BOOL bIsPopupMenu = pMenuButton->GetParentWnd () != NULL &&
				pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

			if (bIsPopupMenu || bIsMenuBar)
			{
				CBCGPVisualManagerXP::OnDrawButtonBorder (pDC, pButton, rect, state);
				return;
			}
		}
	}
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL bHorz)
{
	if (m_hThemeToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawButtonSeparator (pDC, pButton, rect, state, bHorz);
		return;
	}

	rect.InflateRect (2, 2);

	(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), 
		bHorz ? TP_SEPARATOR : TP_SEPARATORVERT,
		0, &rect, 0);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pBar);

	if (m_hThemeToolBar == NULL ||
		pBar->IsDialogControl ())
	{
		CBCGPVisualManagerXP::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	if (m_bOfficeStyleMenus && pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		CBCGPVisualManagerXP::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), 
		bHorz ? TP_SEPARATOR : TP_SEPARATORVERT, 
		0, &rect, 0);
}
//***************************************************************************************
void CBCGPWinXPVisualManager::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, 
								BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
								int nImageID /*= -1*/)
{
	if (m_hThemeToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawCaptionButton (pDC, pButton, bActive, bHorz, bMaximized, bDisabled, nImageID);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT (pButton != NULL);

	BOOL bPushed = pButton->m_bPushed;
	BOOL bFocused = pButton->m_bFocused || pButton->m_bDroppedDown;

	int nState = TS_NORMAL;
	if (bDisabled)
	{
		nState = TS_DISABLED;
	}
	else if (bPushed && bFocused)
	{
		nState = TS_PRESSED;
	}
	else if (bFocused)
	{
		nState = TS_HOT;
	}

	CRect rect = pButton->GetRect ();

	if (pButton->IsMiniFrameButton ())
	{
		rect.bottom -= 3;
		rect.right -= 2;
	}

	int nPart = 0;
	HTHEME hTheme = NULL;

	if (nImageID == -1)
	{
		switch (pButton->m_nHit)
		{
		case HTCLOSE:
		case HTCLOSE_BCG:

			if (pButton->IsMiniFrameButton ())
			{
				nPart = WP_SMALLCLOSEBUTTON;
				hTheme = m_hThemeWindow;
			}
			else
			{
				nPart = EBP_HEADERCLOSE;
				hTheme = m_hThemeExplorerBar;
			}

			break;

		case HTMAXBUTTON:
			nPart = EBP_HEADERPIN;
			hTheme = m_hThemeExplorerBar;

			if (!bMaximized)
			{
				nState += 3;
			}
			break;
		}
	}

	if (nPart == 0 || hTheme == NULL)
	{
		if (!pButton->IsMiniFrameButton ())
		{
			rect.DeflateRect (1, 2);
		}

		(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), TP_BUTTON, nState, &rect, 0);

		CMenuImages::IMAGES_IDS id = (CMenuImages::IMAGES_IDS)-1;
		
		if (nImageID != -1)
		{
			id = (CMenuImages::IMAGES_IDS)nImageID;
		}
		else
		{
			id = pButton->GetIconID (bHorz, bMaximized);
		}

		if (id != (CMenuImages::IMAGES_IDS)-1)
		{
			CSize sizeImage = CMenuImages::Size ();
			CPoint ptImage (rect.left + (rect.Width () - sizeImage.cx) / 2,
							rect.top + (rect.Height () - sizeImage.cy) / 2);

			OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
		}
	}
	else
	{
		(*m_pfDrawThemeBackground) (hTheme, pDC->GetSafeHdc(), nPart, nState, &rect, 0);
	}
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawCaptionButtonIcon (CDC* pDC, 
													CBCGPCaptionButton* pButton,
													CMenuImages::IMAGES_IDS id,
													BOOL bActive, BOOL bDisabled,
													CPoint ptImage)
{
	BOOL bFocused = pButton->m_bFocused || pButton->m_bDroppedDown;

	if (m_hThemeButton == NULL || !bFocused)
	{
		CBCGPVisualManagerXP::OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, 
													bDisabled, ptImage);
		return;
	}

	ASSERT_VALID (pDC);
	CMenuImages::Draw (pDC, id, ptImage, CMenuImages::ImageBlack);
}
//***********************************************************************************
COLORREF CBCGPWinXPVisualManager::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	if (m_hThemeWindow == NULL || m_bOfficeStyleMenus)
	{
		return CBCGPVisualManagerXP::OnFillCommandsListBackground (pDC, rect, bIsSelected);
	}

	::FillRect (pDC->GetSafeHdc (), rect, ::GetSysColorBrush (COLOR_MENU));

	if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brHilite);
		pDC->Draw3dRect (rect, globalData.clrMenuText, globalData.clrMenuText);

		return globalData.clrTextHilite;
	}

	return globalData.clrMenuText;
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	CBCGPVisualManagerXP::OnDrawTearOffCaption (pDC, rect, bIsActive);
}
//***********************************************************************************
void CBCGPWinXPVisualManager::OnDrawMenuSystemButton (CDC* pDC, CRect rect, 
												UINT uiSystemCommand, 
												UINT nStyle, BOOL bHighlight)
{
	if (m_hThemeWindow == NULL)
	{
		CBCGPVisualManagerXP::OnDrawMenuSystemButton (pDC, rect, uiSystemCommand, nStyle, bHighlight);
		return;
	}

	int nPart;
	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		nPart = WP_MDICLOSEBUTTON;
		break;

	case SC_RESTORE:
		nPart = WP_MDIRESTOREBUTTON;
		break;

	case SC_MINIMIZE:
		nPart = WP_MDIMINBUTTON;
		break;

	default:
		return;
	}

	BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
	BOOL bIsPressed = (nStyle & TBBS_PRESSED);

	int nState = CBS_NORMAL;
	if (bIsDisabled)
	{
		nState = CBS_DISABLED;
	}
	else if (bIsPressed && bHighlight)
	{
		nState = CBS_PUSHED;
	}
	else if (bHighlight)
	{
		nState = CBS_HOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), nPart,
		nState, &rect, 0);
}
//********************************************************************************
void CBCGPWinXPVisualManager::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle)
{
	if (m_hThemeStatusBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawStatusBarPaneBorder (pDC, pBar, rectPane, uiID, nStyle);
		return;
	}

	if (!(nStyle & SBPS_NOBORDERS))
	{
		(*m_pfDrawThemeBackground) (m_hThemeStatusBar, pDC->GetSafeHdc(), SP_PANE,
			0, &rectPane, 0);
	}
}
//*****************************************************************************************
void CBCGPWinXPVisualManager::OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
			CRect rectSizeBox)
{
	if (m_hThemeScrollBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawStatusBarSizeBox (pDC, pStatBar, rectSizeBox);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeScrollBar, pDC->GetSafeHdc(), SBP_SIZEBOX,
		SZB_RIGHTALIGN, &rectSizeBox, 0);
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	if (m_hThemeWindow == NULL || m_bOfficeStyleMenus)
	{
		ASSERT_VALID (pMenu);

		BOOL bConnectMenuToParent = m_bConnectMenuToParent;
		m_bConnectMenuToParent = FALSE;

		if (m_hThemeWindow == NULL)
		{
			m_bConnectMenuToParent = TRUE;
		}
		else if (!CBCGPToolBar::IsCustomizeMode ())
		{
			CBCGPToolbarMenuButton* pMenuButton = pMenu->GetParentButton ();

			if (pMenuButton != NULL)
			{
				BOOL bIsMenuBar = pMenuButton->GetParentWnd () != NULL &&
					pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));
				if (bIsMenuBar)
				{
					m_bConnectMenuToParent = TRUE;
				}
			}
		}

		CBCGPVisualManagerXP::OnDrawMenuBorder (pDC, pMenu, rect);

		m_bConnectMenuToParent = bConnectMenuToParent;
	}
	else
	{
		CBCGPVisualManager::OnDrawMenuBorder (pDC, pMenu, rect);
	}
}
//****************************************************************************************
void CBCGPWinXPVisualManager::OnDrawComboDropButton (CDC* pDC, CRect rect,
											    BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (m_hThemeComboBox == NULL)
	{
		CBCGPVisualManagerXP::OnDrawComboDropButton (pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	int nState = bDisabled ? CBXS_DISABLED : bIsDropped ? CBXS_PRESSED : bIsHighlighted ? CBXS_HOT : CBXS_NORMAL;

	(*m_pfDrawThemeBackground) (m_hThemeComboBox, pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, 
		nState, &rect, 0);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (m_hThemeWindow == NULL)
	{
		CBCGPVisualManagerXP::OnDrawComboBorder (pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (&rect,  globalData.clrHilite, globalData.clrHilite);
	}
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pWndTab)
{
	if (!m_b3DTabsXPTheme || m_hThemeTab == NULL || pWndTab->IsFlatTab () || 
		pWndTab->IsOneNoteStyle () || pWndTab->IsVS2005Style ())
	{
		CBCGPVisualManagerXP::OnEraseTabsButton (pDC, rect, pButton, pWndTab);
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
		rectTabs.bottom += 2;
	}
	else
	{
		rectTabs.bottom = rectTabArea.bottom;
	}

	pWndTab->MapWindowPoints (pButton, rectTabs);

	OnEraseTabsArea (pDC, rectTabs, pWndTab);

	pDC->SelectClipRgn (NULL);
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT uiState,
												 CBCGPBaseTabWnd* pWndTab)
{
	if (m_hThemeToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawTabsButtonBorder (pDC, rect, pButton, uiState, pWndTab);
		return;
	}

	int nState = TS_NORMAL;

	if (!pButton->IsWindowEnabled ())
	{
		nState = TS_DISABLED;
	}
	else if (pButton->IsPressed () || pButton->GetCheck ())
	{
		nState = TS_PRESSED;
	}
	else if (pButton->IsHighlighted ())
	{
		nState = TS_HOT;
	}

	globalData.DrawParentBackground (pButton, pDC, rect);

	(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), TP_BUTTON, nState, &rect, 0);
}
//**************************************************************************************
COLORREF CBCGPWinXPVisualManager::OnFillMiniFrameCaption (
											CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd, 
											BOOL bActive)
{
	if (m_hThemeWindow == NULL)
	{
		return CBCGPVisualManager::OnFillMiniFrameCaption (pDC, rectCaption, pFrameWnd, bActive);
	}

	return globalData.clrCaptionText;
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	if (m_hThemeWindow == NULL)
	{
		CBCGPVisualManager::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), 
		WP_SMALLCAPTION,
		0, &rectBorder, 0);
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* pToolBar, 
												CRect rectBorder, CRect rectBorderSize)
{
	if (m_hThemeWindow == NULL)
	{
		CBCGPVisualManager::OnDrawFloatingToolbarBorder (pDC, pToolBar, 
												rectBorder, rectBorderSize);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), 
		WP_SMALLCAPTION,
		0, &rectBorder, 0);
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnFillOutlookPageButton (CDC* pDC, const CRect& rectClient,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText)
{
	if (m_hThemeButton == NULL)
	{
		CBCGPVisualManagerXP::OnFillOutlookPageButton (pDC, rectClient, bIsHighlighted, bIsPressed, clrText);
		return;
	}

	int nState = PBS_NORMAL;
	if (bIsHighlighted)
	{
		nState = PBS_PRESSED;
	}
	else if (bIsPressed)
	{
		nState = PBS_HOT;
	}

	CRect rect = rectClient;
	rect.InflateRect (1, 1);

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON, 
		nState, &rect, 0);
}
//****************************************************************************************
void CBCGPWinXPVisualManager::OnDrawOutlookPageButtonBorder (CDC* pDC, 
							CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	if (m_hThemeButton == NULL)
	{
		CBCGPVisualManagerXP::OnDrawOutlookPageButtonBorder (pDC, rectBtn, bIsHighlighted, bIsPressed);
	}
}
//**************************************************************************************
void CBCGPWinXPVisualManager::OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* pStatusBar,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
			BOOL bProgressText)
{
	if (!DrawStatusBarProgress (pDC, pStatusBar,
			rectProgress, nProgressTotal, nProgressCurr,
			clrBar, clrProgressBarDest, clrProgressText, bProgressText))
	{
		CBCGPVisualManagerXP::OnDrawStatusBarProgress (pDC, pStatusBar,
			rectProgress, nProgressTotal, nProgressCurr,
			clrBar, clrProgressBarDest, clrProgressText, bProgressText);
	}
}
//****************************************************************************************
CSize CBCGPWinXPVisualManager::GetButtonExtraBorder () const
{
	if (m_hThemeWindow == NULL)
	{
		return CBCGPVisualManagerXP::GetButtonExtraBorder ();
	}

	return CSize (2, 2);
}
//****************************************************************************************
CSize CBCGPWinXPVisualManager::GetCaptionButtonExtraBorder () const
{
	if (m_hThemeWindow == NULL)
	{
		return CBCGPVisualManagerXP::GetCaptionButtonExtraBorder ();
	}

	return CSize (1, 1);
}
//****************************************************************************************
void CBCGPWinXPVisualManager::OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
		CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	if (m_hThemeHeader == NULL)
	{
		CBCGPVisualManagerXP::OnDrawHeaderCtrlBorder (pCtrl, pDC, rect, bIsPressed, bIsHighlighted);
		return;
	}

	int nState = HIS_NORMAL;

	if (bIsPressed)
	{
		nState = HIS_PRESSED;
	}
	else if (bIsHighlighted)
	{
		nState = HIS_HOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeHeader, pDC->GetSafeHdc(), 
								HP_HEADERITEM, nState, &rect, 0);
}
//*****************************************************************************************
void CBCGPWinXPVisualManager::OnDrawHeaderCtrlSortArrow (CBCGPHeaderCtrl* pCtrl,
												   CDC* pDC,
												   CRect& rect, BOOL bIsUp)
{
	if (m_hThemeHeader == NULL)
	{
		CBCGPVisualManagerXP::OnDrawHeaderCtrlSortArrow (pCtrl, pDC, rect, bIsUp);
		return;
	}

/* TODO
	int nState = bIsUp ? HSAS_SORTEDUP : HSAS_SORTEDDOWN;

	(*m_pfDrawThemeBackground) (m_hThemeHeader, pDC->GetSafeHdc(), 
								HP_HEADERSORTARROW, nState, &rect, 0);
*/
	#define POINTS_NUM	3
	POINT pts [POINTS_NUM];

	if (bIsUp)
	{
		pts [0].x = rect.left;
		pts [0].y = rect.bottom;

		pts [1].x = rect.CenterPoint ().x;
		pts [1].y = rect.top;

		pts [2].x = rect.right;
		pts [2].y = rect.bottom;
	}
	else
	{
		pts [0].x = rect.left;
		pts [0].y = rect.top;

		pts [1].x = rect.CenterPoint ().x;
		pts [1].y = rect.bottom;

		pts [2].x = rect.right;
		pts [2].y = rect.top;
	}

	CBrush br (globalData.clrBtnShadow);
	CBrush* pOldBrush = pDC->SelectObject (&br);

	CPen* pOldPen = (CPen*) pDC->SelectStockObject (NULL_PEN);

	pDC->Polygon (pts, POINTS_NUM);

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);
}

//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* pButton)
{
	if (m_hThemeWindow == NULL)
	{
		CBCGPVisualManagerXP::OnDrawEditBorder (pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted)
	{
		pDC->Draw3dRect (&rect,  globalData.clrBtnHilite, globalData.clrBtnHilite);
	}
}

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPWinXPVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	ASSERT_VALID (pDC);

	if (m_hThemeExplorerBar == NULL)
	{
		CBCGPVisualManagerXP::OnFillTasksPaneBackground (pDC, rectWorkArea);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_HEADERBACKGROUND,
		0, &rectWorkArea, 0);
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup,
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

	if (m_hThemeExplorerBar == NULL || bIsToolBox)
	{
		CBCGPVisualManagerXP::OnDrawTasksGroupCaption (pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
		return;
	}

	if (pGroup->m_strName.IsEmpty())
	{
		return;
	}

	// -------------------------------
	// Draw group caption (Windows XP)
	// -------------------------------
	if (pGroup->m_bIsSpecial)
	{
		(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_SPECIALGROUPHEAD,
			0, &pGroup->m_rect, 0);
	}
	else
	{
		(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_NORMALGROUPHEAD,
			0, &pGroup->m_rect, 0);
	}

	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL 
		&& pGroup->m_sizeIcon.cx < pGroup->m_rect.Width () - pGroup->m_rect.Height());
	if (bShowIcon)
	{
		OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
	}
	
	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject (&globalData.fontBold);
	COLORREF clrTextOld = pDC->GetTextColor();
	if (bCanCollapse && bIsHighlighted)
	{
		if (pGroup->m_bIsSpecial)
		{
			pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ? 
				globalData.clrWindow : pGroup->m_clrTextHot);
		}
		else
		{
			pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ? 
				globalData.clrHilite : pGroup->m_clrTextHot);
		}
	}
	else
	{
		if (pGroup->m_bIsSpecial)
		{
			pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ? 
				globalData.clrWindow : pGroup->m_clrText);
		}
		else
		{
			pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ? 
				globalData.clrHilite : pGroup->m_clrText);
		}
	}
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);
	
	CRect rectText = pGroup->m_rect;
	rectText.left += (bShowIcon ? pGroup->m_sizeIcon.cx	+ 5: nCaptionHOffset);
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, 
						rectText.right - (bCanCollapse ? pGroup->m_rect.Height() : nCaptionHOffset));

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_END_ELLIPSIS);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse)
	{
		CRect rectButton = pGroup->m_rect;
		rectButton.left = max(rectButton.left, rectButton.right - rectButton.Height());
		
		if (pGroup->m_bIsSpecial)
		{
			if (!pGroup->m_bIsCollapsed)
			{
				if (bIsHighlighted)
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_SPECIALGROUPCOLLAPSE, EBSGC_HOT, &rectButton, 0);
				}
				else
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_SPECIALGROUPCOLLAPSE, EBSGC_NORMAL, &rectButton, 0);
				}
			}
			else
			{
				if (bIsHighlighted)
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_SPECIALGROUPEXPAND, EBSGE_HOT, &rectButton, 0);
				}
				else
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_SPECIALGROUPEXPAND, EBSGE_NORMAL, &rectButton, 0);
				}
			}
		}
		else
		{
			if (!pGroup->m_bIsCollapsed)
			{
				if (bIsHighlighted)
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_NORMALGROUPCOLLAPSE, EBNGC_HOT, &rectButton, 0);
				}
				else
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_NORMALGROUPCOLLAPSE, EBNGC_NORMAL, &rectButton, 0);
				}
			}
			else
			{
				if (bIsHighlighted)
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_NORMALGROUPEXPAND, EBNGE_HOT, &rectButton, 0);
				}
				else
				{
					(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
						EBP_NORMALGROUPEXPAND, EBNGE_NORMAL, &rectButton, 0);
				}
			}
		}
	}
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial)
{
	ASSERT_VALID(pDC);

	if (m_hThemeExplorerBar == NULL)
	{
		CBCGPVisualManagerXP::OnFillTasksGroupInterior (pDC, rect);
		return;
	}

	if (!bSpecial)
	{
		(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
			EBP_NORMALGROUPBACKGROUND, 0, &rect, 0);
	}
	else
	{
		(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
			EBP_SPECIALGROUPBACKGROUND, 0, &rect, 0);
	}
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial, 
														BOOL bNoTitle)
{
	if (m_hThemeExplorerBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawTasksGroupAreaBorder(pDC, rect, bSpecial, bNoTitle);
		return;
	}

	ASSERT_VALID(pDC);

	// Draw underline
	if (bNoTitle)
	{
		CRect rectDraw = rect;
		rectDraw.bottom = rectDraw.top + 1;

		if (bSpecial)
		{
			(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_SPECIALGROUPHEAD,
				0, &rectDraw, 0);
		}
		else
		{
			(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_NORMALGROUPHEAD,
				0, &rectDraw, 0);
		}
	}
	
	return;
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
										BOOL bIsHighlighted, BOOL bIsSelected)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	if (m_hThemeExplorerBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawTask (pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
		return;
	}

	if (pTask->m_bIsSeparator)
	{
		// --------------
		// Draw separator
		// --------------
		CRect rectDraw = pTask->m_rect;
		rectDraw.top = pTask->m_rect.CenterPoint ().y;
		rectDraw.bottom = rectDraw.top + 1;

		// draw same as group caption
		(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), EBP_NORMALGROUPHEAD,
			0, &rectDraw, 0);
		return;
	}
	
	// ---------
	// Draw icon
	// ---------
	CRect rectText = pTask->m_rect;
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
	COLORREF clrTextOld = pDC->GetTextColor();
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
			globalData.clrHilite : pTask->m_clrTextHot);
	}
	else
	{
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrHilite : pTask->m_clrText);
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
		pDC->DrawText (pTask->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);
	}

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//************************************************************************************
void CBCGPWinXPVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
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

#endif // BCGP_EXCLUDE_TASK_PANE

void CBCGPWinXPVisualManager::OnDrawExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox)
{
	ASSERT_VALID(pDC);

	if (m_hThemeTree == NULL)
	{
		CBCGPVisualManagerXP::OnDrawExpandingBox (pDC, rect, bIsOpened, colorBox);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeTree, pDC->GetSafeHdc(), TVP_GLYPH,
		bIsOpened ? GLPS_OPENED : GLPS_CLOSED, &rect, 0);
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawCheckBoxEx (CDC *pDC, CRect rect, 
										 int nState,
										 BOOL bHighlighted, 
										 BOOL bPressed,
										 BOOL bEnabled)
{
	if (!DrawCheckBox (pDC, rect, bHighlighted, nState, bEnabled, bPressed))
	{
		CBCGPVisualManagerXP::OnDrawCheckBoxEx (pDC, rect, nState, bHighlighted, bPressed, bEnabled);
	}
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawControlBorder (CWnd* pWndCtrl)
{
	if (m_hThemeComboBox == NULL)
	{
		CBCGPVisualManagerXP::OnDrawControlBorder (pWndCtrl);
		return;
	}

	ASSERT_VALID (pWndCtrl);

	CWindowDC dc (pWndCtrl);

	CRect rect;
	pWndCtrl->GetWindowRect (rect);

	rect.bottom -= rect.top;
	rect.right -= rect.left;
	rect.left = rect.top = 0;

	COLORREF clrBorder = (COLORREF)-1;

	if ((*m_pfGetThemeColor) (m_hThemeComboBox, 5, 0, 3801, &clrBorder) != S_OK)
	{
		CBCGPVisualManagerXP::OnDrawControlBorder (pWndCtrl);
		return;
	}

	dc.Draw3dRect (&rect, clrBorder, clrBorder);
	rect.DeflateRect (1, 1);
	dc.Draw3dRect (rect, globalData.clrWindow, globalData.clrWindow);
}
//********************************************************************************
BOOL CBCGPWinXPVisualManager::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* pButton, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int cmd /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* pCalculator)
{
	if (m_hThemeButton == NULL)
	{
		return CBCGPVisualManagerXP::OnDrawCalculatorButton (pDC, rect, pButton, state, cmd, pCalculator);
	}

	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);

	int nState = PBS_NORMAL;

	switch (state)
	{
	case ButtonsIsPressed:
		nState = PBS_PRESSED;
		break;

	case ButtonsIsHighlighted:
		nState = PBS_HOT;
		break;
	}

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON, 
		nState, &rect, 0);
	
	return TRUE;
}
//*********************************************************************************
BOOL CBCGPWinXPVisualManager::OnDrawBrowseButton (CDC* pDC, CRect rect, 
											 CBCGPEdit* pEdit, 
											 CBCGPVisualManager::BCGBUTTON_STATE state,
											 COLORREF& clrText)
{
	if (m_hThemeButton == NULL)
	{
		return CBCGPVisualManagerXP::OnDrawBrowseButton (pDC, rect, 
			pEdit, state, clrText);
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brWindow);

	int nState = PBS_NORMAL;

	switch (state)
	{
	case ButtonsIsPressed:
		nState = PBS_PRESSED;
		break;

	case ButtonsIsHighlighted:
		nState = PBS_HOT;
		break;
	}

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON,
		nState, &rect, 0);
	
	return TRUE;
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawSpinButtons (CDC* pDC, CRect rect, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl)
{
	if (m_hThemeSpin == NULL)
	{
		CBCGPVisualManagerXP::OnDrawSpinButtons (pDC, rect, 
			nState, bOrientation, pSpinCtrl);
		return;
	}

	// Draw up part:
	CRect rectUp = rect;
	rectUp.bottom = rect.CenterPoint ().y - 1;

	int nDrawState = UPS_NORMAL;

	if (nState & SPIN_DISABLED)
	{
		nDrawState = UPS_DISABLED;
	}
	else if (nState & SPIN_PRESSEDUP)
	{
		nDrawState = UPS_PRESSED;
	}
	else if (nState & SPIN_HIGHLIGHTEDUP)
	{
		nDrawState = UPS_HOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeSpin, pDC->GetSafeHdc(), 
		bOrientation ? SPNP_UPHORZ : SPNP_UP, nDrawState, &rectUp, 0);

	// Draw up part:
	CRect rectDown = rect;
	rectDown.top = rect.CenterPoint ().y;

	nDrawState = UPS_NORMAL;

	if (nState & SPIN_DISABLED)
	{
		nDrawState = UPS_DISABLED;
	}
	else if (nState & SPIN_PRESSEDDOWN)
	{
		nDrawState = UPS_PRESSED;
	}
	else if (nState & SPIN_HIGHLIGHTEDDOWN)
	{
		nDrawState = UPS_HOT;
	}

	(*m_pfDrawThemeBackground) (m_hThemeSpin, pDC->GetSafeHdc(), 
		bOrientation ? SPNP_DOWNHORZ : SPNP_DOWN, nDrawState, &rectDown, 0);
}
//*************************************************************************************
void CBCGPWinXPVisualManager::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (!m_b3DTabsXPTheme || m_hThemeTab == NULL || pTabWnd->IsFlatTab () || 
		pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style () ||
		pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManagerXP::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	int nState = TIS_NORMAL;
	if (bIsActive)
	{
		nState = TIS_SELECTED;
	}
	else if (iTab == pTabWnd->GetHighlightedTab ())
	{
		nState = TIS_HOT;
	}

	rectTab.right++;

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_TOP && !bIsActive)
	{
		rectTab.bottom--;
	}

	(*m_pfDrawThemeBackground) (m_hThemeTab, pDC->GetSafeHdc(), TABP_TABITEM, nState, &rectTab, 0);

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		CBCGPDrawManager dm (*pDC);
		dm.MirrorRect (rectTab, FALSE);
	}

	COLORREF clrTabText = globalData.clrWindowText;
	if (m_pfGetThemeColor != NULL)
	{
		(*m_pfGetThemeColor) (m_hThemeTab, TABP_TABITEM, nState, TMT_TEXTCOLOR, &clrTabText);
	}
	
	COLORREF cltTextOld = pDC->SetTextColor (clrTabText);

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, (COLORREF)-1);

	pDC->SetTextColor (cltTextOld);
}
//***********************************************************************************
void CBCGPWinXPVisualManager::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (!m_b3DTabsXPTheme || m_hThemeTab == NULL || pTabWnd->IsFlatTab () || 
		pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style () ||
		pTabWnd->IsDialogControl ())
	{
		CBCGPVisualManagerXP::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	rect.right += 10;

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		rect.top -= 3;

		CBCGPMemDC memDC (*pDC, (CWnd*) pTabWnd);

		(*m_pfDrawThemeBackground) (m_hThemeTab, memDC.GetDC ().GetSafeHdc (), TABP_PANE, 0, &rect, 
			NULL);

		CBCGPDrawManager dm (memDC.GetDC ());
		dm.MirrorRect (rect, FALSE);
	}
	else
	{
		rect.bottom += 2;

		(*m_pfDrawThemeBackground) (m_hThemeTab, pDC->GetSafeHdc(), TABP_PANE, 0, &rect, 0);
	}
}
//***********************************************************************************
BOOL CBCGPWinXPVisualManager::OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (!m_b3DTabsXPTheme || m_hThemeTab == NULL || pTabWnd->IsFlatTab () || 
		pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ())
	{
		return CBCGPVisualManagerXP::OnEraseTabsFrame (pDC, rect, pTabWnd);
	}

	return FALSE;
}

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPWinXPVisualManager::OnErasePopupWindowButton (CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (m_hThemeButton == NULL || pButton->IsCaptionButton ())
	{
		CBCGPVisualManagerXP::OnErasePopupWindowButton (pDC, rect, pButton);
		return;
	}

	CRect rectParent;
	pButton->GetParent ()->GetClientRect (rectParent);

	pButton->GetParent ()->MapWindowPoints (pButton, rectParent);
	OnFillPopupWindowBackground (pDC, rectParent);
}
//**********************************************************************************
void CBCGPWinXPVisualManager::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pButton);

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
	else
	{
		nState = PBS_NORMAL;
	}

	if (m_hThemeWindow != NULL && pButton->IsCloseButton () && pButton->IsCaptionButton ())
	{
		(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), 
			WP_SMALLCLOSEBUTTON, nState, &rect, 0);
		return;
	}

	if (m_hThemeButton == NULL)
	{
		CBCGPVisualManagerXP::OnDrawPopupWindowButtonBorder (pDC, rect, pButton);
		return;
	}

	globalData.DrawParentBackground (pButton, pDC, rect);
	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, &rect, 0);
}
//**********************************************************************************
BOOL CBCGPWinXPVisualManager::IsDefaultWinXPPopupButton (CBCGPPopupWndButton* pButton) const
{
	ASSERT_VALID (pButton);
	return m_hThemeWindow != NULL && pButton->IsCloseButton () && pButton->IsCaptionButton ();
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW

#ifndef BCGP_EXCLUDE_GRID_CTRL

void CBCGPWinXPVisualManager::OnFillGridHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		m_hThemeHeader == NULL)
	{
		CBCGPVisualManagerXP::OnFillGridHeaderBackground (pCtrl, pDC, rect);
		return;
	}

	ASSERT_VALID(pDC);
	pDC->FillRect (rect, &globalData.brBtnFace);
}
//********************************************************************************
BOOL CBCGPWinXPVisualManager::OnDrawGridHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		m_hThemeHeader == NULL)
	{
		return CBCGPVisualManagerXP::OnDrawGridHeaderItemBorder (pCtrl, pDC, rect, bPressed);
	}

	ASSERT_VALID (pDC);
	(*m_pfDrawThemeBackground) (m_hThemeHeader, pDC->GetSafeHdc(), 
								HP_HEADERITEM, HIS_NORMAL, &rect, 0);
	return TRUE;
}

#endif // BCGP_EXCLUDE_GRID_CTRL

void CBCGPWinXPVisualManager::GetCalendarColors (const CBCGPCalendar* pCalendar,
				   CBCGPCalendarColors& colors)
{
	CBCGPVisualManagerXP::GetCalendarColors (pCalendar, colors);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		m_clrCalendarCaption == (COLORREF)-1 ||
		m_clrCalendarCaptionText == (COLORREF)-1)
	{
		return;
	}

	colors.clrCaption = m_clrCalendarCaption;
	colors.clrCaptionText = m_clrCalendarCaptionText;
}
//**************************************************************************************
COLORREF CBCGPWinXPVisualManager::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
			BOOL bActive, CRect rectCaption, CRect rectButtons)
{
	if (m_hThemeExplorerBar == NULL || m_pfGetThemeColor == NULL)
	{
		return CBCGPVisualManagerXP::OnDrawControlBarCaption (pDC, pBar, 
			bActive, rectCaption, rectButtons);
	}

	(*m_pfDrawThemeBackground) (m_hThemeExplorerBar, pDC->GetSafeHdc(), 
		EBP_HEADERBACKGROUND,
		0, &rectCaption, 0);

	COLORREF clrText;
	if ((*m_pfGetThemeColor) (m_hThemeExplorerBar, EBP_HEADERBACKGROUND, 0, TMT_TEXTCOLOR, &clrText) != S_OK)
	{
		clrText = globalData.clrInactiveCaptionText;
	}

	if (bActive)
	{
		CBCGPDrawManager dm (*pDC);
		int nPercentage = 110;

		if (GetRValue (clrText) > 128 &&
			GetGValue (clrText) > 128 &&
			GetBValue (clrText) > 128)
		{
			nPercentage = 80;
		}

		dm.HighlightRect (rectCaption, nPercentage);
	}

	return clrText;
}

#ifndef BCGP_EXCLUDE_TOOLBOX

BOOL CBCGPWinXPVisualManager::OnEraseToolBoxButton (CDC* pDC, CRect rect,
											CBCGPToolBoxButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (m_hThemeToolBar == NULL)
	{
		return CBCGPVisualManagerXP::OnEraseToolBoxButton (pDC, rect, pButton);
	}

	if (pButton->IsHighlighted () || pButton->GetCheck ())
	{
		int nState = TS_NORMAL;
		
		if (pButton->GetCheck ())
		{
			nState = (pButton->IsHighlighted ()) ? TS_HOTCHECKED : TS_CHECKED;
		}
		else if (pButton->IsHighlighted ())
		{
			nState = TS_HOT;
		}

		(*m_pfDrawThemeBackground) (m_hThemeToolBar, pDC->GetSafeHdc(), TP_BUTTON, nState, &rect, 0);
	}

	return TRUE;
}
//**********************************************************************************
BOOL CBCGPWinXPVisualManager::OnDrawToolBoxButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPToolBoxButton* pButton, UINT uiState)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (m_hThemeToolBar == NULL)
	{
		return CBCGPVisualManagerXP::OnDrawToolBoxButtonBorder (pDC, rect, pButton, uiState);
	}

	return TRUE;
}

#endif
