// ExplorerBar.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPIEDemo.h"
#include "ExplorerBar.h"
#include "BCGPIEDemoDoc.h"
#include "BCGPIEDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int idTree = 1;

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar

CExplorerBar::CExplorerBar()
{
}

CExplorerBar::~CExplorerBar()
{
}

BEGIN_MESSAGE_MAP(CExplorerBar, CBCGPTasksPane)
	//{{AFX_MSG_MAP(CExplorerBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, idTree, OnSelectTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar message handlers

int CExplorerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText (_T("Explorer Bar"));
	EnableOffsetCustomControls (FALSE);

	// ----------------------
	// Create favorites list:
	// ----------------------
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE | TVS_SINGLEEXPAND | TVS_TRACKSELECT;

	if(!m_wndFavorites.Create (dwTreeStyle, rectDummy,
		this, idTree))
	{
		TRACE0("Failed to create the Favorites window\n");
		return FALSE;      // fail to create
	}

	theApp.m_Favorites.FillTree (m_wndFavorites);

	// -----------
	// Add groups:
	// -----------
	int nGroup1 = AddGroup (_T("Favorites"), FALSE, TRUE);
	AddWindow (nGroup1, m_wndFavorites.GetSafeHwnd (), 200);

	int nGroup2 = AddGroup (_T("Links"));

	AddTask (nGroup2, _T("Best Company"), -1, ID_LINK_BESTCOMPANY);
	AddTask (nGroup2, _T("Best Car"), -1, ID_LINK_BESTCAR);
	AddTask (nGroup2, _T("Best Rock-Band"), -1, ID_LINK_BESTROCKBAND);
	AddTask (nGroup2, _T("Best City"), -1, ID_LINK_BESTCITY);

	return 0;
}
//************************************************************************************
void CExplorerBar::OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTreeItem = m_wndFavorites.GetSelectedItem ();
	if (hTreeItem != NULL)
	{
		CFavorit* pFavorit = (CFavorit*) m_wndFavorites.GetItemData (hTreeItem);
		ASSERT_VALID (pFavorit);

		if (!pFavorit->GetURL ().IsEmpty ())
		{
			CBCGPIEDemoView* pView = 
				(CBCGPIEDemoView*)((CFrameWnd*) AfxGetMainWnd ())->GetActiveView ();
			ASSERT_VALID (pView);
	
			pView->Navigate2 (pFavorit->GetURL ());
		}
	}
	
	*pResult = 0;
}
