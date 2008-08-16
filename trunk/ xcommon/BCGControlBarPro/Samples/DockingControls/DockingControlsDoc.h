// DockingControlsDoc.h : interface of the CDockingControlsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKINGCONTROLSDOC_H__A12D118F_EB33_41BD_8781_7A28E0073C56__INCLUDED_)
#define AFX_DOCKINGCONTROLSDOC_H__A12D118F_EB33_41BD_8781_7A28E0073C56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDockingControlsDoc : public CDocument
{
protected: // create from serialization only
	CDockingControlsDoc();
	DECLARE_DYNCREATE(CDockingControlsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingControlsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDockingControlsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDockingControlsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGCONTROLSDOC_H__A12D118F_EB33_41BD_8781_7A28E0073C56__INCLUDED_)
