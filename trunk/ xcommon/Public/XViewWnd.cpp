/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XViewWnd.h					
文件内容:		将view转换为wnd一样使用
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/04/05
*********************************************************************/
#include "stdafx.h"
#include "XViewWnd.h"
#include <vector>
#include <algorithm>
using namespace std;
using namespace Gdiplus;
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXViewWnd
CXString CXViewWnd:: m_strClassName = TEXT("HWX_VIEWWND");
CXString CXViewWnd:: m_strDefWndName = TEXT("HWX_RENDER");

IMPLEMENT_DYNCREATE(CXViewWnd, CScrollView)

BEGIN_MESSAGE_MAP(CXViewWnd, CScrollView)
	//{{AFX_MSG_MAP(CXViewWnd)		  
	ON_WM_SIZE()	
	ON_WM_ERASEBKGND()		
	ON_WM_SETFOCUS()	
	ON_WM_DESTROY()
	ON_WM_CREATE() 
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
  ON_WM_MOUSEACTIVATE()
  ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXViewWnd construction/destruction

CXViewWnd::CXViewWnd()
{
	m_bActive = TRUE;//FALSE; 
}

CXViewWnd::~CXViewWnd()
{
  
}
BOOL CXViewWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXViewWnd drawing

void CXViewWnd::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	__super::OnPrepareDC(pDC, pInfo); 
}

BOOL CXViewWnd::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// do the scroll
	if (!__super::OnScrollBy(sizeScroll, bDoScroll))
		return FALSE;

	// update the position of any in-place active item
	if (bDoScroll)
	{		
		UpdateWindow();
	}
	return TRUE;
}

void CXViewWnd::OnDraw(CDC* pDC)
{  
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = 0;
	// only paint the rect that needs repainting
  CRect client;
	pDC->GetClipBox(client);
  CRect rect = client; 
  DocToClient(rect); 
  if (dc.CreateCompatibleDC(pDC))
  {
    if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
    {
      OnPrepareDC(&dc, NULL);
      pDrawDC = &dc;      
      dc.OffsetViewportOrg(-rect.left, -rect.top);
      pOldBitmap = dc.SelectObject(&bitmap);
      dc.SetBrushOrg(rect.left % 8, rect.top % 8);     
      dc.IntersectClipRect(client);
    }
  }	
	
	Draw(pDrawDC->GetSafeHdc());


  if (pDrawDC != pDC)
  {
    pDC->SetViewportOrg(0, 0);
    pDC->SetWindowOrg(0, 0);
    pDC->SetMapMode(MM_TEXT);
    dc.SetViewportOrg(0, 0);
    dc.SetWindowOrg(0, 0);
    dc.SetMapMode(MM_TEXT);
    pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
      &dc, 0, 0, SRCCOPY);
    dc.SelectObject(pOldBitmap);
  }	
}
void CXViewWnd::Draw( HDC pDC)
{

}
void CXViewWnd::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, m_szPaper);
}

void CXViewWnd::SetPageSize(CSize size)
{	
	SetScrollSizes(MM_TEXT, size);
  m_szPaper = size;
}
/////////////////////////////////////////////////////////////////////////////
// CXViewWnd printing

BOOL CXViewWnd::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXViewWnd::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnBeginPrinting(pDC,pInfo);

}

void CXViewWnd::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CXViewWnd::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CXViewWnd::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);  
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);	
}

void CXViewWnd::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CXViewWnd::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}




BOOL CXViewWnd::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXViewWnd::OnFilePrint()
{
	CScrollView::OnFilePrint();
	//GetDocument()->ComputePageSize();
}

////////////////////////////////////////////////////////////////////////////
// CXViewWnd diagnostics

#ifdef _DEBUG
void CXViewWnd::AssertValid() const
{
	CScrollView::AssertValid();
}

void CXViewWnd::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

BOOL CXViewWnd::RegisterWndClass()
{  
  WNDCLASS tWC;
  HINSTANCE hInst = AfxGetInstanceHandle();
  if (!(::GetClassInfo(hInst, m_strClassName, &tWC)))
  {    
    tWC.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    tWC.lpfnWndProc = ::DefWindowProc;
    tWC.cbClsExtra = tWC.cbWndExtra = 0;
    tWC.hInstance = hInst;
    tWC.hIcon = NULL;
    tWC.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
    tWC.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
    tWC.lpszMenuName = NULL;
    tWC.lpszClassName = m_strClassName;
    if (!AfxRegisterClass(&tWC))
    {
      AfxThrowResourceException();
      return FALSE;
    }
  }
  return TRUE;
}
BOOL CXViewWnd::Create(CWnd * pParent, const CRect &rect, const CSize &szPaper, 
                       DWORD dwStyple, DWORD dwStypleEx, LPCTSTR pszWndName)
{
  RegisterWndClass();
  if (szPaper == CSize(0, 0))
  {
    m_szPaper.cx = rect.Width();
    m_szPaper.cy = rect.Height();
  }
  else
  {
    m_szPaper = szPaper;
  }  
  if (!pszWndName)
  {
    pszWndName = m_strDefWndName;
  }
  BOOL blCreated = __super::Create(m_strClassName, pszWndName, dwStyple | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
    rect, pParent, 0); 
  if (blCreated)
  {
    SetPageSize(szPaper);
  }
  return  blCreated;
}
BOOL CXViewWnd::Create(HWND hParent, const CRect &rect, const CSize &szPaper, 
                       DWORD dwStyple, DWORD dwStypleEx, LPCTSTR pszWndName)
{
  return Create(CWnd::FromHandle(hParent), rect, szPaper, dwStyple, dwStypleEx);  
}
void CXViewWnd::PostNcDestroy()
{
  // TODO: 在此添加专用代码和/或调用基类
  //CScrollView::PostNcDestroy();
}
CSize CXViewWnd::GetPaperSize()
{
  return m_szPaper;
}
int CXViewWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  LONG nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
  OnActivateView(TRUE, this, NULL);  
  return nResult;
}


CXStringT CXViewWnd::GetWndClass()
{
  RegisterWndClass();
  return m_strClassName;
}

BOOL CXViewWnd::Create(LPCTSTR lpszClassName,
                    LPCTSTR lpszWindowName, DWORD dwStyle,
                    const RECT& rect,
                    CWnd* pParentWnd, UINT nID,
                    CCreateContext* pContext)
{  
  
  return CreateEx(0, lpszClassName, lpszWindowName,
    dwStyle | WS_CHILD,
    rect, pParentWnd, nID, pContext);
}



BOOL CXViewWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
                      LPCTSTR lpszWindowName, DWORD dwStyle,
                      const RECT& rect,
                      CWnd* pParentWnd, UINT nID,
                      LPVOID lpParam)
{
  if (!lpszClassName)
  {
    return Create(pParentWnd, rect, CSize(_abs(rect.right - rect.left), _abs(rect.bottom - rect.top)),
      dwStyle, dwExStyle, lpszWindowName);    
  }
  else
  {
    return __super::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
      rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
      pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID, lpParam);
  } 
}

void CXViewWnd::OnSize(UINT nType, int cx, int cy)
{  
  __super::OnSize(nType, cx, cy);	
  //m_szPaper = CSize(cx, cy);
}


void CXViewWnd::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	// TODO: 在此添加专用代码和/或调用基类
	__super::OnActivateFrame(nState, pDeactivateFrame);	
}

void CXViewWnd::OnActivateView(BOOL bActivate, CView* pActiveView,
															 CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);

	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if (bActivate)  // if becoming active update as if active
			m_bActive = bActivate;
		m_bActive = bActivate;
	}
}
