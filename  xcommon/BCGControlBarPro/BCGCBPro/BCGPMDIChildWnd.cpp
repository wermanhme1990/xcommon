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

// BCGPMDIChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBPro.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPMainClientAreaWnd.h"
#include "BCGPGlobalUtils.h"
#include "BCGPDockingControlBar.h"

#if _MSC_VER >= 1300
	#include <..\atlmfc\src\mfc\oleimpl2.h>
#else
	#include <..\src\oleimpl2.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CBCGPMDIChildWnd::m_bEnableFloatingBars	= FALSE;
DWORD CBCGPMDIChildWnd::m_dwExcludeStyle		= WS_CAPTION | WS_BORDER | WS_THICKFRAME;


/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIChildWnd

IMPLEMENT_DYNCREATE(CBCGPMDIChildWnd, CMDIChildWnd)

#pragma warning (disable : 4355)

CBCGPMDIChildWnd::CBCGPMDIChildWnd() :
	m_Impl (this)
{
	m_pMDIFrame = NULL;
	m_bToBeDestroyed = FALSE;
	m_bWasMaximized = FALSE;
	m_bIsMinimized = FALSE;
	m_rectOriginal.SetRectEmpty ();
	m_bActivating = FALSE;

	// ---- MDITabGroup+
	m_pRelatedTabGroup = NULL;
	// ---- MDITabGroup-

	m_pTabbedControlBar = NULL;

}

#pragma warning (default : 4355)

CBCGPMDIChildWnd::~CBCGPMDIChildWnd()
{
}


BEGIN_MESSAGE_MAP(CBCGPMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_SETICON,OnSetIcon)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_STYLECHANGED()
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIChildWnd message handlers

BOOL CBCGPMDIChildWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST &&
		m_pMDIFrame != NULL &&
		m_pMDIFrame->GetActivePopup () != NULL)
	{
		// Don't process accelerators if popup window is active
		return FALSE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && 
		m_pRelatedTabGroup != NULL && GetCapture () == m_pRelatedTabGroup)
	{
		m_pRelatedTabGroup->PostMessage (WM_CANCELMODE);
		return CMDIChildWnd::PreTranslateMessage(pMsg);
	}

	return CMDIChildWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************
int CBCGPMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_pMDIFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetMDIFrame ());
	ASSERT_VALID (m_pMDIFrame);

	m_Impl.m_bHasBorder = (lpCreateStruct->style & WS_BORDER) != NULL;

	if ((GetStyle () & WS_SYSMENU) == 0)
	{
		GetParent ()->SetRedraw (FALSE);	
		
		m_rectOriginal = CRect (CPoint (lpCreateStruct->x, lpCreateStruct->y),
			CSize (lpCreateStruct->cx, lpCreateStruct->cy));
		
		if (m_pMDIFrame != NULL && !m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ())
		{
			CRect rect;
			m_pMDIFrame->m_wndClientArea.GetClientRect (rect);

			CRect rectClient;
			GetClientRect (rectClient);
			ClientToScreen (rectClient);

			CRect rectScreen;
			GetWindowRect (rectScreen);

			rect.left -= rectClient.left - rectScreen.left;
			rect.top -= rectClient.top - rectScreen.top;
			rect.right += rectScreen.right - rectClient.right;
			rect.bottom += rectScreen.bottom - rectClient.bottom;

			SetWindowPos (NULL, rect.left, rect.top, rect.Width (), rect.Height (), 
							SWP_NOACTIVATE | SWP_NOZORDER);
		}

		GetParent ()->SetRedraw (TRUE);
	}

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (m_pMDIFrame->IsPrintPreview ())
	{
		m_pMDIFrame->SendMessage (WM_CLOSE);
	}

	CBCGPFrameImpl::AddFrame (this);
	PostMessage (BCGM_CHANGEVISUALMANAGER);

	return 0;
}
//*************************************************************************************
BOOL CBCGPMDIChildWnd::DockControlBarLeftOf(CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf)
{
	m_dockManager.DockControlBarLeftOf (pBar, pLeftOf);
	return TRUE;
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	static BOOL bActivating = FALSE;

	m_dockManager.OnActivateFrame (bActivate);

	m_bActivating = bActivate;

	if (!bActivating)
	{
		bActivating = TRUE;

		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

		if (bActivate && m_pMDIFrame != NULL)
		{
			ASSERT_VALID (m_pMDIFrame);
			m_pMDIFrame->m_wndClientArea.SetActiveTab (pActivateWnd->GetSafeHwnd ());
		}

		bActivating = FALSE;

		if (bActivate && m_pMDIFrame != NULL)
		{
			CBCGPDockManager* parentDockManager = m_pMDIFrame->GetDockManager ();
			ASSERT_VALID (parentDockManager);

			if (parentDockManager != NULL && 
				parentDockManager->IsOLEContainerMode () ||
				m_dockManager.IsOLEContainerMode ())
			{
				//parentDockManager->AdjustDockingLayout ();
				globalUtils.ForceAdjustLayout (parentDockManager, TRUE);
			}
		}
	}
}
//*************************************************************************************
void CBCGPMDIChildWnd::ActivateFrame(int nCmdShow) 
{
	CWnd* pWndParent = GetParent ();
	ASSERT_VALID (pWndParent);

	if (CBCGPMDIFrameWnd::m_bDisableSetRedraw ||
		pWndParent->GetSafeHwnd () == NULL)
	{
		if ((GetStyle () & WS_SYSMENU) == 0)
		{
			nCmdShow = SW_SHOWMAXIMIZED;
		}

		if (m_pMDIFrame != 0 && m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ())
		{
			nCmdShow = SW_SHOWNORMAL;
		}


		CMDIChildWnd::ActivateFrame(nCmdShow);	
		return;
	}
	
	pWndParent->SetRedraw (FALSE);

	CMDIChildWnd::ActivateFrame(nCmdShow);

	pWndParent->SetRedraw (TRUE);
	pWndParent->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	
}
//*************************************************************************************
LRESULT CBCGPMDIChildWnd::OnSetText(WPARAM, LPARAM lParam)
{
	LRESULT lRes = Default();

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs (TRUE);
	}

	m_Impl.OnSetText ((LPCTSTR)lParam);
	return lRes;
}
//*************************************************************************************
LRESULT CBCGPMDIChildWnd::OnSetIcon(WPARAM,LPARAM)
{
	LRESULT lRes = Default();

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs ();
	}

	return lRes;
}
//*************************************************************************************
CString CBCGPMDIChildWnd::GetFrameText () const
{
	ASSERT_VALID (this);

	CString strText;
	GetWindowText (strText);

	return strText;
}
//*************************************************************************************
HICON CBCGPMDIChildWnd::GetFrameIcon () const
{
	ASSERT_VALID (this);

	HICON hIcon = GetIcon (FALSE);
	if (hIcon == NULL)
	{
		hIcon = (HICON)(LONG_PTR) GetClassLongPtr (GetSafeHwnd (), GCLP_HICONSM);
	}

	return hIcon;
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnUpdateFrameTitle (BOOL bAddToTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle (bAddToTitle);

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs ();
	}
}
//*****************************************************************************
void CBCGPMDIChildWnd::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bToBeDestroyed)
	{
		// prevents main menu flickering when the last dockument is being closed
		return;
	}
	
	m_bIsMinimized = (nType == SIZE_MINIMIZED);

	if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		m_dockManager.m_bSizeFrame = TRUE;
		CMDIChildWnd::OnSize(nType, cx, cy);
		AdjustDockingLayout ();			
		m_dockManager.m_bSizeFrame = FALSE;

		m_Impl.UpdateCaption ();
		return;
	}

	CMDIChildWnd::OnSize(nType, cx, cy);

	if ((nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized)))
	{
		RecalcLayout ();

		if (m_pNotifyHook != NULL && nType == SIZE_RESTORED)
		{
			CBCGPMDIFrameWnd* pTopLevelFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
			if (pTopLevelFrame == NULL || !pTopLevelFrame->AreMDITabs ())
			{
				ModifyStyle(0, WS_OVERLAPPEDWINDOW);
			}
		}
	}

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
	m_Impl.UpdateCaption ();
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	m_dockManager.Create (this);
	return CMDIChildWnd::PreCreateWindow(cs);
}

