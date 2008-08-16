// SilverRing.cpp: implementation of the CSilverRing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "SilverRingSkin.h"
#include "SilverRing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSilverRing, CBCGPVisualManagerXP)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSilverRing::CSilverRing()
{
	// TODO: add skin initialization code here
	
}

CSilverRing::~CSilverRing()
{

}

void CSilverRing::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!pButton->IsExtraSize () ||
		CBCGPToolBar::IsCustomizeMode ())
	{
		CBCGPVisualManagerXP::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	if (pButton->m_bTextBelow && pButton->IsHorizontal ())
	{
		int nTextHeight = pButton->GetTextSize ().cy;
		rect.bottom -= nTextHeight;

		if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton)))
		{
			int xCenter = (rect.left + rect.right) / 2;
			rect.left = xCenter - rect.Height () / 2;
			rect.right = xCenter + rect.Height () / 2;
		}
	}

	rect.DeflateRect (1, 1);

	if (state != ButtonsIsHighlighted &&
		(pButton->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
	{
		dm.DrawGradientRing (rect, 
			::GetSysColor (COLOR_3DFACE), 
			::GetSysColor (COLOR_3DHILIGHT),
			::GetSysColor (COLOR_3DDKSHADOW), 225, 10);
		return;
	}

	switch (state)
	{
	case ButtonsIsHighlighted:
		if ((pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) == 0)
		{
			dm.DrawGradientRing (rect, 
				::GetSysColor (COLOR_3DHILIGHT), 
				::GetSysColor (COLOR_3DSHADOW),
				::GetSysColor (COLOR_3DDKSHADOW), 225, 5);
			return;
		}

	case ButtonsIsPressed:
		dm.DrawGradientRing (rect, 
			::GetSysColor (COLOR_3DSHADOW), 
			::GetSysColor (COLOR_3DHILIGHT),
			::GetSysColor (COLOR_3DDKSHADOW), 225, 2);
		return;
	}

	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		dm.DrawGradientRing (rect, 
			::GetSysColor (COLOR_3DHILIGHT), 
			::GetSysColor (COLOR_3DSHADOW), 
			-1, 225, 2);
	}
	else
	{
		dm.DrawGradientRing (rect, 
			::GetSysColor (COLOR_3DHILIGHT), 
			::GetSysColor (COLOR_3DSHADOW), 
			-1, 225, 4);
	}
}

void CSilverRing::OnDrawButtonBorder (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!pButton->IsExtraSize () ||
		CBCGPToolBar::IsCustomizeMode ())
	{
		CBCGPVisualManagerXP::OnDrawButtonBorder (pDC, pButton, rect, state);
	}
}
