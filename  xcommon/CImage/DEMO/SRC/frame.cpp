// frame.cpp : implementation of the CFrame class
//

#include "stdafx.h"
#include "app.h"

#include "frame.h"
#include "imgdoc.h"

//#include "..\cimage\include\cimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrame

IMPLEMENT_DYNAMIC(CFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrame construction/destruction
CFrame::CFrame()
{ }

CFrame::~CFrame()
{ }

/////////////////////////////////////////////////////////////////////////////
int CFrame::OnCreate(LPCREATESTRUCT lpcs)
{
   static const UINT indicators[] =
   {
	   ID_SEPARATOR,           // status line indicator
	   ID_INDICATOR1,
	   ID_INDICATOR2,
	   ID_INDICATOR3,
	   ID_INDICATOR_CAPS,
	   ID_INDICATOR_NUM,
	   ID_INDICATOR_SCRL,
   };

	if (CMDIFrameWnd::OnCreate(lpcs) == -1)
		return -1;
	
	if (!(m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_RIGHT) && m_wndToolBar.LoadToolBar(CFrame::IDR)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!(m_wndStatusBar.Create(this) && m_wndStatusBar.SetIndicators(indicators, _countof(indicators))))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
   m_wndToolBar.SetWindowText(_T("Standard"));

	return 0;
}

BOOL CFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFrame diagnostics
#ifdef _DEBUG
void CFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrame message handlers

void CFrame::OnEditPaste() 
{
   CImageDoc* pDoc = (CImageDoc*)CApp::GetApp().demoTemplate->OpenDocumentFile(NULL); 
   BOOL bOK = (pDoc != NULL);
	if (bOK)
   {
      bOK = OpenClipboard();
      if (bOK)
      {
			CImage((HBITMAP)::GetClipboardData(CF_BITMAP)).Detach(&pDoc->m_image);
         ::CloseClipboard();

		   CString str;
		   str.Format(_T("Clipboard Image %d"),CApp::GetApp().m_nDocCount++);
		   pDoc->SetTitle(str);
         pDoc->UpdateAllViews(NULL, CImageDoc::ENUM_hint_newimage);
		   
		   str.Format(_T("(%dx%dx%d)"),pDoc->GetImage().GetWidth(),pDoc->GetImage().GetHeight(),pDoc->GetImage().GetBPP());
		   CStatusBar& statusBar = GetStatusBar();
		   statusBar.SetPaneText(2, str);
      }
	}
}

void CFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}

BOOL CFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	BOOL bOK = CMDIFrameWnd::OnCreateClient(lpcs, pContext);
   if (bOK && 1)
   {  
      bOK = m_wndMDIFace.SubclassWindow(m_hWndMDIClient); 
	} 
   return bOK;
}


void CFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
   if (bPreview)
   {
		m_hAccelTable = NULL;
		LoadAccelTable(MAKEINTRESOURCE(LMFC_IDR_PREVIEW));
   }
}

BOOL CFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	BOOL bOK = CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
   if (bOK)
   {
      CString str = GetTitle();
#ifdef _UNICODE
      str+=_T(" - Unicode");
#endif
#ifdef _DEBUG
      str+=_T(" - Debug");
#endif
      SetTitle(str);
      SetWindowText(str);
   }
   return bOK;
}
