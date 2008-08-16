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
$$IF(TOOLBAR || STATUSBAR || MENUBAR || REBAR || OUTLOOKBAR || CAPTIONBAR)
	ON_WM_CREATE()
$$ENDIF //TOOLBAR || STATUSBAR
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
$$IF(TOOLBAR_CUSTOMIZATION || TOOLBAR || STATUSBAR || OUTLOOKBAR || CAPTIONBAR)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
$$ENDIF
$$IF(USER_TOOLBAR)
	ON_COMMAND_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
$$ENDIF
$$IF(HELP)
$$IF(TOOLBAR_CUSTOMIZATION)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
$$ENDIF
$$ENDIF
$$IF(WORKSPACEBAR)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_WORKSPACE2, OnViewWorkspace2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE2, OnUpdateViewWorkspace2)
$$ENDIF
$$IF(OUTPUTBAR)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
$$ENDIF
$$IF(OUTLOOKBAR)
	ON_COMMAND(ID_VIEW_OUTLOOKBAR, OnViewOutlookBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLOOKBAR, OnUpdateViewOutlookBar)
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_4, OnOutlookBarShortcut)
$$ENDIF
$$IF(CAPTIONBAR)
	ON_COMMAND(ID_VIEW_CAPTIONBAR, OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTIONBAR, OnUpdateViewCaptionBar)
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
$$IF(VERBOSE)
	// TODO: add member initialization code here
	
$$ENDIF
}

$$FRAME_CLASS$$::~$$FRAME_CLASS$$()
{
}
$$IF(TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR)

int $$FRAME_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ($$FRAME_BASE_CLASS$$::OnCreate(lpCreateStruct) == -1)
		return -1;

$$IF(OFFICE_XP_LOOK)
$$IF(VERBOSE)
	// enable Office XP look:
$$ENDIF
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));

$$ENDIF
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
$$IF(MDI_TABS)
	EnableMDITabs (TRUE, FALSE, CBCGPTabWnd::LOCATION_TOP, TRUE);

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

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
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
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	// Each rebar pane will ocupy its own row:
	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;

	if (!m_wndReBar.Create(this) ||
$$IF(MENUBAR)
		!m_wndReBar.AddBar (&m_wndMenuBar) ||
$$ENDIF // MENUBAR
		!m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, dwStyle) ||
		!m_wndReBar.AddBar (&m_wndDlgBar, NULL, NULL, dwStyle))
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
	m_wndToolBar.EnableCustomizeButton (TRUE, -1, _T(""));

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
$$IF(WORKSPACEBAR)

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
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
$$ENDIF //OUTPUTBAR
$$IF(TOOLBAR)

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);
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
$$IF(MINI_SERVER || FULL_SERVER || CONTAINER_SERVER)
	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
$$ENDIF//MINI_SERVER || FULL_SERVER || CONTAINER_SERVER
$$IF(OUTLOOKBAR)

	if (!CreateShortcutsBar ())
	{
		TRACE0("Failed to create shortcuts bar\n");
		return -1;      // fail to create
	}

$$ENDIF //!OUTLOOKBAR
$$ELSE
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
	EnableAutoHideBars (CBRS_ALIGN_ANY);
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
$$IF(MENUBAR)
	DockControlBar(&m_wndMenuBar);
$$ENDIF	// MENUBAR
	DockControlBar(&m_wndToolBar);
$$IF(WORKSPACEBAR)
	CBCGPTabbedControlBar* pTabbededBar = new CBCGPTabbedControlBar (TRUE);
	if (!pTabbededBar->Create (_T(""), this, CRect (0, 0, 200, 200),
		TRUE, 
		(UINT) -1,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create workspace bars\n");
		return FALSE;      // fail to create
	}

	pTabbededBar->AddTab (&m_wndWorkSpace);
	pTabbededBar->AddTab (&m_wndWorkSpace2);

	pTabbededBar->EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(pTabbededBar);
$$ENDIF	// WORKSPACEBAR
$$IF(OUTPUTBAR)
	DockControlBar(&m_wndOutput);
$$ENDIF	// OUTPUTBAR
$$ENDIF //REBAR
$$ENDIF //TOOLBAR	

$$IF(TOOLBAR_CUSTOMIZATION)
$$IF(USER_TOOLBAR)

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);
$$ENDIF
$$ENDIF
$$IF(WINDOWS_DLG)

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
$$ENDIF
	return 0;
}
$$ENDIF //TOOLBAR || STATUSBAR
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
$$IF(TOOLBAR || STATUSBAR || MENUBAR || REBAR)

