// ToolBoxDemoView.cpp : implementation of the CToolBoxDemoView class
//

#include "stdafx.h"
#include "ToolBoxDemo.h"

#include "ToolBoxDemoDoc.h"
#include "ToolBoxDemoView.h"

#include "MainFrm.h"
#include "AddButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView

IMPLEMENT_DYNCREATE(CToolBoxDemoView, CFormView)

BEGIN_MESSAGE_MAP(CToolBoxDemoView, CFormView)
	//{{AFX_MSG_MAP(CToolBoxDemoView)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_CBN_SELCHANGE(IDC_PAGE_LAYOUT, OnSelchangePageLayout)
	ON_CBN_SELCHANGE(IDC_TOOLBOX_MODE, OnSelchangeToolboxMode)
	ON_CBN_SELCHANGE(IDC_TOOLBOX_THEME, OnSelchangeToolboxTheme)
	ON_BN_CLICKED(IDC_CLEAR_LIST, OnClearList)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView construction/destruction

CToolBoxDemoView::CToolBoxDemoView()
	: CFormView(CToolBoxDemoView::IDD)
{
	//{{AFX_DATA_INIT(CToolBoxDemoView)
	m_nPageLayout = theApp.GetInt (_T("PageLayout"), 0);
	m_nToolBoxMode = theApp.GetInt (_T("ToolBoxMode"), 0);
	m_nTheme = 0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CToolBoxDemoView::~CToolBoxDemoView()
{
	theApp.WriteInt (_T("PageLayout"), m_nPageLayout);
	theApp.WriteInt (_T("ToolBoxMode"), m_nToolBoxMode);
}

void CToolBoxDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBoxDemoView)
	DDX_Control(pDX, IDC_TBLIST, m_TBListCtrl);
	DDX_CBIndex(pDX, IDC_PAGE_LAYOUT, m_nPageLayout);
	DDX_CBIndex(pDX, IDC_TOOLBOX_MODE, m_nToolBoxMode);
	DDX_CBIndex(pDX, IDC_TOOLBOX_THEME, m_nTheme);
	//}}AFX_DATA_MAP
}

BOOL CToolBoxDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CToolBoxDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	m_nTheme = ((CMainFrame*) GetTopLevelFrame ())->m_nAppLook - ID_VIEW_APPLOOK_2000;
	UpdateData (FALSE);

	OnSelchangeToolboxMode();
	OnSelchangePageLayout();

	CRect rtClient;
	m_TBListCtrl.GetClientRect (rtClient);

	m_TBListCtrl.InsertColumn (0, _T("Name"), LVCFMT_LEFT, rtClient.Width () * 2 / 3);
	m_TBListCtrl.InsertColumn (1, _T("Page"), LVCFMT_LEFT, rtClient.Width () / 6);
	m_TBListCtrl.InsertColumn (2, _T("Tool"), LVCFMT_LEFT, rtClient.Width () / 6);
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView printing

void CToolBoxDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CToolBoxDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToolBoxDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToolBoxDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CToolBoxDemoView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView diagnostics

#ifdef _DEBUG
void CToolBoxDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CToolBoxDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CToolBoxDemoDoc* CToolBoxDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBoxDemoDoc)));
	return (CToolBoxDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView message handlers

void CToolBoxDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CToolBoxDemoView::OnAddButton() 
{
	CAddButtonDlg dlg (this);
	if (dlg.DoModal () != IDOK)
	{
		return;
	}

	const int nPage = dlg.m_nPage - 1;
	const CString strLabel = dlg.m_strLabel;
	HICON hIcon = dlg.GetSelIcon ();

	CBCGPToolBoxPage* pPage = ((CMainFrame*) GetTopLevelFrame ())->GetToolBoxPage (nPage);
	if (pPage == NULL)
	{
		MessageBox (_T("Cannot find toolbox page!"));
	}
	else
	{
		ASSERT_VALID (pPage);
		pPage->AddButton (strLabel, hIcon);
	}

	::DestroyIcon (hIcon);
}

void CToolBoxDemoView::OnSelchangePageLayout() 
{
	UpdateData ();
	((CMainFrame*) GetTopLevelFrame ())->SetToolBoxLayout (m_nPageLayout);
}

void CToolBoxDemoView::OnSelchangeToolboxMode() 
{
	UpdateData ();
	((CMainFrame*) GetTopLevelFrame ())->SetToolBoxMode (m_nToolBoxMode);
}

void CToolBoxDemoView::OnSelchangeToolboxTheme() 
{
	UpdateData ();
	((CMainFrame*) GetTopLevelFrame ())->OnAppLook (m_nTheme + ID_VIEW_APPLOOK_2000);
}

void CToolBoxDemoView::OnClearList() 
{
	m_TBListCtrl.DeleteAllItems ();
}
