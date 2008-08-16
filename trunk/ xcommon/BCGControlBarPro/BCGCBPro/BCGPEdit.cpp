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
//
// BCGPEdit.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "bcgglobals.h"
#include "BCGPToolBarImages.h"
#include "BCGPVisualManager.h"
#include "BCGProRes.h"
#include "BCGPLocalResource.h"
#include "BCGPShellManager.h"
#include "BCGPCalculator.h"
#include "BCGPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPEdit

CBCGPEdit::CBCGPEdit()
{
	m_rectBtn.SetRectEmpty ();
	m_bIsButtonPressed = FALSE;
	m_bIsButtonHighlighted = FALSE;
	m_bIsButtonCaptured = FALSE;
	m_Mode = BrowseMode_None;
	m_sizeImage = CSize (0, 0);
	m_pCalcPopup = NULL;
	m_nBrowseButtonWidth = 20;
	m_bDefaultImage = TRUE;
}

CBCGPEdit::~CBCGPEdit()
{
}

BEGIN_MESSAGE_MAP(CBCGPEdit, CEdit)
	//{{AFX_MSG_MAP(CBCGPEdit)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPEdit message handlers

void CBCGPEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bIsButtonCaptured)
	{
		ReleaseCapture ();

		m_bIsButtonPressed = FALSE;
		m_bIsButtonCaptured = FALSE;
		m_bIsButtonHighlighted = FALSE;

		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if (m_rectBtn.PtInRect (point))
		{
			OnBrowse ();
		}

		return;
	}
	
	CEdit::OnLButtonUp(nFlags, point);
}
//*************************************************************************************
void CBCGPEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bIsButtonCaptured)
	{
		BOOL bIsButtonPressed = m_rectBtn.PtInRect (point);
		if (bIsButtonPressed != m_bIsButtonPressed)
		{
			m_bIsButtonPressed = bIsButtonPressed;
			RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}

		return;
	}
	
	if (m_bIsButtonHighlighted)
	{
		if (!m_rectBtn.PtInRect (point))
		{
			m_bIsButtonHighlighted = FALSE;
			ReleaseCapture ();

			RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}
	
	CEdit::OnMouseMove(nFlags, point);
}
//*************************************************************************************
void CBCGPEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (m_Mode != BrowseMode_None)
	{
		lpncsp->rgrc [0].right -= m_nBrowseButtonWidth;
	}
}
//*************************************************************************************
void CBCGPEdit::OnNcPaint() 
{
	CEdit::OnNcPaint ();

	if (m_Mode == BrowseMode_None)
	{
		return;
	}

	CWindowDC dc (this);

	CRect rectWindow;
	GetWindowRect (rectWindow);

	m_rectBtn = rectWindow;
	m_rectBtn.left = m_rectBtn.right -  m_nBrowseButtonWidth;

	CRect rectClient;
	GetClientRect (rectClient);
	ClientToScreen (&rectClient);

	m_rectBtn.OffsetRect (rectClient.right + m_nBrowseButtonWidth - rectWindow.right, 0);
	m_rectBtn.top += rectClient.top - rectWindow.top;
	m_rectBtn.bottom -= rectWindow.bottom - rectClient.bottom;

	CRect rect = m_rectBtn;
	rect.OffsetRect (-rectWindow.left, -rectWindow.top);

	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect (&rect);

	dc.SelectClipRgn (&rgnClip);

	OnDrawBrowseButton (&dc, rect, m_bIsButtonPressed, m_bIsButtonHighlighted);

	dc.SelectClipRgn (NULL);

	ScreenToClient (&m_rectBtn);
}
//********************************************************************************
BCGNcHitTestType CBCGPEdit::OnNcHitTest(CPoint point) 
{
	CPoint ptClient = point;
	ScreenToClient (&ptClient);

	if (m_Mode != BrowseMode_None && m_rectBtn.PtInRect (ptClient))
	{
		return HTCAPTION;
	}
	
	return CEdit::OnNcHitTest(point);
}
//********************************************************************************
void CBCGPEdit::OnDrawBrowseButton (CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bHighlight)
{
	ASSERT (m_Mode != BrowseMode_None);
	ASSERT_VALID (pDC);

	CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

	if (bIsButtonPressed)
	{
		state = CBCGPVisualManager::ButtonsIsPressed;
	}
	else if (bHighlight)
	{
		state = CBCGPVisualManager::ButtonsIsHighlighted;
	}

	COLORREF clrText = globalData.clrBtnText;

	if (!CBCGPVisualManager::GetInstance ()->OnDrawBrowseButton (pDC, rect, this, state, clrText))
	{
		return;
	}

	int iImage = 0;

	if (m_ImageBrowse.GetSafeHandle () != NULL)
	{
		if (m_bDefaultImage)
		{
			switch (m_Mode)
			{
			case BrowseMode_Folder:
				iImage = 0;
				break;

			case BrowseMode_File:
				iImage = 1;
				break;

			case BrowseMode_Calculator:
				iImage = 2;
				break;
			}
		}

		CPoint ptImage;
		ptImage.x = rect.CenterPoint ().x - m_sizeImage.cx / 2;
		ptImage.y = rect.CenterPoint ().y - m_sizeImage.cy / 2;

		if (bIsButtonPressed && CBCGPVisualManager::GetInstance ()->IsOffsetPressedButton ())
		{
			ptImage.x++;
			ptImage.y++;
		}

		m_ImageBrowse.Draw (pDC, iImage, ptImage, ILD_NORMAL);
	}
	else if (!m_strLabel.IsEmpty ())
	{
		COLORREF clrTextOld = pDC->SetTextColor (clrText);
		int nTextMode = pDC->SetBkMode (TRANSPARENT);
		CFont* pFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);

		CRect rectText = rect;
		rectText.DeflateRect (1, 2);
		rectText.OffsetRect (0, -2);

		if (bIsButtonPressed)
		{
			rectText.OffsetRect (1, 1);
		}

		pDC->DrawText (m_strLabel, rectText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		pDC->SetTextColor (clrTextOld);
		pDC->SetBkMode (nTextMode);
		pDC->SelectObject (pFont);
	}
}
//********************************************************************************
void CBCGPEdit::EnableBrowseButton (BOOL bEnable/* = TRUE*/,
									LPCTSTR szLabel/* = _T("...")*/)
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);
	ASSERT (szLabel != NULL);

	m_Mode = bEnable ? BrowseMode_Default : BrowseMode_None;
	m_strLabel = szLabel;

	m_ImageBrowse.DeleteImageList ();
	m_sizeImage = CSize (0, 0);

	OnChangeLayout ();
}
//********************************************************************************
void CBCGPEdit::OnChangeLayout ()
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);

	m_nBrowseButtonWidth = max (20, m_sizeImage.cx + 8);

	SetWindowPos (NULL, 0, 0, 0, 0, 
		SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	if (m_Mode != BrowseMode_None)
	{
		GetWindowRect (m_rectBtn);
		m_rectBtn.left = m_rectBtn.right -  m_nBrowseButtonWidth;

		ScreenToClient (&m_rectBtn);
	}
	else
	{
		m_rectBtn.SetRectEmpty ();
	}
}
//********************************************************************************
void CBCGPEdit::OnNcLButtonDblClk(UINT /*nHitTest*/, CPoint /*point*/)
{
}
//********************************************************************************
void CBCGPEdit::OnBrowse ()
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);

	switch (m_Mode)
	{
	case BrowseMode_Folder:
#ifndef BCGP_EXCLUDE_SHELL
		if (g_pShellManager != NULL)
		{
			CString strFolder;
			GetWindowText (strFolder);

			CString strResult;
			if (g_pShellManager->BrowseForFolder (strResult, this, strFolder) &&
				(strResult != strFolder))
			{
				SetWindowText (strResult);
				SetModify (TRUE);
				OnAfterUpdate ();
			}
		}
		else
#endif
		{
			ASSERT (FALSE);
		}
		break;

	case BrowseMode_File:
		{
			CString strFile;
			GetWindowText (strFile);

			CFileDialog dlg (TRUE, m_strDefFileExt, strFile, 0, m_strFileFilter, NULL);
			if (dlg.DoModal () == IDOK &&
				strFile != dlg.GetPathName ())
			{
				SetWindowText (dlg.GetPathName ());
				SetModify (TRUE);
				OnAfterUpdate ();
			}

			if (GetParent () != NULL)
			{
				GetParent ()->RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
			}
		}
		break;

	case BrowseMode_Calculator:
		{
			if (m_pCalcPopup != NULL)
			{
				m_pCalcPopup->SendMessage (WM_CLOSE);
				m_pCalcPopup = NULL;
				return;
			}

			CString strValue;
			GetWindowText (strValue);

			double dblValue = 0.;
			if (!strValue.IsEmpty ())
			{
				strValue.Replace (_T(','), _T('.'));
				_stscanf (strValue, _T("%lf"), &dblValue);
			}

			m_pCalcPopup = new CBCGPCalculatorPopup (dblValue, 0, this);

			if (!m_lstCalcAdditionalCommands.IsEmpty ())
			{
				CBCGPCalculator* pCalc = DYNAMIC_DOWNCAST (CBCGPCalculator, 
					m_pCalcPopup->GetMenuBar());
				ASSERT_VALID (pCalc);

				pCalc->SetAdditionalCommands (m_lstCalcAdditionalCommands);
			}

			CRect rectWindow;
			GetWindowRect (rectWindow);

			if (!m_pCalcPopup->Create (this, rectWindow.left, rectWindow.bottom, NULL, TRUE))
			{
				ASSERT (FALSE);
				m_pCalcPopup = NULL;
			}
			else
			{
				m_pCalcPopup->GetMenuBar()->SetFocus ();
				
				CRect rect;
				m_pCalcPopup->GetWindowRect (&rect);
				m_pCalcPopup->UpdateShadow (&rect);

				return;
			}
		}
		break;
	}

	SetFocus ();
}
//********************************************************************************
void CBCGPEdit::SetBrowseButtonImage (HICON hIcon, BOOL bAutoDestroy)
{
	if (m_ImageBrowse.GetSafeHandle () != NULL)
	{
		m_ImageBrowse.DeleteImageList ();
	}

	if (hIcon == NULL)
	{
		m_sizeImage = CSize (0, 0);
		return;
	}

	ICONINFO info;
	::GetIconInfo (hIcon, &info);

	BITMAP bmp;
	::GetObject (info.hbmColor, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	::DeleteObject (info.hbmColor);
	::DeleteObject (info.hbmMask);

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_ImageBrowse.Create (bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);
	m_ImageBrowse.Add (hIcon);

	m_bDefaultImage = FALSE;

	if (bAutoDestroy)
	{
		::DestroyIcon (hIcon);
	}
}
//********************************************************************************
void CBCGPEdit::SetBrowseButtonImage (HBITMAP hBitmap, BOOL bAutoDestroy)
{
	if (m_ImageBrowse.GetSafeHandle () != NULL)
	{
		m_ImageBrowse.DeleteImageList ();
	}

	if (hBitmap == NULL)
	{
		m_sizeImage = CSize (0, 0);
		return;
	}

	BITMAP bmp;
	::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_ImageBrowse.Create (bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);

	HBITMAP hbmpCopy = (HBITMAP) ::CopyImage (hBitmap, IMAGE_BITMAP, 0, 0, 0);
	m_ImageBrowse.Add (CBitmap::FromHandle (hbmpCopy), RGB (192, 192, 192));

	::DeleteObject (hbmpCopy);

	m_bDefaultImage = FALSE;

	if (bAutoDestroy)
	{
		::DeleteObject (hBitmap);
	}
}
//********************************************************************************
void CBCGPEdit::SetBrowseButtonImage (UINT uiBmpResId)
{
	if (m_ImageBrowse.GetSafeHandle () != NULL)
	{
		m_ImageBrowse.DeleteImageList ();
	}

	if (uiBmpResId == 0)
	{
		m_sizeImage = CSize (0, 0);
		return;
	}

	CBCGPToolBarImages images;
	if (!images.Load (uiBmpResId))
	{
		ASSERT (FALSE);
		return;
	}

	SetBrowseButtonImage ((HBITMAP) ::CopyImage (images.GetImageWell (), IMAGE_BITMAP, 0, 0, 0), TRUE /* bAutoDestroy */);
	m_bDefaultImage = FALSE;
}
//*********************************************************************************
void CBCGPEdit::EnableFileBrowseButton (LPCTSTR lpszDefExt/* = NULL*/, LPCTSTR lpszFilter/* = NULL*/)
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);

	m_Mode = BrowseMode_File;

	m_strDefFileExt = lpszDefExt == NULL ? _T("") : lpszDefExt;
	m_strFileFilter = lpszFilter == NULL ? _T("") : lpszFilter;

	SetIntenalImage ();

	OnChangeLayout ();
}
//*********************************************************************************
void CBCGPEdit::EnableFolderBrowseButton ()
{
#ifdef BCGP_EXCLUDE_SHELL
	ASSERT (FALSE);
#else
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);
	ASSERT (g_pShellManager != NULL);	// You need to call CBCGPWorkspace::InitShellManager () first!

	m_Mode = BrowseMode_Folder;
	SetIntenalImage ();
	OnChangeLayout ();
