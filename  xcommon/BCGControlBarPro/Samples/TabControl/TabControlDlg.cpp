// TabControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TabControl.h"
#include "TabControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabControlDlg dialog

CTabControlDlg::CTabControlDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTabControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabControlDlg)
	m_bAutoColors = TRUE;
	m_bDefaultColors = TRUE;
	m_nTab = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabControlDlg)
	DDX_Control(pDX, IDC_COLOR, m_wndColor);
	DDX_Control(pDX, IDC_DEFAULT_COLORS, m_wndDefaultColors);
	DDX_Control(pDX, IDC_TABS_AREA, m_wndTabsArea);
	DDX_Check(pDX, IDC_ENABLE_AUTO_COLOR, m_bAutoColors);
	DDX_Check(pDX, IDC_DEFAULT_COLORS, m_bDefaultColors);
	DDX_Text(pDX, IDC_TAB, m_nTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabControlDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CTabControlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DEFAULT_COLORS, OnDefaultColors)
	ON_BN_CLICKED(IDC_ENABLE_AUTO_COLOR, OnEnableAutoColor)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabControlDlg message handlers

BOOL CTabControlDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));

	CRect rectTabs;

	m_wndTabsArea.GetWindowRect (rectTabs);
	ScreenToClient (rectTabs);

	m_wndTabs.Create (CBCGPTabWnd::STYLE_3D_ONENOTE, rectTabs, this, 1, 
		CBCGPTabWnd::LOCATION_TOP);
	m_wndTabs.EnableTabSwap (FALSE);

	m_wnd1.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 1);
	m_wnd2.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 2);
	m_wnd3.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 3);
	m_wnd4.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 4);
	m_wnd5.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 5);
	m_wnd6.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_wndTabs, 6);

	m_wndTabs.AddTab (&m_wnd1, "One", (UINT)-1, FALSE);
	m_wndTabs.AddTab (&m_wnd2, "Two", (UINT)-1, FALSE);
	m_wndTabs.AddTab (&m_wnd3, "Three", (UINT)-1, FALSE);
	m_wndTabs.AddTab (&m_wnd4, "Four", (UINT)-1, FALSE);
	m_wndTabs.AddTab (&m_wnd5, "Five", (UINT)-1, FALSE);
	m_wndTabs.AddTab (&m_wnd6, "Six", (UINT)-1, FALSE);

	m_wndColor.EnableOtherButton ("Other");

	SetTabColors ();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTabControlDlg::SetTabColors ()
{
	m_wndTabs.EnableAutoColor (m_bAutoColors);

	CArray<COLORREF, COLORREF> arColors;

	if (!m_bDefaultColors)
	{
		arColors.Add (RGB (234, 142, 117));
		arColors.Add (RGB (190, 218, 153));
		arColors.Add (RGB (102, 200, 238));
	}

	m_wndTabs.SetAutoColors (arColors);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTabControlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTabControlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTabControlDlg::OnDefaultColors() 
{
	UpdateData ();
	SetTabColors ();
}

void CTabControlDlg::OnEnableAutoColor() 
{
	UpdateData ();
	m_wndDefaultColors.EnableWindow (m_bAutoColors);

	SetTabColors ();
}

void CTabControlDlg::OnColor() 
{
	UpdateData ();

	m_wndTabs.SetTabBkColor (m_nTab, m_wndColor.GetColor ());
	m_wndTabs.RedrawWindow ();
}
