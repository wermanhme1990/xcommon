//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2005 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPPlannerManagerCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPPlannerManagerCtrl.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerClockIcons.h"

#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewWorkWeek.h"
#include "BCGPPlannerViewWeek.h"
#include "BCGPPlannerViewMonth.h"

#include "BCGPPlannerPrintDay.h"
#include "BCGPPlannerPrintWeek.h"
#include "BCGPPlannerPrintMonth.h"
#include "BCGPPlannerPrintDual.h"

#include "BCGPPlannerManagerView.h"

#include "BCGPCalendarBar.h"

#include <math.h>

#ifndef _BCGPCALENDAR_STANDALONE
	#include "bcgprores.h"
#else
	#include "resource.h"
#endif

#include "BCGPLocalResource.h"

#include "BCGPAppointmentStorage.h"

#include "BCGPRecurrenceRules.h"
#include "BCGPTooltipManager.h"

#ifndef _BCGPCALENDAR_STANDALONE
	#include "BCGPVisualManager.h"
	#define visualManager	CBCGPVisualManager::GetInstance ()
#else
	#include "BCGPCalendarVisualManager.h"
	#define visualManager	CBCGPCalendarVisualManager::GetInstance ()
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// uncomment this string if you want drag & drop on another planner
#define BCGP_PLANNER_DND_TO_ANOTHER_PLANNER

#ifdef _UNICODE
	#define _TCF_TEXT	CF_UNICODETEXT
#else
	#define _TCF_TEXT	CF_TEXT
#endif

// wParam - not used, lParam - points to the appointment
UINT BCGP_PLANNER_ADD_APPOINTMENT			= ::RegisterWindowMessage (_T("BCGP_PLANNER_ADD_APPOINTMENT"));
// wParam - not used, lParam - points to the appointment
// Return value:
// -1L - cancel update
//  0L - update
UINT BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT	= ::RegisterWindowMessage (_T("BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT"));
// wParam - not used, lParam - points to the appointment
UINT BCGP_PLANNER_UPDATE_APPOINTMENT		= ::RegisterWindowMessage (_T("BCGP_PLANNER_UPDATE_APPOINTMENT"));
// wParam - not used, lParam - points to the appointment
// Return value:
// -1L - cancel delete
//	0L - delete single appointment or only occurrence of the series
//	1L - delete full series
UINT BCGP_PLANNER_REMOVE_APPOINTMENT		= ::RegisterWindowMessage (_T("BCGP_PLANNER_REMOVE_APPOINTMENT"));
// wParam, lParam - not used
UINT BCGP_PLANNER_REMOVE_ALL_APPOINTMENTS	= ::RegisterWindowMessage (_T("BCGP_PLANNER_REMOVE_ALL_APPOINTMENTS"));

// wParam - old type, lParam - new type
UINT BCGP_PLANNER_TYPE_CHANGED				= ::RegisterWindowMessage (_T("BCGP_PLANNER_TYPE_CHANGED"));
// wParam, lParam - not used
UINT BCGP_PLANNER_DATE_CHANGED				= ::RegisterWindowMessage (_T("BCGP_PLANNER_DATE_CHANGED"));

// wParam, lParam - see WM_LBUTTONDBLCLK
UINT BCGP_PLANNER_LBUTTONDBLCLK 			= ::RegisterWindowMessage (_T("BCGP_PLANNER_LBUTTONDBLCLK"));
// wParam, lParam - see WM_KEYDOWN
UINT BCGP_PLANNER_KEYDOWN					= ::RegisterWindowMessage (_T("BCGP_PLANNER_KEYDOWN"));
// click on "up-down" icon
// wParam - hit test code of icon, see CBCGPPlannerView
// Return value - return FALSE if default handler is needed
UINT BCGP_PLANNER_ICONUPDOWN_CLICK			= ::RegisterWindowMessage (_T("BCGP_PLANNER_ICONUPDOWN_CLICK"));

// wParam, lParam - not used
UINT BCGP_PLANNER_DROP_APPOINTMENTS         = ::RegisterWindowMessage (_T("BCGP_PLANNER_DROP_APPOINTMENTS"));
// wParam - drop effect code, lParam - not used
UINT BCGP_PLANNER_DRAG_APPOINTMENTS         = ::RegisterWindowMessage (_T("BCGP_PLANNER_DRAG_APPOINTMENTS"));

// wParam - operation code, lParam - not used
// Return value:
// -1L - cancel operation
//  0L - allow operation
UINT BCGP_PLANNER_BEGIN_CHANGE_OPERATION    = ::RegisterWindowMessage (_T("BCGP_PLANNER_BEGIN_CHANGE_OPERATION"));
// wParam - operation code, lParam - result of operation
UINT BCGP_PLANNER_END_CHANGE_OPERATION      = ::RegisterWindowMessage (_T("BCGP_PLANNER_END_CHANGE_OPERATION"));

IMPLEMENT_DYNCREATE(CBCGPPlannerManagerCtrl, CWnd)

CLIPFORMAT CBCGPPlannerManagerCtrl::s_ClpFormat = 0;
CString CBCGPPlannerManagerCtrl::s_ClpFormatName = _T("BCGPPlannerManagerClpFmt");

CSize CBCGPPlannerManagerCtrl::s_ImageSize (0, 0);
CImageList CBCGPPlannerManagerCtrl::s_ImageList;

CBCGPRecurrenceRuleRegistrator CBCGPPlannerManagerCtrl::s_RecurrenceRules;

int CBCGPPlannerManagerCtrl::m_nWeekStart = 1;

template <class TYPE>
void CopyList (const TYPE& lstSrc, TYPE& lstDst)
{
	lstDst.RemoveAll ();
	
	POSITION pos = lstSrc.GetHeadPosition ();

	while (pos != NULL)
	{
		lstDst.AddTail (lstSrc.GetNext (pos));
	}
}


int CBCGPPlannerManagerCtrl::GetSystemFirstDayOfWeek ()
{
	const int c_Size = 10;
	TCHAR szLocaleData[c_Size];
	::GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_ITIME, szLocaleData, c_Size);
	szLocaleData[1] = 0;

	return _ttoi (szLocaleData);
}

CString CBCGPPlannerManagerCtrl::GetSystemWeekDayName (int nDay)
{
	CString str;

	::GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SDAYNAME1 + nDay,
		str.GetBuffer (100), 100);
	str.ReleaseBuffer ();
	
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerManagerCtrl

CBCGPPlannerManagerCtrl::CBCGPPlannerManagerCtrl
(
	CRuntimeClass* pStorageClass,
	CRuntimeClass* pClockIconsClass
)
	: m_pAppsStorage	  (NULL)
	, m_pClockIcons 	  (NULL)
	, m_pPrinter		  (NULL)
	, m_pCurrentView	  (NULL)
	, m_Type			  (BCGP_PLANNER_TYPE_DAY)
	, m_ChangeOperation   (BCGP_PLANNER_CHANGE_OPERATION_NONE)
	, m_TimeDelta		  (CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA_60)
	, m_nFirstWorkingHour ( 9.0)
	, m_nLastWorkingHour  (18.0)
	, m_szUpDown		  (0, 0)
	, m_bCaptured		  (FALSE)
	, m_pWndLastCapture   (NULL)
	, m_bDragDrop		  (FALSE)
	, m_ptCaptureStart	  (0, 0)
	, m_ptCaptureCurrent  (0, 0)
	, m_dragEffect		  (DROPEFFECT_NONE)
	, m_clrBackground	  (CLR_DEFAULT)
	, m_bNotifyParent	  (TRUE)
	, m_pWndCalendar	  (NULL)
	, m_dateFirst		  ()
	, m_dateLast		  ()
	, m_bReadOnly		  (FALSE)
	, m_ToolTipCount	  (0)
	, m_bShowToolTip	  (TRUE)
	, m_bToolTipShowAlways(FALSE)
	, m_pToolTip		  (NULL)
	, m_bRedrawAfterDrop  (FALSE)
	, m_bDefaultDrawFlags (TRUE)
	, m_dwDrawFlags		  (0)
{
	if (GetClipboardFormat () == 0)
	{
		s_ClpFormat = (CLIPFORMAT)
			::RegisterClipboardFormat (GetClipboardFormatName ());
	}

	SetStorageRTC (pStorageClass);
	SetClockIconsRTC (pClockIconsClass);

	RegisterRule (RUNTIME_CLASS (CBCGPRecurrenceRuleDaily));
	RegisterRule (RUNTIME_CLASS (CBCGPRecurrenceRuleWeekly));
	RegisterRule (RUNTIME_CLASS (CBCGPRecurrenceRuleMonthly));
	RegisterRule (RUNTIME_CLASS (CBCGPRecurrenceRuleYearly));

	m_pViews [0] = new CBCGPPlannerViewDay;
	m_pViews [1] = new CBCGPPlannerViewWorkWeek;
	m_pViews [2] = new CBCGPPlannerViewWeek;
	m_pViews [3] = new CBCGPPlannerViewMonth;

	for (int i = 0; i < 4; i++)
	{
		ASSERT_VALID (m_pViews [3]);
	}

	{
		CBCGPLocalResource locaRes;

		CBitmap bmp;
		if (bmp.LoadBitmap (IDB_BCGBARRES_PLANNER_APP_UPDOWN))
		{
			BITMAP bm;
			bmp.GetBitmap (&bm);

			m_szUpDown.cx = bm.bmWidth / 2;
			m_szUpDown.cy = bm.bmHeight;

			m_ilUpDown.Create (m_szUpDown.cx, m_szUpDown.cy, ILC_COLOR | ILC_MASK, 2, 0);
			m_ilUpDown.Add (&bmp, RGB(255, 0, 255));
		}
	}

	m_nWeekStart = GetSystemFirstDayOfWeek () + 1;

	m_pCurrentView = GetView (m_Type);

	m_pCurrentView->OnActivate (this, NULL);

	m_pCurrentView->SetDate (COleDateTime::GetCurrentTime ());

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta ());

	COleDateTime dtSel (m_pCurrentView->GetDate ());
	dtSel += COleDateTimeSpan (0, GetFirstWorkingHour (), 
		(int)(GetFirstWorkingMinute () / nMinuts) * nMinuts, 0);

	m_pCurrentView->SetSelection (dtSel, dtSel);
}

CBCGPPlannerManagerCtrl::~CBCGPPlannerManagerCtrl()
{
	if (m_pAppsStorage != NULL)
	{
		delete m_pAppsStorage;
		m_pAppsStorage = NULL;
	}

	if (m_pClockIcons != NULL)
	{
		delete m_pClockIcons;
		m_pClockIcons = NULL;
	}

	if (m_pPrinter != NULL)
	{
		delete m_pPrinter;
		m_pPrinter = NULL;
	}
}

void CBCGPPlannerManagerCtrl::SetStorageRTC (CRuntimeClass* pStorageClass/* = NULL*/)
{
	if (m_pAppsStorage != NULL)
	{
		ASSERT_VALID (m_pAppsStorage);

		delete m_pAppsStorage;
		m_pAppsStorage = NULL;
	}

	if (pStorageClass != NULL)
	{
		if (!pStorageClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPAppointmentBaseStorage)))
		{
			ASSERT (FALSE);
			pStorageClass = NULL;
		}
	}
	
	if (pStorageClass == NULL)
	{
		pStorageClass = RUNTIME_CLASS (CBCGPAppointmentStorage);
	}

	m_pAppsStorage = (CBCGPAppointmentBaseStorage*)pStorageClass->CreateObject ();
	ASSERT_VALID (m_pAppsStorage);
}

void CBCGPPlannerManagerCtrl::SetClockIconsRTC (CRuntimeClass* pClockIconsClass/* = NULL*/)
{
	if (m_pClockIcons != NULL)
	{
		delete m_pClockIcons;
		m_pClockIcons = NULL;
	}

	if (pClockIconsClass != NULL)
	{
		if (!pClockIconsClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPPlannerClockIcons)))
		{
			ASSERT (FALSE);
			pClockIconsClass = NULL;
		}
	}

	if (pClockIconsClass == NULL)
	{
		pClockIconsClass = RUNTIME_CLASS (CBCGPPlannerClockIcons);
	}

	m_pClockIcons  = (CBCGPPlannerClockIcons*)pClockIconsClass->CreateObject ();
	ASSERT_VALID (m_pClockIcons);
}

BEGIN_MESSAGE_MAP(CBCGPPlannerManagerCtrl, CWnd)
	//{{AFX_MSG_MAP(CBCGPPlannerManagerCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_TIMECHANGE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedToolTipText)
	ON_REGISTERED_MESSAGE(BCGM_UPDATETOOLTIPS, OnBCGUpdateToolTips)
END_MESSAGE_MAP()

CLIPFORMAT CBCGPPlannerManagerCtrl::GetClipboardFormat ()
{
	return s_ClpFormat;
}

LPCTSTR CBCGPPlannerManagerCtrl::GetClipboardFormatName ()
{
	return s_ClpFormatName;
}

void CBCGPPlannerManagerCtrl::SetImages (UINT nResID, int cxImage, COLORREF clrTransparent)
{
	SetImages (MAKEINTRESOURCE (nResID), cxImage, clrTransparent);
}

