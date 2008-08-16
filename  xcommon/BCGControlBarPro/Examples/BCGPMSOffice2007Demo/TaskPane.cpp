// TaskPane.cpp : implementation of the CTaskPane class
//

#include "stdafx.h"
#include "BCGPMSOffice2007Demo.h"
#include "TaskPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskPane

BEGIN_MESSAGE_MAP(CTaskPane, CBCGPTasksPane)
	//{{AFX_MSG_MAP(CTaskPane)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskPane construction/destruction

CTaskPane::CTaskPane()
{
	m_nDocumentsGroup = -1;
	// TODO: add one-time construction code here

}

CTaskPane::~CTaskPane()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPane message handlers

int CTaskPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCaption (_T("Tasks Pane"));

	SetIconsList (IDB_TASKS, 16);
	EnableNavigationToolbar (TRUE);

	m_nDocumentsGroup = AddGroup (_T("Open a document"), FALSE, TRUE);

	// Add MRU list:
	AddMRUFilesList (m_nDocumentsGroup);
	AddTask (m_nDocumentsGroup, _T("More Documents..."), 0, ID_FILE_OPEN);

	int nPage1Gr2 = AddGroup (_T("Custom group"));

	AddTask (nPage1Gr2, _T("Task 1"), 1, ID_TASK1);
	AddTask (nPage1Gr2, _T("Task 2"), 2, ID_TASK2);
	AddTask (nPage1Gr2, _T("Task 3"), 3, ID_TASK3);
	AddTask (nPage1Gr2, _T("Task 4"), 4, ID_TASK4);
	AddLabel (nPage1Gr2, _T("Label"));

	// Create tasks pane windows.
	// TODO: create your own tasks panes here:

	return 0;
}

void CTaskPane::UpdateMRUFilesList ()
{
	AddMRUFilesList (m_nDocumentsGroup);
	RedrawWindow ();
}
