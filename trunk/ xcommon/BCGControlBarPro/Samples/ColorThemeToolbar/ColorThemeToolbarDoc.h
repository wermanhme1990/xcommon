// ColorThemeToolbarDoc.h : interface of the CColorThemeToolbarDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORTHEMETOOLBARDOC_H__D6B4D16C_6CF5_4640_B7D8_EAE660D3E45A__INCLUDED_)
#define AFX_COLORTHEMETOOLBARDOC_H__D6B4D16C_6CF5_4640_B7D8_EAE660D3E45A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CColorThemeToolbarDoc : public CDocument
{
protected: // create from serialization only
	CColorThemeToolbarDoc();
	DECLARE_DYNCREATE(CColorThemeToolbarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorThemeToolbarDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorThemeToolbarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CColorThemeToolbarDoc)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTHEMETOOLBARDOC_H__D6B4D16C_6CF5_4640_B7D8_EAE660D3E45A__INCLUDED_)