void CBCGPPlannerManagerCtrl::SetImages (LPCTSTR szResID, int cxImage, COLORREF clrTransparent)
{
	if (s_ImageList.GetSafeHandle () != NULL)
	{
		s_ImageList.DeleteImageList ();
	}

	s_ImageSize = CSize (0, 0);

	if (szResID == NULL)
	{
		return;
	}

	CBitmap bmp;

	if (bmp.LoadBitmap (szResID))
	{
		BITMAP bm;
		ZeroMemory (&bm, sizeof (BITMAP));

		bmp.GetBitmap (&bm);

		UINT nFlags = clrTransparent == (COLORREF)-1 ? 0 : ILC_MASK;

		switch (bm.bmBitsPixel)
		{
		case 4 :
			nFlags |= ILC_COLOR4;
			break;
		case 8 :
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
			if (clrTransparent == (COLORREF)-1)
			{
				nFlags |= ILC_MASK;
			}
			break;
		default:
			ASSERT (FALSE);
		}

		s_ImageSize.cx = cxImage;
		s_ImageSize.cy = bm.bmHeight;

		int nCount = bm.bmWidth / cxImage;

		s_ImageList.Create (s_ImageSize.cx, s_ImageSize.cy, nFlags, nCount, 0);

		if ((nFlags & ILC_COLOR32) == ILC_COLOR32 &&
			clrTransparent == (COLORREF)-1)
		{
			s_ImageList.Add (&bmp, (CBitmap*) NULL);
		}
		else
		{
			s_ImageList.Add (&bmp, clrTransparent);
		}
	}
}

void CBCGPPlannerManagerCtrl::InitImages ()
{
	if (s_ImageSize != CSize (0, 0))
	{
		return;
	}

	CBCGPLocalResource locaRes;
	SetImages (IDB_BCGBARRES_CALENDAR_ICONS, 16);
}

void CBCGPPlannerManagerCtrl::DrawImageIcon (CDC* pDC, const CPoint& pt, int nIndex)
{
	ASSERT_VALID (pDC);
	ASSERT (nIndex != -1);

	InitImages ();

	if (pDC == NULL || pDC->GetSafeHdc () == NULL || 
		!(0 <= nIndex && nIndex < s_ImageList.GetImageCount ()))
	{
		return;
	}

	s_ImageList.Draw (pDC, nIndex, pt, ILD_NORMAL);
}


DWORD CBCGPPlannerManagerCtrl::RegisterRule (CRuntimeClass* pRuleClass)
{
	return s_RecurrenceRules.RegisterRule (pRuleClass);
}

CBCGPRecurrenceBaseRule* CBCGPPlannerManagerCtrl::CreateRule (DWORD ID)
{
	CBCGPRecurrenceBaseRule* pRule = s_RecurrenceRules.CreateRule (ID);

	return pRule;
}

void CBCGPPlannerManagerCtrl::GetRulesID (CBCGPRecurrenceRuleRegistrator::XBCGPRecurrenceRuleIDArray& arID)
{
	s_RecurrenceRules.GetRulesID (arID);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerManagerCtrl message handlers

BOOL CBCGPPlannerManagerCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	return CWnd::Create (	
		globalData.RegisterWindowClass (_T("BCGPPlannerManagerCtrl")),
		_T(""), dwStyle, rect, pParentWnd, nID, NULL);
}

void CBCGPPlannerManagerCtrl::OnDraw (CDC* pDC)
{
	CBCGPPlannerView* pView = GetCurrentView ();	

	CRect rectClient;
	GetClientRect (rectClient);
	rectClient.right = pView->m_rectApps.right;

	pView->OnPaint (pDC, rectClient);
}

void CBCGPPlannerManagerCtrl::OnPreparePrinting(CPrintInfo* pInfo, CRuntimeClass* pPrinterClass)
{
	ASSERT_VALID(this);

	if (pPrinterClass != NULL)
	{
		if (!pPrinterClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPPlannerPrint)))
		{
			ASSERT (FALSE);

			pPrinterClass = NULL;
		}
	}

	if (pPrinterClass == NULL)
	{
		switch (m_Type)
		{
		case BCGP_PLANNER_TYPE_DAY:
			if (GetViewDuration () == 1)
			{
				pPrinterClass = RUNTIME_CLASS (CBCGPPlannerPrintDay);
			}
			else
			{
				pPrinterClass = RUNTIME_CLASS (CBCGPPlannerPrintDual);
			}
			break;
		case BCGP_PLANNER_TYPE_WORK_WEEK:
			pPrinterClass = RUNTIME_CLASS (CBCGPPlannerPrintWeek);
			break;
		case BCGP_PLANNER_TYPE_WEEK:
			pPrinterClass = RUNTIME_CLASS (CBCGPPlannerPrintWeek);
			break;
		case BCGP_PLANNER_TYPE_MONTH:
			pPrinterClass = RUNTIME_CLASS (CBCGPPlannerPrintMonth);
			break;
		}
	}

	CBCGPPlannerPrint* pPrinter = (CBCGPPlannerPrint*)pPrinterClass->CreateObject ();

	ASSERT_VALID (pPrinter);

	if (m_pPrinter != NULL)
	{
		delete m_pPrinter;
		m_pPrinter = NULL;
	}

	m_pPrinter = pPrinter;

	m_pPrinter->PrepareInfo (pInfo);
}

void CBCGPPlannerManagerCtrl::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo, CRuntimeClass* pPrinterClass /*= NULL*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	
	if (m_pPrinter == NULL)
	{
		OnPreparePrinting(pInfo, pPrinterClass);
		ASSERT_VALID(m_pPrinter);
	}

	m_pPrinter->PreparePrinting (pDC, pInfo, this);
}

void CBCGPPlannerManagerCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	ASSERT_VALID(this);

	if (m_pPrinter != NULL)
	{
		delete m_pPrinter;
		m_pPrinter = NULL;
	}

	AdjustLayout ();
}

void CBCGPPlannerManagerCtrl::OnPrint (CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	ASSERT_VALID(m_pPrinter);

	if (m_pPrinter != NULL)
	{
		m_pPrinter->OnPaint (pDC, pInfo);
	}
}

void CBCGPPlannerManagerCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBCGPPlannerView* pView = GetCurrentView ();	

	CRect rectClient;
	GetClientRect (rectClient);
	rectClient.right = pView->m_rectApps.right;

	CDC memDC;
	if (memDC.CreateCompatibleDC (&dc))
	{
		CBitmap memBitmap;
		if (memBitmap.CreateCompatibleBitmap (&dc, rectClient.Width (), rectClient.Height ()))
		{
			memDC.SelectObject (&memBitmap);
		}
		else
		{
			memDC.DeleteDC ();
		}
	}

	BOOL bMemDC = memDC.GetSafeHdc () != NULL;

	CDC* pDC = bMemDC ? &memDC : &dc;

	pDC->SetBkMode (TRANSPARENT);

	HFONT hfontOld = pView->SetCurrFont (pDC);

	OnDraw (pDC);

	if (bMemDC)
	{
		dc.BitBlt (rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (),
			&memDC, 0, 0, SRCCOPY);
	}

	::SelectObject (pDC->GetSafeHdc (), hfontOld);
}

BOOL CBCGPPlannerManagerCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPPlannerManagerCtrl::OnDestroy() 
{
	CBCGPTooltipManager::DeleteToolTip (m_pToolTip);

	RemoveAllAppointments (FALSE);

	int i = 0;

	for (i = 0; i < 4; i++)
	{
		ASSERT_VALID (m_pViews [i]);

		if (GetCurrentView () == m_pViews [i])
		{
			m_pViews [i]->OnDeactivate (this);
		}
	}

	for (i = 0; i < 4; i++)
	{
		ASSERT_VALID (m_pViews [i]);
		delete m_pViews [i];
	}

	CWnd::OnDestroy();
}

int CBCGPPlannerManagerCtrl::GetViewDuration () const
{
	return GetCurrentView ()->GetViewDuration ();
}

const COleDateTime& CBCGPPlannerManagerCtrl::GetDate () const
{
	return GetCurrentView ()->GetDate ();
}

const COleDateTime& CBCGPPlannerManagerCtrl::GetDateStart () const
{
	return GetCurrentView ()->GetDateStart ();
}

const COleDateTime& CBCGPPlannerManagerCtrl::GetDateEnd () const
{
	return GetCurrentView ()->GetDateEnd ();
}

void CBCGPPlannerManagerCtrl::SetReadOnly (BOOL bReadOnly /*= TRUE*/)
{
	if (m_bReadOnly != bReadOnly)
	{
		m_bReadOnly = bReadOnly;

		if (GetSafeHwnd () != NULL)
		{
			GetCurrentView ()->StopEditAppointment ();
		}
	}
}

void CBCGPPlannerManagerCtrl::SetToday (BOOL bRedraw /*= TRUE*/)
{
	SetDate (COleDateTime::GetCurrentTime (), bRedraw);
}

void CBCGPPlannerManagerCtrl::SetDateInterval (const COleDateTime& date1, 
											   const COleDateTime& date2, 
											   BOOL bRedraw /*= TRUE*/)
{
	ASSERT (date2 >= date1);

	COleDateTimeSpan span (date2 - date1);

	SetRedraw (FALSE);

	CBCGPCalendar* pCalendar = m_pWndCalendar;
	m_pWndCalendar = NULL;

	if (span.GetTotalDays () == 6 && 
		date1 == CBCGPPlannerView::GetFirstWeekDay (date1, GetFirstDayOfWeek () + 1))
	{
		SetType (BCGP_PLANNER_TYPE_WEEK, FALSE);

		COleDateTime dt (CBCGPPlannerView::GetFirstWeekDay (date1, 
			CBCGPPlannerManagerCtrl::GetFirstDayOfWeek () + 1));

		// finding monday
		while (dt.GetDayOfWeek () != 2)
		{
			dt += COleDateTimeSpan (1, 0, 0, 0);
		}

		GetCurrentView ()->m_DateStart = dt;
		GetCurrentView ()->m_DateEnd   = dt + COleDateTimeSpan (6, 23, 59, 59);
		GetCurrentView ()->SetDate (dt);
	}
	else if (span.GetTotalDays () > 6)
	{
		SetType (BCGP_PLANNER_TYPE_MONTH, FALSE);
		GetCurrentView ()->SetDateInterval (date1, date2);
	}
	else
	{
		BOOL bUpdate = m_Type == BCGP_PLANNER_TYPE_DAY ||
			m_Type == BCGP_PLANNER_TYPE_WORK_WEEK;

		CBCGPPlannerView* pView = GetCurrentView ();

		COleDateTime dtSel1 (pView->GetSelectionStart ());
		COleDateTime dtSel2 (pView->GetSelectionEnd ());

		SetType (BCGP_PLANNER_TYPE_DAY, FALSE);
		GetCurrentView ()->SetDateInterval (date1, date2);

		if (bUpdate)
		{
			COleDateTime dt 	 (pView->GetDate ());
			COleDateTime dtStart (pView->GetDateStart ());
			COleDateTime dtEnd	 (pView->GetDateEnd ());

			BOOL bAdd = dtSel1 < dtSel2;
			COleDateTimeSpan span (bAdd ? dtSel2 - dtSel1 : dtSel1 - dtSel2);

			dt = COleDateTime (dt.GetYear (), dt.GetMonth (), dt.GetDay (),
							   dtSel1.GetHour (), dtSel1.GetMinute (), dtSel1.GetSecond ()); 

			if (dt != dtSel1)
			{
				dtSel1 = dt;
			}

			if (bAdd)
			{
				dtSel2 = dtSel1 + span;
			}
			else
			{
				dtSel2 = dtSel1;
				dtSel1 -= span;
			}

			if (dtSel1 < dtStart ||
				dtSel1 > dtEnd ||
				dtSel2 < dtStart ||
				dtSel2 > dtEnd)
			{
				const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta ());

				dtSel1 = COleDateTime (dtSel1.GetYear (), dtSel1.GetMonth (), dtSel1.GetDay (),
					GetFirstWorkingHour (), (int)(GetFirstWorkingMinute () / nMinuts) * nMinuts, 0);
				dtSel2 = dtSel1;
			}

			pView->SetSelection 
				(
					dtSel1, 
					dtSel2,
					FALSE
				);
		}
	}

	SetRedraw (TRUE);

	if (bRedraw)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	m_pWndCalendar = pCalendar;

	OnDateChanged ();
}

