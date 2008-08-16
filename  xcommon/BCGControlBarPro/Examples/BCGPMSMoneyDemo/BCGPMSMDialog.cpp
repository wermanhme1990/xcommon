// BCGPMSMDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMSMDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMDialog dialog


IMPLEMENT_DYNAMIC(CBCGPMSMDialog, CBCGPDialog)

CBCGPMSMDialog::CBCGPMSMDialog()
	: CBCGPDialog ()
{
	CommonConstruct ();
}

CBCGPMSMDialog::CBCGPMSMDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CBCGPDialog (nIDTemplate, pParentWnd)
{
	CommonConstruct ();
}

CBCGPMSMDialog::CBCGPMSMDialog(LPCTSTR lpszTemplateName, CWnd *pParentWnd /*=NULL*/)
	: CBCGPDialog (lpszTemplateName, pParentWnd)
{
	CommonConstruct ();
}

void CBCGPMSMDialog::CommonConstruct ()
{
	SetBackgroundColor (RGB (239, 247, 253));
}


BEGIN_MESSAGE_MAP(CBCGPMSMDialog, CBCGPDialog)
	//{{AFX_MSG_MAP(CBCGPMSMDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMDialog message handlers
