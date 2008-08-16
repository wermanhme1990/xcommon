// BCGPAppointmentStorage.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPAppointmentStorage.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerManagerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPAppointmentBaseStorage

IMPLEMENT_DYNAMIC(CBCGPAppointmentBaseStorage, CObject)

/////////////////////////////////////////////////////////////////////////////
// CBCGPAppointmentStorage

IMPLEMENT_DYNCREATE(CBCGPAppointmentStorage, CBCGPAppointmentBaseStorage)

CBCGPAppointmentStorage::CBCGPAppointmentStorage ()
	: m_bAutoDelete        (TRUE)
	, m_dwRecurrenceLastID (0)
{
}

CBCGPAppointmentStorage::~CBCGPAppointmentStorage ()
{
	RemoveAll ();
}

BOOL CBCGPAppointmentStorage::Add (CBCGPAppointment* pApp)
{
	ASSERT_VALID (pApp);

	if (pApp == NULL)
	{
		return FALSE;
	}

	if (pApp->IsRecurrence ())
	{
		if (AddRecurrence (pApp))
		{
			return TRUE;
		}
	}

	COleDateTime dtS (pApp->GetStart ());
	COleDateTime dtF (pApp->GetFinish ());

	int nYear = dtS.GetYear ();
	int nDay  = dtS.GetDayOfYear ();

	XDayCollection* yearVal = NULL;
	m_Collection.Lookup (nYear, yearVal);

	if (yearVal == NULL)
	{
		yearVal = new XDayCollection;
		m_Collection.SetAt (nYear, yearVal);
	}

	XBCGPAppointmentList* dayVal = NULL;
	yearVal->Lookup (nDay, dayVal);

	if (dayVal == NULL)
	{
		dayVal = new XBCGPAppointmentList;
		yearVal->SetAt (nDay, dayVal);
	}

	BOOL bAdd = FALSE;

	POSITION pos = dayVal->GetTailPosition ();
	
	while (pos != NULL)
	{
		POSITION posPrev = pos;

		CBCGPAppointment* pAppPrev = dayVal->GetPrev (pos);

		COleDateTime dtPrevS (pAppPrev->GetStart ());
		COleDateTime dtPrevF (pAppPrev->GetFinish ());

		if (dtS.GetHour () == dtPrevS.GetHour () && dtS.GetMinute () == dtPrevS.GetMinute () &&
			dtF.GetHour () == dtPrevF.GetHour () && dtF.GetMinute () == dtPrevF.GetMinute ())
		{
			dayVal->InsertAfter (posPrev, pApp);
			bAdd = TRUE;
			break;
		}
	}

	if (!bAdd)
	{
		pos = dayVal->GetHeadPosition ();

		while (pos != NULL)
		{
			POSITION posPrev = pos;

			CBCGPAppointment* pAppNext = dayVal->GetNext (pos);

			COleDateTime dtNextS (pAppNext->GetStart ());

			if (dtS.GetHour () < dtNextS.GetHour () ||
				   (dtS.GetHour () == dtNextS.GetHour () && 
					dtS.GetMinute () < dtNextS.GetMinute ())
			   )
			{
				dayVal->InsertBefore (posPrev, pApp);
				bAdd = TRUE;
				break;
			}
		}
	}

	if (!bAdd)
	{
		dayVal->AddTail (pApp);
	}

	if (!CBCGPPlannerView::IsOneDay (pApp->GetStart (), pApp->GetFinish ()))
	{
		m_CollectionMulti.Add (pApp);
		SortAppointments (m_CollectionMulti, (int) m_CollectionMulti.GetSize ());
	}

	return TRUE;
}

BOOL CBCGPAppointmentStorage::Update (CBCGPAppointment* pApp, 
									  const COleDateTime& dtOld, BOOL bForceAdd/* = FALSE*/)
{
	ASSERT_VALID (pApp);

	if (pApp == NULL)
	{
		return FALSE;
	}

	if (pApp->IsRecurrence ())
	{
		if (UpdateRecurrence (pApp, dtOld))
		{
			return TRUE;
		}
	}

	BOOL bDelete = IsAutoDelete ();

	SetAutoDelete (FALSE);

	COleDateTime dtS (pApp->GetStart ());
	COleDateTime dtF (pApp->GetFinish ());

	if (dtOld != COleDateTime ())
	{
		pApp->SetInterval (dtOld, dtOld);
	}

	BOOL bFound = Remove (pApp);
	if (!bForceAdd && !bFound)
	{
		SetAutoDelete (bDelete);
		return FALSE;
	}

	pApp->SetInterval (dtS, dtF);

	Add (pApp);
	SetAutoDelete (bDelete);

	return TRUE;
}

