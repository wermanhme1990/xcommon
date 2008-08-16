// StatusBarDemoDoc.h : interface of the CStatusBarDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSBARDEMODOC_H__27730524_B96E_4E9C_A5B9_DCFD5FDB5F88__INCLUDED_)
#define AFX_STATUSBARDEMODOC_H__27730524_B96E_4E9C_A5B9_DCFD5FDB5F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStatusBarDemoDoc : public CDocument
{
protected: // create from serialization only
	CStatusBarDemoDoc();
	DECLARE_DYNCREATE(CStatusBarDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatusBarDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStatusBarDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARDEMODOC_H__27730524_B96E_4E9C_A5B9_DCFD5FDB5F88__INCLUDED_)
