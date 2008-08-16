// SetBarSizeView.cpp : implementation of the CSetBarSizeView class
//

#include "stdafx.h"
#include "SetBarSize.h"

#include "SetBarSizeDoc.h"
#include "SetBarSizeView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeView

IMPLEMENT_DYNCREATE(CSetBarSizeView, CFormView)

BEGIN_MESSAGE_MAP(CSetBarSizeView, CFormView)
	//{{AFX_MSG_MAP(CSetBarSizeView)
	ON_BN_CLICKED(IDC_BUTTON_SET_DLGBAR_SIZE, OnButtonSetDlgBarSize)
	ON_BN_CLICKED(IDC_BUTTON_SET_CONTAINER_SIZE, OnButtonSetContainerSize)
	ON_BN_CLICKED(IDC_BTN_SET_HEIGHT_IN_PIXELS, OnBtnSetHeightInPixels)
	ON_BN_CLICKED(IDC_BTN_SET_WIDTH_IN_PIXELS, OnBtnSetWidthInPixels)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeView construction/destruction

CSetBarSizeView::CSetBarSizeView()
	: CFormView(CSetBarSizeView::IDD)
{
	//{{AFX_DATA_INIT(CSetBarSizeView)
	m_nSizePercent = 50;
	m_nContainerSize = 100;
	m_nHeightInPixels = 100;
	m_nWidthInPixels = 100;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CSetBarSizeView::~CSetBarSizeView()
{
}

void CSetBarSizeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBarSizeView)
	DDX_Text(pDX, IDC_EDIT_SIZE, m_nSizePercent);
	DDV_MinMaxUInt(pDX, m_nSizePercent, 1, 99);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nContainerSize);
	DDV_MinMaxUInt(pDX, m_nContainerSize, 1, 2056);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_IN_PIXELS, m_nHeightInPixels);
	DDV_MinMaxUInt(pDX, m_nHeightInPixels, 1, 2056);
	DDX_Text(pDX, IDC_EDIT_WIDTH_IN_PIXELS, m_nWidthInPixels);
	DDV_MinMaxUInt(pDX, m_nWidthInPixels, 1, 2056);
	//}}AFX_DATA_MAP
}

BOOL CSetBarSizeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSetBarSizeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeView printing

void CSetBarSizeView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CSetBarSizeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSetBarSizeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSetBarSizeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSetBarSizeView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeView diagnostics

#ifdef _DEBUG
void CSetBarSizeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSetBarSizeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSetBarSizeDoc* CSetBarSizeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSetBarSizeDoc)));
	return (CSetBarSizeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeView message handlers

void CSetBarSizeView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CSetBarSizeView::OnButtonSetDlgBarSize() 
{
	UpdateData ();
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, GetTopLevelFrame());
	if (pMainFrame != NULL)
	{
		pMainFrame->SetDlgBarSizeInContainer (m_nSizePercent);
	}
}

void CSetBarSizeView::OnButtonSetContainerSize() 
{
	UpdateData ();	
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, GetTopLevelFrame());
	if (pMainFrame != NULL)
	{
		pMainFrame->SetContainerSize (m_nContainerSize);
	}
}

void CSetBarSizeView::OnBtnSetHeightInPixels() 
{
	UpdateData ();	
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, GetTopLevelFrame());
	if (pMainFrame != NULL)
	{
		pMainFrame->SetDlgBarHeightInPixels (m_nHeightInPixels);
	}
}

void CSetBarSizeView::OnBtnSetWidthInPixels() 
{
	UpdateData ();	
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, GetTopLevelFrame());
	if (pMainFrame != NULL)
	{
		pMainFrame->SetDlgBarWidthInPixels (m_nWidthInPixels);
	}
}
