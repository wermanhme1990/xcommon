// DrawEffectsDoc.cpp : implementation of the CDrawEffectsDoc class
//

#include "stdafx.h"
#include "DrawEffects.h"

#include "DrawEffectsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsDoc

IMPLEMENT_DYNCREATE(CDrawEffectsDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawEffectsDoc, CDocument)
	//{{AFX_MSG_MAP(CDrawEffectsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsDoc construction/destruction

CDrawEffectsDoc::CDrawEffectsDoc()
{
	// TODO: add one-time construction code here

}

CDrawEffectsDoc::~CDrawEffectsDoc()
{
}

BOOL CDrawEffectsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsDoc serialization

void CDrawEffectsDoc::Serialize(CArchive& ar)
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
// CDrawEffectsDoc diagnostics

#ifdef _DEBUG
void CDrawEffectsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDrawEffectsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsDoc commands
