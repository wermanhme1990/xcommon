// MyVisualManager.cpp: implementation of the CMyVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPPopupDlg.h"
#include "MyVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE (CMyVisualManager, CBCGPVisualManager2003)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyVisualManager::CMyVisualManager()
{

}

CMyVisualManager::~CMyVisualManager()
{

}

void CMyVisualManager::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, m_clrBarGradientDark, m_clrBarGradientLight);
}

void CMyVisualManager::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
}

void CMyVisualManager::OnDrawPopupWindowCaption (CDC* pDC, CRect rect)
{
	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, 
		m_clrCaptionBarGradientDark, m_clrCaptionBarGradientLight, TRUE);

	CRect rectGripper = rect;

	int xCenter = rectGripper.CenterPoint ().x;
	int yCenter = rectGripper.CenterPoint ().y;

	rectGripper.left = xCenter - 20;
	rectGripper.right = xCenter + 20;

	rectGripper.top = yCenter - 4;
	rectGripper.bottom = yCenter + 2;

	OnDrawBarGripper (pDC, rectGripper, FALSE, NULL);
}

void CMyVisualManager::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea)
{
	if (pBar == NULL ||
		!pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupDlg)))
	{
		CBCGPVisualManager2003::OnFillBarBackground (pDC, pBar,
									rectClient, rectClip, bNCArea);
		return;
	}

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rectClient, m_clrBarGradientDark, m_clrBarGradientLight);
}

void CMyVisualManager::OnErasePopupWindowButton (CDC* pDC, CRect rc, CBCGPButton* pButton)
{
	if (pButton->IsPressed ())
	{
		COLORREF color = m_clrHighlightDnGradientLight == (COLORREF)-1 ?
			m_clrHighlightDn : m_clrHighlightDnGradientLight;
		
		CBrush br (color);
		pDC->FillRect (&rc, &br);
		return;
	}
	else if (pButton->IsHighlighted () || pButton->IsPushed ())
	{
		COLORREF color = m_clrHighlightMenuItem == (COLORREF)-1 ?
			m_clrHighlight : m_clrHighlightMenuItem;
		
		CBrush br (color);
		pDC->FillRect (&rc, &br);
		return;
	}

	CRect rectParent;
	pButton->GetParent ()->GetClientRect (rectParent);

	pButton->GetParent ()->MapWindowPoints (pButton, rectParent);
	OnFillPopupWindowBackground (pDC, rectParent);
}

void CMyVisualManager::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rc, CBCGPButton* pButton)
{
	if (pButton->IsHighlighted () || pButton->IsPushed ())
	{
		pDC->Draw3dRect (rc, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}

