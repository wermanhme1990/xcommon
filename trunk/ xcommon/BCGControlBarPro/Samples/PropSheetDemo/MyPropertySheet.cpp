// MyPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CBCGPPropertySheet)

CMyPropertySheet::CMyPropertySheet(CBCGPPropertySheet::PropSheetLook look,
								   UINT uiIconsResID, int cxIcon)
	 : CBCGPPropertySheet(IDS_PROPSHT_CAPTION, NULL)
{
	SetLook (look);

	if (uiIconsResID != 0)
	{
		SetIconsList (uiIconsResID, cxIcon);
	}

	if (look == PropSheetLook_Tree)
	{
		CBCGPPropSheetCategory* pCategory1 = AddTreeCategory (
			_T("Category 1"), 0, 1);
		AddPageToTree (pCategory1, &m_Page1, -1, 2);
		AddPageToTree (pCategory1, &m_Page2, -1, 2);

		CBCGPPropSheetCategory* pCategory2 = AddTreeCategory (
			_T("Category 2"), 0, 1);

		AddPageToTree (pCategory2, &m_Page3, -1, 2);
		AddPageToTree (pCategory2, &m_Page4, -1, 2);
	}
	else
	{
		AddPage(&m_Page1);
		AddPage(&m_Page2);
		AddPage(&m_Page3);
		AddPage(&m_Page4);
	}
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CBCGPPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers


void CMyPropertySheet::OnDrawPageHeader (CDC* pDC, int nPage, CRect rectHeader)
{
	rectHeader.top += 2;
	rectHeader.right -= 2;
	rectHeader.bottom -= 2;

	pDC->FillRect (rectHeader, &globalData.brBtnFace);
	pDC->Draw3dRect (rectHeader, globalData.clrBtnShadow, globalData.clrBtnShadow);

	CBCGPDrawManager dm (*pDC);
	dm.DrawShadow (rectHeader, 2);	

	CString strText;
	strText.Format (_T("Page %d description..."), nPage + 1);

	CRect rectText = rectHeader;
	rectText.DeflateRect (10, 0);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (globalData.clrBtnText);

	pDC->DrawText (strText, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
}
