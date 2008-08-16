// $$workspace2_ifile$$.cpp : implementation of the $$WORKSPACEBAR_CLASS2$$ class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "$$workspace2_hfile$$.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// $$WORKSPACEBAR_CLASS2$$

BEGIN_MESSAGE_MAP($$WORKSPACEBAR_CLASS2$$, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP($$WORKSPACEBAR_CLASS2$$)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// $$WORKSPACEBAR_CLASS2$$ construction/destruction

$$WORKSPACEBAR_CLASS2$$::$$WORKSPACEBAR_CLASS2$$()
{
$$IF(VERBOSE)
	// TODO: add one-time construction code here

$$ENDIF
}

$$WORKSPACEBAR_CLASS2$$::~$$WORKSPACEBAR_CLASS2$$()
{
}

/////////////////////////////////////////////////////////////////////////////
// $$WORKSPACEBAR_CLASS2$$ message handlers

int $$WORKSPACEBAR_CLASS2$$::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

$$IF(VERBOSE)
	// Create tree windows.
	// TODO: create your own tab windows here:
$$ENDIF
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

$$IF(VERBOSE)
	// Setup trees content:
$$ENDIF
	HTREEITEM hRoot1 = m_wndTree.InsertItem (_T("Root 2"));
	m_wndTree.InsertItem (_T("Item 1"), hRoot1);
	m_wndTree.InsertItem (_T("Item 2"), hRoot1);

	return 0;
}

void $$WORKSPACEBAR_CLASS2$$::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

$$IF(VERBOSE)
	// Tab control should cover a whole client area:
$$ENDIF
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void $$WORKSPACEBAR_CLASS2$$::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}
