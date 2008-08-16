// $$frame_ifile$$.cpp : implementation of the $$FRAME_CLASS$$ class
//

#include "stdafx.h"
#include "$$root$$.h"

#include "$$frame_hfile$$.h"
$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)
#include "$$treeview_hfile$$.h"
#include "$$view_hfile$$.h"
$$ENDIF
$$ENDIF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$

$$IF(PROJTYPE_MDI || NODOCVIEW)
IMPLEMENT_DYNAMIC($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
$$ELSE
IMPLEMENT_DYNCREATE($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
$$ENDIF

$$IF(USER_TOOLBAR)
const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;
$$ENDIF

BEGIN_MESSAGE_MAP($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
	//{{AFX_MSG_MAP($$FRAME_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
$$ENDIF
$$IF(TOOLBAR || STATUSBAR || MENUBAR || REBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE)
	ON_WM_CREATE()
$$ENDIF //TOOLBAR || STATUSBAR || MENUBAR || REBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
	ON_WM_SETFOCUS()
$$ENDIF 
$$ENDIF 
	//}}AFX_MSG_MAP
$$IF(HELP)
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, $$FRAME_BASE_CLASS$$::OnHelpFinder)
	ON_COMMAND(ID_HELP, $$FRAME_BASE_CLASS$$::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, $$FRAME_BASE_CLASS$$::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, $$FRAME_BASE_CLASS$$::OnHelpFinder)
$$ENDIF
$$IF(WINDOWS_DLG)
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
$$ENDIF
$$IF(TOOLBAR_CUSTOMIZATION)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
$$ENDIF
$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)
$$IF(CListView)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
$$ENDIF
$$ENDIF
$$ENDIF
$$IF(TOOLBAR_CUSTOMIZATION)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
$$ENDIF
$$IF(HELP)
$$IF(TOOLBAR_CUSTOMIZATION)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
$$ENDIF
$$ENDIF
$$IF(OUTLOOKBAR)
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_4, OnOutlookBarShortcut)
$$ENDIF
$$IF(TASKPANE)
	ON_COMMAND(ID_TASK1, OnTask)
	ON_COMMAND(ID_TASK2, OnTask)
	ON_COMMAND(ID_TASK3, OnTask)
	ON_COMMAND(ID_TASK4, OnTask)
	ON_COMMAND(ID_TASK5, OnTask)
$$ENDIF
$$IF(VISUAL_MANAGER)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007, OnUpdateAppLook)
$$ENDIF
$$IF(MDI_TAB_GROUPS)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
$$ENDIF
END_MESSAGE_MAP()
$$IF(STATUSBAR)

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
$$IF(DO_KANA)
	ID_INDICATOR_KANA,
$$ENDIF	// DO_KANA
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
$$ENDIF //STATUSBAR

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ construction/destruction

$$FRAME_CLASS$$::$$FRAME_CLASS$$()
{
$$IF(VISUAL_MANAGER)
$$IF(OFFICE_XP_LOOK)
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_XP);
$$ELIF(WIN_XP_LOOK)
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_WIN_XP);
$$ELIF(OFFICE2003_LOOK)
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2003);
$$ELIF(VS2005_LOOK)
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_VS2005);
$$ELIF(OFFICE2007_LOOK)
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2007);
$$ELSE
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2000);
$$ENDIF
$$ENDIF
$$IF(VERBOSE)

	// TODO: add member initialization code here
	
$$ENDIF
}

$$FRAME_CLASS$$::~$$FRAME_CLASS$$()
{
}
$$IF(TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE)

int $$FRAME_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ($$FRAME_BASE_CLASS$$::OnCreate(lpCreateStruct) == -1)
		return -1;

$$IF(VISUAL_MANAGER)
	OnAppLook (m_nAppLook);

$$ELSE
$$IF(OFFICE_XP_LOOK)
$$IF(VERBOSE)
	// enable Office XP look:
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
$$ENDIF
$$IF(WIN_XP_LOOK)
$$IF(VERBOSE)
	// enable Windows XP look (in other OS Office XP look will be used):
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
$$ENDIF
$$IF(OFFICE2003_LOOK)
$$IF(VERBOSE)
	// enable Office 2003 look:
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
$$ENDIF
$$IF(OFFICE2007_LOOK)
$$IF(VERBOSE)
	// enable Office 2007 look:
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
$$ENDIF
$$IF(VS2005_LOOK)
$$IF(VERBOSE)
	// enable VS.NET 2005 look:
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
	CBCGPVisualManager::GetInstance ();