void CBCGPPlannerManagerCtrl::SetDate (const COleDateTime& date, BOOL bRedraw /*= TRUE*/)
{
	CBCGPPlannerView* pView = GetCurrentView ();
	ASSERT_VALID (pView);

	COleDateTime dt (date.GetYear (), date.GetMonth (), date.GetDay (), 0, 0, 0);

	if (dt == pView->GetDate ())
	{
		return;
	}

	COleDateTime dtStart (pView->GetDateStart ());
	COleDateTime dtEnd (pView->GetDateEnd ());

	if ((m_Type == BCGP_PLANNER_TYPE_WEEK ||
		 m_Type == BCGP_PLANNER_TYPE_MONTH))
	{
		COleDateTime dtS (pView->CalculateDateStart (dt));

		if (dtStart <= dt && dt <= dtEnd && dtS == dtStart)
		{
			ClearAppointmentSelection (FALSE);
			
			pView->m_Date = dt;
			pView->SetSelection (dt, dt, TRUE);
		}
		else
		{
			pView->SetDate (dt);
		}
	}
	else
	{
		SetRedraw (FALSE);

		COleDateTime dtSel1 (pView->GetSelectionStart ());
		COleDateTime dtSel2 (pView->GetSelectionEnd ());

		if (GetViewDuration () > 1 && 
			dtStart <= dt && dt <= dtEnd)
		{
			pView->m_Date = dt;
		}
		else
		{
			BOOL bSetDate = TRUE;

			if (m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
			{
				if (dtStart <= dt && 
					dt <= (dtStart + COleDateTimeSpan (6, 23, 59, 59)))
				{
					bSetDate = FALSE;
				}
			}
			
			if (bSetDate)
			{
				pView->SetDate (dt);

				if (m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
				{
					if (pView->GetDateEnd () < dt)
					{
						pView->m_Date = pView->GetDateStart ();
					}
				}
			}
			else
			{
				SetRedraw (TRUE);
				OnDateChanged ();
				return;
			}
		}

		dt = pView->GetDate ();

		dtStart = pView->GetDateStart ();
		dtEnd	= pView->GetDateEnd ();

		BOOL bAdd = dtSel1 < dtSel2;
		COleDateTimeSpan span (bAdd ? dtSel2 - dtSel1 : dtSel1 - dtSel2);

		dt = COleDateTime (dt.GetYear (), dt.GetMonth (), dt.GetDay (),
						   dtSel1.GetHour (), dtSel1.GetMinute (), dtSel1.GetSecond ()); 

		if (dt != dtSel1)
		{
			dtSel1 = dt;
		}

		if (bAdd)
		{
			dtSel2 = dtSel1 + span;
		}
		else
		{
			dtSel2 = dtSel1;
			dtSel1 -= span;
		}

		if (dtSel1 < dtStart ||
			dtSel1 > dtEnd ||
			dtSel2 < dtStart ||
			dtSel2 > dtEnd)
		{
			const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta ());

			dtSel1 = COleDateTime (dtSel1.GetYear (), dtSel1.GetMonth (), dtSel1.GetDay (),
				GetFirstWorkingHour (), (int)(GetFirstWorkingMinute () / nMinuts) * nMinuts, 0);
			dtSel2 = dtSel1;
		}

		pView->SetSelection 
			(
				dtSel1, 
				dtSel2,
				FALSE
			);

		SetRedraw (TRUE);

		if (bRedraw)
		{
			RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}

	OnDateChanged ();
}

void CBCGPPlannerManagerCtrl::OnDateChanged ()
{
	BOOL bUpdateState = TRUE;

	const COleDateTime date = GetDate ();

	if (m_pWndCalendar != NULL)
	{
		bUpdateState = FALSE;

		ASSERT_VALID (m_pWndCalendar);

		UpdateCalendarsSelection ();

		if (m_dateFirst != m_pWndCalendar->GetFirstDate () ||
			m_dateLast != m_pWndCalendar->GetLastDate ())
		{
			m_dateFirst = m_pWndCalendar->GetFirstDate ();
			m_dateLast = m_pWndCalendar->GetLastDate ();

			bUpdateState = TRUE;
		}
	}

	if (bUpdateState)
	{
		UpdateCalendarsState ();
	}

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_DATE_CHANGED, 0, 0);
	}
}

void CBCGPPlannerManagerCtrl::SetCompressWeekend (BOOL bCompress)
{
	GetCurrentView ()->SetCompressWeekend (bCompress);
	
	if (GetFirstDayOfWeek () != 1)
	{
		OnDateChanged ();
	}
}

BOOL CBCGPPlannerManagerCtrl::IsCompressWeekend () const
{
	return GetCurrentView ()->IsCompressWeekend ();
}

void CBCGPPlannerManagerCtrl::SetDrawTimeFinish (BOOL bDraw)
{
	GetCurrentView ()->SetDrawTimeFinish (bDraw);
}

void CBCGPPlannerManagerCtrl::SetNotifyParent (BOOL bNotifyParent)
{
	m_bNotifyParent = bNotifyParent;
}

BOOL CBCGPPlannerManagerCtrl::IsNotifyParent () const
{
	return m_bNotifyParent;
}

BOOL CBCGPPlannerManagerCtrl::IsDrawTimeFinish () const
{
	return GetCurrentView ()->IsDrawTimeFinish ();
}

void CBCGPPlannerManagerCtrl::SetDrawTimeAsIcons (BOOL bDraw)
{
	GetCurrentView ()->SetDrawTimeAsIcons (bDraw);
}

BOOL CBCGPPlannerManagerCtrl::IsDrawTimeAsIcons () const
{
	return GetCurrentView ()->IsDrawTimeAsIcons ();
}

void CBCGPPlannerManagerCtrl::SetSelection (const COleDateTime& sel1, const COleDateTime& sel2, BOOL bRedraw)
{
	GetCurrentView ()->SetSelection (sel1, sel2, bRedraw);
}

COleDateTime CBCGPPlannerManagerCtrl::GetSelectionStart () const
{
	return GetCurrentView ()->GetSelectionStart ();
}

COleDateTime CBCGPPlannerManagerCtrl::GetSelectionEnd () const
{
	return GetCurrentView ()->GetSelectionEnd ();
}

void CBCGPPlannerManagerCtrl::SetType (BCGP_PLANNER_TYPE type, BOOL bRedraw /*= TRUE*/)
{
	ASSERT (BCGP_PLANNER_TYPE_FIRST <= type);
	ASSERT (type <= BCGP_PLANNER_TYPE_LAST);

	if (m_Type == type)
	{
		return;
	}

	BCGP_PLANNER_TYPE oldType = m_Type;

	const BOOL bIsWndCreated = GetSafeHwnd () != NULL;

	if (bIsWndCreated)
	{
		SetRedraw (FALSE);
	}

	XBCGPAppointmentList lst;
	CopyList<XBCGPAppointmentList>(m_lsSelectedApps, lst);
	
	//COleDateTime date = GetCurrentView ()->GetDate ();
	CBCGPPlannerView* pOldView = m_pCurrentView;

	COleDateTime dtSel1 (pOldView->GetSelectionStart ());
	COleDateTime dtSel2 (pOldView->GetSelectionEnd ());

	if (pOldView != NULL)
	{
		pOldView->OnDeactivate (this);
	}

	m_Type = type;

	m_pCurrentView = GetView (m_Type);
	m_pCurrentView->OnActivate (this, pOldView);

	if (m_Type == BCGP_PLANNER_TYPE_DAY ||
		m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
	{
		if (oldType == BCGP_PLANNER_TYPE_WEEK || 
			oldType == BCGP_PLANNER_TYPE_MONTH || 
			dtSel1 < m_pCurrentView->GetDateStart () ||
			dtSel1 > m_pCurrentView->GetDateEnd () ||
			dtSel2 < m_pCurrentView->GetDateStart () ||
			dtSel2 > m_pCurrentView->GetDateEnd ())
		{
			const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta ());

			dtSel1 = m_pCurrentView->GetDateStart () + 
				COleDateTimeSpan (0, GetFirstWorkingHour (), 
					(int)(GetFirstWorkingMinute () / nMinuts) * nMinuts, 0);
			dtSel2 = dtSel1;
		}

		m_pCurrentView->SetSelection 
			(
				dtSel1, 
				dtSel2,
				FALSE
			);
	}

	RestoreAppointmentSelection (lst, FALSE);

	if (bIsWndCreated)
	{
		SetRedraw (TRUE);

		if (bRedraw)
		{
			RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}

	UpdateCalendarsSelection ();
	UpdateCalendarsState ();

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_TYPE_CHANGED, (WPARAM) oldType, (LPARAM) m_Type);
	}
}

BOOL CBCGPPlannerManagerCtrl::AddAppointment (CBCGPAppointment* pApp, BOOL bQuery, BOOL bRedraw)
{
	ASSERT_VALID (pApp);
	ASSERT_VALID (m_pAppsStorage);

	LRESULT lRes = 0L;

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		lRes = GetParent ()->SendMessage (BCGP_PLANNER_ADD_APPOINTMENT, 0, (LPARAM) pApp);
	}

	if (lRes == LRESULT(-1L))
	{
		return FALSE;
	}

	if (!m_pAppsStorage->Add (pApp))
	{
		return FALSE;
	}

	CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION action = 
		((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
			(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
			? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
			: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;

	if (bQuery)
	{
		QueryAppointments ();
	}

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
		{
			GetCurrentView ()->AdjustLayout (FALSE);
		}
		else
		{
			GetCurrentView ()->AdjustAppointments ();
		}

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	if (bQuery)
	{
		UpdateCalendarsState ();
	}

	return TRUE;
}

BOOL CBCGPPlannerManagerCtrl::UpdateAppointment (CBCGPAppointment* pApp, 
												 const COleDateTime& dtOld,
												 BOOL bQuery, BOOL bRedraw)
{
	ASSERT_VALID (pApp);
	ASSERT_VALID (m_pAppsStorage);

	BOOL bRecurrenceClone = pApp->IsRecurrence () && pApp->IsRecurrenceClone ();

	LRESULT lRes = 0L;

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		lRes = GetParent ()->SendMessage (BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT, 0, (LPARAM) pApp);
	}

	if (lRes == LRESULT(-1L))
	{
		return FALSE;
	}

	if (!m_pAppsStorage->Update (pApp, dtOld))
	{
		return FALSE;
	}

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_UPDATE_APPOINTMENT, 0, 
			(LPARAM)((bRecurrenceClone && bQuery) ? NULL : pApp));
	}

	if (bQuery)
	{
		QueryAppointments ();
	}
	else
	{
		SortAppointments (m_arQueryApps, (int) m_arQueryApps.GetSize ());
		GetCurrentView ()->AdjustAppointments ();
	}

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		GetCurrentView ()->AdjustAppointments ();
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	if (bQuery)
	{
		UpdateCalendarsState ();
	}

	return TRUE;
}

