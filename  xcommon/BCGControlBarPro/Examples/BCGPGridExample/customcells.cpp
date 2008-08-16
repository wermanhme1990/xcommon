// CustomCells.cpp: implementation of the CColorGridItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPGridExample.h"
#include "CustomCells.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PROP_HAS_LIST	0x0001
#define PROP_HAS_BUTTON	0x0002
#define PROP_HAS_SPIN	0x0004
#define PROP_AUTOGROUP	0x0100

//////////////////////////////////////////////////////////////////////
// CPasswordItem Class

const TCHAR cPassword = _T('*');

CPasswordItem::CPasswordItem (const CString& strPassword) :
	CBCGPGridItem ((LPCTSTR) strPassword)
{
}
//******************************************************************************
CWnd* CPasswordItem::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CEdit* pWndEdit = new CEdit;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_PASSWORD;

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create (dwStyle, rectEdit, GetOwnerList(), BCGPROPLIST_ID_INPLACE);
	pWndEdit->SetPasswordChar (cPassword);

	bDefaultFormat = TRUE;
	return pWndEdit;
}
//******************************************************************************
CString CPasswordItem::FormatItem ()
{
	CString strVal = (LPCTSTR)(_bstr_t)m_varValue;
	
	for (int i = 0; i < strVal.GetLength (); i++)
	{
		strVal.SetAt (i, cPassword);
	}

	return strVal;
}

/////////////////////////////////////////////////////////////////////////////
// CIitemSliderCtrl

IMPLEMENT_DYNAMIC(CItemSliderCtrl, CSliderCtrl)

CItemSliderCtrl::CItemSliderCtrl(CSliderItem* pItem, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush (m_clrBack);
	m_pItem = pItem;
}
//******************************************************************************
CItemSliderCtrl::~CItemSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CItemSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CItemSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemSliderCtrl message handlers

HBRUSH CItemSliderCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor (m_clrBack);
	return m_brBackground;
}
//************************************************************************************
void CItemSliderCtrl::Redraw()
{
	ASSERT_VALID (m_pItem);

	long lCurrValue = (long) m_pItem->GetValue();
	long lNewValue = (long)GetPos ();

	if (lCurrValue != lNewValue)
	{
		m_pItem->SetValue (lNewValue);
	}
}

////////////////////////////////////////////////////////////////////////////////
// CSliderItem class

CSliderItem::CSliderItem (long lValue)
	: CBCGPGridItem  (lValue)
{
}
//************************************************************************************
CWnd* CSliderItem::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);
	
	CRect rectSpin;
	AdjustInPlaceEditRect (rectEdit, rectSpin);

	rectEdit.left += rectEdit.Height ()+ 5;

	CItemSliderCtrl* pWndSlider = new CItemSliderCtrl (this, 
		pWndList->GetBkColor ());

	pWndSlider->Create (WS_VISIBLE | WS_CHILD, rectEdit, pWndList, BCGPROPLIST_ID_INPLACE);
	pWndSlider->SetPos ((long) m_varValue);
	
	bDefaultFormat = FALSE;
	
	return pWndSlider;
}
//******************************************************************************************
BOOL CSliderItem::OnUpdateValue ()
{
	ASSERT_VALID (this);

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CItemSliderCtrl* pWndSlider = (CItemSliderCtrl*) m_pWndInPlace;

	long lNewValue = (long) pWndSlider->GetPos ();
	long lCurrValue = m_varValue;

	if (lCurrValue != lNewValue)
	{
		SetValue (lNewValue);
	}

	return TRUE;
}
//************************************************************************************
void CSliderItem::SetValue (const _variant_t& varValue)
{
	ASSERT_VALID (this);

	if (m_varValue.vt != VT_EMPTY && m_varValue.vt != varValue.vt)
	{
		ASSERT (FALSE);
		return;
	}

	m_varValue = varValue;
	Redraw ();
}

/////////////////////////////////////////////////////////////////////////////
// CFileItem Class

