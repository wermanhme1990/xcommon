// MDIBarsDoc.cpp : implementation of the CMDIBarsDoc class
//

#include "stdafx.h"
#include "MDIBars.h"

#include "MDIBarsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIBarsDoc

IMPLEMENT_DYNCREATE(CMDIBarsDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIBarsDoc, CDocument)
	//{{AFX_MSG_MAP(CMDIBarsDoc)
	ON_COMMAND(ID_COMMAND_1, OnCommand1)
	ON_COMMAND(ID_COMMAND2, OnCommand2)
	ON_COMMAND(ID_COMMAND3, OnCommand3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIBarsDoc construction/destruction

CMDIBarsDoc::CMDIBarsDoc()
{
	// TODO: add one-time construction code here

}

CMDIBarsDoc::~CMDIBarsDoc()
{
}

BOOL CMDIBarsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDIBarsDoc serialization

void CMDIBarsDoc::Serialize(CArchive& ar)
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
// CMDIBarsDoc diagnostics

#ifdef _DEBUG
void CMDIBarsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIBarsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIBarsDoc commands

void CMDIBarsDoc::OnCommand1() 
{
	AfxMessageBox ("OnCommand1...");
}

void CMDIBarsDoc::OnCommand2() 
{
	AfxMessageBox ("OnCommand2...");
}

void CMDIBarsDoc::OnCommand3() 
{
	AfxMessageBox ("OnCommand3...");
}
