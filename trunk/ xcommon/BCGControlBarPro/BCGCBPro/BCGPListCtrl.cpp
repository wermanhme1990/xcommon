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
// BCGListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPListCtrl.h"
#include "BCGPDrawManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPListCtrl

IMPLEMENT_DYNAMIC(CBCGPListCtrl, CListCtrl)

CBCGPListCtrl::CBCGPListCtrl()
{
	m_iSortedColumn = -1;
	m_bAscending = TRUE;
	m_bMarkSortedColumn = FALSE;
	m_clrSortedColumn = (COLORREF)-1;
	m_hOldFont = NULL;
}
//*********************************************************************************
CBCGPListCtrl::~CBCGPListCtrl()
{
}

BEGIN_MESSAGE_MAP(CBCGPListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CBCGPListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_MESSAGE(WM_STYLECHANGED, OnStyleChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPListCtrl message handlers

BOOL CBCGPListCtrl::InitList ()
{
	InitHeader ();
	InitColors ();
	return TRUE;
}
//*********************************************************************************
void CBCGPListCtrl::InitHeader ()
{
	//---------------------------
	// Initialize header control:
	//---------------------------
	m_wndHeader.SubclassDlgItem (0, this);
}
//*********************************************************************************
void CBCGPListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		if (!InitList ())
		{
			ASSERT(FALSE);
		}
	}
}
//*********************************************************************************
int CBCGPListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!InitList ())
	{
		return -1;
	}

	return 0;
}
//*********************************************************************************
void CBCGPListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ASSERT (pNMListView != NULL);

	int iColumn = pNMListView->iSubItem;
	BOOL bShiftIsPressed = (::GetAsyncKeyState (VK_SHIFT) & 0x8000);
	int nColumnState = GetHeaderCtrl ().GetColumnState (iColumn);
	BOOL bAscending = TRUE;
	
	if (nColumnState != 0)
	{
		bAscending = nColumnState <= 0;
	}
	else
	{
		bAscending = FALSE;
	}

	Sort (iColumn, bAscending, bShiftIsPressed && IsMultipleSort ());
	*pResult = 0;
}
//*********************************************************************************
void CBCGPListCtrl::Sort (int iColumn, BOOL bAscending, BOOL bAdd)
{
	CWaitCursor wait;

	GetHeaderCtrl ().SetSortColumn (iColumn, bAscending, bAdd);

	m_iSortedColumn = iColumn;
	m_bAscending = bAscending;

	SortItems (CompareProc, (LPARAM) this);
}
//*********************************************************************************
void CBCGPListCtrl::SetSortColumn (int iColumn, BOOL bAscending, BOOL bAdd)
{
	GetHeaderCtrl ().SetSortColumn (iColumn, bAscending, bAdd);
}
//*********************************************************************************
void CBCGPListCtrl::RemoveSortColumn (int iColumn)
{
	GetHeaderCtrl ().RemoveSortColumn (iColumn);
}
//*********************************************************************************
void CBCGPListCtrl::EnableMultipleSort (BOOL bEnable)
{
	GetHeaderCtrl ().EnableMultipleSort (bEnable);
}
//*********************************************************************************
BOOL CBCGPListCtrl::IsMultipleSort () const
{
	return ((CBCGPListCtrl*) this)->GetHeaderCtrl ().IsMultipleSort ();
}
//*********************************************************************************
int CBCGPListCtrl::OnCompareItems (LPARAM /*lParam1*/, 
								  LPARAM /*lParam2*/, 
								  int /*iColumn*/)
{
	return 0;
}
//***************************************************************************************
int CALLBACK CBCGPListCtrl::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CBCGPListCtrl* pList = (CBCGPListCtrl*) lParamSort;
	ASSERT_VALID (pList);

	int nRes = pList->OnCompareItems (lParam1, lParam2, pList->m_iSortedColumn);
	if (!pList->m_bAscending)
	{
		nRes = -nRes;
	}

	return nRes;
}
//****************************************************************************************
void CBCGPListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	    *pResult = CDRF_NOTIFYITEMDRAW;
	    break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int iColumn = lplvcd->iSubItem;
			int iRow = (int) lplvcd->nmcd.dwItemSpec;

			lplvcd->clrTextBk = OnGetCellBkColor (iRow, iColumn);
			lplvcd->clrText = OnGetCellTextColor (iRow, iColumn);

			if (iColumn == m_iSortedColumn && m_bMarkSortedColumn &&
				lplvcd->clrTextBk == GetBkColor ())
			{
				lplvcd->clrTextBk = m_clrSortedColumn;
			}

			HFONT hFont = OnGetCellFont (	iRow, iColumn, 
											(DWORD) lplvcd->nmcd.lItemlParam);
				
			if (hFont != NULL)
			{
				m_hOldFont = (HFONT) SelectObject (lplvcd->nmcd.hdc, hFont);
				ASSERT (m_hOldFont != NULL);

				*pResult = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
			}
			else
			{
				*pResult = CDRF_DODEFAULT;
			}
		}
	    break;

	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
		if (m_hOldFont != NULL)
		{
			SelectObject (lplvcd->nmcd.hdc, m_hOldFont);
			m_hOldFont = NULL;
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//****************************************************************************************
void CBCGPListCtrl::EnableMarkSortedColumn (BOOL bMark/* = TRUE*/,
										   BOOL bRedraw/* = TRUE */)
{
	m_bMarkSortedColumn = bMark;

	if (GetSafeHwnd () != NULL && bRedraw)
	{
		RedrawWindow ();
	}
}
//****************************************************************************************
BOOL CBCGPListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = CListCtrl::OnEraseBkgnd(pDC);

	if (m_iSortedColumn >= 0 && m_bMarkSortedColumn)
	{
		CRect rectClient;
		GetClientRect (&rectClient);

		CRect rectHeader;
		GetHeaderCtrl ().GetItemRect (m_iSortedColumn, &rectHeader);
		GetHeaderCtrl ().MapWindowPoints (this, rectHeader);

		CRect rectColumn = rectClient;
		rectColumn.left = rectHeader.left;
		rectColumn.right = rectHeader.right;

		CBrush br (m_clrSortedColumn);
		pDC->FillRect (rectColumn, &br);
	}

	return bRes;
}
//*****************************************************************************************
void CBCGPListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	
	InitColors ();
	RedrawWindow ();
}
//****************************************************************************************
void CBCGPListCtrl::InitColors ()
{
	m_clrSortedColumn = CBCGPDrawManager::PixelAlpha (
			GetBkColor (), .97, .97, .97);
}
//***************************************************************************************
LRESULT CBCGPListCtrl::OnStyleChanged(WPARAM wp, LPARAM lp)
{
	int nStyleType = (int) wp;
	LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT) lp;

	CListCtrl::OnStyleChanged (nStyleType, lpStyleStruct);

	if ((lpStyleStruct->styleNew & LVS_REPORT) &&
		(lpStyleStruct->styleOld & LVS_REPORT) == 0)
	{
		if (GetHeaderCtrl ().GetSafeHwnd () == NULL)
		{
			InitHeader ();
		}
	}

	return 0;
}

void CBCGPListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	if (m_wndHeader.GetSafeHwnd () != NULL)
	{
		m_wndHeader.RedrawWindow ();
	}
}