CFileItem::CFileItem(const CString& strFileName) :
	CBCGPGridItem (_variant_t((LPCTSTR) strFileName))
{
	m_dwFlags = PROP_HAS_BUTTON;
}
//****************************************************************************************
void CFileItem::OnClickButton (CPoint point)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList();

	m_bButtonIsDown = TRUE;
	Redraw ();

	CString strPath = (LPCTSTR)(_bstr_t)m_varValue;

	CFileDialog dlg (TRUE,  strPath);

	if (dlg.DoModal () == IDOK)
	{
		strPath = dlg.GetPathName ();
		m_varValue = (LPCTSTR) strPath;
	}

	m_bButtonIsDown = FALSE;
	Redraw ();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText (strPath);
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		pGridCtrl->SetFocus ();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFolderItem Class

CFolderItem::CFolderItem(const CString& strFolderName) :
	CBCGPGridItem (_variant_t((LPCTSTR) strFolderName))
{
	m_dwFlags = PROP_HAS_BUTTON;
}
//****************************************************************************************
void CFolderItem::OnClickButton (CPoint point)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList ();

	m_bButtonIsDown = TRUE;
	Redraw ();

	CString strPath = (LPCTSTR)(_bstr_t)m_varValue;

	if (theApp.GetShellManager ()->BrowseForFolder (strPath, NULL, strPath))
	{
		m_varValue = (LPCTSTR) strPath;
	}

	m_bButtonIsDown = FALSE;
	Redraw ();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText (strPath);
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		pGridCtrl->SetFocus ();
	}
}

/////////////////////////////////////////////////////////////////////////////
// FontItem Class

CFontItem::CFontItem(LOGFONT& lf) :
	CBCGPGridItem (_variant_t((LPCTSTR)lf.lfFaceName))
{
	m_lf = lf;
	m_lfOrig = lf;
	m_dwFlags = PROP_HAS_BUTTON;
}
//*****************************************************************************************
void CFontItem::OnClickButton (CPoint point)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList();
	ASSERT_VALID (pGridCtrl);

	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	COLORREF nColorPrev = m_Color;

	m_bButtonIsDown = TRUE;
	Redraw ();

	CFontDialog dlg (&m_lf);

	if (m_Color != (COLORREF)-1)
	{
		dlg.m_cf.rgbColors = m_Color;
	}

	if (dlg.DoModal () == IDOK)
	{
		dlg.GetCurrentFont (&m_lf);
		m_Color = dlg.GetColor ();

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText (FormatProperty ());
		}
		else
		{
			m_varValue = (LPCTSTR) FormatProperty ();
		}
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		pGridCtrl->SetFocus ();
	}

	m_bButtonIsDown = FALSE;
	Redraw ();
}
//*****************************************************************************
CString CFontItem::FormatProperty ()
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList ();
	ASSERT_VALID (pGridCtrl);

	CWindowDC dc (pGridCtrl);

	int nLogY = dc.GetDeviceCaps (LOGPIXELSY);

	CString str;

	if (nLogY != 0)
	{
		str.Format( _T("%s(%i)"), m_lf.lfFaceName, 
			MulDiv (72, -m_lf.lfHeight, nLogY));
	}
	else
	{
		str = m_lf.lfFaceName;
	}

	return str;
}

////////////////////////////////////////////////////////////////////////////////
// CIconGridItem class

const int nIconMargin = 3;

CIconGridItem::CIconGridItem(const CImageList& imageListIcons, int nItem
							)
	:	CBCGPGridItem ("")
{
	m_imageListIcons.CreateFromImageList (imageListIcons);
	m_nItem=nItem;
	m_imageListIcons.SetTransparentColor (::GetSysColor (COLOR_3DFACE));

	AllowEdit (FALSE);
}
//***********************************************************************
void CIconGridItem::OnDrawValue (CDC* pDC, CRect rect)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList();
	ASSERT_VALID (pGridCtrl);

	ASSERT_VALID (pDC);


	CRect rectImage = rect;
	rectImage.right = rectImage.left + rectImage.Height ();
	rectImage.DeflateRect (1, 1);

	CBCGPDrawState ds;
		
	m_imageListIcons.PrepareDrawImage (ds, rectImage.Size ());
	m_imageListIcons.Draw (pDC, rectImage.left, rectImage.top, m_nItem);
	m_imageListIcons.EndDrawImage (ds);
	rect.left = rectImage.right + 2 * nIconMargin;
	m_bValueIsTrancated = FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// CIconListItem class

