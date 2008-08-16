//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //*******************************************************************************

#include "stdafx.h"

#include "BCGPDialog.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPLocalResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPDialog, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog dialog

#pragma warning (disable : 4355)

CBCGPDialog::CBCGPDialog() :
	m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
				CDialog (nIDTemplate, pParent),
				m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
				CDialog(lpszTemplateName, pParentWnd),
				m_Impl (*this)
{
	CommonConstruct ();
}

#pragma warning (default : 4355)

//*************************************************************************************
void CBCGPDialog::CommonConstruct ()
{
	m_hBkgrBitmap = NULL;
	m_sizeBkgrBitmap = CSize (0, 0);
	m_BkgrLocation = (BackgroundLocation) -1;
	m_bAutoDestroyBmp = FALSE;
	m_bIsLocal = FALSE;
	m_pLocaRes = NULL;
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundColor (COLORREF color, BOOL bRepaint)
{
	if (m_brBkgr.GetSafeHandle () != NULL)
	{
		m_brBkgr.DeleteObject ();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush (color);
	}

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundImage (HBITMAP hBitmap, 
								BackgroundLocation location,
								BOOL bAutoDestroy,
								BOOL bRepaint)
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
	}

	m_hBkgrBitmap = hBitmap;
	m_BkgrLocation = location;
	m_bAutoDestroyBmp = bAutoDestroy;

	if (hBitmap != NULL)
	{
		BITMAP bmp;
		::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

		m_sizeBkgrBitmap = CSize (bmp.bmWidth, bmp.bmHeight);
	}
	else
	{
		m_sizeBkgrBitmap = CSize (0, 0);
	}

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*************************************************************************************
BOOL CBCGPDialog::SetBackgroundImage (UINT uiBmpResId,
									BackgroundLocation location,
									BOOL bRepaint)
{
	HBITMAP hBitmap = NULL;

	if (uiBmpResId != 0)
	{
		hBitmap = ::LoadBitmap (AfxGetResourceHandle (), 
										MAKEINTRESOURCE (uiBmpResId));
		if (hBitmap == NULL)
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	SetBackgroundImage (hBitmap, location, TRUE /* Autodestroy */, bRepaint);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CBCGPDialog, CDialog)
	//{{AFX_MSG_MAP(CBCGPDialog)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog message handlers

void CBCGPDialog::OnActivate(UINT nState, CWnd *pWndOther, BOOL /*bMinimized*/) 
{
	m_Impl.OnActivate (nState, pWndOther);
}
//*************************************************************************************
BOOL CBCGPDialog::OnNcActivate(BOOL bActive) 
{
	m_Impl.OnNcActivate (bActive);

	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	return (BOOL) DefWindowProc (WM_NCACTIVATE, bActive, 0L);
}
//**************************************************************************************
BOOL CBCGPDialog::OnEraseBkgnd(CDC* pDC) 
{
	if (m_brBkgr.GetSafeHandle () == NULL && m_hBkgrBitmap == NULL)
	{
		return CDialog::OnEraseBkgnd (pDC);
	}

	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_BkgrLocation != BACKGR_TILE || m_hBkgrBitmap == NULL)
	{
		if (m_brBkgr.GetSafeHandle () != NULL)
		{
			pDC->FillRect (rectClient, &m_brBkgr);
		}
		else
		{
			CDialog::OnEraseBkgnd (pDC);
		}
	}

	if (m_hBkgrBitmap == NULL)
	{
		return TRUE;
	}

	ASSERT (m_sizeBkgrBitmap != CSize (0, 0));

	if (m_BkgrLocation != BACKGR_TILE)
	{
		CPoint ptImage = rectClient.TopLeft ();

		switch (m_BkgrLocation)
		{
		case BACKGR_TOPLEFT:
			break;

		case BACKGR_TOPRIGHT:
			ptImage.x = rectClient.right - m_sizeBkgrBitmap.cx;
			break;

		case BACKGR_BOTTOMLEFT:
			ptImage.y = rectClient.bottom - m_sizeBkgrBitmap.cy;
			break;

		case BACKGR_BOTTOMRIGHT:
			ptImage.x = rectClient.right - m_sizeBkgrBitmap.cx;
			ptImage.y = rectClient.bottom - m_sizeBkgrBitmap.cy;
			break;
		}

		pDC->DrawState (ptImage, m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
	}
	else
	{
		// Tile background image:
		for (int x = rectClient.left; x < rectClient.Width (); x += m_sizeBkgrBitmap.cx)
		{
			for (int y = rectClient.top; y < rectClient.Height (); y += m_sizeBkgrBitmap.cy)
			{
				pDC->DrawState (CPoint (x, y), m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
			}
		}
	}

	return TRUE;
}
//**********************************************************************************
void CBCGPDialog::OnDestroy() 
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
		m_hBkgrBitmap = NULL;
	}

	m_Impl.OnDestroy ();

	CDialog::OnDestroy();
}
//***************************************************************************************
HBRUSH CBCGPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (m_brBkgr.GetSafeHandle () != NULL || m_hBkgrBitmap != NULL)
	{
		#define MAX_CLASS_NAME	255
		#define STATIC_CLASS	_T("Static")
		#define BUTTON_CLASS	_T("Button")

		if (nCtlColor == CTLCOLOR_STATIC)
		{
			TCHAR lpszClassName [MAX_CLASS_NAME + 1];

			::GetClassName (pWnd->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == STATIC_CLASS)
			{
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
			}

			if (strClass == BUTTON_CLASS)
			{
//				if ((pWnd->GetStyle () & BS_GROUPBOX) == 0)
				{
					pDC->SetBkMode(TRANSPARENT);
				}

				return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
			}
		}
	}	

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
//**************************************************************************************
BOOL CBCGPDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//**************************************************************************************
void CBCGPDialog::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//*************************************************************************************
BOOL CBCGPDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}
//*************************************************************************************
INT_PTR CBCGPDialog::DoModal() 
{
	if (m_bIsLocal)
	{
		m_pLocaRes = new CBCGPLocalResource ();
	}

	return CDialog::DoModal();
}
//*************************************************************************************
void CBCGPDialog::PreInitDialog()
{
	if (m_pLocaRes != NULL)
	{
		delete m_pLocaRes;
		m_pLocaRes = NULL;
	}
}
//*************************************************************************************
void CBCGPDialog::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	if (AfxGetMainWnd () == this)
	{
		globalData.UpdateSysColors ();
	}
}
//*************************************************************************************
void CBCGPDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
	if (AfxGetMainWnd () == this)
	{
		globalData.OnSettingChange ();
	}
}
