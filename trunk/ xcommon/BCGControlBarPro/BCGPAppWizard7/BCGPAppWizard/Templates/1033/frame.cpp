// [!output MAIN_FRAME_IMPL] : implementation of the [!output MAIN_FRAME_CLASS] class
//

#include "stdafx.h"
#include "[!output APP_HEADER]"

#include "[!output MAIN_FRAME_HEADER]"
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
#include "[!output TREE_VIEW_HEADER]"
#include "[!output VIEW_HEADER]"
[!endif]
[!endif]

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output MAIN_FRAME_CLASS]

[!if APP_TYPE_MDI || !DOCVIEW]
IMPLEMENT_DYNAMIC([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
[!else]
IMPLEMENT_DYNCREATE([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
[!endif]

[!if USER_TOOLBAR]
const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;
[!endif]

BEGIN_MESSAGE_MAP([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
[!if DOCKING_TOOLBAR || STATUS_BAR]
	ON_WM_CREATE()
[!else]
[!if APP_TYPE_SDI && !DOCVIEW]
	ON_WM_CREATE()
[!endif]
[!endif]
[!if APP_TYPE_SDI]
[!if !DOCVIEW]
	ON_WM_SETFOCUS()
[!endif] 
[!endif] 
[!if PROJTYPE_MDI && DOCVIEW]
	ON_WM_CLOSE()
[!endif] 
[!if APP_TYPE_MTLD]
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
[!if !DOCVIEW]
	ON_WM_CLOSE()
[!endif]
[!endif] 
[!if CONTEXT_HELP]
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, [!output MAIN_FRAME_BASE_CLASS]::OnHelpFinder)
	ON_COMMAND(ID_HELP, [!output MAIN_FRAME_BASE_CLASS]::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, [!output MAIN_FRAME_BASE_CLASS]::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, [!output MAIN_FRAME_BASE_CLASS]::OnHelpFinder)
[!endif]
[!if WINDOWS_DLG]
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
[!endif]
[!if TOOLBAR_CUSTOMIZATION]
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
[!endif]
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if LIST_VIEW]
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
[!endif]
[!endif]
[!endif]
[!if TOOLBAR_CUSTOMIZATION]
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
[!endif]
[!if HELP]
[!if TOOLBAR_CUSTOMIZATION]
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
[!endif]
[!endif]
[!if OUTLOOKBAR]
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_4, OnOutlookBarShortcut)
[!endif]
[!if VISUAL_MANAGER]
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007, OnUpdateAppLook)
[!endif]
[!if MDI_TAB_GROUPS]
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
[!endif]
END_MESSAGE_MAP()
[!if STATUS_BAR]

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
[!if DO_KANA]
	ID_INDICATOR_KANA,
[!endif]
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
[!endif]


// [!output MAIN_FRAME_CLASS] construction/destruction

[!output MAIN_FRAME_CLASS]::[!output MAIN_FRAME_CLASS]()
{
[!if ACCESSIBILITY]
	EnableActiveAccessibility();
[!endif]

[!if VISUAL_MANAGER]
[!if OFFICEXP_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_XP);
[!endif]
[!if WINXP_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_WIN_XP);
[!endif]
[!if OFFICE2003_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2003);
[!endif]
[!if OFFICE2007_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2007);
[!endif]
[!if VS2005_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_VS2005);
[!endif]
[!if OFFICE200_LOOK]
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2000);
[!endif]
[!endif]
	// TODO: add member initialization code here
}

[!output MAIN_FRAME_CLASS]::~[!output MAIN_FRAME_CLASS]()
{
}

int [!output MAIN_FRAME_CLASS]::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ([!output MAIN_FRAME_BASE_CLASS]::OnCreate(lpCreateStruct) == -1)
		return -1;
[!if VISUAL_MANAGER]
	OnAppLook (m_nAppLook);

[!else]
[!if OFFICE200_LOOK]
	CBCGPToolBar::SetLook2000 ();
[!endif]
[!if OFFICEXP_LOOK]
	// enable Office XP look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
[!endif]
[!if WINXP_LOOK]
	// enable Windows XP look (in other OS Office XP look will be used):
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
[!endif]
[!if OFFICE2003_LOOK]
	// enable Office 2003 look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
[!endif]
[!if OFFICE2007_LOOK]
	// enable Office 2007 look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
[!endif]
[!if VS2005_LOOK]
	// enable VS.NET 2005 look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
	CBCGPVisualManager::GetInstance ();
[!endif]
[!if DOCK_TYPE_SMART]
	CBCGPDockManager::SetDockMode (DT_SMART);
[!else]
	CBCGPDockManager::SetDockMode (DT_STANDARD);
[!endif]

[!endif]	// VISUAL_MANAGER
[!if APP_TYPE_SDI]
[!if !DOCVIEW]
	
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
[!endif] 
[!endif]
	
[!if USER_IMAGES]
	if (CBCGPToolBar::GetUserImages () == NULL)
	{
		// Load toolbar user images:
		if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
		{
			TRACE(_T("Failed to load user images\n"));
			ASSERT (FALSE);
		}
		else
		{
			CBCGPToolBar::SetUserImages (&m_UserImages);
		}
	}

[!endif]
[!if ALT_CUSTOMIZATION]
	CBCGPToolBar::EnableQuickCustomization ();

[!endif]
[!if !VISUAL_MANAGER]
[!if MDI_TABS]
[!if MDI_TAB_GROUPS]
	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;
[!if OFFICE2003_LOOK]
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_ONENOTE;
	mdiTabParams.m_bAutoColor = TRUE;
[!endif]
[!if VS2005_LOOK]
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
	mdiTabParams.m_bDocumentMenu = TRUE;
[!endif]
	EnableMDITabbedGroups (TRUE, mdiTabParams);

[!else]
	EnableMDITabs (TRUE /* Enable */, TRUE /* With icons */,
		CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */
[!if OFFICE2003_LOOK]
		, CBCGPTabWnd::STYLE_3D_ONENOTE
[!endif]
[!if VS2005_LOOK]
		, CBCGPTabWnd::STYLE_3D_VS2005
[!endif]
		);
[!if OFFICE2003_LOOK]

	GetMDITabs().EnableAutoColor ();
[!endif]
[!if VS2005_LOOK]

	GetMDITabs().EnableTabDocumentsMenu ();
[!endif]
[!endif]
[!endif]
[!endif]

[!if RU_MENUS]
	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

[!if DOCKING_TOOLBAR]
	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
[!endif]
[!if PROJTYPE_MDI || !NODOCVIEW]
	lstBasicCommands.AddTail (ID_FILE_NEW);
[!endif]
[!if !NODOCVIEW]
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
[!endif]
[!if PRINT]
	lstBasicCommands.AddTail (ID_FILE_PRINT);
[!endif]
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
[!if CRecordView || CDaoRecordView || COleDBRecordView]
	lstBasicCommands.AddTail (ID_RECORD_NEXT);
	lstBasicCommands.AddTail (ID_RECORD_LAST);
[!endif]
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
[!if PROJTYPE_MDI]
	lstBasicCommands.AddTail (ID_WINDOW_TILE_HORZ);
[!endif]
[!if APP_TYPE_MTLD]
	lstBasicCommands.AddTail (ID_FILE_NEW_FRAME);
	lstBasicCommands.AddTail (ID_FILE_CLOSE);
[!endif]
[!if VISUAL_MANAGER]
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);
[!endif]

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

