// SmartDockingDemoDoc.h : interface of the CSmartDockingDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTDOCKINGDEMODOC_H__124BE9D1_4051_4603_8F0A_FBE033E55FB6__INCLUDED_)
#define AFX_SMARTDOCKINGDEMODOC_H__124BE9D1_4051_4603_8F0A_FBE033E55FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSmartDockingDemoDoc : public CDocument
{
protected: // create from serialization only
	CSmartDockingDemoDoc();
	DECLARE_DYNCREATE(CSmartDockingDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartDockingDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmartDockingDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSmartDockingDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMARTDOCKINGDEMODOC_H__124BE9D1_4051_4603_8F0A_FBE033E55FB6__INCLUDED_)
