// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "stdafx.h"
#include "MDITabsDemo.h"
#include "WorkspaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

static TCHAR* lpszMDITabsStyles [] = 
{
	_T("None"),
	_T("MDI Tabs (Standard)"),
	_T("MDI Tabbed Groups"),
	NULL
};


static TCHAR* lpszStyles [] = 
{
	_T("3D Tabs"),
	_T("OneNote"),
	_T("Visual Studio 2005"),
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	// TODO: add one-time construction code here

}

CWorkspaceBar::~CWorkspaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	m_wndPropList.EnableHeaderCtrl (FALSE);
	m_wndPropList.EnableDesciptionArea ();
	m_wndPropList.SetVSDotNetLook ();

	CBCGPProp* pMDITabsProp = new CBCGPProp (_T("Enable MDI Tabs"), 
		lpszMDITabsStyles [1],
		_T("Enable or disable either the standard MDI Tabs feature, or MDI Tabbed Groups feature"), 
		idShowMDITabs);

	pMDITabsProp->AddOption (_T ("None"));
	pMDITabsProp->AddOption (_T ("MDI Tabs (Standard)"));
	pMDITabsProp->AddOption (_T ("MDI Tabbed Groups"));

	int i = 0;
	for (i = 0; lpszMDITabsStyles [i] != NULL; i++)
	{
		pMDITabsProp->AddOption (lpszMDITabsStyles [i]);
	}

	switch (theApp.m_Options.m_nMDITabsType)
	{
	case CMDITabOptions::None:
		pMDITabsProp->SetValue (lpszMDITabsStyles [0]);
		break;

	case CMDITabOptions::MDITabsStandard:
		pMDITabsProp->SetValue (lpszMDITabsStyles [1]);
		break;

	case CMDITabOptions::MDITabbedGroups:
		pMDITabsProp->SetValue (lpszMDITabsStyles [2]);
		theApp.m_Options.m_bMaximizeMDIChild = TRUE;
		break;
	}

	pMDITabsProp->AllowEdit (FALSE);

	m_wndPropList.AddProperty (pMDITabsProp);
	
	m_wndPropList.AddProperty (new CBCGPProp (_T("Maximize child"), 
		(_variant_t) (theApp.m_Options.m_bMaximizeMDIChild == TRUE),
		_T("Always maximize MDI child windows"), idMaximizeMDIChild));
	
	m_wndPropList.AddProperty (new CBCGPProp (_T("Tabs on top"), 
		(_variant_t) (theApp.m_Options.m_bTabsOnTop == TRUE),
		_T("Show MDI tabs on top"), idTabsOnTop));

	CBCGPProp* pProp = new CBCGPProp (_T("MDI Tabs style"), _T(""),
		_T("One of: VS.NET, OneNote or Whidbey"), idTabsStyle);

	for (i = 0; lpszStyles [i] != NULL; i++)
	{
		pProp->AddOption (lpszStyles [i]);
	}

	switch (theApp.m_Options.m_nTabsStyle)
	{
	case CBCGPTabWnd::STYLE_3D_SCROLLED:
		pProp->SetValue (lpszStyles [0]);
		break;

	case CBCGPTabWnd::STYLE_3D_ONENOTE:
		pProp->SetValue (lpszStyles [1]);
		break;

	case CBCGPTabWnd::STYLE_3D_VS2005:
		pProp->SetValue (lpszStyles [2]);
		break;
	}

	pProp->AllowEdit (FALSE);
	m_wndPropList.AddProperty (pProp);
	
	m_wndPropList.AddProperty (new CBCGPProp (_T("Tabs auto-color"), 
		(_variant_t) (theApp.m_Options.m_bTabsAutoColor == TRUE),
		_T("Set MDI tabs automatic color"), idTabsAutoColor));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Tab icons"), 
		(_variant_t) (theApp.m_Options.m_bMDITabsIcons == TRUE),
		_T("Show document icons on MDI tabs"), idMDITabsIcons));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Tabs document menu"), 
		(_variant_t) (theApp.m_Options.m_bMDITabsDocMenu == TRUE),
		_T("Show menu with a list of opened documents"), idMDITabsDocMenu));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Drag tabs"), 
		(_variant_t) (theApp.m_Options.m_bDragMDITabs == TRUE),
		_T("Enable MDI tabs order changing"), idDragMDITabs));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Context Menu"), 
		(_variant_t) (theApp.m_Options.m_bMDITabsContextMenu == TRUE),
		_T("Enable MDI tabs context menu"), idMDITabsContextMenu));

	CBCGPProp* pBorderProp = new CBCGPProp (_T("Border Size"), 
		(_variant_t) ((short) theApp.m_Options.m_nMDITabsBorderSize),
		_T("Set MDI border"), idMDITabsBorderSize);
	pBorderProp->EnableSpinControl (TRUE, 0, 10);

	m_wndPropList.AddProperty (pBorderProp);

	m_wndPropList.AddProperty (new CBCGPProp (_T("No optimize MDI child redraw"), 
		(_variant_t) (theApp.m_Options.m_bDisableMDIChildRedraw == TRUE),
		_T("Disable MDI Child redraw optimization"), idDisableMDIChildRedraw));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Flat Frame"), 
		(_variant_t) (theApp.m_Options.m_bFlatFrame == TRUE),
		_T("Enable or disable 3D frame around MDI client area"), idFlatFrame));

	m_wndPropList.AddProperty (new CBCGPProp (_T("Custom Tooltips"), 
		(_variant_t) (theApp.m_Options.m_bCustomTooltips == TRUE),
		_T("Enable MDI tabs custom tooltips"), idCustomTooltips));

	SetPropState ();
	return 0;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndPropList.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkspaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndPropList.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

