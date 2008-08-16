// SliderDoc.h : interface of the CSliderDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIDERDOC_H__EC66C179_EE3C_4FC1_98B0_3B31DFB547FD__INCLUDED_)
#define AFX_SLIDERDOC_H__EC66C179_EE3C_4FC1_98B0_3B31DFB547FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSliderDoc : public CDocument
{
protected: // create from serialization only
	CSliderDoc();
	DECLARE_DYNCREATE(CSliderDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSliderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSliderDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERDOC_H__EC66C179_EE3C_4FC1_98B0_3B31DFB547FD__INCLUDED_)
