#if !defined(AFX_PREVIEWPANE_H__008A70C2_C887_11D4_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_PREVIEWPANE_H__008A70C2_C887_11D4_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane view

class CPreviewPane : public CView
{
protected:
	CPreviewPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPreviewPane)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewPane)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPreviewPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewPane)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWPANE_H__008A70C2_C887_11D4_95C8_00A0C9289F1B__INCLUDED_)