[!endif]
[!if DOCKING_TOOLBAR]
[!if MENUBAR]
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

[!if TOOLBAR_STYLE_REBAR]
	// Remove menubar gripper and borders:
	m_wndMenuBar.SetBarStyle (m_wndMenuBar.GetBarStyle() &
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

[!endif]
[!endif]
[!if TOOLBAR_256_COLORS]
	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
[!if TOOLBAR_HOT_TRACK]
	UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
[!endif]

[!endif]
[!if TOOLBAR_STYLE_REBAR]
	if (!m_wndToolBar.CreateEx(this) ||
[!else]
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
[!endif]
[!if TOOLBAR_256_COLORS]
[!if TOOLBAR_HOT_TRACK]
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, uiToolbarColdID, 0, FALSE, 0, 0, uiToolbarHotID))
[!else]
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
[!endif]
[!else]
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
[!endif]
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
[!if TOOLBAR_STYLE_REBAR]
[!if CONTAINER || CONTAINER_SERVER]
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
[!endif]
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	// Each rebar pane will ocupy its own row:
	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;

	if (!m_wndReBar.Create(this) ||
[!if MENUBAR]
		!m_wndReBar.AddBar (&m_wndMenuBar) ||
[!endif] // MENUBAR
		!m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, dwStyle) ||
		!m_wndReBar.AddBar (&m_wndDlgBar, NULL, NULL, dwStyle))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
