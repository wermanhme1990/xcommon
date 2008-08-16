// PropsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserTools.h"
#include "PropsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg dialog


CPropsDlg::CPropsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropsDlg)
	m_strProp2 = _T("");
	//}}AFX_DATA_INIT
}


void CPropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropsDlg)
	DDX_Text(pDX, IDC_PROP2, m_strProp2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropsDlg, CDialog)
	//{{AFX_MSG_MAP(CPropsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg message handlers
