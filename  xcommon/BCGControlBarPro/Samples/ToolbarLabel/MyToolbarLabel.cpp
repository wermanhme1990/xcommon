// MyToolbarLabel.cpp: implementation of the CMyToolbarLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolbarLabel.h"
#include "MyToolbarLabel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMyToolbarLabel, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyToolbarLabel::CMyToolbarLabel (UINT uiID, LPCTSTR lpszText)
{
	if (lpszText != NULL)	
	{		
		m_strText = lpszText;	
	}	

	m_bText = TRUE;	
	m_nID = uiID;
}

CMyToolbarLabel::~CMyToolbarLabel()
{

}

void CMyToolbarLabel::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
							  BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,	BOOL bDrawBorder,					BOOL bGrayDisabledButtons)
{
	UINT nStyle = m_nStyle;	
	m_nStyle &= ~TBBS_DISABLED;   	
	CBCGPToolbarButton::OnDraw (pDC, rect, pImages, bHorz, bCustomizeMode, 
								bHighlight, bDrawBorder, bGrayDisabledButtons);   	
	m_nStyle = nStyle;
}

SIZE CMyToolbarLabel::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	m_iImage = -1;
	return CBCGPToolbarButton::OnCalculateSize (pDC, sizeDefault, bHorz);
}