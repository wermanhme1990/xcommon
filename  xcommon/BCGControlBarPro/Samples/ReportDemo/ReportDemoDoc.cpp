// ReportDemoDoc.cpp : implementation of the CReportDemoDoc class
//

#include "stdafx.h"
#include "ReportDemo.h"

#include "ReportDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDemoDoc

IMPLEMENT_DYNCREATE(CReportDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CReportDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CReportDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDemoDoc construction/destruction

CReportDemoDoc::CReportDemoDoc()
{
	// TODO: add one-time construction code here

}

CReportDemoDoc::~CReportDemoDoc()
{
}

BOOL CReportDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CReportDemoDoc serialization

void CReportDemoDoc::Serialize(CArchive& ar)
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
// CReportDemoDoc diagnostics

#ifdef _DEBUG
void CReportDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CReportDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportDemoDoc commands