BOOL CBCGPAppointmentStorage::Remove (CBCGPAppointment* pApp)
{
	ASSERT_VALID (pApp);

	if (pApp == NULL)
	{
		return FALSE;
	}

	if (pApp->IsRecurrence ())
	{
		if (RemoveRecurrence (pApp))
		{
			return TRUE;
		}
	}

	COleDateTime dt (pApp->GetStart ());

	int nYear = dt.GetYear ();
	int nDay = dt.GetDayOfYear ();

	XDayCollection* yearVal = NULL;
	m_Collection.Lookup (nYear, yearVal);

	if (yearVal == NULL)
	{
		return FALSE;
	}

	XBCGPAppointmentList* dayVal = NULL;
	yearVal->Lookup (nDay, dayVal);

	if (dayVal == NULL)
	{
		return FALSE;
	}

	POSITION pos = dayVal->Find (pApp);

	if (pos == NULL)
	{
		return FALSE;
	}

	dayVal->RemoveAt (pos);

	if (dayVal->GetCount () == 0)
	{
		delete dayVal;
		yearVal->RemoveKey (nDay);
	}

	if (yearVal->GetCount () == 0)
	{
		delete yearVal;
		m_Collection.RemoveKey (nYear);
	}

	for (int i = 0; i < m_CollectionMulti.GetSize (); i++)
	{
		if (m_CollectionMulti [i] == pApp)
		{
			m_CollectionMulti.RemoveAt (i);
			break;
		}
	}

	if (IsAutoDelete ())
	{
		delete pApp;
	}

	return TRUE;
}

BOOL CBCGPAppointmentStorage::RemoveAllRecurrence ()
{
	POSITION pos = m_Recurrences.GetStartPosition ();

	while (pos != NULL)
	{
		DWORD nID = 0;
		CBCGPAppointment* pRecurrence = NULL;

		m_Recurrences.GetNextAssoc (pos, nID, pRecurrence);
		ASSERT_VALID (pRecurrence);

		if (pRecurrence != NULL)
		{
			if (IsAutoDelete ())
			{
				delete pRecurrence;
			}
		}
	}

	m_Recurrences.RemoveAll ();

	return TRUE;
}

BOOL CBCGPAppointmentStorage::RemoveAll ()
{
	RemoveAllRecurrence ();

	POSITION yearPos = m_Collection.GetStartPosition ();
	int yearKey = 0;
	XDayCollection* yearVal = NULL;

	while (yearPos != NULL)
	{
		m_Collection.GetNextAssoc (yearPos, yearKey, yearVal);

		if (yearVal != NULL)
		{
			POSITION dayPos = yearVal->GetStartPosition ();
			int dayKey = 0;
			XBCGPAppointmentList* dayVal = NULL;

			while (dayPos != NULL)
			{
				yearVal->GetNextAssoc (dayPos, dayKey, dayVal);

				if (dayVal != NULL)
				{
					if (IsAutoDelete ())
					{
						POSITION pos = dayVal->GetHeadPosition ();

						while (pos != NULL)
						{
							CBCGPAppointment* pApp = dayVal->GetNext (pos);

							if (pApp != NULL)
							{
								delete pApp;
							}
						}
					}

					dayVal->RemoveAll ();
					delete dayVal;
				}
			}


			yearVal->RemoveAll ();
			delete yearVal;
		}
	}

	m_Collection.RemoveAll ();
	m_CollectionMulti.RemoveAll ();

	return TRUE;
}

void CBCGPAppointmentStorage::Query (XBCGPAppointmentArray& ar, const COleDateTime& date1, const COleDateTime& date2)
{
	ar.RemoveAll ();

	{
		POSITION pos = m_Recurrences.GetStartPosition ();

		while (pos != NULL)
		{
			DWORD nID = 0;
			CBCGPAppointment* pRecurrence = NULL;

			m_Recurrences.GetNextAssoc (pos, nID, pRecurrence);
			ASSERT_VALID (pRecurrence);

			if (pRecurrence != NULL)
			{
				XBCGPAppointmentArray arRec;

				pRecurrence->GetRecurrence ()->Query (arRec, date1, date2);

				ar.Append (arRec);
			}
		}
	}

	int nCount = (int)(date2 - date1).GetTotalDays () + 1;

	COleDateTime dt (date1);
	COleDateTimeSpan span (1, 0, 0, 0);

	int i = 0;

	for (i = 0; i < nCount; i++)
	{
		int nYear = dt.GetYear ();
		int nDay = dt.GetDayOfYear ();

		XDayCollection* yearVal = NULL;
		m_Collection.Lookup (nYear, yearVal);

		if (yearVal != NULL)
		{
			XBCGPAppointmentList* dayVal = NULL;
			yearVal->Lookup (nDay, dayVal);

			if (dayVal != NULL)
			{
				POSITION pos = dayVal->GetHeadPosition ();

				while (pos != NULL)
				{
					CBCGPAppointment* pApp = dayVal->GetNext (pos);

					if (pApp != NULL)
					{
						ar.Add (pApp);
					}
				}
			}
		}

		dt += span;
	}

	for (i = 0; i < m_CollectionMulti.GetSize (); i++)
	{
		CBCGPAppointment* pApp = m_CollectionMulti [i];
		if (pApp->GetStart () < date1)
		{
			if ((date1 <= pApp->GetFinish () && pApp->GetFinish () <= date2) ||
				date2 < pApp->GetFinish ())
			{
				if (pApp->GetFinish () != COleDateTime (date1.GetYear (), 
					date1.GetMonth (), date1.GetDay (), 0, 0, 0) || pApp->IsAllDay ())
				{
					ar.Add (pApp);
				}
			}
		}
		else
		{
			break;
		}
	}

	if (ar.GetSize () > 0)
	{
		SortAppointments (ar, (int) ar.GetSize ());
	}
}

