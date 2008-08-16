// PopupDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PopupDemo.h"
#include "PopupDemoDlg.h"
#include "BCGPPopupWindow.h"
#include "MyPopupDlg.h"
#include "MSNDlg.h"
#include "MSNVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupDemoDlg dialog

CPopupDemoDlg::CPopupDemoDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CPopupDemoDlg::IDD, pParent),
	m_wndIcons (m_IconsSmall)
{
	//{{AFX_DATA_INIT(CPopupDemoDlg)
	m_nVisualMngr = 2;
	m_nAnimation = 3;
	m_nAnimationSpeed = 30;
	m_strAnimSpeed = _T("");
	m_nTransparency = 200;
	m_strTransparency = _T("");
	m_bSmallCaption = TRUE;
	m_nPopupSource = 0;
	m_nIcon = 2;
	m_strLink = _T("Click here to open");
	m_strText = _T("Type your message text here");
	m_nAutoCloseTime = 0;
	m_strAutoClose = _T("");
	m_bAutoClose = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ptPopup = CPoint (-1, -1);
}

void CPopupDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDemoDlg)
	DDX_Control(pDX, IDC_AUTOCLOSE_TIME, m_wndAutoCloseTime);
	DDX_Control(pDX, IDC_TEXT, m_wndText);
	DDX_Control(pDX, IDC_LINK, m_wndLink);
	DDX_Control(pDX, IDC_URL_LABEL, m_wndLabel3);
	DDX_Control(pDX, IDC_TEXT_LABEL, m_wndLabel2);
	DDX_Control(pDX, IDC_ICON_LABEL, m_wndLabel1);
	DDX_Control(pDX, IDC_ICONS, m_wndIcons);
	DDX_Control(pDX, IDC_TRANSP, m_wndTransparency);
	DDX_Control(pDX, IDC_ANIM_SPEED, m_wndANimationSpeed);
	DDX_CBIndex(pDX, IDC_VISUAL_MNGR, m_nVisualMngr);
	DDX_CBIndex(pDX, IDC_ANIMATION, m_nAnimation);
	DDX_Slider(pDX, IDC_ANIM_SPEED, m_nAnimationSpeed);
	DDX_Text(pDX, IDC_ANIM_SPEED_VAL, m_strAnimSpeed);
	DDX_Slider(pDX, IDC_TRANSP, m_nTransparency);
	DDX_Text(pDX, IDC_TRANSP_VAL, m_strTransparency);
	DDX_Check(pDX, IDC_SMALL_CAPTION, m_bSmallCaption);
	DDX_Radio(pDX, IDC_POPUP_SOURCE, m_nPopupSource);
	DDX_CBIndex(pDX, IDC_ICONS, m_nIcon);
	DDX_Text(pDX, IDC_LINK, m_strLink);
	DDX_Text(pDX, IDC_TEXT, m_strText);
	DDX_Slider(pDX, IDC_AUTOCLOSE_TIME, m_nAutoCloseTime);
	DDX_Text(pDX, IDC_AUTOCLOSE_VAL, m_strAutoClose);
	DDX_Check(pDX, IDC_AUTO_CLOSE, m_bAutoClose);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPopupDemoDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CPopupDemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_CBN_SELCHANGE(IDC_VISUAL_MNGR, OnSelchangeVisualMngr)
	ON_CBN_SELCHANGE(IDC_ANIMATION, OnSelchangeAnimation)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SMALL_CAPTION, OnSmallCaption)
	ON_BN_CLICKED(IDC_POPUP_SOURCE, OnPopupSource)
	ON_BN_CLICKED(IDC_POPUP_SOURCE2, OnPopupSource)
	ON_BN_CLICKED(IDC_AUTO_CLOSE, OnAutoClose)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ON_CLOSEPOPUPWINDOW, OnClosePopup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDemoDlg message handlers

