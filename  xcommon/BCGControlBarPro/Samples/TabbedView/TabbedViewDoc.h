// TabbedViewDoc.h : interface of the CTabbedViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDVIEWDOC_H__95B05E3A_7C35_4952_8C2A_49C4155018FC__INCLUDED_)
#define AFX_TABBEDVIEWDOC_H__95B05E3A_7C35_4952_8C2A_49C4155018FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabbedViewDoc : public CDocument
{
protected: // create from serialization only
	CTabbedViewDoc();
	DECLARE_DYNCREATE(CTabbedViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabbedViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDVIEWDOC_H__95B05E3A_7C35_4952_8C2A_49C4155018FC__INCLUDED_)