LRESULT CWorkspaceBar::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CBCGPProp* pProp = (CBCGPProp*) lParam;

	BOOL bResetMDIChild = FALSE;

	switch ((int) pProp->GetData ())
	{
	case idShowMDITabs:
		{
			CString strShowMdiTabOption = (LPCTSTR) (_bstr_t)pProp->GetValue ();
			bResetMDIChild = TRUE;

			for (int i = 0; lpszMDITabsStyles [i] != NULL; i++)
			{
				if (strShowMdiTabOption == lpszMDITabsStyles [i])
				{
					switch (i)
					{
					case 0:
						theApp.m_Options.m_nMDITabsType = CMDITabOptions::None;
						break;

					case 1:
						theApp.m_Options.m_nMDITabsType = CMDITabOptions::MDITabsStandard;
						break;

					case 2:
						theApp.m_Options.m_nMDITabsType = CMDITabOptions::MDITabbedGroups;
						theApp.m_Options.m_bMaximizeMDIChild = FALSE;
						CBCGPProp* pMaximizeChildProp = m_wndPropList.GetProperty (1);
						if (pMaximizeChildProp != NULL)
						{
							_variant_t var ((short)theApp.m_Options.m_bMaximizeMDIChild, VT_BOOL);
							pMaximizeChildProp->SetValue (var);
							pMaximizeChildProp->Enable (FALSE);
						}
						bResetMDIChild = FALSE;
						break;
					}
					break;
				}
			}
			
			SetPropState ();
		}
		break;

	case idMaximizeMDIChild:
		theApp.m_Options.m_bMaximizeMDIChild = (bool) pProp->GetValue ();
		bResetMDIChild = TRUE;
		break;

	case idTabsOnTop:
		theApp.m_Options.m_bTabsOnTop = (bool) pProp->GetValue ();
		break;

	case idTabsStyle:
		{
			CString strStyle = (LPCTSTR) (_bstr_t)pProp->GetValue ();

			for (int i = 0; lpszStyles [i] != NULL; i++)
			{
				if (strStyle == lpszStyles [i])
				{
					switch (i)
					{
					case 0:
						theApp.m_Options.m_nTabsStyle = CBCGPTabWnd::STYLE_3D_SCROLLED;
						break;

					case 1:
						theApp.m_Options.m_nTabsStyle = CBCGPTabWnd::STYLE_3D_ONENOTE;
						break;

					case 2:
						theApp.m_Options.m_nTabsStyle = CBCGPTabWnd::STYLE_3D_VS2005;
						break;
					}
					break;
				}
			}
		}
		break;

	case idTabsAutoColor:
		theApp.m_Options.m_bTabsAutoColor = (bool) pProp->GetValue ();
		break;

	case idMDITabsIcons:
		theApp.m_Options.m_bMDITabsIcons = (bool) pProp->GetValue ();
		break;

	case idMDITabsDocMenu:
		theApp.m_Options.m_bMDITabsDocMenu = (bool) pProp->GetValue ();
		break;

	case idDragMDITabs:
		theApp.m_Options.m_bDragMDITabs = (bool) pProp->GetValue ();
		break;

	case idMDITabsContextMenu:
		theApp.m_Options.m_bMDITabsContextMenu = (bool) pProp->GetValue ();
		break;

	case idMDITabsBorderSize:
		{
			int nBorderSize = (short) pProp->GetValue ();
			theApp.m_Options.m_nMDITabsBorderSize = min (10, max (0, nBorderSize));
		}
		break;

	case idDisableMDIChildRedraw:
		theApp.m_Options.m_bDisableMDIChildRedraw = (bool) pProp->GetValue ();
		break;

	case idFlatFrame:
		theApp.m_Options.m_bFlatFrame = (bool) pProp->GetValue ();
		break;

	case idCustomTooltips:
		theApp.m_Options.m_bCustomTooltips = (bool) pProp->GetValue ();
		break;
	}

	theApp.UpdateMDITabs (bResetMDIChild);
	return 0;
}

void CWorkspaceBar::SetPropState ()
{
	for (int i = 0; i < m_wndPropList.GetPropertyCount (); i++)
	{
		CBCGPProp* pProp = m_wndPropList.GetProperty (i);
		ASSERT_VALID (pProp);

		if ((int) pProp->GetData () != idShowMDITabs)
		{
			pProp->Enable (theApp.m_Options.m_nMDITabsType != CMDITabOptions::None);
		}
		
		if ((int) pProp->GetData () == idMaximizeMDIChild)
		{
			pProp->Enable (theApp.m_Options.m_nMDITabsType == CMDITabOptions::MDITabsStandard);
		}
	}

	if (m_wndPropList.GetSafeHwnd () != NULL)
	{
		m_wndPropList.RedrawWindow ();
	}
}
