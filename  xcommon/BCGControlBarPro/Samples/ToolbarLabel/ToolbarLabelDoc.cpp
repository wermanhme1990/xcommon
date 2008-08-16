// ToolbarLabelDoc.cpp : implementation of the CToolbarLabelDoc class
//

#include "stdafx.h"
#include "ToolbarLabel.h"

#include "ToolbarLabelDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelDoc

IMPLEMENT_DYNCREATE(CToolbarLabelDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolbarLabelDoc, CDocument)
	//{{AFX_MSG_MAP(CToolbarLabelDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelDoc construction/destruction

CToolbarLabelDoc::CToolbarLabelDoc()
{
	// TODO: add one-time construction code here

}

CToolbarLabelDoc::~CToolbarLabelDoc()
{
}

BOOL CToolbarLabelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelDoc serialization

void CToolbarLabelDoc::Serialize(CArchive& ar)
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
// CToolbarLabelDoc diagnostics

#ifdef _DEBUG
void CToolbarLabelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolbarLabelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelDoc commands
