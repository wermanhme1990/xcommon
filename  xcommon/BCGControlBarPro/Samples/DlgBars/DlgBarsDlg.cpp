// DlgBarsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBars.h"
#include "DlgBarsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_SHORTCUT_1	101
#define ID_SHORTCUT_2	102
#define ID_SHORTCUT_3	103
#define ID_SHORTCUT_4	104

/////////////////////////////////////////////////////////////////////////////
// CDlgBarsDlg dialog

CDlgBarsDlg::CDlgBarsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBarsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBarsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMenuFrame = NULL;
}

void CDlgBarsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarsDlg)
	DDX_Control(pDX, IDC_MENU_LOCATION, m_wndMenuBarLocation);
	DDX_Control(pDX, IDC_STATUSBAR_LOCATION, m_wndStatusBarLocation);
	DDX_Control(pDX, IDC_TOOLBAR_LOCATION, m_wndToolbarLocation);
	DDX_Control(pDX, IDC_OUTLOOK_LOCATION, m_wndOutlookBarLocation);
	DDX_Control(pDX, IDC_CAPTION_LOCATION, m_wndCaptionLocation);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgBarsDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgBarsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_4, OnShortcut)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CDlgBarsDlg message handlers

BOOL CDlgBarsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// Create Outlook Bar:
	DWORD dwStyle = WS_CAPTION | WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT;
	DWORD dwBCGStyle = 0;
	m_wndOutlookBar.Create (_T("Shortcuts"), this, CRect (0, 0, 100, 100), 
		AFX_IDW_TOOLBAR, dwStyle, dwBCGStyle);

	m_wndOutlookBar.EnableGripper (TRUE);

    m_wndOutlookBar.SetBarStyle (CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndOutlookBar.EnableSetCaptionTextToTabName (FALSE);

	m_wndPane1.Create (&m_wndOutlookBar, dwDefaultToolbarStyle, 1);
	m_wndPane1.SetWindowText (_T("Page 1"));
	m_wndOutlookBar.AddTab (&m_wndPane1);

	m_wndPane1.EnableTextLabels (TRUE);
	m_wndPane1.SetOwner (this);
	

	m_wndPane2.Create (&m_wndOutlookBar, dwDefaultToolbarStyle, 1);
	m_wndPane2.SetWindowText (_T("Page 2"));
	m_wndOutlookBar.AddTab (&m_wndPane2);

	m_wndPane2.EnableTextLabels (TRUE);
	m_wndPane2.SetOwner (this);

	// Add some shortcuts:
	m_wndPane1.AddButton (IDB_SHORTCUT1, "Shortcut 1", ID_SHORTCUT_1);
	m_wndPane1.AddButton (IDB_SHORTCUT2, "Shortcut 2", ID_SHORTCUT_2);


	m_wndPane2.AddButton (IDB_SHORTCUT3, "Shortcut 3", ID_SHORTCUT_3);
	m_wndPane2.AddButton (IDB_SHORTCUT4, "Shortcut 4", ID_SHORTCUT_4);

	CRect rectClient;
	GetClientRect (rectClient);

	// Set OutlookBar position and size:
	CRect rectOutlookBar;
	m_wndOutlookBarLocation.GetWindowRect (&rectOutlookBar);
	ScreenToClient (&rectOutlookBar);

	m_wndOutlookBar.SetWindowPos (&wndTop, rectOutlookBar.left, rectOutlookBar.top, 
						rectOutlookBar.Width (), 
						rectClient.Height () - 2 * rectOutlookBar.top,
						SWP_NOACTIVATE);

	// Create menu bar:
	m_wndMenuBar.Create (this);

	CMenu menu;
	menu.LoadMenu (IDR_MAINFRAME);
	m_wndMenuBar.CreateFromMenu (menu.GetSafeHmenu (), TRUE, TRUE);

	m_wndMenuBar.SetBarStyle (
		m_wndMenuBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	// Set menu bar position and size:
	CRect rectMenuBar;
	m_wndMenuBarLocation.GetWindowRect (&rectMenuBar);
	ScreenToClient (&rectMenuBar);

	m_wndMenuBar.SetWindowPos (&wndTop, rectMenuBar.left, rectMenuBar.top, 
						rectMenuBar.Width (), 
						rectMenuBar.Height (),
						SWP_NOACTIVATE);

	m_pMenuFrame = new CCmdFrame (this);
	m_pMenuFrame->Create (NULL, _T(""));
	m_pMenuFrame->ShowWindow (SW_HIDE);

	m_wndMenuBar.SetOwner (m_pMenuFrame);
	BCGCBProSetTopLevelFrame (m_pMenuFrame);

	// Create caption bar:
	m_wndCaptionBar.Create (WS_CHILD | WS_VISIBLE, this, (UINT)-1);
	m_wndCaptionBar.SetText (_T("Caption"), CBCGPCaptionBar::ALIGN_LEFT);
	m_wndCaptionBar.SetFlatBorder ();

	// Load caption image:
	m_bmpCaption.LoadBitmap (IDB_CAPTION);
	m_wndCaptionBar.SetBitmap ((HBITMAP) m_bmpCaption.GetSafeHandle (), RGB (255, 0, 255));

	m_wndCaptionBar.SetBarStyle (
		m_wndCaptionBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	// Set caption bar position and size:
	CRect rectCaptionBar;
	m_wndCaptionLocation.GetWindowRect (&rectCaptionBar);
	ScreenToClient (&rectCaptionBar);

	m_wndCaptionBar.SetWindowPos (&wndTop, rectCaptionBar.left, rectCaptionBar.top, 
						rectCaptionBar.Width (), 
						rectCaptionBar.Height (),
						SWP_NOACTIVATE);
	// Create toolbar:
	m_wndToolBar.Create (this);
	m_wndToolBar.LoadToolBar (IDR_MAINFRAME, 0, 0, TRUE /* Locked bar */);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	CSize sizeToolBar = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);

	// Set ToolBar position and size:
	CRect rectToolBar;
	m_wndToolbarLocation.GetWindowRect (&rectToolBar);
	ScreenToClient (&rectToolBar);

	m_wndToolBar.SetWindowPos (&wndTop, rectToolBar.left, rectToolBar.top, 
		sizeToolBar.cx, sizeToolBar.cy, SWP_NOACTIVATE);

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this dialog, not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);
	
	// Create status bar:
	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT));

	// Set status bar position and size:
	CRect rectStatusBar;
	m_wndStatusBarLocation.GetWindowRect (&rectStatusBar);
	ScreenToClient (&rectStatusBar);

	m_wndStatusBar.SetWindowPos (&wndTop, rectStatusBar.left, rectStatusBar.top, 
		rectStatusBar.Width (), rectStatusBar.Height (), SWP_NOACTIVATE);

	m_wndStatusBar.SetWindowText ("Test");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgBarsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgBarsDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgBarsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlgBarsDlg::OnShortcut(UINT uiCmdID)
{
	CString str;
	str.Format (_T("Shortut %d ..."), uiCmdID - ID_SHORTCUT_1 + 1);

	MessageBox (str);
}

void CDlgBarsDlg::OnEditCopy() 
{
	MessageBox ("OnEditCopy");
}

void CDlgBarsDlg::OnEditCut() 
{
	MessageBox ("OnEditCut");
}

void CDlgBarsDlg::OnEditPaste() 
{
	MessageBox ("OnEditPaste");
}

void CDlgBarsDlg::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck ();
}

void CDlgBarsDlg::OnFileNew() 
{
	MessageBox ("OnFileNew");
}

void CDlgBarsDlg::OnFileOpen() 
{
	MessageBox ("OnFileOpen");
}

void CDlgBarsDlg::OnFilePrint() 
{
	MessageBox ("OnFilePrint");
}

void CDlgBarsDlg::OnFileSave() 
{
	MessageBox ("OnFileSave");
}

void CDlgBarsDlg::OnAppAbout() 
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}

LRESULT CDlgBarsDlg::OnKickIdle(WPARAM, LPARAM)
{
	m_wndToolBar.OnUpdateCmdUI ((CFrameWnd*) this, TRUE);
    return 0;
}

void CDlgBarsDlg::OnDestroy() 
{
	m_pMenuFrame->SendMessage (WM_CLOSE);
	CDialog::OnDestroy();
}
