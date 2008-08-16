// CustomPagesDoc.h : interface of the CCustomPagesDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMPAGESDOC_H__283FE519_A266_48C3_831C_4D1FF30AE847__INCLUDED_)
#define AFX_CUSTOMPAGESDOC_H__283FE519_A266_48C3_831C_4D1FF30AE847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomPagesDoc : public CDocument
{
protected: // create from serialization only
	CCustomPagesDoc();
	DECLARE_DYNCREATE(CCustomPagesDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomPagesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomPagesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomPagesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMPAGESDOC_H__283FE519_A266_48C3_831C_4D1FF30AE847__INCLUDED_)
