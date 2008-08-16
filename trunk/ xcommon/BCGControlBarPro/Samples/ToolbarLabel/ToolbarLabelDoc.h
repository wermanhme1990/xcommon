// ToolbarLabelDoc.h : interface of the CToolbarLabelDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARLABELDOC_H__0EF36C73_2EB3_4469_9170_952C49CD3835__INCLUDED_)
#define AFX_TOOLBARLABELDOC_H__0EF36C73_2EB3_4469_9170_952C49CD3835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolbarLabelDoc : public CDocument
{
protected: // create from serialization only
	CToolbarLabelDoc();
	DECLARE_DYNCREATE(CToolbarLabelDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarLabelDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolbarLabelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarLabelDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARLABELDOC_H__0EF36C73_2EB3_4469_9170_952C49CD3835__INCLUDED_)
