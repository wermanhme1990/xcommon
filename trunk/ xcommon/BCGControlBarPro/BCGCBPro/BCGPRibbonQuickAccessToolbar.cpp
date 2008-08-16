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
// BCGPRibbonQuickAccessToolbar.cpp: implementation of the CBCGPRibbonQuickAccessToolbar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonCategory.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonPanelMenu.h"
#include "menuimages.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////
// CBCGPRibbonQuickAccessCustomizeButton class

CBCGPRibbonQATDefaultState::CBCGPRibbonQATDefaultState()
{
}

void CBCGPRibbonQATDefaultState::AddCommand (UINT uiCmd, BOOL bIsVisible/* = TRUE*/)
{
	m_arCommands.Add (uiCmd);
	m_arVisibleState.Add (bIsVisible);
}

void CBCGPRibbonQATDefaultState::RemoveAll ()
{
	m_arCommands.RemoveAll ();
	m_arVisibleState.RemoveAll ();
}

void CBCGPRibbonQATDefaultState::CopyFrom (const CBCGPRibbonQATDefaultState& src)
{
	RemoveAll ();

	for (int i = 0; i < src.m_arCommands.GetSize (); i++)
	{
		AddCommand (src.m_arCommands [i], src.m_arVisibleState [i]);
	}
}

////////////////////////////////////////////////////////
// CBCGPRibbonQuickAccessCustomizeButton class

class BCGCBPRODLLEXPORT CBCGPRibbonQuickAccessCustomizeButton : public CBCGPRibbonButton
{
	friend class CBCGPRibbonQuickAccessToolbar;

	DECLARE_DYNCREATE(CBCGPRibbonQuickAccessCustomizeButton)

	CBCGPRibbonQuickAccessCustomizeButton ()
	{
		CBCGPLocalResource locaRes;
		m_strMoreButtons.LoadString (IDS_BCGBARRES_MORE_BUTTONS);
	}

	virtual CSize GetImageSize (RibbonImageType /*type*/) const
	{
		ASSERT_VALID (this);
		return CMenuImages::Size ();
	}

	virtual CSize GetCompactSize (CDC* pDC)
	{
		ASSERT_VALID (this);

		CSize size = CBCGPRibbonButton::GetCompactSize (pDC);
		size.cx = CMenuImages::Size ().cx + 4;
		size.cy = CMenuImages::Size ().cy * 2;

		return size;
	}

	virtual BOOL HasMenu () const
	{
		return TRUE;
	}

	virtual void OnShowPopupMenu ()
	{
		OnLButtonDown (m_rect.TopLeft ());
	}

	virtual void OnDraw (CDC* pDC)
	{
		ASSERT_VALID (this);
		ASSERT_VALID (pDC);

		if (m_rect.IsRectEmpty ())
		{
			return;
		}

		CBCGPVisualManager::GetInstance ()->OnFillRibbonButton (pDC, this);

		CMenuImages::IMAGES_IDS nImageIndex = m_arHidden.GetSize () == 0 ? 
			CMenuImages::IdCustomizeArowDown :
			CMenuImages::IdCustomizeMoreButtonsHorz;

		CRect rectWhite = m_rect;
		rectWhite.OffsetRect (0, 1);

		CMenuImages::Draw (pDC, nImageIndex, rectWhite, CMenuImages::ImageWhite);
		CMenuImages::Draw (pDC, nImageIndex, m_rect);
		
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonButtonBorder 
			(pDC, this);
	}

	virtual void OnLButtonDown (CPoint /*point*/)
	{
		ASSERT_VALID (this);
		ASSERT_VALID (m_pRibbonBar);

		if (m_bIsDroppedDown)
		{
			m_bIsHighlighted = TRUE;
			Redraw ();
			return;
		}

		CWnd* pWndParent = m_pRibbonBar;

		if (m_pParentMenu != NULL)
		{
			pWndParent = m_pParentMenu;
		}

		ASSERT_VALID (pWndParent);

		HWND hwndParent = pWndParent->GetSafeHwnd ();

		CRect rectBtn = m_rect;
		pWndParent->ClientToScreen (&rectBtn);

		if (m_arHidden.GetSize () == 0)
		{
			m_bIsDroppedDown = TRUE;

			if (!m_pRibbonBar->OnShowRibbonQATMenu (pWndParent, 
				rectBtn.left, rectBtn.bottom, this) &&
				::IsWindow (hwndParent))
			{
				m_bIsDroppedDown = FALSE;
				Redraw ();
			}
		}
		else
		{		
			CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (
				m_pRibbonBar,
				m_arHidden);

			((CBCGPRibbonPanelMenuBar*)pMenu->GetMenuBar ())->m_bIsQATPopup = TRUE;

			pMenu->SetParentRibbonElement (this);
			pMenu->Create (m_pRibbonBar, rectBtn.left, rectBtn.bottom, (HMENU) NULL);
			
			SetDroppedDown (pMenu);
		}
	}

