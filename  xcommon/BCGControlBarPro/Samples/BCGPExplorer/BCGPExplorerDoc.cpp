// BCGPExplorerDoc.cpp : implementation of the CBCGPExplorerDoc class
//

#include "stdafx.h"
#include "BCGPExplorer.h"

#include "BCGPExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerDoc

IMPLEMENT_DYNCREATE(CBCGPExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CBCGPExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CBCGPExplorerDoc)
	ON_COMMAND(ID_COMMAND_HISTORY, OnCommandHistory)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerDoc construction/destruction

CBCGPExplorerDoc::CBCGPExplorerDoc()
{
	// TODO: add one-time construction code here

}

CBCGPExplorerDoc::~CBCGPExplorerDoc()
{
}

BOOL CBCGPExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerDoc serialization

void CBCGPExplorerDoc::Serialize(CArchive& ar)
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
// CBCGPExplorerDoc diagnostics

#ifdef _DEBUG
void CBCGPExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBCGPExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerDoc commands

void CBCGPExplorerDoc::OnCommandHistory() 
{
	// TODO: Add your command handler code here
	
}

void CBCGPExplorerDoc::OnGoBack() 
{
	// TODO: Add your command handler code here
	
}

void CBCGPExplorerDoc::OnGoForward() 
{
	// TODO: Add your command handler code here
	
}