[!if MENUBAR]

	m_wndMenuBar.AdjustLayout ();
[!endif]

	m_wndToolBar.AdjustLayout ();

	// TODO: Remove this if you don't want chevrons:
[!if MENUBAR]
	m_wndMenuBar.EnableCustomizeButton (TRUE, -1, _T(""));
[!endif]
[!if DOCKING_TOOLBAR]
[!if TOOLBAR_CUSTOMIZATION]
	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
[!else]
	m_wndToolBar.EnableCustomizeButton (TRUE, -1, _T(""));
[!endif]
[!endif]

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndReBar.EnableDocking (CBRS_TOP);
	DockControlBar (&m_wndReBar);

[!if CONTAINER || CONTAINER_SERVER]
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
[!endif]
[!endif]
[!endif]
[!if STATUS_BAR]

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
[!endif]
[!if WORKSPACEBAR]

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_WORKSPACE);
	
	if (!m_wndWorkSpace.Create (_T("View  1"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	if (!m_wndWorkSpace2.Create (_T("View 2"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE2,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return FALSE;      // fail to create
	}

	m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);

[!endif]
[!if OUTPUTBAR]

	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
[!endif]
[!if DOCKING_TOOLBAR]

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

[!if TOOLBAR_STYLE_REBAR]
	// TODO: Remove this if you don't want tool tips
[!if MENUBAR]
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
[!endif]
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]
	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
[!endif]
[!if OUTLOOKBAR]

	if (!CreateShortcutsBar ())
	{
		TRACE0("Failed to create shortcuts bar\n");
		return -1;      // fail to create
	}

[!endif]
[!if CAPTIONBAR]

	if (!m_wndCaptionBar.Create (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this,
		ID_VIEW_CAPTIONBAR, -1))
	{
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}

	m_wndCaptionBar.SetFlatBorder ();
	m_wndCaptionBar.SetText (_T("<Put your caption here>"));
	m_bmpCaption.LoadBitmap (IDB_CAPTION);
	m_wndCaptionBar.SetBitmap (m_bmpCaption, RGB (255, 0, 255));

[!endif]
[!else]
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
[!if MENUBAR]
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
[!endif]
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
[!if WORKSPACEBAR]
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
[!endif]
[!if OUTPUTBAR]
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
[!endif]
[!if OUTLOOKBAR]

	//------------------------------------------------------------------
	// Outlook bar will be created at left, so temporary disable docking
	// at the left side:
	//------------------------------------------------------------------
	EnableDocking (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	if (!CreateShortcutsBar ())
	{
		TRACE0("Failed to create shortcuts bar\n");
		return -1;      // fail to create
	}

	//----------------------------------------------------
	// Outlook bar is created and docking on the left side
	// should be allowed.
	//----------------------------------------------------
	EnableDocking (CBRS_ALIGN_LEFT);
[!else]
	EnableDocking(CBRS_ALIGN_ANY);
[!endif]
[!if CAPTIONBAR]

	if (!m_wndCaptionBar.Create (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this,
		ID_VIEW_CAPTIONBAR, -1))
	{
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}

	m_wndCaptionBar.SetFlatBorder ();
	m_wndCaptionBar.SetText (_T("<Put your caption here>"));
	m_bmpCaption.LoadBitmap (IDB_CAPTION);
	m_wndCaptionBar.SetBitmap (m_bmpCaption, RGB (255, 0, 255));

[!endif]
[!if AUTOHIDE]
	EnableAutoHideBars(CBRS_ALIGN_ANY);
[!endif]
[!if MENUBAR]
	DockControlBar(&m_wndMenuBar);
[!endif]
	DockControlBar(&m_wndToolBar);
[!if WORKSPACEBAR]
	DockControlBar(&m_wndWorkSpace);
	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, DM_STANDARD, FALSE, NULL);
[!endif]
[!if OUTPUTBAR]
	DockControlBar(&m_wndOutput);
[!endif]
[!endif]
[!endif]

[!if TOOLBAR_CUSTOMIZATION]
[!if !TOOLBAR_STYLE_REBAR]

	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
[!endif]
[!if USER_TOOLBAR]

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);
[!endif]
[!endif]
[!if WINDOWS_DLG]

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
[!endif]
[!if DOCKING_TOOLBAR]

	// Enable control bar context menu (list of bars + customize command):
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
							// toolbars menu

[!endif]
	return 0;
}
[!if SPLITTER || PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]

BOOL [!output MAIN_FRAME_CLASS]::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
[!if PROJECT_STYLE_EXPLORER]
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS([!output TREE_VIEW_CLASS]), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS([!output VIEW_CLASS]), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
[!else]
[!if HTML_VIEW || HTML_EDITVIEW]
	return m_wndSplitter.Create(this,
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext, WS_CHILD | WS_VISIBLE | SPLS_DYNAMIC_SPLIT);
[!else]
	return m_wndSplitter.Create(this,
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext);
[!endif]
[!endif]
}
[!endif]
[!endif]

