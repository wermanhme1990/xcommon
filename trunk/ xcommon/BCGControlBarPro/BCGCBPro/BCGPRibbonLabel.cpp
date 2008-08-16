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
// BCGPRibbonLabel.cpp: implementation of the CBCGPRibbonLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonLabel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef BCGP_EXCLUDE_RIBBON

IMPLEMENT_DYNCREATE(CBCGPRibbonLabel, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonLabel::CBCGPRibbonLabel()
{
}
//***********************************************************************************
CBCGPRibbonLabel::CBCGPRibbonLabel(LPCTSTR lpszText)
{
	ASSERT (lpszText != NULL);
	m_strText = lpszText;
}
//***********************************************************************************
CBCGPRibbonLabel::~CBCGPRibbonLabel()
{
}
//***********************************************************************************
void CBCGPRibbonLabel::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CRect rectText = m_rect;
	rectText.DeflateRect (m_szMargin.cx, 0);

	if (IsMenuMode ())
	{
		rectText.bottom -= 2;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonLabel (pDC, 
		this, m_rect);

	DrawText (pDC, m_strText, rectText, DT_SINGLELINE | DT_VCENTER);
}
//******************************************************************************
void CBCGPRibbonLabel::OnAfterChangeRect (CDC* /*pDC*/)
{
	ASSERT_VALID (this);

	if (m_strToolTip.IsEmpty ())
	{
		UpdateTooltipInfo ();
	}
}

#endif // BCGP_EXCLUDE_RIBBON
