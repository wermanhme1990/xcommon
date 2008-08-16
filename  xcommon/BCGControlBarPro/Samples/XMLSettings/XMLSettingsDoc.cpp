// XMLSettingsDoc.cpp : implementation of the CXMLSettingsDoc class
//

#include "stdafx.h"
#include "XMLSettings.h"

#include "XMLSettingsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsDoc

IMPLEMENT_DYNCREATE(CXMLSettingsDoc, CDocument)

BEGIN_MESSAGE_MAP(CXMLSettingsDoc, CDocument)
	//{{AFX_MSG_MAP(CXMLSettingsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsDoc construction/destruction

CXMLSettingsDoc::CXMLSettingsDoc()
{
	// TODO: add one-time construction code here

}

CXMLSettingsDoc::~CXMLSettingsDoc()
{
}

BOOL CXMLSettingsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsDoc serialization

void CXMLSettingsDoc::Serialize(CArchive& ar)
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
// CXMLSettingsDoc diagnostics

#ifdef _DEBUG
void CXMLSettingsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXMLSettingsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsDoc commands
