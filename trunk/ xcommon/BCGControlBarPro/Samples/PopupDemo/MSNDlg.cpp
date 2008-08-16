// MSNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "popupdemo.h"
#include "MSNDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSNDlg dialog

IMPLEMENT_DYNCREATE(CMSNDlg, CBCGPPopupDlg)

CMSNDlg::CMSNDlg()
{
	//{{AFX_DATA_INIT(CMSNDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMSNDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPopupDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSNDlg)
	DDX_Control(pDX, IDC_OPTIONS, m_Options);
	DDX_Control(pDX, IDC_BUTTON1, m_btnRL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMSNDlg, CBCGPPopupDlg)
	//{{AFX_MSG_MAP(CMSNDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSNDlg message handlers

void CMSNDlg::OnDraw (CDC* pDC)
{
	CBCGPPopupDlg::OnDraw (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	CSize sizeLogo = m_imgLogo.GetImageSize ();

	CBCGPDrawState ds;
	m_imgLogo.PrepareDrawImage (ds);

	m_imgLogo.Draw (pDC, 
		rectClient.right - sizeLogo.cx - 5,
		rectClient.bottom - sizeLogo.cy - 5,
		0);

	m_imgLogo.EndDrawImage (ds);
}


BOOL CMSNDlg::OnInitDialog() 
{
	CBCGPPopupDlg::OnInitDialog();
	
	m_Options.m_bAlwaysUnderlineText = FALSE;
	m_Options.m_bDefaultClickProcess = FALSE;
	
	m_btnRL.m_bMultilineText = TRUE;
	m_btnRL.m_bAlwaysUnderlineText = FALSE;
	m_btnRL.m_bDefaultClickProcess = TRUE;

	m_imgLogo.Load (IDB_LOGO);
	m_imgLogo.SetTransparentColor (RGB (236, 0, 140));
	m_imgLogo.SetSingleImage ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMSNDlg::OnButton1() 
{
	::ShellExecute (NULL, NULL, _T("http://www.bcgsoft.com"), NULL, NULL, NULL);
}

void CMSNDlg::OnOptions() 
{
	// TODO: Add your control notification handler code here
	
}
