// BCGPGridExampleView.cpp : implementation of the CBCGPGridExampleView class
//

#include "stdafx.h"
#include "BCGPGridExample.h"

#include "MainFrm.h"

#include "BCGPGridExampleDoc.h"
#include "BCGPGridExampleView.h"

#include "BasicGridCtrl.h"
#include "ReportGridCtrl.h"
#include "CellTypesGridCtrl.h"
#include "ColorGridCtrl.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CToolbarLabel : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CToolbarLabel)

public:
	CToolbarLabel (UINT nID = 0, LPCTSTR lpszText = NULL);

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
						 BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,						
						 BOOL bHighlight = FALSE,						
						 BOOL bDrawBorder = TRUE, 
						 BOOL bGrayDisabledButtons = TRUE);
};

IMPLEMENT_SERIAL(CToolbarLabel, CBCGPToolbarButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolbarLabel::CToolbarLabel (UINT nID, LPCTSTR lpszText)
{
	if (lpszText != NULL)	
	{		
		m_strText = lpszText;	
	}	

	m_bText = TRUE;
	m_nID = nID;
	m_iImage = -1;
}

void CToolbarLabel::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
							  BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,	
							  BOOL bDrawBorder,	BOOL bGrayDisabledButtons)
{
	UINT nStyle = m_nStyle;	
	m_nStyle &= ~TBBS_DISABLED;

	CBCGPToolbarButton::OnDraw (pDC, rect, pImages, bHorz, FALSE,
								FALSE, FALSE, FALSE);

	m_nStyle = nStyle;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView

IMPLEMENT_DYNCREATE(CBCGPGridExampleView, CBCGPGridView)

BEGIN_MESSAGE_MAP(CBCGPGridExampleView, CBCGPGridView)
	//{{AFX_MSG_MAP(CBCGPGridExampleView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FIELD_CHOOSER, OnFieldChooser)
	ON_UPDATE_COMMAND_UI(ID_FIELD_CHOOSER, OnUpdateFieldChooser)
	ON_COMMAND(ID_GROUP_BY_BOX, OnGroupByBox)
	ON_UPDATE_COMMAND_UI(ID_GROUP_BY_BOX, OnUpdateGroupByBox)
	ON_COMMAND(ID_COLOR_THEME_COMBO, OnColorThemeCombo)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ON_HIDE_COLUMNCHOOSER, OnHideColumnChooser)
	ON_CBN_SELENDOK(ID_COLOR_THEME_COMBO, OnColorThemeCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView construction/destruction

CBCGPGridExampleView::CBCGPGridExampleView()
{
	m_pCurrGrid = NULL;
	m_nCurrType = -1;
	m_bIsToolbar = FALSE;
}

CBCGPGridExampleView::~CBCGPGridExampleView()
{
}

BOOL CBCGPGridExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPGridView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView drawing

void CBCGPGridExampleView::OnDraw(CDC* pDC)
{
	CBCGPGridExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView printing

void CBCGPGridExampleView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CBCGPGridExampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return CBCGPGridView::OnPreparePrinting (pInfo);
}

void CBCGPGridExampleView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnBeginPrinting (pDC, pInfo);
}

void CBCGPGridExampleView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnEndPrinting (pDC, pInfo);
}

void CBCGPGridExampleView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView diagnostics

#ifdef _DEBUG
void CBCGPGridExampleView::AssertValid() const
{
	CBCGPGridView::AssertValid();
}

void CBCGPGridExampleView::Dump(CDumpContext& dc) const
{
	CBCGPGridView::Dump(dc);
}

CBCGPGridExampleDoc* CBCGPGridExampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPGridExampleDoc)));
	return (CBCGPGridExampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridExampleView message handlers

void CBCGPGridExampleView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CBCGPGridExampleView::SetViewType (int nType)
{
	CBCGPGridCtrl* pGridCtrl = m_arGrids [nType];
	ASSERT_VALID (pGridCtrl);

	m_nCurrType = nType;

	// Load toolbar:
	m_bIsToolbar = FALSE;

	UINT uiToolBarId = 0;

	switch (m_nCurrType)
	{
	case BCGGRID_BASIC:
		break;

	case BCGGRID_REPORT:
		uiToolBarId = IDR_TOOLBAR_REPORT;
		break;

	case BCGGRID_CELLTYPES:
		break;

	case BCGGRID_COLOR:
		uiToolBarId = IDR_TOOLBAR_COLOR;
		break;

	case BCGGRID_VIRTUAL:
		break;
	}

	if (uiToolBarId != NULL)
	{
		m_wndToolBar.LoadToolBar (uiToolBarId, 0, 0, TRUE /* Is locked */);

		// Setup toolbar buttons:
		for (int i = 0; i < m_wndToolBar.GetCount (); i++)
		{
			m_wndToolBar.SetToolBarBtnText (i, NULL, TRUE, FALSE);
		}

		m_wndToolBar.ReplaceButton (ID_COLOR_LABEL, 
			CToolbarLabel (ID_COLOR_LABEL, _T("Color theme: ")));

		CBCGPToolbarComboBoxButton comboButton (ID_COLOR_THEME_COMBO, 
			CImageHash::GetImageOfCommand (ID_COLOR_THEME_COMBO, FALSE), CBS_DROPDOWNLIST);

		comboButton.AddItem ("Light Blue Theme");
		comboButton.AddItem ("Silver Theme");
		comboButton.AddItem ("Green Theme");

		comboButton.SelectItem (theApp.m_Options.m_nColorTheme);

		m_wndToolBar.ReplaceButton (ID_COLOR_THEME_COMBO, comboButton);

		m_wndToolBar.ShowWindow (SW_SHOWNOACTIVATE);

		m_bIsToolbar = TRUE;
	}
	else
	{
		m_wndToolBar.ShowWindow (SW_HIDE);
	}

	if (pGridCtrl != m_pCurrGrid)
	{
		if (m_pCurrGrid->GetSafeHwnd () != NULL)
		{
			m_pCurrGrid->ShowWindow (SW_HIDE);
			m_pCurrGrid->ShowColumnsChooser (FALSE);
		}

		m_pCurrGrid = pGridCtrl;
		UpdateGrid ();
		m_pCurrGrid->ShowWindow (SW_SHOW);
	}
	m_pWndGridCtrl = m_pCurrGrid;

	AdjustLayout ();
	m_pCurrGrid->SetFocus ();
}

int CBCGPGridExampleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create toolbar:
	if (!m_wndToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);

	// Create grid:
	CRect rectClient;
	GetClientRect (rectClient);
		
	for (int i = 0; i < BCGGRID_VIEWS_NUM; i++)
	{
		CBCGPGridCtrl* pGridCtrl = NULL;

		switch (i)
		{
		case BCGGRID_BASIC:
			pGridCtrl = new CBasicGridCtrl;
			break;

		case BCGGRID_REPORT:
			pGridCtrl = new CReportGridCtrl;
			break;

		case BCGGRID_CELLTYPES:
			pGridCtrl = new CCellTypesGridCtrl;
			break;

		case BCGGRID_COLOR:
			pGridCtrl = new CColorGridCtrl;
			break;

		case BCGGRID_VIRTUAL:
			pGridCtrl = new CVirtualGridCtrl;
			break;

		default:
			ASSERT (FALSE);
			return -1;
		}

		if (pGridCtrl == NULL)
		{
			ASSERT (FALSE);
			return -1;
		}

		ASSERT_VALID (pGridCtrl);

		if (!pGridCtrl->Create (WS_CHILD, rectClient, this, 1))
		{
			ASSERT (FALSE);
			return -1;
		}

		m_arGrids.Add (pGridCtrl);
	}
	
	SetViewType (((CMainFrame*)GetParent ())->GetCurrentViewType ());
	return 0;
}

