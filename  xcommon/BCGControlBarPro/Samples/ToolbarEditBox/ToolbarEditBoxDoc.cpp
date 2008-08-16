// ToolbarEditBoxDoc.cpp : implementation of the CToolbarEditBoxDoc class
//

#include "stdafx.h"
#include "ToolbarEditBox.h"

#include "ToolbarEditBoxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarEditBoxDoc

IMPLEMENT_DYNCREATE(CToolbarEditBoxDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolbarEditBoxDoc, CDocument)
	//{{AFX_MSG_MAP(CToolbarEditBoxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarEditBoxDoc construction/destruction

CToolbarEditBoxDoc::CToolbarEditBoxDoc()
{
	// TODO: add one-time construction code here

}

CToolbarEditBoxDoc::~CToolbarEditBoxDoc()
{
}

BOOL CToolbarEditBoxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToolbarEditBoxDoc serialization

void CToolbarEditBoxDoc::Serialize(CArchive& ar)
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
// CToolbarEditBoxDoc diagnostics

#ifdef _DEBUG
void CToolbarEditBoxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolbarEditBoxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolbarEditBoxDoc commands
