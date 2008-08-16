// SliderDoc.cpp : implementation of the CSliderDoc class
//

#include "stdafx.h"
#include "Slider.h"

#include "SliderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderDoc

IMPLEMENT_DYNCREATE(CSliderDoc, CDocument)

BEGIN_MESSAGE_MAP(CSliderDoc, CDocument)
	//{{AFX_MSG_MAP(CSliderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderDoc construction/destruction

CSliderDoc::CSliderDoc()
{
	// TODO: add one-time construction code here

}

CSliderDoc::~CSliderDoc()
{
}

BOOL CSliderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSliderDoc serialization

void CSliderDoc::Serialize(CArchive& ar)
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
// CSliderDoc diagnostics

#ifdef _DEBUG
void CSliderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSliderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSliderDoc commands
