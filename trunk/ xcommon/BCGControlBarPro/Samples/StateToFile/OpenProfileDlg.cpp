// OpenProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StateToFile.h"
#include "OpenProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static CString g_strDefault = _T ("Default Profile");
/////////////////////////////////////////////////////////////////////////////
// COpenProfileDlg dialog


COpenProfileDlg::COpenProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenProfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COpenProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenProfileDlg)
	DDX_Control(pDX, IDC_LIST_PROFILES, m_lstListProfiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenProfileDlg, CDialog)
	//{{AFX_MSG_MAP(COpenProfileDlg)
	ON_LBN_DBLCLK(IDC_LIST_PROFILES, OnDblclkListProfiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenProfileDlg message handlers

void COpenProfileDlg::OnOK() 
{
	// TODO: Add extra validation here
	int iIdx = m_lstListProfiles.GetCurSel ();

	if (iIdx != LB_ERR)
	{
		m_lstListProfiles.GetText (iIdx, m_strProfileName);
		if (m_strProfileName == g_strDefault)
		{
			CDialog::OnCancel ();
			return;
		}
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox ("Please select a profile");
	}
}

BOOL COpenProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);

	CFileFind finder;
    BOOL bWorking = finder.FindFile("*.reg");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		m_lstListProfiles.AddString (finder.GetFileTitle());
	}

	if (m_lstListProfiles.GetCount () == 0)
	{
		// there is no profiles saved
		CDialog::OnCancel();
		return TRUE; 
	}

	m_lstListProfiles.InsertString (0, g_strDefault);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenProfileDlg::OnDblclkListProfiles() 
{
	OnOK();		
}
