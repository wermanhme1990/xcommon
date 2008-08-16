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
//
// BCGPRibbonColorButton.cpp: implementation of the CBCGPRibbonColorButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonColorButton.h"
#include "BCGPColorMenuButton.h"
#include "BCGPRibbonPanelMenu.h"
#include "ColorPopup.h"
#include "BCGPDrawManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPRibbonColorButton, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonColorButton::CBCGPRibbonColorButton()
{
	m_Color = RGB (0, 0, 0);
	CommonInit ();
}
//********************************************************************************
CBCGPRibbonColorButton::CBCGPRibbonColorButton(UINT nID, LPCTSTR lpszText, int nSmallImageIndex, COLORREF color) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex)
{
	m_Color = color;
	CommonInit ();
}
//********************************************************************************
void CBCGPRibbonColorButton::CommonInit ()
{
	CBCGPColorBar::InitColors (NULL, m_Colors);

	m_ColorAutomatic = RGB (0, 0, 0);
	m_nColumns = -1;
	m_bIsAutomaticButton = FALSE;
	m_bIsOtherButton = FALSE;
	m_bIsDocumentColors = FALSE;
	m_bIsDefaultCommand = TRUE;
}
//********************************************************************************
CBCGPRibbonColorButton::~CBCGPRibbonColorButton()
{
}
//********************************************************************************
void CBCGPRibbonColorButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

	if (IsDisabled ())
	{
		return;
	}

	CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
	ASSERT_VALID (pRibbonBar);

	CList<COLORREF,COLORREF> lstDocColors;

	if (m_bIsDocumentColors)
	{
		CFrameWnd* pOwner = BCGCBProGetTopLevelFrame (pRibbonBar);
		ASSERT_VALID (pOwner);

		//---------------------------
		// Fill document colors list:
		//---------------------------
		pOwner->SendMessage (BCGM_GETDOCUMENTCOLORS, (WPARAM) m_nID, 
			(LPARAM) &lstDocColors);
	}

	CRect rectBtn = GetRect ();

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		m_pParentMenu->ClientToScreen (&rectBtn);
	}
	else
	{
		pRibbonBar->ClientToScreen (&rectBtn);
	}

	CColorPopup* pColorPopup =
		new CColorPopup (this, m_Colors, m_Color, 
		(m_bIsAutomaticButton ? (LPCTSTR) m_strAutomaticButtonLabel : NULL),
		(m_bIsOtherButton ? (LPCTSTR) m_strOtherButtonLabel : NULL),
		(m_bIsDocumentColors ? (LPCTSTR) m_strDocumentColorsLabel : NULL),
		lstDocColors,
		m_nColumns, m_ColorAutomatic, m_nID);

	pColorPopup->SetParentRibbonElement (this);
	pColorPopup->Create (pRibbonBar, rectBtn.left, rectBtn.bottom, (HMENU) NULL, TRUE);

	if (m_pParent != NULL)
	{
		m_pParent->HighlightPanel (NULL, CPoint (-1, -1));
	}

	SetDroppedDown (pColorPopup);
}
//*******************************************************************************
void CBCGPRibbonColorButton::DrawImage (CDC* pDC, RibbonImageType type, CRect rectImage)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CRect rectColor = rectImage;
	rectColor.top = rectColor.bottom - 4;

	rectImage.OffsetRect (0, -1);
	CBCGPRibbonButton::DrawImage (pDC, type, rectImage);

	COLORREF color = (IsDisabled ()) ?
		globalData.clrBarShadow :
			(m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);

		rectColor.DeflateRect (1, 1);
		dm.DrawRect (rectColor, color, (COLORREF)-1);
	}
	else
	{
		CBrush br (PALETTERGB(	GetRValue (color),
								GetGValue (color), 
								GetBValue (color)));

		CBrush* pOldBrush = pDC->SelectObject (&br);
		CPen* pOldPen = (CPen*) pDC->SelectStockObject (NULL_PEN);
		
		pDC->Rectangle (&rectColor);

		pDC->SelectObject (pOldPen);
		pDC->SelectObject (pOldBrush);
	}
}
//*******************************************************************************
void CBCGPRibbonColorButton::UpdateColor (COLORREF color)
{
	ASSERT_VALID (this);

	if (m_Color == color)
	{
		return;
	}

	m_Color = color;

	CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
	if (pRibbonBar != NULL)
	{
		ASSERT_VALID (pRibbonBar);

		CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arButtons;
		pRibbonBar->GetElementsByID (m_nID, arButtons);

		for (int i = 0; i < arButtons.GetSize (); i++)
		{
			CBCGPRibbonColorButton* pOther =
				DYNAMIC_DOWNCAST (CBCGPRibbonColorButton, arButtons [i]);
			if (pOther != NULL && pOther != this)
			{
				ASSERT_VALID (pOther);

				pOther->m_Color = color;
				pOther->Redraw ();
			}
		}
	}

	NotifyCommand ();

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);

		CFrameWnd* pParentFrame = BCGPGetParentFrame (m_pParentMenu);
		ASSERT_VALID (pParentFrame);

		pParentFrame->PostMessage (WM_CLOSE);
	}
	else
	{
		Redraw ();
	}
}
//*****************************************************************************************
void CBCGPRibbonColorButton::EnableAutomaticButton (LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable)
{
	ASSERT_VALID (this);

	m_strAutomaticButtonLabel = (bEnable && lpszLabel == NULL) ? _T("") : lpszLabel;
	m_ColorAutomatic = colorAutomatic;
	m_bIsAutomaticButton = bEnable;
}
//*****************************************************************************************
void CBCGPRibbonColorButton::EnableOtherButton (LPCTSTR lpszLabel)
{
	ASSERT_VALID (this);

	m_bIsOtherButton = (lpszLabel != NULL);
	m_strOtherButtonLabel = (lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*****************************************************************************************
void CBCGPRibbonColorButton::EnableDocumentColors (LPCTSTR lpszLabel)
{
	ASSERT_VALID (this);

	m_bIsDocumentColors = (lpszLabel != NULL);
	m_strDocumentColorsLabel = (lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*****************************************************************************
void CBCGPRibbonColorButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	if (!s.IsKindOf (RUNTIME_CLASS (CBCGPRibbonColorButton)))
	{
		return;
	}

	CBCGPRibbonColorButton& src = (CBCGPRibbonColorButton&) s;

	m_Color = src.m_Color;
	m_ColorAutomatic = src.m_ColorAutomatic;

	m_Colors.RemoveAll ();

	for (int i = 0; i < src.m_Colors.GetSize (); i++)
	{
		m_Colors.Add (src.m_Colors [i]);
	}

	m_nColumns = src.m_nColumns;

	m_bIsAutomaticButton = src.m_bIsAutomaticButton;
	m_bIsOtherButton = src.m_bIsOtherButton;
	m_bIsDocumentColors = src.m_bIsDocumentColors;

	m_strAutomaticButtonLabel = src.m_strAutomaticButtonLabel;
	m_strOtherButtonLabel = src.m_strOtherButtonLabel;
	m_strDocumentColorsLabel = src.m_strDocumentColorsLabel;
}
//*****************************************************************************************
void CBCGPRibbonColorButton::SetPalette (CPalette* pPalette)
{
	ASSERT_VALID (this);

	if (m_Colors.GetSize () != 0)
	{
		m_Colors.SetSize (0);
		m_Colors.FreeExtra ();
	}

	CBCGPColorBar::InitColors (pPalette, m_Colors);
}

#endif // BCGP_EXCLUDE_RIBBON
