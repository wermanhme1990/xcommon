//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// BCGFrameImpl.cpp: implementation of the CBCGPFrameImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "multimon.h"

#include "BCGPFrameImpl.h"
#include "BCGPToolBar.h"
#include "BCGPMenuBar.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPWorkspace.h"
#include "RegPath.h"
#include "BCGPRegistry.h"
#include "BCGPTearOffManager.h"
#include "BCGPVisualManager.h"
#include "BCGPDockBar.h"
#include "BCGPKeyboardManager.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPCustomizeMenuButton.h"
#include "CustomizeButton.h"
#include "BCGPToolbarCustomize.h"
#include "BCGPVisualManager.h"
#include "BCGPDropDown.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPRibbonBar.h"
#include "BCGPCaptionButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CObList	gAllToolbars;
extern CBCGPWorkspace* g_pWorkspace;

class CCustomizeButton;

static const CString strTearOffBarsRegEntry = _T("ControlBars-TearOff");

BOOL CBCGPFrameImpl::m_bControlBarExtraPixel = TRUE;
CList<CFrameWnd*, CFrameWnd*> CBCGPFrameImpl::m_lstFrames;

UINT BCGM_POSTSETPREVIEWFRAME = ::RegisterWindowMessage (_T("BCGM_POSTSETPREVIEWFRAME"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4355)

CBCGPFrameImpl::CBCGPFrameImpl(CFrameWnd* pFrame) :
	m_pFrame (pFrame),
	m_pDockManager (NULL),
	m_uiUserToolbarFirst ((UINT)-1),
	m_uiUserToolbarLast ((UINT)-1),
	m_pMenuBar (NULL),
	m_hDefaultMenu (NULL),
	m_nIDDefaultResource (0),
	m_FullScreenMgr(this),
	m_bLoadDockState(TRUE),
	m_uiControlbarsMenuEntryID (0),
	m_bViewMenuShowsToolbarsOnly (FALSE),
	m_pRibbonBar (NULL),
	m_bCaptured (FALSE),
	m_nHotSysButton (HTNOWHERE),
	m_nHitSysButton (HTNOWHERE),
	m_bIsWindowRgn (FALSE),
	m_bHasBorder (FALSE)
{
	ASSERT_VALID (m_pFrame);

	m_pCustomUserToolBarRTC	= RUNTIME_CLASS (CBCGPToolBar);
	m_rectRedraw.SetRectEmpty ();
}

#pragma warning (default : 4355)

//**************************************************************************************
CBCGPFrameImpl::~CBCGPFrameImpl()
{
	//-----------------------------
	// Clear user-defined toolbars:
	//-----------------------------
	while (!m_listUserDefinedToolbars.IsEmpty ())
	{
		delete m_listUserDefinedToolbars.RemoveHead ();
	}

	//-------------------------
	// Clear tear-off toolbars:
	//-------------------------
	while (!m_listTearOffToolbars.IsEmpty ())
	{
		delete m_listTearOffToolbars.RemoveHead ();
	}

	//------------------------------
	// Clear caption system buttons:
	//------------------------------
	while (!m_lstCaptionSysButtons.IsEmpty ())
	{
		delete m_lstCaptionSysButtons.RemoveHead ();
	}
}
//**************************************************************************************
void CBCGPFrameImpl::OnCloseFrame()
{
	ASSERT_VALID (m_pFrame);

	//----------------------------------------------------------------------
	// Automatically load state and frame position if CBCGPWorkspace is used:
	//----------------------------------------------------------------------
	if (g_pWorkspace != NULL)
	{
		
		if (m_FullScreenMgr.IsFullScreen())
		{
			if(::IsWindow (m_pFrame->GetSafeHwnd ()))
			{
				m_FullScreenMgr.RestoreState(m_pFrame);
			}
		}

		g_pWorkspace->OnClosingMainFrame (this);

		//---------------------------
		// Store the Windowplacement:
		//---------------------------
		StoreWindowPlacement ();
	}
}
//**************************************************************************************
void CBCGPFrameImpl::StoreWindowPlacement ()
{
	if (::IsWindow (m_pFrame->GetSafeHwnd ()))
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof (WINDOWPLACEMENT);

		if (m_pFrame->GetWindowPlacement (&wp))
		{
			//---------------------------
			// Make sure we don't pop up 
			// minimized the next time
			//---------------------------
			if (wp.showCmd != SW_SHOWMAXIMIZED)
			{
				wp.showCmd = SW_SHOWNORMAL;
			}

			RECT rectDesktop;
			SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rectDesktop,0);
			OffsetRect(&wp.rcNormalPosition, rectDesktop.left, rectDesktop.top);
  
    		g_pWorkspace->StoreWindowPlacement (
				wp.rcNormalPosition, wp.flags, wp.showCmd);
		}
	}
}
//**************************************************************************************
void CBCGPFrameImpl::RestorePosition(CREATESTRUCT& cs)
{
	if (g_pWorkspace != NULL &&
		cs.hInstance != NULL)	
	{
		CRect rectNormal (CPoint (cs.x, cs.y), CSize (cs.cx, cs.cy));
		int nFlags = 0;
		int nShowCmd = SW_SHOWNORMAL;

		if (!g_pWorkspace->LoadWindowPlacement (rectNormal, nFlags, nShowCmd))
		{
			return;
		}

		if (nShowCmd != SW_MAXIMIZE)
		{
			nShowCmd = SW_SHOWNORMAL;
		}

		switch (AfxGetApp()->m_nCmdShow)
		{
		case SW_MAXIMIZE:
		case SW_MINIMIZE:
		case SW_SHOWMINIMIZED:
		case SW_SHOWMINNOACTIVE:
			break;	// don't change!

		default:
			AfxGetApp()->m_nCmdShow = nShowCmd;
		}

		CRect rectDesktop;
		CRect rectInter;

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);
		if (GetMonitorInfo (MonitorFromPoint (rectNormal.TopLeft (), 
			MONITOR_DEFAULTTONEAREST), &mi))
		{
			rectDesktop = mi.rcWork;
		}
		else
		{
			::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectDesktop, 0);
		}

		if (nShowCmd == SW_MAXIMIZE)
		{
			cs.x = rectDesktop.left;
			cs.y = rectDesktop.top;
			cs.cx = rectDesktop.Width ();
			cs.cy = rectDesktop.Height ();

			return;
		}

		if (rectInter.IntersectRect (&rectDesktop, &rectNormal))
		{
			cs.x = rectInter.left;
			cs.y = rectInter.top;
			cs.cx = rectNormal.Width ();
			cs.cy = rectNormal.Height ();
		}
	}
}
//**************************************************************************************
void CBCGPFrameImpl::OnLoadFrame()
{
	//---------------------------------------------------
	// Automatically load state if CBCGPWorkspace is used:
	//---------------------------------------------------
	if (g_pWorkspace != NULL)
	{
		g_pWorkspace->LoadState (0, this);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::LoadUserToolbars ()
{
	ASSERT_VALID (m_pFrame);
	ASSERT (m_pCustomUserToolBarRTC != NULL);

	if (m_uiUserToolbarFirst == (UINT) -1 ||
		m_uiUserToolbarLast == (UINT) -1)
	{
		return;
	}

	for (UINT uiNewToolbarID = m_uiUserToolbarFirst;
		uiNewToolbarID <= m_uiUserToolbarLast;
		uiNewToolbarID ++)
	{
		CBCGPToolBar* pNewToolbar = (CBCGPToolBar*) m_pCustomUserToolBarRTC->CreateObject();
		if (!pNewToolbar->Create (m_pFrame, 
			dwDefaultToolbarStyle,
			uiNewToolbarID))
		{
			TRACE0 ("Failed to create a new toolbar!\n");
			delete pNewToolbar;
			continue;
		}

		if (!pNewToolbar->LoadState (m_strControlBarRegEntry))
		{
			pNewToolbar->DestroyWindow ();
			delete pNewToolbar;
		}
		else
		{
			pNewToolbar->SetBarStyle (pNewToolbar->GetBarStyle () |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			pNewToolbar->EnableDocking (CBRS_ALIGN_ANY);

			ASSERT_VALID (m_pDockManager);
			m_pDockManager->DockControlBar (pNewToolbar);
			m_listUserDefinedToolbars.AddTail (pNewToolbar);
		}
	}
}
//**********************************************************************************************
void CBCGPFrameImpl::SaveUserToolbars (BOOL bFrameBarsOnly)
{
	for (POSITION pos = m_listUserDefinedToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolBar* pUserToolBar = 
			(CBCGPToolBar*) m_listUserDefinedToolbars.GetNext (pos);
		ASSERT_VALID(pUserToolBar);

		if (!bFrameBarsOnly || pUserToolBar->GetTopLevelFrame () == m_pFrame)
		{
			pUserToolBar->SaveState (m_strControlBarRegEntry);
		}
	}
}
//**********************************************************************************************
CBCGPToolBar* CBCGPFrameImpl::GetUserBarByIndex (int iIndex) const
{
	POSITION pos = m_listUserDefinedToolbars.FindIndex (iIndex);
	if (pos == NULL)
	{
		return NULL;
	}

	CBCGPToolBar* pUserToolBar = 
		(CBCGPToolBar*) m_listUserDefinedToolbars.GetAt (pos);
	ASSERT_VALID (pUserToolBar);

	return pUserToolBar;
}
//**********************************************************************************************
BOOL CBCGPFrameImpl::IsUserDefinedToolbar (const CBCGPToolBar* pToolBar) const
{
	ASSERT_VALID (pToolBar);

	UINT uiCtrlId = pToolBar->GetDlgCtrlID ();
	return	uiCtrlId >= m_uiUserToolbarFirst &&
			uiCtrlId <= m_uiUserToolbarLast;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::IsDockStateValid (const CDockState& /*state*/)
{
	ASSERT_VALID (m_pFrame);
	return TRUE;
}
//**********************************************************************************
void CBCGPFrameImpl::InitUserToolbars (	LPCTSTR lpszRegEntry,
										UINT uiUserToolbarFirst, 
										UINT uiUserToolbarLast)
{
	ASSERT (uiUserToolbarLast >= uiUserToolbarFirst);

	if (uiUserToolbarFirst == (UINT) -1 ||
		uiUserToolbarLast == (UINT) -1)
	{
		ASSERT (FALSE);
		return;
	}

	m_uiUserToolbarFirst = uiUserToolbarFirst;
	m_uiUserToolbarLast = uiUserToolbarLast;

	// ET: get Path automatically from workspace if needed
	m_strControlBarRegEntry = (lpszRegEntry == NULL) ? 
		( g_pWorkspace ? g_pWorkspace->GetRegSectionPath() : _T("") )
		: lpszRegEntry;
}
//**************************************************************************************
UINT CBCGPFrameImpl::GetFreeCtrlBarID (UINT uiFirstID, UINT uiLastID, const CObList& lstCtrlBars)
{
	if (uiFirstID == (UINT)-1 || uiLastID == (UINT)-1)
	{
		return 0;
	}

	int iMaxToolbars = uiLastID - uiFirstID + 1;
	if (lstCtrlBars.GetCount () == iMaxToolbars)
	{
		return 0;
	}

	for (UINT uiNewToolbarID = uiFirstID; uiNewToolbarID <= uiLastID;
		uiNewToolbarID ++)
	{
		BOOL bUsed = FALSE;

		for (POSITION pos = lstCtrlBars.GetHeadPosition (); 
			!bUsed && pos != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) lstCtrlBars.GetNext (pos);
			ASSERT_VALID (pToolBar);

			bUsed = (pToolBar->GetDlgCtrlID () == (int) uiNewToolbarID);
		}

		if (!bUsed)
		{
			return uiNewToolbarID;
		}
	}

	return 0;
}
//**************************************************************************************
void CBCGPFrameImpl::SetNewUserToolBarRTC (CRuntimeClass* pCustomUserToolBarRTC)
{
	ASSERT (pCustomUserToolBarRTC != NULL);
	m_pCustomUserToolBarRTC	= pCustomUserToolBarRTC;
}
//**************************************************************************************
const CBCGPToolBar* CBCGPFrameImpl::CreateNewToolBar (LPCTSTR lpszName)
{
	ASSERT_VALID (m_pFrame);
	ASSERT (lpszName != NULL);

	UINT uiNewToolbarID = 
		GetFreeCtrlBarID (m_uiUserToolbarFirst, m_uiUserToolbarLast, m_listUserDefinedToolbars);

	if (uiNewToolbarID == 0)
	{
		CBCGPLocalResource locaRes;

		CString strError;
		strError.Format (IDS_BCGBARRES_TOO_MANY_TOOLBARS_FMT, 
			m_uiUserToolbarLast - m_uiUserToolbarFirst + 1);

		AfxMessageBox (strError, MB_OK | MB_ICONASTERISK);
		return NULL;
	}

	CBCGPToolBar* pNewToolbar = (CBCGPToolBar*) m_pCustomUserToolBarRTC->CreateObject();
	if (!pNewToolbar->Create (m_pFrame,
		dwDefaultToolbarStyle,
		uiNewToolbarID))
	{
		TRACE0 ("Failed to create a new toolbar!\n");
		delete pNewToolbar;
		return NULL;
	}

	pNewToolbar->SetWindowText (lpszName);

	pNewToolbar->SetBarStyle (pNewToolbar->GetBarStyle () |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	pNewToolbar->EnableDocking (CBRS_ALIGN_ANY);

	CRect rectBar;
	pNewToolbar->GetWindowRect (rectBar);
	int nLeft = ::GetSystemMetrics (SM_CXFULLSCREEN) / 2;
	int nTop  = ::GetSystemMetrics (SM_CYFULLSCREEN) / 2;

	CRect rectFloat (nLeft, nTop, nLeft + rectBar.Width (), nTop + rectBar.Height ());
	pNewToolbar->FloatControlBar (rectFloat, DM_UNKNOWN);
	pNewToolbar->m_nMRUWidth = 32767;
	m_pFrame->RecalcLayout ();

	m_listUserDefinedToolbars.AddTail (pNewToolbar);
	return pNewToolbar;
}
//**************************************************************************************
void CBCGPFrameImpl::AddTearOffToolbar (CBCGPBaseControlBar* pToolBar)
{
	ASSERT_VALID (pToolBar);
	m_listTearOffToolbars.AddTail (pToolBar);
}
//**************************************************************************************
void CBCGPFrameImpl::RemoveTearOffToolbar (CBCGPBaseControlBar* pToolBar)
{
	ASSERT_VALID (pToolBar);

	POSITION pos = m_listTearOffToolbars.Find (pToolBar);
	if (pos != NULL)
	{
		m_listTearOffToolbars.RemoveAt (pos);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::LoadTearOffMenus ()
{
	ASSERT_VALID (m_pFrame);

	//------------------------------
	// Remove current tear-off bars:
	//------------------------------
	for (POSITION pos = m_listTearOffToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) m_listTearOffToolbars.GetNext (pos);
		ASSERT_VALID (pBar);

		if (pBar->IsDocked ())
		{
			pBar->UnDockControlBar (TRUE);
		}
		
		pBar->DestroyWindow ();
		delete pBar;
	}
 
	m_listTearOffToolbars.RemoveAll ();

	CString strProfileName = g_pWorkspace != NULL ?
		g_pWorkspace->GetRegSectionPath() : _T("");

	strProfileName += strTearOffBarsRegEntry;

	for (int iIndex = 0;; iIndex++)
	{
		CString strKey;
		strKey.Format (_T("%s-%d"), strProfileName, iIndex);

		int iId = 0;
		CBCGPToolBar* pToolBar = NULL;
		CString strName;

		CBCGPRegistrySP regSP;
		CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

		if (!reg.Open (strKey) ||
			!reg.Read (_T("ID"), iId) ||
			!reg.Read (_T("Name"), strName) ||
			!reg.Read (_T("State"), (CObject*&) pToolBar))
		{
			break;
		}

		ASSERT_VALID (pToolBar);

		if (!pToolBar->Create (m_pFrame,
			dwDefaultToolbarStyle,
			(UINT) iId))
		{
			TRACE0 ("Failed to create a new toolbar!\n");
			delete pToolBar;
			break;
		}

		pToolBar->SetWindowText (strName);

		pToolBar->SetBarStyle (pToolBar->GetBarStyle () |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		pToolBar->EnableDocking (CBRS_ALIGN_ANY);

		ASSERT_VALID (m_pDockManager);
		m_listTearOffToolbars.AddTail (pToolBar);
		pToolBar->LoadState (strProfileName, iIndex);
		m_pDockManager->DockControlBar (pToolBar);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::SaveTearOffMenus (BOOL bFrameBarsOnly)
{
	CString strProfileName = g_pWorkspace != NULL ?
		g_pWorkspace->GetRegSectionPath() : _T("");
	strProfileName += strTearOffBarsRegEntry;

	int iIndex = 0;

	//------------------------------------------------
	// First, clear old tear-off toolbars in registry:
	//------------------------------------------------
	for (iIndex = 0;; iIndex++)
	{
		CString strKey;
		strKey.Format (_T("%s-%d"), strProfileName, iIndex);

		CBCGPRegistrySP regSP;
		CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

		if (!reg.DeleteKey (strKey))
		{
			break;
		}
	}

	iIndex = 0;

	for (POSITION pos = m_listTearOffToolbars.GetHeadPosition (); pos != NULL; iIndex ++)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) m_listTearOffToolbars.GetNext (pos);
		ASSERT_VALID (pBar);

		if ((!bFrameBarsOnly || pBar->GetTopLevelFrame () == m_pFrame) &&
			pBar->IsBarVisible ())
		{
			CString strName;
			pBar->GetWindowText (strName);

			CString strKey;
			strKey.Format (_T("%s-%d"), strProfileName, iIndex);

			CBCGPRegistrySP regSP;
			CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

			reg.CreateKey (strKey);

			reg.Write (_T("ID"), (int) pBar->GetDlgCtrlID ());
			reg.Write (_T("Name"), strName);
			reg.Write (_T("State"), pBar);
			pBar->SaveState (strProfileName, iIndex);
		}
	}
}
//**************************************************************************************
BOOL CBCGPFrameImpl::DeleteToolBar (CBCGPToolBar* pToolBar)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (pToolBar);

	POSITION pos = m_listUserDefinedToolbars.Find (pToolBar);
	if (pos == NULL)
	{
		return FALSE;
	}

	m_listUserDefinedToolbars.RemoveAt (pos);
	pToolBar->RemoveStateFromRegistry (m_strControlBarRegEntry);

	CBCGPDockBar* pParentDockBar = pToolBar->GetParentDockBar ();
	CBCGPMiniFrameWnd* pParentMiniFrame = pToolBar->GetParentMiniFrame ();
	if (pParentDockBar != NULL)
	{
		ASSERT_VALID (pParentDockBar);
		pParentDockBar->RemoveControlBar (pToolBar, DM_UNKNOWN);
	}
	else if (pParentMiniFrame != NULL)
	{
		ASSERT_VALID (pParentMiniFrame);
		pParentMiniFrame->RemoveControlBar (pToolBar);
	}

	pToolBar->DestroyWindow ();
	delete pToolBar;

	m_pFrame->RecalcLayout ();
	return TRUE;
}
//*******************************************************************************************
void CBCGPFrameImpl::SetMenuBar (CBCGPMenuBar* pMenuBar)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (pMenuBar);
	ASSERT (m_pMenuBar == NULL);	// Method should be called once!

	m_pMenuBar = pMenuBar;

	m_hDefaultMenu=*m_pFrame->GetMenu();

	// ET: Better support for dynamic menu
	m_pMenuBar->OnDefaultMenuLoaded (m_hDefaultMenu);
	m_pMenuBar->CreateFromMenu (m_hDefaultMenu, TRUE /* Default menu */);

	m_pFrame->SetMenu (NULL);
	
	m_pMenuBar->SetDefaultMenuResId (m_nIDDefaultResource);
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessKeyboard (int nKey, BOOL* pbProcessAccel)
{
	ASSERT_VALID (m_pFrame);

	if (pbProcessAccel != NULL)
	{
		*pbProcessAccel = TRUE;
	}

	//--------------------------------------------------------
	// If popup menu is active, pass keyboard control to menu:
	//--------------------------------------------------------
	if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
		::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
	{
		CWnd* pFocus = CWnd::GetFocus();

		if (CBCGPPopupMenu::m_pActivePopupMenu->IsFloaty ())
		{
			BOOL bIsFloatyActive = (pFocus->GetSafeHwnd () != NULL && 
				(CBCGPPopupMenu::m_pActivePopupMenu->IsChild (pFocus) || 
				pFocus->GetSafeHwnd () == CBCGPPopupMenu::m_pActivePopupMenu->GetSafeHwnd ()));

			if (bIsFloatyActive)
			{
				return FALSE;
			}

			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
			return FALSE;
		}

		if (pFocus->GetSafeHwnd () != NULL && 
			CBCGPPopupMenu::m_pActivePopupMenu->IsChild (pFocus))
		{
			return FALSE;
		}

		BOOL bIsDropList = CBCGPPopupMenu::m_pActivePopupMenu->GetMenuBar ()->IsDropDownListMode ();

		CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_KEYDOWN, nKey);
		return !bIsDropList;
	}

	//------------------------------------------
	// If appication is minimized, don't handle
	// any keyboard accelerators:
	//------------------------------------------
	if (m_pFrame->IsIconic ())
	{
		return TRUE;
	}

	//----------------------------------------------------------
	// Don't handle keybaord accererators in customization mode:
	//----------------------------------------------------------
	if (CBCGPToolBar::IsCustomizeMode ())
	{
		return FALSE;
	}

	//-----------------------------------------------------
	// Is any toolbar control (such as combobox) has focus?
	//-----------------------------------------------------
	BOOL bIsToolbarCtrlFocus = FALSE;
	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); 
		!bIsToolbarCtrlFocus && posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);

			for (int i = 0; i < pToolBar->GetCount (); i++)
			{
				CBCGPToolbarButton* pButton = pToolBar->GetButton (i);
				ASSERT_VALID (pButton);

				if (pButton->HasFocus ())
				{
					bIsToolbarCtrlFocus = TRUE;
					break;
				}
			}
		}
	}

	//-------------------------------------
	// Check for the keyboard accelerators:
	//-------------------------------------
	BYTE fVirt = 0;

	if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
	{
		fVirt |= FCONTROL;
	}

	if (::GetAsyncKeyState (VK_MENU) & 0x8000)
	{
		fVirt |= FALT;
	}

	if (::GetAsyncKeyState (VK_SHIFT) & 0x8000)
	{
		fVirt |= FSHIFT;
	}

	if (!bIsToolbarCtrlFocus)
	{
		if (CBCGPKeyboardManager::IsKeyHandled ((WORD) nKey, (BYTE)(fVirt | FVIRTKEY), 
												m_pFrame, TRUE) ||
			CBCGPKeyboardManager::IsKeyHandled ((WORD) nKey, (BYTE)(fVirt | FVIRTKEY), 
												m_pFrame->GetActiveFrame (), FALSE))
		{
			return FALSE;
		}
	}

	if (fVirt == FALT)
	{
		//--------------------------------------------
		// Handle menu accelerators (such as "Alt-F"):
		//--------------------------------------------
		if (OnMenuChar (nKey))
		{
			return TRUE;
		}
	}

	if (bIsToolbarCtrlFocus && pbProcessAccel != NULL)
	{
		//---------------------------------------------
		// Don't process default keyboard accelerators:
		//---------------------------------------------
		*pbProcessAccel = FALSE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessMouseClick (UINT uiMsg, POINT pt, HWND hwnd)
{
	ASSERT_VALID (m_pFrame);

	//------------------------------------------------
	// Maybe user start drag the button with control?
	//------------------------------------------------
	if (uiMsg == WM_LBUTTONDOWN &&
		(CBCGPToolBar::IsCustomizeMode () ||
		(::GetAsyncKeyState (VK_MENU) & 0x8000)))	// ALT is pressed
	{
		for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
			ASSERT (pToolBar != NULL);

			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID(pToolBar);

				CPoint ptToolBar = pt;
				pToolBar->ScreenToClient (&ptToolBar);

				int iHit = pToolBar->HitTest (ptToolBar);
				if (iHit >= 0)
				{
					CBCGPToolbarButton* pButton = pToolBar->GetButton (iHit);
					ASSERT_VALID (pButton);

					if (pButton->GetHwnd () != NULL &&
						pButton->GetHwnd () == hwnd &&
						pButton->Rect ().PtInRect (ptToolBar))
					{
						pToolBar->SendMessage (WM_LBUTTONDOWN, 
							0, MAKELPARAM (ptToolBar.x, ptToolBar.y));
						return TRUE;
					}

					break;
				}
			}
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
		::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
	{
		CBCGPPopupMenu::MENUAREA_TYPE clickArea = CBCGPPopupMenu::m_pActivePopupMenu->CheckArea (pt);

		if (clickArea == CBCGPPopupMenu::OUTSIDE)
		{
			// Click outside of menu

			//--------------------------------------------
			// Maybe secondary click on the parent button?
			//--------------------------------------------
			CRect rectParentBtn;
			CWnd* pWndParent = CBCGPPopupMenu::m_pActivePopupMenu->
				GetParentArea (rectParentBtn);

			if (pWndParent != NULL)
			{
				CBCGPPopupMenuBar* pWndParentPopupMenuBar = 
					DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pWndParent);

				CPoint ptClient = pt;
				pWndParent->ScreenToClient (&ptClient);

				if (rectParentBtn.PtInRect (ptClient))
				{
					//-------------------------------------------------------
					// If user clicks second time on the parent button,
					// we should close an active menu on the toolbar/menubar
					// and leave it on the popup menu:
					//-------------------------------------------------------
					if ((pWndParentPopupMenuBar == NULL ||
						pWndParentPopupMenuBar->IsRibbonPanelInRegularMode ()) &&
						!CBCGPPopupMenu::m_pActivePopupMenu->InCommand ())
					{
						//----------------------------------------
						// Toolbar/menu bar: close an active menu!
						//----------------------------------------
						CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
					}

					return TRUE;
				}

				if (pWndParentPopupMenuBar != NULL && 
					!pWndParentPopupMenuBar->IsRibbonPanelInRegularMode ())
				{
					pWndParentPopupMenuBar->CloseDelayedSubMenu ();
					
					CBCGPPopupMenu* pWndParentPopupMenu = 
						DYNAMIC_DOWNCAST (CBCGPPopupMenu, 
						pWndParentPopupMenuBar->GetParent ());

					if (pWndParentPopupMenu != NULL)
					{
						CBCGPPopupMenu::MENUAREA_TYPE clickAreaParent = 
							pWndParentPopupMenu->CheckArea (pt);

						switch (clickAreaParent)
						{
						case CBCGPPopupMenu::MENU:
						case CBCGPPopupMenu::TEAROFF_CAPTION:
						case CBCGPPopupMenu::LOGO:
							return FALSE;

						case CBCGPPopupMenu::SHADOW_RIGHT:
						case CBCGPPopupMenu::SHADOW_BOTTOM:
							pWndParentPopupMenu->SendMessage (WM_CLOSE);
							m_pFrame->SetFocus ();

							return TRUE;
						}
					}
				}
			}

			if (!CBCGPPopupMenu::m_pActivePopupMenu->InCommand ())
			{
				CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);

				CWnd* pWndFocus = CWnd::GetFocus ();
				if (pWndFocus != NULL && pWndFocus->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
				{
					m_pFrame->SetFocus ();
				}

				if (clickArea != CBCGPPopupMenu::OUTSIDE)	// Click on shadow
				{
					return TRUE;
				}
			}
		}
		else if (clickArea == CBCGPPopupMenu::SHADOW_RIGHT ||
				clickArea == CBCGPPopupMenu::SHADOW_BOTTOM)
		{
			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
			m_pFrame->SetFocus ();

			return TRUE;
		}
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessMouseMove (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL)
	{
		CRect rectMenu;
		CBCGPPopupMenu::m_pActivePopupMenu->GetWindowRect (rectMenu);

		if (rectMenu.PtInRect (pt) ||
			CBCGPPopupMenu::m_pActivePopupMenu->GetMenuBar ()->FindDestBar (pt) != NULL)
		{
			return FALSE;	// Default processing
		}

		return TRUE;		// Active menu "capturing"
	}

	return FALSE;	// Default processing
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessMouseWheel (WPARAM wParam, LPARAM lParam)
{
	if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
		::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
	{
		if (CBCGPPopupMenu::m_pActivePopupMenu->IsScrollable ())
		{
			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_MOUSEWHEEL,
				wParam, lParam);
		}

		return TRUE;
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pRibbonBar != NULL && m_pRibbonBar->IsWindowVisible ())
	{
		return (BOOL) m_pRibbonBar->SendMessage (WM_MOUSEWHEEL,
			wParam, lParam);
	}
#endif
	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup, CFrameWnd* /*pWndFrame*/)
{
	CBCGPSmartDockingManager* pSDMananger = 
		m_pDockManager == NULL ? NULL : m_pDockManager->GetSDManagerPermanent ();
	if (pSDMananger != NULL && pSDMananger->IsStarted ())
	{
		return FALSE;
	}

	if (pMenuPopup != NULL && m_uiControlbarsMenuEntryID != 0)
	{
		CBCGPPopupMenuBar* pPopupMenuBar = pMenuPopup->GetMenuBar ();

		if (m_pDockManager != NULL &&
			pPopupMenuBar->CommandToIndex (m_uiControlbarsMenuEntryID) >= 0)
		{
			if (CBCGPToolBar::IsCustomizeMode ())
			{
				return FALSE;
			}

			pMenuPopup->RemoveAllItems ();

			CMenu menu;
			menu.CreatePopupMenu ();

			m_pDockManager->BuildControlBarsMenu (menu, m_bViewMenuShowsToolbarsOnly);

			pMenuPopup->GetMenuBar ()->ImportFromMenu ((HMENU) menu, TRUE);
			m_pDockManager->m_bControlBarsMenuIsShown = TRUE;
		}
	}

	CBCGPPopupMenu::m_pActivePopupMenu = pMenuPopup;

	if (pMenuPopup != NULL && IsCustomizePane(pMenuPopup))
    {
		ShowQuickCustomizePane (pMenuPopup);
	}

	if (pMenuPopup != NULL && !CBCGPToolBar::IsCustomizeMode ())
	{
		ASSERT_VALID (pMenuPopup);

		CBCGPBaseControlBar* pTopLevelBar = NULL;

		for (CBCGPPopupMenu* pMenu = pMenuPopup; pMenu != NULL;
			pMenu = pMenu->GetParentPopupMenu ())
		{
			CBCGPToolbarMenuButton* pParentButton = pMenu->GetParentButton ();
			if (pParentButton == NULL)
			{
				break;
			}
		
			pTopLevelBar = 
				DYNAMIC_DOWNCAST (CBCGPBaseControlBar, pParentButton->GetParentWnd ());
		}

		if (pTopLevelBar != NULL && 
			!pTopLevelBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
		{
			ASSERT_VALID (pTopLevelBar);

			if (pTopLevelBar->IsDocked () &&
				::GetFocus () != pTopLevelBar->GetSafeHwnd () &&
				CBCGPPopupMenu::GetForceMenuFocus ())
			{
				pTopLevelBar->SetFocus ();
			}
		}
	}

	return TRUE;
}
//****************************************************************************************
void CBCGPFrameImpl::SetupToolbarMenu (CMenu& menu, 
									  const UINT uiViewUserToolbarCmdFirst,
									  const UINT uiViewUserToolbarCmdLast)
{
	//---------------------------------------------------------------
	// Replace toolbar dummy items to the user-defined toolbar names:
	//---------------------------------------------------------------
	for (int i = 0; i < (int) menu.GetMenuItemCount ();)
	{
		UINT uiCmd = menu.GetMenuItemID (i);

		if (uiCmd >= uiViewUserToolbarCmdFirst && 
			uiCmd <= uiViewUserToolbarCmdLast)
		{
			//-------------------------------------------------------------------
			// "User toolbar" item. First check that toolbar number 'x' is exist:
			//-------------------------------------------------------------------
			CBCGPToolBar* pToolBar = GetUserBarByIndex (uiCmd - uiViewUserToolbarCmdFirst);
			if (pToolBar != NULL)
			{
				ASSERT_VALID (pToolBar);

				//-----------------------------------------------------------
				// Modify the current menu item text to the toolbar title and
				// move next:
				//-----------------------------------------------------------
				CString strToolbarName;
				pToolBar->GetWindowText (strToolbarName);

				menu.ModifyMenu (i ++, MF_BYPOSITION | MF_STRING, uiCmd, strToolbarName);
			}
			else
			{
				menu.DeleteMenu (i, MF_BYPOSITION);
			}
		}
		else	// Not "user toolbar" item, move next
		{
			i ++;
		}
	}
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnMenuChar (UINT nChar)
{
	ASSERT_VALID (m_pFrame);

	BOOL bInPrintPreview = 
		m_pDockManager != NULL && m_pDockManager->IsPrintPreviewValid ();

	if (!bInPrintPreview)
	{
		if (m_pMenuBar != NULL &&
			(m_pMenuBar->GetStyle () & WS_VISIBLE) &&
			m_pMenuBar->TranslateChar (nChar))
		{
			return TRUE;
		}
	}

	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (bInPrintPreview && !pToolBar->IsKindOf (RUNTIME_CLASS (CBCGPPrintPreviewToolBar)))
		{
			continue;
		}

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL &&
			pToolBar != m_pMenuBar &&
			(pToolBar->GetStyle () & WS_VISIBLE) &&
			pToolBar->GetTopLevelFrame () == m_pFrame &&
			pToolBar->TranslateChar (nChar))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//************************************************************************************
void CBCGPFrameImpl::SaveDockState (LPCTSTR lpszSectionName)
{
	if (m_pDockManager != NULL)
	{
		m_pDockManager->SaveState (lpszSectionName, m_nIDDefaultResource);
	}
}
//************************************************************************************
void CBCGPFrameImpl::LoadDockState (LPCTSTR lpszSectionName)
{
	if (m_pDockManager != NULL && m_bLoadDockState)
	{
		m_pDockManager->LoadState (lpszSectionName, m_nIDDefaultResource);
	}
}

//************************************************************************************
void CBCGPFrameImpl::SetDockState(const CDockState& /*state*/)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (m_pDockManager);

	if (m_pDockManager != NULL)
	{
		m_pDockManager->SetDockState ();
	}
}
//**************************************************************************************
BOOL CBCGPFrameImpl::IsHelpKey (LPMSG lpMsg)
{
	return lpMsg->message == WM_KEYDOWN &&
		   lpMsg->wParam == VK_F1 &&
		   !(HIWORD(lpMsg->lParam) & KF_REPEAT) &&
		   GetKeyState(VK_SHIFT) >= 0 &&
		   GetKeyState(VK_CONTROL) >= 0 &&
		   GetKeyState(VK_MENU) >= 0;
}
//***************************************************************************************
void CBCGPFrameImpl::DeactivateMenu ()
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL)
	{
		if (m_pMenuBar != NULL)
		{
			m_pMenuBar->Deactivate ();
		}
	}
}
//***************************************************************************************
BOOL CBCGPFrameImpl::LoadLargeIconsState ()
{
	if (g_pWorkspace != NULL)
	{
		return CBCGPToolBar::LoadLargeIconsState (g_pWorkspace->GetRegSectionPath ());
	}
	else
	{
		return FALSE;
	}
}
//*********************************************************************************
void CBCGPFrameImpl::ShowQuickCustomizePane(CBCGPPopupMenu* pMenuPopup)
{
	//---------------------------
	// Get Actual toolbar pointer
	//---------------------------
	CBCGPToolBar* pWndParentToolbar = NULL;

	CBCGPPopupMenu* pPopupLevel2 = pMenuPopup->GetParentPopupMenu();
	if (pPopupLevel2 == NULL)
	{
		return;
	}
	
	CBCGPPopupMenu* pPopupLevel1 = pPopupLevel2->GetParentPopupMenu();
	if (pPopupLevel1 == NULL)
	{
		return;
	}

	CCustomizeButton* pCustom = (CCustomizeButton*)pPopupLevel1->GetParentButton();
	if (pCustom == NULL)
	{
		//May be MiniFrameWnd
		CWnd* pFrame = pPopupLevel1->GetOwner();
		if (pFrame == NULL)
		{
			return;
		}

		if (pFrame->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd)))
		{
			CBCGPMiniFrameWnd* pMinFrm = (CBCGPMiniFrameWnd*)pFrame;

			pWndParentToolbar = (CBCGPToolBar*)pMinFrm->GetControlBar();

		}else
		{
			 return;
		}
	}
	else
	{
		if (!pCustom->IsKindOf(RUNTIME_CLASS(CCustomizeButton)))
		{
			return;
		}

		CBCGPToolBar* pCurrentToolBar = pCustom->GetParentToolbar ();

		CBCGPToolbarMenuButton* btnDummy = pMenuPopup->GetMenuItem (0);
		int nID = _ttoi (btnDummy->m_strText);

		const CObList& gAllToolbars = CBCGPToolBar::GetAllToolbars ();
	
		CBCGPToolBar* pRealToolBar = NULL;
		for (POSITION pos = gAllToolbars.GetHeadPosition (); pos != NULL;)
		{
			pRealToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
			ASSERT (pRealToolBar != NULL);
			if (nID == pRealToolBar->GetDlgCtrlID () &&
				pRealToolBar->IsAddRemoveQuickCustomize ())
			{
				break;
			}
			
			pRealToolBar = NULL;
		}

		if (pRealToolBar == NULL)
		{
			pWndParentToolbar = pCurrentToolBar;
		}
		else
		{
			pWndParentToolbar = pRealToolBar;
		}
	}

	pMenuPopup->RemoveAllItems ();

	CBCGPToolbarCustomize* pStdCust = new CBCGPToolbarCustomize(
											m_pFrame,
											TRUE,
											BCGCUSTOMIZE_MENUAMPERS);

	CBCGPCustomizeMenuButton::SetParentToolbar(pWndParentToolbar);

	//--------------------------
	// Populate pop-up menu
	//-------------------------
	UINT uiRealCount = 0;
	CBCGPCustomizeMenuButton::m_mapPresentIDs.RemoveAll();

	UINT uiCount = pWndParentToolbar->GetCount();
	for (UINT i=0; i< uiCount; i++)
	{
		CBCGPToolbarButton* pBtn = pWndParentToolbar->GetButton(i);

		if (pBtn->IsKindOf(RUNTIME_CLASS(CCustomizeButton)) || (pBtn->m_nStyle & TBBS_SEPARATOR))
		{
			continue;
		}

		CBCGPCustomizeMenuButton::m_mapPresentIDs.SetAt(pBtn->m_nID, 0);

		//---------------------------
		//Find Command Text if empty
		//---------------------------
		CString strText = pBtn->m_strText;
		if (pBtn->m_strText.IsEmpty())
		{
			strText = pStdCust->GetCommandName(pBtn->m_nID);
		}

		UINT uiID = pBtn->m_nID;
		if ((pBtn->m_nID == 0) || (pBtn->m_nID == -1))
		{
			uiID = BCGPCUSTOMIZE_INTERNAL_ID;
		}

		CBCGPCustomizeMenuButton button(uiID, NULL, pBtn->GetImage(), strText, pBtn->m_bUserButton);
		button.SetItemIndex(i);
		pMenuPopup->InsertItem(button);

		uiRealCount++;
	}

	delete pStdCust;

	pMenuPopup->SetQuickCustomizeType(CBCGPPopupMenu::QUICK_CUSTOMIZE_PANE);

	//------------------------------------------
	//Give User ability to customize pane
	//-----------------------------------------
	OnShowCustomizePane(pMenuPopup, pWndParentToolbar->GetResourceID());

	if (uiRealCount > 0)
	{
		pMenuPopup->InsertSeparator();
	}

	//--------------------------
	// Add Reset Toolbar Button
	//--------------------------
	CString strCommand;

	{
		CBCGPLocalResource locaRes;
		strCommand.LoadString (IDS_BCGBARRES_RESET_TOOLBAR);
	}

	CBCGPCustomizeMenuButton btnReset (BCGPCUSTOMIZE_INTERNAL_ID, NULL, -1, strCommand, FALSE);
	btnReset.SetItemIndex(ID_BCGBARRES_TOOLBAR_RESET_PROMT);

	pMenuPopup->InsertItem(btnReset);
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnShowCustomizePane(CBCGPPopupMenu* pMenuPane, UINT uiToolbarID)
{
	BOOL bResult = FALSE;

	CBCGPMDIFrameWnd* pMainFrame =
				DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, m_pFrame);

	if (pMainFrame != NULL)
	{
		bResult = pMainFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, m_pFrame);
		if (pFrame != NULL)
		{
			bResult = pFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);

		}else	// Maybe, OLE frame
		{
			CBCGPOleIPFrameWnd* pOleFrame = 
					DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, m_pFrame);
			if (pOleFrame != NULL)
			{
				bResult = pOleFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
			}
			else
			{
				CBCGPOleDocIPFrameWnd* pOleDocFrame = 
					DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, m_pFrame);
				if (pOleDocFrame != NULL)
				{
					bResult = pOleDocFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
				}
			}
		}
	}

	return bResult;
}
//********************************************************************************
void CBCGPFrameImpl::AddDefaultButtonsToCustomizePane(
						CBCGPPopupMenu* pMenuPane, UINT /*uiToolbarID*/)
{
	CBCGPToolBar* pWndParentToolbar = CBCGPCustomizeMenuButton::GetParentToolbar();
	
	if (pWndParentToolbar == NULL)
	{
		return;
	}
	
	CBCGPToolbarCustomize* pStdCust = new CBCGPToolbarCustomize(m_pFrame, TRUE, 
		BCGCUSTOMIZE_MENUAMPERS);
	
	const CObList& lstOrigButtons = pWndParentToolbar->GetOrigResetButtons(); 

	int i = 0;
	int nTmp = 0;
	for (POSITION posCurr = lstOrigButtons.GetHeadPosition (); posCurr != NULL; i++)
	{
		CBCGPToolbarButton* pButtonCurr = (CBCGPToolbarButton*)lstOrigButtons.GetNext (posCurr);

		UINT uiID = pButtonCurr->m_nID;

		if ((pButtonCurr == NULL) || 
			(pButtonCurr->m_nStyle & TBBS_SEPARATOR) ||
			(pButtonCurr->IsKindOf(RUNTIME_CLASS(CCustomizeButton))) ||
			 CBCGPCustomizeMenuButton::m_mapPresentIDs.Lookup(uiID, nTmp))
		{
				continue;
		}

		if (pButtonCurr->IsKindOf (RUNTIME_CLASS (CBCGPDropDownToolbarButton)))
		{
			CBCGPDropDownToolbarButton* pDropButton = 
				DYNAMIC_DOWNCAST (CBCGPDropDownToolbarButton, pButtonCurr);

			CBCGPToolBar* pDropToolBar = pDropButton->GetDropDownToolBar ();
			if (pDropToolBar != NULL)
			{
				int nIndex = pDropToolBar->CommandToIndex (uiID);
				if (nIndex != -1)
				{
					continue;
				}
			}
		}

		if (pButtonCurr->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton)))
		{
			CBCGPToolbarMenuButton* pMenuButton = 
				DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButtonCurr);

			if (pMenuButton->IsMenuPaletteMode ())
			{
				const CObList& lstMenuItems = pMenuButton->GetCommands ();
				BOOL bIsExist = FALSE;

				for (POSITION posCommand = lstMenuItems.GetHeadPosition (); 
					!bIsExist && posCommand != NULL;)
				{
					CBCGPToolbarMenuButton* pMenuItem = (CBCGPToolbarMenuButton*) lstMenuItems.GetNext (posCommand);
					ASSERT_VALID (pMenuItem);

					bIsExist = (pMenuItem->m_nID == uiID);
				}

				if (bIsExist)
				{
					continue;
				}
			}
		}

		if ((pButtonCurr->m_nID == 0) || (pButtonCurr->m_nID == -1))
		{
			uiID = BCGPCUSTOMIZE_INTERNAL_ID;
		}

		CBCGPCustomizeMenuButton button(uiID, NULL, pButtonCurr->GetImage(), 
			pStdCust->GetCommandName(pButtonCurr->m_nID), pButtonCurr->m_bUserButton); 

		button.SetItemIndex(i, FALSE);

		int nIndex = pMenuPane->InsertItem(button, i);
		if (nIndex == -1)
		{
			pMenuPane->InsertItem(button);
		}
	}

	delete pStdCust;
}
//********************************************************************************
BOOL CBCGPFrameImpl::IsCustomizePane(const CBCGPPopupMenu* pMenuPopup) const
{
	CBCGPPopupMenu* pPopupLevel2 = pMenuPopup->GetParentPopupMenu();

	if (pPopupLevel2 == NULL)
	{
		return FALSE;
	}

	CString strLabel;

	{
		CBCGPLocalResource locaRes;
		strLabel.LoadString (IDS_BCGBARRES_ADD_REMOVE_BTNS);
	}

	CBCGPToolbarMenuButton* pButton = pPopupLevel2->GetParentButton ();
	if (pButton != NULL && pButton->m_strText.Find (strLabel) == -1)
	{
		return FALSE;
	}

		
	CBCGPPopupMenu* pPopupLevel1 = pPopupLevel2->GetParentPopupMenu();

	if (pPopupLevel1 == NULL)
	{
		return FALSE;
	}

	if (pPopupLevel1->GetQuickCustomizeType() == CBCGPPopupMenu::QUICK_CUSTOMIZE_ADDREMOVE)
	{
		return TRUE;
	}

	return FALSE;
}
//********************************************************************************
void CBCGPFrameImpl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	ASSERT_VALID (m_pFrame);

	if ((lpwndpos->flags & SWP_NOSIZE) == 0 && 
		(m_pRibbonBar != NULL || CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ()))
	{
		BOOL oldState = FALSE;

		if (m_pDockManager != NULL)
		{
			oldState = m_pDockManager->m_bDisableRecalcLayout;
			m_pDockManager->m_bDisableRecalcLayout = TRUE;
		}

		m_bIsWindowRgn = CBCGPVisualManager::GetInstance ()->OnSetWindowRegion (
			m_pFrame, CSize (lpwndpos->cx, lpwndpos->cy));

		if (m_pDockManager != NULL)
		{
			m_pDockManager->m_bDisableRecalcLayout = oldState;
		}
	}
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnNcPaint ()
{
	ASSERT_VALID (m_pFrame);

	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return FALSE;
	}

	return CBCGPVisualManager::GetInstance ()->OnNcPaint (m_pFrame,
		m_lstCaptionSysButtons, m_rectRedraw);
}
//********************************************************************************
void CBCGPFrameImpl::OnGetMinMaxInfo (MINMAXINFO FAR* lpMMI)
{
	ASSERT_VALID (m_pFrame);
	ASSERT (lpMMI != NULL);

	if ((m_pFrame->GetStyle () & WS_CAPTION) == 0 ||
		(m_pFrame->GetStyle () & WS_BORDER) == 0)
	{
		CRect rect (0, 0, 0, 0);

		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rect, 0);

		CRect rectWindow;
		m_pFrame->GetWindowRect (&rectWindow);

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);
		if (GetMonitorInfo (MonitorFromPoint (rectWindow.CenterPoint (), MONITOR_DEFAULTTONEAREST),
			&mi))
		{
			CRect rectMI = mi.rcWork;

			rect.right = rect.left + rectMI.Width ();
			rect.bottom = rect.top + rectMI.Height ();
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_BOTTOM)
		{
			rect.bottom -= 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_TOP)
		{
			rect.top += 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_RIGHT)
		{
			rect.right -= 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_LEFT)
		{
			rect.left += 2;
		}

		lpMMI->ptMaxPosition.x = rect.left;
		lpMMI->ptMaxPosition.y = rect.top;
		lpMMI->ptMaxSize.x = rect.Width ();
		lpMMI->ptMaxSize.y = rect.Height ();
	}
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	ASSERT_VALID (m_pFrame);
	ASSERT (lpncsp != NULL);

	BOOL bIsRibbonCaption = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pRibbonBar != NULL &&
		m_pRibbonBar->IsWindowVisible () &&
		m_pRibbonBar->IsReplaceFrameCaption ())
	{
		bIsRibbonCaption = TRUE;

		if (globalData.DwmIsCompositionEnabled ())
		{
			lpncsp->rgrc[0].bottom -= GetSystemMetrics (SM_CYSIZEFRAME);
			lpncsp->rgrc[0].left += GetSystemMetrics (SM_CYSIZEFRAME);
			lpncsp->rgrc[0].right -= GetSystemMetrics (SM_CXSIZEFRAME);

			return TRUE;
		}
	}
