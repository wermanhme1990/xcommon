// MyCustomuzeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserTools.h"
#include "MyCustomuzeDlg.h"
#include "SampleUserTool.h"
#include "PropsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCustomuzeDlg

IMPLEMENT_DYNAMIC(CMyCustomuzeDlg, CBCGPToolbarCustomize)

CMyCustomuzeDlg::CMyCustomuzeDlg (CFrameWnd* pWndParentFrame)
	: CBCGPToolbarCustomize (pWndParentFrame, TRUE /* AutoSetFromMenus */)
{
}

CMyCustomuzeDlg::~CMyCustomuzeDlg()
{
}


BEGIN_MESSAGE_MAP(CMyCustomuzeDlg, CBCGPToolbarCustomize)
	//{{AFX_MSG_MAP(CMyCustomuzeDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCustomuzeDlg message handlers

void CMyCustomuzeDlg::OnInitToolsPage ()
{
	ASSERT_VALID (m_pToolsPage);

	//----------------------------------------------------------
	// Create additional propety controls in the bottom of page:
	//----------------------------------------------------------
	CRect rectToolsPage;
	m_pToolsPage->GetClientRect (rectToolsPage);
	rectToolsPage.DeflateRect (10, 10);

	CClientDC dc (m_pToolsPage);
	CFont* pOldFont = dc.SelectObject (m_pToolsPage->GetFont ());

	const CString strLabel1 = _T("Param1: ");
	CSize sizeLabel1 = dc.GetTextExtent (strLabel1);

	CRect rectLabel1 (CPoint (rectToolsPage.left, rectToolsPage.bottom - sizeLabel1.cy), sizeLabel1);
	m_wndParam1Label.Create (strLabel1, WS_CHILD | WS_VISIBLE, rectLabel1, m_pToolsPage);
	m_wndParam1Label.SetFont (m_pToolsPage->GetFont ());

	CRect rectEdit1 (CPoint (rectLabel1.right + 10, rectToolsPage.bottom - sizeLabel1.cy - 2),
					CSize (100, sizeLabel1.cy + 4));
	m_wndParam1.Create (WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
						rectEdit1, m_pToolsPage, (UINT)-1);
	m_wndParam1.SetFont (m_pToolsPage->GetFont ());

	m_wndParam1.EnableWindow (FALSE);

	const CString strButton2 = _T("Param2...");
	CSize sizeButton2 = dc.GetTextExtent (strButton2);
	CRect rectBtn (CPoint (rectEdit1.right + 20, rectToolsPage.bottom - sizeButton2.cy - 5),
					CSize (sizeButton2.cx + 10, sizeButton2.cy + 10));
	m_wndParam2.Create (strButton2, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
						rectBtn, m_pToolsPage, 101);
	m_wndParam2.SetFont (m_pToolsPage->GetFont ());
	m_wndParam2.EnableWindow (FALSE);

	dc.SelectObject (pOldFont);
}

void CMyCustomuzeDlg::OnBeforeChangeTool (CBCGPUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		CSampleUserTool* pTool = DYNAMIC_DOWNCAST (CSampleUserTool, pSelTool);
		ASSERT_VALID (pTool);

		m_wndParam1.GetWindowText (pTool->m_strProp1);
	}
}

void CMyCustomuzeDlg::OnAfterChangeTool (CBCGPUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (data -> screen):
		//--------------------------------------------------
		CSampleUserTool* pTool = DYNAMIC_DOWNCAST (CSampleUserTool, pSelTool);
		ASSERT_VALID (pTool);

		m_wndParam1.SetWindowText (pTool->m_strProp1);
		m_wndParam2.m_pTool = pTool;
	}
	else
	{
		m_wndParam1.SetWindowText (_T(""));
		m_wndParam2.m_pTool = NULL;
	}

	m_wndParam1.EnableWindow (pSelTool != NULL);
	m_wndParam2.EnableWindow (pSelTool != NULL);
}

void CMyCustomuzeDlg::OnDestroy() 
{
	ASSERT_VALID (m_pToolsPage);

	if (m_pToolsPage->GetSafeHwnd () != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		int iCurSel = m_pToolsPage->m_wndToolsList.GetSelItem ();
		if (iCurSel >= 0)
		{
			CSampleUserTool* pTool = DYNAMIC_DOWNCAST (CSampleUserTool, 
				(CBCGPUserTool*) m_pToolsPage->m_wndToolsList.GetItemData (iCurSel));
			ASSERT_VALID (pTool);

			m_wndParam1.GetWindowText (pTool->m_strProp1);
		}
	}

	CBCGPToolbarCustomize::OnDestroy();
}

BOOL CMyCustomuzeDlg::CheckToolsValidity (const CObList& lstTools)
{
	for (POSITION pos = lstTools.GetHeadPosition (); pos != NULL;)
	{
		CSampleUserTool* pTool = (CSampleUserTool*) lstTools.GetNext (pos);
		ASSERT_VALID (pTool);

		if (pTool->GetCommand ().IsEmpty ())
		{
			MessageBox (_T("Command is required"));
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPropsBtn

CPropsBtn::CPropsBtn()
{
	m_pTool = NULL;
}

CPropsBtn::~CPropsBtn()
{
}


BEGIN_MESSAGE_MAP(CPropsBtn, CButton)
	//{{AFX_MSG_MAP(CPropsBtn)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropsBtn message handlers

void CPropsBtn::OnClicked() 
{
	ASSERT_VALID (m_pTool);

	CPropsDlg dlg;
	dlg.m_strProp2 = m_pTool->m_strProp2;

	if (dlg.DoModal () == IDOK)
	{
		m_pTool->m_strProp2 = dlg.m_strProp2;
	}
}