CIconListItem::CIconListItem(const _variant_t varValue, const CImageList& imageListIcons,
							int nSelectedIcon,
							CStringList* plstIconNames
							)
	:	CBCGPGridItem (varValue)
{
	m_imageListIcons.CreateFromImageList (imageListIcons);
	m_imageListIcons.SetTransparentColor (::GetSysColor (COLOR_3DFACE));
	
	if (plstIconNames != NULL)
	{
		m_lstIconNames.AddTail (plstIconNames);
		ASSERT (m_lstIconNames.GetCount () == m_imageListIcons.GetCount ());
	}

	for (int i = 0; i < m_imageListIcons.GetCount (); i++)
	{
		CString strItem;
		strItem.Format (_T("%d"), i);

		AddOption (strItem);
	}

	AllowEdit (1);
}
//*****************************************************************************************
CComboBox* CIconListItem::CreateCombo (CWnd* pWndParent, CRect rect)
{
	rect.bottom = rect.top + 400;

	CIconCombo* pWndCombo = new CIconCombo (m_imageListIcons, m_lstIconNames);
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}
	pWndCombo->SetIconListItem(this);
	return pWndCombo;
}
//*****************************************************************************************
void CIconListItem::OnDrawValue (CDC* pDC, CRect rect)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList();
	ASSERT_VALID (pGridCtrl);

	ASSERT_VALID (pDC);

	CString strVal = (LPCTSTR)(_bstr_t) m_varValue;
	if (strVal.IsEmpty () || m_imageListIcons.GetCount () == 0)
	{
		return;
	}
	
	int nIndex = -1;
	_stscanf (strVal, _T("%d"), &nIndex);

	if (nIndex < 0)
	{
		return;
	}

	COLORREF clrTextOld = pDC->GetTextColor ();

	CFont* pOldFont = pDC->SelectObject (pGridCtrl->GetFont ());

	CRect rectImage = rect;
	rectImage.right = rectImage.left + rectImage.Height ();
	rectImage.DeflateRect (1, 1);

	CBCGPDrawState ds;
	m_imageListIcons.PrepareDrawImage (ds, rectImage.Size ());
	m_imageListIcons.Draw (pDC, rectImage.left, rectImage.top, nIndex);
	m_imageListIcons.EndDrawImage (ds);

	if (!m_lstIconNames.IsEmpty ())
	{
		CString str = m_lstIconNames.GetAt (m_lstIconNames.FindIndex (nIndex));

		rect.left = rectImage.right + 2 * nIconMargin;

		pDC->DrawText (str, rect, DT_SINGLELINE | DT_VCENTER);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SelectObject (pOldFont);

	m_bValueIsTrancated = FALSE;
}
//*****************************************************************************************
CWnd* CIconListItem::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CWnd* pWnd = CBCGPGridItem::CreateInPlaceEdit (rectEdit, bDefaultFormat);
	if (pWnd != NULL)
	{
		pWnd->ShowWindow (SW_HIDE);
	}

	return pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CIconCombo

CIconCombo::CIconCombo(CBCGPToolBarImages& imageListIcons, CStringList& lstIconNames) :
	m_imageListIcons (imageListIcons),
	m_lstIconNames (lstIconNames)
{
}
//*****************************************************************************************
CIconCombo::~CIconCombo()
{
}

BEGIN_MESSAGE_MAP(CIconCombo, CComboBox)
	//{{AFX_MSG_MAP(CIconCombo)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconCombo message handlers

