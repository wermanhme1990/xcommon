// FileDialogDemoDoc.h : interface of the CFileDialogDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDIALOGDEMODOC_H__81D7C48C_030D_41CC_8E58_5DD23BD14035__INCLUDED_)
#define AFX_FILEDIALOGDEMODOC_H__81D7C48C_030D_41CC_8E58_5DD23BD14035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFileDialogDemoDoc : public CDocument
{
protected: // create from serialization only
	CFileDialogDemoDoc();
	DECLARE_DYNCREATE(CFileDialogDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDialogDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileDialogDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFileDialogDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGDEMODOC_H__81D7C48C_030D_41CC_8E58_5DD23BD14035__INCLUDED_)
