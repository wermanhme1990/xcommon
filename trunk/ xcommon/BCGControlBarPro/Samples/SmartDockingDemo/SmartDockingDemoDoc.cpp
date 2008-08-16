// SmartDockingDemoDoc.cpp : implementation of the CSmartDockingDemoDoc class
//

#include "stdafx.h"
#include "SmartDockingDemo.h"

#include "SmartDockingDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoDoc

IMPLEMENT_DYNCREATE(CSmartDockingDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CSmartDockingDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CSmartDockingDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoDoc construction/destruction

CSmartDockingDemoDoc::CSmartDockingDemoDoc()
{
	// TODO: add one-time construction code here

}

CSmartDockingDemoDoc::~CSmartDockingDemoDoc()
{
}

BOOL CSmartDockingDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoDoc serialization

void CSmartDockingDemoDoc::Serialize(CArchive& ar)
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
// CSmartDockingDemoDoc diagnostics

#ifdef _DEBUG
void CSmartDockingDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSmartDockingDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoDoc commands
