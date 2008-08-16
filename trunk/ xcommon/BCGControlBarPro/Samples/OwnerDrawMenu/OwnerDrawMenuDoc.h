// OwnerDrawMenuDoc.h : interface of the COwnerDrawMenuDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERDRAWMENUDOC_H__F578A1D2_26EB_4D41_8395_653889EC7B6B__INCLUDED_)
#define AFX_OWNERDRAWMENUDOC_H__F578A1D2_26EB_4D41_8395_653889EC7B6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COwnerDrawMenuDoc : public CDocument
{
protected: // create from serialization only
	COwnerDrawMenuDoc();
	DECLARE_DYNCREATE(COwnerDrawMenuDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawMenuDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwnerDrawMenuDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COwnerDrawMenuDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWMENUDOC_H__F578A1D2_26EB_4D41_8395_653889EC7B6B__INCLUDED_)
