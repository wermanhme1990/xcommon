// CmdFrame.cpp : implementation file
//

#include "stdafx.h"
#include "dlgbars.h"
#include "CmdFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdFrame

CCmdFrame::CCmdFrame(CBCGPDialog* pDlg) :
	m_pDlg (pDlg)
{
}

CCmdFrame::~CCmdFrame()
{
}


BEGIN_MESSAGE_MAP(CCmdFrame, CBCGPFrameWnd)
	//{{AFX_MSG_MAP(CCmdFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdFrame message handlers

BOOL CCmdFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	ASSERT_VALID (m_pDlg);
	return m_pDlg->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
