// BCGPMSMLinkssBar.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMSMoneyDemo.h"
#include "BCGPMSMLinksBar.h"
#include "BCGPMSMLinksBarButton.h"

#include "BCGPMSMVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMLinksBar

IMPLEMENT_DYNCREATE(CBCGPMSMLinksBar, CBCGPMSMToolBar)

CBCGPMSMLinksBar::CBCGPMSMLinksBar ()
{
	m_CurrentCommand = 0;
}

CBCGPMSMLinksBar::~CBCGPMSMLinksBar ()
{
}

BEGIN_MESSAGE_MAP(CBCGPMSMLinksBar, CBCGPMSMToolBar)
	//{{AFX_MSG_MAP(CBCGPMSMLinksBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMLinksBar message handlers
