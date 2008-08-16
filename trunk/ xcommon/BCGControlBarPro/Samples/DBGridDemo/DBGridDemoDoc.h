// DBGridDemoDoc.h : interface of the CDBGridDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBGRIDDEMODOC_H__A9449700_89D2_436A_8D6F_019692B0C9DB__INCLUDED_)
#define AFX_DBGRIDDEMODOC_H__A9449700_89D2_436A_8D6F_019692B0C9DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDBGridDemoDoc : public CDocument
{
protected: // create from serialization only
	CDBGridDemoDoc();
	DECLARE_DYNCREATE(CDBGridDemoDoc)

// Attributes
public:
	CString	m_strDBPath;

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBGridDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDBGridDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDBGridDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBGRIDDEMODOC_H__A9449700_89D2_436A_8D6F_019692B0C9DB__INCLUDED_)
