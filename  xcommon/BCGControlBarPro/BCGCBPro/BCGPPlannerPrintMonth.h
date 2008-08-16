#if !defined(AFX_BCGPPLANNERPRINTMONTH_H__5B346DCD_33F3_4E8B_AA24_5D2015EF78D1__INCLUDED_)
#define AFX_BCGPPLANNERPRINTMONTH_H__5B346DCD_33F3_4E8B_AA24_5D2015EF78D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPPlannerPrintMonth.h : header file
//

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerPrint.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerPrintMonth

class BCGCBPRODLLEXPORT CBCGPPlannerPrintMonth : public CBCGPPlannerPrint
{
	DECLARE_DYNCREATE(CBCGPPlannerPrintMonth)

public:
	virtual ~CBCGPPlannerPrintMonth();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void OnPaint (CDC* pDC, CPrintInfo* pInfo);

	virtual void SetDrawTimeFinish (BOOL bDraw);
	virtual BOOL IsDrawTimeFinish () const;

	virtual void SetCompressWeekend (BOOL bCompress);
	virtual BOOL IsCompressWeekend () const;

protected:
	CBCGPPlannerPrintMonth ();

	virtual void CalculateDates (const COleDateTime& date);

	virtual CString GetPageHeaderText () const;
	virtual void GetCaptionFormatStrings (CStringArray& sa);
	virtual void AdjustLayout (CDC* pDC, const CRect& rectClient);
	virtual void AdjustRects ();
	virtual void AdjustAppointments ();

	virtual void OnDrawClient (CDC* pDC);
	virtual void OnDrawHeader (CDC* pDC, const CRect& rectHeader);

	COleDateTime GetFirstWeekDay2 (const COleDateTime& date, int nWeekStart) const;

	int     m_nHeaderHeight;

	BOOL	m_bDrawTimeFinish;
	BOOL	m_bCompressWeekend;
};

#endif // BCGP_EXCLUDE_PLANNER

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPPLANNERPRINTMONTH_H__5B346DCD_33F3_4E8B_AA24_5D2015EF78D1__INCLUDED_)