$$ENDIF
$$IF(DOCKING_STYLE_SMART)
	CBCGPDockManager::SetDockMode (DT_SMART);
$$ELSE
	CBCGPDockManager::SetDockMode (DT_STANDARD);
$$ENDIF

$$ENDIF	// VISUAL_MANAGER
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

$$ENDIF 
$$ENDIF //SDI + NODOCVIEW
$$IF(USER_IMAGES)
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

$$ENDIF
$$IF(ALT_CUSTOMIZATION)
	CBCGPToolBar::EnableQuickCustomization ();

$$ENDIF
$$IF(!VISUAL_MANAGER)
$$IF(MDI_TABS)
$$IF(MDI_TAB_GROUPS)
	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;
$$IF(OFFICE2003_LOOK)
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_ONENOTE;
	mdiTabParams.m_bAutoColor = TRUE;
$$ELIF(VS2005_LOOK)
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
	mdiTabParams.m_bDocumentMenu = TRUE;
$$ELSE
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_SCROLLED;
$$ENDIF
	EnableMDITabbedGroups (TRUE, mdiTabParams);

$$ELSE
	EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */, 
					CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */
$$IF(OFFICE2003_LOOK)
			, CBCGPTabWnd::STYLE_3D_ONENOTE);
$$ELIF(VS2005_LOOK)
			, CBCGPTabWnd::STYLE_3D_VS2005);
$$ELSE
		);
$$ENDIF
$$IF(OFFICE2003_LOOK)

	GetMDITabs().EnableAutoColor ();

$$ENDIF
$$IF(VS2005_LOOK)

	GetMDITabs().EnableTabDocumentsMenu ();

$$ENDIF	// VS2005_LOOK
$$ENDIF	// MDI_TAB_GROUPS
$$ENDIF	// MDITABS
$$ENDIF
$$IF(OFFICE200_LOOK)
	CBCGPToolBar::SetLook2000 ();
$$ENDIF
$$IF(RU_MENUS)
$$IF(VERBOSE)

	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.
$$ENDIF

	CList<UINT, UINT>	lstBasicCommands;

$$IF(TOOLBAR)
	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
$$ENDIF
$$IF(PROJTYPE_MDI || !NODOCVIEW)
	lstBasicCommands.AddTail (ID_FILE_NEW);
$$ENDIF
$$IF(!NODOCVIEW)
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
$$ENDIF
$$IF(PRINT)
	lstBasicCommands.AddTail (ID_FILE_PRINT);
$$ENDIF //PRINT
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
$$IF(CRecordView || CDaoRecordView || COleDBRecordView)
	lstBasicCommands.AddTail (ID_RECORD_NEXT);
	lstBasicCommands.AddTail (ID_RECORD_LAST);
$$ENDIF
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
$$IF(PROJTYPE_MDI)
	lstBasicCommands.AddTail (ID_WINDOW_TILE_HORZ);
$$ENDIF
$$IF(VISUAL_MANAGER)
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);
$$ENDIF

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

$$ENDIF
$$IF(TOOLBAR)
$$IF(MENUBAR)
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

$$IF(REBAR)
	// Remove menubar gripper and borders:
	m_wndMenuBar.SetBarStyle (m_wndMenuBar.GetBarStyle() &
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

$$ENDIF // REBAR
$$ENDIF // MENUBAR
$$IF(TOOLBAR_256_COLORS)
	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
$$IF(TOOLBAR_HOT_TRACK)
	UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
$$ENDIF // TOOLBAR_HOT_TRACK

$$ENDIF // TOOLBAR_256_COLORS
$$IF(REBAR)
	if (!m_wndToolBar.CreateEx(this) ||
$$ELSE
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
$$ENDIF //REBAR
$$IF(TOOLBAR_256_COLORS)
$$IF(TOOLBAR_HOT_TRACK)
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, uiToolbarColdID, 0, FALSE, 0, 0, uiToolbarHotID))
$$ELSE
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
$$ENDIF // TOOLBAR_HOT_TRACK
$$ELSE
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
$$ENDIF // TOOLBAR_256_COLORS
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
$$IF(REBAR)

	// Remove toolbar gripper and borders:
	m_wndToolBar.SetBarStyle (m_wndToolBar.GetBarStyle() &
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

$$ENDIF // REBAR
$$IF(REBAR)
$$IF( CONTAINER || CONTAINER_SERVER)
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
$$ENDIF // ANY CONTAINER

	// Each rebar pane will ocupy its own row:
	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;

	if (!m_wndReBar.Create(this) ||
$$IF(MENUBAR)
		!m_wndReBar.AddBar (&m_wndMenuBar) ||
$$ENDIF // MENUBAR
		!m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, dwStyle))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

