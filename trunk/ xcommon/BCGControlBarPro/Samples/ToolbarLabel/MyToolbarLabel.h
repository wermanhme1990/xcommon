// MyToolbarLabel.h: interface for the CMyToolbarLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTOOLBARLABEL_H__5E65C64A_3CF1_46C0_BACC_61D0C67D32AB__INCLUDED_)
#define AFX_MYTOOLBARLABEL_H__5E65C64A_3CF1_46C0_BACC_61D0C67D32AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyToolbarLabel : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CMyToolbarLabel)
public:
	CMyToolbarLabel(UINT uiID = 0, LPCTSTR lpszText = NULL);
	virtual ~CMyToolbarLabel();

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
						 BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,						BOOL bHighlight = FALSE,						BOOL bDrawBorder = TRUE, 
						 BOOL bGrayDisabledButtons = TRUE);   	

	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}

	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);

};

#endif // !defined(AFX_MYTOOLBARLABEL_H__5E65C64A_3CF1_46C0_BACC_61D0C67D32AB__INCLUDED_)
