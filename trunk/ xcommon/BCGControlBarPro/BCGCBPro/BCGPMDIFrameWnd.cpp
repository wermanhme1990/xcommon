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

// BCGPMDIFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPToolbar.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPWindowsManagerDlg.h"
#include "BCGPUserToolsManager.h"
#include "BCGPContextMenuManager.h"

#if _MSC_VER >= 1300
	#include <..\atlmfc\src\mfc\oleimpl2.h>
#else
	#include <..\src\oleimpl2.h>
#endif

#include "BCGPDockingControlBar.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPRibbonBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIFrameWnd

IMPLEMENT_DYNCREATE(CBCGPMDIFrameWnd, CMDIFrameWnd)

BOOL CBCGPMDIFrameWnd::m_bDisableSetRedraw = TRUE;

#pragma warning (disable : 4355)

CBCGPMDIFrameWnd::CBCGPMDIFrameWnd() :
	m_Impl (this),
	m_hmenuWindow (NULL),
	m_bContextHelp (FALSE),
	m_bDoSubclass (TRUE),
	m_uiWindowsDlgMenuId (0),
	m_bShowWindowsDlgAlways (FALSE),
	m_bShowWindowsDlgHelpButton (FALSE),
	m_bWasMaximized (FALSE),
	m_bIsMinimized (FALSE),
	m_bClosing (FALSE),
	m_nFrameID (0),
	m_pPrintPreviewFrame (NULL),
	m_bCanCovertControlBarToMDIChild (FALSE)
{
}

#pragma warning (default : 4355)

CBCGPMDIFrameWnd::~CBCGPMDIFrameWnd()
{
}

//BEGIN_MESSAGE_MAP(CBCGPMDIFrameWnd, CMDIFrameWnd)
BEGIN_MESSAGE_MAP(CBCGPMDIFrameWnd, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CBCGPMDIFrameWnd)
	ON_WM_MENUCHAR()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CONTEXTMENU()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATEAPP()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_REGISTERED_MESSAGE(BCGM_CREATETOOLBAR, OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(BCGM_DELETETOOLBAR, OnToolbarDelete)
	ON_COMMAND( ID_CONTEXT_HELP, OnContextHelp)
	ON_MESSAGE( WM_EXITSIZEMOVE, OnExitSizeMove )
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_REGISTERED_MESSAGE(BCGM_POSTSETPREVIEWFRAME, OnPostPreviewFrame)
	ON_MESSAGE(WM_DWMCOMPOSITIONCHANGED, OnDWMCompositionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIFrameWnd message handlers

BOOL CBCGPMDIFrameWnd::OnSetMenu (HMENU hmenu)
{
	COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
	if (pActiveItem != NULL && 
		pActiveItem->GetInPlaceWindow () != NULL)
	{
		return FALSE;
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_Impl.m_pRibbonBar != NULL &&
		(m_Impl.m_pRibbonBar->GetStyle () & WS_VISIBLE))
	{
		SetMenu (NULL);
		m_Impl.m_pRibbonBar->SetActiveMDIChild (MDIGetActive ());
		return TRUE;
	}
#endif

	if (m_Impl.m_pMenuBar != NULL)
	{
		SetMenu (NULL);
		m_Impl.m_pMenuBar->CreateFromMenu (hmenu == NULL ? m_Impl.m_hDefaultMenu : hmenu);
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!CMDIFrameWnd::OnCreateClient(lpcs, pContext))
	{
		return FALSE;
	}

	if (m_bDoSubclass)
	{
		m_wndClientArea.SubclassWindow (m_hWndMDIClient);
	}

	return TRUE;
}
//*******************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	if (m_Impl.OnMenuChar (nChar))
	{
		return MAKELPARAM (MNC_EXECUTE, -1);
	}
		
	return CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}
//*******************************************************************************************
void CBCGPMDIFrameWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMDIFrameWnd::OnWindowPosChanged(lpwndpos);
	
	if (m_Impl.m_pMenuBar != NULL)
	{
		BOOL bMaximized;
		CMDIChildWnd* pChild = MDIGetActive (&bMaximized);

		if (pChild == NULL || !bMaximized)
		{
			m_Impl.m_pMenuBar->SetMaximizeMode (FALSE);
		}
		else
		{
			m_Impl.m_pMenuBar->SetMaximizeMode (TRUE, pChild);
		}
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_Impl.m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_Impl.m_pRibbonBar);

		BOOL bMaximized;
		CMDIChildWnd* pChild = MDIGetActive (&bMaximized);

		if (pChild == NULL || !bMaximized)
		{
			m_Impl.m_pRibbonBar->SetMaximizeMode (FALSE);
		}
		else
		{
			m_Impl.m_pRibbonBar->SetMaximizeMode (TRUE, pChild);
		}
	}
