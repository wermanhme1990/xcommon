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

#include "stdafx.h"
#include "BCGGlobals.h"
#include "BCGCBPro.h"
#include "BCGPToolBar.h"
#include "BCGPWorkspace.h"

#include "BCGPFrameImpl.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"

#include "BCGPMouseManager.h"
#include "BCGPContextMenuManager.h"
#include "BCGPKeyboardManager.h"
#include "BCGPUserToolsManager.h"
#include "BCGPTearOffManager.h"
#include "BCGPSkinManager.h"
#include "BCGPShellManager.h"
#include "BCGPTooltipManager.h"

#include "BCGPRegistry.h"
#include "RegPath.h"
#include "BCGPRebarState.h"
#include "BCGCBProVer.h"	// Library version info.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBCGPWorkspace* g_pWorkspace = NULL;
BOOL           g_bWorkspaceAutocreated = FALSE;

CBCGPWorkspace* GetWorkspace()
{ 
	//---------------------------------------------------------------------
	// You must either:
	// ----------------
	// a) construct a CBCGPWorkspace object
	// b) mix a CBCGPWorkspace class somewhere in (e.g. your CWinApp object)
	// c) call CBCGPWorkspace::UseWorkspaceManager() to automatically
	//    initialize an object for you
	//---------------------------------------------------------------------
	ASSERT (g_pWorkspace != NULL);
	return g_pWorkspace; 
}

//-----------------------
// clean up if necessary:
//-----------------------
struct _WORKSPACE_TERM
{
	~_WORKSPACE_TERM()
	{
		if (g_pWorkspace != NULL && g_bWorkspaceAutocreated)
		{
			delete g_pWorkspace;
			g_pWorkspace = NULL;
			g_bWorkspaceAutocreated = FALSE;
		}
	}
};
static const _WORKSPACE_TERM workspaceTerm;

//*************************************************************************************

static const CString strRegEntryNameControlBars		= _T("\\ControlBars");
static const CString strWindowPlacementRegSection	= _T("WindowPlacement");
static const CString strRectMainKey					= _T("MainWindowRect");
static const CString strFlagsKey					= _T("Flags");
static const CString strShowCmdKey					= _T("ShowCmd");
static const CString strRegEntryNameSizingBars		= _T("\\SizingBars");
static const CString strRegEntryVersion				= _T("BCGControlBarVersion");
static const CString strVersionMajorKey				= _T("Major");
static const CString strVersionMinorKey				= _T("Minor");

extern CObList	gAllToolbars;

