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
// BCGPRibbonCheckBox.cpp: implementation of the CBCGPRibbonCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgglobals.h"
#include "BCGPRibbonCheckBox.h"
#include "BCGPVisualManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPRibbonCheckBox, CBCGPRibbonButton)

#define CHECK_BOX_DEFAULT_SIZE	20

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonCheckBox::CBCGPRibbonCheckBox()
{
}
//*******************************************************************************
CBCGPRibbonCheckBox::CBCGPRibbonCheckBox(UINT nID, LPCTSTR lpszText) :
	CBCGPRibbonButton (nID, lpszText)
{
}
//*******************************************************************************
CBCGPRibbonCheckBox::~CBCGPRibbonCheckBox()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

CSize CBCGPRibbonCheckBox::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_bQuickAccessMode)
	{
		return CSize (
			CHECK_BOX_DEFAULT_SIZE + 2 * m_szMargin.cx,
			CHECK_BOX_DEFAULT_SIZE + 2 * m_szMargin.cy);
	}

	return CBCGPRibbonButton::GetRegularSize (pDC);
}
//*******************************************************************************
CSize CBCGPRibbonCheckBox::GetCompactSize (CDC* pDC)
{
	return GetRegularSize (pDC);
}
//*******************************************************************************
CSize CBCGPRibbonCheckBox::GetImageSize (RibbonImageType type) const
{
	return type == RibbonImageLarge ? CSize (0, 0) : CSize (16, 16);
}
//*******************************************************************************
void CBCGPRibbonCheckBox::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	const int nCheckBoxSize = 
		(m_sizeTextRight == CSize (0, 0)) ? CHECK_BOX_DEFAULT_SIZE : m_sizeTextRight.cy + 2;

	//----------------
	// Draw check box:
	//----------------
	CRect rectCheck = m_rect;
	rectCheck.DeflateRect (m_szMargin);
	rectCheck.right = rectCheck.left + nCheckBoxSize;

	const BOOL bIsHighlighted = (IsHighlighted () || IsFocused ()) && !IsDisabled ();

	CBCGPVisualManager::GetInstance ()->OnDrawCheckBoxEx (pDC,
		rectCheck, IsChecked () || (IsPressed () && bIsHighlighted) ? 1 : 0,
		bIsHighlighted, IsPressed () && bIsHighlighted, !IsDisabled ());

	//-----------
	// Draw text:
	//-----------
	if (!m_bQuickAccessMode)
	{
		COLORREF clrTextOld = (COLORREF)-1;

		if (m_bIsDisabled)
		{
			clrTextOld = pDC->SetTextColor (
				CBCGPVisualManager::GetInstance ()->GetToolbarDisabledTextColor ());
		}

		CRect rectText = m_rect;
		rectText.left = rectCheck.right + m_szMargin.cx;

		DrawText (pDC, m_strText, rectText, DT_SINGLELINE | DT_VCENTER);

		if (clrTextOld != (COLORREF)-1)
		{
			pDC->SetTextColor (clrTextOld);
		}
	}
}
//********************************************************************************
void CBCGPRibbonCheckBox::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectCheck = rect;
	rectCheck.right = rect.left + nTextOffset;

	CBCGPVisualManager::GetInstance ()->OnDrawCheckBoxEx (pDC,
		rectCheck, 1, FALSE, FALSE, TRUE);

	rect.left += nTextOffset;
	rect.DeflateRect (m_szMargin.cx, 0);

	pDC->DrawText (strText, rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	m_bIsDisabled = bIsDisabled;
}

#endif // BCGP_EXCLUDE_RIBBON