#endif
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::PreTranslateMessage(MSG* pMsg) 
{
	BOOL bProcessAccel = TRUE;

	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
			::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd) &&
			pMsg->wParam == VK_MENU)
		{
			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
			return TRUE;
		}
		else if (m_Impl.ProcessKeyboard ((int) pMsg->wParam))
		{
			return TRUE;
		}
		break;

	case WM_SYSKEYUP:
		{
			BOOL  isCtrlPressed =  (0x8000 & GetKeyState(VK_CONTROL)) != 0;
			BOOL  isShiftPressed = (0x8000 & GetKeyState(VK_SHIFT)) != 0;

			if (m_Impl.m_pMenuBar != NULL && 
				(pMsg->wParam == VK_MENU || 
					(pMsg->wParam == VK_F10 && !isCtrlPressed && !isShiftPressed)))
			{
				if (m_Impl.m_pMenuBar == GetFocus ())
				{
					SetFocus ();
				}
				else
				{
					if ((pMsg->lParam & (1 << 29)) == 0)
					{
						m_Impl.m_pMenuBar->SetFocus ();
					}
				}
				return TRUE;
			}

			if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
				::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
			{
				return TRUE;	// To prevent system menu opening
			}
		}
		break;

	case WM_KEYDOWN:
		//-----------------------------------------
		// Pass keyboard action to the active menu:
		//-----------------------------------------
		if (!CBCGPFrameImpl::IsHelpKey (pMsg) && 
			m_Impl.ProcessKeyboard ((int) pMsg->wParam))
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			if (IsFullScreen())
			{
				m_Impl.m_FullScreenMgr.RestoreState (this);
			}

			CBCGPSmartDockingManager* pSDManager = NULL;
			if ((pSDManager = m_dockManager.GetSDManagerPermanent()) != NULL &&
				pSDManager->IsStarted())
			{
				pSDManager->CauseCancelMode ();
			}

			CBCGPSlider* pSlider = DYNAMIC_DOWNCAST (CBCGPSlider, GetCapture ());
			if (pSlider != NULL)
			{
				pSlider->SendMessage (WM_CANCELMODE);
				return TRUE;
			}
		}

		if (!bProcessAccel)
		{

			return FALSE;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);

			if (pWnd != NULL && ::IsWindow (pMsg->hwnd))
			{
				pWnd->ClientToScreen (&pt);
			}

			if (m_Impl.ProcessMouseClick (pMsg->message, pt, pMsg->hwnd))
			{
				return TRUE;
			}

			if (!::IsWindow (pMsg->hwnd))
			{
				return TRUE;
			}
		}
		break;

	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
		if (m_Impl.ProcessMouseClick (pMsg->message,
			CPoint (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam)),
			pMsg->hwnd))
		{
			return TRUE;
		}
		break;

	case WM_MOUSEWHEEL:
		if (m_Impl.ProcessMouseWheel (pMsg->wParam, pMsg->lParam))
		{
			return TRUE;
		}
		break;

	case WM_MOUSEMOVE:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
			if (pWnd != NULL)
			{
				pWnd->ClientToScreen (&pt);
			}

			if (m_Impl.ProcessMouseMove (pt))
			{
				return TRUE;
			}
		}
	}

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD (wParam) == 1)
	{
		UINT uiCmd = LOWORD (wParam);

		CBCGPToolBar::AddCommandUsage (uiCmd);

		//---------------------------
		// Simmulate ESC keystroke...
		//---------------------------
		if (m_Impl.ProcessKeyboard (VK_ESCAPE))
		{
			return TRUE;
		}

		if (g_pUserToolsManager != NULL &&
			g_pUserToolsManager->InvokeTool (uiCmd))
		{
			return TRUE;
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode ())
	{
		return CMDIFrameWnd::OnCommand(wParam, lParam);
	}

	return FALSE;
}
//*******************************************************************************************
HMENU CBCGPMDIFrameWnd::GetWindowMenuPopup (HMENU hMenuBar)
{
	m_hmenuWindow = CMDIFrameWnd::GetWindowMenuPopup (hMenuBar);
	return m_hmenuWindow;
}
//********************************************************************************************
BOOL CBCGPMDIFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	m_Impl.m_nIDDefaultResource = nIDResource;
	m_Impl.LoadLargeIconsState ();
	
	if (!CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	m_Impl.OnLoadFrame ();

	if (GetMenuBar () != NULL)
	{
		m_hMenuDefault = m_Impl.m_hDefaultMenu;
	}

	return TRUE;
}
//*******************************************************************************************
void CBCGPMDIFrameWnd::OnClose() 
{
	if (m_pPrintPreviewFrame != NULL)
	{
		m_pPrintPreviewFrame->SendMessage (WM_CLOSE);
		m_pPrintPreviewFrame = NULL;
		return;
	}

	if (!m_Impl.IsPrintPreview ())
	{
		m_bClosing = TRUE;

		// Deactivate OLE container first:
		COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
		if (pActiveItem != NULL)
		{
			pActiveItem->Deactivate ();
		}

		m_Impl.OnCloseFrame();
	}

	CMDIFrameWnd::OnClose();
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_Impl.SetDockManager (&m_dockManager);
	m_Impl.RestorePosition(cs);	
	return CMDIFrameWnd::PreCreateWindow(cs);
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::ShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (!m_Impl.OnShowPopupMenu (pMenuPopup, this))
	{
		return FALSE;
	}


	if (!CBCGPToolBar::IsCustomizeMode () && m_hmenuWindow != NULL &&
		pMenuPopup != NULL && pMenuPopup->GetMenu () != NULL)
	{
		//-----------------------------------------------------------
		// Check the popup menu for the "Windows..." menu maching...:
		//-----------------------------------------------------------
		HMENU hMenuPop = pMenuPopup->GetMenu ();
		BOOL bIsWindowMenu = FALSE;

		int iItemMax = ::GetMenuItemCount (hMenuPop);
		for (int iItemPop = 0; !bIsWindowMenu && iItemPop < iItemMax; iItemPop ++)
		{
			UINT nID = ::GetMenuItemID( hMenuPop, iItemPop);
			bIsWindowMenu =  (nID >= AFX_IDM_WINDOW_FIRST && nID <= AFX_IDM_WINDOW_LAST);
		}

		if (bIsWindowMenu)
		{
			CMenu* pMenu = CMenu::FromHandle (m_hmenuWindow);
			if (pMenu != NULL)
			{
				int iCount = (int) pMenu->GetMenuItemCount ();
				BOOL bIsFirstWindowItem = TRUE;
				BOOL bIsStandradWindowsDlg = FALSE;

				for (int i = 0; i < iCount; i ++)
				{
					UINT uiCmd = pMenu->GetMenuItemID (i);
					if (uiCmd < AFX_IDM_FIRST_MDICHILD || uiCmd == (UINT) -1)
					{
						continue;
					}

					if (m_uiWindowsDlgMenuId != 0 &&
						uiCmd == AFX_IDM_FIRST_MDICHILD + 9)
					{
						// Don't add standrd "Windows..." command
						bIsStandradWindowsDlg = TRUE;
						continue;
					}

					if (bIsFirstWindowItem)
					{
						pMenuPopup->InsertSeparator ();
						bIsFirstWindowItem = FALSE;

						::SendMessage (m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
     				}

					CString strText;
					pMenu->GetMenuString (i, strText, MF_BYPOSITION);

					CBCGPToolbarMenuButton button (uiCmd, NULL, 
													-1, strText);

					UINT uiState = pMenu->GetMenuState (i, MF_BYPOSITION);
					if (uiState & MF_CHECKED)
					{
						button.m_nStyle |= TBBS_CHECKED;
					}

					pMenuPopup->InsertItem (button);
				}

				if (m_uiWindowsDlgMenuId != 0 &&
					(bIsStandradWindowsDlg || m_bShowWindowsDlgAlways))
				{
					if (!CBCGPToolBar::GetBasicCommands ().IsEmpty ())
					{
						CBCGPToolBar::AddBasicCommand (m_uiWindowsDlgMenuId);
					}

					//-----------------------------
					// Add our "Windows..." dialog:
					//-----------------------------
					pMenuPopup->InsertItem (
						CBCGPToolbarMenuButton (m_uiWindowsDlgMenuId,
								NULL, -1, m_strWindowsDlgMenuText));
				}
			}
		}
	}

	if (pMenuPopup != NULL && pMenuPopup->m_bShown)
	{
		return TRUE;
	}

	BOOL bResult = OnShowPopupMenu (pMenuPopup);
	
	if (globalData.IsAccessibilitySupport () && pMenuPopup != NULL)
	{
		pMenuPopup->DestroyRecentMenu ();
		pMenuPopup->ExportToMenu ();
	}

	return bResult; 
}
//**********************************************************************************
void CBCGPMDIFrameWnd::OnClosePopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (globalData.IsAccessibilitySupport () && pMenuPopup != NULL)
	{
		CBCGPPopupMenu* pPopupParent = pMenuPopup->GetParentPopupMenu ();
		CBCGPToolbarMenuButton* pParentButton  = pMenuPopup->GetParentButton ();

		if (pMenuPopup->IsEscClose () || pPopupParent != NULL || pParentButton == NULL)
		{
			globalData.NotifyWinEvent (EVENT_SYSTEM_MENUPOPUPEND,
				pMenuPopup->GetSafeHwnd (), OBJID_WINDOW, CHILDID_SELF);
		}
		else
		{
			globalData.NotifyWinEvent (EVENT_SYSTEM_MENUEND,
				pMenuPopup->GetSafeHwnd (), OBJID_WINDOW, CHILDID_SELF);
		}
				
		pMenuPopup->DestroyRecentMenu ();

		CBCGPMenuBar* pMenuBar = (CBCGPMenuBar*)GetMenuBar ();
		
		if (pMenuBar->GetSafeHwnd () != NULL && pMenuBar == GetFocus ())
		{
			pMenuBar->AccNotifyObjectFocusEvent ();
		}
	}

	if (CBCGPPopupMenu::m_pActivePopupMenu == pMenuPopup)
	{
		CBCGPPopupMenu::m_pActivePopupMenu = NULL;
	}

	m_dockManager.OnClosePopupMenu ();
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::OnDrawMenuImage (CDC* pDC, 
									const CBCGPToolbarMenuButton* pMenuButton, 
									const CRect& rectImage)
{
	ASSERT_VALID (this);

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_Impl.m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_Impl.m_pRibbonBar);
		return m_Impl.m_pRibbonBar->DrawMenuImage (pDC, pMenuButton, rectImage);
	}