$$IF(MENUBAR)
	m_wndMenuBar.AdjustLayout ();
$$ENDIF // MENUBAR
	m_wndToolBar.AdjustLayout ();

	// TODO: Remove this if you don't want chevrons:
$$IF(MENUBAR)
	m_wndMenuBar.EnableCustomizeButton (TRUE, -1, _T(""));
$$ENDIF // MENUBAR
$$IF(TOOLBAR)
$$IF(TOOLBAR_CUSTOMIZATION)
	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
$$ELSE
	m_wndToolBar.EnableCustomizeButton (TRUE, -1, _T(""));
$$ENDIF //TOOLBAR_CUSTOMIZATION
$$ENDIF //TOOLBAR

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndReBar.EnableDocking (CBRS_TOP);
	DockControlBar (&m_wndReBar);

$$IF( CONTAINER || CONTAINER_SERVER)
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

$$ENDIF// ANY CONTAINER
$$ENDIF //REBAR
$$ENDIF //TOOLBAR
$$IF(STATUSBAR)
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
$$ENDIF //STATUSBAR
$$IF(WORKSPACEBAR || OUTPUTBAR)

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_WORKSPACE);
$$ENDIF //STATUSBAR
$$IF(WORKSPACEBAR)

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
$$ENDIF //WORKSPACEBAR
$$IF(OUTPUTBAR)

	if (!m_wndOutput.Create (_T("Output"), this, CRect (0, 0, 150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);
$$ENDIF //OUTPUTBAR
$$IF(TASKPANE)

	if (!CreateTaskPane ())
	{
		TRACE0("Failed to create tasks pane\n");
		return -1;      // fail to create
	}
$$ENDIF //TASKPANE
$$IF(TOOLBAR)

	m_wndToolBar.SetWindowText (_T("Standard"));
$$IF(REBAR)
$$IF(VERBOSE)
	// TODO: Remove this if you don't want tool tips
$$ENDIF
$$IF(MENUBAR)
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
$$ENDIF	// MENUBAR
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
$$IF(OUTLOOKBAR)

	if (!CreateShortcutsBar ())
	{
		TRACE0("Failed to create shortcuts bar\n");
		return -1;      // fail to create
	}

$$ENDIF //!OUTLOOKBAR
$$IF(CAPTIONBAR)

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
$$ENDIF //CAPTIONBAR
$$ELSE // REBAR
$$IF(VERBOSE)
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
$$ENDIF
$$IF(MENUBAR)
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
$$ENDIF	// MENUBAR
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
$$IF(WORKSPACEBAR)
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
$$ENDIF	// WORKSPACEBAR
$$IF(OUTPUTBAR)
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
$$ENDIF	// OUTPUTBAR
$$IF(TASKPANE)
	m_wndTaskPane.EnableDocking(CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT);
$$ENDIF	// TASKPANE
$$IF(OUTLOOKBAR)

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
$$ELSE //!OUTLOOKBAR
	EnableDocking(CBRS_ALIGN_ANY);
$$ENDIF //OUTLOOKBAR
$$IF(CAPTIONBAR)

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

$$ENDIF //CAPTIONBAR
$$IF(AUTO_HIDE)
	EnableAutoHideBars(CBRS_ALIGN_ANY);
$$ENDIF //AUTOHIDE
$$IF(MENUBAR)
	DockControlBar(&m_wndMenuBar);
$$ENDIF	// MENUBAR
	DockControlBar(&m_wndToolBar);
$$IF(WORKSPACEBAR)
	DockControlBar (&m_wndWorkSpace);
	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, DM_STANDARD, FALSE, NULL);
$$ENDIF	// WORKSPACEBAR
$$IF(OUTPUTBAR)
	DockControlBar(&m_wndOutput);
$$ENDIF	// OUTPUTBAR
$$IF(TASKPANE)
	DockControlBar(&m_wndTaskPane);
$$ENDIF	// TASKPANE
$$ENDIF //REBAR
$$ENDIF //TOOLBAR	

$$IF(TOOLBAR_CUSTOMIZATION)
$$IF(USER_TOOLBAR)

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);
$$ENDIF
$$IF(!REBAR)

