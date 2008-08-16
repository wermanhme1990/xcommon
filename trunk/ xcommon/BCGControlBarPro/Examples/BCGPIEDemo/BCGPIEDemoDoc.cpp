// BCGPIEDemoDoc.cpp : implementation of the CBCGPIEDemoDoc class
//

#include "stdafx.h"
#include "BCGPIEDemo.h"

#include "BCGPIEDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoDoc

IMPLEMENT_DYNCREATE(CBCGPIEDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CBCGPIEDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CBCGPIEDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoDoc construction/destruction

CBCGPIEDemoDoc::CBCGPIEDemoDoc()
{
	m_iHistoryOffset = 0;
	m_arHistory.SetSize (0, 1);
}

CBCGPIEDemoDoc::~CBCGPIEDemoDoc()
{
	for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		ASSERT (m_arHistory [i] != NULL);
		delete m_arHistory [i];
	}
}

BOOL CBCGPIEDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoDoc serialization

void CBCGPIEDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoDoc diagnostics

#ifdef _DEBUG
void CBCGPIEDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBCGPIEDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoDoc commands

CHistoryObj* CBCGPIEDemoDoc::AddURLToHistory (const CString& strTitle, const CString& strURL)
{
	ASSERT (m_arHistory.GetSize () <= HISTORY_LEN);

	for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		CHistoryObj* pObj = m_arHistory [i];
		ASSERT (pObj != NULL);

		if (pObj->GetTitle () == strTitle &&
			pObj->GetURL () == strURL)
		{
			return pObj;
		}
	}

	if (m_arHistory.GetSize () == HISTORY_LEN)
	{
		delete m_arHistory [0];
		m_arHistory.RemoveAt (0);
	}

	CHistoryObj* pObj = new CHistoryObj (strTitle, strURL, 
		FIRST_HISTORY_COMMAND + (UINT)m_arHistory.GetSize ());
	m_arHistory.InsertAt (0, pObj);

	m_iHistoryOffset = 0;
	return pObj;
}
//****************************************************************************************
void CBCGPIEDemoDoc::GetBackList (_T_HistotyList& lst) const
{
	lst.RemoveAll ();
	for (int i = m_iHistoryOffset + 1; i < m_arHistory.GetSize () ; i ++)
	{
		lst.AddTail (m_arHistory [i]);
	}
}
//****************************************************************************************
void CBCGPIEDemoDoc::GetFrwdList (_T_HistotyList& lst) const
{
	lst.RemoveAll ();
	for (int i = m_iHistoryOffset - 1; i >= 0; i --)
	{
		ASSERT (i < m_arHistory.GetSize ());
		lst.AddTail (m_arHistory [i]);
	}
}
//****************************************************************************************
CHistoryObj* CBCGPIEDemoDoc::Go (UINT uiCmd)
{
	for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		CHistoryObj* pObj = m_arHistory [i];
		ASSERT (pObj != NULL);

		if (pObj->GetCommand () == uiCmd)
		{
			m_arHistory.RemoveAt (i);
			m_arHistory.Add (pObj);

			m_iHistoryOffset = 0;
			return pObj;
		}
	}

	return NULL;
}
