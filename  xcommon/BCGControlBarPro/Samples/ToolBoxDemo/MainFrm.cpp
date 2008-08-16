// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ToolBox.h"
#include "ToolBoxDemo.h"

#include "MainFrm.h"
#include "TBListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007_4, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_2007_4, OnUpdateAppLook)
	ON_COMMAND(ID_TOOLBOX, OnToolBox)
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
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_VS2005);
	m_bIsToolBoxEx = TRUE;
	m_nPageLayout = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnAppLook (m_nAppLook);

	CBCGPToolBar::EnableQuickCustomization ();

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

	CBCGPToolBar::AddToolBarForImageCollection (IDR_IMAGES, IDB_IMAGES24);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetWindowText (_T("Standard"));

	if (!CreateToolBox ())
	{
		TRACE0("Failed to create toolbox\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_pWndToolBoxBar->EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking (CBRS_ALIGN_ANY);

	DockControlBar (&m_wndMenuBar);
	DockControlBar (&m_wndToolBar);
	DockControlBar (m_pWndToolBoxBar);

	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));

	// Enable conttol bar context menu (list of bars + customize command):
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
							// toolbars menu

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
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
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
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

BOOL CMainFrame::CreateToolBox ()
{
	CToolBoxBarEx* pToolBox = new CToolBoxBarEx;

	if (!pToolBox->Create (_T("ToolBox"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_TOOLBOX,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		0, CBRS_BCGP_FLOAT | CBRS_BCGP_RESIZE))
	{
		TRACE0("Failed to create ToolBox Bar\n");
		return FALSE;		// fail to create
	}
	
	m_pWndToolBoxBar = pToolBox;
	return TRUE;
}

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS.NET 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_1:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_2:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_3:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_4:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}

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

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}

CBCGPToolBoxPage* CMainFrame::GetToolBoxPage (int nPage)
{
	if (m_bIsToolBoxEx)
	{
		CToolBoxBarEx* pToolBox = (CToolBoxBarEx*) m_pWndToolBoxBar;
		ASSERT_VALID (pToolBox);

		return pToolBox->GetPage (nPage);
	}

	CToolBoxBar* pToolBox = (CToolBoxBar*) m_pWndToolBoxBar;
	ASSERT_VALID (pToolBox);

	return pToolBox->GetPage (nPage);
}

void CMainFrame::SetToolBoxLayout (int nLayout)
{
	m_nPageLayout = nLayout;

	if (m_bIsToolBoxEx)
	{
		CToolBoxBarEx* pToolBox = (CToolBoxBarEx*) m_pWndToolBoxBar;
		ASSERT_VALID (pToolBox);

		for (int nPage = 0; nPage < pToolBox->GetPageCount (); nPage++)
		{
			pToolBox->GetPage (nPage)->SetMode ((CBCGPToolBoxPage::ToolBoxPageMode) nLayout);
		}

		pToolBox->AdjustLayout ();
		return;
	}

	CToolBoxBar* pToolBox = (CToolBoxBar*) m_pWndToolBoxBar;
	ASSERT_VALID (pToolBox);

	for (int nPage = 0; nPage < pToolBox->GetTabWnd ()->GetTabsNum (); nPage++)
	{
		pToolBox->GetPage (nPage)->SetMode ((CBCGPToolBoxPage::ToolBoxPageMode) nLayout);
	}

	pToolBox->AdjustLayout ();
}

