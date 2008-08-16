// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SetBarSize.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_DLGBAR, OnViewDialogBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DLGBAR, OnUpdateViewDialogBar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// enable Office XP look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));

	CBCGPToolBar::EnableQuickCustomization ();

	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	lstBasicCommands.AddTail (ID_FILE_NEW);
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
	lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CImageList imagesWorkspace;
	imagesWorkspace.Create (IDB_WORKSPACE, 16, 0, RGB (255, 0, 255));
	
	if (!m_wndWorkSpace.Create (_T("View  1"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	if (!m_wndDlgBar.Create (_T("DialogBar"), this, TRUE, 
							 MAKEINTRESOURCE (IDD_DLG_BAR), 
							 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI, 
							 ID_VIEW_DLGBAR))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}

	m_wndDlgBar.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar (&m_wndWorkSpace);
	m_wndDlgBar.DockToWindow (&m_wndWorkSpace, CBRS_ALIGN_BOTTOM);

	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
		);

	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	 UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
    CFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	return TRUE;
}

void CMainFrame::OnViewWorkspace() 
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsVisible ());
}

void CMainFrame::OnViewDialogBar() 
{
	ShowControlBar (&m_wndDlgBar,
					!(m_wndDlgBar.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewDialogBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndDlgBar.IsVisible ());
}

BOOL CMainFrame::FindInternalSlider (CBCGPDockingControlBar* pBar, 
									 CBCGPBarContainer*& pContainer, 
									 CBCGPSlider*& pSlider,
									 BOOL& bLeftBar, 
									 BOOL& bAloneInContainer)
{
	pContainer = NULL;
	pSlider = NULL;
	bLeftBar = FALSE;
	bAloneInContainer = FALSE;

	CBCGPSlider* pDefaultSlider = pBar->GetDefaultSlider ();
	CBCGPMultiMiniFrameWnd* pParentMiniFrame = 
		DYNAMIC_DOWNCAST (CBCGPMultiMiniFrameWnd, pBar->GetParentMiniFrame ()) ;

	CBCGPBarContainer* pStartContainer = NULL;
	
	if (pParentMiniFrame != NULL)
	{
		CBCGPBarContainerManager& manager = pParentMiniFrame->GetContainerManager ();
		pStartContainer = manager.FindContainer (pBar, bLeftBar);
	}
	else if (pDefaultSlider != NULL)
	{
		pStartContainer = pDefaultSlider->FindContainer (pBar, bLeftBar);
	}
	else
	{
		return FALSE;
	}

	if (pStartContainer != NULL)
	{
		pContainer = pStartContainer;
		
		while (pContainer != NULL)
		{
			if (!pContainer->IsRightPartEmpty (TRUE) && 
				!pContainer->IsLeftPartEmpty (TRUE))
			{
				break;	
			}
			pContainer = pContainer->GetParentContainer ();
		}

		if (pContainer == NULL)
		{
			bAloneInContainer = TRUE;
			return FALSE;
		}
		else
		{
			pSlider = (CBCGPSlider*) pContainer->GetSlider ();

			bLeftBar = (pContainer->IsLeftBar (pBar) || 
						pContainer->GetLeftContainer () != NULL && 
						((CBCGPBarContainer*)pContainer->GetLeftContainer ())-> 
						FindSubContainer (pBar, CBCGPBarContainer::BC_FIND_BY_LEFT_BAR) != NULL);
			return TRUE;
		}
	}
	return FALSE;
}

void CMainFrame::SetDlgBarSizeInContainer (UINT nSize)
{
	BOOL bLeftBar = FALSE;
	BOOL bAloneInContainer = FALSE;
	CBCGPBarContainer* pContainer = NULL;
	CBCGPSlider* pSlider = NULL;

	BOOL bResult = FindInternalSlider (&m_wndDlgBar, pContainer, pSlider, bLeftBar, bAloneInContainer);

	if (bAloneInContainer)
	{
		MessageBox (_T ("DialogBar is docked alone in container."));
		return;
	}
	if (!bResult || pContainer == NULL || pSlider == NULL)
	{
		AfxMessageBox (_T ("The DialogBar is not docked."));
		return;
	}


	CRect rectContainer;
	pContainer->GetWindowRect (rectContainer, FALSE);

	CRect rectDlgBar;
	m_wndDlgBar.GetWindowRect (rectDlgBar);

	CPoint ptOffset (0, 0);
	if (pSlider->IsHorizontal ())
	{
		ptOffset.y = rectDlgBar.Height () - rectContainer.Height () * nSize / 100;
		if (bLeftBar)
		{
			ptOffset.y = -ptOffset.y;
		}
	}
	else
	{
		ptOffset.x = rectDlgBar.Width () - rectContainer.Width () * nSize / 100;
		if (bLeftBar)
		{
			ptOffset.x = -ptOffset.x;
		}
	}

	pSlider->MoveSlider (ptOffset);
}
void CMainFrame::SetDlgBarHeightInPixels (UINT nHeight)
{
	BOOL bLeftBar = FALSE;
	BOOL bAloneInContainer = FALSE;
	CBCGPBarContainer* pContainer = NULL;
	CBCGPSlider* pSlider = NULL;

	BOOL bResult = FindInternalSlider (&m_wndDlgBar, pContainer, pSlider, bLeftBar, bAloneInContainer);

	if (bAloneInContainer)
	{
		MessageBox (_T ("DialogBar is docked alone in container."));
		return;
	}
	if (!bResult || pContainer == NULL || pSlider == NULL)
	{
		AfxMessageBox (_T ("The DialogBar is not docked."));
		return;
	}

	if (!pSlider->IsHorizontal ())
	{
		MessageBox (_T ("Internal slider is vertical, can change width only."));
		return;
	}

	CBCGPBarContainer* pRootContainer = m_wndDlgBar.GetDefaultSlider ()->FindContainer (&m_wndDlgBar, bLeftBar);	

	while (pRootContainer->GetParentContainer () != NULL)
	{
		pRootContainer = pRootContainer->GetParentContainer ();
	}

	CRect rectContainer;
	pRootContainer->GetWindowRect (rectContainer, FALSE);

	if ((UINT)rectContainer.Height () - 4 < nHeight)
	{
		CString strFormat = _T ("Required height exceeds allowed height in the current container.\nAllowed height is %u pixels.");
		CString strMessage; 
		strMessage.Format (strFormat, rectContainer.Height () - 4);
		MessageBox (strMessage);
		return;
	}


	CRect rectDlgBar;
	m_wndDlgBar.GetWindowRect (rectDlgBar);

	CPoint ptOffset (0, 0);
	ptOffset.y = rectDlgBar.Height () - nHeight;
	if (bLeftBar)
	{
		ptOffset.y = -ptOffset.y;
	}
	pSlider->MoveSlider (ptOffset);
}

void CMainFrame::SetDlgBarWidthInPixels (UINT nWidth)
{

	BOOL bLeftBar = FALSE;
	BOOL bAloneInContainer = FALSE;
	CBCGPBarContainer* pContainer = NULL;
	CBCGPSlider* pSlider = NULL;

	BOOL bResult = FindInternalSlider (&m_wndDlgBar, pContainer, pSlider, bLeftBar, bAloneInContainer);

	if (bAloneInContainer)
	{
		MessageBox (_T ("DialogBar is docked alone in container."));
		return;
	}
	if (!bResult || pContainer == NULL || pSlider == NULL)
	{
		AfxMessageBox (_T ("The DialogBar is not docked."));
		return;
	}

	if (pSlider->IsHorizontal ())
	{
		MessageBox (_T ("Internal slider is horizontal, can change height only."));
		return;
	}

	CBCGPBarContainer* pRootContainer = m_wndDlgBar.GetDefaultSlider ()->FindContainer (&m_wndDlgBar, bLeftBar);	

	while (pRootContainer->GetParentContainer () != NULL)
	{
		pRootContainer = pRootContainer->GetParentContainer ();
	}

	CRect rectContainer;
	pRootContainer->GetWindowRect (rectContainer, FALSE);

	if ((UINT) rectContainer.Width () - 4 < nWidth)
	{
		CString strFormat = _T ("Required width exceeds allowed width in the current container.\nAllowed width is %u pixels.");
		CString strMessage; 
		strMessage.Format (strFormat, rectContainer.Width () - 4);
		MessageBox (strMessage);
		return;
	}

	CRect rectDlgBar;
	m_wndDlgBar.GetWindowRect (rectDlgBar);

	CPoint ptOffset (0, 0);
	ptOffset.x = rectDlgBar.Width () - nWidth;
	if (bLeftBar)
	{
		ptOffset.x = -ptOffset.x;
	}
	pSlider->MoveSlider (ptOffset);
}
void CMainFrame::SetContainerSize (UINT nSize)
{
	CBCGPSlider* pDefaultSlider = m_wndDlgBar.GetDefaultSlider ();
	if (pDefaultSlider == NULL)
	{
		AfxMessageBox (_T ("The DialogBar is not docked."));
		return;
	}

	BOOL bLeftBar = FALSE;
	CBCGPBarContainer* pContainer = pDefaultSlider->FindContainer (&m_wndDlgBar, bLeftBar);	

	while (pContainer->GetParentContainer () != NULL)
	{
		pContainer = pContainer->GetParentContainer ();
	}

	CRect rectContainer;
	pContainer->GetWindowRect (rectContainer, FALSE);

	DWORD dwSliderStyle = pDefaultSlider->GetCurrentAlignment ();
	CPoint ptOffset (0, 0);
	switch (dwSliderStyle)
	{
	case CBRS_ALIGN_TOP:
		ptOffset.y = nSize - rectContainer.Height ();
		break;
	case CBRS_ALIGN_BOTTOM:
		ptOffset.y = rectContainer.Height () - nSize;
		break;
	case CBRS_ALIGN_LEFT:
		ptOffset.x = nSize - rectContainer.Width ();
		break;
	case CBRS_ALIGN_RIGHT:
		ptOffset.x = rectContainer.Width () - nSize;
		break;
	}
	pDefaultSlider->MoveSlider (ptOffset);
}
