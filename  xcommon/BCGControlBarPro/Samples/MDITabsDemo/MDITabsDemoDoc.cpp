// MDITabsDemoDoc.cpp : implementation of the CMDITabsDemoDoc class
//

#include "stdafx.h"
#include "MDITabsDemo.h"

#include "MDITabsDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoDoc

IMPLEMENT_DYNCREATE(CMDITabsDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDITabsDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CMDITabsDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoDoc construction/destruction

CMDITabsDemoDoc::CMDITabsDemoDoc()
{
	// TODO: add one-time construction code here

}

CMDITabsDemoDoc::~CMDITabsDemoDoc()
{
}

BOOL CMDITabsDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoDoc serialization

void CMDITabsDemoDoc::Serialize(CArchive& ar)
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
// CMDITabsDemoDoc diagnostics

#ifdef _DEBUG
void CMDITabsDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDITabsDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoDoc commands
