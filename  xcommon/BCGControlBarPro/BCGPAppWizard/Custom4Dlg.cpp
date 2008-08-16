// Custom4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBProAppWizard.h"
#include "Custom4Dlg.h"
#include "BCGCBProAppWizardaw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustom4Dlg dialog


CCustom4Dlg::CCustom4Dlg()
	: CAppWizStepDlg(CCustom4Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom4Dlg)
	m_bDownloadSkins = TRUE;
	m_bSkinsManager = FALSE;
	m_nSkinsDir = 0;
	m_strSkinsFolder = _T("");
	m_strSkinURL = _T("http://www.bcgsoft.com/Skins");
	m_bVisualManager = TRUE;
	//}}AFX_DATA_INIT
}


void CCustom4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom4Dlg)
	DDX_Control(pDX, IDC_SKIN_MANAGER, m_wndSkinManager);
	DDX_Control(pDX, IDC_SKIN_URL, m_wndSkinURL);
	DDX_Control(pDX, IDC_IMAGE, m_wndImage);
	DDX_Control(pDX, IDC_SKINS_FOLDER, m_wndSkinsFolder);
	DDX_Control(pDX, IDC_DOWNLOAD_SKINS, m_wndDownloadSkins);
	DDX_Control(pDX, IDC_CAPTION, m_wndCaption);
	DDX_Control(pDX, IDC_BROWSE_SKINS_FOLDER, m_btnBrowseSkinsFolder);
	DDX_Check(pDX, IDC_DOWNLOAD_SKINS, m_bDownloadSkins);
	DDX_Check(pDX, IDC_SKIN_MANAGER, m_bSkinsManager);
	DDX_Radio(pDX, IDC_SKINS_DIR1, m_nSkinsDir);
	DDX_Text(pDX, IDC_SKINS_FOLDER, m_strSkinsFolder);
	DDX_Text(pDX, IDC_SKIN_URL, m_strSkinURL);
	DDX_Check(pDX, IDC_VISUAL_MANAGER, m_bVisualManager);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustom4Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom4Dlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SKINS_DIR1, OnUpdateControls)
	ON_BN_CLICKED(IDC_BROWSE_SKINS_FOLDER, OnBrowseSkinsFolder)
	ON_BN_CLICKED(IDC_SKINS_DIR2, OnUpdateControls)
	ON_BN_CLICKED(IDC_DOWNLOAD_SKINS, OnUpdateControls)
	ON_BN_CLICKED(IDC_SKIN_MANAGER, OnUpdateControls)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustom4Dlg message handlers

BOOL CCustom4Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();
	
	m_Bmp.LoadBitmap (IDB_BCG);
	m_wndImage.SetBitmap (m_Bmp);

	m_wndCaption.GetClientRect (&m_rectCaption);
	m_wndCaption.MapWindowPoints (this, &m_rectCaption);

	OnUpdateControls ();
	
	HideHelpButton (this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustom4Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	::FillRect (dc.GetSafeHdc (), m_rectCaption, 
		::GetSysColorBrush (COLOR_3DSHADOW));

	const CString strCaption = _T("BCGControlBar: step 4/4 - Visualization");
	
	CFont* pOldFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (::GetSysColor (COLOR_WINDOW));

	CRect rectCaption = m_rectCaption;
	rectCaption.DeflateRect (5, 0);

	dc.DrawText (strCaption, rectCaption, DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject (pOldFont);
}

void CCustom4Dlg::OnUpdateControls() 
{
	UpdateData ();

	m_wndSkinsFolder.EnableWindow (m_bSkinsManager && m_nSkinsDir == 1);
	m_wndDownloadSkins.EnableWindow (m_bSkinsManager);
	m_btnBrowseSkinsFolder.EnableWindow (m_bSkinsManager && m_nSkinsDir == 1);
	m_wndSkinURL.EnableWindow (m_bSkinsManager && m_bDownloadSkins);

	GetDlgItem (IDC_SKINS_DIR1)->EnableWindow (m_bSkinsManager);
	GetDlgItem (IDC_SKINS_DIR2)->EnableWindow (m_bSkinsManager);
}

void CCustom4Dlg::OnBrowseSkinsFolder() 
{
	// TODO: Add your control notification handler code here
	
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CCustom4Dlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	SetBoolVar (_T("VISUAL_MANAGER"), m_bVisualManager);
	SetBoolVar (_T("SKIN_MANAGER"), m_bSkinsManager);
	SetBoolVar (_T("DOWNLOAD_SKINS"), m_bDownloadSkins);
	SetBoolVar (_T("DEFAULT_SKIN_FOLDER"), m_nSkinsDir == 0);
	BCGCBProAppWizardaw.m_Dictionary.SetAt (_T("SKINS_FOLDER"), m_strSkinsFolder);
	BCGCBProAppWizardaw.m_Dictionary.SetAt (_T("SKINS_URL"), m_strSkinURL);

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}

void CCustom4Dlg::SetBoolVar (LPCTSTR lpszName, BOOL bVal)
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

void CCustom4Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		CString strKey;

		BOOL bBCGStatic = 
			BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("BCGSTATIC_LINK"), strKey) ||
			!BCGCBProAppWizardaw.m_Dictionary.Lookup (_T("MFCDLL"), strKey);

		if (bBCGStatic)
		{
			m_bSkinsManager = FALSE;
			UpdateData (m_bSkinsManager);

			OnUpdateControls ();
		}

		m_wndSkinManager.EnableWindow (!bBCGStatic);
		HideHelpButton (this);
	}
}
