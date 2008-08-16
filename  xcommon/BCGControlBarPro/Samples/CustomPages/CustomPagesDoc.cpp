// CustomPagesDoc.cpp : implementation of the CCustomPagesDoc class
//

#include "stdafx.h"
#include "CustomPages.h"

#include "CustomPagesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesDoc

IMPLEMENT_DYNCREATE(CCustomPagesDoc, CDocument)

BEGIN_MESSAGE_MAP(CCustomPagesDoc, CDocument)
	//{{AFX_MSG_MAP(CCustomPagesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesDoc construction/destruction

CCustomPagesDoc::CCustomPagesDoc()
{
	// TODO: add one-time construction code here

}

CCustomPagesDoc::~CCustomPagesDoc()
{
}

BOOL CCustomPagesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCustomPagesDoc serialization

void CCustomPagesDoc::Serialize(CArchive& ar)
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
// CCustomPagesDoc diagnostics

#ifdef _DEBUG
void CCustomPagesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCustomPagesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesDoc commands