BOOL [!output MAIN_FRAME_CLASS]::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ![!output MAIN_FRAME_BASE_CLASS]::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

[!if !MAIN_FRAME_DEFAULT_STYLES]
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		[!output MAIN_FRAME_STYLE_FLAGS];

[!endif]
[!if APP_TYPE_SDI]
[!if !DOCVIEW]
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
[!endif] 
[!endif]
	return TRUE;
}


// [!output MAIN_FRAME_CLASS] diagnostics

#ifdef _DEBUG
void [!output MAIN_FRAME_CLASS]::AssertValid() const
{
	[!output MAIN_FRAME_BASE_CLASS]::AssertValid();
}

void [!output MAIN_FRAME_CLASS]::Dump(CDumpContext& dc) const
{
	[!output MAIN_FRAME_BASE_CLASS]::Dump(dc);
}

#endif //_DEBUG


// [!output MAIN_FRAME_CLASS] message handlers

[!if APP_TYPE_SDI]
[!if !DOCVIEW]
void [!output MAIN_FRAME_CLASS]::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL [!output MAIN_FRAME_CLASS]::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CBCGPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
[!endif] 
[!endif]

[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if PROJECT_STYLE_EXPLORER]
[!if LIST_VIEW]
[!output VIEW_CLASS]* [!output MAIN_FRAME_CLASS]::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	[!output VIEW_CLASS]* pView = DYNAMIC_DOWNCAST([!output VIEW_CLASS], pWnd);
	return pView;
}

void [!output MAIN_FRAME_CLASS]::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the View menu

	[!output VIEW_CLASS]* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void [!output MAIN_FRAME_CLASS]::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the View menu
	[!output VIEW_CLASS]* pView = GetRightPane();

	// if the right-hand pane has been created and is a [!output VIEW_CLASS],
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}
[!endif]
[!endif]
[!endif]

[!if APP_TYPE_MTLD]
BOOL [!output MAIN_FRAME_CLASS]::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CBCGPFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pMainWnd == NULL)
		pApp->m_pMainWnd = this;

	return TRUE;
}

void [!output MAIN_FRAME_CLASS]::OnFileClose()
{
   DestroyWindow();
}
[!if !DOCVIEW]

void [!output MAIN_FRAME_CLASS]::OnClose() 
{
	[!output APP_CLASS] *pApp = reinterpret_cast<[!output APP_CLASS]*>(AfxGetApp());
	// main window is being closed so cleanup
	if (pApp->m_pMainWnd == this)
	{
		for (int iCntr=0; iCntr < pApp->m_aryFrames.GetSize(); iCntr++)
		{
			HWND hFrame = pApp->m_aryFrames.GetAt(iCntr);
			if (::IsWindow(hFrame))
				::SendMessage(hFrame, WM_CLOSE, 0, 0);
		}
	}
	CBCGPFrameWnd::OnClose();
}
[!endif]
[!endif]

[!if TOOLBAR_CUSTOMIZATION]
void [!output MAIN_FRAME_CLASS]::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
[!if HELP]
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */,
		BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
[!if SKIN_MANAGER]
		BCGCUSTOMIZE_SELECT_SKINS |
[!endif]
		BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS | BCGCUSTOMIZE_CONTEXT_HELP);
