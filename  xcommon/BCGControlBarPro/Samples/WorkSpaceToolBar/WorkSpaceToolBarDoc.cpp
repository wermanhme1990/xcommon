// WorkSpaceToolBarDoc.cpp : implementation of the CWorkSpaceToolBarDoc class
//

#include "stdafx.h"
#include "WorkSpaceToolBar.h"

#include "WorkSpaceToolBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarDoc

IMPLEMENT_DYNCREATE(CWorkSpaceToolBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CWorkSpaceToolBarDoc, CDocument)
	//{{AFX_MSG_MAP(CWorkSpaceToolBarDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarDoc construction/destruction

CWorkSpaceToolBarDoc::CWorkSpaceToolBarDoc()
{
	// TODO: add one-time construction code here

}

CWorkSpaceToolBarDoc::~CWorkSpaceToolBarDoc()
{
}

BOOL CWorkSpaceToolBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarDoc serialization

void CWorkSpaceToolBarDoc::Serialize(CArchive& ar)
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
// CWorkSpaceToolBarDoc diagnostics

#ifdef _DEBUG
void CWorkSpaceToolBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorkSpaceToolBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarDoc commands
