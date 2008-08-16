// BCGPCaptionButton.cpp: implementation of the CBCGPCaptionButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGGlobals.h"
#include "BCGPCaptionButton.h"
#include "BCGPVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBCGPCaptionButton::m_nButtonMargin = 3;
int CBCGPCaptionButton::m_nButtonMarginVert = 4;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPCaptionButton::CBCGPCaptionButton()
{
	m_bPushed = FALSE;
	m_bFocused = FALSE;
	m_bDroppedDown = FALSE;
	m_bHidden = FALSE;
	m_bEnabled = TRUE;
	m_nHit = HTNOWHERE;
	m_bLeftAlign = FALSE;
	m_clrForeground = (COLORREF)-1;
	m_bIsMiniFrameButton = FALSE;
}

CBCGPCaptionButton::CBCGPCaptionButton(UINT nHit, BOOL bLeftAlign)
{
	m_bPushed = FALSE;
	m_bFocused = FALSE;
	m_bDroppedDown = FALSE;
	m_bHidden = FALSE;
	m_bEnabled = TRUE;
	m_nHit = nHit;
	m_bLeftAlign = bLeftAlign;
	m_clrForeground = (COLORREF)-1;
	m_bIsMiniFrameButton = FALSE;
}

CBCGPCaptionButton::~CBCGPCaptionButton()
{
}

UINT CBCGPCaptionButton::GetHit () const
{
	return m_nHit;
}

void CBCGPCaptionButton::OnDraw (CDC* pDC, BOOL bActive,
								 BOOL /*bHorz*/, BOOL bMaximized, BOOL bDisabled)
{
	if (m_bHidden)
	{
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawCaptionButton (
			pDC, this, bActive, FALSE, bMaximized, bDisabled || !m_bEnabled);
}

CMenuImages::IMAGES_IDS CBCGPCaptionButton::GetIconID (BOOL bHorz, BOOL bMaximized) const
{
	switch (m_nHit)
	{
	case HTCLOSE:
	case HTCLOSE_BCG:
		return CMenuImages::IdClose;

	case HTMINBUTTON:
		return bHorz ? 
			bMaximized ? CMenuImages::IdArowLeft : CMenuImages::IdArowRight : 
			bMaximized ? CMenuImages::IdArowDownLarge :  CMenuImages::IdArowUp;

	case HTMAXBUTTON:
		return bMaximized ? CMenuImages::IdPinHorz : CMenuImages::IdPinVert;

	case HTLEFTBUTTON_BCG:
		return CMenuImages::IdArowBack;

	case HTRIGHTBUTTON_BCG:
		return CMenuImages::IdArowForward;

	case HTMENU_BCG:
		return CMenuImages::IdArowDownLarge;
	}

	return (CMenuImages::IMAGES_IDS)-1;
}

CBCGPFrameCaptionButton::CBCGPFrameCaptionButton (UINT nHit)
{
	m_nHit = nHit;
	m_rect.SetRectEmpty ();
}

CBCGPFrameCaptionButton::~CBCGPFrameCaptionButton()
{
}
