// DockingViewDoc.cpp : implementation of the CDockingViewDoc class
//

#include "stdafx.h"
#include "DockingView.h"

#include "DockingViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockingViewDoc

IMPLEMENT_DYNCREATE(CDockingViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CDockingViewDoc, CDocument)
	//{{AFX_MSG_MAP(CDockingViewDoc)
	ON_COMMAND(ID_VIEW_UPDATEALLVIEWS, OnViewUpdateallviews)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDockingViewDoc construction/destruction

CDockingViewDoc::CDockingViewDoc()
{
	// TODO: add one-time construction code here

}

CDockingViewDoc::~CDockingViewDoc()
{
}

BOOL CDockingViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDockingViewDoc serialization

void CDockingViewDoc::Serialize(CArchive& ar)
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
// CDockingViewDoc diagnostics

#ifdef _DEBUG
void CDockingViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDockingViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDockingViewDoc commands

void CDockingViewDoc::OnViewUpdateallviews() 
{
	UpdateAllViews (NULL);
	
}
