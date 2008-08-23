// imgframe.cpp : implementation of the CImageFrame class
//

#include "stdafx.h"
#include "resource.h"

#include "imgframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageFrame
IMPLEMENT_DYNCREATE(CImageFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CImageFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CImageFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageFrame construction/destruction
CImageFrame::CImageFrame()
{
	// TODO: add member initialization code here
}

CImageFrame::~CImageFrame()
{
}
/////////////////////////////////////////////////////////////////////////////
BOOL CImageFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CMDIChildWnd::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////
// CImageFrame diagnostics
#ifdef _DEBUG
void CImageFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}
void CImageFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CImageFrame message handlers

