// SideBarDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SideBarDemo.h"
#include "SideBarDemoDlg.h"
#include "SidebarWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSideBarDemoDlg* CSideBarDemoDlg::m_pActiveDlg = NULL;


/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoDlg dialog

CSideBarDemoDlg::CSideBarDemoDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CSideBarDemoDlg::IDD, pParent)
{
	ASSERT_VALID (theApp.m_pwndAppBar);

	//{{AFX_DATA_INIT(CSideBarDemoDlg)
	m_nLocation = -1;
	m_nLook = theApp.GetLook ();
	m_bIsAlwaysOnTop = theApp.m_pwndAppBar->GetAlwaysOnTop ();
	m_bIsAutoHide = theApp.m_pwndAppBar->GetAutoHide ();
	m_bShowOnStartup = theApp.m_bShowOptionsOnStartup;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	switch (theApp.m_pwndAppBar->GetSide ())
	{
	case ABE_LEFT:
		m_nLocation = 0;
		break;

	case ABE_RIGHT:
		m_nLocation = 1;
		break;

	case ABE_FLOAT:
		m_nLocation = 2;
		break;
	}
}

void CSideBarDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSideBarDemoDlg)
	DDX_Radio(pDX, IDC_LOCATION1, m_nLocation);
	DDX_Radio(pDX, IDC_LOOK1, m_nLook);
	DDX_Check(pDX, IDC_ALWAYS_ON_TOP, m_bIsAlwaysOnTop);
	DDX_Check(pDX, IDC_AUTO_HIDE, m_bIsAutoHide);
	DDX_Check(pDX, IDC_SHOW_ON_STARTUP, m_bShowOnStartup);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSideBarDemoDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CSideBarDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoDlg message handlers

BOOL CSideBarDemoDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_pActiveDlg = this;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSideBarDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSideBarDemoDlg::OnPaint() 
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
HCURSOR CSideBarDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSideBarDemoDlg::OnApply() 
{
	UpdateData ();

	switch (m_nLocation)
	{
	case 0:
		theApp.m_pwndAppBar->SetSide (ABE_LEFT);
		m_nLocation = 0;
		break;

	case 1:
		theApp.m_pwndAppBar->SetSide (ABE_RIGHT);
		break;

	case 2:
		theApp.m_pwndAppBar->Float ();
		break;
	}
	
	theApp.SetLook (m_nLook);

	theApp.m_pwndAppBar->SetAutoHide (m_bIsAutoHide);
	theApp.m_pwndAppBar->SetAlwaysOnTop (m_bIsAlwaysOnTop);

	theApp.m_bShowOptionsOnStartup = m_bShowOnStartup;
}

void CSideBarDemoDlg::OnOK() 
{
	OnApply();
	CBCGPDialog::OnOK();
}

BOOL CSideBarDemoDlg::DestroyWindow() 
{
	m_pActiveDlg = NULL;
	return CBCGPDialog::DestroyWindow();
}
