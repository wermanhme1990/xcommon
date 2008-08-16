// LineStyleMenuItem.h: interface for the CLineStyleMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINESTYLEMENUITEM_H__AD17275B_786E_4584_B0EF_54958D4D1F6D__INCLUDED_)
#define AFX_LINESTYLEMENUITEM_H__AD17275B_786E_4584_B0EF_54958D4D1F6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLineStyleMenuItem : public CBCGPToolbarMenuButton  
{
	DECLARE_DYNCREATE(CLineStyleMenuItem)

public:
	CLineStyleMenuItem(int nWidth = 1, UINT uiCmdID = 0, LPCTSTR lpszText = NULL,
						BOOL bIsChecked = FALSE);
	virtual ~CLineStyleMenuItem();

protected:
	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);

	virtual void CopyFrom (const CBCGPToolbarButton& src);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);

	int	m_nWidth;	// Line width
};

#endif // !defined(AFX_LINESTYLEMENUITEM_H__AD17275B_786E_4584_B0EF_54958D4D1F6D__INCLUDED_)
