

#include "stdafx.h"
#include "Resource.h"
#include "XFontCombo.h"
#include "assert.h"
#include <XBitmap.h>
#include <algorithm>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXFontComboBox, CXFlatComboBox)

CXFontComboBox::CXFontComboBox()
{
	m_bFlatLook = TRUE;
	m_csSymbol  = _T("AaBbCc");
	m_dwStyle   = xtFontBoth;
	m_FontDefaultGUI = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  m_listFonts.clear();
	m_hIconFontType = NULL;
  InitFontLst();
}

CXFontComboBox::~CXFontComboBox()
{
	if (m_hIconFontType)
	{
		DestroyIcon(m_hIconFontType);
		m_hIconFontType = NULL;
	}
}
BEGIN_MESSAGE_MAP(CXFontComboBox, CXFlatComboBox)
	//{{AFX_MSG_MAP(CXFontComboBox)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	UINT  itemState = lpDIS->itemState;
	UINT  itemID    = lpDIS->itemID;
	CRect rcItem    = lpDIS->rcItem;

	if (itemID == (UINT)-1)
	{
		return;
	}

	//CXLogFont* pLF = (CXLogFont*)lpDIS->itemData;
  list<CXLogFont>::const_iterator itLstTravel = m_listFonts.begin();
  while (itLstTravel != m_listFonts.end() && itemID --)
  {
    itLstTravel ++;
  }  
	if (itLstTravel == m_listFonts.end())
	{
		return;
	}
  CXLogFont xLogFont(*itLstTravel);
  BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bFocus    = ((itemState & ODS_FOCUS)    == ODS_FOCUS);

	// draw background.
	if (bDisabled)
	{
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
		pDC->FillSolidRect(&rcItem, GetSysColor(COLOR_3DFACE));
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetSysColor(COLOR_WINDOW));
		}

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rcItem);
		}
	}

	CString csFaceName = xLogFont.lfFaceName;
	CRect rcText(rcItem);	
	if (m_hIconFontType)
	{
		if (xLogFont.dwType & TRUETYPE_FONTTYPE)
		{				
			DrawIconEx(pDC->m_hDC, rcItem.left, rcText.top, m_hIconFontType,	
				rcText.Height()	, rcText.Height(),	0,NULL, DI_NORMAL);
		}
		rcText.left += 16 + 2;
	}			
  rcText.DeflateRect(2, 0);
	pDC->SetBkMode(TRANSPARENT);

	if (m_dwStyle & xtFontGUI)
	{
		HFONT tFontOld = (HFONT)SelectObject(pDC->m_hDC, m_FontDefaultGUI);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
		SelectObject(pDC->m_hDC, tFontOld);
		tFontOld = NULL;
	}
	if (m_dwStyle & xtFontSample)
	{
		if (m_dwStyle & xtFontGUI)
		{
			rcText.left = rcText.left + GetMaxWidth() + 5;
		}

		if ((m_dwStyle & xtFontGUI) || (xLogFont.lfCharSet == SYMBOL_CHARSET))
		{
			csFaceName = m_csSymbol;
		}
	
		// Draw the text item.
    /*
      xLogFont.lfHeight = MulDiv(rcText.Height() - 10,
            GetDeviceCaps(pDC->m_hDC, LOGPIXELSY),       
            72);//rcText.Height() - 2;
      */
		xLogFont.lfHeight = rcText.Height() - 2;    
		HFONT tFont =	CreateFontIndirect(&xLogFont), hFontOld = NULL;
		hFontOld = (HFONT)SelectObject(pDC->m_hDC, tFont);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
		SelectObject(pDC->m_hDC, hFontOld);
		DeleteObject(tFont);
		tFont = NULL;
	}  
}

/////////////////////////////////////////////////////////////////////////////
// CXFontComboBox diagnostics

#ifdef _DEBUG
void CXFontComboBox::AssertValid() const
{
	__super::AssertValid();

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	// combo box is owner drawn, and must be created with the
	// following styles:

	ASSERT(dwStyle & CBS_OWNERDRAWFIXED);
	ASSERT(dwStyle & CBS_DROPDOWN);
	//ASSERT(dwStyle & CBS_SORT);
	//ASSERT(dwStyle & CBS_HASSTRINGS);
}
#endif

void CXFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_COMBOBOX);
	lpMIS->itemHeight = (::GetSystemMetrics(SM_CYVTHUMB) - ::GetSystemMetrics(SM_CYEDGE));
	lpMIS->itemWidth = 0; 
	lpMIS->itemHeight = 20;
}
/*
int CXFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{	
	ASSERT_VALID(this);
	//ASSERT(lpCIS->CtlType == ODT_LISTBOX);

	int iItem1 = (int)(WORD)lpCIS->itemID1;
	if (iItem1 == -1)
	{
		return -1;
	}

	CString strItem1;
	GetLBText(iItem1, strItem1);

	int iItem2 = (int)(WORD)lpCIS->itemID2;
	if (iItem2 == -1)
	{
		return -1;
	}

	CString strItem2;
	GetLBText(iItem2, strItem2);

	return strItem1.Collate(strItem2);
}
*/
void CXFontComboBox::InitControl(LPCTSTR lpszFaceName, UINT nWidth/*= 0*/, BOOL bEnable/*= TRUE*/)
{
	ASSERT_VALID(this);
	if (!nWidth)
	{
		SetDroppedWidth(nWidth);
	}
	if (lpszFaceName && _tcslen(lpszFaceName))
	{
		SetSelFont(lpszFaceName);
	}
}

BOOL CXFontComboBox::GetSelFont(CXLogFont& lf)
{
	LONG iCurSel = GetCurSel();
	if (iCurSel == CB_ERR)
	{
		CString strWindowText;
		GetWindowText(strWindowText);

		iCurSel = SelectString(-1, strWindowText);

		if (iCurSel == CB_ERR)
		{
			return FALSE;
		}
	}
	list<CXLogFont>::const_iterator itlstTravel = m_listFonts.begin();
	while(itlstTravel != m_listFonts.end() && iCurSel > 0)
	{
		itlstTravel ++;
		iCurSel --;
	}
	assert(!iCurSel);
	const CXLogFont& tLF = *itlstTravel;
	lf = tLF;	
	return TRUE;
}

BOOL CXFontComboBox::GetSelFont(CString& strFaceName)
{
	CXLogFont lf;
	if (GetSelFont(lf))
	{
		strFaceName = lf.lfFaceName;
		return TRUE;
	}
	return FALSE;
}

BOOL CXFontComboBox::SetSelFont(CXLogFont& lf)
{
	return SetSelFont(lf.lfFaceName);
}

BOOL CXFontComboBox::SetSelFont(const CString& strFaceName)
{
	LONG nIndex = 0;
	list<CXLogFont>::const_iterator itlstTravel = m_listFonts.begin();
	while (itlstTravel != m_listFonts.end())
	{
		const CXLogFont& tLF = *itlstTravel;
		if (0 == strFaceName.CompareNoCase(tLF.lfFaceName))
		{
			break;
		}
		nIndex ++;
		itlstTravel ++;
	}
	if (nIndex < (LONG)m_listFonts.size())
	{
		SetCurSel(nIndex);
		return TRUE;
	}	
	return FALSE;
}

BOOL CXFontComboBox::OnDropDown()
{
	GetSelFont(m_csSelected);
	return FALSE; // continue routing.
}


BOOL CXFontComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					SetSelFont(m_csSelected);
					NotifyOwner(CBN_SELENDCANCEL);
				}
				return TRUE;
			}

		case VK_SPACE:
		case VK_RETURN:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					NotifyOwner(CBN_SELENDOK);
				}
				return TRUE;
			}

		case VK_UP:
		case VK_DOWN:
			{
				if (GetDroppedState() == FALSE)
				{
					ShowDropDown(TRUE);
					return TRUE;
				}
				break;
			}
		}
	}
	return CXFlatComboBox::PreTranslateMessage(pMsg);
}



BOOL CALLBACK CXFontComboBox::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
                                             NEWTEXTMETRICEX* /*lpntm*/,
                                             DWORD dwFontType, LPARAM lParam)
{
  CXFontComboBox* pFontEnum = (CXFontComboBox*)lParam;
  if (pFontEnum != NULL)
  {
    if (pFontEnum->AddFont(&pelf->elfLogFont, dwFontType))
    {
      // TODO:
    }
  }
  
  return TRUE;
}


