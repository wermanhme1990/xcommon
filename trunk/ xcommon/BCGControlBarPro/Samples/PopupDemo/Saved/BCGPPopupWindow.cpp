// BCGPPopupWindow.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPPopupWindow.h"
#include "MyVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPPopupWindow, CWnd)

static const int idClosePopupEvt = 1;

typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, 
              BYTE bAlpha, DWORD dwFlags);
lpfn g_pSetLayeredWindowAttributes;

//*************************************************************************************
void CBCGPPopupWndButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	CMyVisualManager* pVM = (CMyVisualManager*) CBCGPVisualManager::GetInstance ();
	pVM->OnErasePopupWindowButton (pDC, rectClient, this);
}
//*************************************************************************************
void CBCGPPopupWndButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState)
{
	CMyVisualManager* pVM = (CMyVisualManager*) CBCGPVisualManager::GetInstance ();
	pVM->OnDrawPopupWindowButtonBorder (pDC, rectClient, this);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWindow

CBCGPPopupWindow::CBCGPPopupWindow()
{
	m_nAutoCloseTime = 3000;
	m_nCaptionHeight = 7;
	m_bHasCloseButton = TRUE;
	m_hMenu = /*NULL*/(HMENU)123;

	m_nBtnMarginVert = 2;
	m_nBtnMarginHorz = 2;
}

CBCGPPopupWindow::~CBCGPPopupWindow()
{
}


BEGIN_MESSAGE_MAP(CBCGPPopupWindow, CWnd)
	//{{AFX_MSG_MAP(CBCGPPopupWindow)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWindow message handlers

BOOL CBCGPPopupWindow::Create(UINT uiDlgResID, CPoint ptPos) 
{
	CString strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1), NULL);

	CRect rectDummy (0, 0, 0, 0);

	if (!CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, strClassName, _T(""), 
		WS_POPUP, rectDummy, NULL, 0))
	{
		return FALSE;
	}

	if (!m_wndDlg.Create (_T(""), this, FALSE, 
		MAKEINTRESOURCE (uiDlgResID), WS_VISIBLE | WS_CHILD, 1, 0, 0))
	{
		return FALSE;
	}

	CSize sizeDialog = m_wndDlg.m_sizeDialog;

	CSize sizeBtn = CMenuImages::Size() + CSize (6, 6);
	BOOL bButtonsOnCaption = (sizeBtn.cy + 2 < m_nCaptionHeight);

	if (!bButtonsOnCaption && (m_bHasCloseButton || m_hMenu != NULL))
	{
		sizeDialog.cx += m_nBtnMarginHorz;

		if (m_bHasCloseButton)
		{
			sizeDialog.cx += sizeBtn.cx;
		}

		if (m_hMenu != NULL)
		{
			sizeDialog.cx += sizeBtn.cx;
		}
	}

	CSize sizePopup = sizeDialog;
	sizePopup.cy += m_nCaptionHeight + 2;
	sizePopup.cx += 2;

	CWnd* pBtnParent = bButtonsOnCaption ? (CWnd*) this : &m_wndDlg;

	int nBtnVertOffset = bButtonsOnCaption ?
		(m_nCaptionHeight - sizeBtn.cy) / 2 : m_nBtnMarginVert;

	CRect rectBtn = CRect (	CPoint (sizeDialog.cx - sizeBtn.cx - m_nBtnMarginHorz, nBtnVertOffset),
							sizeBtn);

	if (m_bHasCloseButton)
	{
		m_btnClose.Create (_T(""), WS_CHILD | WS_VISIBLE, rectBtn, pBtnParent, 
			(UINT) -1);

		m_btnClose.SetStdImage (CMenuImages::IdClose, CMenuImages::ImageBlack);
		m_btnClose.m_bDrawFocus = FALSE;
		m_btnClose.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		rectBtn.OffsetRect (-sizeBtn.cx, 0);
	}

	if (m_hMenu != NULL)
	{
		m_btnMenu.Create (_T(""), WS_CHILD | WS_VISIBLE, rectBtn, pBtnParent, 
			(UINT) -1);

		m_btnMenu.SetStdImage (CMenuImages::IdArowDownLarge, CMenuImages::ImageBlack);
		m_btnMenu.m_bDrawFocus = FALSE;
		m_btnMenu.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	}

	if (ptPos == CPoint (-1, -1))
	{
		CRect rectScreen;
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);

		ptPos.x = rectScreen.right - sizePopup.cx;
		ptPos.y = rectScreen.bottom - sizePopup.cy;
	}

	CRect rectPopup (ptPos, sizePopup);
	MoveWindow (rectPopup);

	m_wndDlg.MoveWindow (CRect (1, m_nCaptionHeight + 1, 
		sizeDialog.cx + 1, sizeDialog.cy + m_nCaptionHeight + 1));
	m_wndDlg.ShowWindow (SW_SHOWNOACTIVATE);

///////////////////////
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32,
              "SetLayeredWindowAttributes");

	ModifyStyleEx (0, WS_EX_LAYERED);
	g_pSetLayeredWindowAttributes (GetSafeHwnd (), 0,
                        /*400*/255, LWA_ALPHA);
////////////////////////

	SetWindowPos (&wndTop, -1, -1, -1, -1,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	SetTimer (idClosePopupEvt, m_nAutoCloseTime, NULL);
	return TRUE;
}

void CBCGPPopupWindow::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent = idClosePopupEvt)
	{
		if (m_wndDlg.HasFocus () || 
			GetFocus ()->GetSafeHwnd () == GetSafeHwnd ())
		{
			return;
		}

		SendMessage (WM_CLOSE);
		return;
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CBCGPPopupWindow::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	delete this;
}

BOOL CBCGPPopupWindow::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CBCGPPopupWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMyVisualManager* pVM = (CMyVisualManager*) CBCGPVisualManager::GetInstance ();
	
	CRect rectClient;
	GetClientRect (&rectClient);
	
//	pVM->OnFillPopupWindowBackground (&dc, rectClient);
	pVM->OnDrawPopupWindowBorder (&dc, rectClient);
	pVM->OnDrawPopupWindowCaption (&dc, GetCaptionRect ());
}

UINT CBCGPPopupWindow::OnNcHitTest(CPoint point) 
{
//	return CWnd::OnNcHitTest(point);
	return HTCAPTION;
}

CRect CBCGPPopupWindow::GetCaptionRect ()
{
	CRect rectClient;
	GetClientRect (&rectClient);
	
	CRect rectCaption = rectClient;

	rectCaption.DeflateRect (1, 1);
	rectCaption.bottom = rectCaption.top + m_nCaptionHeight;

	return rectCaption;
}

BOOL CBCGPPopupWindow::ProcessCommand (HWND hwnd)
{
	if (hwnd == m_btnClose.GetSafeHwnd ())
	{
		SendMessage (WM_CLOSE);
		return TRUE;
	}

	return FALSE;
}

BOOL CBCGPPopupWindow::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (ProcessCommand ((HWND)lParam))
	{
		return TRUE;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

