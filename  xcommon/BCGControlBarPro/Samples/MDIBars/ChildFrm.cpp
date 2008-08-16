// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MDIBars.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDW_DIALOGBAR				153
#define IDW_RESIZABLE_DIALOGBAR		154

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CHILDBAR, OnViewChildbar)
	ON_COMMAND(IDC_BTN_TEST, OnBtnTest)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHILDBAR, OnUpdateViewChildbar)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create (this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|
		CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D, ID_VIEW_CHILDBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strMDIToolbarTitle;
	strMDIToolbarTitle.LoadString(IDS_MDI_TOOLBAR);
	m_wndToolBar.SetWindowText(strMDIToolbarTitle);

	m_wndToolBar.EnableDocking (CBRS_ALIGN_ANY);
	EnableDocking (CBRS_ALIGN_ANY);
	EnableAutoHideBars (CBRS_ALIGN_RIGHT);
	DockControlBar (&m_wndToolBar, AFX_IDW_DOCKBAR_BOTTOM);

	//
	// Dialog bar creation:
	//
	if (!m_wndDialogBar.Create ("Dialog Bar", 
								this, 
								TRUE, 
								MAKEINTRESOURCE (IDD_DIALOGBAR), 
								WS_VISIBLE | WS_CHILD | CBRS_LEFT, 
								IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

	if (!m_wndResizableDlgBar.Create ("Resizable Dialog Bar", 
								this, 
								TRUE, 
								MAKEINTRESOURCE (IDD_RESIZABLE_DIALOGBAR), 
								WS_VISIBLE | WS_CHILD | CBRS_RIGHT, 
								IDW_RESIZABLE_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}
	

	m_wndDialogBar.EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndDialogBar);

	m_wndResizableDlgBar.EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndResizableDlgBar);

	return 0;
}

void CChildFrame::OnViewChildbar() 
{
	ShowControlBar (&m_wndToolBar,
					!(m_wndToolBar.GetStyle () & WS_VISIBLE),
					FALSE, TRUE);
}

void CChildFrame::OnUpdateViewChildbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndToolBar.GetStyle () & WS_VISIBLE);
}


BOOL CChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CBCGPMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	return TRUE;
}

void CChildFrame::OnClose() 
{
	CBCGPMDIChildWnd::OnClose();
}

LRESULT CChildFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_MDI_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

void CChildFrame::OnBtnTest ()
{
	MessageBox ("Test Clicked!");
}