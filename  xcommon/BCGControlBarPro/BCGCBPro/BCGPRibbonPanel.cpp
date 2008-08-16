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
// BCGPRibbonPanel.cpp: implementation of the CBCGPRibbonPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPDrawManager.h"
#include "bcgglobals.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPVisualManager.h"
#include "BCGPToolBar.h"
#include "BCGPRibbonButtonsGroup.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonLabel.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonLaunchButton

CBCGPRibbonLaunchButton::CBCGPRibbonLaunchButton ()
{
	m_pParentPanel = NULL;
}
//****************************************************************************
void CBCGPRibbonLaunchButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pParentPanel);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonLaunchButton (
		pDC, this, m_pParentPanel);
}
//****************************************************************************
CSize CBCGPRibbonLaunchButton::GetRegularSize (CDC* /*pDC*/)
{
	return CSize (0, 0);
}
//****************************************************************************
void CBCGPRibbonLaunchButton::OnClick (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		m_pParentMenu->OnClickButton (this, point);
		return;
	}

	NotifyCommand ();
}

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonDefaultPanelButton

IMPLEMENT_DYNCREATE(CBCGPRibbonDefaultPanelButton, CBCGPRibbonButton)

CBCGPRibbonDefaultPanelButton::CBCGPRibbonDefaultPanelButton (CBCGPRibbonPanel* pPanel)
{
	m_hIcon = NULL;
	m_pPanel = pPanel;
}
//****************************************************************************
void CBCGPRibbonDefaultPanelButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonDefaultPaneButton 
		(pDC, this);
}
//****************************************************************************
void CBCGPRibbonDefaultPanelButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonDefaultPanelButton& src = (CBCGPRibbonDefaultPanelButton&) s;

	m_pPanel = src.m_pPanel;
	m_pParent = src.m_pParent;

	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		m_strToolTip = m_pPanel->GetName ();
	}
}
//****************************************************************************
void CBCGPRibbonDefaultPanelButton::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);
	
	CBCGPBaseRibbonElement::OnLButtonDown (point);
	OnShowPopupMenu ();
}
//****************************************************************************
void CBCGPRibbonDefaultPanelButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pPanel);

	CWnd* pWndParent = GetParentWnd ();
	if (pWndParent == NULL)
	{
		return;
	}

	if (m_pPanel->m_arWidths.GetSize () == NULL)
	{
		ASSERT_VALID (m_pPanel);
		ASSERT_VALID (m_pPanel->m_pParent);

		CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
		ASSERT_VALID (pRibbonBar);

		CClientDC dc (pRibbonBar);

		CFont* pOldFont = dc.SelectObject (pRibbonBar->GetFont ());
		ASSERT (pOldFont != NULL);

		int nHeight = m_pPanel->m_pParent->GetMaxHeight (&dc);
		m_pPanel->RecalcWidths (&dc, nHeight);

		dc.SelectObject (pOldFont);
	}

	CRect rectBtn = m_rect;
	pWndParent->ClientToScreen (&rectBtn);

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (m_pPanel);
	pMenu->SetParentRibbonElement (this);

	pMenu->Create (pWndParent, rectBtn.left, rectBtn.bottom, (HMENU) NULL);
	SetDroppedDown (pMenu);
}
//********************************************************************************
void CBCGPRibbonDefaultPanelButton::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL bIsSelected,
									  BOOL bHighlighted)
{
	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectText = rect;

	rectText.left += nTextOffset;
	rectText.DeflateRect (m_szMargin.cx, 0);

	pDC->DrawText (strText, rectText, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	m_bIsDisabled = bIsDisabled;

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonDefaultPaneButtonIndicator
		(pDC, this, rect, bIsSelected, bHighlighted);
}
//*****************************************************************************
void CBCGPRibbonDefaultPanelButton::DrawImage (CDC* pDC, 
	RibbonImageType type, CRect rectImage)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPRibbonDefaultPanelButton* pOrigButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonDefaultPanelButton, m_pOriginal);

	if (pOrigButton != NULL)
	{
		ASSERT_VALID (pOrigButton);

		pOrigButton->DrawImage (pDC, type, rectImage);
		return;
	}

	if (m_hIcon == NULL)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawDefaultRibbonImage (
			pDC, rectImage);
		return;
	}

	CSize sizeIcon = globalData.m_sizeSmallIcon;

	pDC->DrawState (
		CPoint (
			rectImage.CenterPoint ().x - sizeIcon.cx / 2,
			rectImage.CenterPoint ().y - sizeIcon.cy / 2),
		sizeIcon, m_hIcon, DSS_NORMAL, (HBRUSH) NULL);
}

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonPanel

UINT CBCGPRibbonPanel::m_nNextPanelID = (UINT)-10;

IMPLEMENT_DYNCREATE(CBCGPRibbonPanel, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4355)

CBCGPRibbonPanel::CBCGPRibbonPanel(LPCTSTR lpszName, HICON hIcon) :
	m_btnDefault (this)
{
	CommonInit (lpszName, hIcon);
}
//****************************************************************************
CBCGPRibbonPanel::CBCGPRibbonPanel (CBCGPRibbonPaletteButton* pPaletteButton) :
	m_btnDefault (this)
{
	CommonInit ();

	ASSERT_VALID (pPaletteButton);
	m_pPaletteButton = pPaletteButton;
}
//****************************************************************************
void CBCGPRibbonPanel::CopyFrom (CBCGPRibbonPanel& src)
{
	m_strName = src.m_strName;
	m_dwData = src.m_dwData;
	m_pParent = src.m_pParent;
	m_nXMargin = src.m_nXMargin;
	m_nYMargin = src.m_nYMargin;
	m_bShowCaption = src.m_bShowCaption;
	m_bAlignByColumn = src.m_bAlignByColumn;
	m_bJustifyColumns = src.m_bJustifyColumns;

	int i = 0;

	for (i = 0; i < src.m_arWidths.GetSize (); i++)
	{
		m_arWidths.Add (src.m_arWidths [i]);
	}

	for (i = 0; i < src.m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pSrcElem = src.m_arElements [i];
		ASSERT_VALID (pSrcElem);

		CBCGPBaseRibbonElement* pElem =
			(CBCGPBaseRibbonElement*) pSrcElem->GetRuntimeClass ()->CreateObject ();
		ASSERT_VALID (pElem);

		pElem->CopyFrom (*pSrcElem);
		pElem->SetOriginal (pSrcElem);

		m_arElements.Add (pElem);
	}

	m_btnLaunch.CopyFrom (src.m_btnLaunch);
	m_btnLaunch.SetOriginal (&src.m_btnLaunch);
}
//****************************************************************************
void CBCGPRibbonPanel::CommonInit (LPCTSTR lpszName, HICON hIcon)
{
	m_strName = lpszName != NULL ? lpszName : _T("");
	m_dwData = 0;
	m_btnDefault.m_hIcon = hIcon;
	m_btnDefault.SetText (m_strName);
	m_btnDefault.SetID (m_nNextPanelID--);

	m_rect.SetRectEmpty ();
	m_pParent = NULL;
	m_pParentMenuBar = NULL;
	m_nCurrWidthIndex = 0;
	m_nFullWidth = 0;
	m_nRows = 0;
	m_nXMargin = 2;
	m_nYMargin = 2;
	m_bShowCaption = FALSE;
	m_bIsHighlighted = FALSE;
	m_bIsCalcWidth = FALSE;
	m_pHighlighted = NULL;
	m_bAlignByColumn = TRUE;
	m_bFloatyMode = FALSE;
	m_bIsQATPopup = FALSE;
	m_bMenuMode = FALSE;
	m_bIsFirst = TRUE;
	m_bIsLast = TRUE;
	m_rectCaption.SetRectEmpty ();
	m_pPaletteButton = NULL;
	m_rectMenuArea.SetRectEmpty ();
	m_pScrollBar = NULL;
	m_nScrollOffset = 0;
	m_bSizeIsLocked = FALSE;
	m_bJustifyColumns = FALSE;
}

#pragma warning (default : 4355)

