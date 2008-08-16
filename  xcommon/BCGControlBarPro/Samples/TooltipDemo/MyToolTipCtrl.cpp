// MyToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TooltipDemo.h"
#include "MyToolTipCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolTipCtrl

IMPLEMENT_DYNCREATE(CMyToolTipCtrl, CBCGPToolTipCtrl)

CMyToolTipCtrl::CMyToolTipCtrl()
{
	m_hIcon = (HICON) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE (IDI_ICON1),
			IMAGE_ICON,
			globalData.m_sizeSmallIcon.cx,
			globalData.m_sizeSmallIcon.cy,
			LR_SHARED);
}

CMyToolTipCtrl::~CMyToolTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyToolTipCtrl, CBCGPToolTipCtrl)
	//{{AFX_MSG_MAP(CMyToolTipCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolTipCtrl message handlers

int CMyToolTipCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Override default parameters:
	m_Params.m_bRoundedCorners = TRUE;
	
	return 0;
}

CSize CMyToolTipCtrl::GetIconSize ()
{
	if (m_pHotButton == NULL)
	{
		return CSize (0, 0);
	}

	ASSERT_VALID (m_pHotButton);

	if (m_pHotButton->m_nID == 0 || 
		m_pHotButton->m_nID == (UINT)-1)
	{
		return CSize (0, 0);
	}

	return globalData.m_sizeSmallIcon;
}

void CMyToolTipCtrl::SetDescription (const CString strDesrciption)
{
	CBCGPToolTipCtrl::SetDescription (strDesrciption);

	if (m_pHotButton == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pHotButton);

	if (m_pHotButton->m_nID == 0 || 
		m_pHotButton->m_nID == (UINT)-1)
	{
		return;
	}

	m_strDesrciption.Format (
		_T("This is a custom description text for toolbar/menu button with ID = %d.."),
		m_pHotButton->m_nID);
}

void CMyToolTipCtrl::OnFillBackground (CDC* pDC, CRect rect, COLORREF& clrText, COLORREF& clrLine)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient2 (rect, 
		RGB (104, 165, 225), RGB (37, 92, 222), 45);

	clrLine = RGB (113, 166, 246);
	clrText = RGB (255, 255, 255);
}

BOOL CMyToolTipCtrl::OnDrawIcon (CDC* pDC, CRect rectImage)
{
	if (m_pHotButton == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pHotButton);

	if (m_pHotButton->m_nID == 0 || 
		m_pHotButton->m_nID == (UINT)-1)
	{
		return FALSE;
	}

	ASSERT_VALID (pDC);

	::DrawIconEx (pDC->GetSafeHdc (),
		rectImage.left, rectImage.top, m_hIcon,
		0, 0, 0, NULL, DI_NORMAL);

	return TRUE;
}

void CMyToolTipCtrl::OnDrawSeparator (CDC* pDC, int x1, int x2, int y)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	CRect rect (x1, y, x2, y + 1);

	dm.FillGradient (rect, 
		RGB (255, 201, 109), RGB (255, 247, 225), FALSE);
}
