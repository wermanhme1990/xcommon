// SidebarWnd.cpp : implementation file
//

#include "stdafx.h"
#include "sidebardemo.h"
#include "SidebarWnd.h"
#include "SideBarDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_TRAYNOTIFY	(WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CSidebarWnd

CSidebarWnd::CSidebarWnd()
{
	m_abs.m_auDimsDock[0] = 150;
	m_abs.m_auDimsDock[1] = 150;
	m_abs.m_auDimsDock[2] = 150;
	m_abs.m_auDimsDock[3] = 150;
	m_abs.m_rcFloat = CRect (0, 0, 200, 400);

	SetFlags (ABF_ALLOWLEFTRIGHT | ABF_ALLOWFLOAT | ABF_ALLOWAUTOHIDE | ABF_ALLOWALWAYSONTOP);
}

CSidebarWnd::~CSidebarWnd()
{
	m_nid.hIcon = NULL;
	Shell_NotifyIcon (NIM_DELETE, &m_nid);
}


BEGIN_MESSAGE_MAP(CSidebarWnd, CBCGPAppBarWnd)
	//{{AFX_MSG_MAP(CSidebarWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_APP_OPTIONS, OnAppOptions)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSidebarWnd message handlers

int CSidebarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPAppBarWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create task pane embedded control:
	CRect rectDummy(0, 0, 200, 400);
	if (!m_wndChildBar.Create(_T("Sidebar"), this, rectDummy, TRUE, 1,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_BCGP_CLOSE /*| CBRS_BCGP_FLOAT*/))
	{
		TRACE0("Failed to create embedded task pane\n");
		return FALSE;      // fail to create
	}
	m_wndChildBar.EnableGripper (FALSE);

	m_hEmbeddedBar = m_wndChildBar.GetSafeHwnd ();

	// Initialize NOTIFYICONDATA
	memset(&m_nid, 0 , sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set tray notification window:
	m_nid.hWnd = GetSafeHwnd ();
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;

	// Set tray icon and tooltip:
	m_nid.hIcon = m_hIcon;

	CString strToolTip = _T("BCGSideBar Demo");
	strncpy (m_nid.szTip, strToolTip, strToolTip.GetLength ());

	Shell_NotifyIcon (NIM_ADD, &m_nid);
	return 0;
}

void CSidebarWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPAppBarWnd::OnSize(nType, cx, cy);
	m_wndChildBar.SetWindowPos (NULL, 0, 0, cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndChildBar.RecalcLayout ();
}

void CSidebarWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPAppBarWnd::OnSetFocus(pOldWnd);
	m_wndChildBar.SetFocus ();
}

LRESULT CSidebarWnd::OnTrayNotify(WPARAM wp, LPARAM lp)
{
	UINT uiMsg = (UINT) lp;

	switch (uiMsg)
	{
	case WM_RBUTTONUP:
		OnTrayContextMenu ();
		return 1;

	case WM_LBUTTONDBLCLK:
		OnAppOptions ();
		return 1;
	}

	return 0;
}

void CSidebarWnd::OnTrayContextMenu ()
{
	CPoint point;
	::GetCursorPos (&point);

	CMenu menu;
	menu.LoadMenu (IDR_CONTEXT_MENU);

	CBCGPPopupMenu::SetForceShadow (TRUE);

	HMENU hMenu = menu.GetSubMenu (0)->Detach ();
	CBCGPPopupMenu* pMenu = GetWorkspace ()->GetContextMenuManager()->ShowPopupMenu (
		hMenu, point.x, point.y, this, TRUE);

	pMenu->SetForegroundWindow ();
}


void CSidebarWnd::OnAppExit() 
{
	PostMessage (WM_CLOSE);
}

void CSidebarWnd::OnAppOptions() 
{
	CSideBarDemoDlg* pActiveDlg = CSideBarDemoDlg::GetActiveDlg ();
	if (pActiveDlg != NULL)
	{
		pActiveDlg->SetFocus ();
		return;
	}

	CSideBarDemoDlg dlg (theApp.m_pwndAppBar);
	dlg.DoModal();
}

void CSidebarWnd::OnChangeLook ()
{
	m_wndChildBar.RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}