#endif

	if (!bIsRibbonCaption && CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		lpncsp->rgrc[0].top += ::GetSystemMetrics (SM_CYCAPTION);
	}
	
	return (m_pFrame->GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE &&
		(bIsRibbonCaption || CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ());
}
//********************************************************************************
void CBCGPFrameImpl::OnActivateApp (BOOL bActive)
{
	ASSERT_VALID (m_pFrame);
	CBCGPVisualManager::GetInstance ()->OnActivateApp (m_pFrame, bActive);
}
//********************************************************************************
void CBCGPFrameImpl::OnSetText (LPCTSTR /*lpszText*/)
{
	ASSERT_VALID (m_pFrame);

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pRibbonBar != NULL &&
		m_pRibbonBar->IsWindowVisible () &&
		m_pRibbonBar->IsReplaceFrameCaption ())
	{
		m_pRibbonBar->RedrawWindow ();
	}
#endif
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnNcActivate (BOOL bActive)
{
	ASSERT_VALID (m_pFrame);

	if (!m_pFrame->IsWindowVisible ())
	{
		return FALSE;
	}

	BOOL bRes = CBCGPVisualManager::GetInstance ()->OnNcActivate (m_pFrame, bActive);
	BOOL bFrameIsRedrawn = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
	if (bRes &&
		m_pRibbonBar != NULL &&
		m_pRibbonBar->IsWindowVisible () &&
		m_pRibbonBar->IsReplaceFrameCaption ())
	{
		m_pRibbonBar->RedrawWindow (NULL, NULL, 
			RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

		m_pFrame->RedrawWindow (NULL, NULL,
			RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);

		bFrameIsRedrawn = TRUE;
	}
#endif

	if (!bFrameIsRedrawn &&
		CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		m_pFrame->RedrawWindow (CRect (0, 0, 0, 0), NULL,
			RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
	}

	return bRes && !globalData.DwmIsCompositionEnabled ();
}
//*************************************************************************
CRect CBCGPFrameImpl::GetCaptionRect ()
{
	ASSERT_VALID (m_pFrame);

	CSize szSystemBorder (::GetSystemMetrics (SM_CXSIZEFRAME), ::GetSystemMetrics (SM_CYSIZEFRAME));

	if (m_pFrame->IsIconic () || 
		(m_pFrame->GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE)
	{
		szSystemBorder = CSize (0, 0);
	}

	CRect rectWnd;
	m_pFrame->GetWindowRect (&rectWnd);

	m_pFrame->ScreenToClient (&rectWnd);
	rectWnd.OffsetRect (szSystemBorder.cx, ::GetSystemMetrics (SM_CYCAPTION) + szSystemBorder.cy);

	CRect rectCaption (	rectWnd.left + szSystemBorder.cx, 
						rectWnd.top + szSystemBorder.cy, 
						rectWnd.right - szSystemBorder.cx, 
						rectWnd.top + szSystemBorder.cy + ::GetSystemMetrics (SM_CYCAPTION));

	if (m_pFrame->IsIconic ())
	{
		rectCaption.top += ::GetSystemMetrics (SM_CYSIZEFRAME);
		rectCaption.right -= ::GetSystemMetrics (SM_CXSIZEFRAME);
	}

	return rectCaption;
}
//*************************************************************************
void CBCGPFrameImpl::UpdateCaption ()
{
	ASSERT_VALID (m_pFrame);

	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return;
	}

	if (m_lstCaptionSysButtons.IsEmpty ())
	{
		//------------------------
		// Create caption buttons:
		//------------------------
		const DWORD dwStyle = m_pFrame->GetStyle ();
		
		HMENU hSysMenu = NULL;
		CMenu* pSysMenu = m_pFrame->GetSystemMenu (FALSE);

		if (pSysMenu != NULL && ::IsMenu (pSysMenu->m_hMenu))
		{
			hSysMenu = pSysMenu->GetSafeHmenu ();
			if (!::IsMenu (hSysMenu) || (m_pFrame->GetStyle () & WS_SYSMENU) == 0)
			{
				hSysMenu = NULL;
			}
		}

		if (hSysMenu != NULL)
		{
			m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTCLOSE_BCG));

			if ((dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX)
			{
				m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTMAXBUTTON_BCG));
			}

			if ((dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX)
			{
				m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTMINBUTTON_BCG));
			}
		}
	}

	CRect rectCaption = GetCaptionRect ();

	CSize sizeButton = CBCGPVisualManager::GetInstance ()->GetNcBtnSize (FALSE);
	sizeButton.cy = min (sizeButton.cy, rectCaption.Height () - 2);

	int x = rectCaption.right - sizeButton.cx;
	int y = rectCaption.top + max (0, (rectCaption.Height () - sizeButton.cy) / 2);

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		pButton->SetRect (CRect (CPoint (x, y), sizeButton));

		x -= sizeButton.cx;
	}

	m_pFrame->RedrawWindow (NULL, NULL,
		RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
}
//*************************************************************************
void CBCGPFrameImpl::AddFrame (CFrameWnd* pFrame)
{
	ASSERT_VALID (pFrame);

	for (POSITION pos = m_lstFrames.GetHeadPosition (); pos != NULL;)
	{
		CFrameWnd* pListFrame = m_lstFrames.GetNext (pos);

		if (pListFrame->GetSafeHwnd () == pFrame->GetSafeHwnd ())
		{
			return;
		}
	}

	m_lstFrames.AddTail (pFrame);
}
//*************************************************************************
void CBCGPFrameImpl::RemoveFrame (CFrameWnd* pFrame)
{
	for (POSITION pos = m_lstFrames.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CFrameWnd* pListFrame = m_lstFrames.GetNext (pos);

		if (pListFrame->GetSafeHwnd () == pFrame->GetSafeHwnd ())
		{
			m_lstFrames.RemoveAt (posSave);
			return;
		}
	}
}
//*************************************************************************
UINT CBCGPFrameImpl::OnNcHitTest (CPoint point)
{
	ASSERT_VALID (m_pFrame);

	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return HTNOWHERE;
	}

	m_pFrame->ScreenToClient (&point);

	const CSize szSystemBorder(::GetSystemMetrics (SM_CXSIZEFRAME), ::GetSystemMetrics (SM_CYSIZEFRAME));
	point.Offset (szSystemBorder.cx, ::GetSystemMetrics (SM_CYCAPTION) + szSystemBorder.cy);

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->GetRect ().PtInRect (point))
		{
			return pButton->m_nHit;
		}
	}

	CRect rectCaption = GetCaptionRect ();
	if (rectCaption.PtInRect (point))
	{
		CRect rectSysMenu = rectCaption;
		rectSysMenu.right = rectSysMenu.left + ::GetSystemMetrics (SM_CYCAPTION) + 2 * szSystemBorder.cx;

		return rectSysMenu.PtInRect (point) ? HTSYSMENU : HTCAPTION;
	}

	return HTNOWHERE;
}
//*************************************************************************
void CBCGPFrameImpl::OnNcMouseMove(UINT /*nHitTest*/, CPoint point)
{
	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return;
	}

	if (!m_bCaptured)
	{
		OnTrackCaptionButtons (point);
	}
}
//*************************************************************************
void CBCGPFrameImpl::OnLButtonDown(CPoint /*point*/)
{
	if (m_nHotSysButton == HTNOWHERE)
	{
		return;
	}

	CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHotSysButton);
	if (pBtn != NULL)
	{
		m_nHitSysButton = m_nHotSysButton;
		pBtn->m_bPushed = TRUE;
		RedrawCaptionButton (pBtn);
	}
}
//*************************************************************************
void CBCGPFrameImpl::OnLButtonUp(CPoint /*point*/)
{
	ASSERT_VALID (m_pFrame);

	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return;
	}

	if (m_bCaptured)
	{
		return;
	}

	switch (m_nHitSysButton)
	{
	case HTCLOSE_BCG:
	case HTMAXBUTTON_BCG:
	case HTMINBUTTON_BCG:
		{
			UINT nHot = m_nHotSysButton;
			UINT nHit = m_nHitSysButton;

			StopCaptionButtonsTracking ();

			if (nHot == nHit)
			{
				UINT nSysCmd = 0;

				switch (nHot)
				{
				case HTCLOSE_BCG:
					nSysCmd = SC_CLOSE;
					break;

				case HTMAXBUTTON_BCG:
					nSysCmd = 
						(m_pFrame->GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE;
					break;

				case HTMINBUTTON_BCG:
					nSysCmd = m_pFrame->IsIconic () ? SC_RESTORE : SC_MINIMIZE;
					break;
				}

				m_pFrame->PostMessage (WM_SYSCOMMAND, nSysCmd);
			}
		}
	}
}
//*************************************************************************
void CBCGPFrameImpl::OnMouseMove(CPoint point)
{
	if (!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		return;
	}

	CPoint ptScreen = point;
	m_pFrame->ClientToScreen (&ptScreen);

	OnTrackCaptionButtons (ptScreen);
}
//*************************************************************************
CBCGPFrameCaptionButton* CBCGPFrameImpl::GetSysButton (UINT nHit)
{
	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->m_nHit == nHit)
		{
			return pButton;
		}
	}

	return NULL;
}
//*************************************************************************
void CBCGPFrameImpl::SetHighlightedSysButton (UINT nHit)
{
	BOOL bRedraw = FALSE;

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->m_nHit == nHit)
		{
			if (pButton->m_bFocused)
			{
				return;
			}

			pButton->m_bFocused = TRUE;
			bRedraw = TRUE;
		}
	}
}
//*************************************************************************************
void CBCGPFrameImpl::OnTrackCaptionButtons (CPoint point)
{
	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return;
	}

	UINT nHot = m_nHotSysButton;
	CBCGPFrameCaptionButton* pBtn = GetSysButton (OnNcHitTest (point));

	if (pBtn != NULL && pBtn->m_bEnabled)
	{
		m_nHotSysButton = pBtn->GetHit ();
		pBtn->m_bFocused = TRUE;
	}
	else
	{
		m_nHotSysButton = HTNOWHERE;
	}

	if (m_nHotSysButton != nHot)
	{
		RedrawCaptionButton (pBtn);

		CBCGPFrameCaptionButton* pBtnOld = GetSysButton (nHot);
		if (pBtnOld != NULL)
		{
			pBtnOld->m_bFocused = FALSE;
			RedrawCaptionButton (pBtnOld);
		}
	}

	if (m_nHitSysButton == HTNOWHERE)
	{
		if (nHot != HTNOWHERE && m_nHotSysButton == HTNOWHERE)
		{
			::ReleaseCapture();
		}
		else if (nHot == HTNOWHERE && m_nHotSysButton != HTNOWHERE)
		{
			m_pFrame->SetCapture ();
		}
	}
}
//************************************************************************************
void CBCGPFrameImpl::StopCaptionButtonsTracking ()
{
	if (m_nHitSysButton != HTNOWHERE)
	{
		CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHitSysButton);
		m_nHitSysButton = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bPushed = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}

	if (m_nHotSysButton != HTNOWHERE)
	{
		CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHotSysButton);
		m_nHotSysButton = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bFocused = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}
}
//************************************************************************************
void CBCGPFrameImpl::RedrawCaptionButton (CBCGPFrameCaptionButton* pBtn)
{
	ASSERT_VALID (m_pFrame);

	if (pBtn ==	NULL)
	{
		return;
	}

	ASSERT_VALID (pBtn);

	m_rectRedraw = pBtn->GetRect ();
	m_pFrame->SendMessage (WM_NCPAINT);
	m_rectRedraw.SetRectEmpty ();

	m_pFrame->UpdateWindow ();
}
//************************************************************************************
void CBCGPFrameImpl::OnChangeVisualManager ()
{
	ASSERT_VALID (m_pFrame);

	BOOL bIsRibbonCaption = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pRibbonBar != NULL &&
		m_pRibbonBar->IsWindowVisible () &&
		m_pRibbonBar->IsReplaceFrameCaption ())
	{
		bIsRibbonCaption = TRUE;
		m_pRibbonBar->RecalcLayout ();

		if (globalData.DwmIsCompositionEnabled ())
		{
			return;
		}
	}