$$IF(TOOLBAR)
	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
$$ENDIF	//TOOLBAR
$$ENDIF	//!REBAR
$$ENDIF
$$IF(WINDOWS_DLG)

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
$$ENDIF
$$IF(TOOLBAR)

	// Enable conttol bar context menu (list of bars + customize command):
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
							// toolbars menu

$$ENDIF	//TOOLBAR
	return 0;
}
$$ENDIF /// TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE
$$IF(SPLITTER_SDI || PROJTYPE_EXPLORER )
$$IF(PROJTYPE_SDI)

BOOL $$FRAME_CLASS$$::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
$$IF(PROJTYPE_EXPLORER)
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS($$TREEVIEW_CLASS$$), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS($$VIEW_CLASS$$), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
$$ELSE //!PROJTYPE_EXPLORER
	return m_wndSplitter.Create(this,
$$IF(VERBOSE)
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
$$ELSE //!VERBOSE
		2, 2,
		CSize(10, 10),
$$ENDIF //VERBOSE
		pContext);
$$ENDIF //PROJTYPE_EXPLORER
}
$$ENDIF //SDI
$$ENDIF //SPLITTER || EXPLORER

BOOL $$FRAME_CLASS$$::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !$$FRAME_BASE_CLASS$$::PreCreateWindow(cs) )
		return FALSE;
$$IF(VERBOSE)
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

$$ENDIF //VERBOSE
$$IF(FRAME_STYLES)
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		$$FRAME_STYLE_FLAGS$$;

$$ENDIF //FRAME_STYLES
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
$$ENDIF 
$$ENDIF //SDI + NODOCVIEW
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ diagnostics

#ifdef _DEBUG
void $$FRAME_CLASS$$::AssertValid() const
{
	$$FRAME_BASE_CLASS$$::AssertValid();
}

void $$FRAME_CLASS$$::Dump(CDumpContext& dc) const
{
	$$FRAME_BASE_CLASS$$::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ message handlers
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
void $$FRAME_CLASS$$::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL $$FRAME_CLASS$$::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
$$ENDIF 
$$ENDIF //SDI + NODOCVIEW

$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)
$$IF(CListView)
$$VIEW_CLASS$$* $$FRAME_CLASS$$::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	$$VIEW_CLASS$$* pView = DYNAMIC_DOWNCAST($$VIEW_CLASS$$, pWnd);
	return pView;
}

void $$FRAME_CLASS$$::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
$$IF(VERBOSE)
	// TODO: customize or extend this code to handle choices on the
	// View menu.
$$ENDIF //VERBOSE

	$$VIEW_CLASS$$* pView = GetRightPane(); 

$$IF(VERBOSE)
	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range
$$ENDIF //VERBOSE

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

$$IF(VERBOSE)
		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

$$ENDIF //VERBOSE
		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
$$IF(VERBOSE)
			// otherwise, use dots to reflect the style of the view
$$ENDIF //VERBOSE
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


void $$FRAME_CLASS$$::OnViewStyle(UINT nCommandID)
{
$$IF(VERBOSE)
	// TODO: customize or extend this code to handle choices on the
	// View menu.
$$ENDIF //VERBOSE
	$$VIEW_CLASS$$* pView = GetRightPane();

$$IF(VERBOSE)
	// if the right-hand pane has been created and is a $$VIEW_CLASS$$,
	// process the menu commands...
$$ENDIF //VERBOSE
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
$$IF(VERBOSE)
				// ask the list control to snap to grid
$$ENDIF //VERBOSE
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

$$IF(VERBOSE)
		// other commands change the style on the list control
$$ENDIF //VERBOSE
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

$$IF(VERBOSE)
		// change the style; window will repaint automatically
$$ENDIF //VERBOSE
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}
$$ENDIF
$$ENDIF
$$ENDIF

