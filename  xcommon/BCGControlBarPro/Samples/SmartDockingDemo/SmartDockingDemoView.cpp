// SmartDockingDemoView.cpp : implementation of the CSmartDockingDemoView class
//

#include "stdafx.h"
#include "SmartDockingDemo.h"

#include "SmartDockingDemoDoc.h"
#include "SmartDockingDemoView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoView

IMPLEMENT_DYNCREATE(CSmartDockingDemoView, CFormView)

BEGIN_MESSAGE_MAP(CSmartDockingDemoView, CFormView)
	//{{AFX_MSG_MAP(CSmartDockingDemoView)
	ON_BN_CLICKED(IDC_MARKER0, OnChangeLook)
	ON_BN_CLICKED(IDC_MARKER1, OnChangeLook)
	ON_BN_CLICKED(IDC_MARKER2, OnChangeLook)
	ON_BN_CLICKED(IDC_COLOR, OnChangeLook)
	ON_BN_CLICKED(IDC_PROP_VIEW_VERTICAL, OnPropViewVertical)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoView construction/destruction

CSmartDockingDemoView::CSmartDockingDemoView()
	: CFormView(CSmartDockingDemoView::IDD)
{
	//{{AFX_DATA_INIT(CSmartDockingDemoView)
	m_nMarkerType = 0;
	m_bPropViewVertical = FALSE;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CSmartDockingDemoView::~CSmartDockingDemoView()
{
}

void CSmartDockingDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmartDockingDemoView)
	DDX_Control(pDX, IDC_COLOR_LABEL, m_wndColorLabel);
	DDX_Control(pDX, IDC_COLOR, m_wndColor);
	DDX_Radio(pDX, IDC_MARKER0, m_nMarkerType);
	DDX_Check(pDX, IDC_PROP_VIEW_VERTICAL, m_bPropViewVertical);
	//}}AFX_DATA_MAP
}

BOOL CSmartDockingDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

static COLORREF colors [] =
{
	RGB (255, 87, 87),
	RGB (0, 192, 75),
	RGB (255, 0, 255),
	RGB (20, 106, 234),
	RGB (181, 105, 72),
	RGB (48, 168, 204),
	RGB (24, 170, 122),
	RGB (255, 128, 87),
	RGB (203, 201, 138),
	RGB (167, 163, 177),
};

void CSmartDockingDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (m_palColorPicker.GetSafeHandle () == NULL)
	{
		int nNumColours = sizeof (colors)/sizeof(COLORREF);

		// Create the palette
		struct 
		{
			LOGPALETTE    LogPalette;
			PALETTEENTRY  PalEntry [100];
		}pal;

		LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
		pLogPalette->palVersion    = 0x300;
		pLogPalette->palNumEntries = (WORD) nNumColours; 

		for (int i = 0; i < nNumColours; i++)
		{
			pLogPalette->palPalEntry[i].peRed   = GetRValue(colors[i]);
			pLogPalette->palPalEntry[i].peGreen = GetGValue(colors[i]);
			pLogPalette->palPalEntry[i].peBlue  = GetBValue(colors[i]);
			pLogPalette->palPalEntry[i].peFlags = 0;
		}

		m_palColorPicker.CreatePalette (pLogPalette);
	}

	m_wndColor.SetPalette (&m_palColorPicker);
	m_wndColor.SetColumnsNumber (5);

	m_wndColor.EnableAutomaticButton (_T("Default"), RGB (192, 192, 192));
	m_wndColor.EnableOtherButton (_T("Other..."));

	COLORREF color = theApp.GetInt (_T("Color"), colors [7]);
	m_wndColor.SetColor (color);

	m_nMarkerType = theApp.GetInt (_T("MarkerType"), 0);
	m_bPropViewVertical = theApp.GetInt (_T("PropViewVertical"));

	((CMainFrame*) AfxGetMainWnd ())->SetPropViewDockMode (m_bPropViewVertical);

	UpdateData (FALSE);

	OnChangeLook();
}

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoView printing

void CSmartDockingDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CSmartDockingDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSmartDockingDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSmartDockingDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSmartDockingDemoView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoView diagnostics

#ifdef _DEBUG
void CSmartDockingDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSmartDockingDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSmartDockingDemoDoc* CSmartDockingDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmartDockingDemoDoc)));
	return (CSmartDockingDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoView message handlers

void CSmartDockingDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CSmartDockingDemoView::OnChangeLook() 
{
	UpdateData ();
	m_wndColor.EnableWindow (m_nMarkerType == 1);
	m_wndColorLabel.EnableWindow (m_nMarkerType == 1);

	if (m_nMarkerType == 1)
	{
		theApp.WriteInt (_T("Color"), m_wndColor.GetColor ());
	}

	theApp.WriteInt (_T("MarkerType"), m_nMarkerType);

	CBCGPSmartDockingParams params;

	if (m_nMarkerType == 1)	// Custom color
	{
		params.m_uiMarkerBmpResID [0] = IDB_MARKER_LEFT;
		params.m_uiMarkerBmpResID [1] = IDB_MARKER_RIGHT;
		params.m_uiMarkerBmpResID [2] = IDB_MARKER_TOP;
		params.m_uiMarkerBmpResID [3] = IDB_MARKER_BOTTOM;
		params.m_uiMarkerBmpResID [4] = IDB_MARKER_CENTER;
		params.m_clrToneDest = m_wndColor.GetColor ();
		params.m_clrToneSrc = RGB (61, 123, 241);
	}

	CBCGPDockManager::SetSmartDockingParams (params);
}

void CSmartDockingDemoView::OnPropViewVertical() 
{
	UpdateData ();

	theApp.WriteInt (_T("PropViewVertical"), m_bPropViewVertical);

	((CMainFrame*) AfxGetMainWnd ())->SetPropViewDockMode (m_bPropViewVertical);
}
