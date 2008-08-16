#if !defined(AFX_BCGPAPPOINTMENTSTORAGE_H__70804508_FCC2_4727_A8A6_C1B9B0F6DF54__INCLUDED_)
#define AFX_BCGPAPPOINTMENTSTORAGE_H__70804508_FCC2_4727_A8A6_C1B9B0F6DF54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPAppointmentStorage.h : header file
//

#ifndef __AFXTEMPL_H__
	#pragma warning (disable : 4786)
	#include "afxtempl.h"
#endif

#include "BCGCBPro.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPAppointmentBaseStorage

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPAppointment.h"
#include "BCGPRecurrence.h"

class BCGCBPRODLLEXPORT CBCGPAppointmentBaseStorage : public CObject
{
	DECLARE_DYNAMIC(CBCGPAppointmentBaseStorage)

public:
	virtual ~CBCGPAppointmentBaseStorage ()
	{
	}

	virtual BOOL Add (CBCGPAppointment* pApp) = 0;
	virtual BOOL Update (CBCGPAppointment* pApp, const COleDateTime& dtOld, BOOL bForceAdd = FALSE) = 0;
	virtual BOOL Remove (CBCGPAppointment* pApp) = 0;
	virtual BOOL RemoveAll () = 0;
	virtual void Query (XBCGPAppointmentArray& ar, const COleDateTime& date1, const COleDateTime& date2) = 0;
	virtual void QueryAll (XBCGPAppointmentArray& ar) = 0;
	virtual BOOL IsEmpty () const = 0;

	virtual CBCGPAppointment* GetRecurrence (DWORD ID) const = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPAppointmentStorage


class CBCGPAppointmentStorage: public CBCGPAppointmentBaseStorage
{
	DECLARE_DYNCREATE(CBCGPAppointmentStorage)

public:

	virtual ~CBCGPAppointmentStorage ();

	BOOL IsAutoDelete () const
	{
		return m_bAutoDelete;
	}

	void SetAutoDelete (BOOL bDelete)
	{
		m_bAutoDelete = bDelete;
	}

// Overrides
public:
	virtual BOOL Add (CBCGPAppointment* pApp);
	virtual BOOL Update (CBCGPAppointment* pApp, const COleDateTime& dtOld, BOOL bForceAdd = FALSE);
	virtual BOOL Remove (CBCGPAppointment* pApp);
	virtual BOOL RemoveAll ();

	virtual void Query (XBCGPAppointmentArray& ar, const COleDateTime& date1, const COleDateTime& date2);
	virtual void QueryAll (XBCGPAppointmentArray& ar);

	virtual BOOL IsEmpty () const;

	virtual CBCGPAppointment* GetRecurrence (DWORD ID) const;

protected:
	// Construction
	CBCGPAppointmentStorage ();

	virtual BOOL AddRecurrence (CBCGPAppointment* pApp);
	virtual BOOL UpdateRecurrence (CBCGPAppointment* pApp, const COleDateTime& dtOld);
	virtual BOOL RemoveRecurrence (CBCGPAppointment* pApp);
	virtual BOOL RemoveAllRecurrence ();

protected:
	typedef CMap<int, int, XBCGPAppointmentList*, XBCGPAppointmentList*> XDayCollection;
	typedef CMap<int, int, XDayCollection*, XDayCollection*> XYearCollection;

	typedef CMap<DWORD, DWORD, CBCGPAppointment*, CBCGPAppointment*> XRecurrenceCollection;

	XYearCollection& GetCollection ()
	{
		return m_Collection;
	}

private:
	BOOL			m_bAutoDelete;
	XYearCollection	m_Collection;
	XBCGPAppointmentArray
					m_CollectionMulti;

	DWORD			m_dwRecurrenceLastID;
	XRecurrenceCollection
					m_Recurrences;
};

#endif // BCGP_EXCLUDE_PLANNER

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPAPPOINTMENTSTORAGE_H__70804508_FCC2_4727_A8A6_C1B9B0F6DF54__INCLUDED_)
