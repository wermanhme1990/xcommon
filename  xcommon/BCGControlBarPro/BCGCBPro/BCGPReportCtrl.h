// BCGPReportCtrl.h: interface for the CBCGPReportCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPREPORTCTRL_H__4B66884F_8973_472A_BCD1_4A3C27B62366__INCLUDED_)
#define AFX_BCGPREPORTCTRL_H__4B66884F_8973_472A_BCD1_4A3C27B62366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_GRID_CTRL

#include "BCGPGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPReportRow object

class BCGCBPRODLLEXPORT CBCGPReportRow : public CBCGPGridRow
{
	DECLARE_DYNAMIC(CBCGPReportRow)

	friend class CBCGPReportCtrl;

// Construction
protected:
	CBCGPReportRow ();
public:
	// Group constructor
	CBCGPReportRow (const CString& strGroupName, DWORD dwData = 0);

	virtual ~CBCGPReportRow();
	
// Operations:
public:

// Overrides
public:
	virtual void OnDrawName (CDC* pDC, CRect rect);
	virtual void OnDrawExpandBox (CDC* pDC, CRect rectExpand);

	virtual void OnPrintName (CDC* pDC, CRect rect);
	
	virtual int GetHierarchyLevel () const;

protected:
	virtual BOOL HasValueField () const
	{
		return (m_arrRowItems.GetSize () > 0);
	}
	
// Attributes
protected:
	CString			m_strName;
	BOOL			m_bNameIsTrancated;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPReportCtrl window

class BCGCBPRODLLEXPORT CBCGPReportCtrl : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CBCGPReportCtrl)

public:
	CBCGPReportCtrl();
	virtual ~CBCGPReportCtrl();

	virtual CBCGPGridRow* CreateRow ()
	{
		return new CBCGPReportRow;
	}

	virtual CBCGPGridRow* CreateRow (int nColumns)
	{
		return CBCGPGridCtrl::CreateRow (nColumns);
	}

	virtual CBCGPGridRow* CreateRow (CString strName)
	{
		return new CBCGPReportRow (strName);
	}

protected:
	virtual void SetRowHeight ();
	virtual int GetExtraHierarchyOffset () const { return 0;}
	virtual int GetHierarchyOffset () const;

	virtual BOOL AllowInPlaceEdit () const
	{
		return FALSE; // disable inplace editing
	}

	virtual BOOL IsSelectionBorderEnabled () const
	{
		return FALSE;
	}
	
	virtual BOOL OnTrackHeader ()
	{
		return FALSE; // do not resize columns inside client area by mouse
	}

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPReportCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // BCGP_EXCLUDE_GRID_CTRL

#endif // !defined(AFX_BCGPREPORTCTRL_H__4B66884F_8973_472A_BCD1_4A3C27B62366__INCLUDED_)
