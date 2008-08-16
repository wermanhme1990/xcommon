// ReportDemoDoc.h : interface of the CReportDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTDEMODOC_H__51126F62_52A7_4B0D_BFB4_16F0A1F08A7C__INCLUDED_)
#define AFX_REPORTDEMODOC_H__51126F62_52A7_4B0D_BFB4_16F0A1F08A7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CReportDemoDoc : public CDocument
{
protected: // create from serialization only
	CReportDemoDoc();
	DECLARE_DYNCREATE(CReportDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CReportDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDEMODOC_H__51126F62_52A7_4B0D_BFB4_16F0A1F08A7C__INCLUDED_)