void CIconCombo::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rect = lpDIS->rcItem;
	int nIcon = lpDIS->itemID;

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

	if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
		::FillRect (lpDIS->hDC, &rect, brBackground);
	}

	if (nIcon < 0)
	{
		return;
	}

	CBCGPDrawState ds;
	m_imageListIcons.PrepareDrawImage (ds);
	m_imageListIcons.Draw (pDC, rect.left + nIconMargin, rect.top + nIconMargin, nIcon);
	m_imageListIcons.EndDrawImage (ds);

	if (!m_lstIconNames.IsEmpty ())
	{
		CString str = m_lstIconNames.GetAt (m_lstIconNames.FindIndex (nIcon));
		CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);

		pDC->SetBkMode (TRANSPARENT);
		pDC->SetTextColor (clText);

		CRect rectText = rect;
		rectText.left += m_imageListIcons.GetImageSize ().cx + 2 * nIconMargin;

		pDC->DrawText (str, rectText, DT_SINGLELINE | DT_VCENTER);

		pDC->SelectObject (pOldFont);
	}
}
//*****************************************************************************************
void CIconCombo::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	int nTextWidth = 0;
	int nTextHeight = 0;

	if (!m_lstIconNames.IsEmpty ())
	{
		nTextHeight = globalData.GetTextHeight ();

		CClientDC dc (this);
		CFont* pOldFont = dc.SelectObject (&globalData.fontRegular);

		for (POSITION pos = m_lstIconNames.GetHeadPosition (); pos != NULL;)
		{
			CString str = m_lstIconNames.GetNext (pos);

			nTextWidth = max (nTextWidth, dc.GetTextExtent (str).cx + nIconMargin);
		}

		dc.SelectObject (pOldFont);
	}

	lpMeasureItemStruct->itemWidth = m_imageListIcons.GetImageSize ().cx + nTextWidth + 2 * nIconMargin;
	lpMeasureItemStruct->itemHeight = max (nTextHeight, m_imageListIcons.GetImageSize ().cy + 2 * nIconMargin);
}
//*****************************************************************************************
void CIconCombo::OnCbnSelchange ()
{
	GetIconListItem ()->SetValue ((long) GetCurSel ());
}

////////////////////////////////////////////////////////////////////////////////
// CProgressItem Class

CProgressItem::CProgressItem (long lValue, long lTotal) : 
	CBCGPGridItem (lValue)
{
	m_lTotal = lTotal;
}
//*****************************************************************************************
void CProgressItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	long nProgressCurr = (long) m_varValue;

	CRect rectProgress = rect;
	rectProgress.DeflateRect (1, 1);
	rectProgress.top++;

	pDC->Draw3dRect (rectProgress, RGB (0, 83, 250), RGB (0, 83, 250));

	CRect rectComplete = rectProgress;
	rectComplete.right = rectComplete.left + 
		nProgressCurr * rectComplete.Width () / m_lTotal;

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rectComplete, 
		RGB (160, 192, 255), 
		RGB (225, 235, 255), TRUE);

	pDC->Draw3dRect (rectComplete, RGB (0, 83, 250), RGB (0, 83, 250));

	CString strText;
	strText.Format (_T("%d%%"), nProgressCurr * 100 / m_lTotal);

	COLORREF clrText = pDC->SetTextColor (RGB (0, 0, 208));
	pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC->SetTextColor (clrText);
}

////////////////////////////////////////////////////////////////////////////////
// CButtonItem Class

CButtonItem::CButtonItem (LPCTSTR lpszText, UINT id) :
	CBCGPGridItem (lpszText)
{
	m_id = id;
}
//*****************************************************************************************
void CButtonItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	rect.DeflateRect (1, 1);

	pDC->FillRect (rect, &globalData.brBtnFace);

	pDC->Draw3dRect (rect, globalData.clrBtnLight, globalData.clrBtnDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBtnHilite, globalData.clrBtnShadow);

	COLORREF clrText = pDC->SetTextColor (globalData.clrBtnText);

	CString strText = (LPCTSTR)(_bstr_t) m_varValue;

	pDC->DrawText (strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC->SetTextColor (clrText);
}
//*****************************************************************************************
BOOL CButtonItem::OnClickValue (UINT uiMsg, CPoint point)
{
	if (uiMsg != WM_LBUTTONUP)
	{
		return CBCGPGridItem::OnClickValue (uiMsg, point);
	}

	CBCGPGridCtrl* pGridCtrl = GetOwnerList();
	ASSERT_VALID (pGridCtrl);

	pGridCtrl->SendMessage (WM_COMMAND,
							MAKEWPARAM (m_id, BN_CLICKED));

	return TRUE;
}
