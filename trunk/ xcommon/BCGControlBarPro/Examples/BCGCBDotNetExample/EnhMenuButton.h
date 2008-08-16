#if !defined(AFX_ENHMENUBUTTON_H__B6587EF5_DC29_4141_B764_9F4355CBDF81__INCLUDED_)
#define AFX_ENHMENUBUTTON_H__B6587EF5_DC29_4141_B764_9F4355CBDF81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnhMenuButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnhMenuButton window

class CEnhMenuButton : public CBCGPMenuButton
{
// Construction
public:
	CEnhMenuButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnhMenuButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnhMenuButton();

protected:
	virtual void OnDraw (CDC* pDC, const CRect& rect, UINT uiState);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnhMenuButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENHMENUBUTTON_H__B6587EF5_DC29_4141_B764_9F4355CBDF81__INCLUDED_)
