// ToolBox.cpp: implementation of the CToolBoxBar class.
//

#include "stdafx.h"
#include "ToolBoxDemo.h"
#include "ToolBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar

CToolBoxBar::CToolBoxBar()
{
	CBCGPOutlookWnd::EnableAnimation ();
}

CToolBoxBar::~CToolBoxBar()
{
}

BEGIN_MESSAGE_MAP(CToolBoxBar, CBCGPToolBox)
	//{{AFX_MSG_MAP(CToolBoxBar)
	ON_COMMAND(ID_TOOLBOX_ADD_TAB, OnToolboxAddTab)
	ON_COMMAND(ID_TOOLBOX_CHOOSE_ITEMS, OnToolboxChooseItems)
	ON_COMMAND(ID_TOOLBOX_COPY, OnToolboxCopy)
	ON_COMMAND(ID_TOOLBOX_CUT, OnToolboxCut)
	ON_COMMAND(ID_TOOLBOX_DELETE, OnToolboxDelete)
	ON_COMMAND(ID_TOOLBOX_LIST_VIEW, OnToolboxListView)
	ON_COMMAND(ID_TOOLBOX_PASTE, OnToolboxPaste)
	ON_COMMAND(ID_TOOLBOX_RENAME, OnToolboxRename)
	ON_COMMAND(ID_TOOLBOX_RESET, OnToolboxReset)
	ON_UPDATE_COMMAND_UI(ID_TOOLBOX_LIST_VIEW, OnUpdateToolboxListView)
	ON_COMMAND(ID_TOOLBOX_SHOW_ALL, OnToolboxShowAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CToolBoxBar::OnShowToolboxMenu (CPoint point, CBCGPToolBoxPage* pPage, int nHit)
{
	theApp.GetContextMenuManager ()->ShowPopupMenu (
			IDR_TOOLBOX_CONTEXT_MENU, point.x, point.y, this, TRUE);

	return TRUE;
}

void CToolBoxBar::OnToolboxAddTab() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxChooseItems() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxCopy() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxCut() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxDelete() 
{
	int nPage = GetActivePage ();

	if (nPage >= 0)
	{
		int nButton = GetLastClickedTool (nPage);

		if (nButton >= 0)
		{
			GetPage (nPage)->DeleteButton (nButton);
		}
	}
}

void CToolBoxBar::OnToolboxListView() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxPaste() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxRename() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnToolboxReset() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBar::OnUpdateToolboxListView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck ();
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBarEx

CToolBoxBarEx::CToolBoxBarEx()
{
}

CToolBoxBarEx::~CToolBoxBarEx()
{
}

BEGIN_MESSAGE_MAP(CToolBoxBarEx, CBCGPToolBoxEx)
	//{{AFX_MSG_MAP(CToolBoxBarEx)
	ON_COMMAND(ID_TOOLBOX_ADD_TAB, OnToolboxAddTab)
	ON_COMMAND(ID_TOOLBOX_CHOOSE_ITEMS, OnToolboxChooseItems)
	ON_COMMAND(ID_TOOLBOX_COPY, OnToolboxCopy)
	ON_COMMAND(ID_TOOLBOX_CUT, OnToolboxCut)
	ON_COMMAND(ID_TOOLBOX_DELETE, OnToolboxDelete)
	ON_COMMAND(ID_TOOLBOX_LIST_VIEW, OnToolboxListView)
	ON_COMMAND(ID_TOOLBOX_PASTE, OnToolboxPaste)
	ON_COMMAND(ID_TOOLBOX_RENAME, OnToolboxRename)
	ON_COMMAND(ID_TOOLBOX_RESET, OnToolboxReset)
	ON_UPDATE_COMMAND_UI(ID_TOOLBOX_LIST_VIEW, OnUpdateToolboxListView)
	ON_COMMAND(ID_TOOLBOX_SHOW_ALL, OnToolboxShowAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CToolBoxBarEx::OnShowToolboxMenu (CPoint point, CBCGPToolBoxPage* pPage, int nHit)
{
	theApp.GetContextMenuManager ()->ShowPopupMenu (
			IDR_TOOLBOX_CONTEXT_MENU, point.x, point.y, this, TRUE);

	return TRUE;
}


void CToolBoxBarEx::OnToolboxAddTab() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxChooseItems() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxCopy() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxCut() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxDelete() 
{
	int nPage = -1;
	int nButton = GetLastClickedTool (nPage);

	if (nButton < 0 || nPage < 0)
	{
		return;
	}

	GetPage (nPage)->DeleteButton (nButton);
}

void CToolBoxBarEx::OnToolboxListView() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxPaste() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxRename() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxReset() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnUpdateToolboxListView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck ();
}


void CToolBoxBar::OnToolboxShowAll() 
{
	// TODO: Add your command handler code here
	
}

void CToolBoxBarEx::OnToolboxShowAll() 
{
	// TODO: Add your command handler code here
	
}
