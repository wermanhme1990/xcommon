// StateCollectionDoc.h : interface of the CStateCollectionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATECOLLECTIONDOC_H__EBC1E624_A2A3_4B3E_BCD8_A1EADD10237C__INCLUDED_)
#define AFX_STATECOLLECTIONDOC_H__EBC1E624_A2A3_4B3E_BCD8_A1EADD10237C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStateCollectionDoc : public CDocument
{
protected: // create from serialization only
	CStateCollectionDoc();
	DECLARE_DYNCREATE(CStateCollectionDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateCollectionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStateCollectionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStateCollectionDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATECOLLECTIONDOC_H__EBC1E624_A2A3_4B3E_BCD8_A1EADD10237C__INCLUDED_)