BOOL CBCGPPlannerManagerCtrl::RemoveAppointment (CBCGPAppointment* pApp, BOOL bQuery, BOOL bRedraw)
{
	ASSERT_VALID (pApp);
	ASSERT_VALID (m_pAppsStorage);

	if (pApp == NULL)
	{
		return FALSE;
	}

	LRESULT lRes = 0L;

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		lRes = GetParent ()->SendMessage (BCGP_PLANNER_REMOVE_APPOINTMENT, 0, (LPARAM) pApp);
	}

	if (lRes == LRESULT(-1L))
	{
		return FALSE;
	}

	CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION action = 
		((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
			(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
			? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
			: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;

	DWORD ID = 0;
	if (pApp->IsRecurrenceClone () && lRes == 1)
	{
		ID = pApp->GetRecurrenceID ();
		pApp = m_pAppsStorage->GetRecurrence (ID);

		if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS)
		{
			action = ((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
				(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
				? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
				: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;
		}
	}
	else if (pApp->IsRecurrence ())
	{
		ID = pApp->GetRecurrenceID ();
	}

	// remove series
	// remove all cloned appointments from query and selection
	if (ID != 0)
	{
		for (int i = 0; i < m_arQueryApps.GetSize (); i++)
		{
			CBCGPAppointment* p = m_arQueryApps[i];

			if (p != NULL && p != pApp && 
				p->IsRecurrenceClone () && p->GetRecurrenceID () == ID)
			{
				ASSERT_VALID(p);

				m_arQueryApps[i] = NULL;

				POSITION pos = m_lsSelectedApps.Find (p);
				if (pos != NULL)
				{
					m_lsSelectedApps.RemoveAt (pos);
				}

				delete p;
			}
		}
	}

	// remove appointment from query
	for (int i = 0; i < m_arQueryApps.GetSize (); i++)
	{
		if (m_arQueryApps[i] == pApp)
		{
			m_arQueryApps[i] = NULL;
			break;
		}
	}

	// remove appointment from selection
	POSITION pos = m_lsSelectedApps.Find (pApp);
	if (pos != NULL)
	{
		m_lsSelectedApps.RemoveAt (pos);
	}

	m_pAppsStorage->Remove (pApp);

	if (bQuery)
	{
		QueryAppointments ();
	}

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
		{
			GetCurrentView ()->AdjustLayout (FALSE);
		}
		else
		{
			GetCurrentView ()->AdjustAppointments ();
		}

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	if (bQuery)
	{
		UpdateCalendarsState ();
	}

	return TRUE;
}

void CBCGPPlannerManagerCtrl::RemoveSelectedAppointments (BOOL bQuery, BOOL bRedraw)
{
	ASSERT_VALID (m_pAppsStorage);

	CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION action = 
		CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_NONE;

	CBCGPPlannerView* pView = GetCurrentView ();
	ASSERT_VALID (pView);

	CList<DWORD, DWORD> lsID;

	POSITION pos = m_lsSelectedApps.GetHeadPosition ();
	while (pos != NULL)
	{
		CBCGPAppointment* pApp = m_lsSelectedApps.GetNext (pos);
		ASSERT_VALID (pApp);

		if (pApp == NULL)
		{
			continue;
		}

		if (action != CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
		{
			action = ((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
				(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
				? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
				: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;
		}

		LRESULT lRes = 0L;

		BOOL bNotify = TRUE;
		if (pApp->IsRecurrenceClone ())
		{
			bNotify = lsID.Find (pApp->GetRecurrenceID ()) == NULL;
		}

		if (m_bNotifyParent && bNotify && GetSafeHwnd () != NULL)
		{
			lRes = GetParent ()->SendMessage (BCGP_PLANNER_REMOVE_APPOINTMENT, 0, (LPARAM) pApp);
		}

		if (lRes != LRESULT(-1L))
		{
			if (pApp->IsRecurrenceClone () && lRes == 1)
			{
				DWORD ID = pApp->GetRecurrenceID ();

				pApp = m_pAppsStorage->GetRecurrence (ID);

				if (action != CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
				{
					action = ((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
						(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
						? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
						: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;
				}

				lsID.AddTail (ID);
			}

			m_pAppsStorage->Remove (pApp);

			for (int i = 0; i < m_arQueryApps.GetSize (); i++)
			{
				if (m_arQueryApps[i] == pApp)
				{
					m_arQueryApps[i] = NULL;
					break;
				}
			}
		}
	}

	if (action != CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
	{
		for (int i = 0; i < m_arQueryApps.GetSize (); i++)
		{
			CBCGPAppointment* pApp = m_arQueryApps[i];

			if (pApp == NULL)
			{
				continue;
			}

			if (pApp->IsRecurrenceClone ())
			{
				if (lsID.Find (pApp->GetRecurrenceID ()) != NULL)
				{
					action = ((pApp->IsAllDay () || pApp->IsMultiDay ()) &&
						(m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK))
						? CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT
						: CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;

					if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
					{
						break;
					}
				}
			}
		}
	}

	m_lsSelectedApps.RemoveAll ();

	if (bQuery)
	{
		QueryAppointments ();
	}

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_LAYOUT)
		{
			GetCurrentView ()->AdjustLayout (FALSE);
		}
		else
		{
			GetCurrentView ()->AdjustAppointments ();
		}
		
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	if (bQuery)
	{
		UpdateCalendarsState ();
	}
}

void CBCGPPlannerManagerCtrl::_InternalRemoveSelectedAppointments 
	(BOOL bQuery, BOOL bRedraw)
{
	BOOL bNotify = m_bNotifyParent;
	m_bNotifyParent = FALSE;

	RemoveSelectedAppointments (bQuery, bRedraw);

	m_bNotifyParent = bNotify;
}

void CBCGPPlannerManagerCtrl::ClearQuery ()
{
	for (int i = 0; i < m_arQueryApps.GetSize (); i++)
	{
		CBCGPAppointment* pApp = m_arQueryApps[i];

		if (pApp != NULL && pApp->IsRecurrenceClone ())
		{
			delete pApp;
		}
	}

	m_arQueryApps.RemoveAll ();
}

BOOL CBCGPPlannerManagerCtrl::RemoveAllAppointments (BOOL bRedraw)
{
	ASSERT_VALID (m_pAppsStorage);

	BOOL bIsEmpty = m_pAppsStorage->IsEmpty ();

	ClearQuery ();

	if (!m_pAppsStorage->RemoveAll ())
	{
		return FALSE;
	}

	m_lsSelectedApps.RemoveAll ();

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	if (m_bNotifyParent && !bIsEmpty && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_REMOVE_ALL_APPOINTMENTS, 0, 0);
	}

	UpdateCalendarsState ();
	return TRUE;
}

void CBCGPPlannerManagerCtrl::AdjustLayout (BOOL bRedraw)
{
	GetCurrentView ()->AdjustLayout (bRedraw);
}
//******************************************************************************************
int CBCGPPlannerManagerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndScrollVert.Create (WS_CHILD | WS_VISIBLE | SBS_VERT, CRect (0, 0, 0, 0), this, 
		BCGP_PLANNER_ID_SCROLL);

	m_DropTarget.Register (this);

	if (CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
									BCGP_TOOLTIP_TYPE_PLANNER))
	{
		m_pToolTip->Activate (TRUE);
		m_pToolTip->SetMaxTipWidth (150);
		m_pToolTip->SetWindowPos (&wndTop, -1, -1, -1, -1,
								SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	AdjustLayout (FALSE);
	return 0;
}
//******************************************************************************************
HFONT CBCGPPlannerManagerCtrl::SetCurrFont (CDC* pDC)
{
	ASSERT_VALID (pDC);
	
	CFont* pFont = GetFont ();

	if (pFont == NULL)
	{
		return NULL;
	}

	return (HFONT) ::SelectObject (pDC->GetSafeHdc (), pFont->GetSafeHandle () );
}
//******************************************************************************************
LRESULT CBCGPPlannerManagerCtrl::OnSetFont (WPARAM wParam, LPARAM /*lParam*/)
{
	GetCurrentView ()->SetFont ((HFONT) wParam);

	AdjustLayout ();
	return 0;
}
//******************************************************************************************
LRESULT CBCGPPlannerManagerCtrl::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT)GetCurrentView ()->GetFont ();
}
//******************************************************************************************
void CBCGPPlannerManagerCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout ();
}
//******************************************************************************************

CScrollBar* CBCGPPlannerManagerCtrl::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ || m_wndScrollVert.GetSafeHwnd () == NULL)
	{
		return NULL;
	}

	return (CScrollBar* ) &m_wndScrollVert;
}

BOOL CBCGPPlannerManagerCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	GetCurrentView ()->StopEditAppointment ();

	COleDateTime dtOld (GetCurrentView ()->GetDate ());

	BOOL bRes = GetCurrentView ()->OnMouseWheel (nFlags, zDelta, pt);

	if ((m_Type == BCGP_PLANNER_TYPE_WEEK ||
		m_Type == BCGP_PLANNER_TYPE_MONTH) &&
		GetCurrentView ()->GetDate () != dtOld && bRes)
	{
		OnDateChanged ();
	}

	return bRes;
}

void CBCGPPlannerManagerCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	COleDateTime dtOld (GetCurrentView ()->GetDate ());

	GetCurrentView ()->OnVScroll (nSBCode, nPos, pScrollBar);

	if ((m_Type == BCGP_PLANNER_TYPE_WEEK ||
		m_Type == BCGP_PLANNER_TYPE_MONTH) &&
		GetCurrentView ()->GetDate () != dtOld)
	{
		OnDateChanged ();
	}
}

void CBCGPPlannerManagerCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus ();
	
	CBCGPPlannerView::BCGP_PLANNER_HITTEST hit = GetCurrentView ()->HitTest (point);

	if (hit == CBCGPPlannerView::BCGP_PLANNER_HITTEST_ICON_UP ||
		hit == CBCGPPlannerView::BCGP_PLANNER_HITTEST_ICON_DOWN)
	{
		if (m_bNotifyParent && GetSafeHwnd () != NULL)
		{
			if (GetParent ()->SendMessage (BCGP_PLANNER_ICONUPDOWN_CLICK, (WPARAM) hit, 0) != 0L)
			{
				return;
			}
		}

		if (m_Type == BCGP_PLANNER_TYPE_WEEK ||
			m_Type == BCGP_PLANNER_TYPE_MONTH)
		{
			if (hit == CBCGPPlannerView::BCGP_PLANNER_HITTEST_ICON_DOWN)
			{
				SetRedraw (FALSE);

				COleDateTime dt (GetCurrentView ()->GetDateFromPoint (point));

				SetType (BCGP_PLANNER_TYPE_DAY, FALSE);

				if (dt != GetDate ())
				{
					SetDate (dt);
				}

				SetRedraw (TRUE);
				UpdateWindow ();

				return;
			}
		}
	}

	COleDateTime dtOld (GetCurrentView ()->GetDate ());

	if (!GetCurrentView ()->OnLButtonDown (nFlags, point))
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
	else
	{
		StartCapture ();
	}

	if (GetCurrentView ()->GetDate () != dtOld)
	{
		OnDateChanged ();
	}
}

void CBCGPPlannerManagerCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL bWasCaptured = IsCaptured ();

	COleDateTime dateStart = GetCurrentView ()->m_dtCaptureStart;
	COleDateTime dateFinish = GetCurrentView ()->m_dtCaptureCurrent;

	StopDragDrop ();

	StopCapture ();

	if (!GetCurrentView ()->OnLButtonUp (nFlags, point))
	{
		CWnd::OnLButtonUp(nFlags, point);
	}

	if (bWasCaptured && dateStart != dateFinish && m_lsSelectedApps.IsEmpty ())
	{
		if (dateStart > dateFinish)
		{
			COleDateTime dateTmp = dateFinish;
			dateFinish = dateStart;
			dateStart = dateTmp;
		}

		if (m_Type == BCGP_PLANNER_TYPE_DAY ||
			m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
		{
			dateFinish += GetCurrentView ()->GetMinimumSpan ();
		}

		OnSelectTimeInterval (dateStart, dateFinish);
	}
}

void CBCGPPlannerManagerCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (IsCaptured ())
	{
		m_ptCaptureCurrent = point;
	}

	if (IsCaptured () && CanStartDragDrop () && (nFlags & MK_LBUTTON) != 0)
	{
		StartDragDrop ();
	}

	BOOL bHandled = GetCurrentView ()->OnMouseMove (nFlags, point);

	if (!bHandled)
	{
		CWnd::OnMouseMove (nFlags, point);
	}
}

BOOL CBCGPPlannerManagerCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (GetCurrentView ()->OnSetCursor (pWnd, nHitTest, message))
	{
		return TRUE;
	}
	
	return CWnd::OnSetCursor (pWnd, nHitTest, message);
}

void CBCGPPlannerManagerCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus ();
	
	if (!GetCurrentView ()->OnRButtonDown (nFlags, point))
	{
		CWnd::OnRButtonDown(nFlags, point);
	}
}

void CBCGPPlannerManagerCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	COleDateTime dtOld (GetCurrentView ()->GetDate ());

	if (!GetCurrentView ()->OnKeyDown (nChar, nRepCnt, nFlags))
	{
		if (nChar == VK_TAB)
		{
			const int nCount = (int) m_arQueryApps.GetSize ();

			if (nCount > 0)
			{
				CBCGPAppointment *pApp = NULL;

				if (m_lsSelectedApps.GetCount () > 1)
				{
					pApp = m_lsSelectedApps.GetTail ();
					ClearAppointmentSelection (FALSE);
				}
				else
				{
					if (m_lsSelectedApps.GetCount () == 1)
					{
						pApp = m_lsSelectedApps.GetHead ();
					}

					if (pApp == NULL)
					{
						pApp = m_arQueryApps[0];
					}
					else
					{
						CBCGPAppointment *pApp2 = NULL;

						for (int i = 0; i < nCount; i++)
						{
							if (pApp == m_arQueryApps[i] && (i < nCount - 1))
							{
								pApp2 = m_arQueryApps[i + 1];
							}
						}

						if (pApp != pApp2)
						{
							pApp = pApp2;
							ClearAppointmentSelection (pApp == NULL);
						}
					}
				}

				if (pApp != NULL)
				{
					SelectAppointment (pApp, !pApp->IsSelected (), TRUE);
				}
			}
		}

		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	if (GetCurrentView ()->GetDate () != dtOld)
	{
		OnDateChanged ();
	}

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
	}
}

COleDateTime CBCGPPlannerManagerCtrl::GetDateFromPoint (const CPoint& point) const
{
	return GetCurrentView ()->GetDateFromPoint (point);
}

void CBCGPPlannerManagerCtrl::SetFirstDayOfWeek (int nDay)
{
	ASSERT_VALID (this);
	ASSERT (nDay >= 0 && nDay < 7);

	if (m_nWeekStart != nDay + 1)
	{
		m_nWeekStart = nDay + 1;

		CBCGPPlannerView* pView = GetCurrentView ();

		COleDateTimeSpan span (pView->GetDateEnd() - pView->GetDateStart ());
		pView->m_DateStart = pView->CalculateDateStart (pView->GetDate ());
		pView->m_DateEnd   = pView->GetDateStart () + span;
		SetDate (pView->GetDate (), FALSE);

		QueryAppointments ();

		AdjustLayout ();
	}
}

int CBCGPPlannerManagerCtrl::GetFirstDayOfWeek ()
{
	return m_nWeekStart - 1;
}

void CBCGPPlannerManagerCtrl::SetTimeDelta (CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA delta,
											BOOL bRedraw /*= TRUE*/)
{
	ASSERT(CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA_FIRST <= delta);
	ASSERT(delta <= CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA_LAST);

	if (m_TimeDelta != delta)
	{
		m_TimeDelta = delta;

		AdjustLayout (bRedraw);
	}
}

CBCGPPlannerView::BCGP_PLANNER_HITTEST
CBCGPPlannerManagerCtrl::HitTest (const CPoint& point) const
{
	return GetCurrentView ()->HitTest (point);
}

CBCGPPlannerView::BCGP_PLANNER_HITTEST
CBCGPPlannerManagerCtrl::HitTestArea (const CPoint& point) const
{
	return GetCurrentView ()->HitTestArea (point);
}

CBCGPPlannerView::BCGP_PLANNER_HITTEST
CBCGPPlannerManagerCtrl::HitTestAppointment (const CPoint& point) const
{
	return GetCurrentView ()->HitTestAppointment (point);
}

void CBCGPPlannerManagerCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	if (!GetCurrentView ()->OnTimer (nIDEvent))
	{
		CWnd::OnTimer(nIDEvent);
	}
}

