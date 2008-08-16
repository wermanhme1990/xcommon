// TasksPaneDoc.cpp : implementation of the CTasksPaneDoc class
//

#include "stdafx.h"
#include "TasksPane.h"

#include "TasksPaneDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTasksPaneDoc

IMPLEMENT_DYNCREATE(CTasksPaneDoc, CDocument)

BEGIN_MESSAGE_MAP(CTasksPaneDoc, CDocument)
	//{{AFX_MSG_MAP(CTasksPaneDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTasksPaneDoc construction/destruction

CTasksPaneDoc::CTasksPaneDoc()
{
	// TODO: add one-time construction code here

}

CTasksPaneDoc::~CTasksPaneDoc()
{
}

BOOL CTasksPaneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CTasksPaneDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

	((CMainFrame*) AfxGetMainWnd ())->UpdateMRUFilesList ();
}



/////////////////////////////////////////////////////////////////////////////
// CTasksPaneDoc serialization

void CTasksPaneDoc::Serialize(CArchive& ar)
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
// CTasksPaneDoc diagnostics

#ifdef _DEBUG
void CTasksPaneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTasksPaneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTasksPaneDoc commands
