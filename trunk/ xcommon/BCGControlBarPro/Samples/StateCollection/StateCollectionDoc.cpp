// StateCollectionDoc.cpp : implementation of the CStateCollectionDoc class
//

#include "stdafx.h"
#include "StateCollection.h"

#include "StateCollectionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStateCollectionDoc

IMPLEMENT_DYNCREATE(CStateCollectionDoc, CDocument)

BEGIN_MESSAGE_MAP(CStateCollectionDoc, CDocument)
	//{{AFX_MSG_MAP(CStateCollectionDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateCollectionDoc construction/destruction

CStateCollectionDoc::CStateCollectionDoc()
{
	// TODO: add one-time construction code here

}

CStateCollectionDoc::~CStateCollectionDoc()
{
}

BOOL CStateCollectionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CStateCollectionDoc serialization

void CStateCollectionDoc::Serialize(CArchive& ar)
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
// CStateCollectionDoc diagnostics

#ifdef _DEBUG
void CStateCollectionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStateCollectionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStateCollectionDoc commands
