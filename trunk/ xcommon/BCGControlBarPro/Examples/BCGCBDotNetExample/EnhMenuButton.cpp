// EnhMenuButton.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbdotnetexample.h"
#include "EnhMenuButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnhMenuButton

CEnhMenuButton::CEnhMenuButton()
{
}

CEnhMenuButton::~CEnhMenuButton()
{
}


BEGIN_MESSAGE_MAP(CEnhMenuButton, CBCGPMenuButton)
	//{{AFX_MSG_MAP(CEnhMenuButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnhMenuButton message handlers

extern const int nImageHorzMargin = 10;

void CEnhMenuButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	ASSERT_VALID (pDC);

	CSize sizeArrow = CMenuImages::Size ();

	CRect rectParent = rect;
	rectParent.right -= sizeArrow.cx + nImageHorzMargin;

	CBCGPButton::OnDraw (pDC, rectParent, uiState);

	CRect rectArrow = rect;
	rectArrow.left = rectParent.right;

	CPoint pointImage (
		rectArrow.left + (rectArrow.Width () - sizeArrow.cx) / 2,
		rectArrow.top + (rectArrow.Height () - sizeArrow.cy) / 2);

	if (pointImage.x < rect.left) {
		CRect rc = rect;
		LONG m = min(rc.left - pointImage.x, rc.right - (pointImage.x + sizeArrow.cx));
		pointImage.x += m;

// Stiles!
//		if (pointImage.x == rect.left)
//			pointImage.x = (rc.left   +  rc.right - sizeArrow.cx) >> 1;
	}

	CMenuImages::Draw (pDC, 
		m_bRightArrow ? CMenuImages::IdArowRightLarge : CMenuImages::IdArowDownLarge, 
		pointImage,
		(uiState & ODS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
}
