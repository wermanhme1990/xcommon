// cstm1dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBProAppWizard.h"
#include "cstm2dlg.h"
#include "BCGCBProAppWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustom2Dlg dialog


CCustom2Dlg::CCustom2Dlg()
	: CAppWizStepDlg(CCustom2Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom2Dlg)
	m_bOutputBar = TRUE;
	m_bWorkspaceBar = TRUE;
	m_bCaptionBar = FALSE;
	m_bOutlookBar = FALSE;
	m_bOutlookEmbeddedControl = FALSE;
	m_bOutlookBackground = FALSE;
	m_bAutoHide = TRUE;
	m_bTaskPane = FALSE;
	m_bTaskPanePages = FALSE;
	m_nDockingStyle = 0;
	//}}AFX_DATA_INIT

	m_bRebar = FALSE;
}


void CCustom2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom2Dlg)
	DDX_Control(pDX, IDC_CAPTION, m_wndCaption);
	DDX_Control(pDX, IDC_IMAGE, m_wndImage);
	DDX_Check(pDX, IDC_BOTTOM_BAR, m_bOutputBar);
	DDX_Check(pDX, IDC_LEFT_BAR, m_bWorkspaceBar);
	DDX_Check(pDX, IDC_CAPTION_BAR, m_bCaptionBar);
	DDX_Check(pDX, IDC_OUTLOOK_BAR, m_bOutlookBar);
	DDX_Check(pDX, IDC_OUTLOOK_EMBEDDED_CONTROL, m_bOutlookEmbeddedControl);
	DDX_Check(pDX, IDC_OUTLOOK_BACKGROUND, m_bOutlookBackground);
	DDX_Check(pDX, IDC_AUTOHIDE_STYLE, m_bAutoHide);
	DDX_Check(pDX, IDC_RIGHT_BAR, m_bTaskPane);
	DDX_Check(pDX, IDC_TASKPANE_PAGES, m_bTaskPanePages);
	DDX_CBIndex(pDX, IDC_DOCKING_STYLE, m_nDockingStyle);
	//}}AFX_DATA_MAP
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CCustom2Dlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	SetBoolVar (_T("WORKSPACEBAR"), m_bWorkspaceBar);
	SetBoolVar (_T("OUTPUTBAR"), m_bOutputBar);
	SetBoolVar (_T("OUTLOOKBAR"), m_bOutlookBar);
	SetBoolVar (_T("CAPTIONBAR"), m_bCaptionBar);
	SetBoolVar (_T("OUTLOOK_EMBEDDED_CONTROL"), m_bOutlookEmbeddedControl);
	SetBoolVar (_T("OUTLOOK_BACKGROUND"), m_bOutlookBackground);
	SetBoolVar (_T("TASKPANE"), m_bTaskPane);
	SetBoolVar (_T("TASKPANE_PAGES"), m_bTaskPanePages);
	SetBoolVar (_T("IMMEDIATE_DOCKING"), FALSE);
	SetBoolVar (_T("AUTO_HIDE"), m_bAutoHide);

	SetBoolVar (_T("DOCKING_STYLE_REGULAR"), m_nDockingStyle == 0);
	SetBoolVar (_T("DOCKING_STYLE_SMART"), m_nDockingStyle == 1);

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}


BEGIN_MESSAGE_MAP(CCustom2Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom2Dlg)
	ON_BN_CLICKED(IDC_CUSTOMIZATION, OnCustomization)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_OUTLOOK_BAR, OnOutlookBar)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RIGHT_BAR, OnRightBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustom2Dlg message handlers

void CCustom2Dlg::SetBoolVar (LPCTSTR lpszName, BOOL bVal)
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

void CCustom2Dlg::OnCustomization() 
{
	UpdateData ();
	EnableControls ();
}

void CCustom2Dlg::EnableControls ()
{
	CString strTemp;
	if (BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("PROJTYPE_DLG"), strTemp))
	{
		//-------------------------
		// Dialog-based application
		//-------------------------
		GetDlgItem (IDC_LEFT_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_BOTTOM_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_CAPTION_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_OUTLOOK_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_RIGHT_BAR)->EnableWindow (FALSE);
		GetDlgItem (IDC_TASKPANE_PAGES)->EnableWindow (FALSE);
	}
	else
	{
		GetDlgItem (IDC_LEFT_BAR)->EnableWindow (!m_bRebar);
		GetDlgItem (IDC_BOTTOM_BAR)->EnableWindow (!m_bRebar);
		GetDlgItem (IDC_RIGHT_BAR)->EnableWindow (!m_bRebar);
		GetDlgItem (IDC_TASKPANE_PAGES)->EnableWindow (!m_bRebar);
		GetDlgItem (IDC_AUTOHIDE_STYLE)->EnableWindow (!m_bRebar);

		GetDlgItem (IDC_OUTLOOK_EMBEDDED_CONTROL)->EnableWindow (m_bOutlookBar);
		GetDlgItem (IDC_OUTLOOK_BACKGROUND)->EnableWindow (m_bOutlookBar);
		GetDlgItem (IDC_TASKPANE_PAGES)->EnableWindow (m_bTaskPane);
	}
}

BOOL CCustom2Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

	m_Bmp.LoadBitmap (IDB_BCG2);
	m_wndImage.SetBitmap (m_Bmp);

	m_wndCaption.GetClientRect (&m_rectCaption);
	m_wndCaption.MapWindowPoints (this, &m_rectCaption);

	CString strTemp;
	if (BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("VS2005_LOOK"), strTemp) ||
		BCGCBProAppWizardaw.m_Dictionary.Lookup(_T("OFFICE2003_LOOK"), strTemp))
	{
		m_nDockingStyle = 1;
	}
	else
	{
		m_nDockingStyle = 0;
	}

	UpdateData (FALSE);
	HideHelpButton (this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustom2Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		CString strKey;
		m_bRebar = BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("REBAR"), strKey);

		EnableControls ();
		HideHelpButton (this);
	}
}

void CCustom2Dlg::OnOutlookBar() 
{
	UpdateData ();
	EnableControls ();
}

void CCustom2Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	::FillRect (dc.GetSafeHdc (), m_rectCaption, 
		::GetSysColorBrush (COLOR_3DSHADOW));

	const CString strCaption = _T("BCGControlBar: step 2/4 - Control bars");
	
	CFont* pOldFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (::GetSysColor (COLOR_WINDOW));

	CRect rectCaption = m_rectCaption;
	rectCaption.DeflateRect (5, 0);

	dc.DrawText (strCaption, rectCaption, DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject (pOldFont);
}

void CCustom2Dlg::OnRightBar() 
{
	UpdateData ();
	EnableControls ();
}
