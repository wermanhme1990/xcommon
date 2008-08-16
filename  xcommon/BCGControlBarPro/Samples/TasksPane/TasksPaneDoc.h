// TasksPaneDoc.h : interface of the CTasksPaneDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSPANEDOC_H__96A99064_81E4_467D_8E8D_2B4E145CDF12__INCLUDED_)
#define AFX_TASKSPANEDOC_H__96A99064_81E4_467D_8E8D_2B4E145CDF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTasksPaneDoc : public CDocument
{
protected: // create from serialization only
	CTasksPaneDoc();
	DECLARE_DYNCREATE(CTasksPaneDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTasksPaneDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTasksPaneDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTasksPaneDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSPANEDOC_H__96A99064_81E4_467D_8E8D_2B4E145CDF12__INCLUDED_)
