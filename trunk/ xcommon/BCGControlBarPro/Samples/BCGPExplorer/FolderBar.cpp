// FolderBar.cpp : implementation file
//

#include "stdafx.h"
#include "bcgpexplorer.h"
#include "FolderBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int idTree = 1;

/////////////////////////////////////////////////////////////////////////////
// CFolderBar

CFolderBar::CFolderBar()
{
//	m_nSize = 300;	// Initial width
}

CFolderBar::~CFolderBar()
{
}


BEGIN_MESSAGE_MAP(CFolderBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CFolderBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFolderBar message handlers

int CFolderBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndShellTree.Create (dwViewStyle, rectDummy, this, idTree);
	return 0;
}

void CFolderBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	
	m_wndShellTree.SetWindowPos (NULL, 0, 0,
		cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFolderBar::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	m_wndShellTree.SetFocus ();
}

BOOL CFolderBar::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