#endif

	return FALSE;
}
//*******************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnToolbarCreateNew(WPARAM,LPARAM lp)
{
	ASSERT (lp != NULL);
	return (LRESULT) m_Impl.CreateNewToolBar ((LPCTSTR) lp);
}
//***************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnToolbarDelete(WPARAM,LPARAM lp)
{
	CBCGPToolBar* pToolbar = (CBCGPToolBar*) lp;
	ASSERT_VALID (pToolbar);

	return (LRESULT) m_Impl.DeleteToolBar (pToolbar);
}
//***************************************************************************************
#if _MSC_VER >= 1300
void CBCGPMDIFrameWnd::HtmlHelp(DWORD_PTR dwData, UINT nCmd)
{
	if (dwData > 0 || !m_bContextHelp)
	{
		CFrameWnd::HtmlHelp(dwData, nCmd);
	}
	else
	{
		OnContextHelp ();
	}
}
#endif
//***************************************************************************************
void CBCGPMDIFrameWnd::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (dwData > 0 || !m_bContextHelp)
	{
		CMDIFrameWnd::WinHelp(dwData, nCmd);
	}
	else
	{
		OnContextHelp ();
	}
}
//***************************************************************************************
void CBCGPMDIFrameWnd::OnContextHelp ()
{
	m_bContextHelp = TRUE;

	if (!m_bHelpMode && CanEnterHelpMode())
	{
		CBCGPToolBar::SetHelpMode ();
	}

	CMDIFrameWnd::OnContextHelp ();

	if (!m_bHelpMode)
	{
		CBCGPToolBar::SetHelpMode (FALSE);
	}

	m_bContextHelp = FALSE;
}
//***************************************************************************************
BOOL CBCGPMDIFrameWnd::DockControlBarLeftOf (CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf)
{
	return m_dockManager.DockControlBarLeftOf (pBar, pLeftOf);
}
//***************************************************************************************
void CBCGPMDIFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	switch (nState)
	{
	case WA_CLICKACTIVE:
		UpdateWindow ();
		break;

	case WA_INACTIVE:
		if (!CBCGPToolBar::IsCustomizeMode ())
		{
			m_Impl.DeactivateMenu ();
		}
		break;
	}
}
//***************************************************************************************
#if _MSC_VER >= 1300
void CBCGPMDIFrameWnd::OnActivateApp(BOOL bActive, DWORD /*dwThreadID*/)
#else
void CBCGPMDIFrameWnd::OnActivateApp(BOOL bActive, HTASK /*hTask*/) 
#endif
{
	m_dockManager.OnActivateFrame (bActive);
	m_Impl.OnActivateApp (bActive);
}
//***************************************************************************************
void CBCGPMDIFrameWnd::EnableWindowsDialog (UINT uiMenuId, 
										   LPCTSTR lpszMenuText,
										   BOOL bShowAllways,
										   BOOL bShowHelpButton)
{
	ASSERT (lpszMenuText != NULL);
	ASSERT (uiMenuId != 0);

	m_uiWindowsDlgMenuId = uiMenuId;
	m_strWindowsDlgMenuText = lpszMenuText;
	m_bShowWindowsDlgAlways = bShowAllways;
	m_bShowWindowsDlgHelpButton = bShowHelpButton;
}
//****************************************************************************
void CBCGPMDIFrameWnd::EnableWindowsDialog (UINT uiMenuId, 
										   UINT uiMenuTextResId,
										   BOOL bShowAllways,
										   BOOL bShowHelpButton)
{
	CString strMenuText;
	VERIFY (strMenuText.LoadString (uiMenuTextResId));

	EnableWindowsDialog (uiMenuId, strMenuText, bShowAllways, bShowHelpButton);
}
//****************************************************************************
void CBCGPMDIFrameWnd::ShowWindowsDialog ()
{
	CBCGPLocalResource locaRes;

	CBCGPWindowsManagerDlg dlg (this, m_bShowWindowsDlgHelpButton);
	dlg.DoModal ();
}
//****************************************************************************
COleClientItem*	CBCGPMDIFrameWnd::GetInPlaceActiveItem ()
{
	CFrameWnd* pActiveFrame = GetActiveFrame ();
	if (pActiveFrame == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pActiveFrame);

	CView* pView = pActiveFrame->GetActiveView ();
	if (pView == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pView);

	COleDocument* pDoc = DYNAMIC_DOWNCAST (COleDocument, pView->GetDocument ());
	if (pDoc == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pDoc);
	return pDoc->GetInPlaceActiveItem (pView);
}
//****************************************************************************
void CBCGPMDIFrameWnd::OnUpdateFrameMenu (HMENU hMenuAlt)
{
	CMDIFrameWnd::OnUpdateFrameMenu (hMenuAlt);

	BOOL bIsMenuBar = m_Impl.m_pMenuBar != NULL &&
		(m_Impl.m_pMenuBar->GetStyle () & WS_VISIBLE);

	BOOL bIsRibbon = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_Impl.m_pRibbonBar != NULL &&
		(m_Impl.m_pRibbonBar->GetStyle () & WS_VISIBLE))
	{
		bIsRibbon = TRUE;
	}
