// MDICustomBackgroundDoc.cpp : implementation of the CMDICustomBackgroundDoc class
//

#include "stdafx.h"
#include "MDICustomBackground.h"

#include "MDICustomBackgroundDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundDoc

IMPLEMENT_DYNCREATE(CMDICustomBackgroundDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDICustomBackgroundDoc, CDocument)
	//{{AFX_MSG_MAP(CMDICustomBackgroundDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundDoc construction/destruction

CMDICustomBackgroundDoc::CMDICustomBackgroundDoc()
{
	// TODO: add one-time construction code here

}

CMDICustomBackgroundDoc::~CMDICustomBackgroundDoc()
{
}

BOOL CMDICustomBackgroundDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundDoc serialization

void CMDICustomBackgroundDoc::Serialize(CArchive& ar)
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
// CMDICustomBackgroundDoc diagnostics

#ifdef _DEBUG
void CMDICustomBackgroundDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDICustomBackgroundDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundDoc commands
