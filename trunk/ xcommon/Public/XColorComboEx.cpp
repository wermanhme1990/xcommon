
#include "stdafx.h"
#include "XColorComboEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXColorComboEx::CXColorComboEx()
{
	// Add the m_vColors to the array	
	m_vColors.push_back(RGB(0, 0, 0));			  // Black
	m_vColors.push_back(RGB(128, 0, 0));			// Dark Red
	m_vColors.push_back(RGB(0, 128, 0));			// Dark Green
	m_vColors.push_back(RGB(128, 128, 0));		// Dark Yellow
	m_vColors.push_back(RGB(0, 0, 128));			// Dark Blue
	m_vColors.push_back(RGB(128, 0, 128));		// Dark Magenta
	m_vColors.push_back(RGB(0, 128, 128));		// Dark Cyan	
	m_vColors.push_back(RGB(192, 192, 192));	// Light Grey
	m_vColors.push_back(RGB(128, 128, 128));	// Medium Grey
	m_vColors.push_back(RGB(255, 0, 0));			// Red
	m_vColors.push_back(RGB(0, 255, 0));			// Green
	m_vColors.push_back(RGB(255, 255, 0));		// Yellow
	m_vColors.push_back(RGB(0, 0, 255));			// Blue
	m_vColors.push_back(RGB(255, 0, 255));		// Magenta
	m_vColors.push_back(RGB(0, 255, 255));		// Cyan
	m_vColors.push_back(RGB(255, 255, 255));	// White
	m_vColors.push_back(::GetSysColor(COLOR_WINDOWTEXT));	// Automatic
}

CXColorComboEx::~CXColorComboEx()
{

}


BEGIN_MESSAGE_MAP(CXColorComboEx, CXFlatComboBox)
	//{{AFX_MSG_MAP(CXColorComboEx)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
  ON_WM_DRAWITEM_REFLECT()
  ON_WM_MEASUREITEM_REFLECT() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXColorComboEx message handlers
void CXColorComboEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{		
	DWORD dwStyle = GetStyle();	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rect(&(lpDrawItemStruct->rcItem));	
	dc.FillSolidRect( &rect, RGB(255, 255, 255)); 
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.DrawFocusRect(rect);
	}
	if(m_vColors.size() - 1 == lpDrawItemStruct->itemID)
	{
		CBrush brush(m_vColors[m_vColors.size() - 1]);				
		rect.InflateRect(-5, -2);		
		dc.FillRect(rect, &brush);		
		CString strColor = _T("Customed");
		CSize textSize = dc.GetOutputTextExtent(strColor);
		DWORD dwBKMode = dc.SetBkMode(TRANSPARENT);
		COLORREF crTextColor = dc.SetTextColor(RGB(255, 255, 255));
		dc.DrawText(strColor, rect, DT_SINGLELINE | DT_CENTER);
		dc.SetBkMode(dwBKMode);
		dc.SetTextColor(crTextColor);
	}
	else
	{
		CBrush brush(m_vColors[lpDrawItemStruct->itemID]);		
		rect.InflateRect(-5, -2);		
		dc.FillRect(rect, &brush);	
	}

	rect.InflateRect(5, 2);		
	dc.Detach();	
}
void CXColorComboEx::MeasureItem( LPMEASUREITEMSTRUCT lpMIS )
{
  
}


void CXColorComboEx::OnSelendok() 
{
	// TODO: Add your control notification handler code here
	LONG iIndex = this->GetCurSel();	
	COLORREF crColor;
	if(iIndex == m_vColors.size() - 1)
	{
		CColorDialog crDlg(RGB(255, 0, 0), CC_SOLIDCOLOR);		
		if(IDOK == crDlg.DoModal())
		{
			crColor = crDlg.GetColor();
			m_vColors[m_vColors.size() - 1] = crColor;
			Invalidate();
		}
	}	
  CWnd *pParent = GetParent();
  if (pParent)
  {
    LONG nID = this->GetDlgCtrlID();    
    pParent->SendMessage(WM_COMMAND, MAKEWORD(nID, CBN_SELENDOK), (LPARAM)this->GetSafeHwnd());
  }
}
COLORREF CXColorComboEx::GetColor()
{
	LONG nIndex = this->GetCurSel();	
	return m_vColors[nIndex];
}
void CXColorComboEx::InitColor( COLORREF crInitColor )
{
	long nSelIndex = -1;
	for (long n = 0; n < (LONG)m_vColors.size() - 1; n ++)
	{
		if (m_vColors[n] == crInitColor)
		{
			nSelIndex = n;
		}
	}
	if (nSelIndex < 0)
	{
		m_vColors[m_vColors.size() - 1] = crInitColor;
		nSelIndex = m_vColors.size() - 1;
	}
	SetCurSel(nSelIndex);
}

void CXColorComboEx::PreSubclassWindow() 
{
	for (LONG nIndex = 0; nIndex < (LONG)m_vColors.size(); nIndex ++)
	{
		AddString(_T(""));	
	}	
	SetCurSel(0);		
	 
	__super::PreSubclassWindow();	
	ModifyStyle(0, CBS_OWNERDRAWVARIABLE, SWP_DRAWFRAME | SWP_FRAMECHANGED ); 
}
