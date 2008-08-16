// AddButtonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBoxDemo.h"
#include "AddButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddButtonDlg dialog


CAddButtonDlg::CAddButtonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddButtonDlg::IDD, pParent),
	m_wndIconList (m_imageList)
{
	//{{AFX_DATA_INIT(CAddButtonDlg)
	m_nPage = 1;
	m_strLabel = _T("");
	//}}AFX_DATA_INIT
}


void CAddButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddButtonDlg)
	DDX_Control(pDX, IDC_LABEL, m_wndLabel);
	DDX_Control(pDX, IDC_ICON_LIST, m_wndIconList);
	DDX_Control(pDX, IDC_SPIN1, m_wndSpin);
	DDX_Text(pDX, IDC_PAGE, m_nPage);
	DDV_MinMaxInt(pDX, m_nPage, 1, 3);
	DDX_Text(pDX, IDC_LABEL, m_strLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddButtonDlg, CDialog)
	//{{AFX_MSG_MAP(CAddButtonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddButtonDlg message handlers

void CAddButtonDlg::OnOK() 
{
	if (!UpdateData ())
	{
		return;
	}

	if (m_strLabel.IsEmpty ())
	{
		MessageBox (_T("Label is required"));
		m_wndLabel.SetFocus ();
		return;
	}

	m_nSelImage = m_wndIconList.GetCurSel ();
	if (m_nSelImage < 0)
	{
		MessageBox (_T("Image is required"));
		return;
	}

	CDialog::OnOK();
}

BOOL CAddButtonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndSpin.SetRange32 (1, 3);
	
	m_imageList.SetImageSize (CSize (16, 16));
	m_imageList.SetTransparentColor (RGB (192, 192, 192));
	m_imageList.SetMapTo3DColors (FALSE);
	m_imageList.Load (IDB_PROP_ICONS);

	for (int i = 0; i < m_imageList.GetCount (); i++)
	{
		m_wndIconList.AddString (_T(""));
	}

	m_wndIconList.SetCurSel (0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CIconCombo

const int nIconMargin = 3;

CIconCombo::CIconCombo (CBCGPToolBarImages& imageListIcons) :
	m_imageListIcons (imageListIcons)
{
}
//*****************************************************************************************
CIconCombo::~CIconCombo()
{
}

BEGIN_MESSAGE_MAP(CIconCombo, CComboBox)
	//{{AFX_MSG_MAP(CIconCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconCombo message handlers

void CIconCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
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
	m_imageListIcons.Draw (pDC, 
		rect.left + nIconMargin,
		rect.top + nIconMargin / 2,
		nIcon);
	m_imageListIcons.EndDrawImage (ds);

	CString str;
	str.Format (_T("Image %d"), nIcon + 1);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (clText);

	CRect rectText = rect;
	rectText.left += m_imageListIcons.GetImageSize ().cx + 2 * nIconMargin;

	pDC->DrawText (str, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
}
//*****************************************************************************************
void CIconCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CClientDC dc (this);
	CFont* pOldFont = dc.SelectObject (&globalData.fontRegular);

	CString str = _T("Image 99");
	int nTextWidth = dc.GetTextExtent (str).cx + nIconMargin;

	dc.SelectObject (pOldFont);

	lpMeasureItemStruct->itemWidth = m_imageListIcons.GetImageSize ().cx + nTextWidth + 2 * nIconMargin;
	lpMeasureItemStruct->itemHeight = 
		2 * nIconMargin + max (globalData.GetTextHeight (), m_imageListIcons.GetImageSize ().cy);
}