#endif

	if (bIsMenuBar || bIsRibbon)
	{
		COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
		if (pActiveItem == NULL ||
			pActiveItem->GetInPlaceWindow () == NULL)
		{
			SetMenu (NULL);
		}
		else
		{
			SetMenu (CMenu::FromHandle (hMenuAlt));
		}
	}
}
//****************************************************************************************
void CBCGPMDIFrameWnd::OnDestroy() 
{
    if (m_hAccelTable != NULL)
    {
        ::DestroyAcceleratorTable (m_hAccelTable);
        m_hAccelTable = NULL;
    }

	m_dockManager.m_bEnableAdjustLayout = FALSE;

	CList<HWND, HWND> lstChildren;
	CWnd* pNextWnd = GetTopWindow ();
	while (pNextWnd != NULL)
	{
		if (m_wndClientArea.m_hWnd != pNextWnd->m_hWnd)
		{
			lstChildren.AddTail (pNextWnd->m_hWnd);
		}
		pNextWnd = pNextWnd->GetNextWindow ();
	}

	for (POSITION pos = lstChildren.GetHeadPosition (); pos != NULL;)
	{
		HWND hwndNext = lstChildren.GetNext (pos);
		if (IsWindow (hwndNext) && ::GetParent (hwndNext) == m_hWnd)
		{
			::DestroyWindow (hwndNext);
		}
	}


	CBCGPFrameImpl::RemoveFrame (this);
	CMDIFrameWnd::OnDestroy();
}
//*************************************************************************************
void CBCGPMDIFrameWnd::EnableMDITabbedGroups (BOOL bEnable, const CBCGPMDITabParams& params)
{
	m_wndClientArea.EnableMDITabbedGroups (bEnable, params);
}
//*************************************************************************************
void CBCGPMDIFrameWnd::EnableMDITabs (BOOL bEnable/* = TRUE*/,
				BOOL bIcons/* = TRUE*/,
				CBCGPTabWnd::Location tabLocation /* = CBCGPTabWnd::LOCATION_BOTTOM*/,
				BOOL bTabCloseButton/* = FALSE*/,
				CBCGPTabWnd::Style style/* = CBCGPTabWnd::STYLE_3D_SCROLLED*/,
				BOOL bTabCustomTooltips/* = FALSE*/)
{
	ASSERT (style == CBCGPTabWnd::STYLE_3D_SCROLLED ||
			style == CBCGPTabWnd::STYLE_3D_ONENOTE ||
			style == CBCGPTabWnd::STYLE_3D_VS2005 ||
			style == CBCGPTabWnd::STYLE_3D_ROUNDED ||
			style == CBCGPTabWnd::STYLE_3D_ROUNDED_SCROLL);

	CBCGPMDITabParams params;
	params.m_style = style;
	params.m_tabLocation = tabLocation;
	params.m_bTabIcons = bIcons;
	params.m_bTabCloseButton = bTabCloseButton;
	params.m_bTabCustomTooltips = bTabCustomTooltips;

	m_wndClientArea.EnableMDITabs (bEnable, params);
}
//*************************************************************************************
void CBCGPMDIFrameWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	ASSERT_VALID (this);

	if (m_wndClientArea.DoesMDITabExist ())
	{
		m_wndClientArea.m_bTabIsVisible = !bPreview;
		((CWnd&)m_wndClientArea.GetMDITabs ()).ShowWindow (
			bPreview ? SW_HIDE : SW_SHOWNOACTIVATE);
	}

	m_dockManager.SetPrintPreviewMode (bPreview, pState);

	DWORD dwSavedState = pState->dwStates;
	CMDIFrameWnd::OnSetPreviewMode (bPreview, pState);
	pState->dwStates = dwSavedState;

	AdjustDockingLayout ();
	RecalcLayout ();

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_Impl.m_pRibbonBar != NULL &&
		m_Impl.m_pRibbonBar->IsReplaceFrameCaption ())
	{
		PostMessage (BCGM_POSTSETPREVIEWFRAME, bPreview);
	}