#endif
}
//*********************************************************************************
void CBCGPEdit::EnableCalculatorButton (const CStringList* plstAdditionalCommands)
{
	m_Mode = BrowseMode_Calculator;
	m_lstCalcAdditionalCommands.RemoveAll ();

	if (plstAdditionalCommands != NULL)
	{
		m_lstCalcAdditionalCommands.AddTail ((CStringList*) plstAdditionalCommands);
	}

	SetIntenalImage ();
	OnChangeLayout ();
}
//********************************************************************************
void CBCGPEdit::SetIntenalImage ()
{
	if (m_ImageBrowse.GetSafeHandle () != NULL)
	{
		m_ImageBrowse.DeleteImageList ();
	}

	CBCGPLocalResource	lr;

	UINT uiImageListResID = globalData.Is32BitIcons () ? IDB_BCGBARRES_BROWSE32 : IDB_BCGBARRES_BROWSE;

	LPCTSTR lpszResourceName = MAKEINTRESOURCE (uiImageListResID);
	ASSERT(lpszResourceName != NULL);

	HBITMAP hbmp = (HBITMAP) ::LoadImage (
		AfxGetResourceHandle (),
		lpszResourceName,
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION);

	if (hbmp == NULL)
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiImageListResID);
	}

	BITMAP bmpObj;
	::GetObject (hbmp, sizeof (BITMAP), &bmpObj);

	UINT nFlags = ILC_MASK;

	switch (bmpObj.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_ImageBrowse.Create (16, 16, nFlags, 0, 0);
	m_ImageBrowse.Add (CBitmap::FromHandle (hbmp), RGB (255, 0, 255));

	m_sizeImage = CSize (16, 16);
	m_bDefaultImage = TRUE;
}
//********************************************************************************
void CBCGPEdit::OnAfterUpdate ()
{
	if (GetOwner () == NULL)
	{
		return;
	}

	GetOwner ()->PostMessage (EN_CHANGE, GetDlgCtrlID (), (LPARAM) GetSafeHwnd ());
	GetOwner ()->PostMessage (EN_UPDATE, GetDlgCtrlID (), (LPARAM) GetSafeHwnd ());
}
//**********************************************************************************
void CBCGPEdit::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_bIsButtonCaptured)
	{
		CPoint ptClient = point;
		ScreenToClient (&ptClient);

		if (m_rectBtn.PtInRect (ptClient))
		{
			SetCapture ();
			m_bIsButtonHighlighted = TRUE;

			RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}
	
	CEdit::OnNcMouseMove(nHitTest, point);
}
//*********************************************************************************
void CBCGPEdit::OnCancelMode() 
{
	CEdit::OnCancelMode();
	
	if (IsWindowEnabled ())
	{
		ReleaseCapture ();
	}

	m_bIsButtonPressed = FALSE;
	m_bIsButtonCaptured = FALSE;
	m_bIsButtonHighlighted = FALSE;

	RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}
