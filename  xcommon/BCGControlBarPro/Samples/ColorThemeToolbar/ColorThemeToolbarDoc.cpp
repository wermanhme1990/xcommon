// ColorThemeToolbarDoc.cpp : implementation of the CColorThemeToolbarDoc class
//

#include "stdafx.h"
#include "ColorThemeToolbar.h"

#include "ColorThemeToolbarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarDoc

IMPLEMENT_DYNCREATE(CColorThemeToolbarDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorThemeToolbarDoc, CDocument)
	//{{AFX_MSG_MAP(CColorThemeToolbarDoc)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarDoc construction/destruction

CColorThemeToolbarDoc::CColorThemeToolbarDoc()
{
	// TODO: add one-time construction code here

}

CColorThemeToolbarDoc::~CColorThemeToolbarDoc()
{
}

BOOL CColorThemeToolbarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarDoc serialization

void CColorThemeToolbarDoc::Serialize(CArchive& ar)
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
// CColorThemeToolbarDoc diagnostics

#ifdef _DEBUG
void CColorThemeToolbarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorThemeToolbarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarDoc commands

void CColorThemeToolbarDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here
	
}

void CColorThemeToolbarDoc::OnEditCut() 
{
	// TODO: Add your command handler code here
	
}

void CColorThemeToolbarDoc::OnEditPaste() 
{
	// TODO: Add your command handler code here
	
}
