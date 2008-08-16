// StateToFileDoc.cpp : implementation of the CStateToFileDoc class
//

#include "stdafx.h"
#include "StateToFile.h"

#include "StateToFileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStateToFileDoc

IMPLEMENT_DYNCREATE(CStateToFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CStateToFileDoc, CDocument)
	//{{AFX_MSG_MAP(CStateToFileDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateToFileDoc construction/destruction

CStateToFileDoc::CStateToFileDoc()
{
	// TODO: add one-time construction code here

}

CStateToFileDoc::~CStateToFileDoc()
{
}

BOOL CStateToFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CStateToFileDoc serialization

void CStateToFileDoc::Serialize(CArchive& ar)
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
// CStateToFileDoc diagnostics

#ifdef _DEBUG
void CStateToFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStateToFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStateToFileDoc commands
