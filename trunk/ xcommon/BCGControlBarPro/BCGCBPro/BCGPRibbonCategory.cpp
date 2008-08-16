//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonCategoryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonBar.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonPanelMenu.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int nPanelOffsetX = 3;
static const int nPanelOffsetY = 4;

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonTab

CBCGPRibbonTab::CBCGPRibbonTab ()
{
	m_bIsTrancated = FALSE;
	m_Color = BCGPCategoryColor_None;
}

CString CBCGPRibbonTab::GetToolTipText () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	if (!m_bIsTrancated)
	{
		return _T("");
	}

	CString strToolTipText = m_pParent->m_strName;
	strToolTipText.Remove (_T('&'));

	return strToolTipText;
}

void CBCGPRibbonTab::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	CBCGPBaseRibbonElement::CopyFrom (s);

	CBCGPRibbonTab& src = (CBCGPRibbonTab&) s;
	m_Color = src.m_Color;
}

void CBCGPRibbonTab::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pParent);
	ASSERT_VALID (m_pParent->GetParentRibbonBar ());

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	COLORREF clrText = CBCGPVisualManager::GetInstance ()->OnDrawRibbonCategoryTab (
		pDC, this, m_pParent->IsActive () || GetDroppedDown () != NULL);

	COLORREF clrTextOld = pDC->SetTextColor (clrText);

	CRect rectTab = m_rect;
	CRect rectTabText = m_rect;

	pDC->DrawText (m_pParent->m_strName, rectTabText, 
		DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);

	const int cxTabText = rectTabText.Width ();
	const int cxTabTextMargin = max (4, (rectTab.Width () - cxTabText) / 2);

	rectTab.DeflateRect (cxTabTextMargin, 0);
	rectTab.top += 2;

	pDC->DrawText (m_pParent->m_strName, rectTab, DT_SINGLELINE | DT_VCENTER);

	pDC->SetTextColor (clrTextOld);
}

CSize CBCGPRibbonTab::GetRegularSize (CDC* /*pDC*/)
{
	return CSize (0, 0);
}

void CBCGPRibbonTab::OnLButtonDown (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	m_pParent->GetParentRibbonBar ()->SetActiveCategory (m_pParent);
}