void CXFontComboBox::InitFontLst(CDC* pDC/*= NULL*/, BYTE nCharSet/*= DEFAULT_CHARSET*/)
{
  m_listFonts.clear();    
  CXLogFont lf;
  lf.lfFaceName[ 0 ] = '\0';
  lf.lfCharSet = nCharSet;
  
  if (!pDC || !pDC->m_hDC)
  {
    CWindowDC dc(NULL);
    ::EnumFontFamiliesEx(dc.m_hDC, &lf,
      (FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
  }
  else
  {
    ::EnumFontFamiliesEx(pDC->m_hDC, &lf,
      (FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
  }
	//Sort			
	m_listFonts.sort();	
}


BOOL CXFontComboBox::AddFont(const LOGFONT* pLF, DWORD dwType)
{  
  CString strFaceName = pLF->lfFaceName;
  strFaceName.Remove('@');
  
  if (DoesFontExist(strFaceName) != 0)
  {
    return false;
  }
  CXLogFont tFont(*pLF);

	tFont.dwType = dwType;
	tFont.lfHeight = -(::GetSystemMetrics(SM_CYVTHUMB)-(::GetSystemMetrics(SM_CYEDGE)*2));
	tFont.lfWidth = 0;

  m_listFonts.push_back(CXLogFont(tFont));
  
  return TRUE;
}

BOOL CXFontComboBox::DoesFontExist(CString& strFaceName)
{	
  list<CXLogFont>::const_iterator itlstTravel = m_listFonts.begin();
  while(itlstTravel != m_listFonts.end())
  {
    if (strFaceName == (*itlstTravel).lfFaceName)
    {
      return TRUE;
    }
    itlstTravel ++;
  }	
  return FALSE;
}

CXLogFont* CXFontComboBox::GetLogFont(const CString& strFaceName)
{
  list<CXLogFont>::iterator itlstTravel = m_listFonts.begin();
  while(itlstTravel != m_listFonts.end())
  {
    if (strFaceName == (*itlstTravel).lfFaceName)
    {
      return &(*itlstTravel);
    }
    itlstTravel ++;
  }		
  return NULL;
}


LONG CXFontComboBox::GetMaxWidth()
{
  CWindowDC dc(NULL);
  HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);	
  HFONT hFontOld = (HFONT)SelectObject(dc.GetSafeHdc(), hFont);
  LONG iMaxWidth = 0;
  
  list<CXLogFont>::const_iterator itlstTravel = m_listFonts.begin();
  while (itlstTravel != m_listFonts.end())
  {
    const CXLogFont& lf = *itlstTravel;
    LONG nWidth = dc.GetTextExtent(lf.lfFaceName).cx;
    iMaxWidth = __max(iMaxWidth, nWidth);
    
    itlstTravel ++;
  }	
   
  SelectObject(dc.GetSafeHdc(), hFontOld);
  hFont = NULL;
  return iMaxWidth;
}

void CXFontComboBox::PreSubclassWindow()
{
  ResetContent();  
	DWORD dwStyple = ::GetWindowLong(m_hWnd, GWL_STYLE);

	// combo box is owner drawn, and must be created with the
	// following styles:
	dwStyple |= CBS_OWNERDRAWFIXED;
	dwStyple |= CBS_DROPDOWNLIST;
	dwStyple &= ~CBS_SORT;
	//dwStyple &= ~CBS_HASSTRINGS;
	ModifyStyle(CBS_SORT | CBS_HASSTRINGS, 0, 0);
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyple);
	SetItemHeight(-1, (::GetSystemMetrics(SM_CYVTHUMB) - ::GetSystemMetrics(SM_CYEDGE)));
	list<CXLogFont>::const_iterator itlstTravel = m_listFonts.begin();
	while (itlstTravel != m_listFonts.end())
	{
		AddString((*itlstTravel).lfFaceName);
		itlstTravel ++;
	}	
  __super::PreSubclassWindow();
}


void CXFontComboBox::SetListStyle(DWORD dwStyle) 
{
	m_dwStyle = dwStyle;
}
void CXFontComboBox::SetIcon( HICON hIcon)
{
	if (m_hIconFontType)
	{
		DestroyIcon(m_hIconFontType);
		m_hIconFontType = NULL;
	}
	if (hIcon)
	{
		m_hIconFontType = CopyIcon(hIcon);
	}	
}
