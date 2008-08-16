// ColorThemeToolbarView.cpp : implementation of the CColorThemeToolbarView class
//

#include "stdafx.h"
#include "ColorThemeToolbar.h"

#include "ColorThemeToolbarDoc.h"
#include "ColorThemeToolbarView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarView

IMPLEMENT_DYNCREATE(CColorThemeToolbarView, CFormView)

BEGIN_MESSAGE_MAP(CColorThemeToolbarView, CFormView)
	//{{AFX_MSG_MAP(CColorThemeToolbarView)
	ON_BN_CLICKED(IDC_COLOR1, OnColorTheme)
	ON_BN_CLICKED(IDC_COLOR2, OnColorTheme)
	ON_BN_CLICKED(IDC_COLOR3, OnColorTheme)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarView construction/destruction

CColorThemeToolbarView::CColorThemeToolbarView()
	: CFormView(CColorThemeToolbarView::IDD)
{
	//{{AFX_DATA_INIT(CColorThemeToolbarView)
	m_nColorTheme = theApp.GetInt (_T("ColorTheme"), 0);
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CColorThemeToolbarView::~CColorThemeToolbarView()
{
}

void CColorThemeToolbarView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorThemeToolbarView)
	DDX_Radio(pDX, IDC_COLOR1, m_nColorTheme);
	//}}AFX_DATA_MAP
}

BOOL CColorThemeToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CColorThemeToolbarView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	OnColorTheme();
}

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarView printing

void CColorThemeToolbarView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CColorThemeToolbarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CColorThemeToolbarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CColorThemeToolbarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CColorThemeToolbarView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarView diagnostics

#ifdef _DEBUG
void CColorThemeToolbarView::AssertValid() const
{
	CFormView::AssertValid();
}

void CColorThemeToolbarView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CColorThemeToolbarDoc* CColorThemeToolbarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorThemeToolbarDoc)));
	return (CColorThemeToolbarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarView message handlers

void CColorThemeToolbarView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CColorThemeToolbarView::OnColorTheme() 
{
	UpdateData ();

	((CMainFrame*)AfxGetMainWnd ())->SetColorTheme (m_nColorTheme);
	theApp.WriteInt (_T("ColorTheme"), m_nColorTheme);
}
