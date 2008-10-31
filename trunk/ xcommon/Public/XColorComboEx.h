#pragma once
#include <XFlatCombo.h>
#include <vector>	// Used for CArray
using namespace std;
/////////////////////////////////////////////////////////////////////////////
class CXColorComboEx : public CXFlatComboBox
{
// Construction
public:
	CXColorComboEx();
public:
// Operations
public:
	COLORREF GetColor();
	void InitColor(COLORREF crInitColor);  
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorComboEx)
	public:	  	
	protected:
	virtual void PreSubclassWindow();  
	//}}AFX_VIRTUAL
  
public:
	vector<COLORREF> m_vColors;
	virtual ~CXColorComboEx();	
	// Generated message map functions
protected:
	//{{AFX_MSG(CXColorComboEx)
	afx_msg void OnSelendok();  
  afx_msg void DrawItem(LPDRAWITEMSTRUCT);
  afx_msg void MeasureItem(LPMEASUREITEMSTRUCT);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

