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
// BCGPBaseRibbonElement.cpp: implementation of the CBCGPBaseRibbonElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPBaseRibbonElement.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPRibbonCategory.h"
#include "BCGPRibbonBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPKeyboardManager.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonCommandsListBox.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static inline BOOL IsSystemCommand (UINT uiCmd)
{
	return (uiCmd >= 0xF000 && uiCmd < 0xF1F0);
}

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonSeparator

IMPLEMENT_DYNCREATE (CBCGPRibbonSeparator, CBCGPBaseRibbonElement)

CBCGPRibbonSeparator::CBCGPRibbonSeparator (BOOL bIsHoriz)
{
	m_bIsHoriz = bIsHoriz;
}
//******************************************************************************
CSize CBCGPRibbonSeparator::GetRegularSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);
	return CSize (4, 4);
}
//******************************************************************************
void CBCGPRibbonSeparator::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::CopyFrom (s);

	const CBCGPRibbonSeparator& src = (const CBCGPRibbonSeparator&) s;
	m_bIsHoriz = src.m_bIsHoriz;
}
//******************************************************************************
void CBCGPRibbonSeparator::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CRect rect = m_rect;

	if (m_bQuickAccessMode)
	{
		rect.left = rect.CenterPoint ().x - 1;
		rect.right = rect.left + 2;

		rect.DeflateRect (0, 3);

		CBCGPVisualManager::GetInstance ()->OnDrawRibbonQATSeparator (pDC, this, rect);
		return;
	}

	if (m_bIsHoriz)
	{
		rect.top = rect.CenterPoint ().y;
		rect.bottom = rect.top + 1;
	}
	else
	{
		rect.left = rect.CenterPoint ().x;
		rect.right = rect.left + 1;

		rect.DeflateRect (0, 5);
	}

	CBCGPBaseControlBar* pParentBar = NULL;

	if (m_pParentMenu != NULL)
	{
		pParentBar = m_pParentMenu;
	}
	else
	{
		pParentBar = GetTopLevelRibbonBar ();
	}

	if (pParentBar != NULL)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawSeparator (pDC, 
			pParentBar, rect, !m_bIsHoriz);
	}
}
//*************************************************************************************
int CBCGPRibbonSeparator::AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL /*bDeep*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndListBox);
	ASSERT (pWndListBox->GetSafeHwnd () != NULL);

	CString strText;

	{
		CBCGPLocalResource locaRes;
		strText.LoadString (IDS_BCGBARRES_QAT_SEPARATOR);
	}

	int nIndex = pWndListBox->AddString (_T(" ") + strText);	// Should be always first!
	pWndListBox->SetItemData (nIndex, (DWORD_PTR) this);

	return nIndex;
}
//*************************************************************************************
void CBCGPRibbonSeparator::OnDrawOnList (CDC* pDC, CString strText, int nTextOffset, CRect rect, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	const int xMargin = 3;

	rect.DeflateRect (xMargin, 0);
	rect.left += nTextOffset;

	pDC->DrawText (strText, rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	m_bIsDisabled = bIsDisabled;
}

//////////////////////////////////////////////////////////////////////
// CBCGPBaseRibbonElement

IMPLEMENT_DYNAMIC(CBCGPBaseRibbonElement, CObject)

CBCGPBaseRibbonElement::CBCGPBaseRibbonElement ()
{
	m_Location = RibbonElementNotInGroup;
	m_nID = 0;
	m_dwData = 0;
	m_rect.SetRectEmpty ();
	m_pParent = NULL;
	m_pParentGroup = NULL;
	m_pParentMenu = NULL;
	m_bCompactMode = FALSE;
	m_bQuickAccessMode = FALSE;
	m_bIsHighlighted = FALSE;
	m_bIsFocused = FALSE;
	m_bIsPressed = FALSE;
	m_bIsDisabled = FALSE;
	m_bIsChecked = FALSE;
	m_bIsDroppedDown = FALSE;
	m_pOriginal = NULL;
	m_pRibbonBar = NULL;
	m_nRow = -1;
	m_bDontNotify = FALSE;
	m_bTextAlwaysOnRight = FALSE;
	m_pPopupMenu = NULL;
	m_nImageOffset = 0;
	m_bShowGroupBorder = FALSE;
	m_bIsVisible = TRUE;
}
//******************************************************************************
CBCGPBaseRibbonElement::~CBCGPBaseRibbonElement ()
{
	if (m_pPopupMenu != NULL)
	{
		ASSERT_VALID (m_pPopupMenu);
		ASSERT (m_pPopupMenu->m_pParentRibbonElement == this);

		m_pPopupMenu->m_pParentRibbonElement = NULL;

		ClosePopupMenu ();
	}
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetID (UINT nID)
{
	ASSERT_VALID (this);
	m_nID = nID;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetText (LPCTSTR lpszText)
{
	ASSERT_VALID (this);
	m_strText = lpszText == NULL ? _T("") : lpszText;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetTextAlwaysOnRight (BOOL bSet)
{
	ASSERT_VALID (this);
	m_bTextAlwaysOnRight = bSet;
}
//******************************************************************************
void CBCGPBaseRibbonElement::OnLButtonDown (CPoint /*point*/)
{
	CBCGPRibbonPanel* pPanel = GetParentPanel ();
	if (pPanel != NULL)
	{
		ASSERT_VALID (pPanel);

		CBCGPBaseRibbonElement* pDroppedDown = pPanel->GetDroppedDown ();
		if (pDroppedDown != NULL)
		{
			ASSERT_VALID (pDroppedDown);
			pDroppedDown->ClosePopupMenu ();
		}
	}

	if (m_pParentMenu != NULL)
	{
		return;
	}

	if (m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_pRibbonBar);

		CBCGPBaseRibbonElement* pDroppedDown = m_pRibbonBar->GetDroppedDown ();
		if (pDroppedDown != NULL)
		{
			ASSERT_VALID (pDroppedDown);
			pDroppedDown->ClosePopupMenu ();
		}
	}

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);

		CBCGPBaseRibbonElement* pDroppedDown = m_pParent->GetDroppedDown ();
		if (pDroppedDown != NULL)
		{
			ASSERT_VALID (pDroppedDown);
			pDroppedDown->ClosePopupMenu ();
		}
	}
}
//******************************************************************************
void CBCGPBaseRibbonElement::OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI,
											CFrameWnd* pTarget,
											BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);
	ASSERT (pCmdUI != NULL);

	if (m_nID == 0 || IsSystemCommand (m_nID) ||
		m_nID >= AFX_IDM_FIRST_MDICHILD)
	{
		return;
	}

	pCmdUI->m_pUpdated = this;

	pCmdUI->m_nID = m_nID;
	pCmdUI->DoUpdate (pTarget, bDisableIfNoHndler);

	pCmdUI->m_pUpdated = NULL;
}
//******************************************************************************
BOOL CBCGPBaseRibbonElement::NotifyControlCommand (
	BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(bAccelerator);
	UNREFERENCED_PARAMETER(nNotifyCode);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return FALSE;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::HitTest (CPoint /*point*/)
{
	ASSERT_VALID (this);
	return this;
}
//******************************************************************************
CBCGPRibbonBar* CBCGPBaseRibbonElement::GetTopLevelRibbonBar () const
{
	ASSERT_VALID (this);

	if (m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_pRibbonBar);
		return m_pRibbonBar;
	}

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);

		if (m_pParent->GetParentRibbonBar () != NULL)
		{
			ASSERT_VALID (m_pParent->GetParentRibbonBar ());
			return m_pParent->GetParentRibbonBar ();
		}
	}

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		return m_pParentMenu->GetTopLevelRibbonBar ();
	}

	return NULL;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetDroppedDown (CBCGPPopupMenu* pPopupMenu)
{
	ASSERT_VALID (this);

	m_pPopupMenu = pPopupMenu;

	if (pPopupMenu != NULL)
	{
		ASSERT_VALID (pPopupMenu);
		pPopupMenu->SetParentRibbonElement (this);
	}

	BOOL bWasDroppedDown = m_bIsDroppedDown;
	m_bIsDroppedDown = pPopupMenu != NULL;

	if (!m_bIsDroppedDown)
	{
		m_bIsHighlighted = m_bIsPressed = FALSE;
	}

	if (bWasDroppedDown != m_bIsDroppedDown)
	{
		Redraw ();
	}

	if (m_pParentMenu->GetSafeHwnd () != NULL && pPopupMenu == NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		ASSERT_VALID (m_pParentMenu->GetParent ());

		CBCGPPopupMenu::m_pActivePopupMenu = 
			(CBCGPPopupMenu*) m_pParentMenu->GetParent ();
	}
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::FindByID (UINT uiCmdID)
{
	ASSERT_VALID (this);

	return (m_nID == uiCmdID) ? this : NULL;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::FindByData (DWORD_PTR dwData)
{
	ASSERT_VALID (this);

	return (m_dwData == dwData) ? this : NULL;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::GetPressed ()
{
	ASSERT_VALID (this);

	return IsPressed () ? this : NULL;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::GetDroppedDown ()
{
	ASSERT_VALID (this);

	return IsDroppedDown () ? this : NULL;
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPBaseRibbonElement::GetHighlighted ()
{
	ASSERT_VALID (this);

	return IsHighlighted () ? this : NULL;
}
//******************************************************************************
BOOL CBCGPBaseRibbonElement::ReplaceByID (UINT uiCmdID, CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	UNREFERENCED_PARAMETER (uiCmdID);
	UNREFERENCED_PARAMETER (pElem);

	return FALSE;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetParentCategory (CBCGPRibbonCategory* pParent)
{
	ASSERT_VALID (this);
	m_pParent = pParent;
}
//******************************************************************************
void CBCGPBaseRibbonElement::CopyFrom (const CBCGPBaseRibbonElement& src)
{
	ASSERT_VALID (this);

	m_nID = src.m_nID;
	m_dwData = src.m_dwData;
	m_bTextAlwaysOnRight = src.m_bTextAlwaysOnRight;
	m_strText = src.m_strText;
	m_pParent = src.m_pParent;
	m_pParentGroup = src.m_pParentGroup;
	m_strToolTip = src.m_strToolTip;
	m_strDescription = src.m_strDescription;
	m_bQuickAccessMode = src.m_bQuickAccessMode;
	m_bIsVisible = src.m_bIsVisible;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetParentMenu (CBCGPRibbonPanelMenuBar* pMenuBar)
{
	ASSERT_VALID (this);

	m_pParentMenu = pMenuBar;
}
//*****************************************************************************
void CBCGPBaseRibbonElement::SetOriginal (CBCGPBaseRibbonElement* pOriginal)
{
	ASSERT_VALID (this);

	if (pOriginal != NULL)
	{
		ASSERT_VALID (pOriginal);

		while (pOriginal->m_pOriginal != NULL)
		{
			pOriginal = pOriginal->m_pOriginal;
		}
	}

	m_pOriginal = pOriginal;
}
//******************************************************************************
CWnd* CBCGPBaseRibbonElement::GetParentWnd () const
{
	ASSERT_VALID (this);

	if (m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_pRibbonBar);
		return m_pRibbonBar;
	}
	else if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		return m_pParentMenu;
	}
	else if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		ASSERT_VALID (m_pParent->GetParentRibbonBar ());

		return m_pParent->GetParentRibbonBar ();
	}

	return NULL;
}
//******************************************************************************
CBCGPRibbonPanel* CBCGPBaseRibbonElement::GetParentPanel () const
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);

		if (m_pParentMenu->m_pCategory != NULL)
		{
			ASSERT_VALID (m_pParentMenu->m_pCategory);
			return m_pParentMenu->m_pCategory->FindPanelWithElem (m_pParentGroup == NULL ? this : m_pParentGroup);
		}

		return m_pParentMenu->m_pPanel;
	}
	else if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		return m_pParent->FindPanelWithElem (m_pParentGroup == NULL ? this : m_pParentGroup);
	}

	return NULL;
}
//******************************************************************************
BOOL CBCGPBaseRibbonElement::IsMenuMode () const
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);

		if (m_pParentMenu->m_pPanel == NULL)
		{
			return FALSE;
		}

		ASSERT_VALID (m_pParentMenu->m_pPanel);

		return m_pParentMenu->m_pPanel->IsMenuMode ();
	}

	return FALSE;
}
//******************************************************************************
void CBCGPBaseRibbonElement::Redraw ()
{
	ASSERT_VALID (this);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_pParentMenu->GetSafeHwnd () != NULL)
	{
		m_pParentMenu->RedrawWindow (m_rect);
		return;
	}

	CWnd* pWndParent = GetParentWnd ();

	if (pWndParent->GetSafeHwnd () != NULL)
	{
		pWndParent->RedrawWindow (m_rect);
	}
}
//******************************************************************************
CString CBCGPBaseRibbonElement::GetToolTipText () const
{
	ASSERT_VALID (this);

	if (IsDroppedDown ())
	{
		return _T("");
	}

	CString strTipText = m_strToolTip;

	//--------------------
	// Add shortcut label:
	//--------------------
	CWnd* pWndParent = NULL;

	if (m_pRibbonBar != NULL)
	{
		pWndParent = m_pRibbonBar;
	}
	else if (m_pParentMenu != NULL)
	{
		pWndParent = m_pParentMenu;
	}
	else if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		pWndParent = m_pParent->GetParentRibbonBar ();
	}

	CString strLabel;
	CFrameWnd* pParentFrame = BCGPGetParentFrame (pWndParent) == NULL ?
		NULL : BCGCBProGetTopLevelFrame (BCGPGetParentFrame (pWndParent));

	if (pParentFrame != NULL &&
		(CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strLabel, pParentFrame, TRUE) ||
		CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strLabel, pParentFrame->GetActiveFrame (), FALSE)))
	{
		strTipText += _T(" (");
		strTipText += strLabel;
		strTipText += _T(')');
	}

	return strTipText;
}
//******************************************************************************
CString CBCGPBaseRibbonElement::GetDescription () const
{
	ASSERT_VALID (this);
	return m_strDescription;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetToolTipText (LPCTSTR lpszText)
{
	ASSERT_VALID (this);
	m_strToolTip = lpszText == NULL ? _T("") : lpszText;
}
//******************************************************************************
void CBCGPBaseRibbonElement::SetDescription (LPCTSTR lpszText)
{
	ASSERT_VALID (this);
	m_strDescription = lpszText == NULL ? _T("") : lpszText;
}
//******************************************************************************
void CBCGPBaseRibbonElement::UpdateTooltipInfo ()
{
	ASSERT_VALID (this);

	m_strToolTip.Empty ();
	m_strDescription.Empty ();

	if (m_nID == 0 || m_nID == (UINT)-1)
	{
		return;
	}

	CString strText;
	strText.LoadString (m_nID);

	if (strText.IsEmpty ())
	{
		return;
	}

	AfxExtractSubString (m_strDescription, strText, 0);
	AfxExtractSubString (m_strToolTip, strText, 1, '\n');
}
//******************************************************************************
void CBCGPBaseRibbonElement::OnAfterChangeRect (CDC* /*pDC*/)
{
	ASSERT_VALID (this);

	if (m_strToolTip.IsEmpty ())
	{
		UpdateTooltipInfo ();
	}
}
//******************************************************************************
BOOL CBCGPBaseRibbonElement::NotifyCommand (BOOL bWithDelay)
{
	ASSERT_VALID (this);

	if (m_pOriginal != NULL)
	{
		ASSERT_VALID (m_pOriginal);
		return m_pOriginal->NotifyCommand (bWithDelay);
	}

	UINT uiID = GetNotifyID ();

	if (uiID == 0 || uiID == (UINT)-1)
	{
		return FALSE;
	}

	CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
	if (pRibbonBar == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pRibbonBar);

	CWnd* pWndParent = pRibbonBar->GetParent ();
	if (pWndParent == NULL)
	{
		return FALSE;
	}

	m_bIsHighlighted = m_bIsPressed = m_bIsFocused = FALSE;
	Redraw ();

	ASSERT_VALID (pWndParent);

	if (bWithDelay)
	{
		pWndParent->PostMessage (WM_COMMAND, uiID);
	}
	else
	{
		pWndParent->SendMessage (WM_COMMAND, uiID);
	}

	return TRUE;
}
//*************************************************************************************
void CBCGPBaseRibbonElement::PostMenuCommand (UINT /*uiCmdId*/)
{
	ASSERT_VALID (this);

	m_bIsDroppedDown = FALSE;
	Redraw ();

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		m_pParentMenu->GetParent ()->SendMessage (WM_CLOSE);
	}
}
//*************************************************************************************
void CBCGPBaseRibbonElement::GetElementsByID (UINT uiCmdID,
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	if (uiCmdID == m_nID)
	{
		arElements.Add (this);
	}
}
//*************************************************************************************
void CBCGPBaseRibbonElement::GetItemIDsList (CList<UINT,UINT>& lstItems) const
{
	ASSERT_VALID (this);

	if (m_nID != 0 && m_nID != (UINT)-1 && lstItems.Find (m_nID) == NULL)
	{
		lstItems.AddTail (m_nID);
	}
}
//*************************************************************************************
int CBCGPBaseRibbonElement::AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL /*bDeep*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndListBox);
	ASSERT (pWndListBox->GetSafeHwnd () != NULL);

	if (m_nID == 0 || m_nID == (UINT)-1)
	{
		return -1;
	}

	if (m_nID >= ID_FILE_MRU_FILE1 && m_nID <= ID_FILE_MRU_FILE16)
	{
		return -1;
	}

	UpdateTooltipInfo ();

	CString strText = m_strToolTip;
	if (strText.IsEmpty ())
	{
		strText = GetText ();
	}

	const CString strDummyAmpSeq = _T("\001\001");

	strText.Replace (_T("&&"), strDummyAmpSeq);
	strText.Remove (_T('&'));
	strText.Replace (strDummyAmpSeq, _T("&"));

	int nIndex = pWndListBox->AddString (strText);
	pWndListBox->SetItemData (nIndex, (DWORD_PTR) this);

	return nIndex;
}
//*************************************************************************************
void CBCGPBaseRibbonElement::ClosePopupMenu ()
{
	ASSERT_VALID (this);

	if (m_pPopupMenu != NULL && ::IsWindow (m_pPopupMenu->m_hWnd))
	{
		if (m_pPopupMenu->InCommand ())
		{
			return;
		}

		m_pPopupMenu->m_bAutoDestroyParent = FALSE;
		m_pPopupMenu->CloseMenu ();
	}

	m_pPopupMenu = NULL;
}
//*************************************************************************************
BOOL CBCGPBaseRibbonElement::CanBeAddedToQAT () const
{
	ASSERT_VALID (this);

	return m_nID != 0 && m_nID != (UINT)-1 && !IsStandardCommand (m_nID);
}
//*************************************************************************************
BOOL CBCGPBaseRibbonElement::OnAddToQAToolbar (CBCGPRibbonQuickAccessToolbar& qat)
{
	qat.Add (this);
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPBaseRibbonElement::StretchToWholeRow (CDC* /*pDC*/, int nHeight)
{
	ASSERT_VALID (this);

	if (!CanBeStretched () || m_bCompactMode)
	{
		return FALSE;
	}

	m_rect.bottom = m_rect.top + nHeight;
	return TRUE;
}

#endif // BCGP_EXCLUDE_RIBBON