BOOL CBCGPGridExampleView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPGridExampleView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	AdjustLayout ();
}

void CBCGPGridExampleView::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPGridView::OnSetFocus(pOldWnd);
	
	if (m_pCurrGrid->GetSafeHwnd () != NULL)
	{
		m_pCurrGrid->SetFocus ();
	}
}

void CBCGPGridExampleView::OnDestroy() 
{
	for (int i = 0; i < m_arGrids.GetSize (); i++)
	{
		CBCGPGridCtrl* pGridCtrl = m_arGrids [i];
		if (pGridCtrl->GetSafeHwnd () != NULL)
		{
			pGridCtrl->DestroyWindow ();
			delete pGridCtrl;
		}
	}

	m_pCurrGrid = NULL;
	m_pWndGridCtrl = NULL;

	CBCGPGridView::OnDestroy();
}

void CBCGPGridExampleView::UpdateGrid ()
{
	if (m_pCurrGrid == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pCurrGrid);

	switch (m_nCurrType)
	{
	case BCGGRID_BASIC:
		break;

	case BCGGRID_REPORT:
		m_pCurrGrid->EnableGroupByBox (theApp.m_Options.m_bGroupByBox);
		m_pCurrGrid->ShowColumnsChooser (theApp.m_Options.m_bFieldChooser);
		break;

	case BCGGRID_CELLTYPES:
		break;

	case BCGGRID_COLOR:
		{
			CColorGridCtrl* pColorGrid = (CColorGridCtrl*) m_pCurrGrid;
			pColorGrid->UpdateColorTheme ();
		}
		break;

	case BCGGRID_VIRTUAL:
		break;
	}
}

LRESULT CBCGPGridExampleView::OnHideColumnChooser (WPARAM, LPARAM)
{
	theApp.m_Options.m_bFieldChooser = FALSE;
	return 0;
}

void CBCGPGridExampleView::AdjustLayout ()
{
	if (m_pCurrGrid->GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	int cyTlb = 0;

	if (m_bIsToolbar)
	{
		cyTlb = m_wndToolBar.CalcFixedLayout (FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos (NULL, 0, 0, 
									rectClient.Width (), cyTlb,
									SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndToolBar.AdjustLayout ();
	}

	m_pCurrGrid->SetWindowPos (NULL, 0, cyTlb, rectClient.Width (), rectClient.Height () - cyTlb,
							SWP_NOACTIVATE | SWP_NOZORDER);
}

void CBCGPGridExampleView::OnFieldChooser() 
{
	theApp.m_Options.m_bFieldChooser = !theApp.m_Options.m_bFieldChooser;
	UpdateGrid ();
}

void CBCGPGridExampleView::OnUpdateFieldChooser(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (theApp.m_Options.m_bFieldChooser);
}

void CBCGPGridExampleView::OnGroupByBox() 
{
	theApp.m_Options.m_bGroupByBox = !theApp.m_Options.m_bGroupByBox;
	UpdateGrid ();
}

void CBCGPGridExampleView::OnUpdateGroupByBox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (theApp.m_Options.m_bGroupByBox);
}

void CBCGPGridExampleView::OnColorThemeCombo() 
{
	CBCGPToolbarComboBoxButton* pCombobox = 
		(CBCGPToolbarComboBoxButton*) m_wndToolBar.GetButton (1);
	ASSERT_VALID (pCombobox);

	theApp.m_Options.m_nColorTheme = pCombobox->GetCurSel ();
	UpdateGrid ();

	if (m_pCurrGrid->GetSafeHwnd () != NULL)
	{
		m_pCurrGrid->SetFocus ();
	}
}