#endif	

	BOOL bChangeBorder = FALSE;

	if (bIsRibbonCaption || CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		if ((m_pFrame->GetStyle () & WS_BORDER) == WS_BORDER && m_bHasBorder)
		{
			bChangeBorder = TRUE;
			m_pFrame->ModifyStyle (WS_BORDER, 0);
		}

		CRect rectWindow;
		m_pFrame->GetWindowRect (rectWindow);

		m_bIsWindowRgn = CBCGPVisualManager::GetInstance ()->OnSetWindowRegion (
			m_pFrame, rectWindow.Size ());
	}
	else
	{
		if ((m_pFrame->GetStyle () & WS_BORDER) == 0 && m_bHasBorder)
		{
			bChangeBorder = TRUE;
			m_pFrame->ModifyStyle (0, WS_BORDER);

			if (m_bIsWindowRgn)
			{
				m_pFrame->SetWindowRgn (NULL, TRUE);
				m_bIsWindowRgn = FALSE;
			}
		}
	}

	if (bChangeBorder && (m_pFrame->GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE &&
		!m_pFrame->IsKindOf (RUNTIME_CLASS (CMDIChildWnd)))
	{
		m_pFrame->ShowWindow (SW_MINIMIZE);
		m_pFrame->ShowWindow (SW_MAXIMIZE);
	}

	UpdateCaption ();
}
//************************************************************************************
BOOL CBCGPFrameImpl::IsPrintPreview ()
{
	return m_pDockManager != NULL && m_pDockManager->IsPrintPreviewValid ();
}
//************************************************************************************
void CBCGPFrameImpl::OnDWMCompositionChanged ()
{
#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pRibbonBar != NULL &&
		m_pRibbonBar->IsWindowVisible () &&
		m_pRibbonBar->IsReplaceFrameCaption ())
	{
		m_pRibbonBar->DWMCompositionChanged ();
	}
#endif
}
