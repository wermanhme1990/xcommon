// TabbedViewDoc.cpp : implementation of the CTabbedViewDoc class
//

#include "stdafx.h"
#include "TabbedView.h"

#include "TabbedViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc

IMPLEMENT_DYNCREATE(CTabbedViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CTabbedViewDoc, CDocument)
	//{{AFX_MSG_MAP(CTabbedViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc construction/destruction

CTabbedViewDoc::CTabbedViewDoc()
{
	// TODO: add one-time construction code here

}

CTabbedViewDoc::~CTabbedViewDoc()
{
}

BOOL CTabbedViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc serialization

void CTabbedViewDoc::Serialize(CArchive& ar)
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
// CTabbedViewDoc diagnostics

#ifdef _DEBUG
void CTabbedViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTabbedViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc commands