//********************************************************************************
void CBCGPEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_Mode != BrowseMode_None && m_rectBtn.PtInRect (point))
	{
		SetFocus ();

		m_bIsButtonPressed = TRUE;
		m_bIsButtonCaptured = TRUE;

		SetCapture ();

		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		return;
	}
	
	CEdit::OnLButtonDown(nFlags, point);
}
//********************************************************************************
BOOL CBCGPEdit::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
		if (m_Mode != BrowseMode_None && 
			(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RIGHT))
		{
			OnBrowse ();
			return TRUE;
		}
		break;

	case WM_CHAR:
		if (m_Mode == BrowseMode_Calculator && !FilterCalcKey ((int) pMsg->wParam))
		{
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}
//*********************************************************************************
BOOL CBCGPEdit::FilterCalcKey (int nChar)
{
	CString str;
	GetWindowText (str);

	if (isdigit (nChar))
	{
		return TRUE;
	}

	switch (nChar)
	{
	case _T('-'):
		{
			int nStartPos, nEndPos;
			GetSel (nStartPos, nEndPos);

			return str.Find (_T('-')) == -1 && nStartPos == 0;
		}

	case _T('.'):
	case _T(','):
		return str.FindOneOf (_T(".,")) == -1;
	}

	return !isprint (nChar);
}
//***********************************************************************************
void CBCGPEdit::OnCalculatorUserCommand (CBCGPCalculator* /*pCalculator*/, 
										 UINT /*uiCmd*/)
{
	ASSERT (FALSE);	// Must be implemented in derived class
}
