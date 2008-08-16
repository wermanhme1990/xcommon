// CustomVisualManager.cpp: implementation of the CCustomVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SideBarDemo.h"
#include "CustomVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCustomVisualManager, CBCGPVisualManager2003)

const COLORREF clrBackground = RGB (57, 67, 69);
const COLORREF clrBorder = RGB (148, 165, 161);

const COLORREF clrCaptionDark = RGB (75, 75, 75);
const COLORREF clrCaptionLight = RGB (193, 193, 193);
const COLORREF clrCaptionText = RGB (255, 255, 255);

const COLORREF clrGroupText = RGB (220, 220, 220);
const COLORREF clrGroupLight = RGB (93, 140, 97);

const COLORREF clrTaskText = RGB (220, 220, 220);

const COLORREF clrGroupBackground = RGB (113, 111, 100);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomVisualManager::CCustomVisualManager ()
{
	m_nTasksHorzOffset = 2;
	m_nTasksIconHorzOffset = 2;
}

void CCustomVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	CRect rectLeft = rectWorkArea;
	rectLeft.right = rectLeft.left + 80;

	rectWorkArea.left = rectLeft.right;

	dm.FillGradient (rectLeft, 
		CBCGPDrawManager::PixelAlpha (clrBackground, 130), clrBackground, FALSE);

	pDC->FillSolidRect (rectWorkArea, clrBackground);
}

void CCustomVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
	ASSERT_VALID (pDC);

	m_clrTaskPaneGroupCaptionLight = clrGroupLight;
	m_clrTaskPaneGroupCaptionDark = clrBackground;

	COLORREF clrHiliteSaved = globalData.clrHilite;
	globalData.clrHilite = clrGroupText;

	CBCGPVisualManager2003::OnDrawTasksGroupCaption (pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CMenuImages::Size();
		CRect rectButton = pGroup->m_rect;
		rectButton.left = max(rectButton.left, 
			rectButton.right - (rectButton.Height()+1)/2 - (sizeButton.cx+1)/2);
		rectButton.top = max(rectButton.top, 
			rectButton.bottom - (rectButton.Height()+1)/2 - (sizeButton.cy+1)/2);
		rectButton.right = rectButton.left + sizeButton.cx;
		rectButton.bottom = rectButton.top + sizeButton.cy;

		if (rectButton.right <= pGroup->m_rect.right && rectButton.bottom <= pGroup->m_rect.bottom)
		{
			const int nRadius = /*(sizeButton.cy + 4) / 2*/7;

			rectButton = CRect (rectButton.CenterPoint () - CSize (nRadius - 1, nRadius - 1), CSize (nRadius * 2, nRadius * 2));

			CBCGPDrawManager dm (*pDC);
			dm.DrawGradientRing (rectButton, 
				RGB (14, 88, 13), RGB (167, 245, 167),
				(COLORREF)-1,
				45, nRadius);

			CPoint ptImage = CPoint (
				rectButton.CenterPoint ().x - sizeButton.cx / 2,
				rectButton.CenterPoint ().y - sizeButton.cy / 2);

			if (!pGroup->m_bIsCollapsed)
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowUp, ptImage);
			}
			else
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowDown, ptImage);
			}
		}
	}

	globalData.clrHilite = clrHiliteSaved;
}

void CCustomVisualManager::OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient (rect, clrGroupBackground, 
		CBCGPDrawManager::PixelAlpha (clrGroupBackground, 130), TRUE);
}

void CCustomVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial, BOOL bNoTitle)
{
}

void CCustomVisualManager::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL bIsSelected)
{
	ASSERT_VALID (pTask);
	ASSERT_VALID (pDC);

	if (pTask->m_bIsSeparator)
	{
		CRect rectText = pTask->m_rect;

		CPen penSep;
		penSep.CreatePen (PS_SOLID, 1, clrBorder);
		CPen* pPenOld = (CPen*) pDC->SelectObject (&penSep);

		pDC->MoveTo (rectText.left, rectText.CenterPoint ().y);
		pDC->LineTo (rectText.right, rectText.CenterPoint ().y);

		pDC->SelectObject (pPenOld);
		return;
	}

	COLORREF clrHotOld = globalData.clrHotText;
	COLORREF clrTextOld = globalData.clrWindowText;
	globalData.clrHotText = globalData.clrHilite;
	globalData.clrWindowText = clrTaskText;

	CBCGPVisualManagerXP::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);

	globalData.clrHotText = clrHotOld;
	globalData.clrWindowText = clrTextOld;
}

void CCustomVisualManager::OnDrawAppBarBorder (CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
									CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CRect rectInternal = rectBorder;
	rectInternal.DeflateRect (rectBorderSize);

	pDC->ExcludeClipRect (rectInternal);
	pDC->FillSolidRect (rectBorder, clrBorder);
	pDC->Draw3dRect (rectBorder, clrBackground, clrBackground);
	pDC->SelectClipRgn (NULL);
}

void CCustomVisualManager::OnDrawAppBarCaption (CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/, 
											CRect rectCaption, CString strCaption)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient (rectCaption, clrCaptionDark, clrCaptionLight, FALSE);

	// Paint caption text:
	int nOldMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->SetTextColor (clrCaptionText);
	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
	ASSERT_VALID (pOldFont);

	CRect rectText = rectCaption;
	rectText.DeflateRect (10, 0);
	pDC->DrawText (strCaption, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
	pDC->SetBkMode (nOldMode);
	pDC->SetTextColor (clrOldText);
}

void CCustomVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectImage (CPoint (0, 0), CMenuImages::Size ());

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	CBrush brBorder;
	brBorder.CreateSolidBrush (clrBorder);

	pDC->FillRect (rectFill, &brBorder);
	pDC->Draw3dRect (rect, clrCaptionDark, clrCaptionDark);

	if (bHilited)
	{
		CBrush brBorderLight;
		brBorderLight.CreateSolidBrush (clrCaptionLight);

		pDC->FillRect (rect, &brBorderLight);
		pDC->Draw3dRect (rect, clrCaptionDark, clrCaptionDark);
	}

	CPoint point (
		rect.left + (rect.Width () - rectImage.Width ()) / 2,
		rect.top + (rect.Height () - rectImage.Height ()) / 2);

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, point);
}