void CBCGPAppointmentStorage::QueryAll (XBCGPAppointmentArray& ar)
{
	for (POSITION posYear = m_Collection.GetStartPosition (); posYear != NULL;)
	{
		int nYear = -1;
		XDayCollection* yearVal = NULL;

		m_Collection.GetNextAssoc (posYear, nYear, yearVal);

		if (yearVal != NULL)
		{
			for (POSITION posDay = yearVal->GetStartPosition (); posDay != NULL;)
			{
				int nDay = -1;
				XBCGPAppointmentList* dayVal = NULL;

				yearVal->GetNextAssoc (posDay, nDay, dayVal);

				if (dayVal != NULL)
				{
					for (POSITION pos = dayVal->GetHeadPosition (); pos != NULL;)
					{
						CBCGPAppointment* pApp = dayVal->GetNext (pos);
						if (pApp != NULL)
						{
							ASSERT_VALID (pApp);
							ar.Add (pApp);
						}
					}
				}
			}
		}
	}

	for (POSITION pos = m_Recurrences.GetStartPosition (); pos != NULL;)
	{
		DWORD key = 0;
		CBCGPAppointment* pApp = NULL;
		m_Recurrences.GetNextAssoc (pos, key, pApp);

		if (pApp != NULL)
		{
			ASSERT_VALID (pApp);
			ar.Add (pApp);
		}
	}
}

BOOL CBCGPAppointmentStorage::IsEmpty () const
{
	return m_Collection.IsEmpty ();
}

CBCGPAppointment* CBCGPAppointmentStorage::GetRecurrence (DWORD ID) const
{
	CBCGPAppointment* pRecurrence = NULL;

	if (m_Recurrences.Lookup (ID, pRecurrence))
	{
		return pRecurrence;
	}

	return NULL;
}

BOOL CBCGPAppointmentStorage::AddRecurrence (CBCGPAppointment* pApp)
{
	ASSERT_VALID (pApp);
	ASSERT (pApp->IsRecurrence ());

	if (pApp->IsRecurrenceClone ())
	{
		CBCGPAppointment* pRecurrence = GetRecurrence (pApp->GetRecurrenceID ());

		if (pRecurrence != NULL)
		{
			CBCGPAppointmentPropertyList props;
			pApp->GetProperties (props);

			pRecurrence->GetRecurrence ()->
				DoException (pApp->GetRecurrenceDate (), props, FALSE);

			delete pApp;

			return TRUE;
		}
	}
	else
	{
		DWORD ID = pApp->GetRecurrenceID ();

		if (ID == 0)
		{
			m_dwRecurrenceLastID++;

			pApp->SetRecurrenceID (m_dwRecurrenceLastID);
		}
		else
		{
			if (m_dwRecurrenceLastID < ID)
			{
				m_dwRecurrenceLastID = ID;
			}
		}

		m_Recurrences[m_dwRecurrenceLastID] = pApp;

		return TRUE;
	}

	return FALSE;
}

BOOL CBCGPAppointmentStorage::RemoveRecurrence (CBCGPAppointment* pApp)
{
	ASSERT_VALID (pApp);
	ASSERT (pApp->IsRecurrence ());

	DWORD ID = pApp->GetRecurrenceID ();
	CBCGPAppointment* pRecurrence = GetRecurrence (ID);

	if (pApp->IsRecurrenceClone ())
	{
		if (pRecurrence != NULL)
		{
			CBCGPAppointmentPropertyList props;
			pApp->GetProperties (props);

			pRecurrence->GetRecurrence ()->
				DoException (pApp->GetRecurrenceDate (), props, TRUE);
		}

		delete pApp;
	}
	else
	{
		if (pRecurrence != NULL)
		{
			if (IsAutoDelete ())
			{
				delete pRecurrence;
			}
			m_Recurrences.RemoveKey (ID);
		}
	}

	return TRUE;
}

BOOL CBCGPAppointmentStorage::UpdateRecurrence (CBCGPAppointment* pApp, const COleDateTime& /*dtOld*/)
{
	ASSERT_VALID (pApp);
	ASSERT (pApp->IsRecurrence ());

	if (pApp->IsRecurrenceClone ())
	{
		CBCGPAppointment* pRecurrence = GetRecurrence (pApp->GetRecurrenceID ());
		ASSERT_VALID (pRecurrence);

		CBCGPAppointmentPropertyList props;
		pApp->GetProperties (props);

		pRecurrence->GetRecurrence ()->
			DoException (pApp->GetRecurrenceDate (), props, FALSE);

		//delete pApp;
	}

	return TRUE;
}

#endif // BCGP_EXCLUDE_PLANNER
