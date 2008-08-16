// TabbedToolbarView.cpp : implementation of the CTabbedToolbarView class
//

#include "stdafx.h"
#include "TabbedToolbar.h"

#include "TabbedToolbarDoc.h"
#include "TabbedToolbarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabbedToolbarView

IMPLEMENT_DYNCREATE(CTabbedToolbarView, CFormView)

BEGIN_MESSAGE_MAP(CTabbedToolbarView, CFormView)
	//{{AFX_MSG_MAP(CTabbedToolbarView)
	ON_BN_CLICKED(IDC_RADIO_WIN, OnVisualManager)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_OFFICEXP, OnVisualManager)
	ON_BN_CLICKED(IDC_RADIO_2003, OnVisualManager)
	ON_BN_CLICKED(IDC_RADIO_WINXP, OnVisualManager)
	ON_BN_CLICKED(IDC_RADIO_2005, OnVisualManager)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabbedToolbarView construction/destruction

CTabbedToolbarView::CTabbedToolbarView()
	: CFormView(CTabbedToolbarView::IDD)
{
	//{{AFX_DATA_INIT(CTabbedToolbarView)
	m_nVisualManager = theApp.GetInt (_T("VisualManager"), 1);
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CTabbedToolbarView::~CTabbedToolbarView()
{
}

void CTabbedToolbarView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabbedToolbarView)
	DDX_Radio(pDX, IDC_RADIO_WIN, m_nVisualManager);
	//}}AFX_DATA_MAP
}

BOOL CTabbedToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTabbedToolbarView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	OnVisualManager();
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedToolbarView printing

void CTabbedToolbarView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CTabbedToolbarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTabbedToolbarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTabbedToolbarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTabbedToolbarView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedToolbarView diagnostics

#ifdef _DEBUG
void CTabbedToolbarView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTabbedToolbarView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTabbedToolbarDoc* CTabbedToolbarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabbedToolbarDoc)));
	return (CTabbedToolbarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabbedToolbarView message handlers

void CTabbedToolbarView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CTabbedToolbarView::OnVisualManager() 
{
	UpdateData ();

	switch (m_nVisualManager)
	{
	case 0:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		return;

	case 1:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		return;

	case 2:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		return;

	case 3:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		return;
	}
}

void CTabbedToolbarView::OnDestroy() 
{
	UpdateData ();

	theApp.WriteInt (_T("VisualManager"), m_nVisualManager);

	CFormView::OnDestroy();
}
