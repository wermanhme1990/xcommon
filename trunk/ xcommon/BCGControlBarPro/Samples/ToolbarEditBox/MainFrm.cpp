// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ToolbarEditBox.h"

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
	ON_COMMAND(ID_EDIT_1, OnEdit1)
	ON_COMMAND(ID_EDIT_2, OnEdit2)
	ON_COMMAND(ID_EDIT_3, OnEdit3)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
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

	// enable Windows XP look (in other OS Office XP look will be used):
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
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

	lstBasicCommands.AddTail (ID_EDIT_1);
	lstBasicCommands.AddTail (ID_EDIT_2);
	lstBasicCommands.AddTail (ID_EDIT_3);

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

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

	//----Exmaple----//

	CBCGPToolbarEditBoxButton editButton1 (ID_EDIT_1,
		CImageHash::GetImageOfCommand (ID_EDIT_1, FALSE),
		ES_AUTOHSCROLL, 100);
	pDlgCust->ReplaceButton (ID_EDIT_1, editButton1);

	CBCGPToolbarSpinEditBoxButton editButton2 (ID_EDIT_2,
		CImageHash::GetImageOfCommand (ID_EDIT_2, FALSE),
		ES_AUTOHSCROLL, 100);
	pDlgCust->ReplaceButton (ID_EDIT_2, editButton2);

	CBCGPToolbarEditBoxButton editButton3 (ID_EDIT_3,
		CImageHash::GetImageOfCommand (ID_EDIT_3, FALSE),
		ES_AUTOHSCROLL, 100);
	editButton3.EnableCalculator ();
	pDlgCust->ReplaceButton (ID_EDIT_3, editButton3);

	//----Exmaple----//

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

LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	//----Exmaple----//

	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_MAINFRAME:
		{
			CBCGPToolbarEditBoxButton editButton1 (ID_EDIT_1,
				CImageHash::GetImageOfCommand (ID_EDIT_1, FALSE),
				ES_AUTOHSCROLL, 100);
			m_wndToolBar.ReplaceButton (ID_EDIT_1, editButton1);

			CBCGPToolbarSpinEditBoxButton editButton2 (ID_EDIT_2,
				CImageHash::GetImageOfCommand (ID_EDIT_2, FALSE),
				ES_AUTOHSCROLL, 100);
			m_wndToolBar.ReplaceButton (ID_EDIT_2, editButton2);

			CBCGPToolbarEditBoxButton editButton3 (ID_EDIT_3,
				CImageHash::GetImageOfCommand (ID_EDIT_3, FALSE),
				ES_AUTOHSCROLL, 100);
			editButton3.EnableCalculator ();
			m_wndToolBar.ReplaceButton (ID_EDIT_3, editButton3);
		}

		break;
	}

	//----Exmaple----//

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

//----Exmaple----//

void CMainFrame::OnEdit1() 
{
	OnEdit (ID_EDIT_1);
}

void CMainFrame::OnEdit2() 
{
	OnEdit (ID_EDIT_2);
}

void CMainFrame::OnEdit3() 
{
	OnEdit (ID_EDIT_3);
}

void CMainFrame::OnEdit (UINT uiCmd)
{
	CBCGPToolbarEditBoxButton* pSrcEdit = NULL;

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); 
			pSrcEdit == NULL && pos != NULL;)
		{
			CBCGPToolbarEditBoxButton* pEdit = 
				DYNAMIC_DOWNCAST (CBCGPToolbarEditBoxButton, listButtons.GetNext (pos));

			if (pEdit != NULL && ::GetFocus () == pEdit->GetHwnd ())
			{
				pSrcEdit = pEdit;
			}
		}
	}

	if (pSrcEdit != NULL && pSrcEdit->GetEditBox () != NULL &&
		(pSrcEdit->GetEditBox ()->GetStyle () & WS_VISIBLE))
	{
		CString strName;
		pSrcEdit->GetEditBox ()->GetWindowText (strName);
		CString strMsg = _T("Name is: ");
		strMsg += strName;
		MessageBox (strMsg);
	}
	else
	{
		MessageBox ("Enter name here....");
	}
}

//----Exmaple----//