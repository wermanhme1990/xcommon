// ToolBoxBar.cpp: implementation of the CToolBoxBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBoxBar.h"
#include "resource.h"

const COLORREF clrList = RGB (0, 204, 153);

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar

BEGIN_MESSAGE_MAP(CToolBoxBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CToolBoxBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar construction/destruction

CToolBoxBar::CToolBoxBar()
{
	// TODO: add one-time construction code here

}

CToolBoxBar::~CToolBoxBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CToolBoxBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

//	m_images.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 255));
	CBitmap bmpIcons;
	bmpIcons.LoadBitmap (IDB_SHORTCUTS);

	m_images.Create (32, 32, ILC_COLOR8 | ILC_MASK, 0, 10);
	m_images.Add (&bmpIcons, RGB (255, 0, 255));

	// Create CListCtrl windows.
	// TODO: create your own tab windows here:
	const DWORD dwStyle = LVS_ICON | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	m_wndList.Create (dwStyle, CRect (0, 0, 0, 0),
		this, (UINT)-1);

	m_wndList.SetBkColor (clrList);
	m_wndList.SetTextBkColor (clrList);

	// Setup list content:
	m_wndList.SetImageList (&m_images, LVSIL_NORMAL);
	m_wndList.InsertItem (0,_T("One"),0);
	m_wndList.InsertItem (1,_T("Two"),1);
	m_wndList.InsertItem (2,_T("Three"),2);
	m_wndList.InsertItem (3,_T("Four"),3);

	return 0;
}

void CToolBoxBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// List control should cover a whole client area:
	m_wndList.SetWindowPos (NULL, 0, 0, 
		cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxList

CToolBoxList::CToolBoxList()
{
}

CToolBoxList::~CToolBoxList()
{
}


BEGIN_MESSAGE_MAP(CToolBoxList, CListCtrl)
	//{{AFX_MSG_MAP(CToolBoxList)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxList message handlers

BOOL CToolBoxList::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	pDC->FillSolidRect (rectClient, clrList);
	return TRUE;
}