//*************************************************************************************
BOOL CBCGPWorkspace::UseWorkspaceManager(LPCTSTR lpszSectionName /*=NULL*/, BOOL bResourceSmartUpdate)
{
	if(g_pWorkspace != NULL)
	{
		return FALSE;	// already exists
	}

	g_pWorkspace = new CBCGPWorkspace (bResourceSmartUpdate);
	g_bWorkspaceAutocreated = TRUE;	// Cleanup

	if(lpszSectionName != NULL)
	{
		g_pWorkspace->m_strRegSection = lpszSectionName;
	}
	
	return TRUE;
}
//*************************************************************************************
LPCTSTR CBCGPWorkspace::SetRegistryBase(LPCTSTR lpszSectionName /*= NULL*/)
{
	m_strRegSection = (lpszSectionName != NULL) ? 
			lpszSectionName : 
			_T("");

	return m_strRegSection;
}
//*************************************************************************************
CBCGPWorkspace::CBCGPWorkspace (BOOL bResourceSmartUpdate /*= FALSE*/) :
							m_bResourceSmartUpdate (bResourceSmartUpdate)
{
	// ONLY ONE ALLOWED
	ASSERT(g_pWorkspace == NULL);
	g_pWorkspace = this;

	m_bKeyboardManagerAutocreated = FALSE;
	m_bContextMenuManagerAutocreated = FALSE;
	m_bMouseManagerAutocreated = FALSE;
	m_bUserToolsManagerAutoCreated = FALSE;
	m_bTearOffManagerAutoCreated = FALSE;
	m_bSkinManagerAutocreated = FALSE;
	m_bShellManagerAutocreated = FALSE;
	m_bTooltipManagerAutocreated = FALSE;

	const CString strRegEntryNameWorkspace = _T("BCGWorkspace");
	m_strRegSection = strRegEntryNameWorkspace;

	m_iSavedVersionMajor = -1;
	m_iSavedVersionMinor = -1;

	m_bForceDockStateLoad = FALSE;
	m_bLoadSaveFrameBarsOnly = FALSE;

	m_bSaveState = TRUE;
	m_bForceImageReset = FALSE;

	m_bLoadUserToolbars	= TRUE;

	m_bLoadWindowPlacement = TRUE;
}
//*************************************************************************************
CBCGPWorkspace::~CBCGPWorkspace()
{
	// NO OTHER !!
	ASSERT(g_pWorkspace == this);
	g_pWorkspace = NULL;

	// Delete autocreated managers
	if(m_bKeyboardManagerAutocreated && g_pKeyboardManager != NULL)
	{
		delete g_pKeyboardManager;
		g_pKeyboardManager = NULL;
	}

	if (m_bContextMenuManagerAutocreated && g_pContextMenuManager != NULL)
	{
		delete g_pContextMenuManager;
		g_pContextMenuManager = NULL;
	}

	if (m_bMouseManagerAutocreated && g_pMouseManager != NULL)
	{
		delete g_pMouseManager;
		g_pMouseManager = NULL;
	}

	if (m_bUserToolsManagerAutoCreated && g_pUserToolsManager != NULL)
	{
		delete g_pUserToolsManager;
		g_pUserToolsManager = NULL;
	}

	if (m_bTearOffManagerAutoCreated && g_pTearOffMenuManager != NULL)
	{
		delete g_pTearOffMenuManager;
		g_pTearOffMenuManager = NULL;
	}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version
	if (m_bSkinManagerAutocreated && g_pSkinManager != NULL)
	{
		delete g_pSkinManager;
		g_pSkinManager = NULL;
	}
#endif

#ifndef BCGP_EXCLUDE_SHELL
	if (m_bShellManagerAutocreated && g_pShellManager != NULL)
	{
		delete g_pShellManager;
		g_pShellManager = NULL;
	}
#endif

	if (m_bTooltipManagerAutocreated && g_pTooltipManager != NULL)
	{
		delete g_pTooltipManager;
		g_pTooltipManager = NULL;
	}
}
//*************************************************************************************
BOOL CBCGPWorkspace::InitShellManager()
{
#ifdef BCGP_EXCLUDE_SHELL

	ASSERT (FALSE);
	return FALSE;

#else
	if (g_pShellManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pShellManager = new CBCGPShellManager;
	m_bShellManagerAutocreated = TRUE;
	return TRUE;
#endif
}
//*************************************************************************************
BOOL CBCGPWorkspace::InitTooltipManager()
{
	if (g_pTooltipManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pTooltipManager = new CBCGPTooltipManager;
	m_bTooltipManagerAutocreated = TRUE;
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::InitMouseManager()
{
	if (g_pMouseManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pMouseManager = new CBCGPMouseManager;
	m_bMouseManagerAutocreated = TRUE;
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::InitContextMenuManager()
{
	if (g_pContextMenuManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pContextMenuManager = new CBCGPContextMenuManager;
	m_bContextMenuManagerAutocreated = TRUE;
	
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::InitKeyboardManager()
{
	if (g_pKeyboardManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pKeyboardManager = new CBCGPKeyboardManager;
	m_bKeyboardManagerAutocreated = TRUE;

	return TRUE;
}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version

BOOL CBCGPWorkspace::InitSkinManager(LPCTSTR lpszSkinsDirectory/* = BCG_DEFAULT_SKINS_DIR*/)
{
	if (g_pSkinManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pSkinManager = new CBCGPSkinManager (lpszSkinsDirectory);
	m_bSkinManagerAutocreated = TRUE;

	return TRUE;
}

#endif

BOOL CBCGPWorkspace::EnableUserTools (const UINT uiCmdToolsDummy,
									 const UINT uiCmdFirst, const UINT uiCmdLast,
									CRuntimeClass* pToolRTC,
									UINT uArgMenuID, UINT uInitDirMenuID)

{
	if (g_pUserToolsManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pUserToolsManager = new 
		CBCGPUserToolsManager (	uiCmdToolsDummy, uiCmdFirst, uiCmdLast, pToolRTC,
								uArgMenuID, uInitDirMenuID);
	m_bUserToolsManagerAutoCreated = TRUE;

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::EnableTearOffMenus (LPCTSTR lpszRegEntry,
							const UINT uiCmdFirst, const UINT uiCmdLast)
{
	if (g_pTearOffMenuManager != NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	g_pTearOffMenuManager = new CBCGPTearOffManager;
	m_bTearOffManagerAutoCreated = TRUE;

	return g_pTearOffMenuManager->Initialize (lpszRegEntry, uiCmdFirst, uiCmdLast);
}
//**************************************************************************************
CBCGPMouseManager* CBCGPWorkspace::GetMouseManager()
{
	if (g_pMouseManager == NULL)
	{
		InitMouseManager ();
	}

	ASSERT_VALID (g_pMouseManager);
	return g_pMouseManager;
}
//*************************************************************************************
CBCGPShellManager* CBCGPWorkspace::GetShellManager()
{
#ifdef BCGP_EXCLUDE_SHELL

	ASSERT (FALSE);
	return NULL;

#else
	if (g_pShellManager == NULL)
	{
		InitShellManager ();
	}

	ASSERT_VALID (g_pShellManager);
	return g_pShellManager;
#endif
}
//*************************************************************************************
CBCGPTooltipManager* CBCGPWorkspace::GetTooltipManager()
{
	if (g_pTooltipManager == NULL)
	{
		InitTooltipManager ();
	}

	ASSERT_VALID (g_pTooltipManager);
	return g_pTooltipManager;
}
//**************************************************************************************
CBCGPContextMenuManager* CBCGPWorkspace::GetContextMenuManager()
{
	if (g_pContextMenuManager == NULL)
	{
		InitContextMenuManager();
	}

	ASSERT_VALID (g_pContextMenuManager);
	return g_pContextMenuManager;
}
//*************************************************************************************
CBCGPKeyboardManager* CBCGPWorkspace::GetKeyboardManager()
{
	if (g_pKeyboardManager == NULL)
	{
		InitKeyboardManager ();
	}

	ASSERT_VALID (g_pKeyboardManager);
	return g_pKeyboardManager;
}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version

CBCGPSkinManager* CBCGPWorkspace::GetSkinManager()
{
	if (g_pSkinManager == NULL)
	{
		InitSkinManager ();
	}

	ASSERT_VALID (g_pSkinManager);
	return g_pSkinManager;
}

#endif

CBCGPUserToolsManager* CBCGPWorkspace::GetUserToolsManager()
{
	return g_pUserToolsManager;
}
//*************************************************************************************
CString	CBCGPWorkspace::GetRegSectionPath(LPCTSTR szSectionAdd /*=NULL*/)
{
	CString strSectionPath = ::BCGPGetRegPath (m_strRegSection);
	if (szSectionAdd != NULL && _tcslen (szSectionAdd) != 0)
	{
		strSectionPath += szSectionAdd;
		strSectionPath += _T("\\");
	}

	return strSectionPath;
}
//*************************************************************************************
BOOL CBCGPWorkspace::LoadState (LPCTSTR lpszSectionName /*=NULL*/, CBCGPFrameImpl* pFrameImpl /*= NULL*/)
{
	if (lpszSectionName != NULL)
	{
		m_strRegSection = lpszSectionName;
	}

	CString strSection = GetRegSectionPath ();

	//-----------------------------
	// Other things to do before ?:
	//-----------------------------
	PreLoadState();

	//------------------------
	// Loaded library version:
	//------------------------
	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (reg.Open (GetRegSectionPath (strRegEntryVersion)))
	{
		reg.Read (strVersionMajorKey, m_iSavedVersionMajor);
		reg.Read (strVersionMinorKey, m_iSavedVersionMinor);
	}

	//--------------------------------------
	// Save general toolbar/menu parameters:
	//--------------------------------------
	CBCGPToolBar::LoadParameters (strSection);
	CMD_MGR.LoadState (strSection);

	BOOL bResetImages = FALSE;	// Reset images to default 

	if (m_bResourceSmartUpdate)
	{
		CBCGPToolbarButton::m_bUpdateImages = FALSE;
	}

	if (pFrameImpl != NULL) 
	{
		ASSERT_VALID (pFrameImpl->m_pFrame);

		//-------------------
		// Load rebars state:
		//-------------------
		CBCGPRebarState::LoadState (strSection, pFrameImpl->m_pFrame);

		BOOL	bPrevDisableRecalcLayout	= CBCGPDockManager::m_bDisableRecalcLayout;
		CBCGPDockManager::m_bDisableRecalcLayout = TRUE;

		//-----------------------------------------------------
		// Load all toolbars, menubar and docking control bars:
		//-----------------------------------------------------
		for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
			ASSERT (pToolBar != NULL);

			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID(pToolBar);

				if (!m_bLoadSaveFrameBarsOnly ||
					pToolBar->GetTopLevelFrame () == pFrameImpl->m_pFrame)
				{
					if (!pToolBar->IsFloating ())
					{
						pToolBar->LoadState (strSection);
						if (pToolBar->IsResourceChanged ())
						{
							bResetImages = TRUE;
						}
					}
				}
			}
		}

		//----------------------------
		// Load user defined toolbars:
		//----------------------------
		if (m_bLoadUserToolbars)
		{
			pFrameImpl->LoadUserToolbars ();
		}

		//------------------------
		// Load tear-off toolbars:
		//------------------------
		pFrameImpl->LoadTearOffMenus ();

		CBCGPDockManager::m_bDisableRecalcLayout = bPrevDisableRecalcLayout;

		CDockState dockState;
		dockState.LoadState(m_strRegSection + strRegEntryNameControlBars);

		if (m_bForceDockStateLoad || pFrameImpl->IsDockStateValid (dockState))
		{
			if ((GetDataVersionMajor() != -1) && (GetDataVersionMinor() != -1))
			{
				pFrameImpl->LoadDockState (strSection);
				pFrameImpl->SetDockState (dockState);
			}
		}

		if (m_bLoadWindowPlacement)
		{
			//--------------------------------------------------------
			// Set frame default (restored) size:
			//--------------------------------------------------------
			ReloadWindowPlacement (pFrameImpl->m_pFrame);
		}
	}

	//--------------------------------------
	// Load mouse/keyboard/menu managers:
	//--------------------------------------
	if (g_pMouseManager != NULL)
	{
		g_pMouseManager->LoadState (strSection);
	}

	if (g_pContextMenuManager != NULL)
	{
		g_pContextMenuManager->LoadState(strSection);
	}

	if (g_pKeyboardManager != NULL)
	{
		g_pKeyboardManager->LoadState (strSection,
			pFrameImpl == NULL ? NULL : pFrameImpl->m_pFrame);
	}

	if (g_pUserToolsManager != NULL)
	{
		g_pUserToolsManager->LoadState (strSection);
	}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version
	if (g_pSkinManager != NULL)
	{
		g_pSkinManager->LoadState (strSection);
	}
#endif

	if (m_bResourceSmartUpdate)
	{
		CBCGPToolbarButton::m_bUpdateImages = TRUE;
	}

	if (m_bForceImageReset || (m_bResourceSmartUpdate && bResetImages))
	{
		for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
			ASSERT (pToolBar != NULL);

			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID (pToolBar);

				pToolBar->ResetImages ();
			}
		}

		if (pFrameImpl != NULL)
		{
			ASSERT_VALID (pFrameImpl->m_pFrame);
			pFrameImpl->m_pFrame->RecalcLayout ();
		}
	}

	//----------
	// Call Hook
	//----------
	LoadCustomState();

	//----------------------------------------------------------------------
	// To not confuse internal serialization, set version number to current:
	//----------------------------------------------------------------------
	m_iSavedVersionMajor = _BCGCBPRO_VERSION_MAJOR;
	m_iSavedVersionMinor = _BCGCBPRO_VERSION_MINOR;

	if (pFrameImpl != NULL)
	{
		ASSERT_VALID (pFrameImpl->m_pFrame);

		if (pFrameImpl->m_pFrame->IsZoomed ())
		{
			pFrameImpl->m_pFrame->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
		}
	}

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::LoadState (CBCGPMDIFrameWnd* pFrame, LPCTSTR lpszSectionName /*=NULL*/)
{ 
	ASSERT_VALID (pFrame);
	return LoadState (lpszSectionName, &pFrame->m_Impl); 
}
//*************************************************************************************
BOOL CBCGPWorkspace::LoadState (CBCGPFrameWnd* pFrame, LPCTSTR lpszSectionName /*=NULL*/)
{ 
	ASSERT_VALID (pFrame);
	return LoadState (lpszSectionName, &pFrame->m_Impl);
}
//***********************************************************************************
BOOL CBCGPWorkspace::LoadState (CBCGPOleIPFrameWnd* pFrame, LPCTSTR lpszSectionName /*=NULL*/)
{ 
	ASSERT_VALID (pFrame);
	return LoadState (lpszSectionName, &pFrame->m_Impl);
}
//*************************************************************************************
BOOL CBCGPWorkspace::CleanState (LPCTSTR lpszSectionName /*=NULL*/)
{
	if (lpszSectionName != NULL)
	{
		m_strRegSection = lpszSectionName;
	}

	CString strSection = GetRegSectionPath ();

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	return reg.DeleteKey(strSection);
}
//*************************************************************************************
BOOL CBCGPWorkspace::SaveState (LPCTSTR lpszSectionName  /*=NULL*/, CBCGPFrameImpl* pFrameImpl /*= NULL*/)
{
	if (!m_bSaveState)
	{
		return FALSE;
	}

	if (lpszSectionName != NULL)
	{
		m_strRegSection = lpszSectionName;
	}

	CString strSection = GetRegSectionPath ();

	//-----------------------------
	// Other things to do before ?:
	//-----------------------------
	PreSaveState();

	//----------------------
	// Save library version:
	//----------------------
	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (GetRegSectionPath (strRegEntryVersion)))
	{
		reg.Write (strVersionMajorKey, _BCGCBPRO_VERSION_MAJOR);
		reg.Write (strVersionMinorKey, _BCGCBPRO_VERSION_MINOR);
	}

	//--------------------------------------
	// Save general toolbar/menu parameters:
	//--------------------------------------
	CBCGPToolBar::SaveParameters (strSection);
	CMD_MGR.SaveState (strSection);

	if (pFrameImpl != NULL) 
	{
		CDockState dockState;
		
		pFrameImpl->m_pFrame->GetDockState (dockState);
		dockState.SaveState (m_strRegSection + strRegEntryNameControlBars);

		pFrameImpl->SaveDockState (strSection);

		//-----------------------------------------------------
		// Save all toolbars, menubar and docking control bars:
		//-----------------------------------------------------
		for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
			ASSERT (pToolBar != NULL);

			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID(pToolBar);

				if (!m_bLoadSaveFrameBarsOnly ||
					pToolBar->GetTopLevelFrame () == pFrameImpl->m_pFrame)
				{
					pToolBar->SaveState (strSection);
				}
			}
		}

		//----------------------------
		// Save user defined toolbars:
		//----------------------------
		pFrameImpl->SaveUserToolbars (m_bLoadSaveFrameBarsOnly);

		//------------------------
		// Save tear-off toolbars:
		//------------------------
		pFrameImpl->SaveTearOffMenus (m_bLoadSaveFrameBarsOnly);

		//-------------------
		// Save rebars state:
		//-------------------
		CBCGPRebarState::SaveState (strSection, pFrameImpl->m_pFrame);

		//--------------------------
		// Store window placement
		//--------------------------
		pFrameImpl->StoreWindowPlacement ();
	}

	//------------------
	// Save user images:
	//------------------
	if (CBCGPToolBar::m_pUserImages != NULL)
	{
		ASSERT_VALID (CBCGPToolBar::m_pUserImages);
		CBCGPToolBar::m_pUserImages->Save ();
	}

	//--------------------------------------
	// Save mouse/keyboard/menu managers:
	//--------------------------------------
	if (g_pMouseManager != NULL)
	{
		g_pMouseManager->SaveState (strSection);
	}

	if (g_pContextMenuManager != NULL)
	{
		g_pContextMenuManager->SaveState (strSection);
	}

	if (g_pKeyboardManager != NULL)
	{
		g_pKeyboardManager->SaveState (strSection,
			pFrameImpl == NULL ? NULL : pFrameImpl->m_pFrame);
	}

	if (g_pUserToolsManager != NULL)
	{
		g_pUserToolsManager->SaveState (strSection);
	}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version
	if (g_pSkinManager != NULL)
	{
		g_pSkinManager->SaveState (strSection);
	}
#endif

	SaveCustomState();
	return TRUE;
}

//*************************************************************************************
// Overidables for customization

void CBCGPWorkspace::OnClosingMainFrame (CBCGPFrameImpl* pFrame)
{
	// Defaults to automatically saving state.
	SaveState(0, pFrame);
}

//--------------------------------------------------------
// the next one have to be called explicitly in your code:
//--------------------------------------------------------
BOOL CBCGPWorkspace::OnViewDoubleClick (CWnd* pWnd, int iViewId)
{
	if (g_pMouseManager == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT_VALID (g_pMouseManager);

	UINT uiCmd = g_pMouseManager->GetViewDblClickCommand (iViewId);
	if (uiCmd > 0 && uiCmd != (UINT) -1)
	{
		if (g_pUserToolsManager != NULL &&
			g_pUserToolsManager->InvokeTool (uiCmd))
		{
			return TRUE;
		}

		CWnd* pTargetWnd = (pWnd == NULL) ? 
			AfxGetMainWnd () : 
			BCGCBProGetTopLevelFrame (pWnd);
		ASSERT_VALID (pTargetWnd);

		pTargetWnd->SendMessage (WM_COMMAND, uiCmd);
		return TRUE;
	}

	MessageBeep ((UINT) -1);
	return FALSE;
}
//***********************************************************************************
BOOL CBCGPWorkspace::ShowPopupMenu (UINT uiMenuResId, const CPoint& point, CWnd* pWnd)
{
	if (g_pContextMenuManager == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT_VALID (g_pContextMenuManager);
	return g_pContextMenuManager->ShowPopupMenu (uiMenuResId,
				point.x, point.y, pWnd);
}
//***********************************************************************************
BOOL CBCGPWorkspace::ReloadWindowPlacement (CFrameWnd* pFrameWnd)
{
	ASSERT_VALID (pFrameWnd);

	CCommandLineInfo cmdInfo;
	AfxGetApp ()->ParseCommandLine (cmdInfo);
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		//Don't show the main window if Application 
		//was run with /Embedding or /Automation.  
		return FALSE;
	}

	CRect rectNormal;
	int nFlags = 0;
	int nShowCmd = SW_SHOWNORMAL;

	if (LoadWindowPlacement (rectNormal, nFlags, nShowCmd))
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof (WINDOWPLACEMENT);

		if (pFrameWnd->GetWindowPlacement (&wp))
		{
			wp.rcNormalPosition = rectNormal;
			wp.showCmd = nShowCmd;

			RECT rectDesktop;
			SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rectDesktop,0);
			OffsetRect(&wp.rcNormalPosition, -rectDesktop.left, -rectDesktop.top);

			pFrameWnd->SetWindowPlacement (&wp);

			return TRUE;
		}
	}

	return FALSE;
}
//***********************************************************************************
BOOL CBCGPWorkspace::LoadWindowPlacement (
					CRect& rectNormalPosition, int& nFlags, int& nShowCmd)
{
	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (!reg.Open (GetRegSectionPath (strWindowPlacementRegSection)))
	{
		return FALSE;
	}

	return	reg.Read (strRectMainKey, rectNormalPosition) &&
			reg.Read (strFlagsKey, nFlags) &&
			reg.Read (strShowCmdKey, nShowCmd);
}
//***********************************************************************************
BOOL CBCGPWorkspace::StoreWindowPlacement (
					const CRect& rectNormalPosition, int nFlags, int nShowCmd)
{
	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (!reg.CreateKey (GetRegSectionPath (strWindowPlacementRegSection)))
	{
		return FALSE;
	}

	return	reg.Write (strRectMainKey, rectNormalPosition) &&
			reg.Write (strFlagsKey, nFlags) &&
			reg.Write (strShowCmdKey, nShowCmd);
}
//*************************************************************************************
//*************************************************************************************
// These functions load and store values from the "Custom" subkey
// To use subkeys of the "Custom" subkey use GetSectionInt() etc.
// instead
int CBCGPWorkspace::GetInt(LPCTSTR lpszEntry, int nDefault /*= 0*/)
{
	return GetSectionInt(_T(""), lpszEntry, nDefault);
}
//*************************************************************************************
CString	CBCGPWorkspace::GetString(LPCTSTR lpszEntry, LPCTSTR lpszDefault /*= ""*/)
{
	return GetSectionString(_T(""), lpszEntry, lpszDefault);
}
//*************************************************************************************
BOOL CBCGPWorkspace::GetBinary(LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes)
{
	return GetSectionBinary(_T(""), lpszEntry, ppData, pBytes);
}
//*************************************************************************************
BOOL CBCGPWorkspace::GetObject(LPCTSTR lpszEntry, CObject& obj)
{
	return GetSectionObject(_T(""), lpszEntry, obj);
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteInt(LPCTSTR lpszEntry, int nValue )
{
	return WriteSectionInt(_T(""), lpszEntry, nValue);
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteString(LPCTSTR lpszEntry, LPCTSTR lpszValue )
{
	return WriteSectionString(_T(""), lpszEntry, lpszValue);
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteBinary(LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
	return WriteSectionBinary(_T(""), lpszEntry, pData, nBytes);
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteObject(LPCTSTR lpszEntry, CObject& obj)
{
	return WriteSectionObject(_T(""), lpszEntry, obj);
}
//*************************************************************************************
//*************************************************************************************
// These functions load and store values from a given subkey
// of the "Custom" subkey. For simpler access you may use
// GetInt() etc.
int CBCGPWorkspace::GetSectionInt( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nDefault /*= 0*/)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	
	int nRet = nDefault;

	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (reg.Open (strSection))
	{
		reg.Read (lpszEntry, nRet);
	}
	return nRet;
}
//*************************************************************************************
CString CBCGPWorkspace::GetSectionString( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault /*= ""*/)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT(lpszDefault);
	
	CString strRet = lpszDefault;

	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (reg.Open (strSection))
	{
		reg.Read (lpszEntry, strRet);
	}
	return strRet;
}
//*************************************************************************************
BOOL CBCGPWorkspace::GetSectionBinary(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT(ppData);
	
	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (reg.Open (strSection) 
		&& reg.Read (lpszEntry, ppData, pBytes) ) 
	{
		return TRUE;
	}
	return FALSE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::GetSectionObject(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, CObject& obj)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT_VALID(&obj);
	
	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (reg.Open (strSection) && reg.Read (lpszEntry, obj)) 
	{
		return TRUE;
	}
	return FALSE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteSectionInt( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nValue )
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	
	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (strSection))
	{
		return reg.Write (lpszEntry, nValue);
	}
	return FALSE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteSectionString( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszValue )
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT(lpszValue);

	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (strSection))
	{
		return reg.Write (lpszEntry, lpszValue);
	}
	return FALSE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteSectionBinary(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT(pData);

	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (strSection))
	{
		return reg.Write (lpszEntry, pData, nBytes);
	}
	return FALSE;
}
//*************************************************************************************
BOOL CBCGPWorkspace::WriteSectionObject(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, CObject& obj)
{
	ASSERT(lpszSubSection);
	ASSERT(lpszEntry);
	ASSERT_VALID(&obj);

	CString strSection = GetRegSectionPath(lpszSubSection);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (strSection))
	{
		return reg.Write (lpszEntry, obj);
	}

	return FALSE;
}
//**********************************************************************************
void CBCGPWorkspace::OnAppContextHelp (CWnd* pWndControl, const DWORD dwHelpIDArray [])
{
	::WinHelp (pWndControl->GetSafeHwnd (),
				AfxGetApp()->m_pszHelpFilePath, 
				HELP_CONTEXTMENU, (DWORD_PTR)(LPVOID) dwHelpIDArray);
}
//*************************************************************************************
BOOL CBCGPWorkspace::SaveState (CBCGPMDIFrameWnd* pFrame, LPCTSTR
							   lpszSectionName /*=NULL*/)
{ 
	// By Lex Loep
	ASSERT_VALID (pFrame);
	return SaveState (lpszSectionName, &pFrame->m_Impl); 
}
//*************************************************************************************
BOOL CBCGPWorkspace::SaveState (CBCGPFrameWnd* pFrame, LPCTSTR lpszSectionName
							   /*=NULL*/)
{ 
	// By Lex Loep
	ASSERT_VALID (pFrame);
	return SaveState (lpszSectionName, &pFrame->m_Impl);
}
//***********************************************************************************
BOOL CBCGPWorkspace::SaveState (CBCGPOleIPFrameWnd* pFrame, LPCTSTR
							   lpszSectionName /*=NULL*/)
{ 
	// By Lex Loep
	ASSERT_VALID (pFrame);
	return SaveState (lpszSectionName, &pFrame->m_Impl);
}
//***********************************************************************************
BOOL CBCGPWorkspace::IsStateExists(LPCTSTR lpszSectionName /*=NULL*/)
{
	 if (lpszSectionName != NULL)
	 {
		m_strRegSection = lpszSectionName;
	 }

	 CString strSection = GetRegSectionPath ();

	//------------------------
	// Loaded library version:
	//------------------------
	 CBCGPRegistrySP regSP;
	 CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	 return reg.Open (GetRegSectionPath (strRegEntryVersion));
}
//***********************************************************************************
int CBCGPWorkspace::GetDataVersion () const
{
	if (m_iSavedVersionMajor == -1 || m_iSavedVersionMinor == -1)
	{
		return 0xFFFFFFFF;
	}

	int nVersionMinor = m_iSavedVersionMinor / 10;
	int nVersionDigit = m_iSavedVersionMinor % 10;
	
	nVersionMinor *= 0x100;
	nVersionDigit *= 0x10;

	if (nVersionMinor < 10)
	{
		nVersionDigit *=0x10;
	}

	return m_iSavedVersionMajor * 0x10000 + nVersionMinor + nVersionDigit;
}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_	// Skins manager can not be used in the static version

void CBCGPWorkspace::OnSelectSkin ()
{
	CFrameWnd* pMainWnd = DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ());
	if (pMainWnd != NULL)
	{
		pMainWnd->RecalcLayout ();
	}

	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);
			pToolBar->AdjustLayout ();
		}
	}

	CBCGPVisualManager::GetInstance ()->RedrawAll ();
}

#endif