#endif
}
//*************************************************************************************
BOOL CBCGPMDIFrameWnd::OnShowControlBars (BOOL bShow)
{
	ASSERT_VALID (this);
	
	BOOL bResult = m_dockManager.ShowControlBars (bShow);
	AdjustDockingLayout ();
	
	return bResult;
}
//*****************************************************************************
//******************* dockmanager layer ***************************************
//*****************************************************************************
void CBCGPMDIFrameWnd::AddDockBar ()
{
	ASSERT_VALID (this);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::AddControlBar (CBCGPBaseControlBar* pControlBar, BOOL bTail)
{
	ASSERT_VALID (this);

#ifndef BCGP_EXCLUDE_RIBBON
	CBCGPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST (CBCGPRibbonBar, pControlBar);
	if (pRibbonBar != NULL)
	{
		ASSERT_VALID (pRibbonBar);

		if (pRibbonBar->IsMainRibbonBar ())
		{
			m_Impl.m_pRibbonBar = pRibbonBar;
		}
	}
#endif

	return m_dockManager.AddControlBar (pControlBar, bTail);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::InsertControlBar (CBCGPBaseControlBar* pControlBar, 
									  CBCGPBaseControlBar* pTarget, BOOL bAfter)
{
	ASSERT_VALID (this);
	return m_dockManager.InsertControlBar (pControlBar, pTarget, bAfter);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
										 BOOL bAdjustLayout, BOOL bAutoHide, CBCGPBaseControlBar* pBarReplacement)
{
	ASSERT_VALID (this);
	m_dockManager.RemoveControlBarFromDockManager (pControlBar, bDestroy, bAdjustLayout, bAutoHide, pBarReplacement);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID, 
									LPCRECT lpRect)
{
	ASSERT_VALID (this);
	m_dockManager.DockControlBar (pBar, nDockBarID, lpRect);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::GetControlBar (UINT nID)
{
	ASSERT_VALID (this);
	
	CBCGPBaseControlBar* pBar = m_dockManager.FindBarByID (nID, TRUE);

	return pBar;
}
//*****************************************************************************
void CBCGPMDIFrameWnd::ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, 
									   BOOL bDelay, BOOL bActivate)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	pBar->ShowControlBar (bShow, bDelay, bActivate);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::ControlBarFromPoint (CPoint point, 
							int nSensitivity, bool bExactBar, 
							CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, bExactBar, 
												pRTCBarType);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::ControlBarFromPoint (CPoint point, 
								int nSensitivity, DWORD& dwAlignment, 
								CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, dwAlignment, 
												pRTCBarType);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
										   BOOL& bOuterEdge) const
{
	ASSERT_VALID (this);
	return m_dockManager.IsPointNearDockBar (point, dwBarAlignment, bOuterEdge);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::AdjustDockingLayout (HDWP hdwp)
{
	ASSERT_VALID (this);

	if (m_dockManager.IsInAdjustLayout ())
	{
		return;
	}

	m_dockManager.AdjustDockingLayout (hdwp);
	AdjustClientArea ();
	
	if (m_dockManager.IsOLEContainerMode ())
	{
		RecalcLayout ();
	}

}
//*****************************************************************************
void CBCGPMDIFrameWnd::AdjustClientArea ()
{
	CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

	rectClientAreaBounds.left += m_rectBorder.left;
	rectClientAreaBounds.top  += m_rectBorder.top;
	rectClientAreaBounds.right -= m_rectBorder.right;
	rectClientAreaBounds.bottom -= m_rectBorder.bottom;

	if (m_wndClientArea.GetSafeHwnd () != NULL)
	{
		m_wndClientArea.CalcWindowRect (rectClientAreaBounds, 0);
	}
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::OnMoveMiniFrame	(CWnd* pFrame)
{
	ASSERT_VALID (this);
	return m_dockManager.OnMoveMiniFrame (pFrame);
}
//****************************************************************************************
BOOL CBCGPMDIFrameWnd::EnableDocking (DWORD dwDockStyle)
{
	return m_dockManager.EnableDocking (dwDockStyle);
}
//****************************************************************************************
BOOL CBCGPMDIFrameWnd::EnableAutoHideBars (DWORD dwDockStyle)
{
	return m_dockManager.EnableAutoHideBars (dwDockStyle);
}
//****************************************************************************************
void CBCGPMDIFrameWnd::RecalcLayout (BOOL bNotify)
{
	if (m_bInRecalcLayout)
		return;

	m_bInRecalcLayout = TRUE;

	if (!m_bIsMinimized)
	{
		CView* pView = GetActiveView ();

		if (m_dockManager.IsPrintPreviewValid () ||
			m_dockManager.IsOLEContainerMode ())
		{
			if (pView != NULL && pView->IsKindOf (RUNTIME_CLASS (CBCGPPrintPreviewView)))
			{
	
				m_dockManager.RecalcLayout (bNotify);
				CRect rectClient = m_dockManager.GetClientAreaBounds ();
				pView->SetWindowPos (NULL, rectClient.left, rectClient.top, 
										rectClient.Width (), rectClient.Height (),
										SWP_NOZORDER  | SWP_NOACTIVATE);
			}
			else 
			{
				if (bNotify && m_dockManager.IsOLEContainerMode ())
				{
					ActiveItemRecalcLayout ();
				}
				else
				{
					m_bInRecalcLayout = FALSE;
					CMDIFrameWnd::RecalcLayout (bNotify);

					AdjustClientArea ();
				}
			}
		}
		else
		{
			m_dockManager.RecalcLayout (bNotify);
			AdjustClientArea ();
		}
	}

	m_bInRecalcLayout = FALSE;
}
//****************************************************************************************
void CBCGPMDIFrameWnd::ActiveItemRecalcLayout ()
{
	COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
	if (pActiveItem != NULL)
	{
		CView* pActiveView = pActiveItem->GetActiveView ();
		if (pActiveView != NULL)
		{
			CBCGPMDIChildWnd* pFrame = (CBCGPMDIChildWnd*) pActiveView->GetParentFrame ();
			
			if (pFrame != NULL && pFrame->m_bActivating)
			{
				pActiveItem->m_pInPlaceFrame->OnRecalcLayout ();		
			}
		}
	}
	AdjustClientArea ();	
}
//****************************************************************************************
BOOL CBCGPMDIFrameWnd::NegotiateBorderSpace( UINT nBorderCmd, LPRECT lpRectBorder )
{
	CRect border, request;

	switch (nBorderCmd)
	{
	case borderGet:
	{
		CFrameWnd::NegotiateBorderSpace (nBorderCmd, lpRectBorder);
		m_dockManager.AdjustDockingLayout ();
		CRect rectBounds = m_dockManager.GetClientAreaBounds ();
		ASSERT(lpRectBorder != NULL);

		*lpRectBorder = rectBounds;
		break;
	}
	case borderRequest:
		return TRUE;

	case borderSet:
		return CFrameWnd::NegotiateBorderSpace (nBorderCmd, lpRectBorder);

	default:
		ASSERT(FALSE);  // invalid CFrameWnd::BorderCmd
	}

	return TRUE;
}
//****************************************************************************************
int CBCGPMDIFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_dockManager.Create (this);	

	m_Impl.m_bHasBorder = (lpCreateStruct->style & WS_BORDER) != NULL;

	CBCGPFrameImpl::AddFrame (this);
	OnChangeVisualManager (0, 0);

	return 0;
}
//****************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnExitSizeMove (WPARAM, LPARAM)
{
	m_dockManager.FixupVirtualRects ();
	return 0;
}
//*************************************************************************************
void CBCGPMDIFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CBCGPBaseControlBar* pBar = GetControlBar(pCmdUI->m_nID);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck(pBar->IsWindowVisible ());
		return;
	}

	pCmdUI->ContinueRouting();
}
//*************************************************************************************
BOOL CBCGPMDIFrameWnd::OnBarCheck(UINT nID)
{
	ASSERT_VALID (this);

	CBCGPBaseControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, !pBar->IsWindowVisible (), FALSE, FALSE);
		return TRUE;
	}

	return FALSE;
}