CBCGPRibbonPanel::~CBCGPRibbonPanel()
{
	CBCGPBaseRibbonElement* pDroppedDown = GetDroppedDown ();
	if (pDroppedDown != NULL)
	{
		ASSERT_VALID (pDroppedDown);
		pDroppedDown->ClosePopupMenu ();
	}

	RemoveAll ();
}
//********************************************************************************
void CBCGPRibbonPanel::EnableLaunchButton (UINT uiCmdID,
										   int nIconIndex)
{
	ASSERT_VALID (this);
	m_btnLaunch.SetID (uiCmdID);
	m_btnLaunch.m_nSmallImageIndex = nIconIndex;
}
//********************************************************************************
void CBCGPRibbonPanel::Add (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	pElem->SetParentCategory (m_pParent);

	if (!pElem->IsAlignByColumn () && m_bAlignByColumn)
	{
		//---------------------------------------------------
		// If 2 or more elements are alihned by row, set this
		// flag for whole panel:
		//---------------------------------------------------
		for (int i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pListElem = m_arElements [i];
			ASSERT_VALID (pListElem);

			if (!pListElem->IsAlignByColumn ())
			{
				m_bAlignByColumn = FALSE;
				break;
			}
		}
	}

	m_arElements.Add (pElem);
}
//********************************************************************************
void CBCGPRibbonPanel::AddSeparator ()
{
	ASSERT_VALID (this);

	CBCGPRibbonSeparator* pSeparator = new CBCGPRibbonSeparator;
	ASSERT_VALID (pSeparator);

	pSeparator->m_pParent = m_pParent;
	m_arElements.Add (pSeparator);
}
//********************************************************************************
BOOL CBCGPRibbonPanel::AddToolBar (	UINT uiToolbarResID, UINT uiColdResID,
									UINT uiHotResID, UINT uiDisabledResID)
{
	ASSERT_VALID (this);

	//-------------------------------------------
	// Create temporary toolbar and load bitmaps:
	//-------------------------------------------
	CBCGPToolBar wndToolbar;
	if (!wndToolbar.LoadToolBar (uiToolbarResID, uiColdResID, 0, 
								TRUE, uiDisabledResID, 0, uiHotResID))
	{
		return FALSE;
	}

	CBCGPToolBarImages* pImages = wndToolbar.GetLockedImages ();
	CBCGPToolBarImages* pColdImages = wndToolbar.GetLockedColdImages ();
	CBCGPToolBarImages* pDisabledImages = wndToolbar.GetLockedDisabledImages ();
	CBCGPToolBarImages* pHotImages = NULL;

	if (pColdImages != NULL && pColdImages->GetCount () > 0)
	{
		pHotImages = uiHotResID != 0 ? pImages : NULL;
		pImages = pColdImages;
	}

	CList<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	lstButtons;

	for (int i = 0; i < wndToolbar.GetCount (); i++)
	{
		CBCGPToolbarButton* pToolbarButton = wndToolbar.GetButton (i);
		ASSERT_VALID (pToolbarButton);

		if (pToolbarButton->m_nStyle & TBBS_SEPARATOR)
		{
			if (!lstButtons.IsEmpty ())
			{
				CBCGPRibbonButtonsGroup* pGroup = new CBCGPRibbonButtonsGroup;

				pGroup->AddButtons (lstButtons);
				pGroup->SetImages (pImages, pHotImages, pDisabledImages);

				Add (pGroup);

				lstButtons.RemoveAll ();
			}
		}
		else
		{
			CBCGPRibbonButton* pButton = new CBCGPRibbonButton;

			pButton->SetID (pToolbarButton->m_nID);
			pButton->SetText (pToolbarButton->m_strText);	// TODO
			pButton->m_nSmallImageIndex = pToolbarButton->GetImage ();

			lstButtons.AddTail (pButton);
		}
	}

	if (!lstButtons.IsEmpty ())
	{
		CBCGPRibbonButtonsGroup* pGroup = new CBCGPRibbonButtonsGroup;

		pGroup->AddButtons (lstButtons);
		pGroup->SetImages (pImages, pHotImages, pDisabledImages);

		Add (pGroup);
	}

	return TRUE;
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetElement (int nIndex) const
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_arElements [nIndex];
}
//********************************************************************************
int CBCGPRibbonPanel::GetCount () const
{
	ASSERT_VALID (this);
	return (int) m_arElements.GetSize ();
}
//********************************************************************************
BOOL CBCGPRibbonPanel::Remove (int nIndex, BOOL bDelete)
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize ())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPBaseRibbonElement* pElem = m_arElements [nIndex];
	ASSERT_VALID (pElem);

	m_arElements.RemoveAt (nIndex);

	if (bDelete)
	{
		delete pElem;
	}

	return TRUE;
}
//********************************************************************************
void CBCGPRibbonPanel::RemoveAll ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		delete m_arElements [i];
	}

	m_arElements.RemoveAll ();
	m_bAlignByColumn = TRUE;
}
//********************************************************************************
void CBCGPRibbonPanel::DoPaint (CDC* pDC)
{
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	CRect rectInter;

	if (!rectInter.IntersectRect (m_rect, rectClip))
	{
		return;
	}

	COLORREF clrTextOld = pDC->GetTextColor ();

	//-----------------------
	// Fill panel background:
	//-----------------------
	COLORREF clrText = m_bFloatyMode || m_pPaletteButton != NULL ? 
		globalData.clrBarText :
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonPanel (
		pDC, this, m_rect, m_rectCaption);

	//--------------------
	// Draw panel caption:
	//--------------------
	if (!m_rectCaption.IsRectEmpty () &&
		rectInter.IntersectRect (m_rectCaption, rectClip))
	{
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonPanelCaption (
			pDC, this, m_rectCaption);
	}

	//--------------------
	// Draw launch button:
	//--------------------
	if (rectInter.IntersectRect (m_btnLaunch.GetRect (), rectClip))
	{
		m_btnLaunch.OnDraw (pDC);
	}

	pDC->SetTextColor (clrText);

	if (!m_btnDefault.GetRect ().IsRectEmpty ())
	{
		//----------------------------------------------
		// Panel is collapsed, draw default button only:
		//----------------------------------------------
		if (rectInter.IntersectRect (m_btnDefault.GetRect (), rectClip))
		{
			m_btnDefault.OnDraw (pDC);
		}
	}
	else if (m_pPaletteButton != NULL)
	{
		OnDrawPaletteMenu (pDC);
	}
	else
	{
		//---------------------
		// Draw panel elements:
		//---------------------
		for (int i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (rectInter.IntersectRect (pElem->GetRect (), rectClip))
			{
				pDC->SetTextColor (clrText);
				pElem->OnDraw (pDC);
			}
		}
	}

	pDC->SetTextColor (clrTextOld);
}
//********************************************************************************
void CBCGPRibbonPanel::OnDrawPaletteMenu (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pPaletteButton);

	int i = 0;

	CRect rectIcons = m_rect;
	CRect rectSeparator;
	rectSeparator.SetRectEmpty ();

	if (!m_rectMenuArea.IsRectEmpty ())
	{
		if (m_pParentMenuBar != NULL)
		{
			BOOL bDisableSideBarInXPMode = m_pParentMenuBar->m_bDisableSideBarInXPMode;
			m_pParentMenuBar->m_bDisableSideBarInXPMode = FALSE;

			CBCGPVisualManager::GetInstance ()->OnFillBarBackground (
				pDC, m_pParentMenuBar, m_rectMenuArea, m_rectMenuArea);

			m_pParentMenuBar->m_bDisableSideBarInXPMode = bDisableSideBarInXPMode;
		}

		rectSeparator = m_rectMenuArea;
		rectSeparator.top--;
		rectSeparator.bottom = rectSeparator.top + 1;

		rectIcons.bottom = m_rectMenuArea.top - 1;
	}

	CRgn rgnClip;

	rgnClip.CreateRectRgnIndirect (rectIcons);
	pDC->SelectClipRgn (&rgnClip);

	CBCGPDrawState ds;

	m_pPaletteButton->m_imagesPalette.SetTransparentColor (globalData.clrBtnFace);
	m_pPaletteButton->m_imagesPalette.PrepareDrawImage (ds);

	// First, draw icons + labels:
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (bIsLabel || bIsIcon)
		{
			pElem->OnDraw (pDC);
		}
	}

	pDC->SelectClipRgn (NULL);

	// Draw rest of elements:
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (!bIsLabel && !bIsIcon)
		{
			pElem->OnDraw (pDC);
		}
	}

	if (!rectSeparator.IsRectEmpty ())
	{
		CBCGPVisualManager::GetInstance ()->OnDrawSeparator (pDC, 
			m_pParentMenuBar, rectSeparator, FALSE);
	}

	m_pPaletteButton->m_imagesPalette.EndDrawImage (ds);
}
//********************************************************************************
int CBCGPRibbonPanel::GetHeight (CDC* pDC) const
{
	ASSERT_VALID (pDC);

	int nMaxHeight = ((CBCGPRibbonPanel*)this)->m_btnDefault.GetRegularSize (pDC).cy;

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);

		nMaxHeight = max (nMaxHeight, pElem->GetRegularSize (pDC).cy);
	}

	return nMaxHeight + 2 * m_nYMargin + 1;
}
//********************************************************************************
void CBCGPRibbonPanel::Repos (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_rectCaption.SetRectEmpty ();
	m_rectMenuArea.SetRectEmpty ();

	if (m_pPaletteButton != NULL)
	{
		ReposPalette (pDC, rect);
		return;
	}

	if (m_bMenuMode)
	{
		ReposMenu (pDC, rect);
		return;
	}

	m_btnDefault.m_pParent = m_pParent;
	m_btnLaunch.m_pParent = m_pParent;
	m_btnLaunch.m_pParentPanel = this;

	m_btnDefault.OnCalcTextSize (pDC);
	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).cx;

	m_rect = rect;

	m_btnLaunch.SetRect (CRect (0, 0, 0, 0));

	if (rect.Width () <= cxDefaultButton + m_nXMargin && !m_bIsCalcWidth)
	{
		ShowDefaultButton (pDC);
		return;
	}

	m_btnDefault.SetRect (CRect (0, 0, 0, 0));

	m_nFullWidth = 0;
	m_nRows = 0;
	m_bShowCaption = TRUE;

	const CSize sizeCaption = GetCaptionSize (pDC);

	m_rect.right = m_rect.left + max (rect.Width (), sizeCaption.cx);

	CSize size = rect.Size ();
	size.cx -= m_nXMargin;
	size.cy -= sizeCaption.cy + m_nYMargin;

	//--------------------------------------------------
	// First, set all elements to the regular size mode:
	//--------------------------------------------------
	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);
		pElem->m_bCompactMode = m_bFloatyMode;
		pElem->m_nRow = -1;
	}

	BOOL bIsCompacted = m_bFloatyMode;

	//----------------------------------------------
	// Now, try to repos all elements inside bounds:
	//----------------------------------------------
	int nFirstInColumnIndex = -1;
	int x = 0;
	int y = 0;

	if (!m_bAlignByColumn || m_bFloatyMode)
	{
		int nRowHeight = 0;
		int nBottom = 0;
		int nFirstInRow = 0;
		int xStart = 0;
		int i = 0;
		BOOL bIsFullHeight = (m_rect.bottom == 32767);

		//-----------------------------
		// Put all large buttons first:
		//-----------------------------
		BOOL bPrevLarge = FALSE;
		BOOL bPrevSeparator = FALSE;

		CSize sizePrevLargeButton (0, 0);

		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CSize sizeElem = pElem->GetSize (pDC);

			BOOL bIsLargeButton = 
				pElem->GetRegularSize (pDC) == pElem->GetCompactSize (pDC) &&
				pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) != CSize (0, 0) &&
				!m_bFloatyMode;

			BOOL bDrawSeparator = FALSE;

			if (pElem->IsSeparator ())
			{
				bDrawSeparator = bPrevLarge && !bPrevSeparator;
			}

			if (bIsLargeButton || bDrawSeparator)
			{
				if (pElem->IsSeparator ())
				{
					if (sizePrevLargeButton != CSize (0, 0))
					{
						sizeElem.cy = sizePrevLargeButton.cy;
					}
				}
				else
				{
					sizePrevLargeButton = sizeElem;
				}

				CRect rectElem (
					CPoint (rect.left + x + m_nXMargin, rect.top + m_nYMargin), 
					CSize (sizeElem.cx, bIsFullHeight ? sizeElem.cy : size.cy));

				pElem->SetRect (rectElem);
				pElem->m_nRow = 999;

				x += sizeElem.cx;
				xStart = x;
				y = 0;
			}

			bPrevLarge = bIsLargeButton;
			bPrevSeparator = bDrawSeparator;
		}

		for (i = 0; i < m_arElements.GetSize ();)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				i++;
				continue;
			}

			if (pElem->m_nRow != -1)
			{
				//--------------------
				// Already positioned
				//--------------------
				i++;
				continue;
			}

			if (pElem->IsSeparator ())
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				i++;
				continue;
			}

			if (x + sizeElem.cx > size.cx)
			{
				//------------------------------
				// We should start next row now:
				//------------------------------

				if (x == xStart)
				{
					ShowDefaultButton (pDC);
					return;
				}

				//----------------------------------------
				// Set same height for all elemnts in row:
				//----------------------------------------
				for (int j = nFirstInRow; j < m_arElements.GetSize (); j++)
				{
					CBCGPBaseRibbonElement* pRowElem = m_arElements [j];
					ASSERT_VALID (pRowElem);

					if (pRowElem->m_nRow == m_nRows)
					{
						CRect rectElem = pRowElem->GetRect ();

						if (pElem->CanBeStretched ())
						{
							rectElem.bottom = rectElem.top + nRowHeight;
						}
						else
						{
							int yOffset = max (0, (nRowHeight - rectElem.Height ()) / 2);
							rectElem.OffsetRect (0, yOffset);
						}

						pRowElem->SetRect (rectElem);
					}
				}

				y += nRowHeight;

				if (m_bFloatyMode)
				{
					y += m_nYMargin;
				}

				m_nRows++;

				x = xStart;
				nRowHeight = 0;
				nFirstInRow = i;
			}

			if (y + sizeElem.cy > size.cy)
			{
				//------------------------------------------
				// Cannot repos elemnts: panel is too small:
				//------------------------------------------
				ShowDefaultButton (pDC);
				return;
			}

			BOOL bGoToNext = TRUE;

			if (x == xStart)
			{
				//-----------------------------------------------------
				// First element in row, find widest non-ready elemnet:
				//-----------------------------------------------------
				int nMaxWidth = -1;
				int nWidestIndex = -1;

				for (int j = nFirstInRow; j < m_arElements.GetSize (); j++)
				{
					CBCGPBaseRibbonElement* pCurrElem = m_arElements [j];
					ASSERT_VALID (pCurrElem);

					if (pCurrElem->m_nRow == -1)
					{
						CSize sizeCurr = pCurrElem->GetSize (pDC);

						if (sizeCurr.cx > nMaxWidth)
						{
							nMaxWidth = sizeCurr.cx;
							nWidestIndex = j;
						}
					}
				}

				if (nWidestIndex >= 0)
				{
					pElem = m_arElements [nWidestIndex];
					sizeElem = pElem->GetSize (pDC);

					bGoToNext = FALSE;
				}
			}

			CRect rectElem (CPoint (rect.left + x + m_nXMargin, rect.top + y + m_nYMargin), 
							sizeElem);

			pElem->SetRect (rectElem);
			pElem->m_nRow = m_nRows;

			nRowHeight = max (nRowHeight, sizeElem.cy);
			x += sizeElem.cx;

			m_nFullWidth = max (m_nFullWidth, x);
			
			nBottom = max (nBottom, rectElem.bottom);

			if (bGoToNext)
			{
				i++;
			}
		}

		m_nRows++;
		m_nFullWidth += m_nXMargin;

		if (bIsFullHeight)
		{
			m_rect.bottom = nBottom + sizeCaption.cy + m_nYMargin;
			size.cy = m_rect.Height () - sizeCaption.cy - m_nYMargin;
		}
		else if (m_nRows > 1 && !m_bFloatyMode)
		{
			//-----------------------------
			// Add some space between rows:
			//-----------------------------
			int yOffset = (m_rect.top + size.cy - nBottom) / (2 * m_nRows);
			if (yOffset > 0)
			{
				for (i = 0; i < m_arElements.GetSize (); i++)
				{
					CBCGPBaseRibbonElement* pElem = m_arElements [i];
					ASSERT_VALID (pElem);

					int nRow = pElem->m_nRow;
					CRect rectElem = pElem->GetRect ();

					if (nRow > 0 && nRow != 999 && !rectElem.IsRectEmpty ())
					{
						rectElem.OffsetRect (0, yOffset * nRow);
						pElem->SetRect (rectElem);
					}
				}
			}
		}

		if (m_bIsQATPopup && nRowHeight > 0 && m_arElements.GetSize () > 0)
		{
			//--------------------------------------------------------------------
			// Last element (customize button) should occopy the whole row height:
			//--------------------------------------------------------------------
			CBCGPBaseRibbonElement* pElem = m_arElements [m_arElements.GetSize () - 1];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->GetRect ();
			rectElem.bottom = rectElem.top + nRowHeight;

			pElem->SetRect (rectElem);
		}
	}
	else
	{
		int nColumnWidth = 0;

		for (int i = 0; i < m_arElements.GetSize ();)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				i++;
				continue;
			}

			if (pElem->IsSeparator ())
			{
				int nNewColumnWidth = AdjustColumn (pDC,
					i - 1, sizeCaption.cy);
				if (nNewColumnWidth >= 0)
				{
					nColumnWidth = nNewColumnWidth;
				}

				x += nColumnWidth;

				CRect rectSeparator (
					CPoint (rect.left + x + m_nXMargin, rect.top + m_nYMargin), 
					CSize (sizeElem.cx, size.cy));

				pElem->SetRect (rectSeparator);

				x += sizeElem.cx;
				y = 0;
				nColumnWidth = 0;
				nFirstInColumnIndex = -1;
				i++;
				continue;
			}

			if (pElem->IsWholeRowHeight ())
			{
				sizeElem.cy = size.cy;
			}

			if (y + sizeElem.cy > size.cy)
			{
				//---------------------------------
				// We should start next column now:
				//---------------------------------

				if (y == 0)
				{
					ShowDefaultButton (pDC);
					return;
				}

				int nNewColumnWidth = AdjustColumn (pDC, i - 1, sizeCaption.cy);
				if (nNewColumnWidth >= 0)
				{
					nColumnWidth = nNewColumnWidth;
				}

				nColumnWidth = 0;
				int xColumn = -1;

				for (int iColumn = i - 1; iColumn >= 0; iColumn--)
				{
					CBCGPBaseRibbonElement* pColumnElem = m_arElements [iColumn];
					ASSERT_VALID (pColumnElem);

					CSize sizeElem = pColumnElem->GetSize (pDC);

					if (sizeElem == CSize (0, 0))
					{
						continue;
					}

					if (xColumn == -1)
					{
						xColumn = pColumnElem->GetRect ().left;
					}
					else if (xColumn != pColumnElem->GetRect ().left)
					{
						break;
					}

					nColumnWidth = max (nColumnWidth, sizeElem.cx);
				}

				x += nColumnWidth;
				y = 0;

				nColumnWidth = 0;
				nFirstInColumnIndex = -1;
			}

			if (x + sizeElem.cx > size.cx)
			{
				//------------------------------
				// Not enough space, try to
				// set elemnets to compact mode:
				//------------------------------
				if (pElem->m_bCompactMode)
				{
					//------------------------
					// Already in compact mode
					//------------------------

					if (i == 0)
					{
						//------------------------------------------
						// Cannot repos elemnts: panel is too small:
						//------------------------------------------
						ShowDefaultButton (pDC);
						return;
					}

					//-------------------------------
					// Return to last regular element 
					// and set its mode to compact:
					//-------------------------------
					while (i > 0)
					{
						i--;

						pElem = m_arElements [i];
						ASSERT_VALID (pElem);

						if (!pElem->m_bCompactMode)
						{
							pElem->m_bCompactMode = TRUE;
							bIsCompacted = TRUE;
							
							x = pElem->m_rect.left - m_rect.left - m_nXMargin;
							y = pElem->m_rect.top - m_rect.top - m_nYMargin;

							int iFirst = i;

							for (iFirst = i; iFirst >= 0; iFirst--)
							{
								if (m_arElements [iFirst]->m_rect.left != pElem->m_rect.left)
								{
									iFirst++;
									break;
								}
							}

							AdjustColumn (pDC, i, sizeCaption.cy);
							break;
						}
					}

					if (i == 0)
					{
						pElem = m_arElements [0];
						ASSERT_VALID (pElem);

						if (pElem->m_bCompactMode)
						{
							ShowDefaultButton (pDC);
							return;
						}
					}
				}
				else
				{
					pElem->m_bCompactMode = TRUE;
					bIsCompacted = TRUE;
				}

				continue;
			}

			if (y == 0 && nFirstInColumnIndex == -1)
			{
				//--------------------------------
				// This element is first in column
				//--------------------------------
				nFirstInColumnIndex = i;
			}

			CRect rectElem (CPoint (rect.left + x + m_nXMargin, rect.top + y + m_nYMargin), 
							sizeElem);
			pElem->SetRect (rectElem);

			nColumnWidth = max (nColumnWidth, sizeElem.cx);
			y += sizeElem.cy;

			i++;
		}

		//--------------------------
		// Save non-optimized width:
		//--------------------------
		m_nFullWidth = CalcTotalWidth ();

		//-----------------------------------------------------------------
		// Maybe, last element in the previous column has regular mode and
		// can be inserted in the last column as compact?
		//-----------------------------------------------------------------
		if (nFirstInColumnIndex > 0 && bIsCompacted)
		{
			CBCGPBaseRibbonElement* pPrevElem = 
				m_arElements [nFirstInColumnIndex - 1];
			ASSERT_VALID (pPrevElem);

			if (!pPrevElem->m_bCompactMode)
			{
				const int xPrev = pPrevElem->m_rect.left;

				CSize sizePrevCompact = pPrevElem->GetCompactSize (pDC);

				BOOL bIsPrevSingleInColumn = TRUE;

				if (nFirstInColumnIndex > 1)
				{
					CBCGPBaseRibbonElement* pPrevElem2 = 
						m_arElements [nFirstInColumnIndex - 2];
					ASSERT_VALID (pPrevElem2);

					bIsPrevSingleInColumn = pPrevElem2->m_rect.left < xPrev;
				}

				if (bIsPrevSingleInColumn && y + sizePrevCompact.cy <= size.cy)
				{
					pPrevElem->m_bCompactMode = TRUE;

					CRect rectPrevElem (CPoint (xPrev, rect.top), sizePrevCompact);
					pPrevElem->SetRect (rectPrevElem);

					y = rectPrevElem.bottom;

					for (int i = nFirstInColumnIndex; i < m_arElements.GetSize (); i++)
					{
						CBCGPBaseRibbonElement* pColumnElem = m_arElements [i];
						ASSERT_VALID (pColumnElem);

						CRect rectElem = pColumnElem->m_rect;
						CSize sizeElem = rectElem.Size ();

						pColumnElem->SetRect (CRect (CPoint (xPrev, y), sizeElem));

						y += sizeElem.cy;
					}

					nFirstInColumnIndex--;
				}
			}
		}

		//--------------------
		// Adjust last column:
		//--------------------
		AdjustColumn (pDC, (int) m_arElements.GetSize () - 1, sizeCaption.cy);
	}

	if (m_bFloatyMode)
	{
		return;
	}

	if (!m_bAlignByColumn)
	{
		//--------------------------------
		// Center all elements vertically:
		//--------------------------------
		CRect rectElements = m_rect;
		rectElements.top += m_nYMargin;
		rectElements.bottom = rectElements.top + size.cy;

		CenterVert (rectElements);
	}
	else
	{
		//--------------------------------------------
		// Center all elements vertically (by column):
		//--------------------------------------------
		int nFirstInColumnIndex = -1;
		int nLastInColumnIndex = -1;
		int x = -1;

		for (int i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->m_rect;
			if (rectElem.IsRectEmpty ())
			{
				continue;
			}

			if (nFirstInColumnIndex == -1)
			{
				nLastInColumnIndex = nFirstInColumnIndex = i;
				x = rectElem.left;
			}

			if (x != rectElem.left)
			{
				CenterElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex, sizeCaption.cy);

				nLastInColumnIndex = nFirstInColumnIndex = i;
				x = rectElem.left;
			}
			else
			{
				nLastInColumnIndex = i;
			}
		}

		CenterElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex, sizeCaption.cy);
	}

	int nTotalWidth = !m_bAlignByColumn || m_bFloatyMode ? m_nFullWidth : CalcTotalWidth ();

	if (nTotalWidth < sizeCaption.cx)
	{
		//--------------------------------------------
		// Panel is too narrow: center it horizontaly:
		//--------------------------------------------
		const int xOffset = (sizeCaption.cx - nTotalWidth) / 2;

		for (int i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->m_rect;
			rectElem.OffsetRect (xOffset, 0);

			pElem->SetRect (rectElem);
		}

		nTotalWidth = sizeCaption.cx;
	}

	if (nTotalWidth < cxDefaultButton)
	{
		m_rect.right = m_rect.left + cxDefaultButton + m_nXMargin;
	}
	else
	{
		m_rect.right = m_rect.left + nTotalWidth + 2 * m_nXMargin;
	}

	//-----------------------------
	// Set launch button rectangle:
	//-----------------------------
	if (m_btnLaunch.GetID () > 0)
	{
		CRect rectLaunch = m_rect;

		rectLaunch.DeflateRect (1, 1);

		rectLaunch.top = rectLaunch.bottom - sizeCaption.cy + 1;
		rectLaunch.left = rectLaunch.right - sizeCaption.cy + 1;

		m_btnLaunch.SetRect (rectLaunch);
	}

	//-----------------------
	// Set caption rectangle:
	//-----------------------
	if (m_bShowCaption)
	{
		m_rectCaption = m_rect;
		m_rectCaption.top = m_rectCaption.bottom - sizeCaption.cy - 1;
	}
}
//********************************************************************************
void CBCGPRibbonPanel::ReposMenu (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_nXMargin = 0;
	m_nYMargin = 0;

	CSize size = rect.Size ();

	int y = 0;
	int i = 0;

	int nImageWidth = 0;

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		nImageWidth = m_pParent->GetImageSize (TRUE).cx;
	}

	int nColumnWidth = 0;

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);
		pElem->SetCompactMode (FALSE);
		pElem->SetTextAlwaysOnRight ();

		CSize sizeElem = pElem->GetSize (pDC);

		if (sizeElem == CSize (0, 0))
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
			continue;
		}

		if (!rect.IsRectEmpty ())
		{
			sizeElem.cx = rect.Width ();
		}

		CRect rectElem = CRect
			(CPoint (rect.left + m_nXMargin, rect.top + y + m_nYMargin), 
			sizeElem);

		pElem->SetRect (rectElem);

		nColumnWidth = max (nColumnWidth, sizeElem.cx);
		y += sizeElem.cy;
	}

	m_nFullWidth = nColumnWidth;

	if (rect.IsRectEmpty ())
	{
		//----------------------------------------------
		// All menu elements should have the same width:
		//----------------------------------------------
		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->GetRect ();

			if (!rectElem.IsRectEmpty ())
			{
				rectElem.right = rectElem.left + nColumnWidth;
				
				if (nImageWidth > 0 && 
					pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
				{
					rectElem.left += nImageWidth + m_nXMargin;
				}

				pElem->SetRect (rectElem);
			}

			pElem->OnAfterChangeRect (pDC);
		}
	}

	m_rect = rect;
	m_rect.bottom = m_rect.top + y;
	m_rect.right = m_rect.left + m_nFullWidth;
}
//********************************************************************************
void CBCGPRibbonPanel::ReposPalette (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pScrollBar);
	ASSERT_VALID (m_pPaletteButton);

	if (rect == CRect (0, 0, 0, 0))
	{
		return;
	}

	BOOL bShowAllItems = FALSE;

	m_nScrollOffset = 0;

	CRect rectInitial = rect;

	if (rectInitial.bottom <= 0)
	{
		rectInitial.bottom = rectInitial.top + 32676;
		bShowAllItems = TRUE;
	}

	m_nXMargin = 0;
	m_nYMargin = 0;

	const int cxScroll = bShowAllItems ? 0 : ::GetSystemMetrics (SM_CXVSCROLL);

	int nScrollTotal = 0;

	int x = rectInitial.left;
	int y = rectInitial.top;

	m_rect = rectInitial;

	if (m_bSizeIsLocked)
	{
		rectInitial.right -= cxScroll;
	}
	else
	{
		m_rect.right += cxScroll;
	}

	int i = 0;
	BOOL bHasBottomItems = FALSE;

	if (m_bSizeIsLocked)
	{
		// First, find all bottom items and place them on bottom:

		int yBottom = rectInitial.bottom;
		m_rectMenuArea = m_rect;

		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
			BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

			if (bIsIcon || bIsLabel)
			{
				continue;
			}

			bHasBottomItems = TRUE;

			pElem->OnCalcTextSize (pDC);
			pElem->SetCompactMode (TRUE);

			if (pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) == CSize (0, 0))
			{
				pElem->SetCompactMode (FALSE);
			}

			pElem->SetTextAlwaysOnRight ();

			CSize sizeElem = pElem->GetSize (pDC);
			sizeElem.cx = m_rect.Width ();

			CRect rectElem = CRect
				(CPoint (rectInitial.left, yBottom - sizeElem.cy), 
				sizeElem);

			pElem->SetRect (rectElem);

			yBottom -= sizeElem.cy;
			rectInitial.bottom = yBottom - 4;
			m_rectMenuArea.top = yBottom;
		}
	}

	// Set palette icons location:
	int yNextLine = m_rect.bottom;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (!bIsIcon && !bIsLabel)
		{
			bHasBottomItems = TRUE;
		}

		CSize sizeElem (0, 0);

		if (bIsLabel)
		{
			if (x > rectInitial.left)
			{
				y = yNextLine;
			}

			if (i > 0)
			{
				y++;
			}

			pElem->OnCalcTextSize (pDC);

			sizeElem = pElem->GetSize (pDC);
			sizeElem.cx = rectInitial.Width ();

			CRect rectElem = CRect
				(CPoint (rectInitial.left, y), sizeElem);

			pElem->SetRect (rectElem);

			if (!m_bSizeIsLocked)
			{
				m_rect.bottom = rectElem.bottom;
			}

			nScrollTotal = yNextLine = rectElem.bottom;

			y += sizeElem.cy + 1;
			x = rectInitial.left;
		}
		else if (bIsIcon)
		{
			pElem->SetCompactMode (FALSE);

			sizeElem = pElem->GetSize (pDC);

			if (x + sizeElem.cx > rectInitial.right && x != rectInitial.left)
			{
				x = rectInitial.left;
				y += sizeElem.cy;
			}

			CRect rectElem = CRect (CPoint (x, y), sizeElem);

			pElem->SetRect (rectElem);

			if (!m_bSizeIsLocked)
			{
				m_rect.bottom = rectElem.bottom;
			}

			nScrollTotal = yNextLine = rectElem.bottom;

			x += sizeElem.cx;
		}
	}

	if (!m_bSizeIsLocked)
	{
		m_rect.bottom = min (m_rect.bottom, rectInitial.bottom);
	}

	if (!bShowAllItems)
	{
		m_pScrollBar->SetWindowPos (NULL, 
			m_rect.right - cxScroll, m_rect.top,
			cxScroll, rectInitial.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);

		SCROLLINFO si;

		ZeroMemory (&si, sizeof (SCROLLINFO));
		si.cbSize = sizeof (SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;

		si.nMin = 0;

		if (nScrollTotal > rectInitial.Height ())
		{
			si.nMax = nScrollTotal;
			si.nPage = rectInitial.Height ();

			m_pScrollBar->SetScrollInfo (&si, TRUE);
			m_pScrollBar->EnableScrollBar (ESB_ENABLE_BOTH);
			m_pScrollBar->EnableWindow ();
		}
		else
		{
			m_pScrollBar->EnableScrollBar (ESB_DISABLE_BOTH);
		}
	}

	m_nFullWidth = m_rect.Width ();

	if (bHasBottomItems && !m_bSizeIsLocked)
	{
		// Set menu items location (on bottom):
		y = m_rect.bottom + 4;

		m_rectMenuArea = m_rect;
		m_rectMenuArea.top = y;

		int nMaxItemHeight = 0;
		int nMaxImageWidth = 0;
		
		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)) ||
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
			{
				continue;
			}

			pElem->OnCalcTextSize (pDC);
			pElem->SetCompactMode (TRUE);

			if (pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) == CSize (0, 0))
			{
				pElem->SetCompactMode (FALSE);
			}

			pElem->SetTextAlwaysOnRight ();

			CSize sizeElem = pElem->GetSize (pDC);

			nMaxItemHeight = max (nMaxItemHeight, sizeElem.cy);
			nMaxImageWidth = max (nMaxImageWidth, 
				pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageSmall).cx);
		}

		if (nMaxImageWidth == 0)
		{
			nMaxImageWidth = 16;
		}

		// Now, make all items with the same width and image offset:

		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)) ||
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
			{
				continue;
			}

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			pElem->m_nImageOffset = nMaxImageWidth;

			sizeElem.cx = m_rect.Width ();
			sizeElem.cy = nMaxItemHeight;

			CRect rectElem = CRect
				(CPoint (rectInitial.left, rectInitial.top + y), 
				sizeElem);

			pElem->SetRect (rectElem);
			m_rect.bottom = rectElem.bottom;

			y += sizeElem.cy;
		}

		m_rectMenuArea.bottom = y;
	}
}
//********************************************************************************
void CBCGPRibbonPanel::ShowDefaultButton (CDC* pDC)
{
	//-------------------------------
	// Show panel default button only
	//-------------------------------
	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).cx;

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->SetRect (CRect (0, 0, 0, 0));
	}

	m_rect.right = m_rect.left + cxDefaultButton;

	m_btnDefault.SetRect (m_rect);
	m_nRows = 0;
	m_bShowCaption = FALSE;
}
//********************************************************************************
int CBCGPRibbonPanel::CalcTotalWidth ()
{
	//------------------------------------------------------------
	// Total width will be right edge of the last visible element
	// in the right column
	//------------------------------------------------------------
	int xRight = 0;

	for (int i = (int) m_arElements.GetSize () - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->m_rect.IsRectEmpty ())
		{
			continue;
		}

		xRight = max (xRight, pElem->m_rect.right);
	}

	return max (0, xRight - m_rect.left);
}
//********************************************************************************
void CBCGPRibbonPanel::CenterVert (CRect rectElements)
{
	int yMin = rectElements.bottom;
	int yMax = rectElements.top;

	int i = 0;

	for (i = (int) m_arElements.GetSize () - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->m_rect.IsRectEmpty ())
		{
			continue;
		}

		yMin = min (yMin, pElem->m_rect.top);
		yMax = max (yMax, pElem->m_rect.bottom);
	}

	int dyTop = yMin - rectElements.top;
	int dyBottom = rectElements.bottom - yMax;

	const int yOffset = (dyBottom - dyTop) / 2 - 1;

	if (yOffset == 0)
	{
		// Already centered
		return;
	}

	for (i = (int) m_arElements.GetSize () - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (!pElem->m_rect.IsRectEmpty ())
		{
			pElem->m_rect.OffsetRect (0, yOffset);
		}
	}
}
//********************************************************************************
CSize CBCGPRibbonPanel::GetCaptionSize (CDC* pDC) const
{
	ASSERT_VALID (pDC);

	if (m_bFloatyMode)
	{
		return CSize (0, 0);
	}

	CSize size = pDC->GetTextExtent (m_strName);

	size.cy += m_nYMargin;
	size.cx += 4 * m_nXMargin;

	if (m_btnLaunch.GetID () > 0)
	{
		size.cx += m_nXMargin + size.cy;
	}

	return size;
}
//********************************************************************************
int CBCGPRibbonPanel::AdjustColumn (CDC* pDC,
									int nLastInColumnIndex,
									int nCaptionHeight)
{
	if (m_arElements.GetSize () == 0)
	{
		return 0;
	}

	int nFirstInColumnIndex = -1;
	int xColumn = -1;

	for (int iColumn = nLastInColumnIndex; iColumn >= 0; iColumn--)
	{
		CBCGPBaseRibbonElement* pColumnElem = m_arElements [iColumn];
		ASSERT_VALID (pColumnElem);

		CSize sizeElem = pColumnElem->GetSize (pDC);

		if (sizeElem == CSize (0, 0))
		{
			continue;
		}

		if (xColumn == -1)
		{
			xColumn = pColumnElem->GetRect ().left;
		}
		else if (xColumn != pColumnElem->GetRect ().left)
		{
			break;
		}

		nFirstInColumnIndex = iColumn;
	}

	CBCGPBaseRibbonElement* pColumnElem = NULL;

	//---------------------------
	// Find last visible element:
	//---------------------------
	while (nLastInColumnIndex > nFirstInColumnIndex)
	{
		pColumnElem = m_arElements [nLastInColumnIndex - 1];

		if (!pColumnElem->GetRect ().IsRectEmpty ())
		{
			break;
		}

		nLastInColumnIndex--;
	}

	if (nLastInColumnIndex < nFirstInColumnIndex)
	{
		return 0;
	}

	int nColumnWidth = -1;
	int nColumnHeight = m_rect.Height () - nCaptionHeight - 2 * m_nYMargin;

	//-----------------------------------------------------
	// Adjust current column:
	// ----------------------
	// 1. Set all elemnts to the same mode (normal/compact)
	// 2. Center all elements vertically
	//-----------------------------------------------------
	int i = 0;

	BOOL bIsCompactMode = FALSE;

	//----------------------------------
	// Are any elements in compact mode?
	//----------------------------------
	for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
	{
		pColumnElem = m_arElements [i];
		ASSERT_VALID (pColumnElem);

		if (!pColumnElem->GetRect ().IsRectEmpty () &&
			pColumnElem->m_bCompactMode)
		{
			bIsCompactMode = TRUE;
			break;
		}
	}

	if (bIsCompactMode)
	{
		//-------------------------
		// Set all to compact mode:
		//-------------------------
		nColumnWidth = 0;
		int y = -1;

		for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
		{
			pColumnElem = m_arElements [i];
			ASSERT_VALID (pColumnElem);

			CRect rectElem = pColumnElem->m_rect;

			if (i == nFirstInColumnIndex)
			{
				y = rectElem.top;
			}

			pColumnElem->m_bCompactMode = TRUE;

			CSize sizeColumnElem = pColumnElem->GetSize (pDC);

			rectElem.top = y;
			rectElem.right = rectElem.left + sizeColumnElem.cx;
			rectElem.bottom = rectElem.top + sizeColumnElem.cy;

			pColumnElem->SetRect (rectElem);

			nColumnWidth = max (nColumnWidth, pColumnElem->m_rect.Width ());

			y += sizeColumnElem.cy;
		}
	}

	if (nLastInColumnIndex == nFirstInColumnIndex)
	{
		//---------------------------------------------------------------
		// Column has 1 large element only - stretch it to the whole row:
		//---------------------------------------------------------------
		pColumnElem = m_arElements [nFirstInColumnIndex];
		ASSERT_VALID (pColumnElem);

		pColumnElem->StretchToWholeRow (pDC, nColumnHeight);
	}
	else if (m_bJustifyColumns)
	{
		//-------------------------------------------------
		// Set same width (largets) to all column elements:
		//-------------------------------------------------
		nColumnWidth = 0;

		for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
		{
			pColumnElem = m_arElements [i];
			ASSERT_VALID (pColumnElem);

			nColumnWidth = max (nColumnWidth, pColumnElem->m_rect.Width ());
		}

		for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
		{
			pColumnElem = m_arElements [i];
			ASSERT_VALID (pColumnElem);

			CRect rectElem = pColumnElem->m_rect;
			rectElem.right = rectElem.left + nColumnWidth;

			pColumnElem->SetRect (rectElem);
		}
	}

	return nColumnWidth;
}
//********************************************************************************
void CBCGPRibbonPanel::CenterElementsInColumn (int nFirstInColumnIndex, int nLastInColumnIndex,
											   int nCaptionHeight)
{
	if (nFirstInColumnIndex > nLastInColumnIndex ||
		nFirstInColumnIndex < 0 ||
		nLastInColumnIndex < 0)
	{
		return;
	}

	//------------------------------------------
	// Center all elements in column vertically:
	//------------------------------------------
	CBCGPBaseRibbonElement* pLastElem = m_arElements [nLastInColumnIndex];
	ASSERT_VALID (pLastElem);

	const int nColumnHeight = m_rect.Height () - nCaptionHeight - 2 * m_nYMargin;
	const int nTotalHeight = pLastElem->m_rect.bottom - m_rect.top - m_nYMargin;
	const int nOffset = max (0, (nColumnHeight - nTotalHeight) / 2);

	for (int i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
	{
		CBCGPBaseRibbonElement* pColumnElem = m_arElements [i];
		ASSERT_VALID (pColumnElem);

		CRect rectElem = pColumnElem->m_rect;
		rectElem.OffsetRect (0, nOffset);
		pColumnElem->SetRect (rectElem);
	}
}
//********************************************************************************
void CBCGPRibbonPanel::RecalcWidths (CDC* pDC, int nHeight)
{
	ASSERT_VALID (pDC);

	CRect rectScreen;
	::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);

	m_btnDefault.OnCalcTextSize (pDC);

	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).cx;
	const CSize sizeCaption = GetCaptionSize (pDC);
	const int nCaptionWidth = sizeCaption.cx;

	m_arWidths.RemoveAll ();
	m_nCurrWidthIndex = 0;
	m_bIsCalcWidth = TRUE;

	int nLastWidth = -1;
	const int dx = 16;

	if (m_bAlignByColumn && !m_bFloatyMode)
	{
		CRect rect (0, 0, 32767, nHeight);

		do
		{
			Repos (pDC, rect);

			if (nLastWidth == -1 || m_nFullWidth < nLastWidth)
			{
				nLastWidth = m_nFullWidth;

				if (nLastWidth < cxDefaultButton)
				{
					break;
				}

				m_arWidths.Add (nLastWidth);

				if (nLastWidth <= nCaptionWidth)
				{
					break;
				}

				rect.right = nLastWidth - dx;
			}
			else
			{
				rect.right -= dx;
			}
		}
		while (rect.Width () > 2 * m_nXMargin);
	}
	else
	{
		const int nMaxRows = 20;

		for (int nRows = 1; nRows < nMaxRows; nRows++)
		{
			CRect rect (0, 0, cxDefaultButton + 1, nHeight);

			for (;; rect.right += dx)
			{
				if (rect.Width () >= rectScreen.Width ())
				{
					break;
				}

				Repos (pDC, rect);

				if (nLastWidth != -1 && m_nFullWidth > nLastWidth)
				{
					break;
				}

				if (m_nRows == nRows && m_nFullWidth > 0)
				{
					if (m_nRows == 2 && !m_bFloatyMode)
					{
						//------------------------
						// Don't add 1 row layout:
						//------------------------
						m_arWidths.RemoveAll ();
					}

					m_arWidths.Add (m_nFullWidth);
					nLastWidth = m_nFullWidth;
					break;
				}
			}
		}
	}

	m_arWidths.Add (cxDefaultButton);

	m_bIsCalcWidth = FALSE;
}
//********************************************************************************
int CBCGPRibbonPanel::GetMinWidth (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_btnDefault.OnCalcTextSize (pDC);
	return m_btnDefault.GetRegularSize (pDC).cx;
}
//********************************************************************************
void CBCGPRibbonPanel::Highlight (BOOL bHighlight, CPoint point)
{
	ASSERT_VALID (this);

	BOOL bRedrawPanel = m_bIsHighlighted != bHighlight;

	m_bIsHighlighted = bHighlight;

	CBCGPBaseRibbonElement* pHighlighted = NULL;
	if (bHighlight)
	{
		pHighlighted = HitTest (point);

		if (pHighlighted != NULL)
		{
			ASSERT_VALID (pHighlighted);
			pHighlighted->OnMouseMove (point);
		}
	}

	BOOL bNotifyParent = FALSE;

	if (pHighlighted != m_pHighlighted)
	{
		if (m_pParent != NULL && m_pParent->GetParentRibbonBar () != NULL)
		{
			m_pParent->GetParentRibbonBar ()->PopTooltip ();
		}

		if (m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);
			m_pParentMenuBar->PopTooltip ();
		}

		if (m_pHighlighted != NULL)
		{
			ASSERT_VALID (m_pHighlighted);
			m_pHighlighted->m_bIsHighlighted = FALSE;
			m_pHighlighted->m_bIsFocused = FALSE;

			RedrawElement (m_pHighlighted);
			m_pHighlighted = NULL;
		}

		bNotifyParent = TRUE;
	}

	if (pHighlighted != NULL)
	{
		ASSERT_VALID (pHighlighted);

		m_pHighlighted = pHighlighted;
		m_pHighlighted->m_bIsHighlighted = TRUE;

		RedrawElement (m_pHighlighted);
	}

	if (bRedrawPanel && m_pParent != NULL && GetParentWnd () != NULL)
	{
		GetParentWnd ()->RedrawWindow (m_rect);
	}

	if (m_bFloatyMode && bRedrawPanel)
	{
		ASSERT_VALID (m_pParentMenuBar);
		m_pParentMenuBar->SetActive (m_bIsHighlighted);
	}

	if (bNotifyParent)
	{
		if (m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);
			m_pParentMenuBar->OnChangeHighlighted (m_pHighlighted);
		}
	}
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::HitTest (CPoint point, BOOL bCheckPanelCaption)
{
	if (!m_btnDefault.m_rect.IsRectEmpty () &&
		m_btnDefault.m_rect.PtInRect (point))
	{
		return &m_btnDefault;
	}

	if (!m_btnLaunch.m_rect.IsRectEmpty () &&
		m_btnLaunch.m_rect.PtInRect (point))
	{
		return &m_btnLaunch;
	}

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (!pElem->m_rect.IsRectEmpty () &&
			pElem->m_rect.PtInRect (point))
		{
			return pElem->HitTest (point);
		}
	}

	if (bCheckPanelCaption && m_rectCaption.PtInRect (point))
	{
		return &m_btnDefault;
	}

	return NULL;
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::MouseButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		m_pHighlighted->m_bIsPressed = TRUE;

		RedrawElement (m_pHighlighted);

		HWND hwndMenu = m_pParentMenuBar->GetSafeHwnd ();

		m_pHighlighted->OnLButtonDown (point);

		if (hwndMenu != NULL && !::IsWindow (hwndMenu))
		{
			return NULL;
		}
	}

	return m_pHighlighted;
}
//********************************************************************************
void CBCGPRibbonPanel::MouseButtonUp (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		HWND hwndParent = GetParentWnd ()->GetSafeHwnd ();

		CBCGPBaseRibbonElement* pHighlighted = m_pHighlighted;
		m_pHighlighted->OnLButtonUp (point);

		if (::IsWindow (hwndParent) && pHighlighted->m_bIsPressed)
		{
			pHighlighted->m_bIsPressed = FALSE;
			RedrawElement (m_pHighlighted);
		}
	}
}
//********************************************************************************
void CBCGPRibbonPanel::CancelMode ()
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		m_pHighlighted->m_bIsHighlighted = FALSE;
		m_pHighlighted->m_bIsPressed = FALSE;
		m_pHighlighted->m_bIsFocused = FALSE;

		RedrawElement (m_pHighlighted);
		m_pHighlighted = NULL;
	}
}
//********************************************************************************
void CBCGPRibbonPanel::OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI,
									  CFrameWnd* pTarget,
									  BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnUpdateCmdUI (pCmdUI, pTarget, bDisableIfNoHndler);
	}

	m_btnLaunch.OnUpdateCmdUI (pCmdUI, pTarget, bDisableIfNoHndler);
}
//********************************************************************************
BOOL CBCGPRibbonPanel::NotifyControlCommand (
	BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->NotifyControlCommand (bAccelerator, nNotifyCode, wParam, lParam))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//*********************************************************************************
