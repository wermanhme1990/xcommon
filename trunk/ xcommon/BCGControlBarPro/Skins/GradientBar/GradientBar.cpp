// GradientBar.cpp: implementation of the CGradientBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GradientBarSkin.h"
#include "GradientBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGradientBar, CBCGPVisualManagerXP)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGradientBar::CGradientBar()
{
	// TODO: add skin initialization code here
	
}

CGradientBar::~CGradientBar()
{

}

void CGradientBar::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
								CRect rectClient, CRect rectClip,
								BOOL bNCArea)
{
	if (bNCArea || pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		CBCGPVisualManagerXP::OnFillBarBackground (pDC, pBar,
									rectClient, rectClip,
									bNCArea);
		return;
	}

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rectClient,::GetSysColor (COLOR_3DFACE),
								::GetSysColor (COLOR_3DHILIGHT),
								pBar->GetBarStyle () & CBRS_ORIENT_HORZ);
}

void CGradientBar::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	CBCGPVisualManager::OnDrawBarBorder (pDC, pBar, rect);
}