//********************************************************************************
LRESULT CBCGPMDIFrameWnd::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	m_dockManager.SendMessageToMiniFrames (WM_IDLEUPDATECMDUI);
	return 0L;
}
//********************************************************************************
void CBCGPMDIFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bClosing)
	{
		CMDIFrameWnd::OnSize(nType, cx, cy);
		return;
	}

	m_bIsMinimized = (nType == SIZE_MINIMIZED);

	m_dockManager.OnActivateFrame (!m_bIsMinimized);

	if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		m_dockManager.m_bSizeFrame = TRUE;
		AdjustDockingLayout ();			
		CMDIFrameWnd::OnSize(nType, cx, cy);
		m_dockManager.m_bSizeFrame = FALSE;
		BOOL bParam = FALSE;
		SystemParametersInfo (SPI_GETDRAGFULLWINDOWS, 0, &bParam, 0);
		if (!bParam)
		{
			RecalcLayout ();
		}

		m_Impl.UpdateCaption ();
		return;
	} 

	CMDIFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized))
	{
		RecalcLayout ();
	}

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
	m_Impl.UpdateCaption ();
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnWindowNew()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (pActiveChild);

	BOOL bIsZoomed = FALSE;

	if (pActiveChild->IsZoomed ())
	{
		pActiveChild->ShowWindow (SW_RESTORE);
		bIsZoomed = TRUE;
	}

	CMDIFrameWnd::OnWindowNew();

	if (bIsZoomed)
	{
		pActiveChild = MDIGetActive();
		if (pActiveChild != NULL)
		{
			pActiveChild->ShowWindow (SW_MAXIMIZE);
		}
	}
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (IsFullScreen ())
	{
		m_Impl.m_FullScreenMgr.OnGetMinMaxInfo (lpMMI);
	}
	else
	{
		m_Impl.OnGetMinMaxInfo (lpMMI);
		CFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
}
//**************************************************************************************
CBCGPMDIChildWnd* CBCGPMDIFrameWnd::CreateDocumentWindow (LPCTSTR /*lpcszDocName*/, CObject* /*pObj*/)
{
	ASSERT (FALSE);
	TRACE0("If you use save/load state for MDI tabs, you must override this method in a derived class!\n");
	return NULL;
}
//**************************************************************************************
CBCGPMDIChildWnd* CBCGPMDIFrameWnd::CreateNewWindow (LPCTSTR /*lpcszDocName*/, CObject* /*pObj*/)
{
	TRACE0("If you use save/load state for MDI tabs, you should override this method in a derived class!\n");
	OnWindowNew ();
	return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, MDIGetActive ());
}
//**************************************************************************************
BOOL CBCGPMDIFrameWnd::LoadMDIState (LPCTSTR lpszProfileName)
{
	return m_wndClientArea.LoadState (lpszProfileName, m_nFrameID);
}	
//**************************************************************************************
BOOL CBCGPMDIFrameWnd::SaveMDIState (LPCTSTR lpszProfileName)
{
	return m_wndClientArea.SaveState (lpszProfileName, m_nFrameID);
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_wndClientArea.GetMDITabs ().GetSafeHwnd () == NULL)
	{
		return;
	}

	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return;
	}

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)	// Left mouse button is pressed
	{
		return;
	}

	if (pWnd->GetSafeHwnd () == m_wndClientArea.GetSafeHwnd ())
	{
		CWnd* pWnd = WindowFromPoint (point);

		if (IsMemberOfMDITabGroup (pWnd))
		{
			CBCGPTabWnd* pWndTab = DYNAMIC_DOWNCAST (CBCGPTabWnd, pWnd);
			if (pWndTab != NULL)
			{
				CPoint ptTab = point;
				pWndTab->ScreenToClient (&ptTab);

				const int nTab = pWndTab->GetTabFromPoint (ptTab);

				if (nTab >= 0)
				{
					pWndTab->SetActiveTab (nTab);
					OnShowMDITabContextMenu (
						point, GetMDITabsContextMenuAllowedItems (), FALSE);
				}
			}
		}
		else if (CBCGPPopupMenu::GetActiveMenu () == NULL)
		{
			if (SendMessage (BCGM_TOOLBARMENU,
						(WPARAM) GetSafeHwnd (),
						MAKELPARAM (point.x, point.y)))
			{
				m_dockManager.OnControlBarContextMenu (point);
			}
		}
	}
	else if (pWnd->GetSafeHwnd () == m_wndClientArea.GetMDITabs ().GetSafeHwnd ())
	{
		CBCGPTabWnd& wndTab = (CBCGPTabWnd&) (*pWnd);
		
		CRect rectTabs;
		wndTab.GetTabsRect (rectTabs);

		CPoint ptTab = point;
		wndTab.ScreenToClient (&ptTab);

		const int nTab = wndTab.GetTabFromPoint (ptTab);

		if (nTab >= 0)
		{
			wndTab.SetActiveTab (nTab);
			OnShowMDITabContextMenu (
				point, GetMDITabsContextMenuAllowedItems (), FALSE);
		}
	}
}
//**************************************************************************************
BOOL CBCGPMDIFrameWnd::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (globalData.IsAccessibilitySupport () && pMenuPopup != NULL)
	{
		globalData.NotifyWinEvent(EVENT_SYSTEM_MENUPOPUPSTART,
			pMenuPopup->GetSafeHwnd (), OBJID_WINDOW , CHILDID_SELF);
	}

	return TRUE;
}
//**************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnToolbarContextMenu(WPARAM,LPARAM)
{
	return 1l;
}
//**************************************************************************************
BOOL CBCGPMDIFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	return m_dockManager.ProcessControlBarContextMenuCommand (nID, nCode, pExtra, pHandlerInfo);
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CMDIFrameWnd::OnWindowPosChanging(lpwndpos);
	m_Impl.OnWindowPosChanging (lpwndpos);
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnNcPaint() 
{
	if (!m_Impl.OnNcPaint ())
	{
		Default ();
	}
}
//*****************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnSetText(WPARAM, LPARAM lParam)
{
	LRESULT	lRes = Default();

	m_Impl.OnSetText ((LPCTSTR)lParam);
	return lRes;
}
//*****************************************************************************************
BOOL CBCGPMDIFrameWnd::OnNcActivate(BOOL bActive) 
{
	if (m_Impl.OnNcActivate (bActive))
	{
		return TRUE;
	}

	return CMDIFrameWnd::OnNcActivate(bActive);
}
//**************************************************************************
void CBCGPMDIFrameWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_Impl.OnNcMouseMove (nHitTest, point);
	CMDIFrameWnd::OnNcMouseMove(nHitTest, point);
}
//**************************************************************************
BCGNcHitTestType CBCGPMDIFrameWnd::OnNcHitTest(CPoint point) 
{
	UINT nHit = m_Impl.OnNcHitTest (point);
	if (nHit != HTNOWHERE)
	{
		return nHit;
	}

	return CMDIFrameWnd::OnNcHitTest(point);
}
//**************************************************************************
LRESULT CBCGPMDIFrameWnd::OnChangeVisualManager (WPARAM, LPARAM)
{
	m_Impl.OnChangeVisualManager ();
	return 0;
}
//***************************************************************************
void CBCGPMDIFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!m_Impl.OnNcCalcSize (bCalcValidRects, lpncsp))
	{
		CMDIFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//***************************************************************************
void CBCGPMDIFrameWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonUp (point);
	CMDIFrameWnd::OnLButtonUp(nFlags, point);
}
//***************************************************************************
void CBCGPMDIFrameWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_Impl.OnMouseMove (point);
	CMDIFrameWnd::OnMouseMove(nFlags, point);
}
//***************************************************************************
void CBCGPMDIFrameWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonDown (point);
	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}
