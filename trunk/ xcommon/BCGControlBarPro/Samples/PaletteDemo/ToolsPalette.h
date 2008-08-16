#if !defined(AFX_TOOLSPALETTE_H__3F9B3876_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_TOOLSPALETTE_H__3F9B3876_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolsPalette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolsPalette window

class CToolsPalette : public CBCGPToolBar
{
// Construction
public:
	CToolsPalette();

// Attributes
public:
	int GetColumns () const
	{
		return m_nColumns;
	}

protected:
	int		m_nColumns;		// Number of palette columns
	CRect	m_rectInfo;		// Additional information area

// Operations
public:
	void SetColumns (int nColumns);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsPalette)
	//}}AFX_VIRTUAL

	virtual CSize CalcSize (BOOL bVertDock);
	virtual void AdjustLocations ();
	virtual void DoPaint(CDC* pDC);
	virtual BOOL AllowShowOnList () const		
	{	
		return FALSE;	// Don't show me on the toolbars list
	}

// Implementation
public:
	virtual ~CToolsPalette();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolsPalette)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSPALETTE_H__3F9B3876_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
