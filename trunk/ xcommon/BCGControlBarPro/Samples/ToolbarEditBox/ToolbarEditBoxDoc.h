// ToolbarEditBoxDoc.h : interface of the CToolbarEditBoxDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBAREDITBOXDOC_H__524FF1D9_B2D4_460F_BF53_027465DAAB4F__INCLUDED_)
#define AFX_TOOLBAREDITBOXDOC_H__524FF1D9_B2D4_460F_BF53_027465DAAB4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolbarEditBoxDoc : public CDocument
{
protected: // create from serialization only
	CToolbarEditBoxDoc();
	DECLARE_DYNCREATE(CToolbarEditBoxDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarEditBoxDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolbarEditBoxDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarEditBoxDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREDITBOXDOC_H__524FF1D9_B2D4_460F_BF53_027465DAAB4F__INCLUDED_)
