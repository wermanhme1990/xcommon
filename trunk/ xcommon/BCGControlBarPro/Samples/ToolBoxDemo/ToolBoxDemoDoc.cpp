// ToolBoxDemoDoc.cpp : implementation of the CToolBoxDemoDoc class
//

#include "stdafx.h"
#include "ToolBoxDemo.h"

#include "ToolBoxDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoDoc

IMPLEMENT_DYNCREATE(CToolBoxDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolBoxDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CToolBoxDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoDoc construction/destruction

CToolBoxDemoDoc::CToolBoxDemoDoc()
{
	// TODO: add one-time construction code here

}

CToolBoxDemoDoc::~CToolBoxDemoDoc()
{
}

BOOL CToolBoxDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoDoc serialization

void CToolBoxDemoDoc::Serialize(CArchive& ar)
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
// CToolBoxDemoDoc diagnostics

#ifdef _DEBUG
void CToolBoxDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolBoxDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoDoc commands
