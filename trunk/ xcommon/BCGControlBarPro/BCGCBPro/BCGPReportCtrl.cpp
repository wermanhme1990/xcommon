// BCGPReportCtrl.cpp: implementation of the CBCGPReportCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPReportCtrl.h"

#ifndef BCGP_EXCLUDE_GRID_CTRL

#include "bcgglobals.h"

#ifndef _BCGPGRID_STANDALONE
	#include "BCGPVisualManager.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TEXT_MARGIN		3

/////////////////////////////////////////////////////////////////////////////
// CBCGPReportRow object

IMPLEMENT_DYNAMIC(CBCGPReportRow, CBCGPGridRow)

CBCGPReportRow::CBCGPReportRow()
	: CBCGPGridRow (), m_strName ()
{
	m_bNameIsTrancated = FALSE;
	m_bGroup = FALSE;
}
//******************************************************************************************
CBCGPReportRow::CBCGPReportRow (const CString& strGroupName, DWORD dwData)
	: CBCGPGridRow (0, dwData), m_strName (strGroupName)
{
	m_bNameIsTrancated = FALSE;
	m_bGroup = TRUE;
}
//******************************************************************************************
CBCGPReportRow::~CBCGPReportRow()
{
}
//******************************************************************************************
void CBCGPReportRow::OnDrawName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	if (HasValueField ())
	{
		return;
	}

	// Draw group row:
	COLORREF clrTextOld = pDC->GetTextColor ();//(COLORREF)-1;

	COLORREF clrText = (COLORREF)-1;
	if (IsSelected ())
	{
		CRect rectFill = rect;
		rectFill.top++;
		if (IsGroup ())
		{
			rectFill.DeflateRect (0, 0, 0, 1);
		}

		if (m_pWndList->m_ColorData.m_GroupColors.m_clrText != (COLORREF)-1)
		{
			clrText = m_pWndList->m_ColorData.m_GroupColors.m_clrText;
		}
		else
		{
			clrText = CBCGPVisualManager::GetInstance ()->OnFillReportCtrlRowBackground (
				m_pWndList, pDC, rectFill, IsSelected (), IsGroup ());
		}
	}
	else
	{
		if (IsGroup () && !HasValueField ())
		{
			if (m_pWndList->m_ColorData.m_GroupColors.m_clrText != (COLORREF)-1)
			{
				clrText = m_pWndList->m_ColorData.m_GroupColors.m_clrText;
			}
			else
			{
				clrText = CBCGPVisualManager::GetInstance ()->OnFillReportCtrlRowBackground (
					m_pWndList, pDC, rect, IsSelected (), IsGroup () && !HasValueField ());
			}
		}
		else
		{
			clrText = CBCGPVisualManager::GetInstance ()->OnFillReportCtrlRowBackground (
				m_pWndList, pDC, rect, IsSelected (), IsGroup () && !HasValueField ());
		}
	}

	rect.DeflateRect (TEXT_MARGIN, 0);
	if (IsGroup ())
	{
		rect.DeflateRect (0, (m_pWndList->m_nLargeRowHeight - m_pWndList->m_nRowHeight) / 2, 0, 0);
	}

	if (clrText != (COLORREF)-1)
	{
		pDC->SetTextColor (clrText);
	}

	pDC->DrawText (m_strName, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	if (clrText != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	m_bNameIsTrancated = pDC->GetTextExtent (m_strName).cx > rect.Width ();

/*	if (IsSelected () && IsGroup () && !HasValueField ())
	{
		CRect rectFocus = rect;
		rectFocus.top = rectFocus.CenterPoint ().y - nTextHeight / 2;
		rectFocus.bottom = rectFocus.top + nTextHeight;
		rectFocus.right = rect.right;
		rectFocus.InflateRect (2, 0);

		COLORREF clrShadow = m_pWndList->m_bControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;

		pDC->Draw3dRect (rectFocus, clrShadow, clrShadow);
	}
*/
}

void CBCGPReportRow::OnDrawExpandBox (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	ASSERT (IsGroup ());

	if (m_pWndList->m_bIsPrinting)
	{
		// map to printer metrics
		ASSERT_VALID (m_pWndList->m_pPrintDC);
		CSize szOne = m_pWndList->m_PrintParams.m_pageInfo.m_szOne;

		CRect rectFill = rect;
		rectFill.top += szOne.cx;
		if (IsGroup ())
		{
			rectFill.DeflateRect (0, 0, 0, szOne.cy);
		}

		CBrush brGroupBG (m_pWndList->m_clrPrintGroupBG);
		pDC->FillRect (rectFill, &brGroupBG);
	}
	else
	{
		if (IsSelected ())
		{
			CRect rectFill = rect;
			rectFill.top++;
			if (IsGroup ())
			{
				rectFill.DeflateRect (0, 0, 0, 1);
			}

			CBCGPVisualManager::GetInstance ()->OnFillReportCtrlRowBackground (
				m_pWndList, pDC, rectFill, IsSelected (), IsGroup ());
		}
	}

	if (IsGroup ())
	{
		rect.DeflateRect (0, (m_pWndList->m_nLargeRowHeight - m_pWndList->m_nRowHeight) / 2, 0, 0);
	}

	CBCGPGridRow::OnDrawExpandBox (pDC, rect);
}

void CBCGPReportRow::OnPrintName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	ASSERT (m_pWndList->m_bIsPrinting);

	if (HasValueField ())
	{
		return;
	}

	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
	int nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
	int nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	const int CALCULATED_TEXT_MARGIN = ::MulDiv (TEXT_MARGIN, nXMul, nXDiv);
	CSize szOne (::MulDiv (1, nXMul, nXDiv), ::MulDiv (1, nYMul, nYDiv));

	//----------------
	// Fill background
	//----------------
	CRect rectFill = rect;
	rectFill.top += szOne.cy;
	if (IsGroup ())
	{
		rectFill.DeflateRect (0, 0, 0, szOne.cy);
	}

	CBrush brGroupBG (m_pWndList->m_clrPrintGroupBG);
    pDC->FillRect (rectFill, &brGroupBG);

	//----------
	// Draw text
	//----------
	rect.DeflateRect (CALCULATED_TEXT_MARGIN, 0);
	if (IsGroup ())
	{
		rect.DeflateRect (0, (m_pWndList->m_nLargeRowHeight - m_pWndList->m_nRowHeight) / 2 , 0, 0);
	}

	COLORREF clrTextOld = pDC->SetTextColor (m_pWndList->m_clrPrintText);

	pDC->DrawText (m_strName, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	pDC->SetTextColor (clrTextOld);
}

int CBCGPReportRow::GetHierarchyLevel () const
{
	ASSERT_VALID (this);

	int nLevel = CBCGPGridRow::GetHierarchyLevel ();

	if (!IsGroup () && nLevel > 0)
	{
		nLevel--;
	}

	return nLevel;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPReportCtrl window

IMPLEMENT_DYNAMIC(CBCGPReportCtrl, CBCGPGridCtrl)

CBCGPReportCtrl::CBCGPReportCtrl()
{
	m_bDrawFocusRect = TRUE;
	m_bWholeRowSel = TRUE;
}

CBCGPReportCtrl::~CBCGPReportCtrl()
{
}

BEGIN_MESSAGE_MAP(CBCGPReportCtrl, CBCGPGridCtrl)
	//{{AFX_MSG_MAP(CBCGPReportCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBCGPReportCtrl::SetRowHeight ()
{
	CBCGPGridCtrl::SetRowHeight ();

	if (m_bIsPrinting)
	{
		ASSERT_VALID (m_pPrintDC);

		// map to printer metrics
		HDC hDCFrom = ::GetDC(NULL);
		int nYMul = m_pPrintDC->GetDeviceCaps(LOGPIXELSY);	// pixels in print dc
		int nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
		::ReleaseDC(NULL, hDCFrom);

		m_PrintParams.m_nLargeRowHeight = m_PrintParams.m_nRowHeight + ::MulDiv (14 + 2, nYMul, nYDiv);
	}
	else
	{
		m_nLargeRowHeight = m_nRowHeight + 14 + 2;
	}
}

int CBCGPReportCtrl::GetHierarchyOffset () const 
{
	int nLevel = m_Columns.GetGroupColumnCount ();
	if (nLevel > 0)
	{
		nLevel--;
	}

	return nLevel * m_nRowHeight;
}

void CBCGPReportCtrl::OnPaint() 
{
#ifndef _BCGPGRID_STANDALONE
	m_ColorData.m_GroupColors.m_clrBackground = 
		CBCGPVisualManager::GetInstance ()->GetReportCtrlGroupBackgoundColor ();
#else
	m_ColorData.m_GroupColors.m_clrBackground = globalData.clrBtnLight;
#endif

	CBCGPGridCtrl::OnPaint();
}

#endif // BCGP_EXCLUDE_GRID_CTRL
