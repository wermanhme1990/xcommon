// SaveProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StateToFile.h"
#include "SaveProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg dialog


CSaveProfileDlg::CSaveProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveProfileDlg)
	m_strProfileName = _T("");
	m_strCurSelProfileName = _T("");
	//}}AFX_DATA_INIT
}


void CSaveProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveProfileDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDC_LIST_PROFILES, m_lstProfiles);
	DDX_Text(pDX, IDC_EDIT_PROFILE_NAME, m_strProfileName);
	DDX_LBString(pDX, IDC_LIST_PROFILES, m_strCurSelProfileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveProfileDlg)
	ON_LBN_SELCHANGE(IDC_LIST_PROFILES, OnSelchangeListProfiles)
	ON_EN_UPDATE(IDC_EDIT_PROFILE_NAME, OnUpdateEditProfileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg message handlers

void CSaveProfileDlg::OnSelchangeListProfiles() 
{
	UpdateData ();
	m_strProfileName = m_strCurSelProfileName;
	UpdateData (FALSE);
	OnUpdateEditProfileName ();
}

void CSaveProfileDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CSaveProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);

	CFileFind finder;
    BOOL bWorking = finder.FindFile("*.reg");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		m_lstProfiles.AddString (finder.GetFileTitle());
	}

	OnUpdateEditProfileName();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSaveProfileDlg::GetProfileName () const
{
	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);
	CString strFullName;
	strFullName.Format (_T ("%s\\%s"), szCurrDir, m_strProfileName);
	return strFullName;
}

void CSaveProfileDlg::OnUpdateEditProfileName() 
{
	UpdateData ();
	m_wndOK.EnableWindow (!m_strProfileName.IsEmpty ());
}
