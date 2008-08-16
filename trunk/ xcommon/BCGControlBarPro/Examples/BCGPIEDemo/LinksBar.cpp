// LinksBar.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPIEDemo.h"
#include "LinksBar.h"
#include "LinkButton.h"
#include "BCGPIEDemoDoc.h"
#include "BCGPIEDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CLinksBar, CBCGPToolBar, 1)

/////////////////////////////////////////////////////////////////////////////
// CLinksBar

CLinksBar::CLinksBar()
{
}

CLinksBar::~CLinksBar()
{
}


BEGIN_MESSAGE_MAP(CLinksBar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CLinksBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLinksBar message handlers

int CLinksBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	RestoreOriginalstate ();
	return 0;
}

BOOL CLinksBar::OnSendCommand (const CBCGPToolbarButton* pButton)
{
	CLinkButton* pLinkButton = DYNAMIC_DOWNCAST (CLinkButton, pButton);
	if (pLinkButton == NULL)
	{
		// Defauult processing
		return FALSE;
	}

	CString strURL = pLinkButton->GetURL ();

	((CBCGPIEDemoView*)GetParentFrame ()->GetActiveView())->Navigate2 (strURL, 0, NULL);
	return TRUE;
}

BOOL CLinksBar::RestoreOriginalstate ()
{
	RemoveAllButtons ();

	InsertButton (CLinkButton (_T("Best Company"), _T("http://www.bcgsoft.com")));
	InsertButton (CLinkButton (_T("Best Car"), _T("http://www.skoda-auto.com")));
	InsertButton (CLinkButton (_T("Best Rock-Band"), _T("http://www.planetaquarium.com/eng")));
	InsertButton (CLinkButton (_T("Best City"), _T("http://www.enlight.ru/camera/index_e.htm")));

	EnableCustomizeButton (TRUE, -1, _T(""));

	AdjustLayout ();
	Invalidate ();

	return TRUE;
}
