// ClockWnd.cpp : implementation file
//

#include "stdafx.h"
#include "sidebardemo.h"
#include "ClockWnd.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockWnd

CClockWnd::CClockWnd()
{
}

CClockWnd::~CClockWnd()
{
}


BEGIN_MESSAGE_MAP(CClockWnd, CWnd)
	//{{AFX_MSG_MAP(CClockWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CClockWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */) 
{
	return CWnd::CreateEx(0, NULL, _T(""), dwStyle, rect, pParentWnd, nID, pContext);
}

/////////////////////////////////////////////////////////////////////////////
// CClockWnd message handlers

void CClockWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

    // Erase the whole window.
	CRect rectClient;
	GetClientRect (rectClient);

	CBCGPVisualManager::GetInstance ()->OnFillTasksGroupInterior (pDC, rectClient);

	// Draw picture in the middle of the screen.

	// Get the size of the bitmap.
	CSize	sizeImage = m_BackImage.GetImageSize ();
	CPoint	ptImage (( rectClient.Width () - sizeImage.cx ) / 2, 0);

	CBCGPDrawState ds;
	m_BackImage.PrepareDrawImage (ds);

	m_BackImage.Draw (pDC, ptImage.x, ptImage.y, 0);
	m_BackImage.EndDrawImage (ds);

	// Draw clock's hands.
	CPoint ptC (rectClient.Width () / 2, sizeImage.cy / 2); 
	int nC = sizeImage.cy / 2;

	time_t tmCurr;
	time(&tmCurr);
	tm* pTime = localtime(&tmCurr);

	// hour hand
	CPen penHourHand;
	penHourHand.CreatePen (PS_SOLID, 4, RGB (80, 80, 80));
	CPen* pPenOld = pDC->SelectObject (&penHourHand);

	double dA = 3.1415926535897932384626433832795 / 6 * (pTime->tm_hour + (double)pTime->tm_min / 60);
	double dSinA = sin(dA);
	double dCosA = cos(dA);
	double dR = 0.5 * nC;
	pDC->MoveTo (ptC.x, ptC.y);
	pDC->LineTo (ptC.x + (int) (dSinA * dR), ptC.y - (int) (dCosA * dR));

	// minute hand
	CPen penMinHand;
	penMinHand.CreatePen (PS_SOLID, 2, RGB (80, 80, 80));
	pDC->SelectObject (&penMinHand);

	dA = 3.1415926535897932384626433832795 / 30.0 * (pTime->tm_min + (double)pTime->tm_sec / 60);
	dSinA = sin(dA);
	dCosA = cos(dA);
	dR = 0.75 * nC;
	pDC->MoveTo (ptC.x, ptC.y);
	pDC->LineTo (ptC.x + (int) (dSinA * dR), ptC.y - (int) (dCosA * dR));

	// second hand
	CPen penSecHand;
	penSecHand.CreatePen (PS_SOLID, 2, RGB (200, 80, 80));
	pDC->SelectObject (&penSecHand);

	dA = 3.1415926535897932384626433832795 / 30.0 * pTime->tm_sec;
	dSinA = sin(dA);
	dCosA = cos(dA);
	dR = 0.8 * nC;
	double dRS = 0.2 * nC;
	pDC->MoveTo (ptC.x - (int) (dSinA * dRS), ptC.y + (int) (dCosA * dRS));
	pDC->LineTo (ptC.x + (int) (dSinA * dR), ptC.y - (int) (dCosA * dR));

	CBrush brushCircle;
	brushCircle.CreateSolidBrush (RGB (80, 80, 80));
	pDC->SelectObject (&penHourHand);
	pDC->Ellipse (ptC.x - 3, ptC.y - 3, ptC.x + 3, ptC.y + 3);
		
	pDC->SelectObject (pPenOld);
}

void CClockWnd::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	if (nIDEvent == 1)
	{
		Invalidate (FALSE);
	}
}

void CClockWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	SetTimer(1, 1000, NULL);
}

BOOL CClockWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

int CClockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_BackImage.SetMapTo3DColors (FALSE);
	m_BackImage.Load (IDB_CLOCK);
	m_BackImage.SetSingleImage ();
	m_BackImage.SetTransparentColor (RGB (113, 111, 100));
	
	return 0;
}
