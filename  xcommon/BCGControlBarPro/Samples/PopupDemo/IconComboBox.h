#if !defined(AFX_ICONCOMBOBOX_H__F761D90B_99D6_4314_A21A_4B30912AF070__INCLUDED_)
#define AFX_ICONCOMBOBOX_H__F761D90B_99D6_4314_A21A_4B30912AF070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox window

class CIconComboBox : public CComboBox
{
// Construction
public:
	CIconComboBox(CBCGPToolBarImages& Icons);

// Attributes
protected:
	CBCGPToolBarImages&	m_Icons;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONCOMBOBOX_H__F761D90B_99D6_4314_A21A_4B30912AF070__INCLUDED_)
