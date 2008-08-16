// cstm3dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBProAppWizard.h"
#include "cstm3dlg.h"
#include "BCGCBProAppWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustom3Dlg dialog


CCustom3Dlg::CCustom3Dlg()
	: CAppWizStepDlg(CCustom3Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom3Dlg)
	m_bCustomization = TRUE;
	m_bKeyboard = TRUE;
	m_bMouse = FALSE;
	m_bContextMenuCustomization = TRUE;
	m_bUserToolbars = FALSE;
	m_bUserImages = FALSE;
	m_bAltCustomization = TRUE;
	m_bRUMenus = TRUE;
	m_bUserTools = FALSE;
	//}}AFX_DATA_INIT

	m_bRebar = FALSE;
	m_bMenuBar = FALSE;
}


void CCustom3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom3Dlg)
	DDX_Control(pDX, IDC_CAPTION, m_wndCaption);
	DDX_Control(pDX, IDC_IMAGE, m_wndImage);
	DDX_Check(pDX, IDC_CUSTOMIZATION, m_bCustomization);
	DDX_Check(pDX, IDC_KEYBOARD_CUSTOMIZATION, m_bKeyboard);
	DDX_Check(pDX, IDC_MOUSE_CUSTOMIZATION, m_bMouse);
	DDX_Check(pDX, IDC_CONTEXT_MENU_CUSTOMIZATION, m_bContextMenuCustomization);
	DDX_Check(pDX, IDC_USER_TOOLBARS, m_bUserToolbars);
	DDX_Check(pDX, IDC_USER_IMAGES, m_bUserImages);
	DDX_Check(pDX, IDC_ALT_CUSTOMIZATION, m_bAltCustomization);
	DDX_Check(pDX, IDC_RU_MENUS, m_bRUMenus);
	DDX_Check(pDX, IDC_USER_TOOLS, m_bUserTools);
	//}}AFX_DATA_MAP
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CCustom3Dlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	SetBoolVar (_T("TOOLBAR_CUSTOMIZATION"), m_bCustomization);
	SetBoolVar (_T("KEYBOARD_CUSTOMIZATION"), m_bCustomization && m_bKeyboard);
	SetBoolVar (_T("MOUSE_CUSTOMIZATION"), m_bCustomization && m_bMouse);
	SetBoolVar (_T("CONTEXT_MENU_CUSTOMIZATION"), m_bCustomization && m_bContextMenuCustomization);
	SetBoolVar (_T("USER_TOOLBAR"), m_bCustomization && m_bUserToolbars);
	SetBoolVar (_T("USER_IMAGES"), m_bCustomization && m_bUserImages);
	SetBoolVar (_T("ALT_CUSTOMIZATION"), m_bCustomization && m_bAltCustomization);
	SetBoolVar (_T("RU_MENUS"), m_bCustomization && m_bMenuBar && m_bRUMenus);
	SetBoolVar (_T("USER_TOOLS"), m_bCustomization && m_bUserTools);

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}


BEGIN_MESSAGE_MAP(CCustom3Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom3Dlg)
	ON_BN_CLICKED(IDC_CUSTOMIZATION, OnCustomization)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustom3Dlg message handlers

void CCustom3Dlg::SetBoolVar (LPCTSTR lpszName, BOOL bVal)
{
	if (bVal)
	{
		BCGCBProAppWizardaw.m_Dictionary.SetAt (lpszName, _T("1"));
	}
	else
	{
		BCGCBProAppWizardaw.m_Dictionary.RemoveKey (lpszName);
	}
}

void CCustom3Dlg::OnCustomization() 
{
	UpdateData ();
	EnableControls ();
}

void CCustom3Dlg::EnableControls ()
{
	CString strTemp;
	if (BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("PROJTYPE_DLG"), strTemp))
	{
		//-------------------------
		// Dialog-based application
		//-------------------------
		GetDlgItem (IDC_MENU_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_CUSTOMIZATION)->EnableWindow (FALSE);

		m_bCustomization = FALSE;
	}

	GetDlgItem (IDC_ALT_CUSTOMIZATION)->EnableWindow (m_bCustomization);
	GetDlgItem (IDC_USER_TOOLBARS)->EnableWindow (m_bCustomization && !m_bRebar);
	GetDlgItem (IDC_USER_IMAGES)->EnableWindow (m_bCustomization);
	GetDlgItem (IDC_KEYBOARD_CUSTOMIZATION)->EnableWindow (m_bCustomization);
	GetDlgItem (IDC_MOUSE_CUSTOMIZATION)->EnableWindow (m_bCustomization);
	GetDlgItem (IDC_CONTEXT_MENU_CUSTOMIZATION)->EnableWindow (m_bCustomization);
	GetDlgItem (IDC_RU_MENUS)->EnableWindow (m_bCustomization && m_bMenuBar);
}

BOOL CCustom3Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

	m_Bmp.LoadBitmap (IDB_BCG3);
	m_wndImage.SetBitmap (m_Bmp);

	m_wndCaption.GetClientRect (&m_rectCaption);
	m_wndCaption.MapWindowPoints (this, &m_rectCaption);

	HideHelpButton (this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustom3Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		CString strKey;

		m_bRebar = BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("REBAR"), strKey);
		m_bMenuBar = BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("MENUBAR"), strKey);

		EnableControls ();
	}
}

void CCustom3Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	::FillRect (dc.GetSafeHdc (), m_rectCaption, 
		::GetSysColorBrush (COLOR_3DSHADOW));

	const CString strCaption = _T("BCGControlBar: step 3/4 - Customization");
	
	CFont* pOldFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (::GetSysColor (COLOR_WINDOW));

	CRect rectCaption = m_rectCaption;
	rectCaption.DeflateRect (5, 0);

	dc.DrawText (strCaption, rectCaption, DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject (pOldFont);
}
