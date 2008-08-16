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
// BCGPRibbonProgressBar.cpp: implementation of the CBCGPRibbonProgressBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPVisualManager.h"
#include "BCGPPopupMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef BCGP_EXCLUDE_RIBBON

IMPLEMENT_DYNCREATE(CBCGPRibbonProgressBar, CBCGPBaseRibbonElement)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonProgressBar::CBCGPRibbonProgressBar()
{
	CommonInit ();
}
//*****************************************************************************************************
CBCGPRibbonProgressBar::CBCGPRibbonProgressBar(
		UINT	nID, 
		int		nWidth,
		int		nHeight)
{
	CommonInit ();

	m_nID = nID;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}
//*****************************************************************************************************
CBCGPRibbonProgressBar::~CBCGPRibbonProgressBar()
{
}
//*****************************************************************************************************
void CBCGPRibbonProgressBar::CommonInit ()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
	m_nWidth = 100;
	m_nHeight = 22;
	m_bInfiniteMode = FALSE;
}
//********************************************************************************
void CBCGPRibbonProgressBar::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty () || m_nMax - m_nMin <= 0)
	{
		return;
	}

	CRect rectProgress = m_rect;
	rectProgress.DeflateRect (5, 5);

	CRect rectChunk = rectProgress;
	rectChunk.right = rectChunk.left + 
		m_nPos * rectChunk.Width () / (m_nMax - m_nMin);
	rectChunk.DeflateRect (1, 1);

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonProgressBar (
		pDC, this, rectProgress, rectChunk, m_bInfiniteMode);
}
//*****************************************************************************
void CBCGPRibbonProgressBar::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::CopyFrom (s);

	CBCGPRibbonProgressBar& src = (CBCGPRibbonProgressBar&) s;

	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nPos = src.m_nPos;
	m_nWidth = src.m_nWidth;
	m_nHeight = src.m_nHeight;
	m_bInfiniteMode = src.m_bInfiniteMode;
}
//*****************************************************************************
CSize CBCGPRibbonProgressBar::GetRegularSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);
	return CSize (m_nWidth, m_nHeight);
}
//*****************************************************************************
void CBCGPRibbonProgressBar::SetRange (int nMin, int nMax)
{
	ASSERT_VALID (this);

	m_nMin = nMin;
	m_nMax = nMax;
}
//*****************************************************************************
void CBCGPRibbonProgressBar::SetPos (int nPos, BOOL bRedraw)
{
	ASSERT_VALID (this);

	m_nPos = min (max (m_nMin, nPos), m_nMax);

	if (bRedraw)
	{
		Redraw ();

		CWnd* pMenu = CBCGPPopupMenu::GetActiveMenu ();

		if (pMenu != NULL && CWnd::FromHandlePermanent (pMenu->GetSafeHwnd ()) != NULL &&
			GetParentWnd () != NULL)
		{
			CRect rectScreen = m_rect;
			GetParentWnd ()->ClientToScreen (&rectScreen);

			CBCGPPopupMenu::UpdateAllShadows (rectScreen);
		}
	}
}
//*****************************************************************************
void CBCGPRibbonProgressBar::SetInfiniteMode (BOOL bSet)
{
	ASSERT_VALID (this);
	m_bInfiniteMode = bSet;
}

#endif // BCGP_EXCLUDE_RIBBON

