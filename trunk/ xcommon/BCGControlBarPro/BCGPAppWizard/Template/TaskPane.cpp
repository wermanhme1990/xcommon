// $$taskpane_ifile$$.cpp : implementation of the $$TASKPANE_CLASS$$ class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "$$taskpane_hfile$$.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// $$TASKPANE_CLASS$$

BEGIN_MESSAGE_MAP($$TASKPANE_CLASS$$, CBCGPTasksPane)
	//{{AFX_MSG_MAP($$TASKPANE_CLASS$$)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// $$TASKPANE_CLASS$$ construction/destruction

$$TASKPANE_CLASS$$::$$TASKPANE_CLASS$$()
{
	m_nDocumentsGroup = -1;
$$IF(VERBOSE)
	// TODO: add one-time construction code here

$$ENDIF
}

$$TASKPANE_CLASS$$::~$$TASKPANE_CLASS$$()
{
}

/////////////////////////////////////////////////////////////////////////////
// $$TASKPANE_CLASS$$ message handlers

int $$TASKPANE_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCaption (_T("Tasks Pane"));

	SetIconsList (IDB_TASKS, 16);
	EnableNavigationToolbar (TRUE);

	m_nDocumentsGroup = AddGroup (_T("Open a document"), FALSE, TRUE);

$$IF(VERBOSE)
	// Add MRU list:
$$ENDIF
	AddMRUFilesList (m_nDocumentsGroup);
	AddTask (m_nDocumentsGroup, _T("More Documents..."), 0, ID_FILE_OPEN);

	int nPage1Gr2 = AddGroup (_T("Custom group"));

	AddTask (nPage1Gr2, _T("Task 1"), 1, ID_TASK1);
	AddTask (nPage1Gr2, _T("Task 2"), 2, ID_TASK2);
	AddTask (nPage1Gr2, _T("Task 3"), 3, ID_TASK3);
	AddTask (nPage1Gr2, _T("Task 4"), 4, ID_TASK4);
	AddLabel (nPage1Gr2, _T("Label"));
$$IF(TASKPANE_PAGES)

$$IF(VERBOSE)
	// Add second page:
$$ENDIF
	int nPage2 = AddPage (_T("Custom page"));

	int nPage2Gr1 = AddGroup (nPage2, _T("Tree Control"));

	if (!CreateTreeControl())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	const int nControlHeight = 65;

	AddWindow (nPage2Gr1, m_wndTree.GetSafeHwnd (), nControlHeight);
	AddTask (nPage2Gr1, _T("My favorites..."), 5, ID_TASK5);

	int nPage2Gr2 = AddGroup (nPage2, _T("Edit Control"));

	if (!CreateEditControl())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	AddWindow (nPage2Gr2, m_wndEdit.GetSafeHwnd (), nControlHeight);
$$ENDIF
$$IF(VERBOSE)

	// Create tasks pane windows.
	// TODO: create your own tasks panes here:

$$ENDIF
	return 0;
}

void $$TASKPANE_CLASS$$::UpdateMRUFilesList ()
{
	AddMRUFilesList (m_nDocumentsGroup);
	RedrawWindow ();
}
$$IF(TASKPANE_PAGES)

BOOL $$TASKPANE_CLASS$$::CreateTreeControl()
{
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	if(!m_wndTree.Create (dwTreeStyle, rectDummy, this, (UINT)-1))
	{
		TRACE0("Failed to create the custom window\n");
		return FALSE;
	}

	m_wndTree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	HTREEITEM hRoot = m_wndTree.InsertItem (_T("Folders"));

	m_wndTree.InsertItem (_T("Folder 1"), hRoot);
	m_wndTree.InsertItem (_T("Folder 2"), hRoot);
	m_wndTree.InsertItem (_T("Folder 3"), hRoot);
	m_wndTree.InsertItem (_T("Folder 4"), hRoot);

	m_wndTree.Expand (hRoot, TVE_EXPAND);

	m_wndTree.RedrawWindow ();
	return TRUE;
}

BOOL $$TASKPANE_CLASS$$::CreateEditControl()
{
	CRect rectDummy (0, 0, 0, 0);
	DWORD dwEditStyle = WS_CHILD | WS_VISIBLE |
		ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE;

	m_Font.CreateStockObject (DEFAULT_GUI_FONT);

	if (!m_wndEdit.Create (dwEditStyle, rectDummy, this, (UINT)-1))
	{
		TRACE0("Failed to create the edit window\n");
		return FALSE;
	}

	m_wndEdit.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	m_wndEdit.SetFont (&m_Font);
	m_wndEdit.SetWindowText(
		_T("This is the standard multiline Edit Box.\r\nHere you can type your text."));

	return TRUE;
}
$$ENDIF
