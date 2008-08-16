// IconComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "popupdemo.h"
#include "IconComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox

CIconComboBox::CIconComboBox(CBCGPToolBarImages& Icons) :
	m_Icons (Icons)
{
}

CIconComboBox::~CIconComboBox()
{
}


BEGIN_MESSAGE_MAP(CIconComboBox, CComboBox)
	//{{AFX_MSG_MAP(CIconComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox message handlers

int CIconComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}

void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC= CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	HBRUSH		brBackground;
	COLORREF	clText; 

	if (lpDIS->itemState & ODS_SELECTED)  
	{
		brBackground = GetSysColorBrush (COLOR_HIGHLIGHT); 
		clText = globalData.clrTextHilite; 
	} 
	else 
	{
		brBackground = GetSysColorBrush (COLOR_WINDOW); 
		clText = globalData.clrWindowText;
	} 

	CRect rectItem = lpDIS->rcItem;
	::FillRect (lpDIS->hDC, &rectItem, brBackground);

	int nIndex = lpDIS->itemID;

	if (nIndex > 0)
	{
		CBCGPDrawState ds;
		m_Icons.SetTransparentColor (globalData.clrBarFace);
		m_Icons.PrepareDrawImage (ds);
		m_Icons.Draw (pDC, rectItem.left + 1, rectItem.top + 1, nIndex - 1);
		m_Icons.EndDrawImage (ds);
	}

	CString strText;
	GetLBText (nIndex, strText);

	CRect rectText = rectItem;
	rectText.left += 20;

	CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (clText);

	pDC->DrawText (strText, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
}

void CIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	lpMIS->itemHeight = 18;
	lpMIS->itemWidth = 18;
}
