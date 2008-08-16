// BCGPRibbonPaletteButton.cpp: implementation of the CBCGPRibbonPaletteButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonCategory.h"
#include "BCGGlobals.h"
#include "MenuImages.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonLabel.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int nScrollUpID	= -1;
const int nScrollDownID = -2;
const int nMenuID		= -3;
const int nImageMargin	= 4;
const int nBorderMargin	= 2;

////////////////////////////////////////////
// CBCGPRibbonPaletteIcon

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteIcon, CBCGPRibbonButton)

CBCGPRibbonPaletteIcon::CBCGPRibbonPaletteIcon (
		CBCGPRibbonPaletteButton* pOwner, int nIndex) :
		m_pOwner (pOwner),
		m_nIndex (nIndex)
{
}
//**************************************************************************
void CBCGPRibbonPaletteIcon::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pOwner);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_nIndex >= 0)
	{
		OnFillBackground (pDC);
		m_pOwner->DrawPaletteIcon (pDC, this);
		OnDrawBorder (pDC);
	}
	else
	{
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonPaletteButton (
			pDC, this);

		// Draw scroll/menu button:
		CMenuImages::IMAGES_IDS id = 
			m_nIndex == nScrollUpID ? CMenuImages::IdArowUp :
			m_nIndex == nScrollDownID ? CMenuImages::IdArowDown : CMenuImages::IdCustomizeArowDown;

		CRect rectWhite = m_rect;
		rectWhite.OffsetRect (0, 1);

		CMenuImages::Draw (pDC, id, rectWhite, CMenuImages::ImageWhite);
		CMenuImages::Draw (pDC, id, m_rect,
			m_bIsDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
	}
}
//***************************************************************************
void CBCGPRibbonPaletteIcon::OnClick (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	m_pOwner->OnClickPaletteIcon (m_pOriginal == NULL ? this : (CBCGPRibbonPaletteIcon*) m_pOriginal);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);

		if (m_pOwner->m_nPaletteID != 0)
		{
			m_pOwner->SetNotifyParentID (TRUE);
		}

		m_pParentMenu->OnClickButton (m_pOwner, point);
	}
	else
	{
		NotifyCommand ();
	}
}
//*****************************************************************************
void CBCGPRibbonPaletteIcon::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonPaletteIcon& src = (CBCGPRibbonPaletteIcon&) s;

	m_nIndex = src.m_nIndex;
	m_pOwner = src.m_pOwner;
	m_bIsChecked = src.m_bIsChecked;
}
//***************************************************************************
CSize CBCGPRibbonPaletteIcon::GetCompactSize (CDC* pDC)
{
	return GetRegularSize (pDC);
}
//***************************************************************************
CSize CBCGPRibbonPaletteIcon::GetRegularSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	CSize sizeIcon = m_pOwner->m_imagesPalette.GetImageSize ();

	if (!m_pOwner->m_bSmallIcons)
	{
		sizeIcon.cx += 2 * nImageMargin;
		sizeIcon.cy += 2 * nImageMargin;
	}

	return sizeIcon;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsFirst () const
{
	ASSERT_VALID (this);
	return m_nIndex == nScrollUpID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsLast () const
{
	ASSERT_VALID (this);
	return m_nIndex == nMenuID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsAutoRepeatMode (int& /*nDelay*/) const
{
	ASSERT_VALID (this);
	return m_nIndex == nScrollUpID || m_nIndex == nScrollDownID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::OnAutoRepeat ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	if (m_bIsDisabled)
	{
		return FALSE;
	}

	m_pOwner->OnClickPaletteIcon (this);
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPRibbonPaletteIcon::OnAddToQAToolbar (CBCGPRibbonQuickAccessToolbar& qat)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	m_pOwner->OnAddToQAToolbar (qat);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// CBCGPRibbonPaletteButton

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteButton, CBCGPRibbonButton)

CMap<UINT,UINT,int,int>	CBCGPRibbonPaletteButton::m_mapSelectedItems;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton()
{
	CommonInit ();
}
//***************************************************************************
CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton(
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		CBCGPToolBarImages& imagesPalette) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex)
{
	CommonInit ();
	imagesPalette.CopyTo (m_imagesPalette);

	CreateIcons ();
}
//***************************************************************************
CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton(
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		UINT				uiImagesPaletteResID,
		int					cxPaletteImage) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex)
{
	CommonInit ();

	if (uiImagesPaletteResID != 0)
	{
		m_imagesPalette.Load (uiImagesPaletteResID);

		BITMAP bmp;
		GetObject (m_imagesPalette.GetImageWell (), sizeof (BITMAP), &bmp);

		m_imagesPalette.SetImageSize (
			CSize (cxPaletteImage, bmp.bmHeight), TRUE);

		CreateIcons ();
	}
}
//***************************************************************************
CBCGPRibbonPaletteButton::~CBCGPRibbonPaletteButton()
{
	RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::AddGroup (
		LPCTSTR lpszGroupName,
		UINT	uiImagesPaletteResID,
		int		cxPaletteImage)
{
	ASSERT_VALID (this);
	ASSERT (lpszGroupName != NULL);

	m_arGroupNames.Add (lpszGroupName);
	m_arGroupLen.Add (m_imagesPalette.GetCount ());

	if (m_imagesPalette.GetCount () == 0)
	{
		m_imagesPalette.Load (uiImagesPaletteResID);

		BITMAP bmp;
		GetObject (m_imagesPalette.GetImageWell (), sizeof (BITMAP), &bmp);

		m_imagesPalette.SetImageSize (
			CSize (cxPaletteImage, bmp.bmHeight), TRUE);
	}
	else
	{
		ASSERT (cxPaletteImage == m_imagesPalette.GetImageSize ().cx);
		m_imagesPalette.Load (uiImagesPaletteResID, NULL, TRUE);
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::AddGroup (
		LPCTSTR lpszGroupName,
		CBCGPToolBarImages& imagesGroup)
{
	ASSERT_VALID (this);

	m_arGroupNames.Add (lpszGroupName);
	m_arGroupLen.Add (m_imagesPalette.GetCount ());

	if (m_imagesPalette.GetCount () == 0)
	{
		imagesGroup.CopyTo (m_imagesPalette);
	}
	else
	{
		ASSERT (CSize (imagesGroup.GetImageSize ()) == m_imagesPalette.GetImageSize ());
		m_imagesPalette.AddImage (imagesGroup.GetImageWell ());
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::RemoveAll ()
{
	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		delete m_arIcons [i];
	}

	m_arIcons.RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::CommonInit ()
{
	m_bIsDefaultCommand = FALSE;
	m_nImagesInRow = 0;
	m_nImagesInColumn = 0;
	m_bSmallIcons = FALSE;
	m_nScrollOffset = 0;
	m_nScrollTotal = 0;
	m_nSelected = 0;
	m_bEnableMenuResize = FALSE;
	m_nIconsInRow = -1;
	m_nPaletteID = 0;
	m_bNotifyPaletteID = FALSE;
}
//***************************************************************************
CSize CBCGPRibbonPaletteButton::GetCompactSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_bQuickAccessMode)
	{
		return CBCGPRibbonButton::GetCompactSize (pDC);
	}

	return CBCGPRibbonButton::GetRegularSize (pDC);
}
//***************************************************************************
CSize CBCGPRibbonPaletteButton::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL || m_imagesPalette.GetCount () == 0 || m_bQuickAccessMode)
	{
		return CBCGPRibbonButton::GetRegularSize (pDC);
	}

	ASSERT_VALID (m_pParent);

	if (m_arIcons.GetSize () == 0)
	{
		CreateIcons ();
	}

	CSize size = CBCGPRibbonButton::GetRegularSize (pDC);

	const CSize sizeImage = m_imagesPalette.GetImageSize ();
	const CSize sizePanelLargeImage = m_pParent->GetImageSize (TRUE);
	const CSize sizePanelSmallImage = m_pParent->GetImageSize (FALSE);

	if (sizeImage.cx <= sizePanelSmallImage.cx * 3 / 2)
	{
		size.cx = sizeImage.cx * 6;
		m_bSmallIcons = TRUE;
	}
	else
	{
		size.cx = (sizeImage.cx + 2 * nImageMargin) * 3;
		m_bSmallIcons = FALSE;
	}

	//---------------------------------------
	// Add space for menu and scroll buttons:
	//---------------------------------------
	size.cx += CMenuImages::Size ().cx + 3 * nImageMargin;

	return size;
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL || IsCompactMode () || m_nImagesInRow == 0 || m_nImagesInColumn == 0 ||
		m_imagesPalette.GetCount () == 0)
	{
		BOOL bCompactMode = m_bCompactMode;
		m_bCompactMode = FALSE;

		CBCGPRibbonButton::OnDraw (pDC);

		m_bCompactMode = bCompactMode;
		return;
	}

	CRect rectBorder = m_rect;
	rectBorder.DeflateRect (nBorderMargin, nBorderMargin);

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonPaletteBorder (
		pDC, this, rectBorder);

	CRect rectImages = m_rect;
	const CSize sizeImage = m_imagesPalette.GetImageSize ();

	CBCGPDrawState ds;

	if (m_imagesPalette.GetCount () > 0)
	{
		m_imagesPalette.SetTransparentColor (globalData.clrBtnFace);
		m_imagesPalette.PrepareDrawImage (ds);
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		m_arIcons [i]->OnDraw (pDC);
	}

	if (m_imagesPalette.GetCount () > 0)
	{
		m_imagesPalette.EndDrawImage (ds);
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnAfterChangeRect (pDC);

	m_nScrollTotal = 0;
	m_nScrollOffset = 0;

	const CSize sizeImage = m_imagesPalette.GetImageSize ();

	if (m_pParentMenu != NULL || IsCompactMode () || sizeImage.cx == 0 || sizeImage.cy == 0 ||
		m_bQuickAccessMode)
	{
		m_nImagesInRow = 0;
		m_nImagesInColumn = 0;

		RebuildIconLocations ();

		return;
	}

	const int cxMenu = CMenuImages::Size ().cx + 3 * nImageMargin;

	CRect rectImages = m_rect;
	rectImages.right -= cxMenu;

	m_nImagesInRow = rectImages.Width () / sizeImage.cx;
	m_nImagesInColumn = rectImages.Height () / sizeImage.cy;

	if (m_nImagesInRow == 0)
	{
		m_nScrollTotal = 0;
	}
	else
	{
		m_nScrollTotal = m_imagesPalette.GetCount () / m_nImagesInRow - m_nImagesInColumn;

		if (m_imagesPalette.GetCount () % m_nImagesInRow)
		{
			m_nScrollTotal++;
		}
	}

	RebuildIconLocations ();

	CRect rectBorder = m_rect;
	rectBorder.DeflateRect (nBorderMargin, nBorderMargin);

	const int cyMenu = rectBorder.Height () / 3 - 1;

	int yButton = rectBorder.top;

	CRect rectButtons = rectBorder;
	rectButtons.left = rectButtons.right - cxMenu + nImageMargin + nBorderMargin;

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);
		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		if (pIcon->m_nIndex < 0)	// Scroll button
		{
			int yBottom = yButton + cyMenu;

			if (i == m_arIcons.GetSize () - 1)
			{
				yBottom = rectBorder.bottom;
			}

			pIcon->m_rect = CRect (
				rectButtons.left,
				yButton,
				rectButtons.right,
				yBottom);
			
			yButton = yBottom;
		}
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::DrawPaletteIcon (CDC* pDC, 
	CBCGPRibbonPaletteIcon* pIcon)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pIcon);

	CRect rectImage = pIcon->m_rect;

	if (!m_bSmallIcons)
	{
		rectImage.DeflateRect (nImageMargin, nImageMargin);
	}

	m_imagesPalette.Draw (pDC, rectImage.left, rectImage.top,
		pIcon->m_nIndex, FALSE, IsDisabled ());
}
//***************************************************************************
void CBCGPRibbonPaletteButton::CreateIcons ()
{
	ASSERT_VALID (this);

	int nGroupIndex = 0;
	
	for (int i = 0; i < m_imagesPalette.GetCount (); i++)
	{
		if (nGroupIndex < m_arGroupLen.GetSize () &&
			i == m_arGroupLen [nGroupIndex])
		{
			m_arIcons.Add (new CBCGPRibbonLabel (m_arGroupNames [nGroupIndex++]));
		}

		CBCGPRibbonPaletteIcon* pIcon = new CBCGPRibbonPaletteIcon (this, i);

		if (i == m_nSelected)
		{
			pIcon->m_bIsChecked = TRUE;
		}

		m_arIcons.Add (pIcon);
	}

	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nScrollUpID));
	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nScrollDownID));
	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nMenuID));
}
//***************************************************************************
void CBCGPRibbonPaletteButton::RebuildIconLocations ()
{
	ASSERT_VALID (this);

	CRect rectImages = m_rect;

	const CSize sizeImage = m_imagesPalette.GetImageSize ();

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	rectImages.DeflateRect (0, nMargin);

	int yOffset = max (0, 
		(rectImages.Height () - (sizeImage.cy + 2 * nMargin) * m_nImagesInColumn) / 2);

	int nRow = 0;
	int nColumn = 0;

	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);
		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		if (pIcon->m_nIndex < 0)	// Scroll button
		{
			if (pIcon->m_nIndex == nScrollUpID)
			{
				pIcon->m_bIsDisabled = (m_nScrollOffset == 0);
			}
			else if (pIcon->m_nIndex == nScrollDownID)
			{
				pIcon->m_bIsDisabled = (m_nScrollOffset >= m_nScrollTotal);
			}

			continue;
		}

		if (nRow - m_nScrollOffset >= m_nImagesInColumn || 
			nRow < m_nScrollOffset)
		{
			pIcon->m_rect.SetRectEmpty ();
		}
		else
		{
			CRect rectIcon (
				CPoint (
					rectImages.left + sizeIcon.cx * nColumn + nImageMargin,
					rectImages.top + sizeIcon.cy * (nRow - m_nScrollOffset) + yOffset),
				sizeIcon);

			pIcon->m_rect = rectIcon;
		}

		nColumn++;
		
		if (nColumn == m_nImagesInRow)
		{
			nColumn = 0;
			nRow++;
		}
	}
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::HitTest (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL || IsCompactMode () || 
		m_imagesPalette.GetCount () == 0 || m_bQuickAccessMode)
	{
		return CBCGPRibbonButton::HitTest (point);
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		if (m_arIcons [i]->GetRect ().PtInRect (point))
		{
			return m_arIcons [i];
		}
	}

	return NULL;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetPressed ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arIcons [i];
		ASSERT_VALID (pButton);

		CBCGPBaseRibbonElement* pElem = pButton->GetPressed ();
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetHighlighted ()
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL || IsCompactMode () ||
		m_imagesPalette.GetCount () == 0 || m_bQuickAccessMode)
	{
		return CBCGPRibbonButton::GetHighlighted ();
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arIcons [i];
		ASSERT_VALID (pButton);

		CBCGPBaseRibbonElement* pElem = pButton->GetHighlighted ();
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnClickPaletteIcon (CBCGPRibbonPaletteIcon* pIcon)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pIcon);

	switch (pIcon->m_nIndex)
	{
	case nScrollUpID:
		m_nScrollOffset = max (0, m_nScrollOffset - 1);
		RebuildIconLocations ();
		Redraw ();
		break;

	case nScrollDownID:
		m_nScrollOffset = min (m_nScrollTotal, m_nScrollOffset + 1);
		RebuildIconLocations ();
		Redraw ();
		break;

	case nMenuID:
		OnShowPopupMenu ();
		break;

	default:
		{
			int nIconIndex = 0;

			for (int i = 0; i < m_arIcons.GetSize (); i++)
			{
				CBCGPRibbonPaletteIcon* pListIcon = DYNAMIC_DOWNCAST (
					CBCGPRibbonPaletteIcon, m_arIcons [i]);
				if (pListIcon == NULL)
				{
					continue;
				}

				ASSERT_VALID (pListIcon);

				if (pListIcon->m_bIsChecked)
				{
					pListIcon->m_bIsChecked = FALSE;
				}

				if (pListIcon == pIcon)
				{
					m_nSelected = nIconIndex;
					pIcon->m_bIsChecked = TRUE;

					if (pIcon->m_rect.IsRectEmpty () && m_nImagesInRow > 0)
					{
						m_nScrollOffset = nIconIndex / m_nImagesInRow;
						m_nScrollOffset = min (m_nScrollTotal, m_nScrollOffset);
						RebuildIconLocations ();
					}
				}

				nIconIndex++;
			}
		}

		Redraw ();

		m_mapSelectedItems.SetAt (m_nPaletteID == 0 ? m_nID : m_nPaletteID, pIcon->m_nIndex);
	}
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	if (!s.IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteButton)))
	{
		return;
	}

	CBCGPRibbonPaletteButton& src = (CBCGPRibbonPaletteButton&) s;

	RemoveAll ();

	src.m_imagesPalette.CopyTo (m_imagesPalette);
	m_bSmallIcons = src.m_bSmallIcons;
	m_nSelected = src.m_nSelected;
	m_bEnableMenuResize = src.m_bEnableMenuResize;
	m_nPaletteID = src.m_nPaletteID;
	m_nIconsInRow = src.m_nIconsInRow;

	ASSERT (src.m_arGroupNames.GetSize () == src.m_arGroupLen.GetSize ());

	m_arGroupNames.RemoveAll ();
	m_arGroupLen.RemoveAll ();

	for (int i = 0; i < src.m_arGroupNames.GetSize (); i++)
	{
		m_arGroupNames.Add (src.m_arGroupNames [i]);
		m_arGroupLen.Add (src.m_arGroupLen [i]);
	}

	CreateIcons ();
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

	CWnd* pWndParent = GetParentWnd ();
	if (pWndParent->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar ();
	if (pTopLevelRibbon->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	if (m_arIcons.GetSize () == 0)
	{
		CreateIcons ();
	}

	int nSel = GetLastSelectedItem (m_nPaletteID == 0 ? m_nID : m_nPaletteID);
	if (nSel >= 0)
	{
		SelectItem (nSel);
	}

	CBCGPBaseRibbonElement* pMenuButton = 
		(IsCompactMode () || m_pParentMenu != NULL || m_imagesPalette.GetCount () == 0 || m_bQuickAccessMode || m_pParentGroup != NULL) ? this :
		m_arIcons [m_arIcons.GetSize () - 1];	

	CWnd* pWndOwner = pTopLevelRibbon->GetSafeOwner ();

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (this);

	pMenu->SetParentRibbonElement (pMenuButton);
	pMenu->SetMenuMode ();
	
	CRect rectBtn = GetRect ();
	pWndParent->ClientToScreen (&rectBtn);

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = m_imagesPalette.GetImageSize ();
	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	int x = rectBtn.left;
	int y = rectBtn.bottom;

	if (!m_bQuickAccessMode && m_pParentGroup == NULL)
	{
		if (!IsCompactMode () && m_pParentMenu == NULL || m_imagesPalette.GetCount () == 0)
		{
			x = rectBtn.left + nBorderMargin;
			y = rectBtn.top + nBorderMargin;
		}
	}

	if (m_nIconsInRow > 0)
	{
		pMenu->SetPreferedSize (CSize (m_nIconsInRow * sizeIcon.cx, 0));
	}
	else
	{
		int nIconsInRow = m_bSmallIcons ? 10 : 4;
		int nRows = m_bSmallIcons ? 9 : 3;

		pMenu->SetPreferedSize (CSize (nIconsInRow * sizeIcon.cx, nRows * sizeIcon.cy));
	}

	pMenu->Create (pWndOwner, x, y, (HMENU) NULL);
	pMenuButton->SetDroppedDown (pMenu);
}
//**************************************************************************
void CBCGPRibbonPaletteButton::SelectItem (int nItemIndex)
{
	ASSERT_VALID (this);

	m_nSelected = nItemIndex;

	int nCurrIndex = 0;

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);

		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		if (pIcon->m_bIsChecked)
		{
			pIcon->m_bIsChecked = FALSE;
		}

		if (nCurrIndex == nItemIndex)
		{
			pIcon->m_bIsChecked = TRUE;
		}

		nCurrIndex++;
	}

	m_mapSelectedItems.SetAt (m_nPaletteID == 0 ? m_nID : m_nPaletteID, m_nSelected);

	Redraw ();
}
//**************************************************************************
int CBCGPRibbonPaletteButton::GetLastSelectedItem (UINT uiCmdID)
{
	int nIndex = -1;

	m_mapSelectedItems.Lookup (uiCmdID, nIndex);
	return nIndex;
}
//**************************************************************************
void CBCGPRibbonPaletteButton::GetMenuItems (
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arSubItems.GetSize (); i++)
	{
		arButtons.Add (m_arSubItems [i]);
	}

	for (i = 0; i < m_arIcons.GetSize () - 3 /* Scroll buttons */; i++)
	{
		arButtons.Add (m_arIcons [i]);
	}
}
//***************************************************************************
int CBCGPRibbonPaletteButton::GetMenuRowHeight () const
{
	ASSERT_VALID (this);

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = m_imagesPalette.GetImageSize ();
	
	return sizeImage.cy + 2 * nMargin;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetDroppedDown ()
{
	ASSERT_VALID (this);

	if (m_arIcons.GetSize () > 0)
	{
		CBCGPBaseRibbonElement* pMenuButton = m_arIcons [m_arIcons.GetSize () - 1];
		ASSERT_VALID (pMenuButton);

		if (pMenuButton->IsDroppedDown ())
		{
			return pMenuButton;
		}
	}

	return CBCGPRibbonButton::GetDroppedDown ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::SetNotifyParentID (BOOL bSet)
{
	m_bNotifyPaletteID = bSet;

	if (m_pOriginal != NULL)
	{
		ASSERT_VALID (m_pOriginal);

		CBCGPRibbonPaletteButton* pOriginal = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteButton, m_pOriginal);
		if (pOriginal != NULL)
		{
			pOriginal->m_bNotifyPaletteID = bSet;
		}
	}
}

////////////////////////////////////////////////
// CBCGPRibbonPaletteMenuButton

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteMenuButton, CBCGPToolbarMenuButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton()
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton(UINT uiID, int iImage, LPCTSTR lpszText,
		CBCGPToolBarImages& imagesPalette) :
	CBCGPToolbarMenuButton (uiID, NULL, iImage, lpszText),
	m_paletteButton (0, _T(""), -1, -1, imagesPalette)
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton (UINT uiID, int iImage, LPCTSTR lpszText,
		UINT uiImagesPaletteResID, int cxPaletteImage) :
	CBCGPToolbarMenuButton (uiID, NULL, iImage, lpszText),
	m_paletteButton (0, _T(""), -1, -1, uiImagesPaletteResID, cxPaletteImage)
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::~CBCGPRibbonPaletteMenuButton ()
{
}
//**************************************************************************************
void CBCGPRibbonPaletteMenuButton::CommonInit ()
{
	CBCGPRibbonBar* pRibbon = NULL;

	CFrameWnd* pParentFrame = m_pWndParent == NULL ?
		DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ()) :
		BCGCBProGetTopLevelFrame (m_pWndParent);

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pParentFrame);
	if (pMainFrame != NULL)
	{
		pRibbon = pMainFrame->GetRibbonBar ();
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pParentFrame);
		if (pFrame != NULL)
		{
			pRibbon = pFrame->GetRibbonBar ();
		}
	}

	if (pRibbon != NULL)
	{
		ASSERT_VALID (pRibbon);
		m_paletteButton.SetParentRibbonBar (pRibbon);
	}
	else
	{
		ASSERT (FALSE);	// Main farme should have the ribbon bar!
	}
}
//**************************************************************************************
CBCGPPopupMenu* CBCGPRibbonPaletteMenuButton::CreatePopupMenu ()
{
	ASSERT_VALID (this);

	if (m_paletteButton.m_imagesPalette.GetCount () == 0)
	{
		TRACE(_T("The palette is not initialized! You should add palette icons first.\n"));
		ASSERT (FALSE);
		return NULL;
	}

	if (m_paletteButton.m_arIcons.GetSize () == 0)
	{
		m_paletteButton.CreateIcons ();
	}

	m_paletteButton.SelectItem (CBCGPRibbonPaletteButton::GetLastSelectedItem (
		m_paletteButton.m_nPaletteID == 0 ? m_paletteButton.m_nID : m_paletteButton.m_nPaletteID));

	for (int i = 0; i < m_paletteButton.m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_paletteButton.m_arSubItems [i]);
		m_paletteButton.m_arSubItems [i]->SetParentRibbonBar (m_paletteButton.m_pRibbonBar);
	}

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (&m_paletteButton);

	pMenu->SetMenuMode ();
	
	int nMargin = m_paletteButton.m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = m_paletteButton.m_imagesPalette.GetImageSize ();
	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	int nIconsInRow = m_paletteButton.m_nIconsInRow > 0 ? 
		m_paletteButton.m_nIconsInRow : m_paletteButton.m_bSmallIcons ? 10 : 4;

	pMenu->SetPreferedSize (CSize (nIconsInRow * sizeIcon.cx, 0));
	pMenu->EnableCustomizeMenu (FALSE);

	return pMenu;
}
//**************************************************************************************
void CBCGPRibbonPaletteMenuButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarMenuButton::CopyFrom (s);

	const CBCGPRibbonPaletteMenuButton& src = (const CBCGPRibbonPaletteMenuButton&) s;

	m_paletteButton.CopyFrom (src.m_paletteButton);
}

#endif // BCGP_EXCLUDE_RIBBON
