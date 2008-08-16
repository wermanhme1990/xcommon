//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonEdit.cpp: implementation of the CBCGPRibbonEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonEdit.h"
#include "BCGPVisualManager.h"
#include "bcgglobals.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPToolbarComboBoxButton.h"
#include "trackmouse.h"
#include "BCGPPopupMenu.h"
#include "BCGPSpinButtonCtrl.h"
#include "BCGPDrawManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CBCGPRibbonSpinButtonCtrl : public CBCGPSpinButtonCtrl
{
public:
	CBCGPRibbonSpinButtonCtrl ()
	{
		m_bQuickAccessMode = FALSE;
	}

	virtual void OnDraw (CDC* pDC)
	{
		BOOL bIsDrawOnGlass = CBCGPToolBarImages::m_bIsDrawOnGlass;

		if (m_bQuickAccessMode)
		{
			CBCGPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST (CBCGPRibbonBar, GetParent ());
			if (pRibbonBar != NULL)
			{
				ASSERT_VALID (pRibbonBar);

				if (pRibbonBar->IsQuickAccessToolbarOnTop () &&
					pRibbonBar->IsTransparentCaption ())
				{
					CBCGPToolBarImages::m_bIsDrawOnGlass = TRUE;
				}
			}
		}

		CBCGPSpinButtonCtrl::OnDraw (pDC);

		CBCGPToolBarImages::m_bIsDrawOnGlass = bIsDrawOnGlass;
	}

	BOOL m_bQuickAccessMode;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonEdit

IMPLEMENT_DYNCREATE(CBCGPRibbonEdit, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonEdit::CBCGPRibbonEdit(
	UINT	uiID,
	int		nWidth,
	LPCTSTR lpszLabel, 
	int		nImage)
{
	CommonInit ();

	m_nID = uiID;
	m_nWidth = nWidth;
	m_nSmallImageIndex = nImage;
	
	SetText (lpszLabel);
}
//**************************************************************************
CBCGPRibbonEdit::CBCGPRibbonEdit()
{
	CommonInit ();
}
//**************************************************************************
void CBCGPRibbonEdit::CommonInit ()
{
	m_bIsEditFocused = FALSE;
	m_nWidth = 0;
	m_pWndEdit = NULL;
	m_pWndSpin = NULL;
	m_bForceDrawBorder = TRUE;
	m_bHasDropDownList = FALSE;
	m_bHasSpinButtons = FALSE;
	m_nMin = INT_MAX;
	m_nMax = INT_MAX;
	m_szMargin.cx = 1;
}
//**************************************************************************
CBCGPRibbonEdit::~CBCGPRibbonEdit()
{
	if (m_pWndEdit != NULL)
	{
		m_pWndEdit->DestroyWindow ();
		delete m_pWndEdit;
	}

	if (m_pWndSpin != NULL)
	{
		m_pWndSpin->DestroyWindow ();
		delete m_pWndSpin;
		m_pWndSpin = NULL;
	}
}
//**************************************************************************
CSize CBCGPRibbonEdit::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	TEXTMETRIC tm;
	pDC->GetTextMetrics (&tm);

	int cx = m_nWidth;
	int cy = tm.tmHeight + 3 * m_szMargin.cy + 1;

	if (!m_bQuickAccessMode)
	{
		CSize sizeImageSmall = GetImageSize (RibbonImageSmall);

		if (sizeImageSmall != CSize (0, 0))
		{
			cx += sizeImageSmall.cx + 2 * m_szMargin.cx;
			cy = max (cy, sizeImageSmall.cy);
		}

		if (m_sizeTextRight.cx > 0)
		{
			cx += m_sizeTextRight.cx + m_szMargin.cx;
			cy = max (cy, m_sizeTextRight.cy);

			if (sizeImageSmall == CSize (0, 0))
			{
				cx += m_szMargin.cx;
			}
		}
	}

	return CSize (cx, cy);
}
//**************************************************************************
CSize CBCGPRibbonEdit::GetCompactSize (CDC* pDC)
{
	ASSERT_VALID (this);
	return GetRegularSize (pDC);
}
//**************************************************************************
void CBCGPRibbonEdit::SetEditText (CString strText)
{
	ASSERT_VALID (this);

	if (m_strEdit != strText)
	{
		m_strEdit = strText;

		if (m_pWndEdit->GetSafeHwnd () != NULL)
		{
			m_pWndEdit->SetWindowText (m_strEdit);
		}

		Redraw ();
	}

	if (!m_bDontNotify)
	{
		CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
		if (pRibbonBar != NULL)
		{
			ASSERT_VALID (pRibbonBar);

			CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arButtons;
			pRibbonBar->GetElementsByID (m_nID, arButtons);

			for (int i = 0; i < arButtons.GetSize (); i++)
			{
				CBCGPRibbonEdit* pOther =
					DYNAMIC_DOWNCAST (CBCGPRibbonEdit, arButtons [i]);

				if (pOther != NULL && pOther != this)
				{
					ASSERT_VALID (pOther);

					pOther->m_bDontNotify = TRUE;
					pOther->SetEditText (strText);
					pOther->m_bDontNotify = FALSE;
				}
			}
		}
	}
}
//**************************************************************************
void CBCGPRibbonEdit::EnableSpinButtons (int nMin, int nMax)
{
	ASSERT_VALID (this);

	m_nMin = nMin;
	m_nMax = nMax;

	m_bHasSpinButtons = TRUE;

	if (m_pWndSpin->GetSafeHwnd () != NULL)
	{
	   m_pWndSpin->SetRange32 (m_nMin, m_nMax);
	}
}
//**************************************************************************
void CBCGPRibbonEdit::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (!m_bQuickAccessMode)
	{
		CSize sizeImageSmall = GetImageSize (RibbonImageSmall);

		int x = m_rect.left;

		if (sizeImageSmall != CSize (0, 0))
		{
			CRect rectImage = m_rect;
			rectImage.left += m_szMargin.cx;
			rectImage.right = rectImage.left + sizeImageSmall.cx;
			rectImage.OffsetRect (0, max (0, (rectImage.Height () - sizeImageSmall.cy) / 2));

			DrawImage (pDC, RibbonImageSmall, rectImage);

			x = rectImage.right;
		}

		if (m_sizeTextRight.cx > 0)
		{
			COLORREF clrTextOld = (COLORREF)-1;

			if (IsDisabled ())
			{
				clrTextOld = pDC->SetTextColor (
					CBCGPVisualManager::GetInstance ()->GetToolbarDisabledTextColor ());
			}

			CRect rectText = m_rect;
			rectText.left = x + m_szMargin.cx;

			DrawText (pDC, m_strText, rectText, DT_SINGLELINE | DT_VCENTER);

			if (clrTextOld != (COLORREF)-1)
			{
				pDC->SetTextColor (clrTextOld);
			}
		}
	}

	BOOL bIsHighlighted = m_bIsHighlighted;

	if (m_bIsFocused)
	{
		m_bIsHighlighted = TRUE;
	}

	if (IsDisabled ())
	{
		m_bIsHighlighted = FALSE;
	}

	CRect rectSaved = m_rect;
	CRect rectCommandSaved = m_rectCommand;
	m_rectCommand.left = m_rect.left = m_rect.right - m_nWidth;

	CBCGPVisualManager::GetInstance ()->OnFillRibbonButton (pDC, this);

	if (m_pWndEdit->GetSafeHwnd () == NULL)
	{
		CRect rectText = m_rectCommand;
		rectText.DeflateRect (m_szMargin);

		DrawText (pDC, m_strEdit, rectText, DT_SINGLELINE | DT_VCENTER);
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonButtonBorder 
			(pDC, this);

	m_bIsHighlighted = bIsHighlighted;
	m_rect = rectSaved;
	m_rectCommand = rectCommandSaved;
}
//****************************************************************************
void CBCGPRibbonEdit::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL /*bIsSelected*/,
									  BOOL /*bHighlighted*/)
{
	const int nEditWidth = rect.Height () * 2;

	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectImage = rect;
	rectImage.right = rect.left + nTextOffset;

	CSize sizeImageSmall = GetImageSize (RibbonImageSmall);
	if (sizeImageSmall != CSize (0, 0))
	{
		rectImage.DeflateRect (1, 0);
		rectImage.top += max (0, (rectImage.Height () - sizeImageSmall.cy) / 2);
		rectImage.bottom = rectImage.top + sizeImageSmall.cy;

		DrawImage (pDC, RibbonImageSmall, rectImage);
	}

	CRect rectEdit = rect;
	rectEdit.left = rectEdit.right - nEditWidth;
	rectEdit.DeflateRect (1, 1);

	rect.left += nTextOffset;
	rect.right = rectEdit.left;

	rect.DeflateRect (m_szMargin.cx, 0);

	pDC->DrawText (strText, rect, DT_VCENTER | DT_SINGLELINE);

	CRect rectSaved = m_rect;
	CRect rectCommandSaved = m_rectCommand;
	
	m_rect = rectEdit;
	m_rectCommand = m_rect;
	m_rectCommand.right -= 15;

	CBCGPVisualManager::GetInstance ()->OnFillRibbonButton (pDC, this);
	CBCGPVisualManager::GetInstance ()->OnDrawRibbonButtonBorder 
		(pDC, this);

	if (m_bHasDropDownList)
	{
		CBCGPToolbarComboBoxButton buttonDummy;
		CRect rectDropButton = rectEdit;
		rectDropButton.left = m_rectCommand.right;
		rectDropButton.DeflateRect (2, 2);

		CBCGPVisualManager::GetInstance ()->OnDrawComboDropButton (
			pDC, rectDropButton, FALSE, FALSE, FALSE, &buttonDummy);
	}
	else
	{
		CRect rectCaret = rectEdit;
		rectCaret.DeflateRect (3, 3);
		rectCaret.bottom--;

		rectCaret.right = rectCaret.left + 7;

		CPen* pOldPen = (CPen*) pDC->SelectStockObject (BLACK_PEN);

		pDC->MoveTo (rectCaret.left, rectCaret.top);
		pDC->LineTo (rectCaret.right, rectCaret.top);

		pDC->MoveTo (rectCaret.CenterPoint ().x, rectCaret.top);
		pDC->LineTo (rectCaret.CenterPoint ().x, rectCaret.bottom);

		pDC->MoveTo (rectCaret.left, rectCaret.bottom);
		pDC->LineTo (rectCaret.right, rectCaret.bottom);

		pDC->SelectObject (pOldPen);
	}

	m_rect = rectSaved;
	m_rectCommand = rectCommandSaved;
	m_bIsDisabled = bIsDisabled;
}
//**************************************************************************
void CBCGPRibbonEdit::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonDown (point);
}
//**************************************************************************
void CBCGPRibbonEdit::OnLButtonUp (CPoint /*point*/)
{
	ASSERT_VALID (this);
}
//**************************************************************************
void CBCGPRibbonEdit::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonEdit& src = (CBCGPRibbonEdit&) s;

	m_strEdit = src.m_strEdit;

	if (m_pWndEdit != NULL)
	{
		m_pWndEdit->DestroyWindow ();
		delete m_pWndEdit;
		m_pWndEdit = NULL;
	}

	if (m_pWndSpin != NULL)
	{
		m_pWndSpin->DestroyWindow ();
		delete m_pWndSpin;
		m_pWndSpin = NULL;
	}

	m_nWidth = src.m_nWidth;
	m_bHasSpinButtons = src.m_bHasSpinButtons;
	m_bHasDropDownList = src.m_bHasDropDownList;
	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
}
//**************************************************************************
void CBCGPRibbonEdit::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPRibbonButton::OnAfterChangeRect (pDC);

	if (m_rect.IsRectEmpty ())
	{
		if (m_pWndEdit->GetSafeHwnd () != NULL)
		{
			m_pWndEdit->ShowWindow (SW_HIDE);
		}

		if (m_pWndSpin->GetSafeHwnd () != NULL)
		{
			m_pWndSpin->ShowWindow (SW_HIDE);
		}

		return;
	}

	CRect rectCommandOld = m_rectCommand;

	m_Location = RibbonElementSingleInGroup;
	m_rectCommand = m_rect;

	if (m_pWndEdit == NULL)
	{
		DWORD dwEditStyle = WS_CHILD | ES_WANTRETURN | 
							ES_AUTOHSCROLL | WS_TABSTOP;

		CWnd* pWndParent = GetParentWnd ();
		ASSERT_VALID (pWndParent);

		if ((m_pWndEdit = CreateEdit (pWndParent, dwEditStyle)) == NULL)
		{
			return;
		}

		m_pWndEdit->SendMessage (EM_SETTEXTMODE, TM_PLAINTEXT);
		m_pWndEdit->SetEventMask (m_pWndEdit->GetEventMask () | ENM_CHANGE);
		m_pWndEdit->SetFont (GetTopLevelRibbonBar ()->GetFont ());
		m_pWndEdit->SetWindowText (m_strEdit);
	}

	if (rectCommandOld != m_rectCommand || !m_pWndEdit->IsWindowVisible ())
	{
		CRect rectEdit = m_rectCommand;
		rectEdit.left = rectEdit.right - m_nWidth;

		if (m_bHasSpinButtons)
		{
			rectEdit.DeflateRect (m_szMargin.cx, m_szMargin.cy, 2, m_szMargin.cy);
		}
		else
		{
			rectEdit.DeflateRect (m_szMargin.cx, m_szMargin.cy);
		}

		m_pWndEdit->SetWindowPos (NULL, 
			rectEdit.left, rectEdit.top,
			rectEdit.Width (), rectEdit.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);

		m_pWndEdit->ShowWindow (SW_SHOWNOACTIVATE);

		if (m_pWndSpin->GetSafeHwnd () != NULL)
		{
			m_pWndSpin->m_bQuickAccessMode = m_bQuickAccessMode;
			m_pWndSpin->SetBuddy (m_pWndEdit);
			m_pWndSpin->ShowWindow (SW_SHOWNOACTIVATE);
		}
	}
}
//****************************************************************************************
CBCGPRibbonEditCtrl* CBCGPRibbonEdit::CreateEdit (CWnd* pWndParent, DWORD dwEditStyle)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndParent);

	CBCGPRibbonEditCtrl* pWndEdit = new CBCGPRibbonEditCtrl (*this);

	if (!pWndEdit->Create (dwEditStyle, CRect (0, 0, 0, 0), 
							pWndParent, m_nID))
	{
		delete pWndEdit;
		return NULL;
	}

	if (m_bHasSpinButtons)
	{
		m_pWndSpin = new CBCGPRibbonSpinButtonCtrl;

		if (!m_pWndSpin->Create(
			WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_SETBUDDYINT,
			  CRect (0, 0, 0, 0), pWndParent, m_nID))
		{
			delete pWndEdit;
			delete m_pWndSpin;
			return NULL;
		}

	   m_pWndSpin->SetBuddy (pWndEdit);
	   m_pWndSpin->SetRange32 (m_nMin, m_nMax);
	}

	return pWndEdit;
}
//********************************************************************************
void CBCGPRibbonEdit::OnShow (BOOL bShow)
{
	ASSERT_VALID (this);

	if (m_pWndEdit->GetSafeHwnd () != NULL)
	{
		m_pWndEdit->ShowWindow (bShow ? SW_SHOWNOACTIVATE : SW_HIDE);
	}

	if (m_pWndSpin->GetSafeHwnd () != NULL)
	{
		m_pWndSpin->ShowWindow (bShow ? SW_SHOWNOACTIVATE : SW_HIDE);
	}
}
//********************************************************************************
void CBCGPRibbonEdit::Redraw ()
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::Redraw ();

	if (m_pWndEdit->GetSafeHwnd () != NULL &&
		m_pWndEdit->IsWindowVisible ())
	{
		m_pWndEdit->RedrawWindow ();
	}

	if (m_pWndSpin->GetSafeHwnd () != NULL &&
		m_pWndSpin->IsWindowVisible ())
	{
		m_pWndSpin->RedrawWindow ();
	}
}
//********************************************************************************
void CBCGPRibbonEdit::OnEnable (BOOL bEnable)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnEnable (bEnable);

	if (m_pWndEdit->GetSafeHwnd () != NULL)
	{
		m_pWndEdit->EnableWindow (bEnable);
	}

	if (m_pWndSpin->GetSafeHwnd () != NULL)
	{
		m_pWndSpin->EnableWindow (bEnable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonEditCtrl

CBCGPRibbonEditCtrl::CBCGPRibbonEditCtrl(CBCGPRibbonEdit& edit) :
	m_edit (edit)
{
	m_bTracked = FALSE;
	m_bIsContextMenu = FALSE;
}

CBCGPRibbonEditCtrl::~CBCGPRibbonEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CBCGPRibbonEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CBCGPRibbonEditCtrl)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonEditCtrl message handlers

BOOL CBCGPRibbonEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (m_bIsContextMenu)
	{
		return CRichEditCtrl::PreTranslateMessage(pMsg);
	}

	if (pMsg->message == WM_MOUSEMOVE && !m_edit.IsDisabled ())
	{
		if (!m_bTracked)
		{
			m_bTracked = TRUE;
			
			TRACKMOUSEEVENT trackmouseevent;
			trackmouseevent.cbSize = sizeof(trackmouseevent);
			trackmouseevent.dwFlags = TME_LEAVE;
			trackmouseevent.hwndTrack = GetSafeHwnd ();
			trackmouseevent.dwHoverTime = HOVER_DEFAULT;

			::BCGPTrackMouse (&trackmouseevent);

			RedrawWindow ();
		}

		if (!m_edit.IsHighlighted () &&
			m_edit.GetParentWnd () != NULL)
		{
			CPoint point;
			::GetCursorPos (&point);

			m_edit.GetParentWnd ()->ScreenToClient (&point);
			m_edit.GetParentWnd ()->SendMessage (WM_MOUSEMOVE, 0, MAKELPARAM (point.x, point.y));
		}
	}

	if (pMsg->message == WM_KEYDOWN && !m_edit.IsDisabled ())
	{
		switch (pMsg->wParam)
		{
		case VK_DOWN:
			if (!m_edit.IsDroppedDown ())
			{
				m_edit.DropDownList ();
				return TRUE;
			}

		case VK_UP:
		case VK_PRIOR:
		case VK_NEXT:
			if (m_edit.IsDroppedDown ())
			{
				::SendMessage (	CBCGPPopupMenu::GetActiveMenu ()->GetSafeHwnd (),
								WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
			break;

		case VK_TAB:
			if (GetParent () != NULL)
			{
				ASSERT_VALID (GetParent ());
				GetParent ()->GetNextDlgTabItem (this)->SetFocus ();
				return TRUE;
			}
			break;

		case VK_RETURN:
			if (!m_edit.IsDroppedDown ())
			{
				CString str;
				GetWindowText (str);

				m_edit.SetEditText (str);
				m_edit.NotifyCommand (TRUE);
			}

			if (GetTopLevelFrame () != NULL && !m_edit.IsDroppedDown ())
			{
				GetTopLevelFrame ()->SetFocus ();
				return TRUE;
			}
			break;

		case VK_ESCAPE:
			if (m_edit.IsDroppedDown () && CBCGPPopupMenu::GetActiveMenu () != NULL)
			{
				CBCGPPopupMenu::GetActiveMenu ()->SendMessage (WM_CLOSE);
				return TRUE;
			}

			if (!m_edit.IsDroppedDown ())
			{
				SetWindowText (m_strOldText);
				m_edit.SetEditText (m_strOldText);
			}

			if (GetTopLevelFrame () != NULL && !m_edit.IsDroppedDown ())
			{
				GetTopLevelFrame ()->SetFocus ();
				return TRUE;
			}
			break;
		}
	}

	return CRichEditCtrl::PreTranslateMessage(pMsg);
}
//*************************************************************************************
void CBCGPRibbonEditCtrl::OnChange()
{
	CString strText;
	GetWindowText (strText);

	m_edit.m_strEdit = strText;
}
//*************************************************************************************
void CBCGPRibbonEditCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	
	m_edit.m_bIsEditFocused = TRUE;
	m_edit.Redraw ();

	GetWindowText (m_strOldText);
}
//*************************************************************************************
void CBCGPRibbonEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	m_edit.m_bIsEditFocused = FALSE;
	m_edit.Redraw ();

	SetSel (0, 0);
	RedrawWindow ();

	GetWindowText (m_strOldText);
}
//*************************************************************************************
void CBCGPRibbonEditCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

	CString str;
	GetWindowText (str);

	CBCGPRibbonPanel* pPanel = m_edit.GetParentPanel ();

	CRect rect;
	GetClientRect (rect);

	COLORREF clrText = globalData.clrBarText;
	COLORREF clrSelBackground = globalData.clrHilite;
	COLORREF clrSelText = globalData.clrTextHilite;

	BOOL bIsDrawOnGlass = CBCGPToolBarImages::m_bIsDrawOnGlass;

	if (m_edit.m_bQuickAccessMode && m_edit.m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_edit.m_pRibbonBar);

		if (m_edit.m_pRibbonBar->IsQuickAccessToolbarOnTop () &&
			m_edit.m_pRibbonBar->IsTransparentCaption ())
		{
			CBCGPToolBarImages::m_bIsDrawOnGlass = TRUE;
		}
	}

	CBCGPVisualManager::GetInstance ()->OnFillRibbonEdit (pDC, this, rect, 
		m_edit.IsHighlighted () || m_edit.IsDroppedDown () || m_edit.IsFocused (),
		pPanel != NULL && pPanel->IsHighlighted (),
		m_edit.IsDisabled (),
		clrText, clrSelBackground, clrSelText);

	if (str.IsEmpty ())
	{
		return;
	}

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rect);
	pDC->SelectClipRgn (&rgn);

	pDC->SetBkMode (TRANSPARENT);

	CFont* pOldFont = pDC->SelectObject (m_edit.GetTopLevelRibbonBar ()->GetFont ());
	ASSERT_VALID (pOldFont);

	rect.left = GetCharPos (0).x;
	m_edit.DrawText (pDC, str, rect, DT_LEFT | DT_SINGLELINE, clrText);

	long nStart, nEnd;
	GetSel (nStart, nEnd);

	if (nEnd >= nStart)
	{
		CString strSel = GetSelText ();

		CRect rectSel = rect;
		rectSel.left = GetCharPos (nStart).x;

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CRect rectSelFill = rectSel;
			rectSelFill.right = GetCharPos (nEnd).x;

			int cySel = pDC->GetTextExtent (strSel).cy;

			rectSelFill.top = rectSelFill.CenterPoint ().y - cySel / 2 - 1;
			rectSelFill.bottom = rectSelFill.top + cySel + 2;

			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rectSelFill, clrSelBackground, (COLORREF)-1);
		}
		else
		{
			pDC->SetBkMode (OPAQUE);
			pDC->SetBkColor (clrSelBackground);
		}

		m_edit.DrawText (pDC, strSel, rectSel, DT_LEFT | DT_SINGLELINE, clrSelText);
	}

	pDC->SelectObject (pOldFont);
	pDC->SelectClipRgn (NULL);

	CBCGPToolBarImages::m_bIsDrawOnGlass = bIsDrawOnGlass;
}
//*************************************************************************************
void CBCGPRibbonEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nOldX = GetCharPos (0).x;
	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nOldX != GetCharPos (0).x)
	{
		RedrawWindow ();
	}
}
//*****************************************************************************************
LRESULT CBCGPRibbonEditCtrl::OnMouseLeave(WPARAM,LPARAM)
{
	if (m_edit.GetParentWnd () != NULL)
	{
		m_edit.GetParentWnd ()->SendMessage (WM_MOUSELEAVE);
	}

	return 0;
}
//*****************************************************************************************
void CBCGPRibbonEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return;
	}

	CBCGPRibbonBar* pRibbonBar = m_edit.GetTopLevelRibbonBar ();
	if (pRibbonBar != NULL)
	{
		ASSERT_VALID (pRibbonBar);

		m_bIsContextMenu = TRUE;

		pRibbonBar->OnEditContextMenu (this, point);

		m_bIsContextMenu = FALSE;
	}
}

#endif // BCGP_EXCLUDE_RIBBON
