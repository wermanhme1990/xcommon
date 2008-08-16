// ChildTaskPane.cpp : implementation of the CChildBar class
//

#include "stdafx.h"
#include "sidebardemo.h"
#include "ChildBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskPane

BEGIN_MESSAGE_MAP(CChildBar, CBCGPTasksPane)
	//{{AFX_MSG_MAP(CChildBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskPane construction/destruction

CChildBar::CChildBar()
{
	// TODO: add one-time construction code here

}

CChildBar::~CChildBar()
{
}

CBCGPMiniFrameWnd* CChildBar::CreateDefaultMiniframe (CRect rectInitial)
{
	ASSERT_VALID (this);

//	if ((GetStyle () & CBRS_FLOAT_MULTI) == 0)
//	{
//		m_pMiniFrameRTC = RUNTIME_CLASS (CSideBarWnd);
//		return CBCGPControlBar::CreateDefaultMiniframe (rectInitial);
//	}

	return CBCGPTasksPane::CreateDefaultMiniframe (rectInitial);
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPane message handlers

int CChildBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetCaption (_T("Embedded bar"));

	SetIconsList (IDB_TASKS, 16);

	EnableToolTips (FALSE);

	EnableWrapTasks (TRUE);
	EnableWrapLabels (TRUE);
	EnableOffsetCustomControls (FALSE);

	int nGroup2 = AddGroup (_T("News"));
	AddTask (nGroup2, _T("BCGControlBar Professional Edition 8.5 is available for downloading!"), 0, ID_TASK1);
	AddTask (nGroup2, _T("BCGSoft Professional Editor Library 6.75 is available!"), 0, ID_TASK2);
	
	int nGroup3 = AddGroup (_T("Calendar"));

	if (!CreateCalendar())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}


	AddWindow (nGroup3, m_wndCalendar.GetSafeHwnd (), 170);

	int nGroup4 = AddGroup (_T("Calculator"));

	if (!CreateCalculator())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	AddWindow (nGroup4, m_wndCalculator.GetSafeHwnd (), 170);

	int nGroup6 = AddGroup (_T("Clock"));

	if (!CreateClockControl())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	AddWindow (nGroup6, m_wndClock.GetSafeHwnd (), 149);

	return 0;
}

BOOL CChildBar::CreateCalendar()
{
	CRect rectDummy (0, 0, 0, 0);
	m_wndCalendar.Create (WS_VISIBLE, rectDummy, this, 1);
	m_wndCalendar.EnableWeekNumbers ();
	return TRUE;
}

BOOL CChildBar::CreateCalculator()
{
	CRect rectDummy (0, 0, 0, 0);
	if (!m_wndCalculator.CreateControl (this, rectDummy, 2))
	{
		TRACE0("Failed to create the edit window\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CChildBar::CreateClockControl ()
{
	CRect rectDummy (0, 0, 0, 0);
	if (!m_wndClock.Create (WS_CHILD | WS_VISIBLE, rectDummy, this, (UINT)-1))
	{
		TRACE0("Failed to create the custom window\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CChildBar::OnShowControlBarMenu (CPoint)
{
	return FALSE;
}

void CChildBar::OnOK()
{
	CWnd* pFocusWnd = CWnd::GetFocus();

	if (pFocusWnd == &m_wndCalendar)
	{
		// handle IDOK command for calendar window
		return;
	}

	CBCGPTasksPane::OnOK ();
}
//****************************************************************************************
void CChildBar::OnCancel()
{
	CWnd* pFocusWnd = CWnd::GetFocus();

	if (pFocusWnd == &m_wndCalendar)
	{
		// handle IDCANCEL command for calendar window
		return;
	}

	CBCGPTasksPane::OnCancel ();
}
