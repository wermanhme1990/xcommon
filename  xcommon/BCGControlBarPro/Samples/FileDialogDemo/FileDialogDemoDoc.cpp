// FileDialogDemoDoc.cpp : implementation of the CFileDialogDemoDoc class
//

#include "stdafx.h"
#include "FileDialogDemo.h"

#include "FileDialogDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoDoc

IMPLEMENT_DYNCREATE(CFileDialogDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileDialogDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CFileDialogDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoDoc construction/destruction

CFileDialogDemoDoc::CFileDialogDemoDoc()
{
	// TODO: add one-time construction code here

}

CFileDialogDemoDoc::~CFileDialogDemoDoc()
{
}

BOOL CFileDialogDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoDoc serialization

void CFileDialogDemoDoc::Serialize(CArchive& ar)
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
// CFileDialogDemoDoc diagnostics

#ifdef _DEBUG
void CFileDialogDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFileDialogDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoDoc commands
