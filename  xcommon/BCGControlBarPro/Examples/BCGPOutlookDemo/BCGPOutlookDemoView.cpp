// BCGPOutlookDemoView.cpp : implementation of the CBCGPOutlookDemoView class
//

#include "stdafx.h"
#include "BCGPOutlookDemo.h"

#include "BCGPOutlookDemoDoc.h"
#include "BCGPOutlookDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView

IMPLEMENT_DYNCREATE(CBCGPOutlookDemoView, CListView)

BEGIN_MESSAGE_MAP(CBCGPOutlookDemoView, CListView)
	//{{AFX_MSG_MAP(CBCGPOutlookDemoView)
	ON_COMMAND(ID_VIEW_PREVIEW_PANE, OnViewPreviewPane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIEW_PANE, OnUpdateViewPreviewPane)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView construction/destruction

CBCGPOutlookDemoView::CBCGPOutlookDemoView()
{
	// TODO: add construction code here

}

CBCGPOutlookDemoView::~CBCGPOutlookDemoView()
{
}

BOOL CBCGPOutlookDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView drawing

void CBCGPOutlookDemoView::OnDraw(CDC* pDC)
{
	CBCGPOutlookDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CBCGPOutlookDemoView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& wndList = GetListCtrl ();
	wndList.DeleteAllItems ();

	wndList.InsertItem (0, _T("BCGSoft"), 1);
	wndList.SetItemText (0, 1, _T("Welcome to BCGControlBar Pro!"));
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView printing

void CBCGPOutlookDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CBCGPOutlookDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCGPOutlookDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBCGPOutlookDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView diagnostics

#ifdef _DEBUG
void CBCGPOutlookDemoView::AssertValid() const
{
	CListView::AssertValid();
}

void CBCGPOutlookDemoView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CBCGPOutlookDemoDoc* CBCGPOutlookDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPOutlookDemoDoc)));
	return (CBCGPOutlookDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookDemoView message handlers

void CBCGPOutlookDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CBCGPOutlookDemoView::OnViewPreviewPane() 
{
	// TODO: Add your command handler code here
	
}

void CBCGPOutlookDemoView::OnUpdateViewPreviewPane(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

int CBCGPOutlookDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	const int nImageWidth = 12;

	m_ImagesHeader.Create (IDB_HEADER_IMAGES, nImageWidth, 0, RGB (255, 0, 255));
	m_ImagesList.Create (IDB_LIST_IMAGES, 14, 0, RGB (255, 0, 255));

	CListCtrl& wndList = GetListCtrl ();
	wndList.SetExtendedStyle (LVS_EX_FULLROWSELECT);

	wndList.SetImageList (&m_ImagesList, LVSIL_SMALL);

	wndList.InsertColumn (0, _T("From"), LVCFMT_LEFT, 100);
	wndList.InsertColumn (1, _T("Subject"), LVCFMT_LEFT, 200);

	m_wndHeader.SubclassDlgItem (0, this);
	m_wndHeader.SetImageList (&m_ImagesHeader);

	LVCOLUMN lvColumn;
	memset (&lvColumn, 0, sizeof (LVCOLUMN));
	lvColumn.mask = LVCF_IMAGE | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_COL_HAS_IMAGES;
	lvColumn.cx = nImageWidth * 2;

	lvColumn.iImage = 0;
	wndList.InsertColumn (2, &lvColumn);

	lvColumn.iImage = 1;
	wndList.InsertColumn (3, &lvColumn);

	lvColumn.iImage = 2;
	wndList.InsertColumn (4, &lvColumn);

	static int nColumsOrder [] = { 2, 3, 4, 0, 1 };
	wndList.SetColumnOrderArray (sizeof (nColumsOrder) / sizeof (int), nColumsOrder);

	return 0;
}