$$IF(TOOLBAR_CUSTOMIZATION)
void $$FRAME_CLASS$$::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
$$IF(HELP)
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */,
		BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
$$IF(SKIN_MANAGER)
		BCGCUSTOMIZE_SELECT_SKINS |
$$ENDIF	// SKIN_MANAGER
		BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS | BCGCUSTOMIZE_CONTEXT_HELP);
$$ELSE
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
$$IF(SKIN_MANAGER)
		, (	BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
			BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS,
			BCGCUSTOMIZE_SELECT_SKINS)
$$ENDIF	// SKIN_MANAGER
		);
$$ENDIF

$$IF(USER_TOOLBAR)
	pDlgCust->EnableUserDefinedToolbars ();
$$ENDIF
	pDlgCust->Create ();
}
$$ENDIF	// TOOLBAR_CUSTOMIZATION
$$IF(TOOLBAR_CUSTOMIZATION)

afx_msg LRESULT $$FRAME_CLASS$$::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
$$IF(VERBOSE)
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

$$ENDIF
	return 0;
}
$$ENDIF
$$IF(HELP)
$$IF(TOOLBAR_CUSTOMIZATION)

LRESULT $$FRAME_CLASS$$::OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp)
{
	int iPageNum = (int) wp;

	CBCGPToolbarCustomize* pDlg = (CBCGPToolbarCustomize*) lp;
	ASSERT_VALID (pDlg);

$$IF(VERBOSE)
	// TODO: show help about page number iPageNum

$$ENDIF

	return 0;
}
$$ENDIF
$$ENDIF
$$IF(OUTLOOKBAR)

void $$FRAME_CLASS$$::OnOutlookBarShortcut(UINT id)
{
$$IF(VERBOSE)
	// TODO: process shortcuts bar commands here...
$$ENDIF
}

BOOL $$FRAME_CLASS$$::CreateShortcutsBar ()
{
	CBCGPOutlookWnd::EnableAnimation ();

	const int nInitialWidth = 150;
	const CString strCaption = _T("Shortcuts");

	if (!m_wndShortcutsBar.Create (strCaption, this, 
		CRect (0, 0, nInitialWidth, nInitialWidth), 
		ID_VIEW_OUTLOOKBAR, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		TRACE0("Failed to create outlook bar\n");
		return FALSE;      // fail to create
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

	// Create first page:
	m_wndShortcutsPane1.Create (&m_wndShortcutsBar, dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_1);
	m_wndShortcutsPane1.SetOwner (this);
	m_wndShortcutsPane1.EnableTextLabels ();
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);
$$IF(OUTLOOK_BACKGROUND)

	m_wndShortcutsPane1.SetBackImage (IDB_SHORTCUTS_BKGND);
	m_wndShortcutsPane1.SetTextColor (RGB (0, 0, 0));
$$ENDIF	// OUTLOOK_BACKGROUND
	
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
$$IF(OUTLOOK_BACKGROUND)

	m_wndShortcutsPane2.SetBackImage (IDB_SHORTCUTS_BKGND);
	m_wndShortcutsPane2.SetTextColor (RGB (0, 0, 0));
$$ENDIF	// OUTLOOK_BACKGROUND

	m_wndShortcutsPane2.AddButton (images.ExtractIcon (2), _T("Shortcut 3"), ID_SHORTCUT_3);
	m_wndShortcutsPane2.AddButton (images.ExtractIcon (3), _T("Shortcut 4"), ID_SHORTCUT_4);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane2, _T("Page 2"), -1, FALSE);

$$IF(OUTLOOK_EMBEDDED_CONTROL)

	const DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndShortcutsTreeWnd.Create (dwTreeStyle, CRect (0, 0, 0, 0), 
		&m_wndShortcutsBar, ID_SHORTCUTS_TREE);

	HTREEITEM hRoot = m_wndShortcutsTreeWnd.InsertItem (_T("Folders"));
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 1"), hRoot);
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 2"), hRoot);
	m_wndShortcutsTreeWnd.Expand (hRoot, TVE_EXPAND);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsTreeWnd, _T ("Page 3"), -1, FALSE);
