// cstm1dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBProAppWizard.h"
#include "cstm1dlg.h"
#include "BCGCBProAppWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustom1Dlg dialog


CCustom1Dlg::CCustom1Dlg()
	: CAppWizStepDlg(CCustom1Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom1Dlg)
	m_bMenuBar = TRUE;
	m_nBarsLook = 3;
	m_bStaticLink = FALSE;
	m_b256Colors = TRUE;
	m_bHotTrack = FALSE;
	m_bMDITabs = FALSE;
	m_bWindowsDlg = TRUE;
	m_bMDITabGroups = FALSE;
	//}}AFX_DATA_INIT
}


void CCustom1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom1Dlg)
	DDX_Control(pDX, IDC_WINDOWS_DIALOG, m_btnWindowsDlg);
	DDX_Control(pDX, IDC_CAPTION, m_wndCaption);
	DDX_Control(pDX, IDC_IMAGE, m_wndImage);
	DDX_Check(pDX, IDC_MENU_BAR, m_bMenuBar);
	DDX_Radio(pDX, IDC_2000_LOOK, m_nBarsLook);
	DDX_Check(pDX, IDC_STATIC_LINK, m_bStaticLink);
	DDX_Check(pDX, IDC_COLOR256, m_b256Colors);
	DDX_Check(pDX, IDC_HOT_TRACK, m_bHotTrack);
	DDX_Check(pDX, IDC_MDI_TABS, m_bMDITabs);
	DDX_Check(pDX, IDC_WINDOWS_DIALOG, m_bWindowsDlg);
	DDX_Check(pDX, IDC_MDI_TAB_GROUPS, m_bMDITabGroups);
	//}}AFX_DATA_MAP
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CCustom1Dlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	SetBoolVar (_T("MENUBAR"), m_bMenuBar);
	SetBoolVar (_T("OFFICE200_LOOK"), m_nBarsLook == 0);
	SetBoolVar (_T("OFFICE_XP_LOOK"), m_nBarsLook == 1);
	SetBoolVar (_T("WIN_XP_LOOK"), m_nBarsLook == 2);
	SetBoolVar (_T("OFFICE2003_LOOK"), m_nBarsLook == 3);
	SetBoolVar (_T("VS2005_LOOK"), m_nBarsLook == 4);
	SetBoolVar (_T("OFFICE2007_LOOK"), m_nBarsLook == 5);
	SetBoolVar (_T("BCGSTATIC_LINK"), m_bStaticLink);
	SetBoolVar (_T("TOOLBAR_256_COLORS"), m_b256Colors);
	SetBoolVar (_T("TOOLBAR_HOT_TRACK"), m_bHotTrack && m_b256Colors);
	SetBoolVar (_T("MDI_TABS"), m_bMDITabs);
	SetBoolVar (_T("MDI_TAB_GROUPS"), m_bMDITabGroups);
	SetBoolVar (_T("WINDOWS_DLG"), m_bWindowsDlg);

	SetBoolVar (_T("DOCKING_STYLE_SMART"), m_nBarsLook == 3 || m_nBarsLook == 4 || m_nBarsLook == 5);
	SetBoolVar (_T("DOCKING_STYLE_REGULAR"), m_nBarsLook != 4 && m_nBarsLook != 3);

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}


BEGIN_MESSAGE_MAP(CCustom1Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom1Dlg)
	ON_BN_CLICKED(IDC_COLOR256, OnColor256)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MDI_TABS, OnMdiTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustom1Dlg message handlers

void CCustom1Dlg::SetBoolVar (LPCTSTR lpszName, BOOL bVal)
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

void CCustom1Dlg::EnableControls ()
{
	BOOL bIsMDI = FALSE;

	CString strTemp;
	if (BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("PROJTYPE_DLG"), strTemp))
	{
		//-------------------------
		// Dialog-based application
		//-------------------------
		GetDlgItem (IDC_MENU_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_COLOR256)->EnableWindow (FALSE);
		GetDlgItem (IDC_HOT_TRACK)->EnableWindow (FALSE);
	}
	else
	{
		GetDlgItem (IDC_HOT_TRACK)->EnableWindow (m_b256Colors);

		if (BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("PROJTYPE_MDI"), strTemp))
		{
			bIsMDI = TRUE;
		}
	}

	GetDlgItem (IDC_MDI_TABS)->EnableWindow (bIsMDI);
	GetDlgItem (IDC_MDI_TAB_GROUPS)->EnableWindow (bIsMDI && m_bMDITabs);
	m_btnWindowsDlg.EnableWindow (bIsMDI);

	if (!bIsMDI)
	{
		m_bWindowsDlg = FALSE;
		m_btnWindowsDlg.SetCheck (FALSE);
	}
}

BOOL CCustom1Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

	m_wndCaption.GetClientRect (&m_rectCaption);
	m_wndCaption.MapWindowPoints (this, &m_rectCaption);

	m_Bmp.LoadBitmap (IDB_BCG1);
	m_wndImage.SetBitmap (m_Bmp);

	HideHelpButton (this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustom1Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		CString strKey;
		if (!BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("MFCDLL"), strKey))
		{
			m_bStaticLink = FALSE;
			GetDlgItem (IDC_STATIC_LINK)->EnableWindow (FALSE);
			UpdateData (FALSE);
		}

		HideHelpButton (this);
		EnableControls ();
	}
}

void CCustom1Dlg::OnColor256() 
{
	UpdateData ();
	EnableControls ();
}

void CCustom1Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	::FillRect (dc.GetSafeHdc (), m_rectCaption, 
		::GetSysColorBrush (COLOR_3DSHADOW));

	const CString strCaption = _T("BCGControlBar: step 1/4 - General");
	
	CFont* pOldFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (::GetSysColor (COLOR_WINDOW));

	CRect rectCaption = m_rectCaption;
	rectCaption.DeflateRect (5, 0);

	dc.DrawText (strCaption, rectCaption, DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject (pOldFont);
}

void CCustom1Dlg::OnMdiTabs() 
{
	UpdateData ();
	GetDlgItem (IDC_MDI_TAB_GROUPS)->EnableWindow (m_bMDITabs);
}
