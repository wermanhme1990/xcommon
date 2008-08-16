// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBDotNetExample.h"

#include "FindDialog.h"
#include "BCGCBDotNetExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDialog dialog
CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDialog)
	//}}AFX_DATA_INIT
}


void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDialog)
	DDX_Control(pDX, IDC_FINDDIALOG_FINDCOMBO, m_wndCombo);
	DDX_Control(pDX, IDC_FINDDIALOG_REGULARBUTTON, m_btnMenu);
	DDX_Check(pDX, IDC_FINDDIALOG_MATCHCASECHECK, m_bMatchCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	//{{AFX_MSG_MAP(CFindDialog)
	ON_BN_CLICKED(IDC_FINDDIALOG_MARKALLBUTTON, OnMarkAll)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FINDDIALOG_MATCHWHOLEWORDONLYCHECK, OnMatchwholeChkClick)
	ON_BN_CLICKED(IDC_FINDDIALOG_MATCHCASECHECK, OnMatchcaseChkClick)
	ON_BN_CLICKED(IDC_FINDDIALOG_REGULAREXPRESSIONCHECK, OnRegularChkClick)
	ON_BN_CLICKED(IDC_FINDDIALOG_SEARCHALLOPENDOCUMENTSCHECK, OnSearchallChkClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDialog message handlers

BOOL CFindDialog::OnInitDialog() 
{
	m_pDocView = NULL;

	CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)GetTopLevelFrame ();
	if (pMainFrame != NULL)
	{
		CMDIChildWnd* pChildFrame = pMainFrame->MDIGetActive();

		if (pChildFrame)
		{
			CView* pView = pChildFrame->GetActiveView();
				
			if (pView && pView->IsKindOf(RUNTIME_CLASS(CBCGCBDotNetExampleView)))
			{
				m_pDocView = (CBCGCBDotNetExampleView*)pView;
			}
		}
	}

	if (!m_pDocView)
	{
		ShowWindow(SW_HIDE);
		::PostMessage(this->GetSafeHwnd(),WM_CLOSE,0,0);
		return FALSE;
	}

	CDialog::OnInitDialog();

	if (m_pDocView)
	{
		m_bMatchCase = m_pDocView->m_bMatchCase;
	}

	m_menu.LoadMenu(IDR_POPUP_FIND);
	m_btnMenu.m_hMenu = m_menu.GetSubMenu(0)->GetSafeHmenu();
	m_btnMenu.m_bOSMenu = TRUE;
	m_btnMenu.m_bRightArrow = TRUE;
	m_btnMenu.SetWindowText(_T(""));

	CStringList findList;
	m_pDocView->GetFindList(findList);

	for (POSITION pos = findList.GetTailPosition (); pos != NULL; m_wndCombo.InsertString(0, findList.GetPrev (pos)));

	if (!findList.IsEmpty())
	{
		m_wndCombo.SetCurSel(0);
	}

	m_wndCombo.m_pbMatchCase = &m_bMatchCase;

	CRect rect, rcCombo;
	GetClientRect(&rect);
	m_wndCombo.GetWindowRect(&rcCombo);
	ScreenToClient(&rcCombo);

	rcCombo.bottom = rect.Height() + rcCombo.top;
	m_wndCombo.MoveWindow(rcCombo);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindDialog::OnMarkAll() 
{
	// TODO: Add your control notification handler code here
	
}

void CFindDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pDocView)
	{
		m_pDocView->m_bMatchCase = m_bMatchCase;
	}
}

void CFindDialog::OnMatchwholeChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) 
{
}

void CFindDialog::OnMatchcaseChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) 
{
	m_pDocView->m_bMatchCase = m_bMatchCase;
}

void CFindDialog::OnRegularChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) 
{
}

void CFindDialog::OnSearchallChkClick(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) 
{
}
