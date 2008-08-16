// MyCustomizationPage.cpp : implementation file
//

#include "stdafx.h"
#include "CustomPages.h"
#include "MyCustomizationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyCustomizationPage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyCustomizationPage dialog


CMyCustomizationPage::CMyCustomizationPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CMyCustomizationPage::IDD)
{
	//{{AFX_DATA_INIT(CMyCustomizationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyCustomizationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCustomizationPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyCustomizationPage, CDialog)
	//{{AFX_MSG_MAP(CMyCustomizationPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCustomizationPage message handlers
