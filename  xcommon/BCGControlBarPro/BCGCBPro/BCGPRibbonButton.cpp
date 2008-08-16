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
// BCGPRibbonButton.cpp: implementation of the CBCGPRibbonButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonBar.h"
#include "bcgglobals.h"
#include "BCGPVisualManager.h"
#include "MenuImages.h"
#include "BCGPRibbonButtonsGroup.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPToolbarMenuButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPRibbonButton, CBCGPBaseRibbonElement)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonButton::CBCGPRibbonButton()
{
	CommonInit ();
}
//********************************************************************************
CBCGPRibbonButton::CBCGPRibbonButton (UINT nID, LPCTSTR lpszText, 
									  int nSmallImageIndex, 
									  int nLargeImageIndex,
									  BOOL bAlwaysShowDescription)
{
	CommonInit ();

	m_nID = nID;
	SetText (lpszText);

	m_nSmallImageIndex = nSmallImageIndex;
	m_nLargeImageIndex = nLargeImageIndex;

	m_bAlwaysShowDescription = bAlwaysShowDescription;
}
//********************************************************************************
CBCGPRibbonButton::CBCGPRibbonButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		HICON	hIcon,
		BOOL	bAlwaysShowDescription)
{
	CommonInit ();

	m_nID = nID;
	SetText (lpszText);
	m_hIcon = hIcon;
	m_bAlwaysShowDescription = bAlwaysShowDescription;
}
//********************************************************************************
void CBCGPRibbonButton::CommonInit ()
{
	m_hMenu = NULL;
	m_bRightAlignMenu = FALSE;
	m_bIsDefaultCommand = TRUE;
	m_nMenuArrowMargin = 2;
	m_bAutodestroyMenu = FALSE;

	m_nSmallImageIndex = -1;
	m_nLargeImageIndex = -1;

	m_sizeTextRight = CSize (0, 0);
	m_sizeTextBottom = CSize (0, 0);

	m_szMargin = CSize (3, 2);

	m_rectMenu.SetRectEmpty ();
	m_rectCommand.SetRectEmpty ();
	m_bMenuOnBottom = FALSE;

	m_bIsMenuHighlighted = FALSE;
	m_bIsCommandHighlighted = FALSE;

	m_hIcon = NULL;
	m_bForceDrawBorder = FALSE;

	m_bToBeClosed = FALSE;
	m_bAlwaysShowDescription = FALSE;
}
//********************************************************************************
CBCGPRibbonButton::~CBCGPRibbonButton()
{
	RemoveAllSubItems ();

	if (m_bAutodestroyMenu && m_hMenu != NULL)
	{
		::DestroyMenu (m_hMenu);
	}
}
//********************************************************************************
void CBCGPRibbonButton::SetText (LPCTSTR lpszText)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetText (lpszText);

	m_sizeTextRight = CSize (0, 0);
	m_sizeTextBottom = CSize (0, 0);

	m_szMargin.cx = m_strText.IsEmpty () ? 3 : 5;
}
//********************************************************************************
void CBCGPRibbonButton::SetDescription (LPCTSTR lpszText)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetDescription (lpszText);

	if (m_bAlwaysShowDescription)
	{
		m_sizeTextRight = CSize (0, 0);
		m_sizeTextBottom = CSize (0, 0);
	}
}
//********************************************************************************
void CBCGPRibbonButton::SetMenu (HMENU hMenu, BOOL bIsDefaultCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);
	
	if (m_bAutodestroyMenu && m_hMenu != NULL)
	{
		::DestroyMenu (m_hMenu);
	}

	m_bAutodestroyMenu = FALSE;
	m_hMenu = hMenu;
	m_bIsDefaultCommand = bIsDefaultCommand;

	if (m_nID == 0 || m_nID == (UINT)-1)
	{
		m_bIsDefaultCommand = FALSE;
	}

	m_bRightAlignMenu = bRightAlign;

	m_sizeTextRight = CSize (0, 0);
	m_sizeTextBottom = CSize (0, 0);
}
//********************************************************************************
void CBCGPRibbonButton::SetMenu (UINT uiMenuResID, BOOL bIsDefaultCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);

	SetMenu (
		::LoadMenu (::AfxGetResourceHandle (), MAKEINTRESOURCE (uiMenuResID)),
		bIsDefaultCommand, bRightAlign);

	m_bAutodestroyMenu = TRUE;
}
//********************************************************************************
void CBCGPRibbonButton::OnCalcTextSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_strText.IsEmpty () || IsMainRibbonButton ())
	{
		m_sizeTextRight = CSize (0, 0);
		m_sizeTextBottom = CSize (0, 0);
		return;
	}

	if (m_sizeTextRight != CSize (0, 0) && m_sizeTextBottom != CSize (0, 0))
	{
		// Already calculated
		return;
	}

	// Text placed on right will be always single line:
	m_sizeTextRight = pDC->GetTextExtent (m_strText);

	if (m_bAlwaysShowDescription && !m_strDescription.IsEmpty ())
	{
		CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
		ASSERT (pOldFont != NULL);

		m_sizeTextRight = pDC->GetTextExtent (m_strText);

		pDC->SelectObject (pOldFont);

		// Desciption will be draw below the text (in case of text on right only)
		int nTextHeight = 0;
		int nTextWidth = 0;

		CString strText = m_strDescription;

		for (int dx = m_sizeTextRight.cx; dx < m_sizeTextRight.cx * 10; dx += 10)
		{
			CRect rectText (0, 0, dx, 10000);

			nTextHeight = pDC->DrawText (strText, rectText, 
										DT_WORDBREAK | DT_CALCRECT);

			nTextWidth = rectText.Width ();
			
			if (nTextHeight <= 2 * m_sizeTextRight.cy)
			{
				break;
			}
		}
	
		m_sizeTextRight.cx = max (m_sizeTextRight.cx, nTextWidth);
		m_sizeTextRight.cy += min (2 * m_sizeTextRight.cy, nTextHeight) + 2 * m_szMargin.cy;
	}
	else
	{
		// Text placed on right will be always single line:
		m_sizeTextRight = pDC->GetTextExtent (m_strText);
	}

	CSize sizeImageLarge = GetImageSize (RibbonImageLarge);

	if (sizeImageLarge == CSize (0, 0))
	{
		m_sizeTextBottom = CSize (0, 0);
	}
	else
	{
		// Text placed on bottom will occupy large image size and 1-2 text rows:
		CSize sizeDefault (sizeImageLarge.cx, m_sizeTextRight.cy * 3 / 2);

		CString strText = m_strText;

		if (HasMenu ())
		{
			strText += _T("1W");	// Leave space for arrow
		}

		int nTextHeight = 0;
		int nTextWidth = 0;
		int dx = 0;

		do
		{
			CRect rectText (0, 0, sizeDefault.cx * 3 / 2 + dx, sizeDefault.cy);

			nTextHeight = pDC->DrawText (strText, rectText, 
										DT_WORDBREAK | DT_CALCRECT);

			nTextWidth = rectText.Width ();
			
			if (nTextHeight <= 2 * m_sizeTextRight.cy)
			{
				if (nTextHeight == m_sizeTextRight.cy && HasMenu ())
				{
					// Single line, move menu arrow to the next row
					nTextHeight = 2 * m_sizeTextRight.cy;
					nTextWidth = pDC->GetTextExtent (m_strText).cx;
				}

				break;
			}

			dx += sizeImageLarge.cx / 2;
		}
		while (nTextWidth < sizeImageLarge.cx * 4);

		nTextHeight = min (nTextHeight, 2 * m_sizeTextRight.cy);

		m_sizeTextBottom.cx = nTextWidth;
		m_sizeTextBottom.cy = nTextHeight;
	}
}
//********************************************************************************
void CBCGPRibbonButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	BOOL bIsDisabled = m_bIsDisabled;
	BOOL bIsDroppedDown = m_bIsDroppedDown;
	BOOL bIsHighlighted = m_bIsHighlighted;
	BOOL bMenuHighlighted = m_bIsMenuHighlighted;
	BOOL bCommandHighlighted = m_bIsCommandHighlighted;

	if (m_bIsDisabled && HasMenu ())
	{
		if (m_bIsDefaultCommand || 
			(!m_bIsDefaultCommand && !(m_nID == 0 || m_nID == (UINT)-1)))
		{
			m_bIsHighlighted = FALSE;
		}
		else
		{
			m_bIsDisabled = FALSE;
		}
	}

	if (m_bToBeClosed)
	{
		m_bIsDroppedDown = FALSE;
	}

	if (m_bIsFocused)
	{
		m_bIsHighlighted = TRUE;
		m_bIsMenuHighlighted = TRUE;
		m_bIsCommandHighlighted = TRUE;
	}

	CRect rectMenuArrow;
	rectMenuArrow.SetRectEmpty ();

	if (HasMenu ())
	{
		rectMenuArrow = m_rect;

		rectMenuArrow.left = rectMenuArrow.right - CMenuImages::Size ().cx - m_nMenuArrowMargin;
		rectMenuArrow.bottom -= 2 * m_nMenuArrowMargin;

		if (m_bIsDefaultCommand)
		{
			m_rectMenu = m_rect;

			m_rectMenu.left = m_rectMenu.right - CMenuImages::Size ().cx - m_nMenuArrowMargin;
			
			m_rectCommand = m_rect;
			m_rectCommand.right = m_rectMenu.left;

			m_bMenuOnBottom = FALSE;
		}
	}

	CSize sizeImageLarge = GetImageSize (RibbonImageLarge);
	CSize sizeImageSmall = GetImageSize (RibbonImageSmall);

	CRect rectText = m_rect;
	BOOL bIsLargeImage = FALSE;
	BOOL bDrawText = !IsMainRibbonButton () && !m_bQuickAccessMode;

	if (m_bQuickAccessMode)
	{
		bIsLargeImage = FALSE;
		bDrawText = FALSE;
	}
	else if (m_bCompactMode)
	{
		bIsLargeImage = FALSE;

		if (sizeImageLarge != CSize (0, 0))
		{
			if (sizeImageSmall == CSize (0, 0))
			{
				bIsLargeImage = TRUE;
			}
		}
		else
		{
			bDrawText = FALSE;
		}
	}
	else
	{
		if (sizeImageLarge != CSize (0, 0))
		{
			bIsLargeImage = TRUE;

			if (!m_bMenuOnBottom)
			{
				if (!m_rectMenu.IsRectEmpty ())
				{
					m_rectMenu.left -= CMenuImages::Size ().cx;
					m_rectCommand.right = m_rectMenu.left;
				}

				rectMenuArrow.OffsetRect (-CMenuImages::Size ().cx / 2, 0);
			}
		}
	}

	const RibbonImageType imageType = 
		bIsLargeImage ? RibbonImageLarge : RibbonImageSmall;

	CSize sizeImage = GetImageSize (imageType);
	BOOL bDrawDefaultImage = FALSE;

	if (m_bQuickAccessMode && sizeImage == CSize (0, 0))
	{
		// Use default image:
		sizeImage = CSize (16, 16);
		bDrawDefaultImage = TRUE;
	}
	
	CRect rectImage = m_rect;
	rectImage.DeflateRect (m_szMargin);

	if (bIsLargeImage && !m_bTextAlwaysOnRight)
	{
		rectImage.left = rectImage.CenterPoint ().x - sizeImage.cx / 2 + 1;
		rectImage.top += m_szMargin.cy;

		if (!bDrawText)
		{
			rectImage.top = rectImage.CenterPoint ().y - sizeImage.cy / 2;
		}
	}
	else
	{
		rectImage.top = rectImage.CenterPoint ().y - sizeImage.cy / 2;
	}

	rectImage.right = rectImage.left + sizeImage.cx;
	rectImage.bottom = rectImage.top + sizeImage.cy;

	if (bIsLargeImage && !m_bTextAlwaysOnRight && HasMenu () && m_bIsDefaultCommand)
	{
		m_rectMenu = m_rect;
		m_rectMenu.top = rectImage.bottom + m_szMargin.cy + 1;
		
		m_rectCommand = m_rect;
		m_rectCommand.bottom = m_rectMenu.top;

		m_bMenuOnBottom = TRUE;
	}

	COLORREF clrText = (COLORREF)-1;

	if (!IsMainRibbonButton ())
	{
		clrText = OnFillBackground (pDC);
	}

	if (IsMenuMode () && IsChecked () && sizeImage != CSize (0, 0))
	{
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonMenuCheckFrame (pDC,
			this, rectImage);
	}

	//------------
	// Draw image:
	//------------
	if (bDrawDefaultImage)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawDefaultRibbonImage (
			pDC, rectImage, m_bIsDisabled, m_bIsPressed, m_bIsHighlighted);
	}
	else
	{
		DrawImage (pDC, imageType, rectImage);
	}

	//-----------
	// Draw text:
	//-----------
	if (bDrawText)
	{
		CFont* pOldFont = NULL;

		CRect rectText = m_rect;
		UINT uiDTFlags = 0;

		COLORREF clrTextOld = (COLORREF)-1;

		if (bIsDisabled && 
			(m_bIsDefaultCommand ||
			(!m_bIsDefaultCommand && !(m_nID == 0 || m_nID == (UINT)-1))))
		{
			clrTextOld = pDC->SetTextColor (
				clrText == (COLORREF)-1 ? 
					CBCGPVisualManager::GetInstance ()->GetToolbarDisabledTextColor () : clrText);
		}
		else if (clrText != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor (clrText);
		}

		if (bIsLargeImage && !m_bTextAlwaysOnRight)
		{
			rectText.top = rectImage.bottom + m_szMargin.cy;
			uiDTFlags = DT_WORDBREAK | DT_CENTER;
	
			if (HasMenu ())
			{
				rectMenuArrow = m_rect;
				rectMenuArrow.DeflateRect (m_nMenuArrowMargin, m_nMenuArrowMargin);

				if (IsSingleLineText ())
				{
					//----------------------------------------
					// Single line text, draw arrow on bottom:
					//----------------------------------------
					rectMenuArrow.top = rectMenuArrow.bottom - CMenuImages::Size ().cy;
				}
				else
				{
					//-------------------------------------
					// Multi-line text, draw arrow on right:
					//-------------------------------------
					rectMenuArrow.top = rectText.bottom - m_sizeTextRight.cy;
					rectMenuArrow.bottom = rectMenuArrow.top + CMenuImages::Size ().cy;
					rectMenuArrow.left = rectMenuArrow.right - CMenuImages::Size ().cx;
				}
			}
		}
		else
		{
			rectText.left = rectImage.right;

			if (m_nImageOffset > 0)
			{
				rectText.left = m_rect.left + m_nImageOffset + 3 * m_szMargin.cx;
			}
			else if (sizeImage.cx != 0)
			{
				rectText.left += GetTextOffset ();
			}

			uiDTFlags = DT_SINGLELINE;
			
			if (!m_bAlwaysShowDescription || m_strDescription.IsEmpty ())
			{
				uiDTFlags |= DT_VCENTER;
			}
			else
			{
				pOldFont = pDC->SelectObject (&globalData.fontBold);
				ASSERT (pOldFont != NULL);

				rectText.top += max (0, (m_rect.Height () - m_sizeTextRight.cy) / 2);
			}
		}
		
		int nTextHeight = DrawText (pDC, m_strText, rectText, uiDTFlags);

		if (pOldFont != NULL)
		{
			pDC->SelectObject (pOldFont);
		}

		if (m_bAlwaysShowDescription && !m_strDescription.IsEmpty ())
		{
			rectText.top += nTextHeight + m_szMargin.cy;
			rectText.right = m_rect.right - m_szMargin.cx;

			pDC->DrawText (m_strDescription, rectText, DT_WORDBREAK | DT_END_ELLIPSIS);
		}

		if (nTextHeight == m_sizeTextRight.cy &&
			bIsLargeImage && HasMenu ())
		{
			rectMenuArrow = m_rect;
			rectMenuArrow.DeflateRect (m_nMenuArrowMargin, m_nMenuArrowMargin);
			rectMenuArrow.top = rectMenuArrow.bottom - CMenuImages::Size ().cy;
		}

		if (clrTextOld != (COLORREF)-1)
		{
			pDC->SetTextColor (clrTextOld);
		}
	}

	if (!IsMainRibbonButton ())
	{
		if (!rectMenuArrow.IsRectEmpty ())
		{
			CMenuImages::IMAGES_IDS id = CMenuImages::IdArowDown;

			if (IsMenuMode ())
			{
				id = CMenuImages::IdArowRightLarge;
			}

			CRect rectWhite = rectMenuArrow;
			rectWhite.OffsetRect (0, 1);

			CMenuImages::Draw (pDC, id, rectWhite, CMenuImages::ImageWhite);
			CMenuImages::Draw (pDC, id, rectMenuArrow, 
				m_bIsDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
		}

		OnDrawBorder (pDC);
	}

	m_bIsDisabled = bIsDisabled;
	m_bIsDroppedDown = bIsDroppedDown;
	m_bIsHighlighted = bIsHighlighted;
	m_bIsMenuHighlighted = bMenuHighlighted;
	m_bIsCommandHighlighted = bCommandHighlighted;
}
//********************************************************************************
void CBCGPRibbonButton::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL /*bIsSelected*/,
									  BOOL bHighlighted)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectImage = rect;
	rectImage.right = rect.left + nTextOffset;

	CSize sizeImageSmall = GetImageSize (RibbonImageSmall);
	if (sizeImageSmall != CSize (0, 0))
	{
		rectImage.DeflateRect (1, 0);
		rectImage.top += max (0, (rectImage.Height () - sizeImageSmall.cy) / 2);
		rectImage.bottom = rectImage.top + sizeImageSmall.cy;

		DrawImage (pDC, RibbonImageSmall, rectImage);
	}
	else
	{
		CBCGPVisualManager::GetInstance ()->OnDrawDefaultRibbonImage (
			pDC, rectImage);
	}

	CRect rectText = rect;

	if (HasMenu ())
	{
		CRect rectMenuArrow = rect;
		rectMenuArrow.left = rectMenuArrow.right - rectMenuArrow.Height ();

		CRect rectWhite = rectMenuArrow;
		rectWhite.OffsetRect (0, 1);

		BOOL bIsDarkMenu = TRUE;

		if (bHighlighted)
		{
			COLORREF clrText = GetSysColor (COLOR_HIGHLIGHTTEXT);
			
			if (GetRValue (clrText) > 128 &&
				GetGValue (clrText) > 128 &&
				GetBValue (clrText) > 128)
			{
				bIsDarkMenu = FALSE;
			}
		}

		CMenuImages::Draw (pDC, CMenuImages::IdArowRight, rectWhite,
			bIsDarkMenu ? CMenuImages::ImageWhite : CMenuImages::ImageBlack);

		CMenuImages::Draw (pDC, CMenuImages::IdArowRight, rectMenuArrow,
			m_bIsDisabled ? CMenuImages::ImageGray : 
			bIsDarkMenu ? CMenuImages::ImageBlack : CMenuImages::ImageWhite);

		rectText.right = rectMenuArrow.left;
	}

	rectText.left += nTextOffset;
	rectText.DeflateRect (m_szMargin.cx, 0);

	pDC->DrawText (strText, rectText, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	m_bIsDisabled = bIsDisabled;
}
//********************************************************************************
CSize CBCGPRibbonButton::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_bQuickAccessMode)
	{
		m_szMargin = CSize (3, 3);
	}

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arSubItems [i]);
		m_arSubItems [i]->SetParentCategory (m_pParent);
	}

	if (m_bQuickAccessMode)
	{
		return GetCompactSize (pDC);
	}

	CSize sizeImageLarge = GetImageSize (RibbonImageLarge);
	CSize sizeImageSmall = GetImageSize (RibbonImageSmall);

	if (IsMainRibbonButton ())
	{
		return sizeImageLarge;
	}

	if (sizeImageLarge == CSize (0, 0) || m_bTextAlwaysOnRight)
	{
		if (m_bTextAlwaysOnRight && sizeImageLarge != CSize (0, 0))
		{
			sizeImageSmall = sizeImageLarge;
		}

		int cx = sizeImageSmall.cx + 2 * m_szMargin.cx;
		
		if (m_sizeTextRight.cx > 0)
		{
			cx += m_szMargin.cx + m_sizeTextRight.cx;
		}

		int cy = max (sizeImageSmall.cy, m_sizeTextRight.cy) + 2 * m_szMargin.cy;

		if (sizeImageSmall.cy == 0)
		{
			cy += 2 * m_szMargin.cy;
		}

		if (HasMenu ())
		{
			cx += CMenuImages::Size ().cx;

			if (m_bIsDefaultCommand && m_nID != -1 && m_nID != 0)
			{
				cx += m_nMenuArrowMargin;
			}
		}

		return CSize (cx, cy);
	}

	int cx = max (sizeImageLarge.cx, m_sizeTextBottom.cx) + 2 * m_szMargin.cx;

	if (IsDefaultPanelButton ())
	{
		cx += 2 * m_szMargin.cx;
	}

	int cy = sizeImageLarge.cy + 2 * m_szMargin.cy;

	if (m_sizeTextBottom.cy != 0)
	{
		cy += m_sizeTextBottom.cy + 2 * m_szMargin.cy;

		if (m_sizeTextBottom.cy == m_sizeTextRight.cy)	// Single line
		{
			cy += m_sizeTextBottom.cy;
		}
	}
	else if (HasMenu ())
	{
		cy += CMenuImages::Size ().cy + 2 * m_nMenuArrowMargin;
	}

	return CSize (cx, cy);
}
//********************************************************************************
CSize CBCGPRibbonButton::GetCompactSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_bQuickAccessMode)
	{
		m_szMargin = CSize (3, 3);
	}

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arSubItems [i]);
		m_arSubItems [i]->SetParentCategory (m_pParent);
	}

	CSize sizeImageLarge = GetImageSize (RibbonImageLarge);
	CSize sizeImageSmall = GetImageSize (RibbonImageSmall);

	if (m_bQuickAccessMode)
	{
		if (sizeImageSmall == CSize (0, 0))
		{
			sizeImageSmall = CSize (16, 16);
		}

		int cx = sizeImageSmall.cx + 2 * m_szMargin.cx;
		int cy = sizeImageSmall.cy + 2 * m_szMargin.cy;

		if (HasMenu ())
		{
			cx += CMenuImages::Size ().cx + m_nMenuArrowMargin;
		}

		return CSize (cx, cy);
	}

	if (IsMainRibbonButton ())
	{
		return sizeImageSmall;
	}

	if (sizeImageLarge != CSize (0, 0))
	{
		if (sizeImageSmall == CSize (0, 0))
		{
			return GetRegularSize (pDC);
		}

		int cx = sizeImageSmall.cx + m_sizeTextRight.cx + 2 * m_szMargin.cx + GetTextOffset ();
		int cy = max (sizeImageSmall.cy, m_sizeTextRight.cy) + 2 * m_szMargin.cy;

		if (HasMenu ())
		{
			cx += CMenuImages::Size ().cx + 2 * m_nMenuArrowMargin;
		}

		return CSize (cx, cy);
	}

	const int nMenuArrowWidth = HasMenu () ? 
		CMenuImages::Size ().cx + m_nMenuArrowMargin : 0;

	if (sizeImageSmall == CSize (0, 0))
	{
		return CSize (	m_sizeTextRight.cx + 2 * m_szMargin.cx + nMenuArrowWidth, 
						m_sizeTextRight.cy + 2 * m_szMargin.cy);
	}

	return CSize (	sizeImageSmall.cx + 2 * m_szMargin.cx + nMenuArrowWidth, 
					sizeImageSmall.cy + 2 * m_szMargin.cy);
}
//********************************************************************************
void CBCGPRibbonButton::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonDown (point);

	if (!HasMenu () || IsMenuMode ())
	{
		return;
	}

	if (!m_rectMenu.IsRectEmpty () && !m_rectMenu.PtInRect (point))
	{
		return;
	}

	if (m_bIsDefaultCommand && m_bIsDisabled)
	{
		return;
	}

	OnShowPopupMenu ();
}
//********************************************************************************
void CBCGPRibbonButton::OnLButtonUp (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonUp (point);

	if (m_bIsDisabled || !m_bIsPressed || !m_bIsHighlighted || m_bIsDroppedDown)
	{
		return;
	}

	if (!m_rectCommand.IsRectEmpty () && !m_rectCommand.PtInRect (point))
	{
		return;
	}

	OnClick (point);
}
//*****************************************************************************
void CBCGPRibbonButton::OnMouseMove (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnMouseMove (point);

	if (!HasMenu () || m_nID == -1 || m_nID == 0)
	{
		return;
	}

	BOOL bMenuWasHighlighted = m_bIsMenuHighlighted;
	BOOL bCommandWasHighlighted = m_bIsCommandHighlighted;

	m_bIsMenuHighlighted = m_rectMenu.PtInRect (point);
	m_bIsCommandHighlighted = m_rectCommand.PtInRect (point);

	if (bMenuWasHighlighted != m_bIsMenuHighlighted ||
		bCommandWasHighlighted != m_bIsCommandHighlighted)
	{
		Redraw ();

		if (m_pParentMenu != NULL)
		{
			ASSERT_VALID (m_pParentMenu);
			m_pParentMenu->OnChangeHighlighted (this);
		}
	}
}
//*****************************************************************************
void CBCGPRibbonButton::OnClick (CPoint point)
{
	ASSERT_VALID (this);

	if (IsMenuMode () && HasMenu () && m_rectCommand.IsRectEmpty ())
	{
		return;
	}

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		m_pParentMenu->OnClickButton (this, point);
		return;
	}

	NotifyCommand ();
}
//******************************************************************************
void CBCGPRibbonButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

	CWnd* pWndParent = GetParentWnd ();
	if (pWndParent->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar ();
	if (pTopLevelRibbon->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CWnd* pWndOwner = pTopLevelRibbon->GetSafeOwner ();

	if (m_arSubItems.GetSize () > 0)
	{
		//--------------------------------
		// Build popup menu from subitems:
		//--------------------------------
		CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu
			(pTopLevelRibbon, m_arSubItems);

		pMenu->SetParentRibbonElement (this);

		pMenu->SetMenuMode ();
		
		CRect rectBtn = GetRect ();
		pWndParent->ClientToScreen (&rectBtn);

		int x = m_bRightAlignMenu ? rectBtn.right : rectBtn.left;
		int y = rectBtn.bottom;

		if (IsMenuMode ())
		{
			x = rectBtn.right;
			y = rectBtn.top;
		}

		CRect rectMenuLocation;
		rectMenuLocation.SetRectEmpty ();

		CBCGPRibbonPanel* pPanel = GetParentPanel ();

		if (pPanel != NULL && 
			pPanel->GetPreferedMenuLocation (rectMenuLocation))
		{
			pWndParent->ClientToScreen (&rectMenuLocation);

			x = rectMenuLocation.left;
			y = rectMenuLocation.top;

			pMenu->SetPreferedSize (rectMenuLocation.Size ());
		}

		pMenu->Create (pWndOwner, x, y, (HMENU) NULL);
		SetDroppedDown (pMenu);

		return;
	}

	HMENU hPopupMenu = GetMenu ();
	if (hPopupMenu == NULL)
	{
		return;
	}

	CRect rectBtn = GetRect ();
	pWndParent->ClientToScreen (&rectBtn);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;

	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->SetRightAlign (m_bRightAlignMenu);

	pPopupMenu->SetParentRibbonElement (this);

	CBCGPPopupMenu* pMenuActive = CBCGPPopupMenu::GetActiveMenu ();
	if (pMenuActive != NULL &&
		pMenuActive->GetSafeHwnd () != pWndParent->GetParent ()->GetSafeHwnd ())
	{
		pMenuActive->SendMessage (WM_CLOSE);
	}

	int x = m_bRightAlignMenu ? rectBtn.right : rectBtn.left;
	int y = rectBtn.bottom;

	pPopupMenu->Create (pWndOwner, x, y, hPopupMenu, FALSE);

	SetDroppedDown (pPopupMenu);
}
//******************************************************************************
void CBCGPRibbonButton::SetParentCategory (CBCGPRibbonCategory* pParent)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetParentCategory (pParent);

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arSubItems [i]);
		m_arSubItems [i]->SetParentCategory (m_pParent);
	}
}
//*****************************************************************************
void CBCGPRibbonButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	if (m_bAutodestroyMenu && m_hMenu != NULL)
	{
		::DestroyMenu (m_hMenu);
	}

	RemoveAllSubItems ();

	CBCGPBaseRibbonElement::CopyFrom (s);

	CBCGPRibbonButton& src = (CBCGPRibbonButton&) s;

	m_nSmallImageIndex = src.m_nSmallImageIndex;
	m_nLargeImageIndex = src.m_nLargeImageIndex;
	m_hMenu = src.m_hMenu;
	m_bAutodestroyMenu = FALSE;
	m_bRightAlignMenu = src.m_bRightAlignMenu;
	m_bIsDefaultCommand = src.m_bIsDefaultCommand;
	m_szMargin = src.m_szMargin;
	m_hIcon = src.m_hIcon;
	m_bForceDrawBorder = src.m_bForceDrawBorder;
	m_bAlwaysShowDescription = src.m_bAlwaysShowDescription;

	for (int i = 0; i < src.m_arSubItems.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pSrcElem = src.m_arSubItems [i];
		ASSERT_VALID (pSrcElem);

		CBCGPBaseRibbonElement* pElem =
			(CBCGPBaseRibbonElement*) pSrcElem->GetRuntimeClass ()->CreateObject ();
		ASSERT_VALID (pElem);

		pElem->CopyFrom (*pSrcElem);
		m_arSubItems.Add (pElem);
	}
}
//*****************************************************************************
void CBCGPRibbonButton::SetOriginal (CBCGPBaseRibbonElement* pOriginal)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetOriginal (pOriginal);

	CBCGPRibbonButton* pOriginalButton =
		DYNAMIC_DOWNCAST (CBCGPRibbonButton, pOriginal);

	if (pOriginalButton == NULL)
	{
		return;
	}

	ASSERT_VALID (pOriginalButton);

	if (pOriginalButton->m_arSubItems.GetSize () != m_arSubItems.GetSize ())
	{
		ASSERT (FALSE);
		return;
	}

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arSubItems [i];
		ASSERT_VALID (pButton);

		pButton->SetOriginal (pOriginalButton->m_arSubItems [i]);
	}
}
//*****************************************************************************
void CBCGPRibbonButton::DrawImage (CDC* pDC, RibbonImageType type, 
								   CRect rectImage)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPRibbonButton* pOrigButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, m_pOriginal);

	if (pOrigButton != NULL)
	{
		ASSERT_VALID (pOrigButton);

		BOOL bIsDisabled = pOrigButton->m_bIsDisabled;
		pOrigButton->m_bIsDisabled = m_bIsDisabled;

		CRect rect = pOrigButton->m_rect;
		pOrigButton->m_rect = m_rect;

		pOrigButton->DrawImage (pDC, type, rectImage);

		pOrigButton->m_bIsDisabled = bIsDisabled;
		pOrigButton->m_rect = rect;
		return;
	}

	if (m_hIcon != NULL)
	{
		const CSize sizeIcon = globalData.m_sizeSmallIcon;

		CPoint ptIcon = rectImage.TopLeft ();

		if (m_strText.IsEmpty () && GetMenu () == NULL)
		{
			ptIcon.x = m_rect.left + max (0, (m_rect.Width () - sizeIcon.cx) / 2);
			ptIcon.y = m_rect.top + max (0, (m_rect.Height () - sizeIcon.cy) / 2);
		}

		pDC->DrawState (ptIcon, sizeIcon, m_hIcon, 
			IsDisabled () ? DSS_DISABLED : DSS_NORMAL, (HBRUSH) NULL);
		return;
	}

	if (m_pParentGroup != NULL)
	{
		ASSERT_VALID (m_pParentGroup);

		if (m_pParentGroup->HasImages ())
		{
			m_pParentGroup->OnDrawImage (pDC, rectImage, this, m_nSmallImageIndex);
			return;
		}
	}

	if (m_pParent == NULL || rectImage.Width () == 0 || rectImage.Height () == 0)
	{
		return;
	}

	ASSERT_VALID (m_pParent);

	m_pParent->OnDrawImage (
		pDC, rectImage, this, 
		type == RibbonImageLarge, 
		type == RibbonImageLarge ? m_nLargeImageIndex : m_nSmallImageIndex,
		FALSE /* no center */);
}
//*****************************************************************************
CSize CBCGPRibbonButton::GetImageSize (RibbonImageType type) const
{
	ASSERT_VALID (this);

	CBCGPRibbonButton* pOrigButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, m_pOriginal);

	if (pOrigButton != NULL)
	{
		ASSERT_VALID (pOrigButton);
		return pOrigButton->GetImageSize (type);
	}

	if (m_hIcon != NULL)
	{
		return globalData.m_sizeSmallIcon;
	}

	const int nImageIndex = type == RibbonImageLarge  ? 
		m_nLargeImageIndex : m_nSmallImageIndex;

	if (nImageIndex < 0)
	{
		return CSize (0, 0);
	}

	if (m_pParentGroup != NULL)
	{
		ASSERT_VALID (m_pParentGroup);

		if (m_pParentGroup->HasImages ())
		{
			return m_pParentGroup->GetImageSize ();
		}
	}

	if (m_pParent == NULL)
	{
		return CSize (0, 0);
	}

	ASSERT_VALID (m_pParent);

	const int nImageCount = m_pParent->GetImageCount (type == RibbonImageLarge);

	if (nImageIndex >= nImageCount)
	{
		return CSize (0, 0);
	}

	return m_pParent->GetImageSize (type == RibbonImageLarge);
}
//*****************************************************************************
BOOL CBCGPRibbonButton::CanBeStretched ()
{
	ASSERT_VALID (this);
	return GetImageSize (RibbonImageLarge) != CSize (0, 0);
}
//*****************************************************************************
void CBCGPRibbonButton::AddSubItem (CBCGPBaseRibbonElement* pSubItem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pSubItem);

	pSubItem->SetParentCategory (m_pParent);

	m_arSubItems.Add (pSubItem);
}
//*****************************************************************************
void CBCGPRibbonButton::RemoveAllSubItems ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arSubItems [i]);
		delete m_arSubItems [i];
	}

	m_arSubItems.RemoveAll ();
}
//*****************************************************************************
COLORREF CBCGPRibbonButton::OnFillBackground (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	const BOOL bIsDisabled = m_bIsDisabled;

	if (m_bIsDisabled && HasMenu ())
	{
		m_bIsDisabled = FALSE;
	}

	COLORREF clrText = CBCGPVisualManager::GetInstance ()->
		OnFillRibbonButton (pDC, this);

	m_bIsDisabled = bIsDisabled;

	return clrText;
}
//*****************************************************************************
void CBCGPRibbonButton::OnDrawBorder (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	const BOOL bIsDisabled = m_bIsDisabled;

	if (m_bIsDisabled && HasMenu ())
	{
		m_bIsDisabled = FALSE;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonButtonBorder (pDC, this);

	m_bIsDisabled = bIsDisabled;
}
//*****************************************************************************
int CBCGPRibbonButton::AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL bDeep)
{
	ASSERT_VALID (this);

	int nIndex = CBCGPBaseRibbonElement::AddToListBox (pWndListBox, bDeep);

	if (bDeep)
	{
		for (int i = 0; i < m_arSubItems.GetSize (); i++)
		{
			ASSERT_VALID (m_arSubItems [i]);
			nIndex = m_arSubItems [i]->AddToListBox (pWndListBox, TRUE);
		}
	}

	return nIndex;
}
//*****************************************************************************
void CBCGPRibbonButton::ClosePopupMenu ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arSubItems [i]);
		m_arSubItems [i]->ClosePopupMenu ();
	}

	CBCGPBaseRibbonElement::ClosePopupMenu ();
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonButton::FindByID (UINT uiCmdID)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement* pElem = CBCGPBaseRibbonElement::FindByID (uiCmdID);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arSubItems [i];
		ASSERT_VALID (pButton);

		pElem = pButton->FindByID (uiCmdID);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonButton::FindByData (DWORD_PTR dwData)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement* pElem = CBCGPBaseRibbonElement::FindByData (dwData);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arSubItems [i];
		ASSERT_VALID (pButton);

		pElem = pButton->FindByData (dwData);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//******************************************************************************
CString CBCGPRibbonButton::GetToolTipText () const
{
	ASSERT_VALID (this);

	if (!m_bQuickAccessMode && m_bAlwaysShowDescription && !m_strDescription.IsEmpty ())
	{
		return _T("");
	}

	return CBCGPBaseRibbonElement::GetToolTipText ();
}
//******************************************************************************
int CBCGPRibbonButton::DrawText (CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags,
								 COLORREF clrText)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPVisualManager::GetInstance ()->DrawTextOnGlass (pDC, strText, rectText, uiDTFlags, 0, clrText);
		return pDC->GetTextExtent (strText).cy;
	}

	COLORREF clrTextOld = (COLORREF)-1;
	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor (clrText);
	}

	int nRes = pDC->DrawText (strText, rectText, uiDTFlags);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	return nRes;
}
//******************************************************************************
void CBCGPRibbonButton::SetParentRibbonBar (CBCGPRibbonBar* pRibbonBar)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetParentRibbonBar (pRibbonBar);

	for (int i = 0; i < m_arSubItems.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arSubItems [i];
		ASSERT_VALID (pButton);

		pButton->SetParentRibbonBar (pRibbonBar);
	}
}

#endif // BCGP_EXCLUDE_RIBBON
