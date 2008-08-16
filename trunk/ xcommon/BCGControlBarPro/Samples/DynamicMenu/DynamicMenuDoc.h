// DynamicMenuDoc.h : interface of the CDynamicMenuDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNAMICMENUDOC_H__DA541AB1_D646_490E_B5B7_9F92B53EA131__INCLUDED_)
#define AFX_DYNAMICMENUDOC_H__DA541AB1_D646_490E_B5B7_9F92B53EA131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynamicMenuDoc : public CDocument
{
protected: // create from serialization only
	CDynamicMenuDoc();
	DECLARE_DYNCREATE(CDynamicMenuDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicMenuDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynamicMenuDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDynamicMenuDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICMENUDOC_H__DA541AB1_D646_490E_B5B7_9F92B53EA131__INCLUDED_)
