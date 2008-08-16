// BCGPMSMMenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMSMMenuBar.h"

#include "BCGPMSMVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMMenuBar

IMPLEMENT_DYNCREATE(CBCGPMSMMenuBar, CBCGPMenuBar)

CBCGPMSMMenuBar::CBCGPMSMMenuBar ()
{
}

CBCGPMSMMenuBar::~CBCGPMSMMenuBar ()
{
}

BEGIN_MESSAGE_MAP(CBCGPMSMMenuBar, CBCGPMenuBar)
	//{{AFX_MSG_MAP(CBCGPMSMMenuBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMMenuBar message handlers

int CBCGPMSMMenuBar::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPMenuBar::OnCreate (lpCreateStruct) == -1)
		return -1;

	SetBarStyle((GetBarStyle () & 
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT))
		| CBRS_SIZE_DYNAMIC);
	SetBorders ();
	
	return 0;
}
