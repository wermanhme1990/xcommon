// RTLSampleDoc.cpp : implementation of the CRTLSampleDoc class
//

#include "stdafx.h"
#include "RTLSample.h"

#include "RTLSampleDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleDoc

IMPLEMENT_DYNCREATE(CRTLSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CRTLSampleDoc, CDocument)
	//{{AFX_MSG_MAP(CRTLSampleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleDoc construction/destruction

CRTLSampleDoc::CRTLSampleDoc()
{
	// TODO: add one-time construction code here

}

CRTLSampleDoc::~CRTLSampleDoc()
{
}

BOOL CRTLSampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleDoc serialization

void CRTLSampleDoc::Serialize(CArchive& ar)
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
// CRTLSampleDoc diagnostics

#ifdef _DEBUG
void CRTLSampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRTLSampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleDoc commands
