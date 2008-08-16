// TableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBGridDemo.h"
#include "TableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableDlg dialog


CTableDlg::CTableDlg(CStringList& lstTable, CWnd* pParent /*=NULL*/)
	: CDialog(CTableDlg::IDD, pParent),
	m_lstTable (lstTable)
{
	//{{AFX_DATA_INIT(CTableDlg)
	m_strTable = _T("");
	//}}AFX_DATA_INIT
}


void CTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_TABLES, m_wndTables);
	DDX_LBString(pDX, IDC_TABLES, m_strTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableDlg, CDialog)
	//{{AFX_MSG_MAP(CTableDlg)
	ON_LBN_DBLCLK(IDC_TABLES, OnDblclkTables)
	ON_LBN_SELCHANGE(IDC_TABLES, OnSelchangeTables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableDlg message handlers

void CTableDlg::OnDblclkTables() 
{
	OnOK ();
}

void CTableDlg::OnSelchangeTables() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for (POSITION pos = m_lstTable.GetHeadPosition (); pos != NULL;)
	{
		m_wndTables.AddString (m_lstTable.GetNext (pos));
	}

	m_wndTables.SetCurSel (0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTableDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