void CMainFrame::SetToolBoxMode (BOOL bIsExt)
{
	m_bIsToolBoxEx = bIsExt;

	CBCGPDockManager* pDockManager = GetDockManager ();
	ASSERT_VALID (pDockManager);

	CBCGPDockingControlBar*	pWndNewToolBoxBar = NULL;
	const DWORD dwToolBoxStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI;

	if (m_bIsToolBoxEx)
	{
		CToolBoxBarEx* pToolBox = new CToolBoxBarEx;

		if (!pToolBox->Create (_T("ToolBox"), this, CRect (0, 0, 0, 0), TRUE, ID_TOOLBOX, 
			dwToolBoxStyle, 0,
			CBRS_BCGP_FLOAT | CBRS_BCGP_RESIZE))
		{
			TRACE0("Failed to create ToolBox Bar\n");
		}

		pWndNewToolBoxBar = pToolBox;

		pToolBox->AddToolsPage (_T("Windows Forms"), IDB_TOOLS_PAGE1_HC, 16, 
			_T("Pointer\nLabel\nButton\nTextBox\nMainMenu\nCheckBox\nRadioButton\nGroupBox\nPictureBox\nPanel\nDataGrid\nListBox\nCheckedListBox\nComboBox\nListView\nTreeView\nTabControl\nDateTimePicker\nMonthCalendar\nHScrollBar\nVScrollBar\nTimer\nSplitter\nDomainUpDown\nNumericUpDown\nTrackBar\nProgressBar\nRichTextBox"));
		pToolBox->AddToolsPage (_T("Components"),	IDB_TOOLS_PAGE2_HC, 16, _T("Pointer\nDataSet\nOleDbDataAdapter\nOleDbConnection"));
		pToolBox->AddToolsPage (_T("Data"),	IDB_TOOLS_PAGE3_HC, 16, _T("Pointer\nFileSystemWatch\nEventLog\nDyrectoryEntry"));

		pToolBox->SetActivePage (0);
	}
	else
	{
		CToolBoxBar* pToolBox = new CToolBoxBar;

		if (!pToolBox->Create (_T("ToolBox"), this, CRect (0, 0, 0, 0), TRUE, ID_TOOLBOX, 
			dwToolBoxStyle, 0, 
			CBRS_BCGP_FLOAT | CBRS_BCGP_RESIZE))
		{
			TRACE0("Failed to create ToolBox Bar\n");
		}

		pWndNewToolBoxBar = pToolBox;

		pToolBox->AddToolsPage (_T("Windows Forms"), IDB_TOOLS_PAGE1_HC, 16, 
			_T("Pointer\nLabel\nButton\nTextBox\nMainMenu\nCheckBox\nRadioButton\nGroupBox\nPictureBox\nPanel\nDataGrid\nListBox\nCheckedListBox\nComboBox\nListView\nTreeView\nTabControl\nDateTimePicker\nMonthCalendar\nHScrollBar\nVScrollBar\nTimer\nSplitter\nDomainUpDown\nNumericUpDown\nTrackBar\nProgressBar\nRichTextBox"));
		pToolBox->AddToolsPage (_T("Components"),	IDB_TOOLS_PAGE2_HC, 16, _T("Pointer\nDataSet\nOleDbDataAdapter\nOleDbConnection"));
		pToolBox->AddToolsPage (_T("Data"),	IDB_TOOLS_PAGE3_HC, 16, _T("Pointer\nFileSystemWatch\nEventLog\nDyrectoryEntry"));

		pToolBox->GetTabWnd ()->SetActiveTab (0);
	}

	pDockManager->ReplaceControlBar (m_pWndToolBoxBar, pWndNewToolBoxBar);
	m_pWndToolBoxBar->DestroyWindow ();
	delete m_pWndToolBoxBar;
	m_pWndToolBoxBar = pWndNewToolBoxBar;

	SetToolBoxLayout (m_nPageLayout);
}

void CMainFrame::OnDestroy() 
{
	if (m_pWndToolBoxBar != NULL)
	{
		m_pWndToolBoxBar->DestroyWindow ();
		delete m_pWndToolBoxBar;
	}
	
	CBCGPFrameWnd::OnDestroy();
}

void CMainFrame::OnToolBox() 
{
	int nPage = -1;
	int nTool = -1;

	CBCGPToolBoxButton* pTool = NULL;

	if (m_bIsToolBoxEx)
	{
		nTool = ((CToolBoxBarEx*) m_pWndToolBoxBar)->GetLastClickedTool (nPage);

		pTool = 
			((CToolBoxBarEx*)(m_pWndToolBoxBar))->GetPage (nPage)->GetButton (nTool);
	}
	else
	{
		nPage = ((CToolBoxBar*) m_pWndToolBoxBar)->GetActivePage ();
		nTool = ((CToolBoxBar*) m_pWndToolBoxBar)->GetLastClickedTool (nPage);

		pTool = 
			((CToolBoxBar*)(m_pWndToolBoxBar))->GetPage (nPage)->GetButton (nTool);
	}
		
	const MSG* pMsg = CWnd::GetCurrentMessage();

	if (pMsg->lParam != (LPARAM) m_pWndToolBoxBar->GetSafeHwnd ())
	{
		m_pWndToolBoxBar->ShowControlBar (!m_pWndToolBoxBar->IsVisible (), FALSE, TRUE);
		return;
	}

	if (pTool == NULL)
	{
		return;
	}

	ASSERT_VALID (pTool);

	BOOL bDrag = FALSE;

	if (nPage != -1 && nTool != -1)
	{
		COleDataSource dataSource;

		try
		{
			CSharedFile globFile;

			bDrag = CTBData::SerializeTo (globFile, pTool, nPage, nTool);

			if (bDrag)
			{
				dataSource.CacheGlobalData (CTBData::GetClipboardFormat (), 
					globFile.Detach());
			}
		}
		catch (COleException* pEx)
		{
			TRACE(_T("OnToolBox. OLE exception: %x\r\n"),
				pEx->m_sc);
			pEx->Delete ();

			bDrag = FALSE;
		}
		catch (CNotSupportedException *pEx)
		{
			TRACE(_T("OnToolBox. \"Not Supported\" exception\r\n"));
			pEx->Delete ();

			bDrag = FALSE;
		}

		if (bDrag)
		{
			bDrag = dataSource.DoDragDrop (DROPEFFECT_COPY | DROPEFFECT_SCROLL) != DROPEFFECT_NONE;
		}
	}

	if (!bDrag)
	{
		CString str;
		str.Format (_T("Toolbox item clicked\nPage = %d, item = %d"), nPage, nTool);
		MessageBox (str);
	}
}
