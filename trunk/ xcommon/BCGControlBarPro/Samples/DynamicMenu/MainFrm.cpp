// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DynamicMenu.h"

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
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_DYNAMIC_ITEM_1, OnDynamicItem1)
	ON_COMMAND(ID_DYNAMIC_ITEM_2, OnDynamicItem2)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_REGISTERED_MESSAGE(BCGM_RESETMENU, OnResetMenu)
	ON_COMMAND(IDS_EDIT_MYITEM_1, OnEditMyItem1)
	ON_COMMAND(IDS_EDIT_MYITEM_2, OnEditMyItem2)
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

	CBCGPToolBar::EnableQuickCustomization ();

	// enable Office XP look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	//------------- Example --------------------//
	// do not allow modification of the dummy item in customize mode
	CList<UINT, UINT> lstProtectedCmds;
	lstProtectedCmds.AddTail (ID_DYNAMIC_COMMANDS);
	CBCGPToolbarButton::SetProtectedCommands (lstProtectedCmds);
	//-----------------------------------------//

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
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

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);

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
		TRUE /* Automatic menus scaning */);

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

	if (pMenuPopup == NULL)
	{
		return TRUE;
	}

    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
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

	//------------- Example --------------------//

	// we need to find {Dynamic Command} menu item (it's a dummy item)
	// that should be replaced to our dynamic menu items
	// in customize mode we should leave the menu as is
	int iIndex = -1;
	if (!CBCGPToolBar::IsCustomizeMode () &&
		(iIndex = pMenuPopup->GetMenuBar ()->CommandToIndex (ID_DYNAMIC_COMMANDS)) >= 0)
	{
		// remove the {Dynamic Command} item in noncustomize mode
		pMenuPopup->RemoveItem (iIndex);

		pMenuPopup->InsertSeparator (iIndex); // insert the separator at the end
		// IDS_EDIT_MYITEM_1 and IDS_EDIT_MYITEM_1 should be defined in the string table
		// for status text and tooltip
		pMenuPopup->InsertItem (
			CBCGPToolbarMenuButton (IDS_EDIT_MYITEM_1, NULL, -1, _T("&MyItem 1")), iIndex + 1);
		pMenuPopup->InsertItem (
			CBCGPToolbarMenuButton (IDS_EDIT_MYITEM_2, NULL, -1, _T("MyItem &2")), iIndex + 2);

		// don't forget to add message handlers (ON_COMMAND) to the message map
	}
	
	//-------------------------------------------//
	
	return TRUE;
}

//------------- Example --------------------//
void CMainFrame::OnEditMyItem1 ()
{
	AfxMessageBox (_T ("MyItem 1 is selected"));
}

void CMainFrame::OnEditMyItem2 ()
{
	AfxMessageBox (_T ("MyItem 2 is selected"));
}

void CMainFrame::OnDynamicItem1() 
{
	AfxMessageBox (_T ("DynamicItem 1 is selected"));
}

void CMainFrame::OnDynamicItem2() 
{
	AfxMessageBox (_T ("DynamicItem 2 is selected"));
}

BOOL CMainFrame::OnDrawMenuImage (	CDC* pDC, 
									const CBCGPToolbarMenuButton* pMenuButton, 
									const CRect& rectImage)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenuButton);

	if (pMenuButton->m_nID == ID_DYNAMIC_ITEM_2)
	{
		CBrush br (RGB (0, 0, 255));

		CRect rect = rectImage;
		rect.DeflateRect (2, 2);

		pDC->FillRect (rect, &br);
		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::OnResetMenu(WPARAM,LPARAM)
{
	m_wndMenuBar.AddSubMenu ();
	return 0;
}

//-------------------------------------------//