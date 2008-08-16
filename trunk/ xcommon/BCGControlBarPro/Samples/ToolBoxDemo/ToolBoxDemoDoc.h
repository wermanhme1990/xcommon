// ToolBoxDemoDoc.h : interface of the CToolBoxDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBOXDEMODOC_H__875707F6_E768_4A86_B49C_C26AA97F1D6D__INCLUDED_)
#define AFX_TOOLBOXDEMODOC_H__875707F6_E768_4A86_B49C_C26AA97F1D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolBoxDemoDoc : public CDocument
{
protected: // create from serialization only
	CToolBoxDemoDoc();
	DECLARE_DYNCREATE(CToolBoxDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBoxDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBOXDEMODOC_H__875707F6_E768_4A86_B49C_C26AA97F1D6D__INCLUDED_)