void CBCGPPlannerManagerCtrl::OnTimeChange() 
{
	if (m_Type == BCGP_PLANNER_TYPE_DAY || m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
	{
		GetCurrentView ()->OnTimer (CBCGPPlannerViewDay::BCGP_PLANNER_TIMER_EVENT);
	}
}

void CBCGPPlannerManagerCtrl::SetWorkingHourInterval (int nFirstHour, int nLastHour, 
													  BOOL bRedraw /*= TRUE*/)
{
	ASSERT (0 <= (int)nFirstHour);
	ASSERT ((int)nLastHour <= 23);

	ASSERT (nFirstHour < nLastHour);

	SetWorkingHourMinuteInterval ((double)nFirstHour, (double)nLastHour, bRedraw);
}

void CBCGPPlannerManagerCtrl::SetWorkingHourMinuteInterval (double dStart, double dEnd, 
															BOOL bRedraw /*= TRUE*/)
{
	ASSERT (0 <= (int)dStart);
	ASSERT ((int)dEnd <= 23);

	ASSERT (0 <= (int)((dStart - (int)(dStart)) * 100.0));
	ASSERT ((int)((dStart - (int)(dStart)) * 100.0) <= 59);

	ASSERT (0 <= (int)((dEnd - (int)(dEnd)) * 100.0));
	ASSERT ((int)((dEnd - (int)(dEnd)) * 100.0) <= 59);

	ASSERT (dStart < dEnd);

	if (m_nFirstWorkingHour != dStart || m_nLastWorkingHour != dEnd)
	{
		m_nFirstWorkingHour = dStart;
		m_nLastWorkingHour	= dEnd;

		AdjustLayout (bRedraw);
	}
}

void CBCGPPlannerManagerCtrl::QueryAppointments ()
{
	COleDateTime date1 (GetCurrentView()->GetDateStart ());
	COleDateTime date2 (GetCurrentView()->GetDateEnd ());

	ASSERT (date1 <= date2);

	ClearAppointmentSelection (FALSE);

	ClearQuery ();

	QueryAppointments (m_arQueryApps, date1, date2);
}

void CBCGPPlannerManagerCtrl::QueryAppointments (XBCGPAppointmentArray& ar, 
												 const COleDateTime& date1, 
												 const COleDateTime& date2) const
{
	ASSERT_VALID (m_pAppsStorage);
	m_pAppsStorage->Query (ar, date1, date2);
}

void CBCGPPlannerManagerCtrl::ClearAppointmentSelection (BOOL bRedraw)
{
	int nOldCount = (int) m_lsSelectedApps.GetCount ();

	POSITION pos = m_lsSelectedApps.GetHeadPosition ();

	while (pos != NULL)
	{
		CBCGPAppointment* pApp = m_lsSelectedApps.GetNext (pos);

		pApp->SetSelected (FALSE);
	}

	m_lsSelectedApps.RemoveAll ();

	if (nOldCount != 0 && bRedraw)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CBCGPPlannerManagerCtrl::SelectAppointment (CBCGPAppointment* pApp, 
												 BOOL bSelect, BOOL bRedraw)
{
	ASSERT_VALID (pApp);

#ifdef _DEBUG
	if (bSelect)
	{
		ASSERT (!pApp->IsSelected ());
	}
	else
	{
		ASSERT (pApp->IsSelected ());
	}
#endif

	int nOldCount = (int) m_lsSelectedApps.GetCount ();

	POSITION pos = m_lsSelectedApps.Find (pApp);

	if (bSelect)
	{
		if (pos == NULL)
		{
			pApp->SetSelected (TRUE);
			m_lsSelectedApps.AddTail (pApp);
		}
	}
	else
	{
		if (pos != NULL)
		{
			pApp->SetSelected (FALSE);
			m_lsSelectedApps.RemoveAt (pos);
		}
	}

	if (m_lsSelectedApps.GetCount () != nOldCount && bRedraw)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CBCGPPlannerManagerCtrl::RestoreAppointmentSelection 
	(XBCGPAppointmentList& lst, BOOL bRedraw)
{
	ClearAppointmentSelection (FALSE);

	if (lst.GetCount () > 0)
	{
		for (int i = 0; i < m_arQueryApps.GetSize (); i++)
		{
			if (lst.Find (m_arQueryApps[i]) != NULL)
			{
				SelectAppointment (m_arQueryApps[i], TRUE, FALSE);
			}
		}
	}

	if (m_lsSelectedApps.GetCount () > 0 && bRedraw)
	{
		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

HICON CBCGPPlannerManagerCtrl::GetUpDownIcon(int nType)
{
	ASSERT (0 <= nType && nType <= 1);

	HICON hIcon = m_ilUpDown.ExtractIcon (nType);
	return hIcon;
}

BOOL CBCGPPlannerManagerCtrl::UpdateChangeOperation 
	(BCGP_PLANNER_CHANGE_OPERATION operation, BOOL bResult /*= FALSE*/)
{
	BOOL bRes = FALSE;

	if (m_ChangeOperation != operation)
	{
		if (m_ChangeOperation == BCGP_PLANNER_CHANGE_OPERATION_NONE)
		{
			m_ChangeOperation = operation;
			if(m_bNotifyParent && GetSafeHwnd () != NULL && GetParent ()->GetSafeHwnd () != NULL)
			{
				bRes = GetParent ()->SendMessage (BCGP_PLANNER_BEGIN_CHANGE_OPERATION, 
					(WPARAM)m_ChangeOperation, (LPARAM)FALSE) == 0L;
			}

			if(!bRes)
			{
				m_ChangeOperation = BCGP_PLANNER_CHANGE_OPERATION_NONE;
			}
		}
		else if (operation == BCGP_PLANNER_CHANGE_OPERATION_NONE)
		{
			if(m_bNotifyParent && GetSafeHwnd () != NULL && GetParent ()->GetSafeHwnd () != NULL)
			{
				GetParent ()->PostMessage (BCGP_PLANNER_END_CHANGE_OPERATION, 
					(WPARAM)m_ChangeOperation, (LPARAM)bResult);
			}
			m_ChangeOperation = operation;
		}
	}

	return bRes;
}

BOOL CBCGPPlannerManagerCtrl::EditCut ()
{
	CWnd* pWnd = GetDlgItem (BCGP_PLANNER_ID_INPLACE);

	if (pWnd != NULL)
	{
		pWnd->SendMessage (WM_CUT, 0, 0);
		return FALSE;
	}

	if (!IsEditCutEnabled ())
	{
		return FALSE;
	}

	if (!UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_CUT))
	{
		return FALSE;
	}

	BOOL bRes = EditCopy ();

	if (bRes)
	{
		RemoveSelectedAppointments (TRUE, TRUE);
	}

	UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_NONE, bRes);

	return bRes;
}

BOOL CBCGPPlannerManagerCtrl::IsEditCutEnabled () const
{
	return IsEditCopyEnabled () && !IsReadOnly ();
}

BOOL CBCGPPlannerManagerCtrl::SerializeTo (CFile& file)
{
	XBCGPAppointmentArray ar;

	POSITION pos = m_lsSelectedApps.GetHeadPosition ();
	while (pos != NULL)
	{
		CBCGPAppointment* pApp = m_lsSelectedApps.GetNext (pos);
		ASSERT_VALID (pApp);

		ar.Add (pApp);
	}

	return CBCGPPlannerManagerCtrl::SerializeTo (file, ar);
}

BOOL CBCGPPlannerManagerCtrl::SerializeTo (CFile& file, XBCGPAppointmentArray& ar)
{
	const int nCount = (int) ar.GetSize ();

	if (nCount == 0)
	{
		return FALSE;
	}

	CArchive Archive (&file, CArchive::store);

	BOOL bResult = CBCGPPlannerManagerCtrl::SerializeTo (Archive, ar);

	Archive.Close();

	return bResult;
}

BOOL CBCGPPlannerManagerCtrl::SerializeTo (CArchive& Archive, XBCGPAppointmentArray& ar)
{
	const int nCount = (int) ar.GetSize ();

	if (nCount == 0)
	{
		return FALSE;
	}

	BOOL bStored = FALSE;

	try
	{
		Archive << nCount;

		for (int i = 0; i < nCount; i++)
		{
			CBCGPAppointment* pApp = ar[i];
			ASSERT_VALID (pApp);

			if (pApp != NULL && pApp->CanBeStored ())
			{
				Archive << pApp;
				bStored = TRUE;
			}
		}
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CBCGPPlannerManagerCtrl::SerializeTo. Archive exception\r\n"));
		pEx->Delete ();
		return FALSE;
	}

	return bStored;
}

BOOL CBCGPPlannerManagerCtrl::SerializeFrom (CFile& file, const COleDateTime& dtTo)
{
	XBCGPAppointmentArray ar;

	BOOL bResult = CBCGPPlannerManagerCtrl::SerializeFrom (file, ar, m_Type, dtTo);

	if (!bResult)
	{
		return FALSE;
	}

	CBCGPPlannerView* pView = GetCurrentView ();
	ASSERT_VALID(pView);

	BOOL bAllDay = FALSE;
	BOOL bNeedAdjust = m_Type == BCGP_PLANNER_TYPE_DAY || 
					   m_Type == BCGP_PLANNER_TYPE_WORK_WEEK;

	for (int i = 0; i < ar.GetSize (); i++)
	{
		if (ar[i]->IsRecurrenceClone ())
		{
			ar[i]->RemoveRecurrence ();
		}

		AddAppointment (ar[i], FALSE, FALSE);

		if (bNeedAdjust && !bAllDay)
		{
			bAllDay = ar[i]->IsAllDay () || ar[i]->IsMultiDay ();
		}
	}

	if (!IsDragDrop ())
	{
		ClearAppointmentSelection (FALSE);
		QueryAppointments ();

		if (bNeedAdjust && bAllDay)
		{
			pView->AdjustLayout (FALSE);
		}
		else
		{
			pView->AdjustAppointments ();
		}

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return bResult;
}

BOOL CBCGPPlannerManagerCtrl::SerializeFrom (CFile& file, 
	XBCGPAppointmentArray& ar, BCGP_PLANNER_TYPE type, const COleDateTime& dtTo)
{
	CArchive Archive (&file, CArchive::load);

	BOOL bResult = CBCGPPlannerManagerCtrl::SerializeFrom (Archive, ar, type, dtTo);

	Archive.Close();

	return bResult;
}

BOOL CBCGPPlannerManagerCtrl::SerializeFrom (CArchive& Archive, 
	XBCGPAppointmentArray& ar, BCGP_PLANNER_TYPE type, const COleDateTime& dtTo)
{
	try
	{
		int nCount;
		Archive >> nCount;

		if (nCount == 0)
		{
			return FALSE;
		}

		ar.RemoveAll ();
		int i = 0;

		for (i = 0; i < nCount; i++)
		{
			CBCGPAppointment* pApp = NULL;
			Archive >> pApp;

			ASSERT_VALID (pApp);

			if (pApp != NULL)
			{
				ar.Add (pApp);
			}
		}

		if (dtTo != COleDateTime ())
		{
			// correct interval by date TO
			nCount = (int) ar.GetSize ();

			COleDateTime dtMin;

			for (i = 0; i < nCount; i++)
			{
				COleDateTime dt (ar[i]->GetStart ());

				if (i == 0)
				{
					dtMin = dt;
				}
				else if (dtMin > dt)
				{
					dtMin = dt;
				}
			}

			BOOL bAdd = dtMin < dtTo;
			COleDateTimeSpan spanTo;

			if (type == BCGP_PLANNER_TYPE_DAY || type == BCGP_PLANNER_TYPE_WORK_WEEK)
			{
				dtMin = COleDateTime (dtMin.GetYear (), dtMin.GetMonth (), 
					dtMin.GetDay (), dtMin.GetHour (), 0, 0);
			}
			else if (type == BCGP_PLANNER_TYPE_WEEK || type == BCGP_PLANNER_TYPE_MONTH)
			{
				dtMin = COleDateTime (dtMin.GetYear (), dtMin.GetMonth (), 
					dtMin.GetDay (), 0, 0, 0);
			}

			if (bAdd)
			{
				spanTo = dtTo - dtMin;
			}
			else
			{
				spanTo = dtMin - dtTo;
			}

			MoveAppointments (ar, spanTo, bAdd);
		}
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CBCGPPlannerManagerCtrl::SerializeTo. Archive exception\r\n"));
		pEx->Delete ();

		if (ar.GetSize () > 0)
		{
			for (int i = 0; i < ar.GetSize (); i++)
			{
				if (ar[i] != NULL)
				{
					delete ar[i];
				}
			}

			ar.RemoveAll ();
		}
	}

	return ar.GetSize () > 0;
}

void CBCGPPlannerManagerCtrl::MoveAppointments (XBCGPAppointmentArray& ar, const COleDateTimeSpan& spanTo, BOOL bAdd)
{
	const int nCount = (int) ar.GetSize ();

	if (nCount == 0 || spanTo.GetStatus () != COleDateTimeSpan::valid)
	{
		return;
	}

	for (int i = 0; i < nCount; i++)
	{
		CBCGPAppointment* pApp = ar[i];

		COleDateTime dtS (pApp->GetStart ());
		COleDateTime dtF (pApp->GetFinish ());

		if (bAdd)
		{
			dtS += spanTo;
			dtF += spanTo;
		}
		else
		{
			dtS -= spanTo;
			dtF -= spanTo;
		}

		pApp->SetInterval (dtS, dtF);
	}
}

BOOL CBCGPPlannerManagerCtrl::EditCopy ()
{
	CWnd* pWnd = GetDlgItem (BCGP_PLANNER_ID_INPLACE);

	if (pWnd != NULL)
	{
		pWnd->SendMessage (WM_COPY, 0, 0);
		return FALSE;
	}

	if (!IsEditCopyEnabled ())
	{
		return FALSE;
	}

	try
	{
		if (!AfxGetMainWnd ()->OpenClipboard ())
		{
			return FALSE;
		}

		if (!::EmptyClipboard ())
		{
			::CloseClipboard ();
			return FALSE;
		}

		CString strText;

		HGLOBAL hClipbuffer = NULL;

		{
			CSharedFile globFile;

			SerializeTo (globFile);

			POSITION pos = m_lsSelectedApps.GetHeadPosition ();
			while (pos != NULL)
			{
				CBCGPAppointment* pApp = m_lsSelectedApps.GetNext (pos);
				ASSERT_VALID (pApp);

				if (pApp != NULL && pApp->CanBeStored ())
				{
					CString strClip = pApp->GetClipboardText ();
					if (!strClip.IsEmpty ())
					{
						if (!strText.IsEmpty ())
						{
							strText += _T("\r\n");
						}

						strText += strClip;
					}
				}
			}

			globFile.SeekToBegin ();

			hClipbuffer = ::GlobalAlloc (GMEM_DDESHARE, (SIZE_T)globFile.GetLength ());
			LPBYTE lpData = (LPBYTE)::GlobalLock (hClipbuffer);

			globFile.Read (lpData, (UINT) globFile.GetLength ());

			::GlobalUnlock (hClipbuffer);

			globFile.Close ();
		}

		HANDLE hclipData = ::SetClipboardData (GetClipboardFormat (), 
			hClipbuffer);

		if (hclipData != NULL)
		{
			::SetClipboardData (CF_OWNERDISPLAY, NULL);

			if (!strText.IsEmpty ())
			{
				hClipbuffer = ::GlobalAlloc (GMEM_DDESHARE, (strText.GetLength () + 1) * sizeof (TCHAR));
				LPTSTR lpszBuffer = (LPTSTR) ::GlobalLock (hClipbuffer);

				_tcscpy (lpszBuffer, (LPCTSTR) strText);

				::GlobalUnlock (hClipbuffer);
				::SetClipboardData (_TCF_TEXT, hClipbuffer);
			}
			else
			{
				::SetClipboardData (_TCF_TEXT, NULL);
			}
		}

		::CloseClipboard ();		

		return hclipData != NULL;
	}
	catch (...)
	{
		CBCGPLocalResource locaRes;
		AfxMessageBox (IDP_BCGBARRES_INTERLAL_ERROR);
	}

	return FALSE;
}

BOOL CBCGPPlannerManagerCtrl::IsEditCopyEnabled () const
{
	CWnd* pWnd = GetDlgItem (BCGP_PLANNER_ID_INPLACE);

	if (pWnd != NULL)
	{
		return TRUE;
	}

	return GetClipboardFormat () != 0 && m_lsSelectedApps.GetCount () > 0;
}

BOOL CBCGPPlannerManagerCtrl::EditPaste (const COleDateTime& dtTo)
{
	CWnd* pWnd = GetDlgItem (BCGP_PLANNER_ID_INPLACE);

	if (pWnd != NULL)
	{
		pWnd->SendMessage (WM_PASTE, 0, 0);
		return FALSE;
	}

	if (!IsEditPasteEnabled ())
	{
		return FALSE;
	}

	if(!UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_PASTE))
	{
		return FALSE;
	}

	BOOL bRes = FALSE;

	try
	{
		if (!AfxGetMainWnd ()->OpenClipboard ())
		{
			return FALSE;
		}

		HGLOBAL hClipbuffer = ::GetClipboardData(GetClipboardFormat ());

		if (hClipbuffer != NULL) 
		{ 
			LPBYTE lpData = (LPBYTE)::GlobalLock(hClipbuffer);

			if (lpData != NULL) 
			{ 
				CSharedFile globFile;
				globFile.Attach (lpData, (UINT)::GlobalSize (hClipbuffer));

				COleDateTime dt (dtTo);
				if (dt == COleDateTime ())
				{
					dt = GetCurrentView ()->GetSelectionStart ();
				}

				bRes = SerializeFrom (globFile, dt);

				::GlobalUnlock(hClipbuffer);

				if (bRes)
				{
					UpdateCalendarsState ();
				}
			} 
		} 

		::CloseClipboard ();
	}
	catch (...)
	{
		AfxMessageBox (IDP_BCGBARRES_INTERLAL_ERROR);
	}

	UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_NONE, bRes);

	return bRes;
}

BOOL CBCGPPlannerManagerCtrl::IsEditPasteEnabled () const
{
	if (IsReadOnly ())
	{
		return FALSE;
	}

	CWnd* pWnd = GetDlgItem (BCGP_PLANNER_ID_INPLACE);

	if (pWnd != NULL)
	{
		return ::IsClipboardFormatAvailable (CF_TEXT);
	}

	BOOL bRes = GetClipboardFormat () != 0;

	if (bRes)
	{
		bRes = ::IsClipboardFormatAvailable (GetClipboardFormat ());
	}

	return bRes;
}

BOOL CBCGPPlannerManagerCtrl::IsAppointmentInSelection (const CBCGPAppointment* pApp) const
{
	ASSERT_VALID (pApp);
	return m_lsSelectedApps.Find (const_cast<CBCGPAppointment*>(pApp)) != NULL;
}

CBCGPAppointment*
CBCGPPlannerManagerCtrl::GetAppointmentFromPoint (const CPoint& point)
{
	return GetCurrentView ()->GetAppointmentFromPoint (point);
}

CBCGPAppointment* CBCGPPlannerManagerCtrl::GetRecurrence (DWORD ID) const
{
	ASSERT_VALID (m_pAppsStorage);

	return m_pAppsStorage->GetRecurrence (ID);
}

int CBCGPPlannerManagerCtrl::GetSelectedAppointmentsCount () const
{
	return (int) m_lsSelectedApps.GetCount ();
}

POSITION CBCGPPlannerManagerCtrl::GetFirstSelectedAppointment () const
{
	return m_lsSelectedApps.GetHeadPosition ();
}

POSITION CBCGPPlannerManagerCtrl::GetLastSelectedAppointment () const
{
	return m_lsSelectedApps.GetTailPosition ();
}

const CBCGPAppointment* CBCGPPlannerManagerCtrl::GetNextSelectedAppointment (POSITION& pos) const
{
	return m_lsSelectedApps.GetNext (pos);
}

const CBCGPAppointment* CBCGPPlannerManagerCtrl::GetPrevSelectedAppointment (POSITION& pos) const
{
	return m_lsSelectedApps.GetPrev (pos);
}

int CBCGPPlannerManagerCtrl::GetQueryedAppointmentsCount () const
{
	return (int) m_arQueryApps.GetSize ();
}

int CBCGPPlannerManagerCtrl::GetQueryedAppointments (XBCGPAppointmentArray& ar) const
{
	ar.RemoveAll ();
	ar.Copy (m_arQueryApps);

	return (int) ar.GetSize ();
}

CBCGPPlannerView* CBCGPPlannerManagerCtrl::GetView (BCGP_PLANNER_TYPE type)
{
	ASSERT (BCGP_PLANNER_TYPE_FIRST <= type);
	ASSERT (type <= BCGP_PLANNER_TYPE_LAST);

	switch (type)
	{
	case BCGP_PLANNER_TYPE_DAY:
		return m_pViews [0];

	case BCGP_PLANNER_TYPE_WORK_WEEK:
		return m_pViews [1];

	case BCGP_PLANNER_TYPE_WEEK:
		return m_pViews [2];

	case BCGP_PLANNER_TYPE_MONTH:
		return m_pViews [3];
	}

	return NULL;
}

const CBCGPPlannerView* CBCGPPlannerManagerCtrl::GetView (BCGP_PLANNER_TYPE type) const
{
	ASSERT (BCGP_PLANNER_TYPE_FIRST <= type);
	ASSERT (type <= BCGP_PLANNER_TYPE_LAST);

	switch (type)
	{
	case BCGP_PLANNER_TYPE_DAY:
		return m_pViews [0];

	case BCGP_PLANNER_TYPE_WORK_WEEK:
		return m_pViews [1];

	case BCGP_PLANNER_TYPE_WEEK:
		return m_pViews [2];

	case BCGP_PLANNER_TYPE_MONTH:
		return m_pViews [3];
	}

	return NULL;
}

void CBCGPPlannerManagerCtrl::StartDragDrop ()
{
	if (GetCurrentView () == NULL)
	{
		return;
	}

	if (!m_bDragDrop && CanStartDragDrop ())
	{
		if (!UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_DRAG))
		{
			return;
		}

		COleDataSource srcItem;

		try
		{
			CSharedFile globFile;
			
			m_bDragDrop = SerializeTo (globFile);

			if (m_bDragDrop)
			{
				srcItem.CacheGlobalData (GetClipboardFormat (), 
					globFile.Detach());
			}
		}
		catch (COleException* pEx)
		{
			TRACE(_T("CBCGPPlannerManagerCtrl::StartDragDrop. OLE exception: %x\r\n"),
				pEx->m_sc);
			pEx->Delete ();

			m_bDragDrop = FALSE;
		}
		catch (CNotSupportedException *pEx)
		{
			TRACE(_T("CBCGPPlannerManagerCtrl::StartDragDrop. \"Not Supported\" exception\r\n"));
			pEx->Delete ();

			m_bDragDrop = FALSE;
		}

		if (!m_bDragDrop)
		{
			return;
		}

		StopCapture ();

		GetCurrentView ()->StartCapture ();

		m_dragEffect = DROPEFFECT_NONE;

		m_DropSource.Empty ();

		DWORD dwEffects = DROPEFFECT_COPY | DROPEFFECT_SCROLL;

		if (!IsReadOnly ())
		{
			dwEffects |= DROPEFFECT_MOVE;
		}

		DROPEFFECT dropEffect = srcItem.DoDragDrop (dwEffects, NULL, &m_DropSource);

		BOOL bRes = dropEffect == DROPEFFECT_MOVE || dropEffect == DROPEFFECT_COPY;

		GetCurrentView ()->StopCapture ();

		StopDragDrop ();

#ifdef BCGP_PLANNER_DND_TO_ANOTHER_PLANNER
		BOOL bRemove = dropEffect == DROPEFFECT_MOVE && 
			GetSelectedAppointmentsCount () > 0;

		if (bRemove)
		{
			RemoveSelectedAppointments (FALSE, FALSE);
		}
#endif

		GetCurrentView ()->ClearDragedAppointments ();

#ifdef BCGP_PLANNER_DND_TO_ANOTHER_PLANNER
		if (bRemove)
		{
			ClearAppointmentSelection (FALSE);
			QueryAppointments ();

			AdjustLayout (TRUE);

			UpdateCalendarsState ();
		}
		else if (m_bRedrawAfterDrop && !bRes)
		{
			m_bRedrawAfterDrop = FALSE;
			
			AdjustLayout (TRUE);
		}
#endif

		if (m_bNotifyParent && bRes)
		{
			GetParent ()->SendMessage (BCGP_PLANNER_DRAG_APPOINTMENTS, (WPARAM)dropEffect, 0);
		}

		UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_NONE, bRes);
	}
}