[!else]
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
[!if SKIN_MANAGER]
		, (	BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
			BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS,
			BCGCUSTOMIZE_SELECT_SKINS)
[!endif]
		);
[!endif]

[!if USER_TOOLBAR]
	pDlgCust->EnableUserDefinedToolbars ();
[!endif]
	pDlgCust->Create ();
}
[!endif]
[!if TOOLBAR_CUSTOMIZATION]

afx_msg LRESULT [!output MAIN_FRAME_CLASS]::OnToolbarReset(WPARAM /*wp*/,LPARAM)
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
[!endif]
[!if HELP]
[!if TOOLBAR_CUSTOMIZATION]

LRESULT [!output MAIN_FRAME_CLASS]::OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp)
{
	int iPageNum = (int) wp;

	CBCGPToolbarCustomize* pDlg = (CBCGPToolbarCustomize*) lp;
	ASSERT_VALID (pDlg);

	// TODO: show help about page number iPageNum

	return 0;
}
[!endif]
[!endif]
[!if OUTLOOKBAR]

void [!output MAIN_FRAME_CLASS]::OnOutlookBarShortcut(UINT id)
{
	// TODO: process shortcuts bar commands here...
}
[!endif]
[!if OUTLOOKBAR]

BOOL [!output MAIN_FRAME_CLASS]::CreateShortcutsBar ()
{
	CBCGPOutlookWnd::EnableAnimation ();

	const int nInitialWidth = 150;
	const CString strCaption = _T("Shortcuts");

	if (!m_wndShortcutsBar.Create (strCaption, this, 
		CRect (0, 0, nInitialWidth, nInitialWidth), 
		ID_VIEW_OUTLOOKBAR, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE;
	}

	CBCGPOutlookWnd* pShortcutsBarContainer = DYNAMIC_DOWNCAST (CBCGPOutlookWnd, 
							m_wndShortcutsBar.GetUnderlinedWindow ());
	if (pShortcutsBarContainer == NULL)
	{
		TRACE0("Cannot get outlook bar container\n");
		return FALSE;
	}

	CImageList images;
	images.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 255));
[!if OUTLOOK_PAGES]

	// Create first page:
	m_wndShortcutsPane1.Create (&m_wndShortcutsBar, dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_1);
	m_wndShortcutsPane1.SetOwner (this);
	m_wndShortcutsPane1.EnableTextLabels ();
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);

	m_wndShortcutsPane1.AddButton (images.ExtractIcon (0), _T("Shortcut 1"), ID_SHORTCUT_1);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (1), _T("Shortcut 2"), ID_SHORTCUT_2);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane1, _T("Page 1"), -1, FALSE);
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);

	// Create second page:
	m_wndShortcutsPane2.Create (&m_wndShortcutsBar, 
		dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_2);

	m_wndShortcutsPane2.SetOwner (this);
	m_wndShortcutsPane2.EnableTextLabels ();
	m_wndShortcutsPane2.EnableDocking (CBRS_ALIGN_ANY);

	m_wndShortcutsPane2.AddButton (images.ExtractIcon (2), _T("Shortcut 3"), ID_SHORTCUT_3);
	m_wndShortcutsPane2.AddButton (images.ExtractIcon (3), _T("Shortcut 4"), ID_SHORTCUT_4);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane2, _T("Page 2"), -1, FALSE);

[!if OUTLOOK_EMBEDDED_CONTROL]
	const int nNextPage = 2;
[!endif]

[!else]

	m_wndShortcutsPane1.Create (&m_wndShortcutsBar, dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_1);
	m_wndShortcutsPane1.SetOwner (this);
	m_wndShortcutsPane1.EnableTextLabels ();
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);

	m_wndShortcutsPane1.AddButton (images.ExtractIcon (0), _T("Shortcut 1"), ID_SHORTCUT_1);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (1), _T("Shortcut 2"), ID_SHORTCUT_2);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (2), _T("Shortcut 3"), ID_SHORTCUT_3);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (3), _T("Shortcut 4"), ID_SHORTCUT_4);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane1, _T("Page 1"), -1, FALSE);
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);

[!if OUTLOOK_EMBEDDED_CONTROL]
	const int nNextPage = 1;
[!endif]