void CBCGPRibbonTab::OnLButtonDblClk (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	if (m_pParent->IsActive ())
	{
		CBCGPRibbonBar* pBar = m_pParent->GetParentRibbonBar ();
		ASSERT_VALID (pBar);


		if ((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ELEMENTS) != 0)
		{
			// Ribbon is minimized, restore it now:
			if (IsDroppedDown ())
			{
				ClosePopupMenu ();
			}
			
			m_pParent->ShowElements ();
		}
		else
		{
			// Minimize ribbon:
			m_pParent->ShowElements (FALSE);
		}

		pBar->GetParentFrame ()->RecalcLayout ();
		pBar->RedrawWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCategory

IMPLEMENT_DYNCREATE(CBCGPRibbonCategory, CObject)

CBCGPRibbonCategory::CBCGPRibbonCategory ()
{
	CommonInit ();
}
//******************************************************************************************
CBCGPRibbonCategory::CBCGPRibbonCategory (
		CBCGPRibbonBar*	pParenrRibbonBar,
		LPCTSTR			lpszName,
		UINT			uiSmallImagesResID,
		UINT			uiLargeImagesResID,
		CSize			sizeSmallImage,
		CSize			sizeLargeImage)
{
	ASSERT_VALID (pParenrRibbonBar);
	ASSERT (lpszName != NULL);

	CommonInit ();

	m_strName = lpszName;
	m_pParenrRibbonBar = pParenrRibbonBar;
	m_Tab.m_pParent = this;

	//-------------
	// Load images:
	//-------------
	m_SmallImages.SetImageSize (sizeSmallImage);
	m_LargeImages.SetImageSize (sizeLargeImage);

	if (uiSmallImagesResID > 0)
	{
		if (!m_SmallImages.Load (uiSmallImagesResID))
		{
			ASSERT (FALSE);
		}
	}

	if (uiLargeImagesResID > 0)
	{
		if (!m_LargeImages.Load (uiLargeImagesResID))
		{
			ASSERT (FALSE);
		}
	}
}
//******************************************************************************************
void CBCGPRibbonCategory::CommonInit ()
{
	m_pParenrRibbonBar = NULL;
	m_pParentMenuBar = NULL;
	m_bMouseIsPressed = FALSE;
	m_bIsActive = FALSE;
	m_bIsVisible = TRUE;
	m_dwData = 0;
	m_uiContextID = 0;
	m_nLastCategoryWidth = -1;

	m_rect.SetRectEmpty ();
}
//******************************************************************************************
CBCGPRibbonCategory::~CBCGPRibbonCategory()
{
	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		delete m_arPanels [i];
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		delete m_arElements [i];
	}
}
//******************************************************************************************
void CBCGPRibbonCategory::OnDraw (CDC* pDC) 
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonCategory (
		pDC, this, m_rect);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->DoPaint (pDC);
	}
}
//******************************************************************************************
CBCGPRibbonPanel* CBCGPRibbonCategory::AddPanel (LPCTSTR lpszPanelName,
												 HICON hIcon,
												 CRuntimeClass* pRTI)
{
	ASSERT_VALID (this);
	ASSERT (lpszPanelName != NULL);

	CBCGPRibbonPanel* pPanel = NULL;

	if (pRTI != NULL)
	{
		pPanel = DYNAMIC_DOWNCAST (CBCGPRibbonPanel, pRTI->CreateObject ());

		if (pPanel == NULL)
		{
			ASSERT (FALSE);
			return NULL;
		}

		pPanel->CommonInit (lpszPanelName, hIcon);
	}
	else
	{
		pPanel = new CBCGPRibbonPanel (lpszPanelName, hIcon);
	}

	m_arPanels.Add (pPanel);

	pPanel->m_pParent = this;
	m_nLastCategoryWidth = -1;
	return pPanel;
}
//******************************************************************************************
int CBCGPRibbonCategory::GetPanelCount () const
{
	ASSERT_VALID (this);
	return (int) m_arPanels.GetSize ();
}
//******************************************************************************************
CBCGPRibbonPanel* CBCGPRibbonCategory::GetPanel (int nIndex)
{
	ASSERT_VALID (this);
	return m_arPanels [nIndex];
}
//******************************************************************************************
int CBCGPRibbonCategory::GetMaxHeight (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	int nMaxHeight = 0;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		nMaxHeight = max (nMaxHeight, pPanel->GetHeight (pDC));
	}

	return nMaxHeight + globalData.GetTextHeight () + 2 * nPanelOffsetY;
}
//******************************************************************************************
void CBCGPRibbonCategory::RecalcLayout (CDC* pDC)
{
	RecalcPanelWidths (pDC);

	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->OnAfterChangeRect (pDC);
	}

	int nPanels = (int) m_arPanels.GetSize ();
	if (nPanels == 0)
	{
		return;
	}

	const DWORD dwRibbonHideFlags = GetParentRibbonBar ()->m_dwHideFlags;
	const BOOL bHideAll = (dwRibbonHideFlags & BCGPRIBBONBAR_HIDE_ALL) ||
		(dwRibbonHideFlags & BCGPRIBBONBAR_HIDE_ELEMENTS);

	if (bHideAll && m_pParentMenuBar == NULL)
	{
		for (i = 0; i < m_arPanels.GetSize (); i++)
		{
			CBCGPRibbonPanel* pPanel = m_arPanels [i];
			ASSERT_VALID (pPanel);

			pPanel->Repos (pDC, CRect (0, 0, 0, 0));
			pPanel->OnShow (FALSE);
		}

		m_nLastCategoryWidth = -1;
	}
	else if (m_nLastCategoryWidth != m_rect.Width ())
	{
		m_nLastCategoryWidth = m_rect.Width ();

		CRect rectClient = m_rect;
		rectClient.DeflateRect (2 * nPanelOffsetX, nPanelOffsetY);

		//--------------------------
		// all panels in max. width:
		//--------------------------
		for (i = 0; i < m_arPanels.GetSize (); i++)
		{
			CBCGPRibbonPanel* pPanel = m_arPanels [i];
			ASSERT_VALID (pPanel);

			pPanel->m_nCurrWidthIndex = 0;
		}

		int nLastTruncatedLevel = 999;

		while (TRUE)
		{
			int nTotalWidth = 0;
			int nMaxPanelWidth = 0;
			CBCGPRibbonPanel* pWidestPanel = NULL;
			BOOL bAllAreTruncated = TRUE;

			for (i = 0; i < m_arPanels.GetSize (); i++)
			{
				CBCGPRibbonPanel* pPanel = m_arPanels [i];
				ASSERT_VALID (pPanel);

				if (pPanel->m_nCurrWidthIndex < pPanel->m_arWidths.GetSize () - 1)
				{
					bAllAreTruncated = FALSE;
				}

				int nCurrPanelWidth = pPanel->m_arWidths [pPanel->m_nCurrWidthIndex];

				nTotalWidth += nCurrPanelWidth;
				if (i > 0)
				{
					nTotalWidth += nPanelOffsetX;
				}

				if (pPanel->m_nCurrWidthIndex < nLastTruncatedLevel &&
					nCurrPanelWidth > nMaxPanelWidth &&
					pPanel->m_nCurrWidthIndex < pPanel->m_arWidths.GetSize () - 1)
				{
					nMaxPanelWidth = nCurrPanelWidth;
					pWidestPanel = pPanel;
				}
			}

			int x = rectClient.left;
			for (i = 0; i < m_arPanels.GetSize (); i++)
			{
				CBCGPRibbonPanel* pPanel = m_arPanels [i];
				ASSERT_VALID (pPanel);

				const int nCurrPanelWidth = 
					pPanel->m_arWidths [pPanel->m_nCurrWidthIndex] + 
					2 * pPanel->m_nXMargin;

				CRect rectPanel = CRect (x, rectClient.top, 
										x + nCurrPanelWidth, rectClient.bottom);

				pPanel->Repos (pDC, rectPanel);
				x = pPanel->m_rect.right + nPanelOffsetX;
			}

			if (x <= rectClient.right || bAllAreTruncated)
			{
				break;
			}

			if (pWidestPanel == NULL)
			{
				nLastTruncatedLevel = 999;
			}
			else
			{
				ASSERT_VALID (pWidestPanel);
				nLastTruncatedLevel = ++pWidestPanel->m_nCurrWidthIndex;
			}
		}
	}

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->OnAfterChangeRect (pDC);
	}
}
//******************************************************************************************
void CBCGPRibbonCategory::RecalcPanelWidths (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	int nHeight = GetMaxHeight (pDC);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		if (pPanel->m_arWidths.GetSize () == 0)
		{
			pPanel->RecalcWidths (pDC, nHeight);
			m_nLastCategoryWidth = -1;
		}
	}
}
//******************************************************************************************
void CBCGPRibbonCategory::CleanUpSizes ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->CleanUpSizes ();

		pPanel->m_arWidths.RemoveAll ();
	}

	m_nLastCategoryWidth = -1;
}
//******************************************************************************************
int CBCGPRibbonCategory::GetMinWidth (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	int nMinWidth = nPanelOffsetX;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		nMinWidth += pPanel->GetMinWidth (pDC) + nPanelOffsetX;
	}

	return nMinWidth + nPanelOffsetX;
}
//******************************************************************************************
void CBCGPRibbonCategory::OnMouseMove(CPoint point) 
{
	ASSERT_VALID (this);
	HighlightPanel (GetPanelFromPoint (point), point);
}
//******************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::HitTest (CPoint point, BOOL bCheckPanelCaption) const
{
	ASSERT_VALID (this);

	CBCGPRibbonPanel* pPanel = GetPanelFromPoint (point);
	if (pPanel != NULL)
	{
		ASSERT_VALID (pPanel);
		return pPanel->HitTest (point, bCheckPanelCaption);
	}

	return NULL;
}
//******************************************************************************************
CBCGPRibbonPanel* CBCGPRibbonCategory::GetPanelFromPoint (CPoint point) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		if (pPanel->m_rect.PtInRect (point))
		{
			return pPanel;
		}
	}

	return NULL;
}
//******************************************************************************************
CBCGPRibbonPanel* CBCGPRibbonCategory::HighlightPanel (CBCGPRibbonPanel* pHLPanel, CPoint point)
{
	ASSERT_VALID (this);

	CBCGPRibbonPanel* pPrevHLPanel = NULL;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		if (pPanel->IsHighlighted ())
		{
			if (pHLPanel != pPanel)
			{
				pPanel->Highlight (FALSE, point);
			}

			pPrevHLPanel = pPanel;
		}
		
		if (pHLPanel == pPanel)
		{
			pPanel->Highlight (TRUE, point);
		}
	}

	if (m_pParentMenuBar != NULL)
	{
		ASSERT_VALID (m_pParentMenuBar);
		m_pParentMenuBar->UpdateWindow ();
	}
	else
	{
		ASSERT_VALID (m_pParenrRibbonBar);
		m_pParenrRibbonBar->UpdateWindow ();
	}

	return pPrevHLPanel;
}
//******************************************************************************************
void CBCGPRibbonCategory::OnCancelMode() 
{
	m_bMouseIsPressed = FALSE;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->CancelMode ();
	}
}
//******************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::OnLButtonDown (CPoint point)
{
	CBCGPRibbonPanel* pPanel = GetPanelFromPoint (point);
	if (pPanel == NULL)
	{
		return NULL;
	}

	m_bMouseIsPressed = TRUE;

	ASSERT_VALID (pPanel);
	return pPanel->MouseButtonDown (point);
}
//******************************************************************************************
void CBCGPRibbonCategory::OnLButtonUp (CPoint point) 
{
	CBCGPRibbonPanel* pPanel = GetPanelFromPoint (point);
	if (pPanel == NULL)
	{
		return;
	}

	m_bMouseIsPressed = FALSE;

	ASSERT_VALID (pPanel);
	pPanel->MouseButtonUp (point);
}
//********************************************************************************
void CBCGPRibbonCategory::OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI, 
										 CFrameWnd* pTarget, 
										 BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->OnUpdateCmdUI (pCmdUI, pTarget, bDisableIfNoHndler);
	}
}
//********************************************************************************
BOOL CBCGPRibbonCategory::NotifyControlCommand (
	BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		if (pPanel->NotifyControlCommand (bAccelerator, nNotifyCode, wParam, lParam))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//********************************************************************************
void CBCGPRibbonCategory::SetActive (BOOL bIsActive)
{
	ASSERT_VALID (this);

	if (m_bIsActive == bIsActive)
	{
		return;
	}

	if ((m_pParenrRibbonBar->m_dwHideFlags & BCGPRIBBONBAR_HIDE_ELEMENTS) != 0)
	{
		m_bIsActive = bIsActive;
		return;
	}

	ShowElements  ();

	m_bIsActive = bIsActive;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->OnShow (bIsActive);
	}
}
//********************************************************************************
void CBCGPRibbonCategory::ShowElements  (BOOL bShow)
{
	ASSERT_VALID (this);

	GetParentRibbonBar ()->m_dwHideFlags = 
		bShow ? 0 : BCGPRIBBONBAR_HIDE_ELEMENTS;

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->OnShow (bShow);
	}

	GetParentRibbonBar ()->GetParentFrame ()->RecalcLayout ();
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::FindByID (UINT uiCmdID,
													   BOOL bVisibleOnly) const
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		CBCGPBaseRibbonElement* pElem = pPanel->FindByID (uiCmdID);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	if (!bVisibleOnly)
	{
		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->GetID () == uiCmdID)
			{
				return pElem;
			}
		}
	}

	return NULL;
}
//*************************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::FindByData (DWORD_PTR dwData,
													     BOOL bVisibleOnly) const
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		CBCGPBaseRibbonElement* pElem = pPanel->FindByData (dwData);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	if (!bVisibleOnly)
	{
		for (i = 0; i < m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->GetData () == dwData)
			{
				return pElem;
			}
		}
	}

	return NULL;
}
//*************************************************************************************
void CBCGPRibbonCategory::GetElementsByID (UINT uiCmdID, 
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->GetElementsByID (uiCmdID, arButtons);
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->GetID () == uiCmdID)
		{
			arButtons.Add (pElem);
		}
	}
}
//*************************************************************************************
void CBCGPRibbonCategory::GetItemIDsList (CList<UINT,UINT>& lstItems, BOOL bHiddenOnly) const
{
	ASSERT_VALID (this);

	int i = 0;

	if (!bHiddenOnly)
	{
		for (i = 0; i < m_arPanels.GetSize (); i++)
		{
			CBCGPRibbonPanel* pPanel = m_arPanels [i];
			ASSERT_VALID (pPanel);

			pPanel->GetItemIDsList (lstItems);
		}
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetItemIDsList (lstItems);
	}
}
//*************************************************************************************
CBCGPRibbonPanel* CBCGPRibbonCategory::FindPanelWithElem (const CBCGPBaseRibbonElement* pElement)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		if (pPanel->HasElement (pElement))
		{
			return pPanel;
		}
	}

	return NULL;
}
//*************************************************************************************
void CBCGPRibbonCategory::AddHidden (CBCGPBaseRibbonElement* pElement)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);

	pElement->m_pParent = this;
	m_arElements.Add (pElement);
}
//*************************************************************************************
BOOL CBCGPRibbonCategory::OnDrawImage (CDC* pDC, CRect rect, 
		CBCGPBaseRibbonElement* pElement, BOOL bIsLargeImage,
		BOOL nImageIndex, BOOL bCenter)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);

	CBCGPToolBarImages& image = bIsLargeImage ? m_LargeImages : m_SmallImages;

	if (nImageIndex >= image.GetCount ())
	{
		return FALSE;
	}

	CBCGPDrawState ds;

	CPoint ptImage = rect.TopLeft ();

	if (bCenter)
	{
		const CSize sizeImage = image.GetImageSize ();

		ptImage.Offset (
			max (0, (rect.Width () - sizeImage.cx) / 2), 
			max (0, (rect.Height () - sizeImage.cy) / 2));
	}

	image.SetTransparentColor (globalData.clrBtnFace);

	image.PrepareDrawImage (ds);
	image.Draw (pDC, ptImage.x, ptImage.y, nImageIndex, FALSE, 
		pElement->IsDisabled ());

	image.EndDrawImage (ds);
	return TRUE;
}
//*************************************************************************************
CSize CBCGPRibbonCategory::GetImageSize (BOOL bIsLargeImage) const
{
	ASSERT_VALID (this);

	const CBCGPToolBarImages& image = bIsLargeImage ? m_LargeImages : m_SmallImages;
	return image.GetImageSize ();
}
//*************************************************************************************
void CBCGPRibbonCategory::GetElements (
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	arElements.RemoveAll ();

	int i = 0;

	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		pPanel->GetElements (arElements);
	}

	//---------------------
	// Add hidden elements:
	//---------------------
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		arElements.Add (pElem);
	}
}
//*******************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::GetDroppedDown ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanel = m_arPanels [i];
		ASSERT_VALID (pPanel);

		CBCGPBaseRibbonElement* pElem = pPanel->GetDroppedDown ();
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	return NULL;
}
//*******************************************************************************
void CBCGPRibbonCategory::ShowFloating (CRect rectFloating)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParenrRibbonBar);

	if ((m_pParenrRibbonBar->m_dwHideFlags & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (this, rectFloating.Size ());

	m_Tab.SetDroppedDown (pMenu);
	pMenu->Create (m_pParenrRibbonBar, rectFloating.left, rectFloating.top, (HMENU) NULL);
}
//*******************************************************************************
void CBCGPRibbonCategory::CopyFrom (CBCGPRibbonCategory& src)
{
	ASSERT_VALID (this);

	int i = 0;

	m_strName = src.m_strName;
	m_bIsActive = src.m_bIsActive;
	m_bIsVisible = src.m_bIsVisible;

	for (i = 0; i < src.m_arPanels.GetSize (); i++)
	{
		CBCGPRibbonPanel* pPanelSrc = src.m_arPanels [i];
		ASSERT_VALID (pPanelSrc);

		CBCGPRibbonPanel* pPanel = 
			(CBCGPRibbonPanel*) pPanelSrc->GetRuntimeClass ()->CreateObject ();
		ASSERT_VALID (pPanel);

		pPanel->CopyFrom (*pPanelSrc);

		pPanel->m_btnDefault.CopyFrom (pPanelSrc->m_btnDefault);
		pPanel->m_btnDefault.SetOriginal (&pPanelSrc->m_btnDefault);

		m_arPanels.Add (pPanel);
	}

	m_pParenrRibbonBar = src.m_pParenrRibbonBar;
	m_rect = src.m_rect;
	m_Tab.CopyFrom (src.m_Tab);

	m_dwData = src.m_dwData;
	m_uiContextID = src.m_uiContextID;
	m_pParentMenuBar = src.m_pParentMenuBar;

	src.m_SmallImages.CopyTo (m_SmallImages);
	src.m_LargeImages.CopyTo (m_LargeImages);
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonCategory::GetParentButton () const
{
	ASSERT_VALID (this);

	if (m_pParentMenuBar == NULL)
	{
		return NULL;
	}

	return ((CBCGPPopupMenu*)m_pParentMenuBar->GetParent ())->GetParentRibbonElement ();
}

#endif // BCGP_EXCLUDE_RIBBON
