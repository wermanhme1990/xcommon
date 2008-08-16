// BCGPAppointmentDemo.h: interface for the CBCGPAppointmentDemo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPAPPOINTMENTDEMO_H__7B5E460A_B963_4BB9_9705_3AF5DA729112__INCLUDED_)
#define AFX_BCGPAPPOINTMENTDEMO_H__7B5E460A_B963_4BB9_9705_3AF5DA729112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPAppointment.h"

#define BCGP_APPOINTMENT_PROPERTY_LOCATION	BCGP_APPOINTMENT_PROPERTY_FIRST + 8
#define BCGP_APPOINTMENT_PROPERTY_MEMO		BCGP_APPOINTMENT_PROPERTY_FIRST + 9

class CBCGPAppointmentDemo : public CBCGPAppointment  
{
	DECLARE_SERIAL(CBCGPAppointmentDemo)

public:
	CBCGPAppointmentDemo();
	CBCGPAppointmentDemo
	(
		const COleDateTime& dtStart,
		const COleDateTime& dtFinish,
		const CString& strText,
		COLORREF clrBackground = CLR_DEFAULT,
		COLORREF clrForeground = CLR_DEFAULT,
		COLORREF clrDuration   = CLR_DEFAULT
	);

	virtual ~CBCGPAppointmentDemo();

	void SetLocation (const CString& strText);
	const CString& GetLocation () const
	{
		return m_strLocation;
	}

	void SetMemo (const CString& strText);
	const CString& GetMemo () const
	{
		return m_strMemo;
	}

	virtual void GetProperties (CBCGPAppointmentPropertyList& props) const;
	virtual void SetProperties (const CBCGPAppointmentPropertyList& props);

protected:
	virtual void Serialize (CArchive& ar);

protected:
	CString m_strLocation;
	CString m_strMemo;
};

#endif // !defined(AFX_BCGPAPPOINTMENTDEMO_H__7B5E460A_B963_4BB9_9705_3AF5DA729112__INCLUDED_)
