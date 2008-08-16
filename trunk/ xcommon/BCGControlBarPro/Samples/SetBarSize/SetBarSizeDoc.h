// SetBarSizeDoc.h : interface of the CSetBarSizeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETBARSIZEDOC_H__F24B4F35_51F2_4B4F_8236_50EF5EAAD903__INCLUDED_)
#define AFX_SETBARSIZEDOC_H__F24B4F35_51F2_4B4F_8236_50EF5EAAD903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSetBarSizeDoc : public CDocument
{
protected: // create from serialization only
	CSetBarSizeDoc();
	DECLARE_DYNCREATE(CSetBarSizeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBarSizeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetBarSizeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSetBarSizeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBARSIZEDOC_H__F24B4F35_51F2_4B4F_8236_50EF5EAAD903__INCLUDED_)
