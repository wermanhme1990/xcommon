#if !defined(AFX_CMDFRAME_H__3826063B_E3D2_41A4_BFB5_950399B694D1__INCLUDED_)
#define AFX_CMDFRAME_H__3826063B_E3D2_41A4_BFB5_950399B694D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdFrame frame

class CCmdFrame : public CBCGPFrameWnd
{
public:
	CCmdFrame(CBCGPDialog* pDlg);

// Attributes
protected:
	CBCGPDialog* m_pDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdFrame)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCmdFrame();

	// Generated message map functions
	//{{AFX_MSG(CCmdFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDFRAME_H__3826063B_E3D2_41A4_BFB5_950399B694D1__INCLUDED_)
