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
// BCGPRibbonFloaty.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPRibbonFloaty.h"
#include "BCGPRibbonBar.h"
#include "bcgglobals.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonFloaty

CBCGPRibbonFloaty* CBCGPRibbonFloaty::m_pCurrent = NULL;

CBCGPRibbonFloaty::CBCGPRibbonFloaty()
{
	if (m_pCurrent != NULL)
	{
		m_pCurrent->SendMessage (WM_CLOSE);
		m_pCurrent = NULL;
	}
}
//*******************************************************************************
CBCGPRibbonFloaty::~CBCGPRibbonFloaty()
{
	ASSERT (m_pCurrent == this);
	m_pCurrent = NULL;
}

BEGIN_MESSAGE_MAP(CBCGPRibbonFloaty, CBCGPRibbonPanelMenu)
	//{{AFX_MSG_MAP(CBCGPRibbonFloaty)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CBCGPRibbonFloaty::SetCommands (
	CBCGPRibbonBar* pRibbonBar,
	const CList<UINT,UINT>& lstCommands)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRibbonBar);

	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arButtons;

	for (POSITION pos = lstCommands.GetHeadPosition (); pos != NULL;)
	{
		UINT uiCmd = lstCommands.GetNext (pos);
		
		if (uiCmd == 0)
		{
			// TODO: add separator
			continue;
		}

		CBCGPBaseRibbonElement* pSrcElem = pRibbonBar->FindByID (uiCmd, FALSE);
		if (pSrcElem == NULL)
		{
			continue;
		}

		arButtons.Add (pSrcElem);
	}

	m_wndRibbonBar.AddButtons (pRibbonBar, arButtons, TRUE);
}
//*******************************************************************************
BOOL CBCGPRibbonFloaty::Show (int x, int y)
{
	ASSERT_VALID (this);

	CSize size = m_wndRibbonBar.CalcSize (FALSE);

	if (!Create (m_wndRibbonBar.m_pRibbonBar, 
		x - 20, y - size.cy - 20, (HMENU) NULL))
	{
		return FALSE;
	}

	m_pCurrent = this;
	SetActive (FALSE);

	return TRUE;
}
//*******************************************************************************
void CBCGPRibbonFloaty::SetActive (BOOL bIsActive)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_wndRibbonBar.GetPanel ());

	if (m_wndRibbonBar.GetPanel ()->GetDroppedDown () != NULL)
	{
		bIsActive = TRUE;
	}

	if (!bIsActive)
	{
		ModifyStyleEx (0, WS_EX_LAYERED);

		BYTE nTransparency = 100;
		globalData.SetLayeredAttrib (GetSafeHwnd (), 0, 
			nTransparency, LWA_ALPHA);
	}
	else
	{
		ModifyStyleEx (WS_EX_LAYERED, 0);
	}

	RedrawWindow ();
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonFloaty message handlers

#endif // BCGP_EXCLUDE_RIBBON

