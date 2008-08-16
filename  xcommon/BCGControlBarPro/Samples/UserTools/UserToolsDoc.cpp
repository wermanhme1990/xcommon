// UserToolsDoc.cpp : implementation of the CUserToolsDoc class
//

#include "stdafx.h"
#include "UserTools.h"

#include "UserToolsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserToolsDoc

IMPLEMENT_DYNCREATE(CUserToolsDoc, CDocument)

BEGIN_MESSAGE_MAP(CUserToolsDoc, CDocument)
	//{{AFX_MSG_MAP(CUserToolsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserToolsDoc construction/destruction

CUserToolsDoc::CUserToolsDoc()
{
	// TODO: add one-time construction code here

}

CUserToolsDoc::~CUserToolsDoc()
{
}

BOOL CUserToolsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUserToolsDoc serialization

void CUserToolsDoc::Serialize(CArchive& ar)
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
// CUserToolsDoc diagnostics

#ifdef _DEBUG
void CUserToolsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUserToolsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUserToolsDoc commands
