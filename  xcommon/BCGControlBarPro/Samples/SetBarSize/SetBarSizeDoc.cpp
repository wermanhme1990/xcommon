// SetBarSizeDoc.cpp : implementation of the CSetBarSizeDoc class
//

#include "stdafx.h"
#include "SetBarSize.h"

#include "SetBarSizeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeDoc

IMPLEMENT_DYNCREATE(CSetBarSizeDoc, CDocument)

BEGIN_MESSAGE_MAP(CSetBarSizeDoc, CDocument)
	//{{AFX_MSG_MAP(CSetBarSizeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeDoc construction/destruction

CSetBarSizeDoc::CSetBarSizeDoc()
{
	// TODO: add one-time construction code here

}

CSetBarSizeDoc::~CSetBarSizeDoc()
{
}

BOOL CSetBarSizeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeDoc serialization

void CSetBarSizeDoc::Serialize(CArchive& ar)
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
// CSetBarSizeDoc diagnostics

#ifdef _DEBUG
void CSetBarSizeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSetBarSizeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeDoc commands