[!endif]
[!if OUTLOOK_EMBEDDED_CONTROL]

	const DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndShortcutsTreeWnd.Create (dwTreeStyle, CRect (0, 0, 0, 0), 
		&m_wndShortcutsBar, ID_SHORTCUTS_TREE);

	HTREEITEM hRoot = m_wndShortcutsTreeWnd.InsertItem (_T("Folders"));
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 1"), hRoot);
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 2"), hRoot);
	m_wndShortcutsTreeWnd.Expand (hRoot, TVE_EXPAND);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsTreeWnd, _T ("<Control>"), -1, FALSE);

	CBCGPDockingCBWrapper* pWrapper = DYNAMIC_DOWNCAST (CBCGPDockingCBWrapper, 
							pShortcutsBarContainer->GetTabWnd (nNextPage));
	if (pWrapper != NULL)
	{
		ASSERT_VALID (pWrapper);
		pWrapper->SetTabbedControlBarRTC (RUNTIME_CLASS (CBCGPOutlookBar));
	}

[!endif]
	return TRUE;
}
[!endif]
[!if WINDOWS_DLG]

void [!output MAIN_FRAME_CLASS]::OnWindowManager() 
{
	ShowWindowsDialog ();
}
[!endif]
[!if VISUAL_MANAGER]

void [!output MAIN_FRAME_CLASS]::OnAppLook(UINT id)
{
[!if DOCKING_STYLE_SMART]
	CBCGPDockManager::SetDockMode (DT_SMART);
[!else]
	CBCGPDockManager::SetDockMode (DT_STANDARD);
[!endif]

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

[!if MDI_TAB_GROUPS]
	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;

[!endif]
	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
[!if MDI_TABS]
[!if !MDI_TAB_GROUPS]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);

		CBCGPToolBar::SetLook2000 ();
[!endif]
[!endif]
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
[!if MDI_TABS]
[!if !MDI_TAB_GROUPS]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
[!endif]
[!endif]
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
[!if MDI_TABS]
[!if !MDI_TAB_GROUPS]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
[!endif]
[!endif]
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
[!if MDI_TABS]
[!if MDI_TAB_GROUPS]

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bAutoColor = TRUE;
[!else]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_ONENOTE);
		GetMDITabs().EnableAutoColor ();
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
[!endif]
[!endif]
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007:
		// enable Office 2007 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
[!if MDI_TABS]
[!if MDI_TAB_GROUPS]

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bAutoColor = TRUE;
[!else]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_ONENOTE);
		GetMDITabs().EnableAutoColor ();
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
[!endif]
[!endif]
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS.NET 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
[!if MDI_TABS]
[!if MDI_TAB_GROUPS]

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bDocumentMenu = TRUE;
[!else]

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_VS2005);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu ();
[!endif]
[!endif]
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}
[!if MDI_TAB_GROUPS]

	EnableMDITabbedGroups (TRUE, mdiTabParams);
[!endif]

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

void [!output MAIN_FRAME_CLASS]::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}
[!endif]
[!if MDI_TAB_GROUPS]

BOOL [!output MAIN_FRAME_CLASS]::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;
}

void [!output MAIN_FRAME_CLASS]::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void [!output MAIN_FRAME_CLASS]::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void [!output MAIN_FRAME_CLASS]::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void [!output MAIN_FRAME_CLASS]::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void [!output MAIN_FRAME_CLASS]::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}
[!endif]
[!if PROJTYPE_MDI && DOCVIEW]

CBCGPMDIChildWnd* [!output MAIN_FRAME_CLASS]::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/)
{
	if (lpcszDocName != NULL && lpcszDocName [0] != '\0')
	{
		CDocument* pDoc = AfxGetApp()->OpenDocumentFile (lpcszDocName);

		if (pDoc != NULL)
		{
			POSITION pos = pDoc->GetFirstViewPosition();

			if (pos != NULL)
			{
				CView* pView = pDoc->GetNextView (pos);
				if (pView == NULL)
				{
					return NULL;
				}

				return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pView->GetParent ());
			}   
		}
	}

	return NULL;
}

void [!output MAIN_FRAME_CLASS]::OnClose() 
{
	SaveMDIState (theApp.GetRegSectionPath ());
	[!output MAIN_FRAME_BASE_CLASS]::OnClose();
}

[!endif]