BOOL CPopupDemoDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CBCGPToolBar::AddToolBarForImageCollection (IDR_TOOLBAR1, IDB_MENUIMAGES);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_wndANimationSpeed.SetRange (1, 100);
	m_wndTransparency.SetRange (10, 255);
	m_wndAutoCloseTime.SetRange (1, 5);

	m_nVisualMngr = theApp.GetInt (_T("VisualManager"), 2);
	m_nAnimation = theApp.GetInt (_T("AnimationType"), 3);
	m_nAnimationSpeed = theApp.GetInt (_T("AnimationSpeed"), 30);
	m_nTransparency = theApp.GetInt (_T("Transparency"), 200);
	m_bSmallCaption = theApp.GetInt (_T("SmallCaption"), TRUE);
	m_nPopupSource = theApp.GetInt (_T("PopupSource"), 0);

	m_strAnimSpeed.Format (_T("%d"), m_nAnimationSpeed);
	m_strTransparency.Format (_T("%d"), m_nTransparency);

	m_nAutoCloseTime = theApp.GetInt (_T("AutoClose"), 3);
	m_bAutoClose = m_nAutoCloseTime > 0;

	if (m_nAutoCloseTime == 0)
	{
		m_nAutoCloseTime = 3;
	}

	m_strAutoClose.Format (_T("%d"), m_nAutoCloseTime);

	m_wndAutoCloseTime.EnableWindow (m_bAutoClose);

	m_Icons.SetImageSize (CSize (32, 32));
	m_Icons.Load (IDB_ICONS);

	m_IconsSmall.SetImageSize (CSize (16, 16));
	m_IconsSmall.Load (IDB_ICONS_SMALL);

	for (int i = 0; i <= m_Icons.GetCount (); i++)
	{
		CString strText;

		if (i == 0)
		{
			strText = _T("No icon");
		}
		else
		{
			strText.Format (_T("Icon %d"), i);
		}

		m_wndIcons.AddString (strText);
	}

	UpdateData (FALSE);

	OnSelchangeVisualMngr();
	EnableSourceControls ();

	m_menuPopup.LoadMenu (IDR_POPUP);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPopupDemoDlg::OnPaint() 
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
HCURSOR CPopupDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPopupDemoDlg::OnShow() 
{
	UpdateData ();

	CBCGPPopupWindow* pPopup = new CBCGPPopupWindow;

	pPopup->SetAnimationType ((CBCGPPopupMenu::ANIMATION_TYPE) m_nAnimation);
	pPopup->SetAnimationSpeed (m_nAnimationSpeed);
	pPopup->SetTransparency (m_nTransparency);
	pPopup->SetSmallCaption (m_bSmallCaption);
	pPopup->SetAutoCloseTime (m_bAutoClose ? m_nAutoCloseTime * 1000 : 0);

	if (m_nPopupSource == 0)
	{
		if (m_nVisualMngr == 5)	// MSN-style
		{
			pPopup->Create (this, IDD_DIALOG2, NULL, 
				m_ptPopup, RUNTIME_CLASS (CMSNDlg));
		}
		else
		{
			pPopup->Create (this, IDD_DIALOG1, 
				m_menuPopup.GetSubMenu (0)->GetSafeHmenu (), 
				m_ptPopup, RUNTIME_CLASS (CMyPopupDlg));
		}
	}
	else
	{
		// Create indirect:
		CBCGPPopupWndParams params;

		if (m_nIcon > 0)
		{
			params.m_hIcon = m_Icons.ExtractIcon (m_nIcon - 1);
		}

		params.m_strText = m_strText;
		params.m_strURL = m_strLink;
		params.m_nURLCmdID = 101;

		pPopup->Create (this, params, NULL, m_ptPopup);
	}

	HICON hIcon = (HICON) ::LoadImage (::AfxGetResourceHandle (), 
		MAKEINTRESOURCE (IDR_MAINFRAME),
				IMAGE_ICON, ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0);

	pPopup->SetIcon (hIcon, FALSE);
	pPopup->SetWindowText (_T("Message"));

	EnableControls (FALSE);
}

LRESULT CPopupDemoDlg::OnClosePopup(WPARAM,LPARAM lp)
{
	CBCGPPopupWindow* pPopup = (CBCGPPopupWindow*) lp;
	ASSERT_VALID (pPopup);

	m_ptPopup = pPopup->GetLastPos ();

	EnableControls ();
	return 0;
}

void CPopupDemoDlg::EnableControls (BOOL bEnable)
{
	CWnd* pWndChild = GetWindow (GW_CHILD);

	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);
		pWndChild->EnableWindow (bEnable);
		pWndChild = pWndChild->GetNextWindow ();
	}

	if (bEnable)
	{
		EnableSourceControls ();
	}
}

void CPopupDemoDlg::OnSelchangeVisualMngr() 
{
	int nPrevVisualMngr = m_nVisualMngr;

	UpdateData ();

	switch (m_nVisualMngr)
	{
	case 0:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case 1:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case 2:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		break;

	case 3:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		break;

	case 4:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case 5:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CMSNVisualManager));
		break;

	case 6:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 7:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 8:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 9:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;
	}

	if (nPrevVisualMngr == 5 && !m_bSmallCaption)
	{
		m_bSmallCaption = TRUE;

		UpdateData (FALSE);
		OnSmallCaption();
	}

	if (m_nVisualMngr == 5 && m_bSmallCaption)
	{
		m_bSmallCaption = FALSE;

		UpdateData (FALSE);
		OnSmallCaption();
	}

	theApp.WriteInt (_T("VisualManager"), m_nVisualMngr);
}

void CPopupDemoDlg::OnSelchangeAnimation() 
{
	UpdateData ();
	theApp.WriteInt (_T("AnimationType"), m_nAnimation);
}

void CPopupDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	UpdateData ();

	theApp.WriteInt (_T("AnimationSpeed"), m_nAnimationSpeed);
	m_strAnimSpeed.Format (_T("%d"), m_nAnimationSpeed);

	theApp.WriteInt (_T("Transparency"), m_nTransparency);
	m_strTransparency.Format (_T("%d"), m_nTransparency);

	theApp.WriteInt (_T("AutoClose"), m_bAutoClose ? m_nAutoCloseTime : 0);
	m_strAutoClose.Format (_T("%d"), m_nAutoCloseTime);

	UpdateData (FALSE);
}

void CPopupDemoDlg::OnSmallCaption() 
{
	UpdateData ();
	theApp.WriteInt (_T("SmallCaption"), m_bSmallCaption);
}

void CPopupDemoDlg::OnPopupSource() 
{
	UpdateData ();
	theApp.WriteInt (_T("PopupSource"), m_nPopupSource);

	EnableSourceControls ();
}

void CPopupDemoDlg::EnableSourceControls ()
{
	BOOL bEnable = m_nPopupSource != 0;

	m_wndText.EnableWindow (bEnable);
	m_wndLink.EnableWindow (bEnable);
	m_wndLabel3.EnableWindow (bEnable);
	m_wndLabel2.EnableWindow (bEnable);
	m_wndLabel1.EnableWindow (bEnable);
	m_wndIcons.EnableWindow (bEnable);
}

void CPopupDemoDlg::OnAutoClose() 
{
	UpdateData ();

	m_wndAutoCloseTime.EnableWindow (m_bAutoClose);
	theApp.WriteInt (_T("AutoClose"), m_bAutoClose ? m_nAutoCloseTime : 0);
}
