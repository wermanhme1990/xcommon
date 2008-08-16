// BCGPPopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPPopupDlg.h"
#include "BCGPPopupWindow.h"
#include "MyVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CLASS_NAME	255
#define STATIC_CLASS	_T("Static")
#define BUTTON_CLASS	_T("Button")

IMPLEMENT_DYNAMIC (CBCGPPopupDlg, CBCGPDialogBar)

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupDlg

CBCGPPopupDlg::CBCGPPopupDlg()
{
	//{{AFX_DATA_INIT(CBCGPPopupDlg)
	//}}AFX_DATA_INIT
	m_bInitizlized = FALSE;
	m_pParentPopup = NULL;
}

CBCGPPopupDlg::~CBCGPPopupDlg()
{
}


BEGIN_MESSAGE_MAP(CBCGPPopupDlg, CBCGPDialogBar)
	//{{AFX_MSG_MAP(CBCGPPopupDlg)
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupDlg message handlers

UINT CBCGPPopupDlg::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}

void CBCGPPopupDlg::DoDataExchange(CDataExchange* pDX) 
{
	CBCGPDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGPPopupDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_Button);
	//}}AFX_DATA_MAP
}

HBRUSH CBCGPPopupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

//	if (nCtlColor == CTLCOLOR_STATIC)
	{
		TCHAR lpszClassName [MAX_CLASS_NAME + 1];

		::GetClassName (pWnd->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
		CString strClass = lpszClassName;

//		if (strClass == STATIC_CLASS)
		{
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
		}

//		if (strClass == BUTTON_CLASS)
		{
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
		}
	}

	HBRUSH hbr = CBCGPDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CBCGPPopupDlg::OnEraseBkgnd(CDC* pDC) 
{
	CMyVisualManager* pVM = (CMyVisualManager*) CBCGPVisualManager::GetInstance ();
	
	CRect rectClient;
	GetClientRect (&rectClient);
	
	pVM->OnFillPopupWindowBackground (pDC, rectClient);
	return TRUE;
}

void CBCGPPopupDlg::OnPaint() 
{
	if (!m_bInitizlized)
	{
		Init ();
	}

	CPaintDC dc(this); // device context for painting
	
	CWnd* pWndChild = GetWindow (GW_CHILD);
	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		TCHAR lpszClassName [MAX_CLASS_NAME + 1];

		::GetClassName (pWndChild->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
		CString strClass = lpszClassName;

		if (strClass == STATIC_CLASS && (pWndChild->GetStyle () & SS_ICON))
		{
			CRect rectIcon;
			pWndChild->GetWindowRect (rectIcon);
			ScreenToClient (rectIcon);

			HICON hIcon = ((CStatic*) pWndChild)->GetIcon ();
			dc.DrawIcon (rectIcon.TopLeft (), hIcon);
		}

		pWndChild = pWndChild->GetNextWindow ();
	}
}

void CBCGPPopupDlg::Init ()
{
	CWnd* pWndChild = GetWindow (GW_CHILD);
	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		CBCGPButton* pButton = DYNAMIC_DOWNCAST(CBCGPButton, pWndChild);
		if (pButton != NULL)
		{
			pButton->m_bDrawFocus = FALSE;
		}
		else
		{
			TCHAR lpszClassName [MAX_CLASS_NAME + 1];

			::GetClassName (pWndChild->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == STATIC_CLASS && (pWndChild->GetStyle () & SS_ICON))
			{
				pWndChild->ShowWindow (SW_HIDE);
			}
		}

		pWndChild = pWndChild->GetNextWindow ();
	}

	m_bInitizlized = TRUE;
}

void CBCGPPopupDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBCGPDialogBar::OnLButtonDown(nFlags, point);

	GetParent ()->SendMessage (WM_LBUTTONDOWN, 0, MAKELPARAM (point.x, point.y));
	SetFocus ();
}

BOOL CBCGPPopupDlg::HasFocus () const
{
	if (GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	if (GetSafeHwnd () == GetFocus ()->GetSafeHwnd ())
	{
		return TRUE;
	}

	if (IsChild (GetFocus ()))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBCGPPopupDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	ASSERT_VALID (m_pParentPopup);

	if (m_pParentPopup->ProcessCommand ((HWND)lParam))
	{
		return TRUE;
	}
	
	return CBCGPDialogBar::OnCommand(wParam, lParam);
}

int CBCGPPopupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParentPopup = DYNAMIC_DOWNCAST (CBCGPPopupWindow, GetParent ());
	ASSERT_VALID (m_pParentPopup);
	
	return 0;
}
