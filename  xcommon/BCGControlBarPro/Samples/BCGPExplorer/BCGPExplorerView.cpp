// BCGPExplorerView.cpp : implementation of the CBCGPExplorerView class
//

#include "stdafx.h"
#include "BCGPExplorer.h"

#include "BCGPExplorerDoc.h"
#include "BCGPExplorerView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerView

IMPLEMENT_DYNCREATE(CBCGPExplorerView, CView)

BEGIN_MESSAGE_MAP(CBCGPExplorerView, CView)
	//{{AFX_MSG_MAP(CBCGPExplorerView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_LARGEICON, OnViewLargeicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LARGEICON, OnUpdateViewLargeicon)
	ON_COMMAND(ID_VIEW_DETAILS, OnViewDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, OnUpdateViewDetails)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, OnUpdateViewList)
	ON_COMMAND(ID_VIEW_SMALLICON, OnViewSmallicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLICON, OnUpdateViewSmallicon)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FOLDER_UP, OnFolderUp)
	ON_UPDATE_COMMAND_UI(ID_FOLDER_UP, OnUpdateFolderUp)
	ON_COMMAND(ID_COPY_TO, OnCopyTo)
	ON_COMMAND(ID_MOVE_TO, OnMoveTo)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGPM_CHANGE_CURRENT_FOLDER, OnChangeFolder)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerView construction/destruction

CBCGPExplorerView::CBCGPExplorerView()
{
}

CBCGPExplorerView::~CBCGPExplorerView()
{
}

BOOL CBCGPExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerView drawing

void CBCGPExplorerView::OnDraw(CDC* pDC)
{
}

void CBCGPExplorerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	OnChangeFolder (0, 0);	// To update mainframe's address bar
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerView diagnostics

#ifdef _DEBUG
void CBCGPExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CBCGPExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBCGPExplorerDoc* CBCGPExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPExplorerDoc)));
	return (CBCGPExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerView message handlers

int CBCGPExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy (0, 0, 0, 0);
	m_wndList.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 1);
	
	return 0;
}

void CBCGPExplorerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_wndList.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CBCGPExplorerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate && AfxGetMainWnd () != NULL)
	{
		((CMainFrame*) AfxGetMainWnd ())->GetFolders ().SetRelatedList (&m_wndList);
	}
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CBCGPExplorerView::OnViewLargeicon() 
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

void CBCGPExplorerView::OnUpdateViewLargeicon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}

void CBCGPExplorerView::OnViewDetails() 
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

void CBCGPExplorerView::OnUpdateViewDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}

void CBCGPExplorerView::OnViewList() 
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}

void CBCGPExplorerView::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}

void CBCGPExplorerView::OnViewSmallicon() 
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

void CBCGPExplorerView::OnUpdateViewSmallicon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}

BOOL CBCGPExplorerView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPExplorerView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	m_wndList.SetFocus ();
}

LRESULT CBCGPExplorerView::OnChangeFolder(WPARAM,LPARAM)
{
	CString strPath;
	if (!m_wndList.GetCurrentFolder (strPath) &&
		!m_wndList.GetCurrentFolderName (strPath))
	{
		strPath = _T("????");
	}

	if (AfxGetMainWnd () != NULL)
	{
		((CMainFrame*) AfxGetMainWnd ())->SetCurrFolder (strPath);
	}

	return 0;
}

void CBCGPExplorerView::OnFolderUp() 
{
	m_wndList.DisplayParentFolder ();
}

void CBCGPExplorerView::OnUpdateFolderUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (!m_wndList.IsDesktop ());
}

void CBCGPExplorerView::OnCopyTo() 
{
	CString strPath;
	if (m_wndList.GetCurrentFolder (strPath) &&
		theApp.GetShellManager ()->BrowseForFolder (strPath, 
			this, strPath, _T("Copy the selected item(s) to the folder:")))
	{
		MessageBox (CString (_T("The selected path is: ")) + strPath);
	}
}

void CBCGPExplorerView::OnMoveTo() 
{
	CString strPath;
	if (m_wndList.GetCurrentFolder (strPath) &&
		theApp.GetShellManager ()->BrowseForFolder (strPath, 
			this, strPath, _T("Move the selected item(s) to the folder:")))
	{
		MessageBox (CString (_T("The selected path is: ")) + strPath);
	}
}

void CBCGPExplorerView::OnViewRefresh() 
{
	m_wndList.Refresh ();
}
