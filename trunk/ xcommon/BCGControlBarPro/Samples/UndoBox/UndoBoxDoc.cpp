// UndoBoxDoc.cpp : implementation of the CUndoBoxDoc class
//

#include "stdafx.h"
#include "UndoBox.h"

#include "UndoBoxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxDoc

IMPLEMENT_DYNCREATE(CUndoBoxDoc, CDocument)

BEGIN_MESSAGE_MAP(CUndoBoxDoc, CDocument)
	//{{AFX_MSG_MAP(CUndoBoxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxDoc construction/destruction

CUndoBoxDoc::CUndoBoxDoc()
{
	// TODO: add one-time construction code here

}

CUndoBoxDoc::~CUndoBoxDoc()
{
}

BOOL CUndoBoxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUndoBoxDoc serialization

void CUndoBoxDoc::Serialize(CArchive& ar)
{
}

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxDoc diagnostics

#ifdef _DEBUG
void CUndoBoxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUndoBoxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxDoc commands