void CBCGPPlannerManagerCtrl::StopDragDrop ()
{
	if (GetCurrentView () == NULL)
	{
		return;
	}

	if (m_bDragDrop)
	{
		m_DropSource.Empty ();

		m_bDragDrop = FALSE;

		if (m_dragEffect != DROPEFFECT_NONE)
		{
			m_dragEffect = DROPEFFECT_NONE;
		}
	}
}

BOOL CBCGPPlannerManagerCtrl::CanStartDragDrop () const
{
	if (!IsDragDrop () &&
		 GetCurrentView () != NULL && 
		 GetCurrentView ()->CanStartDragDrop () &&
		 m_lsSelectedApps.GetCount () > 0)
	{
		CSize czScroll (::GetSystemMetrics (SM_CXDRAG),
						::GetSystemMetrics (SM_CYDRAG));
		CRect rt (m_ptCaptureStart, czScroll);
		rt.OffsetRect (CPoint (-czScroll.cx / 2, -czScroll.cy / 2));

		return !rt.PtInRect (m_ptCaptureCurrent);
	}

	return FALSE;
}

BOOL CBCGPPlannerManagerCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
#ifndef BCGP_PLANNER_DND_TO_ANOTHER_PLANNER
	// can't be dropped from another planner
	if (!IsDragDrop ())
	{
		return FALSE;
	}
