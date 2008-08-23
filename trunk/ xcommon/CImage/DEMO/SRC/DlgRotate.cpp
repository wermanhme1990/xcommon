// CRotateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog dialog

class CRotateDialog : public CDialog
{
// Construction
public:
	CRotateDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotateDialog)
	enum { IDD = IDD_ROTATE };
	double	m_angle;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotateDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog dialog

CRotateDialog::CRotateDialog(CWnd* pParent /*=NULL*/) : CDialog(CRotateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotateDialog)
	m_angle = 90.0;
	//}}AFX_DATA_INIT
}

void CRotateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateDialog)
	DDX_Text(pDX, IDC_EDIT, m_angle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRotateDialog, CDialog)
	//{{AFX_MSG_MAP(CRotateDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog message handlers

BOOL DoModal_Rotate(CWnd* pParentWnd, double* angle)
{
	CRotateDialog dlg(pParentWnd);
   dlg.m_angle = *angle;
   BOOL bOK = (IDOK == dlg.DoModal());
   if (bOK)
   {
      *angle = dlg.m_angle;
   }
   return bOK;
}