//*****************************************************************************
//******************* dockmanager layer ***************************************
//*****************************************************************************
void CBCGPMDIChildWnd::AddDockBar ()
{
	ASSERT_VALID (this);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::AddControlBar (CBCGPBaseControlBar* pControlBar, BOOL bTail)
{
	ASSERT_VALID (this);
	return m_dockManager.AddControlBar (pControlBar, bTail);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::InsertControlBar (CBCGPBaseControlBar* pControlBar, 
									  CBCGPBaseControlBar* pTarget, BOOL bAfter)
{
	ASSERT_VALID (this);
	return m_dockManager.InsertControlBar (pControlBar, pTarget, bAfter);
}
//*****************************************************************************
void CBCGPMDIChildWnd::RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
										 BOOL bAdjustLayout, BOOL bAutoHide, CBCGPBaseControlBar* pBarReplacement)
{
	ASSERT_VALID (this);
	m_dockManager.RemoveControlBarFromDockManager (pControlBar, bDestroy, bAdjustLayout, bAutoHide, pBarReplacement);
}
//*****************************************************************************
void CBCGPMDIChildWnd::DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID, 
									LPCRECT /*lpRect*/)
{
	ASSERT_VALID (this);

	if (pBar->CanFloat () && !CBCGPMDIChildWnd::m_bEnableFloatingBars)
	{
		// bar can't be floating
		pBar->m_dwBCGStyle &= ~CBRS_BCGP_FLOAT;
	}


	if (pBar->CanBeResized () || pBar->CanFloat ())
	{
		pBar->EnableDocking (CBRS_ALIGN_ANY);
		m_dockManager.DockControlBar (pBar, nDockBarID);
	}
	else
	{
		AddControlBar (pBar, TRUE);
	}
}
//*****************************************************************************
void CBCGPMDIChildWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	ASSERT_VALID (this);

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, AfxGetMainWnd ());
	if (pMainFrame != NULL)
	{
		pMainFrame->SetPrintPreviewFrame (bPreview ? this : NULL);
	}

	m_dockManager.SetPrintPreviewMode (bPreview, pState);
	DWORD dwSavedState = pState->dwStates;
	CMDIChildWnd::OnSetPreviewMode (bPreview, pState);
	pState->dwStates = dwSavedState;

	AdjustDockingLayout ();
	RecalcLayout ();
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::GetControlBar (UINT nID)
{
	ASSERT_VALID (this);
	
	CBCGPBaseControlBar* pBar = m_dockManager.FindBarByID (nID, TRUE);
	return pBar;
}
//*****************************************************************************
void CBCGPMDIChildWnd::ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, 
									   BOOL bDelay, BOOL bActivate)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	pBar->ShowControlBar (bShow, bDelay, bActivate);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::ControlBarFromPoint (CPoint point, 
							int nSensitivity, bool bExactBar, 
							CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, bExactBar, 
												pRTCBarType);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::ControlBarFromPoint (CPoint point, 
								int nSensitivity, DWORD& dwAlignment, 
								CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, dwAlignment, 
												pRTCBarType);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
										   BOOL& bOuterEdge) const
{
	ASSERT_VALID (this);
	return m_dockManager.IsPointNearDockBar (point, dwBarAlignment, bOuterEdge);
}
//*****************************************************************************
void CBCGPMDIChildWnd::AdjustDockingLayout (HDWP hdwp)
{
	ASSERT_VALID (this);
	
	if (m_dockManager.IsInAdjustLayout ())
	{	
		return;
	}

	m_dockManager.AdjustDockingLayout (hdwp);
	AdjustClientArea ();
}
//*****************************************************************************
void CBCGPMDIChildWnd::AdjustClientArea ()
{
	CWnd* pChildWnd = (m_pTabbedControlBar != NULL && m_pTabbedControlBar->IsMDITabbed () && 
						m_pTabbedControlBar->GetParent () == this) ? m_pTabbedControlBar : GetDlgItem (AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		if (!pChildWnd->IsKindOf (RUNTIME_CLASS (CSplitterWnd)))
		{
			pChildWnd->ModifyStyle (0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		}
		else
		{
			pChildWnd->ModifyStyle (0, WS_CLIPSIBLINGS);
		}

		if (!CBCGPDockManager::m_bFullScreenMode)
		{
			CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

			rectClientAreaBounds.left += m_rectBorder.left;
			rectClientAreaBounds.top  += m_rectBorder.top;
			rectClientAreaBounds.right -= m_rectBorder.right;
			rectClientAreaBounds.bottom -= m_rectBorder.bottom;
			
			
			
			pChildWnd->SetWindowPos (&wndBottom, rectClientAreaBounds.left, 
											rectClientAreaBounds.top, 
											rectClientAreaBounds.Width (), 
											rectClientAreaBounds.Height (),
											SWP_NOACTIVATE);
		}
	}
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::OnMoveMiniFrame	(CWnd* pFrame)
{
	ASSERT_VALID (this);
	return m_dockManager.OnMoveMiniFrame (pFrame);
}
//****************************************************************************************
BOOL CBCGPMDIChildWnd::EnableDocking (DWORD dwDockStyle)
{
	return m_dockManager.EnableDocking (dwDockStyle);
}
//****************************************************************************************
BOOL CBCGPMDIChildWnd::EnableAutoHideBars (DWORD dwDockStyle)
{
	return m_dockManager.EnableAutoHideBars (dwDockStyle);
}
//*************************************************************************************
void CBCGPMDIChildWnd::RecalcLayout (BOOL bNotify)
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
				COleClientItem*	pActiveItem = NULL;
				CView* pView = GetActiveView ();
				
				if (pView != NULL && GetParentFrame ()->GetActiveFrame () == this && 
					m_bActivating)
				{
					ASSERT_VALID (pView);

					COleDocument* pDoc = DYNAMIC_DOWNCAST (COleDocument, pView->GetDocument ());
					if (pDoc != NULL)
					{
						ASSERT_VALID (pDoc);
						pActiveItem = pDoc->GetInPlaceActiveItem (pView);

						if (bNotify && pActiveItem != NULL && pActiveItem->m_pInPlaceFrame != NULL)
						{
							pActiveItem->m_pInPlaceFrame->OnRecalcLayout ();
						}
					}
					CRect rectClient;
				
					CFrameWnd* pFrame = pView->GetParentFrame ();
					if (pFrame != NULL)
					{
						pFrame->GetClientRect (rectClient);
						pView->SetWindowPos (NULL, 0, 0, 
											rectClient.Width (), rectClient.Height (),
											SWP_NOZORDER  | SWP_NOACTIVATE);
					}

				}
				else
				{
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
//*************************************************************************************
void CBCGPMDIChildWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CMDIChildWnd::OnSizing(fwSide, pRect);

	CRect rect;
	GetWindowRect (rect);

	if (rect.Size () != CRect (pRect).Size ())
	{
		AdjustDockingLayout ();	
	}
	
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnDestroy() 
{
	if (m_pMDIFrame->IsPrintPreview ())
	{
		m_pMDIFrame->SendMessage (WM_CLOSE);
	}

	if (m_pTabbedControlBar != NULL && CWnd::FromHandlePermanent (m_pTabbedControlBar->GetSafeHwnd ()) != NULL)
	{
		CWnd* pParent = m_pTabbedControlBar->GetParent ();
		if (pParent == this && !m_pMDIFrame->m_bClosing)
		{
			// tabbed MDI is being closed. We need to reassign parent of embedded control bar
			m_pTabbedControlBar->ShowWindow (SW_HIDE);
			m_pTabbedControlBar->SetParent (m_pTabbedControlBar->GetDockSite ());
			m_pMDIFrame->GetDockManager ()->AddHiddenMDITabbedBar (m_pTabbedControlBar);
		}
		m_pTabbedControlBar = NULL;
	}
	CBCGPFrameImpl::RemoveFrame (this);

	POSITION pos = NULL;

	for (pos = m_dockManager.m_lstMiniFrames.GetHeadPosition (); pos != NULL;)
	{
		CBCGPMiniFrameWnd* pNextFrame = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd,
					m_dockManager.m_lstMiniFrames.GetNext (pos));
		if (pNextFrame != NULL)
		{
			pNextFrame->DestroyWindow ();
		}
	}

	CList<HWND, HWND> lstChildren;
	CWnd* pNextWnd = GetTopWindow ();
	while (pNextWnd != NULL)
	{
		lstChildren.AddTail (pNextWnd->m_hWnd);
		pNextWnd = pNextWnd->GetNextWindow ();
	}

	for (pos = lstChildren.GetHeadPosition (); pos != NULL;)
	{
		HWND hwndNext = lstChildren.GetNext (pos);
		if (IsWindow (hwndNext) && ::GetParent (hwndNext) == m_hWnd)
		{
			::DestroyWindow (hwndNext);
		}
	}

	// CBCGPMainClientArea::OnMDIDestroy will take care about removing from the tabs.
	m_pRelatedTabGroup = NULL;

	CMDIChildWnd::OnDestroy();
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_pMDIFrame != NULL && m_pMDIFrame->IsFullScreen() && 
		!m_pMDIFrame->AreMDITabs())
	{
		m_pMDIFrame->m_Impl.GetFullScreenMinMaxInfo(lpMMI);
	}
	else
	{
		CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
	}
}
//*********************************************************************************
void CBCGPMDIChildWnd::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CMDIChildWnd::OnStyleChanged (nStyleType, lpStyleStruct);

	BOOL bWasSysMenu = (lpStyleStruct->styleOld & WS_SYSMENU);
	BOOL bIsSysMenu = (lpStyleStruct->styleNew & WS_SYSMENU);

	if (bWasSysMenu == bIsSysMenu)
	{
		return;
	}

	if (bWasSysMenu)
	{
		if ((lpStyleStruct->styleOld & WS_MAXIMIZE) == 0 &&
			(lpStyleStruct->styleOld & WS_MINIMIZE) == 0)
		{
			CRect rectWindow;
			GetWindowRect (rectWindow);

			GetParent()->ScreenToClient (&rectWindow);

			m_rectOriginal = rectWindow;
		}

		if ((m_pMDIFrame->m_wndClientArea.GetExStyle () & WS_EX_CLIENTEDGE) != 0)
		{
			m_pMDIFrame->m_wndClientArea.ModifyStyleEx (WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		}

		if (m_pMDIFrame != NULL && !m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ())
		{
			CRect rect;
			m_pMDIFrame->m_wndClientArea.GetClientRect (rect);

			
			CRect rectClient;
			GetClientRect (rectClient);
			ClientToScreen (rectClient);

			CRect rectScreen;
			GetWindowRect (rectScreen);

			rect.left -= rectClient.left - rectScreen.left;
			rect.top -= rectClient.top - rectScreen.top;
			rect.right += rectScreen.right - rectClient.right;
			rect.bottom += rectScreen.bottom - rectClient.bottom;

			if (!rect.IsRectNull ())
			{
				SetWindowPos (NULL, rect.left, rect.top, rect.Width (), rect.Height (), 
								SWP_NOACTIVATE | SWP_NOZORDER);
			}
		}
	}
	else if (m_pMDIFrame != NULL && !m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ())
	{
		if ((m_pMDIFrame->m_wndClientArea.GetExStyle () & WS_EX_CLIENTEDGE) == 0)
		{
			m_pMDIFrame->m_wndClientArea.ModifyStyleEx (0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		}
		
		if (!m_rectOriginal.IsRectNull ())
		{
			SetWindowPos (NULL, m_rectOriginal.left, m_rectOriginal.top, 
				m_rectOriginal.Width (), m_rectOriginal.Height (),
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
}
//*********************************************************************************
LRESULT CBCGPMDIChildWnd::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	m_dockManager.SendMessageToMiniFrames (WM_IDLEUPDATECMDUI);
	return 0L;
}
//*********************************************************************************
LPCTSTR CBCGPMDIChildWnd::GetDocumentName (CObject** /*pObj*/)
{
	CDocument* pDoc = GetActiveDocument ();
	if (pDoc != NULL)
	{
		return pDoc->GetPathName ();
	}
	return NULL;
}
//**************************************************************************
void CBCGPMDIChildWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CMDIChildWnd::OnWindowPosChanging(lpwndpos);

	if ((lpwndpos->flags & SWP_NOSIZE) == 0)
	{
		BOOL bIsInMDITabbedGroup = m_pMDIFrame != NULL && m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ();

		if (!IsZoomed () && !bIsInMDITabbedGroup &&
			CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
		{
			m_Impl.OnWindowPosChanging (lpwndpos);
		}
		else if (m_Impl.m_bIsWindowRgn)
		{
			SetWindowRgn (NULL, TRUE);
			m_Impl.m_bIsWindowRgn = FALSE;
		}
	}
}
//**************************************************************************************
void CBCGPMDIChildWnd::OnNcPaint() 
{
	BOOL bIsInMDITabbedGroup = m_pMDIFrame != NULL && m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ();

	if (bIsInMDITabbedGroup || IsZoomed () ||
		!CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ())
	{
		Default ();
		return;
	}
	
	if (!m_Impl.OnNcPaint ())
	{
		Default ();
	}
}
//*****************************************************************************************
BOOL CBCGPMDIChildWnd::OnNcActivate(BOOL bActive) 
{
	if (m_Impl.OnNcActivate (bActive))
	{
		return TRUE;
	}

	return CMDIChildWnd::OnNcActivate(bActive);
}
//**************************************************************************
void CBCGPMDIChildWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_Impl.OnNcMouseMove (nHitTest, point);
	CMDIChildWnd::OnNcMouseMove(nHitTest, point);
}
//**************************************************************************
BCGNcHitTestType CBCGPMDIChildWnd::OnNcHitTest(CPoint point) 
{
	UINT nHit = m_Impl.OnNcHitTest (point);
	if (nHit != HTNOWHERE)
	{
		return nHit;
	}

	return CMDIChildWnd::OnNcHitTest(point);
}
//**************************************************************************
LRESULT CBCGPMDIChildWnd::OnChangeVisualManager (WPARAM, LPARAM)
{
	BOOL bIsInMDITabbedGroup = m_pMDIFrame != NULL && m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ();

	if (!bIsInMDITabbedGroup)
	{
		m_Impl.OnChangeVisualManager ();
	}

	return 0;
}
//***************************************************************************
void CBCGPMDIChildWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	BOOL bIsInMDITabbedGroup = m_pMDIFrame != NULL && m_pMDIFrame->m_wndClientArea.IsMDITabbedGroup ();

	if (!bIsInMDITabbedGroup && !IsZoomed () &&
		CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption () &&
		(GetStyle () & WS_BORDER) == 0)
	{
		lpncsp->rgrc[0].top += ::GetSystemMetrics (SM_CYCAPTION);
	}
	
	CMDIChildWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}
//***************************************************************************
void CBCGPMDIChildWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonUp (point);
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}
//***************************************************************************
void CBCGPMDIChildWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_Impl.OnMouseMove (point);
	CMDIChildWnd::OnMouseMove(nFlags, point);
}
//***************************************************************************
void CBCGPMDIChildWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonDown (point);
	CMDIChildWnd::OnLButtonDown(nFlags, point);
}
//***************************************************************************
void CBCGPMDIChildWnd::AddTabbedControlBar (CBCGPDockingControlBar* pControlBar)
{
	ASSERT_VALID (pControlBar);
	m_pTabbedControlBar = pControlBar;
	
	m_pTabbedControlBar->OnBeforeChangeParent (this);
	m_pTabbedControlBar->EnableGripper (FALSE);
	m_pTabbedControlBar->SetParent (this);
	m_pTabbedControlBar->ShowWindow (SW_SHOW);
	m_pTabbedControlBar->SetMDITabbed (TRUE);

	m_pMDIFrame->GetDockManager ()->RemoveHiddenMDITabbedBar (m_pTabbedControlBar);
	
	AdjustClientArea (); 
}
//***************************************************************************
BOOL CBCGPMDIChildWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
