// StateToFileDoc.h : interface of the CStateToFileDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATETOFILEDOC_H__B738D848_DF73_4AAD_9DD7_F1577A0D0EBA__INCLUDED_)
#define AFX_STATETOFILEDOC_H__B738D848_DF73_4AAD_9DD7_F1577A0D0EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStateToFileDoc : public CDocument
{
protected: // create from serialization only
	CStateToFileDoc();
	DECLARE_DYNCREATE(CStateToFileDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateToFileDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStateToFileDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStateToFileDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATETOFILEDOC_H__B738D848_DF73_4AAD_9DD7_F1577A0D0EBA__INCLUDED_)
