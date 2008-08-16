// ActiveAccessibilityDemoDoc.cpp : implementation of the CActiveAccessibilityDemoDoc class
//

#include "stdafx.h"
#include "ActiveAccessibilityDemo.h"

#include "ActiveAccessibilityDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoDoc

IMPLEMENT_DYNCREATE(CActiveAccessibilityDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CActiveAccessibilityDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CActiveAccessibilityDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoDoc construction/destruction

CActiveAccessibilityDemoDoc::CActiveAccessibilityDemoDoc()
{
	// TODO: add one-time construction code here

}

CActiveAccessibilityDemoDoc::~CActiveAccessibilityDemoDoc()
{
}

BOOL CActiveAccessibilityDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoDoc serialization

void CActiveAccessibilityDemoDoc::Serialize(CArchive& ar)
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
// CActiveAccessibilityDemoDoc diagnostics

#ifdef _DEBUG
void CActiveAccessibilityDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CActiveAccessibilityDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoDoc commands
