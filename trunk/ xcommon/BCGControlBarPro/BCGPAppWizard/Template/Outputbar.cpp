// $$outputbar_ifile$$.cpp : implementation of the $$OUTPUTBAR_CLASS$$ class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "$$outputbar_hfile$$.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// $$OUTPUTBAR_CLASS$$

BEGIN_MESSAGE_MAP($$OUTPUTBAR_CLASS$$, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP($$OUTPUTBAR_CLASS$$)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// $$OUTPUTBAR_CLASS$$ construction/destruction

$$OUTPUTBAR_CLASS$$::$$OUTPUTBAR_CLASS$$()
{
$$IF(VERBOSE)
	// TODO: add one-time construction code here

$$ENDIF
}

$$OUTPUTBAR_CLASS$$::~$$OUTPUTBAR_CLASS$$()
{
}

/////////////////////////////////////////////////////////////////////////////
// $$OUTPUTBAR_CLASS$$ message handlers

int $$OUTPUTBAR_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

$$IF(VERBOSE)
	// Create tabs window:
$$ENDIF
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.SetFlatFrame (FALSE, FALSE);

$$IF(VERBOSE)
	// Create list windows.
	// TODO: create your own window here:
$$ENDIF
	const DWORD dwViewStyle =	
		LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_wndList1.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndList2.Create (dwViewStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndList3.Create (dwViewStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

$$IF(VERBOSE)
	// Attach list windows to tab:
$$ENDIF
	m_wndTabs.AddTab (&m_wndList1, _T("Output 1"), -1);
	m_wndTabs.AddTab (&m_wndList2, _T("Output 2"), -1);
	m_wndTabs.AddTab (&m_wndList3, _T("Output 3"), -1);

	return 0;
}

void $$OUTPUTBAR_CLASS$$::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

$$IF(VERBOSE)
	// Tab control should cover a whole client area:
$$ENDIF
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
