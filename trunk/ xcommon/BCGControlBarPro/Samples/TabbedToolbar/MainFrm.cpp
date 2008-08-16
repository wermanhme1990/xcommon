// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TabbedToolbar.h"

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
	ON_COMMAND_RANGE(ID_TOOLS_BPOINTER, ID_BUTTON33033, OnToolsButton)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_BPOINTER, ID_BUTTON33033, OnUpdateToolsButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
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
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));

	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	CBCGPToolBar::SetSizes (CSize (28, 28), CSize (24, 24));
	CBCGPToolBar::SetMenuSizes (CSize (22, 21), CSize (16, 15));

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

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBarStandard.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+100) ||
		!m_wndToolBarStandard.LoadToolBar(IDR_TOOLS_BSTANDARD))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBarAdditional.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+101) ||
		!m_wndToolBarAdditional.LoadToolBar(IDR_TOOLS_BADDITIONAL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarWin32.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+102) ||
		!m_wndToolBarWin32.LoadToolBar(IDR_TOOLS_BWIN32))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarSystem.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+103) ||
		!m_wndToolBarSystem.LoadToolBar(IDR_TOOLS_BSYSTEM))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarInternet.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+104) ||
		!m_wndToolBarInternet.LoadToolBar(IDR_TOOLS_BINTERNET))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarDataAccess.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+105) ||
		!m_wndToolBarDataAccess.LoadToolBar(IDR_TOOLS_BDATAACCESS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarDataControls.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+106) ||
		!m_wndToolBarDataControls.LoadToolBar(IDR_TOOLS_BDATACONTROLS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarQReports.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+107) ||
		!m_wndToolBarQReports.LoadToolBar(IDR_TOOLS_BQREPORT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarDialogs.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+108) ||
		!m_wndToolBarDialogs.LoadToolBar(IDR_TOOLS_BDIALOGS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarActiveX.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+109) ||
		!m_wndToolBarActiveX.LoadToolBar(IDR_TOOLS_BACTIVEX))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndTabbedToolBar.Create (this))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndTabbedToolBar.SetWindowText (_T("Component palette"));
	m_wndToolBarStandard.SetWindowText (_T("Standard"));
	m_wndToolBarAdditional.SetWindowText (_T("Additional"));
	m_wndToolBarWin32.SetWindowText (_T("Win32"));
	m_wndToolBarSystem.SetWindowText (_T("System"));
	m_wndToolBarInternet.SetWindowText (_T("Internet"));
	m_wndToolBarDataAccess.SetWindowText (_T("Data access"));
	m_wndToolBarDataControls.SetWindowText (_T("Data controls"));
	m_wndToolBarQReports.SetWindowText (_T("QReports"));
	m_wndToolBarDialogs.SetWindowText (_T("Dialogs"));
	m_wndToolBarActiveX.SetWindowText (_T("ActiveX"));
	// TODO: Delete these lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTabbedToolBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndTabbedToolBar);

	m_wndTabbedToolBar.AddTab (&m_wndToolBarStandard);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarAdditional, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarWin32, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarSystem, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarInternet, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarDataAccess, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarDataControls, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarQReports, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarDialogs, TRUE, FALSE, TRUE);
	m_wndTabbedToolBar.AddTab (&m_wndToolBarActiveX, TRUE, FALSE, TRUE);

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

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
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

void CMainFrame::OnToolsButton(UINT uiCmd)
{
}

void CMainFrame::OnUpdateToolsButton(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
