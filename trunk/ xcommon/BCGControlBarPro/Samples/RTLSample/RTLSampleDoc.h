// RTLSampleDoc.h : interface of the CRTLSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTLSAMPLEDOC_H__D3DA624B_4366_43FC_AD66_699425F02D2D__INCLUDED_)
#define AFX_RTLSAMPLEDOC_H__D3DA624B_4366_43FC_AD66_699425F02D2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRTLSampleDoc : public CDocument
{
protected: // create from serialization only
	CRTLSampleDoc();
	DECLARE_DYNCREATE(CRTLSampleDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTLSampleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRTLSampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRTLSampleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTLSAMPLEDOC_H__D3DA624B_4366_43FC_AD66_699425F02D2D__INCLUDED_)