//***************************************************************************
LRESULT CBCGPMDIFrameWnd::OnPostPreviewFrame(WPARAM, LPARAM)
{
	m_Impl.OnChangeVisualManager ();
	return 0;
}
//***************************************************************************
CBCGPMDIChildWnd* CBCGPMDIFrameWnd::ControlBarToTabbedDocument (CBCGPDockingControlBar* pBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	CBCGPMDIChildWnd* pFrame = new CBCGPMDIChildWnd;
	ASSERT_VALID (pFrame);

	CString strName;
	pBar->GetWindowText (strName);

	if (!pFrame->Create (
		AfxRegisterWndClass (
			CS_DBLCLKS, 0, (HBRUSH)(COLOR_BTNFACE + 1), pBar->GetIcon (FALSE)),
		strName, WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		rectDefault, this))
	{
		return NULL;
	}

	pFrame->SetTitle (strName);
	pFrame->SetWindowText (strName);
	pFrame->AddTabbedControlBar (pBar);

	return pFrame;
}
//***************************************************************************
BOOL CBCGPMDIFrameWnd::TabbedDocumentToControlBar (CBCGPMDIChildWnd* pMDIChildWnd)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pMDIChildWnd);

	if (!pMDIChildWnd->IsTabbedControlBar ())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPDockingControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pMDIChildWnd->GetTabbedControlBar ());
	if (pBar != NULL)
	{
		pBar->ShowWindow (SW_HIDE);
		pBar->SetParent (this);
		pBar->SetMDITabbed (FALSE);
		pBar->DockToResentPos ();
	}

	pMDIChildWnd->SendMessage (WM_CLOSE);
	

	return TRUE;
}
//***************************************************************************
void CBCGPMDIFrameWnd::UpdateMDITabbedBarsIcons ()
{
	ASSERT_VALID (this);

	//-----------------------------------
	// Set MDI tabbed control bars icons:
	//-----------------------------------
	HWND hwndMDIChild = ::GetWindow (m_hWndMDIClient, GW_CHILD);

	while (hwndMDIChild != NULL)
	{
		CBCGPMDIChildWnd* pMDIChildFrame = DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, 
			CWnd::FromHandle (hwndMDIChild));

		if (pMDIChildFrame != NULL && pMDIChildFrame->IsTabbedControlBar ())
		{
			CBCGPDockingControlBar* pBar = pMDIChildFrame->GetTabbedControlBar ();
			ASSERT_VALID (pBar);

#pragma warning (disable : 4311)
			SetClassLongPtr (hwndMDIChild, GCLP_HICONSM, (long) pBar->GetIcon (FALSE));
#pragma warning (default : 4311)
		}

		hwndMDIChild = ::GetWindow (hwndMDIChild, GW_HWNDNEXT);
	}
}
//***************************************************************************
BOOL CBCGPMDIFrameWnd::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL /*bTabDrop*/)
{
	if ((dwAllowedItems & BCGP_MDI_CAN_BE_DOCKED) == 0)
	{
		return FALSE;
	}

	if (g_pContextMenuManager == NULL)
	{
		return FALSE;
	}

	const UINT idTabbed	= (UINT) -106;

	CMenu menu;
	menu.CreatePopupMenu ();

	{
		CBCGPLocalResource locaRes;

		CString strItem;

		strItem.LoadString (IDS_BCGBARRES_TABBED);
		menu.AppendMenu (MF_STRING, idTabbed, strItem);

		menu.CheckMenuItem (idTabbed, MF_CHECKED);
	}

	HWND hwndThis = GetSafeHwnd ();

	int nMenuResult = g_pContextMenuManager->TrackPopupMenu (
			menu, point.x, point.y, this);

	if (::IsWindow (hwndThis))
	{
		switch (nMenuResult)
		{
		case idTabbed:
			{
				CBCGPMDIChildWnd* pMDIChild = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd, MDIGetActive ());
				if (pMDIChild != NULL)
				{
					TabbedDocumentToControlBar (pMDIChild);
				}
			}
		}
	}

	return TRUE;
}
//***************************************************************************
LRESULT CBCGPMDIFrameWnd::OnDWMCompositionChanged(WPARAM,LPARAM)
{
	m_Impl.OnDWMCompositionChanged ();
	return 0;
}