#endif

	if (dropEffect != DROPEFFECT_MOVE && dropEffect != DROPEFFECT_COPY)
	{
		return FALSE;
	}

	if (IsReadOnly ())
	{
		return FALSE;
	}

	ASSERT (pDataObject != NULL);
	ASSERT (pDataObject->IsDataAvailable (GetClipboardFormat ()));

	BOOL bRes = FALSE;

	m_dtDrop = GetCurrentView ()->m_dtCaptureCurrent;

	if(!IsDragDrop ())
	{
		if (!UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_DROP))
		{
			m_dtDrop = COleDateTime ();
			AdjustLayout (TRUE);

			return FALSE;
		}
	}
	else
	{
		if(m_bNotifyParent && GetSafeHwnd () != NULL && 
			GetParent ()->GetSafeHwnd () != NULL)
		{
			if (GetParent ()->SendMessage (BCGP_PLANNER_BEGIN_CHANGE_OPERATION, 
				(WPARAM)BCGP_PLANNER_CHANGE_OPERATION_DROP, (LPARAM)FALSE) != 0L)
			{
				m_dtDrop = COleDateTime ();
				m_bRedrawAfterDrop = TRUE;

				return FALSE;
			}
		}
	}

	try
	{
		GetCurrentView ()->ClearDragedAppointments ();

		CFile* pFile = pDataObject->GetFileData (GetClipboardFormat ());
		if (pFile == NULL)
		{
			m_dtDrop = COleDateTime ();

			return FALSE;
		}

		XBCGPAppointmentArray ar;

		bRes = CBCGPPlannerManagerCtrl::SerializeFrom (*pFile, ar, m_Type, 
			m_bDragDrop ? COleDateTime () : GetCurrentView ()->GetDateFromPoint (point));

		delete pFile;

		if (bRes)
		{
			BOOL bAllDay = FALSE;
			BOOL bCanMakeAllDay = m_Type == BCGP_PLANNER_TYPE_DAY || 
								  m_Type == BCGP_PLANNER_TYPE_WORK_WEEK;

			if (bCanMakeAllDay)
			{
				CBCGPPlannerView::BCGP_PLANNER_HITTEST hit = 
					GetCurrentView ()->HitTestArea (point);

				bAllDay = hit == CBCGPPlannerView::BCGP_PLANNER_HITTEST_HEADER_ALLDAY ||
						  hit == CBCGPPlannerView::BCGP_PLANNER_HITTEST_HEADER;
			}

			if (m_bDragDrop)
			{
				COleDateTime dtS (GetCurrentView ()->m_dtCaptureStart);

				BOOL bAdd =  m_dtDrop > dtS;

				COleDateTimeSpan spanTo;

				if (bAdd)
				{
					spanTo = m_dtDrop - dtS;
				}
				else
				{
					spanTo = dtS - m_dtDrop;
				}

				MoveAppointments (ar, spanTo, bAdd);
			}

			BOOL bNotify = m_bNotifyParent;
			BOOL bNotifyParent = FALSE;
			if (m_bDragDrop && dropEffect != DROPEFFECT_COPY)
			{
				// Drop in same view as drag, don't send add & remove
				// notifications - send update notification
				bNotifyParent = m_bNotifyParent;
				m_bNotifyParent = FALSE;
			}

			const INT_PTR c_Count = ar.GetSize ();

			if (c_Count > 0)
			{
				BOOL bRemoveRecurrence = !m_bDragDrop || dropEffect == DROPEFFECT_COPY;

				if (m_bDragDrop)
				{
					for (int i = 0; i < m_arQueryApps.GetSize (); i++)
					{
						CBCGPAppointment* pApp = m_arQueryApps[i];

						if (pApp->IsRecurrenceClone () && pApp->IsSelected ())
						{
							m_lsSelectedApps.RemoveAt(m_lsSelectedApps.Find (pApp));
						}
					}

					if (dropEffect == DROPEFFECT_MOVE)
					{
						RemoveSelectedAppointments (FALSE, FALSE);
					}
				}

				COleDateTime dtSel1 = GetSelectionStart ();
				COleDateTime dtSel2 = GetSelectionEnd ();

				for (int i = 0; i < c_Count; i++)
				{
					CBCGPAppointment* pApp = ar[i];
					ASSERT_VALID(pApp);

					BOOL bRecurrenceClone = pApp->IsRecurrence () && pApp->IsRecurrenceClone ();

					if (bRemoveRecurrence && bRecurrenceClone)
					{
						pApp->RemoveRecurrence ();
					}

					if (bCanMakeAllDay)
					{
						BOOL bAllOrMulti = pApp->IsAllDay () || pApp->IsMultiDay ();

						if (bAllOrMulti)
						{
							if (!bAllDay)
							{
								pApp->SetAllDay (FALSE);
								pApp->SetInterval (m_dtDrop, m_dtDrop);
							}
						}
						else if (bAllDay)
						{
							pApp->SetAllDay (TRUE);
							pApp->SetInterval (m_dtDrop, m_dtDrop);
						}
					}


					LRESULT lRes = 0L;
					if (bNotifyParent)
					{
						lRes = GetParent ()->SendMessage (BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT, 0, (LPARAM)pApp);
					}

					if (lRes == 0L)
					{
						dtSel1 = pApp->GetStart ();
						dtSel2 = pApp->GetFinish ();

						if (AddAppointment (pApp, FALSE, FALSE) && bNotifyParent)
						{
							GetParent ()->SendMessage (BCGP_PLANNER_UPDATE_APPOINTMENT, 0, 
								(LPARAM)(bRecurrenceClone ? NULL : pApp));
						}
					}
					else
					{
						if (pApp->IsRecurrenceClone ())
						{
							delete pApp;
						}
						else
						{
							AddAppointment (pApp, FALSE, FALSE);
						}
					}
				}

				if (m_Type == BCGP_PLANNER_TYPE_DAY ||
					m_Type == BCGP_PLANNER_TYPE_WORK_WEEK)
				{
					if (dtSel1 == dtSel2)
					{
						dtSel2 += GetCurrentView ()->GetMinimumSpan ();
					}

					dtSel2 -= COleDateTimeSpan (0, 0, 0, 1);
				}
				else
				{
					dtSel1 = COleDateTime (dtSel1.GetYear (), dtSel1.GetMonth (), dtSel1.GetDay (), 0, 0, 0);
					dtSel2 = COleDateTime (dtSel2.GetYear (), dtSel2.GetMonth (), dtSel2.GetDay (), 0, 0, 0);
				}

				GetCurrentView ()->SetSelection (dtSel1, dtSel2, FALSE);
			}

			if (!m_bDragDrop ||
				(m_bDragDrop && (dropEffect == DROPEFFECT_MOVE || dropEffect == DROPEFFECT_COPY)))
			{
				ClearAppointmentSelection (FALSE);
				QueryAppointments ();

				AdjustLayout (TRUE);

				UpdateCalendarsState ();
			}

			m_bNotifyParent = bNotify;

			if (m_bNotifyParent)
			{
				GetParent ()->SendMessage (BCGP_PLANNER_DROP_APPOINTMENTS, 0, 0);
			}
		}
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CBCGPPlannerManagerCtrl::OnDrop. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}

	if(!IsDragDrop ())
	{
		UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_NONE, bRes);
	}
	else
	{
		if(m_bNotifyParent && GetSafeHwnd () != NULL && 
			GetParent ()->GetSafeHwnd () != NULL)
		{
			GetParent ()->SendMessage (BCGP_PLANNER_END_CHANGE_OPERATION, 
				(WPARAM)BCGP_PLANNER_CHANGE_OPERATION_DROP, bRes);
		}
	}

	m_dtDrop = COleDateTime ();

	return bRes;
}

DROPEFFECT CBCGPPlannerManagerCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return OnDragOver(pDataObject, dwKeyState, point);
}

void CBCGPPlannerManagerCtrl::OnDragLeave()
{
	CBCGPPlannerView* pView = GetCurrentView ();

	m_dragEffect = DROPEFFECT_NONE;

	if (pView->GetDragedAppointments ().GetSize () > 0)
	{
		pView->ClearDragedAppointments ();
		pView->AdjustAppointments ();

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

DROPEFFECT CBCGPPlannerManagerCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
#ifndef BCGP_PLANNER_DND_TO_ANOTHER_PLANNER
	// can't be dropped from another planner
	if (!IsDragDrop ())
	{
		return DROPEFFECT_NONE;
	}
#endif

	if (IsReadOnly ())
	{
		return DROPEFFECT_NONE;
	}

	CBCGPPlannerView* pView = GetCurrentView ();

	DROPEFFECT dragEffect = pView->OnDragOver (pDataObject, dwKeyState, point);

	CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION action = pView->GetAdjustAction ();

	if (!IsDragDrop () || (IsDragDrop () && 
		action != CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_NONE))
	{
		m_dragEffect = dragEffect;

		if (action == CBCGPPlannerView::BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS)
		{
			pView->AdjustAppointments ();
		}
		else
		{
			pView->AdjustLayout (FALSE);
		}

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return m_dragEffect;
}

BOOL CBCGPPlannerManagerCtrl::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	return GetCurrentView ()->OnScroll (nScrollCode, nPos, bDoScroll);
}

void CBCGPPlannerManagerCtrl::GetDragScrollRect (CRect& rect)
{
	GetCurrentView ()->GetDragScrollRect (rect);
}

DROPEFFECT CBCGPPlannerManagerCtrl::OnDragScroll(DWORD dwKeyState, CPoint point)
{
#ifndef BCGP_PLANNER_DND_TO_ANOTHER_PLANNER
	// can't be dropped from another planner
	if (!IsDragDrop ())
	{
		return DROPEFFECT_NONE;
	}
#endif

	return GetCurrentView ()->OnDragScroll (dwKeyState, point);
}

void CBCGPPlannerManagerCtrl::StartCapture ()
{
	if (!m_bCaptured)
	{
		CPoint pt;
		::GetCursorPos (&pt);
		ScreenToClient (&pt);

		m_ptCaptureStart = pt;
		m_ptCaptureCurrent = m_ptCaptureStart;

		GetCurrentView ()->StartCapture ();

		m_pWndLastCapture = SetCapture ();

		m_bCaptured = TRUE;
	}
}

void CBCGPPlannerManagerCtrl::StopCapture ()
{
	if (m_bCaptured)
	{
		m_bCaptured = FALSE;

		::ReleaseCapture ();

		if (m_pWndLastCapture != NULL)
		{
			m_pWndLastCapture->SetCapture ();
			m_pWndLastCapture = NULL;
		}

		GetCurrentView ()->StopCapture ();

		m_ptCaptureStart   = CPoint (0, 0);
		m_ptCaptureCurrent = m_ptCaptureStart;
	}
}

void CBCGPPlannerManagerCtrl::OnCancelMode() 
{
	StopDragDrop ();

	StopCapture ();

	CWnd::OnCancelMode();
}

LRESULT CBCGPPlannerManagerCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (IsCaptured ())
	{
		if (WM_MOUSEFIRST <= message && message <= WM_MOUSELAST &&
			message != WM_LBUTTONDOWN && message != WM_LBUTTONUP && message != WM_MOUSEMOVE)
		{
			OnCancelMode ();
		}
		else if (message == WM_KEYDOWN && (wParam == VK_CANCEL || wParam == VK_ESCAPE))
		{
			if (!IsDragDrop ())
			{
				GetCurrentView ()->RestoreCapturedAppointment ();
			}

			OnCancelMode ();
		}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CBCGPPlannerManagerCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTipCount > 0 && m_pToolTip->GetSafeHwnd () != NULL)
	{
		m_pToolTip->RelayEvent (pMsg);
	}

	return CWnd::PreTranslateMessage (pMsg);
}

void CBCGPPlannerManagerCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!GetCurrentView ()->OnLButtonDblClk (nFlags, point))
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}

	if (m_bNotifyParent && GetSafeHwnd () != NULL)
	{
		GetParent ()->SendMessage (BCGP_PLANNER_LBUTTONDBLCLK, nFlags, MAKELPARAM(point.x, point.y));
	}
}

void CBCGPPlannerManagerCtrl::OnSelectTimeInterval (COleDateTime dateStart, COleDateTime dateFinish)
{
	CBCGPPlannerManagerView* pView = DYNAMIC_DOWNCAST (CBCGPPlannerManagerView,
		GetParent ());
	if (pView == NULL)
	{
		return;
	}

	ASSERT_VALID (pView);
	pView->OnSelectTimeInterval (dateStart, dateFinish);
}

void CBCGPPlannerManagerCtrl::SerializeRaw (CArchive& ar)
{
	ASSERT_VALID (m_pAppsStorage);

	CObList lstApps;

	if (ar.IsLoading ())
	{
		RemoveAllAppointments (FALSE);
		lstApps.Serialize (ar);

		for (POSITION pos = lstApps.GetHeadPosition () ; pos != NULL;)
		{
			CBCGPAppointment* pApp = (CBCGPAppointment*) lstApps.GetNext (pos);
			ASSERT_VALID (pApp);

			m_pAppsStorage->Add (pApp);
		}
		
		QueryAppointments ();
		AdjustLayout ();


		UpdateCalendarsState ();
	}
	else
	{
		XBCGPAppointmentArray arApps;
		m_pAppsStorage->QueryAll (arApps);

		for (int i = 0; i < arApps.GetSize (); i++)
		{
			lstApps.AddTail (arApps [i]);
		}

		lstApps.Serialize (ar);
	}
}

void CBCGPPlannerManagerCtrl::SetCalendar (CBCGPCalendar* pWndCalendar)
{
	m_pWndCalendar = pWndCalendar;

	if (m_pWndCalendar != NULL)
	{
		ASSERT_VALID (m_pWndCalendar);
		m_pWndCalendar->SetPlanner (this);
	}
}

void CBCGPPlannerManagerCtrl::UpdateCalendarsSelection (CBCGPCalendar* pWndCalendar /*= NULL*/)
{
	if (pWndCalendar == NULL)
	{
		pWndCalendar = m_pWndCalendar;
	}

	if (pWndCalendar == NULL || pWndCalendar->GetSafeHwnd () == NULL)
	{
		return;
	}

	ASSERT_VALID (pWndCalendar);

	CList<DATE, DATE&> lstDates;
	pWndCalendar->GetSelectedDates (lstDates);

	COleDateTime date1;
	COleDateTime date2;

	POSITION pos = lstDates.GetHeadPosition ();
	while (pos != NULL)
	{
		COleDateTime dt (lstDates.GetNext (pos));

		if (date1 == COleDateTime () && 
			date2 == COleDateTime ())
		{
			date1 = dt;
			date2 = dt;
		}
		else
		{
			if (dt < date1)
			{
				date1 = dt;
			}
			else if (date2 < dt)
			{
				date2 = dt;
			}

		}
	}

	COleDateTime dt (GetDateStart ());
	int nDuration = GetViewDuration ();

	if (m_Type == BCGP_PLANNER_TYPE_WEEK && GetFirstDayOfWeek () != 1)
	{
		dt = CBCGPPlannerView::GetFirstWeekDay (dt, GetFirstDayOfWeek () + 1);
	}

	if (date1 != dt || date2 != (dt + COleDateTimeSpan (nDuration - 1, 0, 0 ,0)))
	{
		COleDateTimeSpan span (1, 0, 0, 0);
		CList<DATE, DATE&> lstDates;

		for (int i = 0; i < nDuration; i++)
		{
			lstDates.AddTail (dt.m_dt);
			dt += span;
		}

		pWndCalendar->SetSelectedDates (lstDates, FALSE);
	}
}

void CBCGPPlannerManagerCtrl::UpdateCalendarsState (CBCGPCalendar* pWndCalendar /*= NULL*/)
{
	if (pWndCalendar == NULL)
	{
		pWndCalendar = m_pWndCalendar;
	}

	if (pWndCalendar == NULL || pWndCalendar->GetSafeHwnd () == NULL)
	{
		return;
	}

	ASSERT_VALID (pWndCalendar);

	COleDateTime date1 = pWndCalendar->GetFirstDate ();
	COleDateTime date2 = pWndCalendar->GetLastDate ();

	XBCGPAppointmentArray arApps;
	QueryAppointments (arApps, date1, date2);

	CArray<DATE, DATE&> arDates;
	arDates.SetSize (arApps.GetSize ());

	for (int i = 0; i < arApps.GetSize (); i++)
	{
		const CBCGPAppointment* pApp = arApps [i];

		if (!pApp->IsAllDay ())
		{
			COleDateTime dtStart  (pApp->GetStart  ());
			COleDateTime dtFinish (pApp->GetFinish ());

			arDates [i] = dtStart;

			if (!CBCGPPlannerView::IsOneDay (dtStart, dtFinish))
			{
				dtStart.SetDate (dtStart.GetYear (), dtStart.GetMonth (), dtStart.GetDay ());

				if (dtFinish.GetHour () == 0 && dtFinish.GetMinute () == 0)
				{
					dtFinish -= COleDateTimeSpan (1, 0, 0, 0);
				}

				dtFinish.SetDate (dtFinish.GetYear (), dtFinish.GetMonth (), dtFinish.GetDay ());

				const int nDays = (dtFinish - dtStart).GetDays ();

				for (int nDay = 1; nDay <= nDays; nDay++)
				{
					dtStart += COleDateTimeSpan (1, 0, 0, 0);

					DATE d = dtStart;
					arDates.Add (d);
				}
			}
		}

		if (pApp->IsRecurrenceClone ())
		{
			delete pApp;
		}
	}

	pWndCalendar->MarkDates (arDates);
}

