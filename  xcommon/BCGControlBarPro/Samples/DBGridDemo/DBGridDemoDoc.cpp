// DBGridDemoDoc.cpp : implementation of the CDBGridDemoDoc class
//

#include "stdafx.h"
#include "DBGridDemo.h"

#include "DBGridDemoDoc.h"
#include "DBGridDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoDoc

IMPLEMENT_DYNCREATE(CDBGridDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBGridDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDBGridDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoDoc construction/destruction

CDBGridDemoDoc::CDBGridDemoDoc()
{
}

CDBGridDemoDoc::~CDBGridDemoDoc()
{
}

BOOL CDBGridDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoDoc serialization

void CDBGridDemoDoc::Serialize(CArchive& ar)
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
// CDBGridDemoDoc diagnostics

#ifdef _DEBUG
void CDBGridDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBGridDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoDoc commands

BOOL CDBGridDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_strDBPath = lpszPathName;
	return TRUE;
}

void CDBGridDemoDoc::DeleteContents() 
{
	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		CDBGridDemoView* pCurrentView = (CDBGridDemoView*)GetNextView(pos);
		pCurrentView->DeleteAll ();
	}
	
	CDocument::DeleteContents();
}

