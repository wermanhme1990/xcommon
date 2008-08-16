#if !defined(AFX_LINKSBAR_H__9BDC9475_FAE1_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_LINKSBAR_H__9BDC9475_FAE1_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinksBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinksBar window

class CLinksBar : public CBCGPToolBar
{
	DECLARE_SERIAL(CLinksBar)

// Construction
public:
	CLinksBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinksBar)
	//}}AFX_VIRTUAL

	virtual BOOL OnSendCommand (const CBCGPToolbarButton* pButton);
	virtual BOOL CanBeRestored () const
	{
		return TRUE;
	}
	virtual BOOL RestoreOriginalstate ();

// Implementation
public:
	virtual ~CLinksBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLinksBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKSBAR_H__9BDC9475_FAE1_11D2_A713_0090274409AC__INCLUDED_)