void CBCGPRibbonPanel::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnShow (!pElem->GetRect ().IsRectEmpty ());
		pElem->OnAfterChangeRect (pDC);
	}

	m_btnDefault.OnShow (!m_btnDefault.GetRect ().IsRectEmpty ());
	m_btnDefault.OnAfterChangeRect (pDC);

	m_btnLaunch.OnAfterChangeRect (pDC);
}
//*********************************************************************************
void CBCGPRibbonPanel::OnShow (BOOL bShow)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnShow (bShow);
	}
}
//******************************************************************************
BOOL CBCGPRibbonPanel::IsCollapsed () const
{
	ASSERT_VALID (this);

	return !m_btnDefault.GetRect ().IsRectEmpty ();
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::FindByID (UINT uiCmdID) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		CBCGPBaseRibbonElement* pElem = pElemFromList->FindByID (uiCmdID);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	CBCGPBaseRibbonElement* pElem = ((CBCGPRibbonPanel*) this)->m_btnLaunch.FindByID (uiCmdID);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	if (m_btnDefault.GetID () == uiCmdID)
	{
		return &(((CBCGPRibbonPanel*) this)->m_btnDefault);
	}

	return NULL;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::FindByData (DWORD_PTR dwData) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		CBCGPBaseRibbonElement* pElem = pElemFromList->FindByData (dwData);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	CBCGPBaseRibbonElement* pElem = ((CBCGPRibbonPanel*) this)->m_btnLaunch.FindByData (dwData);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	if (m_btnDefault.GetData () == dwData)
	{
		return &(((CBCGPRibbonPanel*) this)->m_btnDefault);
	}

	return NULL;
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::SetElementMenu (UINT uiCmdID, HMENU hMenu, 
	BOOL bIsDefautCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, FindByID (uiCmdID));

	if (pButton == NULL)
	{
		TRACE(_T("Cannot find element with ID: %d\n"), uiCmdID);
		return FALSE;
	}

	ASSERT_VALID (pButton);
	pButton->SetMenu (hMenu, bIsDefautCommand, bRightAlign);

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::SetElementMenu (UINT uiCmdID, UINT uiMenuResID,
	BOOL bIsDefautCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, FindByID (uiCmdID));

	if (pButton == NULL)
	{
		TRACE(_T("Cannot find element with ID: %d\n"), uiCmdID);
		return FALSE;
	}

	ASSERT_VALID (pButton);
	pButton->SetMenu (uiMenuResID, bIsDefautCommand, bRightAlign);

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::Replace (int nIndex, CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize ())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPBaseRibbonElement* pOldElem = m_arElements [nIndex];
	ASSERT_VALID (pOldElem);

	pElem->CopyFrom (*pOldElem);
	m_arElements [nIndex] = pElem;

	delete pOldElem;
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::ReplaceByID (UINT uiCmdID, CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (uiCmdID == 0 || uiCmdID == (UINT)-1)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		if (pElemFromList->GetID () == uiCmdID)
		{
			return Replace (i, pElem);
		}

		if (pElemFromList->ReplaceByID (uiCmdID, pElem))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::SetElementRTC (int nIndex, 
														 CRuntimeClass* pRTC)
{
	ASSERT_VALID (this);
	ASSERT (pRTC != NULL);

	if (!pRTC->IsDerivedFrom (RUNTIME_CLASS (CBCGPBaseRibbonElement)))
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPBaseRibbonElement* pNewElement = DYNAMIC_DOWNCAST (
		CBCGPBaseRibbonElement, pRTC->CreateObject ());
	ASSERT_VALID (pNewElement);

	if (!Replace (nIndex, pNewElement))
	{
		delete pNewElement;
	}

	return pNewElement;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::SetElementRTCByID (UINT uiCmdID, 
	CRuntimeClass* pRTC)
{
	ASSERT_VALID (this);
	ASSERT (pRTC != NULL);

	if (!pRTC->IsDerivedFrom (RUNTIME_CLASS (CBCGPBaseRibbonElement)))
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPBaseRibbonElement* pNewElement = DYNAMIC_DOWNCAST (
		CBCGPBaseRibbonElement, pRTC->CreateObject ());
	ASSERT_VALID (pNewElement);

	if (!ReplaceByID (uiCmdID, pNewElement))
	{
		delete pNewElement;
	}

	return pNewElement;
}
//*************************************************************************************
CWnd* CBCGPRibbonPanel::GetParentWnd () const
{
	ASSERT_VALID (this);

	CWnd* pParentWnd = NULL;

	if (m_pParentMenuBar != NULL)
	{
		ASSERT_VALID (m_pParentMenuBar);
		pParentWnd = m_pParentMenuBar;
	}
	else if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		pParentWnd = m_pParent->GetParentRibbonBar ();
	}

	return pParentWnd;
}
//*************************************************************************************
void CBCGPRibbonPanel::RedrawElement (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	const CRect rectElem = pElem->GetRect ();

	if (rectElem.IsRectEmpty ())
	{
		return;
	}

	CWnd* pParentWnd = GetParentWnd ();

	if (pParentWnd->GetSafeHwnd () != NULL)
	{
		ASSERT_VALID (pParentWnd);

		pParentWnd->InvalidateRect (rectElem);
		pParentWnd->UpdateWindow ();
	}
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::HasElement (const CBCGPBaseRibbonElement* pElem) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		if (pElem == pElemFromList)
		{
			return TRUE;
		}
	}

	return FALSE;
}
//*************************************************************************************
void CBCGPRibbonPanel::GetElements (
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		arElements.Add (pElem);
	}

	if (m_btnLaunch.GetID () > 0)
	{
		arElements.Add (&m_btnLaunch);
	}

	if (!IsMainPanel ())
	{
		arElements.Add (&m_btnDefault);
	}
}
//*************************************************************************************
void CBCGPRibbonPanel::GetItemIDsList (CList<UINT,UINT>& lstItems) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetItemIDsList (lstItems);
	}

	m_btnDefault.GetItemIDsList (lstItems);
}
//*************************************************************************************
void CBCGPRibbonPanel::GetElementsByID (UINT uiCmdID,
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetElementsByID (uiCmdID, arElements);
	}

	m_btnDefault.GetElementsByID (uiCmdID, arElements);
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetPressed () const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pPressedElem = pElem->GetPressed ();
		if (pPressedElem != NULL)
		{
			ASSERT_VALID (pPressedElem);
			return pPressedElem;
		}
	}

	return NULL;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetDroppedDown () const
{
	ASSERT_VALID (this);

	if (!m_btnDefault.m_rect.IsRectEmpty ())
	{
		CBCGPBaseRibbonElement* pDroppedElem = 
			((CBCGPRibbonPanel*) this)->m_btnDefault.GetDroppedDown ();

		if (pDroppedElem != NULL)
		{
			ASSERT_VALID (pDroppedElem);
			return pDroppedElem;
		}
	}

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pDroppedElem = pElem->GetDroppedDown ();
		if (pDroppedElem != NULL)
		{
			ASSERT_VALID (pDroppedElem);
			return pDroppedElem;
		}
	}

	return NULL;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetHighlighted () const
{
	ASSERT_VALID (this);
	return m_pHighlighted;
}
//*************************************************************************************
BOOL CBCGPRibbonPanel::OnKey (UINT nChar)
{
	ASSERT_VALID (this);

	if (m_arElements.GetSize () == NULL)
	{
		return FALSE;
	}

	if (nChar == VK_TAB)
	{
		if (::GetKeyState (VK_SHIFT) & 0x80)
		{
			nChar = VK_UP;
		}
		else
		{
			nChar = VK_DOWN;
		}
	}

	const int nStep = 5;

	CBCGPBaseRibbonElement* pNewHighlighted = NULL;

	BOOL bIsCyclic = m_bMenuMode || (m_bIsFirst && m_bIsLast);

	switch (nChar)
	{
	case VK_RETURN:
	case VK_SPACE:
		if (m_pHighlighted != NULL)
		{
			ASSERT_VALID (m_pHighlighted);

			CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
				CBCGPRibbonButton, m_pHighlighted);

			if (pButton != NULL)
			{
				if (pButton->HasMenu ())
				{
					pButton->OnShowPopupMenu ();
				}
				else if (!pButton->IsDisabled ())
				{
					pButton->OnClick (pButton->GetRect ().TopLeft ());
				}

				return TRUE;
			}
		}

		break;

	case VK_DOWN:
	case VK_UP:
		if (m_pHighlighted != NULL)
		{
			ASSERT_VALID (m_pHighlighted);

			int x = m_pHighlighted->GetRect ().CenterPoint ().x;

			int yStart = nChar == VK_DOWN ? 
				m_pHighlighted->GetRect ().bottom + 1 :
				m_pHighlighted->GetRect ().top - 1;

			int yStep = nChar == VK_DOWN ? nStep : -nStep;

			for (int nTry = 0; nTry < 2 && pNewHighlighted == NULL; nTry++)
			{
				for (int i = 0; pNewHighlighted == NULL; i++)
				{
					int y = yStart;

					int x1 = x - i * nStep;
					int x2 = x + i * nStep;

					if (x1 < m_rect.left && x2 > m_rect.right)
					{
						break;
					}

					while (pNewHighlighted == NULL)
					{
						if ((pNewHighlighted = HitTest (CPoint (x1, y))) == NULL)
						{
							pNewHighlighted = HitTest (CPoint (x2, y));
						}

						if (pNewHighlighted != NULL)
						{
							ASSERT_VALID (pNewHighlighted);
							
							if (!pNewHighlighted->IsTabStop ())
							{
								pNewHighlighted = NULL;
							}
						}

						y += yStep;

						if (nChar == VK_DOWN)
						{
							if (y > m_rect.bottom)
							{
								break;
							}
						}
						else
						{
							if (y < m_rect.top)
							{
								break;
							}
						}
					}
				}

				if (nTry == 0 && pNewHighlighted == NULL)
				{
					if (bIsCyclic)
					{
						yStart = nChar == VK_DOWN ? m_rect.top : m_rect.bottom;
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			pNewHighlighted = nChar == VK_DOWN ? 
				GetFirstTabStop () : GetLastTabStop ();
		}
		break;

	case VK_RIGHT:
		if (m_bMenuMode &&
			m_pHighlighted != NULL && m_pHighlighted->HasMenu () &&
			!m_pHighlighted->IsDroppedDown ())
		{
			m_pHighlighted->OnShowPopupMenu ();
			break;
		}

	case VK_LEFT:
		if (m_bMenuMode && nChar == VK_LEFT && m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);

			CBCGPPopupMenu* pParentMenu = 
				DYNAMIC_DOWNCAST (CBCGPPopupMenu, m_pParentMenuBar->GetParent ());

			if (pParentMenu->GetParentPopupMenu () != NULL)
			{
				pParentMenu->CloseMenu ();
				return TRUE;
			}
		}

		if (m_pHighlighted != NULL)
		{
			ASSERT_VALID (m_pHighlighted);

			int y = m_pHighlighted->GetRect ().CenterPoint ().y;

			int xStart = nChar == VK_RIGHT ? 
				m_pHighlighted->GetRect ().right + 1 :
				m_pHighlighted->GetRect ().left - 1;

			int xStep = nChar == VK_RIGHT ? nStep : -nStep;

			for (int nTry = 0; nTry < 2 && pNewHighlighted == NULL; nTry++)
			{
				for (int i = 0; pNewHighlighted == NULL; i++)
				{
					int x = xStart;

					int y1 = y - i * nStep;
					int y2 = y + i * nStep;

					if (y1 < m_rect.top && y2 > m_rect.bottom)
					{
						break;
					}

					while (pNewHighlighted == NULL)
					{
						if ((pNewHighlighted = HitTest (CPoint (x, y1))) == NULL)
						{
							pNewHighlighted = HitTest (CPoint (x, y2));
						}

						if (pNewHighlighted != NULL)
						{
							ASSERT_VALID (pNewHighlighted);
							
							if (!pNewHighlighted->IsTabStop ())
							{
								pNewHighlighted = NULL;
							}
						}

						x += xStep;

						if (nChar == VK_RIGHT)
						{
							if (x > m_rect.right)
							{
								break;
							}
						}
						else
						{
							if (x < m_rect.left)
							{
								break;
							}
						}
					}
				}

				if (nTry == 0 && pNewHighlighted == NULL)
				{
					if (bIsCyclic)
					{
						xStart = nChar == VK_RIGHT ? m_rect.left : m_rect.right;
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			pNewHighlighted = nChar == VK_RIGHT ? 
				GetFirstTabStop () : GetLastTabStop ();
		}
		break;
	}

	if (pNewHighlighted != NULL)
	{
		ASSERT_VALID (pNewHighlighted);

		if (m_pHighlighted != pNewHighlighted)
		{
			if (m_pHighlighted != NULL)
			{
				ASSERT_VALID (m_pHighlighted);
				m_pHighlighted->m_bIsHighlighted = FALSE;
				m_pHighlighted->m_bIsFocused = FALSE;

				m_pHighlighted->Redraw ();
				m_pHighlighted = NULL;
			}

			m_pHighlighted = pNewHighlighted;
			pNewHighlighted->m_bIsHighlighted = TRUE;
			pNewHighlighted->m_bIsFocused = TRUE;

			m_pHighlighted->Redraw ();
		}

		return TRUE;
	}

	return FALSE;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetFirstTabStop () const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->IsTabStop ())
		{
			return pElem;
		}
	}

	return NULL;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetLastTabStop () const
{
	ASSERT_VALID (this);

	for (int i = (int) m_arElements.GetSize () - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->IsTabStop ())
		{
			return pElem;
		}
	}

	return NULL;
}
//***************************************************************************
void CBCGPRibbonPanel::CleanUpSizes ()
{
	ASSERT_VALID (this);
	m_arWidths.RemoveAll ();

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->CleanUpSizes ();
	}
}
//***************************************************************************
void CBCGPRibbonPanel::ScrollPalette (int nScrollOffset)
{
	ASSERT_VALID (this);

	int nDelta = m_nScrollOffset - nScrollOffset;

	if (nDelta == 0)
	{
		return;
	}

	m_nScrollOffset = nScrollOffset;

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (bIsLabel || bIsIcon)
		{
			pElem->m_rect.OffsetRect (0, nDelta);
		}
	}
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetParentButton () const
{
	ASSERT_VALID (this);

	if (m_pParentMenuBar == NULL)
	{
		return NULL;
	}

	return ((CBCGPPopupMenu*)m_pParentMenuBar->GetParent ())->GetParentRibbonElement ();
}
//***************************************************************************
CSize CBCGPRibbonPanel::GetPaltteMinSize () const
{
	ASSERT_VALID (this);

	if (m_pPaletteButton == NULL)
	{
		ASSERT (FALSE);
		return CSize (-1, -1);
	}

	ASSERT_VALID (m_pPaletteButton);

	CBCGPRibbonBar* pRibbonBar = m_pPaletteButton->GetTopLevelRibbonBar ();
	ASSERT_VALID (pRibbonBar);

	CClientDC dc (pRibbonBar);

	CFont* pOldFont = dc.SelectObject (pRibbonBar->GetFont ());
	ASSERT (pOldFont != NULL);

	const int cxScroll = ::GetSystemMetrics (SM_CXVSCROLL);

	int cxIcon = 0;
	int cyIcon = 0;

	int cxLabel = 0;
	int cyLabel = 0;

	int cxBottom = 0;
	int cyBottom = 0;

	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetSize (&dc);

		if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)))
		{

			cxIcon = max (cxIcon, sizeElem.cx + 2);
			cyIcon = max (cyIcon, sizeElem.cy + 2);
		}
		else if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
		{
			cxLabel = max (cxLabel, sizeElem.cx + 2);
			cyLabel = max (cyLabel, sizeElem.cy + 2);
		}
		else
		{
			cxBottom = max (cxBottom, sizeElem.cx);
			cyBottom += sizeElem.cy;
		}
	}

	dc.SelectObject (pOldFont);

	int cx = max (cxIcon, cxLabel);

	return CSize (max (cx + cxScroll, cxBottom), cyIcon + cyBottom + cyLabel);
}

#endif // BCGP_EXCLUDE_RIBBON
