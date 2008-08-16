// RecurrenceRuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bcgpcalendardemo.h"
#include "RecurrenceRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecurrenceRuleDlg dialog

IMPLEMENT_DYNAMIC(CRecurrenceRuleDlg, CDialog);

CRecurrenceRuleDlg::CRecurrenceRuleDlg()
	: m_Rule (NULL)
{
	//{{AFX_DATA_INIT(CRecurrenceRuleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CRecurrenceRuleDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create (GetID (), pParentWnd);
}

void CRecurrenceRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecurrenceRuleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecurrenceRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CRecurrenceRuleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecurrenceRuleDlg message handlers
