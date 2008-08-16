// MSNVisualManager.cpp: implementation of the CMSNVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "popupdemo.h"
#include "MSNVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMSNVisualManager, CBCGPWinXPVisualManager)

const COLORREF clrFillDark	= RGB (220, 228, 247);
const COLORREF clrFillLight	= RGB (255, 255, 255);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSNVisualManager::CMSNVisualManager()
{
}
//**********************************************************************************
CMSNVisualManager::~CMSNVisualManager()
{
}
//**********************************************************************************
void CMSNVisualManager::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	int nStep = 0;
	int nPartHeight = 30;

	for (int y = rect.top; y < rect.bottom; y += nPartHeight)
	{
		CRect rectFill = rect;
		
		rectFill.top = y;
		rectFill.bottom = y + nPartHeight;

		if ((nStep % 2) != 0)
		{
			dm.FillGradient (rectFill, clrFillDark, clrFillLight, TRUE);
		}
		else
		{
			dm.FillGradient (rectFill, clrFillLight, clrFillDark, TRUE);
		}

		nStep++;
	}

	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, RGB (116, 141, 181), RGB (185, 200, 241));
}
//**********************************************************************************
void CMSNVisualManager::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, RGB (166, 180, 207), RGB (68, 87, 145));
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, RGB (255, 255, 255), RGB (209, 221, 245));
}
//**********************************************************************************
COLORREF  CMSNVisualManager::OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* /*pPopupWnd*/)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	CRect rectFill = rectCaption;
	rectFill.bottom = rectFill.CenterPoint ().y;

	dm.FillGradient (rectFill, clrFillLight, clrFillDark, TRUE);

	rectFill = rectCaption;
	rectFill.top = rectFill.CenterPoint ().y;

	dm.FillGradient (rectFill, clrFillDark, clrFillLight, TRUE);

	return RGB (37, 50, 118);
}
