// OwnerDrawMenuDoc.cpp : implementation of the COwnerDrawMenuDoc class
//

#include "stdafx.h"
#include "OwnerDrawMenu.h"

#include "OwnerDrawMenuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawMenuDoc

IMPLEMENT_DYNCREATE(COwnerDrawMenuDoc, CDocument)

BEGIN_MESSAGE_MAP(COwnerDrawMenuDoc, CDocument)
	//{{AFX_MSG_MAP(COwnerDrawMenuDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawMenuDoc construction/destruction

COwnerDrawMenuDoc::COwnerDrawMenuDoc()
{
	// TODO: add one-time construction code here

}

COwnerDrawMenuDoc::~COwnerDrawMenuDoc()
{
}

BOOL COwnerDrawMenuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COwnerDrawMenuDoc serialization

void COwnerDrawMenuDoc::Serialize(CArchive& ar)
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
// COwnerDrawMenuDoc diagnostics

#ifdef _DEBUG
void COwnerDrawMenuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COwnerDrawMenuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawMenuDoc commands