	virtual void CopyFrom (const CBCGPBaseRibbonElement& s)
	{
		ASSERT_VALID (this);

		CBCGPRibbonButton::CopyFrom (s);

		CBCGPRibbonQuickAccessCustomizeButton& src = (CBCGPRibbonQuickAccessCustomizeButton&) s;
		m_pRibbonBar = src.m_pRibbonBar;

		m_strMoreButtons = src.m_strMoreButtons;
	}

	virtual CString GetToolTipText () const
	{
		ASSERT_VALID (this);

		if (IsDroppedDown ())
		{
			return _T("");
		}

		if (m_arHidden.GetSize () == 0)
		{
			return CBCGPRibbonButton::GetToolTipText ();
		}

		return m_strMoreButtons;
	}

protected:
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	m_arHidden;
	CString														m_strMoreButtons;
};

IMPLEMENT_DYNCREATE(CBCGPRibbonQuickAccessCustomizeButton, CBCGPRibbonButton)

////////////////////////////////////////////////////////
// CBCGPRibbonQuickAccessToolbar class

IMPLEMENT_DYNCREATE(CBCGPRibbonQuickAccessToolbar, CBCGPRibbonButtonsGroup)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonQuickAccessToolbar::CBCGPRibbonQuickAccessToolbar()
{
	m_pRibbonBar = NULL;
	m_rectCommands.SetRectEmpty ();
}
//********************************************************************************
CBCGPRibbonQuickAccessToolbar::~CBCGPRibbonQuickAccessToolbar()
{
}
//********************************************************************************
void CBCGPRibbonQuickAccessToolbar::SetCommands (
	CBCGPRibbonBar* pRibbonBar,
	const CList<UINT,UINT>& lstCommands,
	LPCTSTR lpszToolTip)
{
	CBCGPRibbonQuickAccessCustomizeButton* pButton = new CBCGPRibbonQuickAccessCustomizeButton;
	pButton->SetToolTipText (lpszToolTip);

	SetCommands (pRibbonBar, lstCommands, pButton);
}
//********************************************************************************
void CBCGPRibbonQuickAccessToolbar::SetCommands (
	CBCGPRibbonBar* pRibbonBar,
	const CList<UINT,UINT>& lstCommands,
	CBCGPRibbonQuickAccessCustomizeButton* pCustButton)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRibbonBar);

	BOOL bSetByUser = TRUE;

	if (pCustButton == NULL)
	{
		bSetByUser = FALSE;

		// Leave existing:
		if (m_arButtons.GetSize () == 0)
		{
			return;
		}

		int nSize = (int) m_arButtons.GetSize ();

		pCustButton = DYNAMIC_DOWNCAST (CBCGPRibbonQuickAccessCustomizeButton,
			m_arButtons [nSize - 1]);
		m_arButtons.SetSize (nSize - 1);
	}
	
	RemoveAll ();

	ASSERT_VALID (pCustButton);

	m_pRibbonBar = pRibbonBar;

	BOOL bAddToState = bSetByUser && m_DefaultState.m_arCommands.GetSize () == 0;

	for (POSITION pos = lstCommands.GetHeadPosition (); pos != NULL;)
	{
		UINT uiCmd = lstCommands.GetNext (pos);

		if (uiCmd == 0)
		{
			CBCGPRibbonSeparator* pSeparator = new CBCGPRibbonSeparator (TRUE);
			
			pSeparator->m_pRibbonBar = m_pRibbonBar;
			pSeparator->m_bQuickAccessMode = TRUE;

			m_arButtons.Add (pSeparator);
			continue;
		}

		CBCGPBaseRibbonElement* pElem = pRibbonBar->FindByID (uiCmd, FALSE);
		if (pElem == NULL)
		{
			ASSERT (FALSE);
			continue;
		}

		CBCGPBaseRibbonElement* pButton =
			(CBCGPBaseRibbonElement*) pElem->GetRuntimeClass ()->CreateObject ();
		ASSERT_VALID (pButton);

		pButton->CopyFrom (*pElem);
		pButton->m_pRibbonBar = m_pRibbonBar;
		pButton->SetOriginal (pElem);
		pButton->m_bQuickAccessMode = TRUE;

		m_arButtons.Add (pButton);

		if (bAddToState)
		{
			m_DefaultState.AddCommand (uiCmd);
		}
	}

	pCustButton->m_pRibbonBar = m_pRibbonBar;
	m_arButtons.Add (pCustButton);
}
//********************************************************************************
void CBCGPRibbonQuickAccessToolbar::GetCommands (CList<UINT,UINT>& lstCommands)
{
	ASSERT_VALID (this);

	lstCommands.RemoveAll ();

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		if (pButton->GetID () != 0 || pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
		{
			lstCommands.AddTail (pButton->GetID ());
		}
	}
}
//*****************************************************************************
int CBCGPRibbonQuickAccessToolbar::GetActualWidth () const
{
	ASSERT_VALID (this);

	if (m_arButtons.GetSize () == 0)
	{
		return 0;
	}

	CBCGPBaseRibbonElement* pLastButton = m_arButtons [m_arButtons.GetSize () - 1];
	ASSERT_VALID (pLastButton);

	return pLastButton->m_rect.right - m_rect.left;
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pRibbonBar);

	CBCGPRibbonButtonsGroup::OnAfterChangeRect (pDC);
	RebuildHiddenItems ();

	m_rectCommands = m_rect;

	if (m_arButtons.GetSize () > 0)
	{
		CBCGPRibbonQuickAccessCustomizeButton* pCustButton = 
			DYNAMIC_DOWNCAST (CBCGPRibbonQuickAccessCustomizeButton,
			m_arButtons [m_arButtons.GetSize () - 1]);
		ASSERT_VALID (pCustButton);

		CRect rectChevron = pCustButton->GetRect ();

		m_rectCommands.right = rectChevron.left;

		if (m_pRibbonBar->IsQuickAccessToolbarOnTop ())
		{
			rectChevron.OffsetRect (CBCGPVisualManager::GetInstance ()->GetRibbonQATChevronOffset (), 0);

			if (CBCGPVisualManager::GetInstance ()->GetRibbonQATRightMargin () > 0)
			{
				rectChevron.top -= 2;
			}
		}

		pCustButton->SetRect (rectChevron);
		pCustButton->OnAfterChangeRect (pDC);
	}
}
//*****************************************************************************
CSize CBCGPRibbonQuickAccessToolbar::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pRibbonBar);

	CSize size = CBCGPRibbonButtonsGroup::GetRegularSize (pDC);

	if (m_pRibbonBar->IsQuickAccessToolbarOnTop ())
	{
		size.cx += CBCGPVisualManager::GetInstance ()->GetRibbonQATChevronOffset ();
	}

	return size;
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::RebuildHiddenItems ()
{
	ASSERT_VALID (this);

	if (m_arButtons.GetSize () == 0)
	{
		return;
	}

	CBCGPRibbonQuickAccessCustomizeButton* pCustButton = 
		DYNAMIC_DOWNCAST (CBCGPRibbonQuickAccessCustomizeButton,
		m_arButtons [m_arButtons.GetSize () - 1]);
	ASSERT_VALID (pCustButton);

	pCustButton->m_arHidden.RemoveAll ();

	for (int i = 0; i < m_arButtons.GetSize () - 1; i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		if (pButton->m_rect.IsRectEmpty ())
		{
			pCustButton->m_arHidden.Add (pButton);
		}
	}

	if (pCustButton->m_arHidden.GetSize () > 0)
	{
		pCustButton->m_arHidden.Add (pCustButton);
	}
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::ReplaceCommands (const CList<UINT,UINT>& lstCommands)
{
	ASSERT_VALID (this);
	SetCommands (m_pRibbonBar, lstCommands, 
		(CBCGPRibbonQuickAccessCustomizeButton*) NULL);
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::ResetCommands ()
{
	ASSERT_VALID (this);

	CList<UINT,UINT> lstCommands;
	GetDefaultCommands (lstCommands);

	SetCommands (m_pRibbonBar, lstCommands,
		(CBCGPRibbonQuickAccessCustomizeButton*) NULL);
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::Add (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	CBCGPBaseRibbonElement* pButton =
		(CBCGPBaseRibbonElement*) pElem->GetRuntimeClass ()->CreateObject ();
	ASSERT_VALID (pButton);

	pButton->CopyFrom (*pElem);
	pButton->m_pRibbonBar = m_pRibbonBar;

	if (pElem->m_pOriginal != NULL)
	{
		pButton->SetOriginal (pElem->m_pOriginal);
	}
	else
	{
		pButton->SetOriginal (pElem);
	}

	pButton->m_bQuickAccessMode = TRUE;

	m_arButtons.InsertAt (m_arButtons.GetSize () - 1, pButton);
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::Remove (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	for (int i = 0; i < m_arButtons.GetSize () - 1; i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		if (pButton->GetID () == pElem->GetID ())
		{
			m_arButtons.RemoveAt (i);
			delete pButton;

			RebuildHiddenItems ();
			return;
		}
	}
}
//*****************************************************************************
void CBCGPRibbonQuickAccessToolbar::GetDefaultCommands (CList<UINT,UINT>& lstCommands)
{
	ASSERT_VALID (this);

	lstCommands.RemoveAll ();

	for (int i = 0; i < m_DefaultState.m_arCommands.GetSize (); i++)
	{
		if (m_DefaultState.m_arVisibleState [i])
		{
			lstCommands.AddTail (m_DefaultState.m_arCommands [i]);
		}
	}
}

#endif // BCGP_EXCLUDE_RIBBON
