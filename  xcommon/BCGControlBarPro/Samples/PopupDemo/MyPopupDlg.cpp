// MyPopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "popupdemo.h"
#include "MyPopupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPopupDlg dialog

IMPLEMENT_DYNCREATE(CMyPopupDlg, CBCGPPopupDlg)

CMyPopupDlg::CMyPopupDlg()
{
	//{{AFX_DATA_INIT(CMyPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPopupDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPopupDlg)
	DDX_Control(pDX, IDC_FROM, m_wndFrom);
	DDX_Control(pDX, IDC_FLAG, m_btnFlag);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON1, m_btnRL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPopupDlg, CBCGPPopupDlg)
	//{{AFX_MSG_MAP(CMyPopupDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_FLAG, OnFlag)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPopupDlg message handlers

BOOL CMyPopupDlg::OnInitDialog() 
{
	CBCGPPopupDlg::OnInitDialog();
	
	m_wndFrom.SetFont (&globalData.fontBold);
	m_btnFlag.SetImage (IDB_FLAG);
	m_btnDelete.SetImage (IDB_DELETE);

	m_btnRL.m_bMultilineText = TRUE;
	m_btnRL.m_bAlwaysUnderlineText = FALSE;
	m_btnRL.m_bDefaultClickProcess = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyPopupDlg::OnDelete() 
{
	GetParent ()->PostMessage (WM_CLOSE);
}

void CMyPopupDlg::OnFlag() 
{
	GetParent ()->PostMessage (WM_CLOSE);
}

void CMyPopupDlg::OnButton1() 
{
	GetParent ()->PostMessage (WM_CLOSE);
}