LRESULT $$FRAME_CLASS$$::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

$$IF(USER_TOOLBAR)
	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);

$$ENDIF
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}
$$ENDIF
$$IF(USER_TOOLBAR)

void $$FRAME_CLASS$$::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->GetStyle () & WS_VISIBLE), FALSE, TRUE);
	RecalcLayout ();
}

void $$FRAME_CLASS$$::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->GetStyle () & WS_VISIBLE);
}
$$ENDIF
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
$$IF(TOOLBAR)

BOOL $$FRAME_CLASS$$::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
$$IF(VERBOSE)
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
$$ENDIF
    $$FRAME_BASE_CLASS$$::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
$$IF(VERBOSE)
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
$$ENDIF
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

$$IF(USER_TOOLBAR)
		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
$$ENDIF
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	return TRUE;
}
$$ENDIF
$$IF(WORKSPACEBAR)

void CMainFrame::OnViewWorkspace() 
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewWorkspace2() 
{
	ShowControlBar (&m_wndWorkSpace2,
					!(m_wndWorkSpace2.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace2.GetStyle () & WS_VISIBLE);
}
$$ENDIF
$$IF(OUTPUTBAR)

void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.GetStyle () & WS_VISIBLE);
}
$$ENDIF
$$IF(OUTLOOKBAR)

void CMainFrame::OnViewOutlookBar() 
{
	ShowControlBar (&m_wndShortcutsBar,
					!(m_wndShortcutsBar.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutlookBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndShortcutsBar.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnOutlookBarShortcut(UINT id)
{
$$IF(VERBOSE)
	// TODO: process shortcuts bar commands here...
$$ENDIF
}
$$ENDIF
$$IF(CAPTIONBAR)

void CMainFrame::OnViewCaptionBar() 
{
	ShowControlBar (&m_wndCaptionBar,
					!(m_wndCaptionBar.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndCaptionBar.GetStyle () & WS_VISIBLE);
}
$$ENDIF
$$IF(OUTLOOKBAR)

BOOL CMainFrame::CreateShortcutsBar ()
{
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

$$IF(OUTLOOK_CAPTION)
	// Error: Outlook bar caption is not implemented yet
$$ENDIF	// OUTLOOK_CAPTION

	CImageList images;
	images.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 255));

	// Create first page:
	m_wndShortcutsPane1.Create (&m_wndShortcutsBar, 
		dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_1);

	m_wndShortcutsPane1.SetOwner (this);
	m_wndShortcutsPane1.EnableTextLabels ();

	m_wndShortcutsPane1.AddButton (images.ExtractIcon (0), _T("Shortcut 1"), ID_SHORTCUT_1);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (1), _T("Shortcut 2"), ID_SHORTCUT_2);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane1, _T("Page 1"));

	// Create second page:
	m_wndShortcutsPane2.Create (&m_wndShortcutsBar, 
		dwDefaultToolbarStyle, ID_SHORTCUTS_PANE_2);

	m_wndShortcutsPane2.SetOwner (this);
	m_wndShortcutsPane2.EnableTextLabels ();

	m_wndShortcutsPane2.AddButton (images.ExtractIcon (2), _T("Shortcut 3"), ID_SHORTCUT_3);
	m_wndShortcutsPane2.AddButton (images.ExtractIcon (3), _T("Shortcut 4"), ID_SHORTCUT_4);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsPane2, _T("Page 2"));
$$IF(OUTLOOK_EMBEDDED_CONTROL)

	const DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndShortcutsTreeWnd.Create (dwTreeStyle, CRect (0, 0, 0, 0), 
		&m_wndShortcutsBar, ID_SHORTCUTS_TREE);

	HTREEITEM hRoot = m_wndShortcutsTreeWnd.InsertItem (_T("Folders"));
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 1"), hRoot);
	m_wndShortcutsTreeWnd.InsertItem (_T("Folder 2"), hRoot);
	m_wndShortcutsTreeWnd.Expand (hRoot, TVE_EXPAND);

	pShortcutsBarContainer->AddTab (&m_wndShortcutsTreeWnd, _T ("Page 3"));
$$ENDIF	// OUTLOOK_EMBEDDED_CONTROL

//	m_wndShortcutsBar.SetDefaultState ();
	return TRUE;
}
$$ENDIF
$$IF(WINDOWS_DLG)

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}
$$ENDIF
