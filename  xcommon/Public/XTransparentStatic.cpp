/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XTransparentStatic.h					
文件内容:			透明CStatic
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/05
*********************************************************************/
#include "stdafx.h"
#include "XTransparentStatic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CXTransparentStatic, CStatic)

CXTransparentStatic::CXTransparentStatic()
{
	//{{AFX_DATA_INIT(CXTransparentStatic)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bAdjustToFit = TRUE;
}

CXTransparentStatic::~CXTransparentStatic()
{
}

void CXTransparentStatic::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTransparentStatic)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CXTransparentStatic, CStatic)
	//{{AFX_MSG_MAP(CXTransparentStatic)
	ON_WM_CTLCOLOR_REFLECT()
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()


HBRUSH CXTransparentStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hBrush;	
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	_ASSERT(pDC);
	pDC->SetBkMode(TRANSPARENT);
	return hBrush;
}
BOOL CXTransparentStatic::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID /*= 0xffff */ )
{
	dwStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;	
	return __super::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

void CXTransparentStatic::PositionWindow()
{
	if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit) 
		return;

	// Get the current window position
	CRect rect;
	GetWindowRect(rect);

	CWnd* pParent = GetParent();
	if (pParent)
	{
		pParent->ScreenToClient(rect);
	}

	// Get the size of the window text
	CString strWndText;
	GetWindowText(strWndText);

	CDC* pDC = GetDC();
	//CFont* pOldFont = pDC->SelectObject(&m_Font);
	CSize Extent = pDC->GetTextExtent(strWndText);
	//pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// Get the text justification via the window style
	DWORD dwStyle = GetStyle();

	// Recalc the window size and position based on the text justification
	if (dwStyle & SS_CENTERIMAGE)
	{
		rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
	}
	else
	{
		rect.bottom = rect.top + Extent.cy;
	}

	if (dwStyle & SS_CENTER)   
	{
		rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
	}
	else if (dwStyle & SS_RIGHT) 
	{
		rect.left  = rect.right - Extent.cx;
	}
	else // SS_LEFT = 0, so we can't test for it explicitly 
	{
		rect.right = rect.left + Extent.cx;
	}
	// Move the window
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}