$$IF(OUTLOOK_TABING)

	CBCGPDockingCBWrapper* pWrapper = DYNAMIC_DOWNCAST (CBCGPDockingCBWrapper, 
							pShortcutsBarContainer->GetTabWnd (2));
	ASSERT_VALID (pWrapper);
	pWrapper->SetTabbedControlBarRTC (RUNTIME_CLASS (CBCGPOutlookBar));
$$ENDIF

$$ENDIF	// OUTLOOK_EMBEDDED_CONTROL
	return TRUE;
}
$$ENDIF
$$IF(TASKPANE)

BOOL $$FRAME_CLASS$$::CreateTaskPane ()
{
	CRect rectDummy(0, 0, 200, 400);
	if (!m_wndTaskPane.Create(_T("Tasks Pane"), this, rectDummy, TRUE, ID_VIEW_TASKPANE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_BCGP_CLOSE | CBRS_BCGP_FLOAT))
	{
		TRACE0("Failed to create task pane\n");
		return FALSE;      // fail to create
	}

	return TRUE;
}

void $$FRAME_CLASS$$::OnTask()
{
	MessageBox (_T("TODO: Task"));
}
$$ENDIF
$$IF(WINDOWS_DLG)

void $$FRAME_CLASS$$::OnWindowManager() 
{
	ShowWindowsDialog ();
}
$$ENDIF
$$IF(VISUAL_MANAGER)

void $$FRAME_CLASS$$::OnAppLook(UINT id)
{
$$IF(DOCKING_STYLE_SMART)
	CBCGPDockManager::SetDockMode (DT_SMART);
$$ELSE
	CBCGPDockManager::SetDockMode (DT_STANDARD);
$$ENDIF

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

$$IF(MDI_TAB_GROUPS)
	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;

$$ENDIF
	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
$$IF(MDI_TABS)
$$IF(!MDI_TAB_GROUPS)

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
$$ENDIF
$$ENDIF
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
$$IF(MDI_TABS)
$$IF(!MDI_TAB_GROUPS)

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
$$ENDIF
$$ENDIF
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
$$IF(MDI_TABS)
$$IF(!MDI_TAB_GROUPS)

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
$$ENDIF
$$ENDIF
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
$$IF(MDI_TABS)
$$IF(MDI_TAB_GROUPS)

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bAutoColor = TRUE;
$$ELSE

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_ONENOTE);
		GetMDITabs().EnableAutoColor ();
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
$$ENDIF
$$ENDIF
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS.NET 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
$$IF(MDI_TABS)
$$IF(MDI_TAB_GROUPS)

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bDocumentMenu = TRUE;
$$ELSE

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_VS2005);
		GetMDITabs().EnableAutoColor (FALSE);
		GetMDITabs().EnableTabDocumentsMenu ();
$$ENDIF
$$ENDIF
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007:
		// enable Office 2007 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
$$IF(MDI_TABS)
$$IF(MDI_TAB_GROUPS)

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_SCROLLED;
$$ELSE

		EnableMDITabs (	TRUE /* Enable */, TRUE /* With icons */,
						CBCGPTabWnd::LOCATION_TOP, TRUE /* Close button */,
						CBCGPTabWnd::STYLE_3D_SCROLLED);
		GetMDITabs().EnableTabDocumentsMenu (FALSE);
$$ENDIF
$$ENDIF
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}
$$IF(MDI_TAB_GROUPS)

	EnableMDITabbedGroups (TRUE, mdiTabParams);
$$ENDIF
$$IF(TASKPANE)

	m_wndTaskPane.RecalcLayout ();
$$ENDIF

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

void $$FRAME_CLASS$$::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}
$$ENDIF
$$IF(MDI_TAB_GROUPS)

BOOL $$FRAME_CLASS$$::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
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

void $$FRAME_CLASS$$::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void $$FRAME_CLASS$$::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void $$FRAME_CLASS$$::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void $$FRAME_CLASS$$::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void $$FRAME_CLASS$$::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}
$$ENDIF
$$IF(PROJTYPE_MDI)

CBCGPMDIChildWnd* $$FRAME_CLASS$$::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/)
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
$$ENDIF

