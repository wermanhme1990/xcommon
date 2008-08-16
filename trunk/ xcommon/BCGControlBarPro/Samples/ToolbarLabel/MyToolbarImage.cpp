// MyToolbarImage.cpp: implementation of the CMyToolbarImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolbarLabel.h"
#include "MyToolbarImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMyToolbarImage, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyToolbarImage::CMyToolbarImage (UINT uiID)
{
	m_bText = FALSE;	
	m_bImage = TRUE;
	m_nID = uiID;

	m_bmpMyImage.LoadBitmap (IDB_MY_BMP_LABEL);
}

CMyToolbarImage::~CMyToolbarImage()
{

}

void CMyToolbarImage::OnDraw (CDC* pDC, const CRect& rect, 
							 CBCGPToolBarImages* pImages, 
							 BOOL bHorz, BOOL bCustomizeMode,						
							 BOOL bHighlight,						
							 BOOL bDrawBorder, 
							 BOOL bGrayDisabledButtons)
{
	pDC->DrawState (rect.TopLeft (), rect.Size (), &m_bmpMyImage, DST_BITMAP); 
}		 


SIZE CMyToolbarImage::OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	BITMAP bm;
	m_bmpMyImage.GetBitmap (&bm);
	CSize size (bm.bmWidth, bm.bmHeight);
	return size;
}