BOOL CBCGPPlannerManagerCtrl::OnDropAppointmentToCalendar (COleDataObject* pDataObject,
												 DROPEFFECT dropEffect, COleDateTime dateTo)
{
	if (IsReadOnly ())
	{
		return FALSE;
	}

	if (dropEffect != DROPEFFECT_MOVE && dropEffect != DROPEFFECT_COPY)
	{
		return FALSE;
	}

	ASSERT (pDataObject != NULL);
	ASSERT (pDataObject->IsDataAvailable (GetClipboardFormat ()));

	BOOL bRes = FALSE;

	m_dtDrop = dateTo;

	if(!IsDragDrop ())
	{
		if (!UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_DROP))
		{
			m_dtDrop = COleDateTime ();

			return FALSE;
		}
	}
	else
	{
		if(m_bNotifyParent && GetSafeHwnd () != NULL && 
			GetParent ()->GetSafeHwnd () != NULL)
		{
			if (GetParent ()->SendMessage (BCGP_PLANNER_BEGIN_CHANGE_OPERATION, 
				(WPARAM)BCGP_PLANNER_CHANGE_OPERATION_DROP, (LPARAM)FALSE) != 0L)
			{
				m_dtDrop = COleDateTime ();

				return FALSE;
			}
		}
	}

	try
	{
		GetCurrentView ()->ClearDragedAppointments ();

		CFile* pFile = pDataObject->GetFileData (GetClipboardFormat ());
		if (pFile == NULL)
		{
			m_dtDrop = COleDateTime ();
			
			return FALSE;
		}

		XBCGPAppointmentArray ar;

		bRes = CBCGPPlannerManagerCtrl::SerializeFrom 
			(*pFile, ar, BCGP_PLANNER_TYPE_WEEK, m_dtDrop);
		// BCGP_PLANNER_TYPE_WEEK or BCGP_PLANNER_TYPE_MONTH move appointments with
		// storing hours and minutes

		delete pFile;

		if (bRes)
		{
			CBCGPPlannerView* pView = GetCurrentView ();
			ASSERT_VALID(pView);

			BOOL bNotify = m_bNotifyParent;
			BOOL bNotifyParent = FALSE;
			if (m_bDragDrop && dropEffect != DROPEFFECT_COPY)
			{
				// Drop in same view as drag, don't send add & remove
				// notifications - send update notification
				bNotifyParent = m_bNotifyParent;
				m_bNotifyParent = FALSE;
			}

			const INT_PTR c_Count = ar.GetSize ();

			if (c_Count > 0)
			{
				BOOL bRemoveRecurrence = !m_bDragDrop || dropEffect == DROPEFFECT_COPY;

				if (m_bDragDrop)
				{
					for (int i = 0; i < m_arQueryApps.GetSize (); i++)
					{
						CBCGPAppointment* pApp = m_arQueryApps[i];

						if (pApp->IsRecurrenceClone () && pApp->IsSelected ())
						{
							m_lsSelectedApps.RemoveAt(m_lsSelectedApps.Find (pApp));
						}
					}

					if (dropEffect == DROPEFFECT_MOVE)
					{
						RemoveSelectedAppointments (FALSE, FALSE);
					}
				}

				for (int i = 0; i < c_Count; i++)
				{
					CBCGPAppointment* pApp = ar[i];
					ASSERT_VALID (pApp);

					BOOL bRecurrenceClone = pApp->IsRecurrence () && pApp->IsRecurrenceClone ();

					if (bRemoveRecurrence && bRecurrenceClone)
					{
						pApp->RemoveRecurrence ();
					}

					LRESULT lRes = 0L;
					if (bNotifyParent)
					{
						lRes = GetParent ()->SendMessage (BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT, 0, (LPARAM)pApp);
					}

					if (lRes == 0L)
					{
						if (AddAppointment (pApp, FALSE, FALSE) && bNotifyParent)
						{
							GetParent ()->SendMessage (BCGP_PLANNER_UPDATE_APPOINTMENT, 0, 
								(LPARAM)(bRecurrenceClone ? NULL : pApp));
						}
					}
					else
					{
						if (pApp->IsRecurrenceClone ())
						{
							delete pApp;
						}
						else
						{
							AddAppointment (pApp, FALSE, FALSE);
						}
					}
				}
			}

			if (!m_bDragDrop ||
				(m_bDragDrop && (dropEffect == DROPEFFECT_MOVE || dropEffect == DROPEFFECT_COPY)))
			{
				ClearAppointmentSelection (FALSE);
				QueryAppointments ();

				SetDate (dateTo, FALSE);

				pView->AdjustLayout (TRUE);

				UpdateCalendarsState ();
			}

			m_bNotifyParent = bNotify;

			if (m_bNotifyParent)
			{
				GetParent ()->SendMessage (BCGP_PLANNER_DROP_APPOINTMENTS, 0, 0);
			}
		}
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CBCGPPlannerManagerCtrl::OnDrop. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}

	if(!IsDragDrop ())
	{
		UpdateChangeOperation(BCGP_PLANNER_CHANGE_OPERATION_NONE, bRes);
	}
	else
	{
		if(m_bNotifyParent && GetSafeHwnd () != NULL && 
			GetParent ()->GetSafeHwnd () != NULL)
		{
			GetParent ()->SendMessage (BCGP_PLANNER_END_CHANGE_OPERATION, 
				(WPARAM)BCGP_PLANNER_CHANGE_OPERATION_DROP, bRes);
		}
	}

	m_dtDrop = COleDateTime ();

	return bRes;
}

BOOL CBCGPPlannerManagerCtrl::IsClipboardFormatAvailable (
	COleDataObject* pDataObject)
{
	ASSERT (pDataObject != NULL);
	return (pDataObject->IsDataAvailable (GetClipboardFormat ()));
}

void CBCGPPlannerManagerCtrl::SetBackgroundColor (COLORREF clr, BOOL bRedraw /*= TRUE*/)
{
	if (clr != m_clrBackground)
	{
		m_clrBackground = clr;

		if (bRedraw)
		{
			RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

void CBCGPPlannerManagerCtrl::Print ()
{
	CPrintInfo printInfo;
	printInfo.m_bPreview = FALSE;

	OnPreparePrinting(&printInfo);

	printInfo.m_pPD->m_pd.nFromPage = (WORD)printInfo.GetMinPage();
	printInfo.m_pPD->m_pd.nToPage = (WORD)printInfo.GetMaxPage();
	printInfo.m_pPD->m_pd.nCopies = 1;

	if (printInfo.m_pPD->DoModal() == IDCANCEL)
	{
		OnEndPrinting(NULL, NULL);
		return;
	}
	
	CDC dc;
	dc.Attach(printInfo.m_pPD->GetPrinterDC());
	dc.m_bPrinting = TRUE;

	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	DOCINFO di;
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = dc.StartDoc(&di);

	printInfo.m_rectDraw.SetRect(0, 0,
							dc.GetDeviceCaps(HORZRES), 
							dc.GetDeviceCaps(VERTRES));

	OnBeginPrinting(&dc, &printInfo);

	dc.StartPage();

	OnPrint(&dc, &printInfo);

	bPrintingOK = (dc.EndPage() > 0);

	OnEndPrinting(&dc, &printInfo);

	if (bPrintingOK)
	{
		dc.EndDoc();
	}
	else
	{
		dc.AbortDoc();
	}

	dc.DeleteDC();
}

void CBCGPPlannerManagerCtrl::SetShowToolTip (BOOL bShowToolTip)
{
	if (m_bShowToolTip != bShowToolTip)
	{
		m_bShowToolTip = bShowToolTip;

		if (GetSafeHwnd () != NULL)
		{
			InitToolTipInfo ();
		}
	}
}

void CBCGPPlannerManagerCtrl::SetToolTipShowAlways (BOOL bToolTipShowAlways)
{
	if (m_bToolTipShowAlways != bToolTipShowAlways)
	{
		m_bToolTipShowAlways = bToolTipShowAlways;

		if (GetSafeHwnd () != NULL)
		{
			InitToolTipInfo ();
		}
	}
}

void CBCGPPlannerManagerCtrl::InitToolTipInfo ()
{
	ClearToolTipInfo ();

	if (!m_bShowToolTip)
	{
		return;
	}

	for(int i = 0; i < m_arQueryApps.GetSize (); i++)
	{
		const CBCGPAppointment* pApp = m_arQueryApps[i];

		if (pApp != NULL)
		{
			AddToolTipInfo (pApp);
		}
	}
}

void CBCGPPlannerManagerCtrl::AddToolTipInfo (const CBCGPAppointment* pApp)
{
	ASSERT_VALID (pApp);

	if (m_pToolTip->GetSafeHwnd () == NULL)
	{
		return;
	}

	if (pApp != NULL)
	{
		BOOL bResizeVert = m_Type == BCGP_PLANNER_TYPE_DAY ||
						   m_Type == BCGP_PLANNER_TYPE_WORK_WEEK;

		if (pApp->GetDSDraw ().IsEmpty ())
		{
			if (pApp->IsVisibleDraw () && 
				(m_bToolTipShowAlways || pApp->IsToolTipNeeded ()))
			{
				m_ToolTipCount++;

				CRect rt (pApp->GetRectEditHitTest ());
				pApp->AdjustToolTipRect (rt, bResizeVert);
				m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, rt, m_ToolTipCount);
			}
		}
		else
		{
			for (int i = 0; i < pApp->GetDSDraw ().GetCount (); i++)
			{
				const CBCGPAppointmentDrawStructEx* pDS = 
					(const CBCGPAppointmentDrawStructEx*)(pApp->GetDSDraw ().GetByIndex (i));

				if (pDS != NULL && 
					pDS->IsVisible () && 
					(m_bToolTipShowAlways || pDS->IsToolTipNeeded ()))
				{
					m_ToolTipCount++;

					CRect rt (pDS->GetRectEditHitTest ());
					pApp->AdjustToolTipRect (rt, bResizeVert);
					m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, rt, m_ToolTipCount);
				}
			}
		}
	}
}

void CBCGPPlannerManagerCtrl::ClearToolTipInfo ()
{
	if (m_ToolTipCount > 0 && m_pToolTip->GetSafeHwnd () != NULL)
	{
		for (int i = 0; i < m_ToolTipCount; i++)
		{
			m_pToolTip->DelTool (this, i + 1);
		}

		m_ToolTipCount = 0;
	}
}

BOOL CBCGPPlannerManagerCtrl::OnNeedToolTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	if (!m_bShowToolTip)
	{
		return FALSE;
	}

	static CString strToolTip;

	if (m_ToolTipCount == 0 || m_pToolTip->GetSafeHwnd () == NULL || 
		pNMH->hwndFrom != m_pToolTip->GetSafeHwnd ())
	{
		return FALSE;
	}

	CPoint point;
	::GetCursorPos (&point);
	ScreenToClient (&point);

	const CBCGPAppointment* pApp = GetAppointmentFromPoint (point);

	if (pApp == NULL)
	{
		return FALSE;
	}

	strToolTip = pApp->GetToolTipText ();

	if (strToolTip.IsEmpty ())
	{
		return FALSE;
	}

	LPNMTTDISPINFO	pTTDispInfo = (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	pTTDispInfo->lpszText = (LPTSTR)((LPCTSTR) strToolTip);
	m_pToolTip->SetFont (GetFont (), FALSE);

	return TRUE;
}
//**************************************************************************
LRESULT CBCGPPlannerManagerCtrl::OnBCGUpdateToolTips (WPARAM wp, LPARAM)
{
	UINT nTypes = (UINT) wp;

	if (m_pToolTip->GetSafeHwnd () == NULL)
	{
		return 0;
	}

	if (nTypes & BCGP_TOOLTIP_TYPE_PLANNER)
	{
		ClearToolTipInfo ();

		CString str;
		m_pToolTip->GetText (str, this);

		CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
			BCGP_TOOLTIP_TYPE_PLANNER);

		if (m_pToolTip->GetSafeHwnd () != NULL)
		{
			InitToolTipInfo ();
		}
	}

	return 0;
}
//**************************************************************************
void CBCGPPlannerManagerCtrl::SetDrawFlags (DWORD dwFlags, BOOL bRedraw/* = TRUE*/)
{
	ASSERT_VALID (this);

	m_bDefaultDrawFlags = FALSE;

	if (m_dwDrawFlags != dwFlags)
	{
		m_dwDrawFlags = dwFlags;

		if (bRedraw && GetSafeHwnd () != NULL)
		{
			RedrawWindow ();
		}
	}
}
//**************************************************************************
DWORD CBCGPPlannerManagerCtrl::GetDrawFlags () const
{
	ASSERT_VALID (this);

	if (m_bDefaultDrawFlags)
	{
		return visualManager->GetPlannerDrawFlags ();
	}

	return m_dwDrawFlags;
}

#endif // BCGP_EXCLUDE_PLANNER
