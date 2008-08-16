// UndoBoxDoc.h : interface of the CUndoBoxDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOBOXDOC_H__5AAF5FBD_1952_4570_9390_463B37CDC8C9__INCLUDED_)
#define AFX_UNDOBOXDOC_H__5AAF5FBD_1952_4570_9390_463B37CDC8C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUndoBoxDoc : public CDocument
{
protected: // create from serialization only
	CUndoBoxDoc();
	DECLARE_DYNCREATE(CUndoBoxDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoBoxDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUndoBoxDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUndoBoxDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOBOXDOC_H__5AAF5FBD_1952_4570_9390_463B37CDC8C9__INCLUDED_)
