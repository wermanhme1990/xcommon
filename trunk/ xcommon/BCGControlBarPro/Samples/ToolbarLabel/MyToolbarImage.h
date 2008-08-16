// MyToolbarImage.h: interface for the CMyToolbarImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTOOLBARIMAGE_H__8093DA5D_A40C_4B4B_BD3F_8DA8DCE1CB14__INCLUDED_)
#define AFX_MYTOOLBARIMAGE_H__8093DA5D_A40C_4B4B_BD3F_8DA8DCE1CB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyToolbarImage : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CMyToolbarImage)
public:
	CMyToolbarImage(UINT uiID = 0);
	virtual ~CMyToolbarImage();

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
						 BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,						BOOL bHighlight = FALSE,						BOOL bDrawBorder = TRUE, 
						 BOOL bGrayDisabledButtons = TRUE);   	

	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}

	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);

protected:
	CBitmap	m_bmpMyImage;
};

#endif // !defined(AFX_MYTOOLBARIMAGE_H__8093DA5D_A40C_4B4B_BD3F_8DA8DCE1CB14__INCLUDED_)
