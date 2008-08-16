//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonCustomizePage.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPRibbonCustomizePage.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonKeyboardCustomizeDlg.h"
#include "BCGPLocalResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef BCGP_EXCLUDE_RIBBON

class CBCGPRibbonCustomCategory : public CObject
{
	DECLARE_DYNAMIC(CBCGPRibbonCustomCategory)

	friend class CBCGPRibbonCustomizePage;

	CString				m_strName;
	CList<UINT,UINT>	m_lstIDs;
};

IMPLEMENT_DYNAMIC(CBCGPRibbonCustomCategory, CObject)

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCustomize

IMPLEMENT_DYNAMIC(CBCGPRibbonCustomize, CBCGPPropertySheet)

CBCGPRibbonCustomize::CBCGPRibbonCustomize (CWnd* pWndParent, CBCGPRibbonBar* pRibbon) :
	CBCGPPropertySheet (_T(""), pWndParent)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CBCGPLocalResource locaRes;

	m_pPage = new CBCGPRibbonCustomizePage (pRibbon);
	AddPage (m_pPage);
}
//**********************************************************************
CBCGPRibbonCustomize::~CBCGPRibbonCustomize() 
{
	delete m_pPage;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCustomizePage property page

IMPLEMENT_DYNCREATE(CBCGPRibbonCustomizePage, CBCGPPropertyPage)

CBCGPRibbonCustomizePage::CBCGPRibbonCustomizePage(
	CBCGPRibbonBar*	pRibbonBar) : 
		CBCGPPropertyPage(CBCGPRibbonCustomizePage::IDD),
		m_wndCommandsList (pRibbonBar),
		m_wndQATList (pRibbonBar)
{
	ASSERT_VALID (pRibbonBar);

	//{{AFX_DATA_INIT(CBCGPRibbonCustomizePage)
	m_nCategory = -1;
	m_bQAToolbarOnBottom = FALSE;
	//}}AFX_DATA_INIT

	m_pRibbonBar = pRibbonBar;
	m_bQAToolbarOnBottom = !m_pRibbonBar->IsQuickAccessToolbarOnTop ();
}
//**********************************************************************
CBCGPRibbonCustomizePage::~CBCGPRibbonCustomizePage()
{
	while (!m_lstCustomCategories.IsEmpty ())
	{
		delete m_lstCustomCategories.RemoveHead ();
	}
}
//**********************************************************************
void CBCGPRibbonCustomizePage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGPRibbonCustomizePage)
	DDX_Control(pDX, IDS_BCGBARRES_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_BCGBARRES_REMOVE, m_wndRemove);
	DDX_Control(pDX, IDC_BCGBARRES_COMMANDS_LIST, m_wndCommandsList);
	DDX_Control(pDX, IDC_BCGBARRES_CATEGORY, m_wndCategoryCombo);
	DDX_Control(pDX, IDC_BCGBARRES_QAT_COMMANDS_LIST, m_wndQATList);
	DDX_Control(pDX, IDC_BCGBARRES_MOVEUP, m_wndUp);
	DDX_Control(pDX, IDC_BCGBARRES_MOVEDOWN, m_wndDown);
	DDX_CBIndex(pDX, IDC_BCGBARRES_CATEGORY, m_nCategory);
	DDX_Check(pDX, IDC_BCGBARRES_QAT_ON_BOTTOM, m_bQAToolbarOnBottom);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBCGPRibbonCustomizePage, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CBCGPRibbonCustomizePage)
	ON_CBN_SELENDOK(IDC_BCGBARRES_CATEGORY, OnSelendokCategoryCombo)
	ON_BN_CLICKED(IDS_BCGBARRES_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BCGBARRES_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_BCGBARRES_MOVEUP, OnUp)
	ON_BN_CLICKED(IDC_BCGBARRES_MOVEDOWN, OnDown)
	ON_BN_CLICKED(IDC_BCGBARRES_RESET, OnToolbarReset)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_QAT_COMMANDS_LIST, OnSelchangeQATCommands)
	ON_BN_CLICKED(IDC_BCGBARRES_KEYBOARD, OnCustomizeKeyboard)
	ON_LBN_DBLCLK(IDC_BCGBARRES_COMMANDS_LIST, OnAdd)
	ON_LBN_DBLCLK(IDC_BCGBARRES_QAT_COMMANDS_LIST, OnRemove)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_COMMANDS_LIST, OnSelchangeCommandsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCustomizePage message handlers

void CBCGPRibbonCustomizePage::OnSelendokCategoryCombo() 
{
	ASSERT_VALID (m_pRibbonBar);

	UpdateData ();

	DWORD_PTR dwData = m_wndCategoryCombo.GetItemData (m_nCategory);
	
	if (dwData == 0)	// Separator, get next
	{
		if (m_nCategory == m_wndCategoryCombo.GetCount () - 1)
		{
			return;
		}

		m_nCategory++;
		UpdateData (FALSE);
	}

	CBCGPRibbonCustomCategory* pCustCategory = DYNAMIC_DOWNCAST (
		CBCGPRibbonCustomCategory, (CObject*)dwData);
	if (pCustCategory != NULL)
	{
		ASSERT_VALID (pCustCategory);

		m_wndCommandsList.FillFromIDs (pCustCategory->m_lstIDs, FALSE);
		OnSelchangeCommandsList();
	}
	else
	{
		CBCGPRibbonCategory* pCategory = DYNAMIC_DOWNCAST (
			CBCGPRibbonCategory, (CObject*)dwData);
		if (pCategory != NULL)
		{
			ASSERT_VALID (pCategory);

			m_wndCommandsList.FillFromCategory (pCategory);
			OnSelchangeCommandsList();
		}
	}
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnAdd() 
{
	CBCGPBaseRibbonElement* pCmd = m_wndCommandsList.GetSelected ();
	if (pCmd == NULL)
	{
		return;
	}

	ASSERT_VALID (pCmd);
	
	if (!m_wndQATList.AddCommand (pCmd, TRUE, FALSE))
	{
		return;
	}

	int nSel = m_wndCommandsList.GetCurSel ();
	if (nSel < m_wndCommandsList.GetCount () - 1)
	{
		m_wndCommandsList.SetCurSel (nSel + 1);
	}

	OnSelchangeQATCommands();
	OnSelchangeCommandsList();
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnRemove() 
{
	int nIndex = m_wndQATList.GetCurSel ();
	if (nIndex >= 0)
	{
		m_wndQATList.DeleteString (nIndex);

		nIndex = min (nIndex, m_wndQATList.GetCount () - 1);

		if (nIndex >= 0)
		{
			m_wndQATList.SetCurSel (nIndex);
		}
	}

	OnSelchangeQATCommands();
	OnSelchangeCommandsList();
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnUp() 
{
	MoveItem (TRUE);
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnDown() 
{
	MoveItem (FALSE);
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnToolbarReset() 
{
	CString strPrompt;

	{
		CBCGPLocalResource locaRes;

		CString strCaption;
		strPrompt.Format (IDS_BCGBARRES_RESET_TOOLBAR_FMT, strCaption);
	}

	if (AfxMessageBox (strPrompt, MB_OKCANCEL | MB_ICONWARNING) != IDOK)
	{
		return;
	}

	CList<UINT,UINT> lstCmds;
	m_pRibbonBar->m_QAToolbar.GetDefaultCommands (lstCmds);

	m_wndQATList.FillFromIDs (lstCmds, FALSE);
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnSelchangeQATCommands() 
{
	m_wndUp.EnableWindow (m_wndQATList.GetCurSel () > 0);
	m_wndDown.EnableWindow (m_wndQATList.GetCurSel () < m_wndQATList.GetCount () - 1);
	m_wndRemove.EnableWindow (m_wndQATList.GetCurSel () >= 0);
}
//**********************************************************************
BOOL CBCGPRibbonCustomizePage::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	ASSERT_VALID (m_pRibbonBar);

	const CString strSeparator = _T("----------");

	if (DYNAMIC_DOWNCAST (CBCGPRibbonCustomize, GetParent ()) != NULL)
	{
		CString strTitle;
		GetWindowText (strTitle);

		GetParent ()->SetWindowText (strTitle);
	}

	m_wndUp.SetStdImage (CMenuImages::IdArowUpLarge);
	m_wndDown.SetStdImage (CMenuImages::IdArowDownLarge);

	//-----------------------
	// Add custom categories:
	//-----------------------
	for (POSITION pos = m_lstCustomCategories.GetHeadPosition (); pos != NULL;)
	{
		CBCGPRibbonCustomCategory* pCustCategory = m_lstCustomCategories.GetNext (pos);
		ASSERT_VALID (pCustCategory);

		int nIndex = m_wndCategoryCombo.AddString (pCustCategory->m_strName);
		m_wndCategoryCombo.SetItemData (nIndex, (DWORD_PTR) pCustCategory);
	}

	if (m_wndCategoryCombo.GetCount () > 0)
	{
		m_wndCategoryCombo.AddString (strSeparator);
	}

	//-------------------
	// Add main category:
	//-------------------
	CBCGPRibbonCategory* pMainCategory = m_pRibbonBar->GetMainCategory ();
	if (pMainCategory != NULL)
	{
		ASSERT_VALID (pMainCategory);
		
		int nIndex = m_wndCategoryCombo.AddString (pMainCategory->GetName ());
		m_wndCategoryCombo.SetItemData (nIndex, (DWORD_PTR) pMainCategory);
		m_wndCategoryCombo.AddString (strSeparator);
	}

	int i = 0;
	BOOL bHasContextCategories = FALSE;

	//----------------------------
	// Add non-context categories:
	//----------------------------
	for (i = 0; i < m_pRibbonBar->GetCategoryCount (); i++)
	{
		CBCGPRibbonCategory* pCategory = m_pRibbonBar->GetCategory (i);
		ASSERT_VALID (pCategory);

		if (pCategory->GetContextID () == 0)
		{
			int nIndex = m_wndCategoryCombo.AddString (pCategory->GetName ());
			m_wndCategoryCombo.SetItemData (nIndex, (DWORD_PTR) pCategory);
		}
		else
		{
			bHasContextCategories = TRUE;
		}
	}

	if (bHasContextCategories)
	{
		//------------------------
		// Add context categories:
		//------------------------
		m_wndCategoryCombo.AddString (strSeparator);

		for (i = 0; i < m_pRibbonBar->GetCategoryCount (); i++)
		{
			CBCGPRibbonCategory* pCategory = m_pRibbonBar->GetCategory (i);
			ASSERT_VALID (pCategory);

			const UINT uiContextID = pCategory->GetContextID ();

			if (uiContextID != 0)
			{
				CString strName;
				CString strContext;

				if (m_pRibbonBar->GetContextName (uiContextID, strContext))
				{
					strName = strContext + _T(" | ") + pCategory->GetName ();
				}
				else
				{
					strName = pCategory->GetName ();
				}

				int nIndex = m_wndCategoryCombo.AddString (strName);
				m_wndCategoryCombo.SetItemData (nIndex, (DWORD_PTR) pCategory);
			}
		}
	}

	if (m_wndCategoryCombo.GetCount () > 0)
	{
		m_nCategory = 0;
		UpdateData (FALSE);

		OnSelendokCategoryCombo ();
	}

	CList<UINT,UINT> lstQACommands;
	m_pRibbonBar->GetQuickAccessCommands (lstQACommands);

	m_wndQATList.FillFromIDs (lstQACommands, FALSE);

	OnSelchangeQATCommands ();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//**********************************************************************
void CBCGPRibbonCustomizePage::MoveItem (BOOL bMoveUp) 
{
	int nSel = m_wndQATList.GetCurSel ();

	CString str;
	m_wndQATList.GetText (nSel, str);

	DWORD_PTR dwData = m_wndQATList.GetItemData (nSel);

	m_wndQATList.DeleteString (nSel);

	int nNewIndex = bMoveUp ? nSel - 1 : nSel + 1;

	int nIndex = m_wndQATList.InsertString (nNewIndex, str);

	m_wndQATList.SetItemData (nIndex, dwData);

	m_wndQATList.SetCurSel (nIndex);
	OnSelchangeQATCommands();
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnOK() 
{
	UpdateData ();

	ASSERT_VALID (m_pRibbonBar);

	CList<UINT,UINT> lstQACommands;

	for (int i = 0; i < m_wndQATList.GetCount (); i++)
	{
		lstQACommands.AddTail (m_wndQATList.GetCommand (i)->GetID ());
	}

	m_pRibbonBar->m_QAToolbar.ReplaceCommands (lstQACommands);
	m_pRibbonBar->SetQuickAccessToolbarOnTop (!m_bQAToolbarOnBottom);

	m_pRibbonBar->RecalcLayout ();

	CFrameWnd* pParentFrame = m_pRibbonBar->GetParentFrame ();
	
	if (pParentFrame->GetSafeHwnd () != NULL)
	{
		pParentFrame->RecalcLayout ();
		pParentFrame->RedrawWindow ();
	}

	CBCGPPropertyPage::OnOK();
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnCustomizeKeyboard()
{
	ASSERT_VALID (m_pRibbonBar);

	CBCGPRibbonKeyboardCustomizeDlg dlg (m_pRibbonBar, this);
	dlg.DoModal ();
}
//**********************************************************************
void CBCGPRibbonCustomizePage::OnSelchangeCommandsList() 
{
	BOOL bEnableAddButton = TRUE;

	CBCGPBaseRibbonElement* pCmd = m_wndCommandsList.GetSelected ();
	if (pCmd == NULL)
	{
		bEnableAddButton = FALSE;
	}
	else
	{
		ASSERT_VALID (pCmd);
		bEnableAddButton = 
			pCmd->GetID () == 0 || m_wndQATList.GetCommandIndex (pCmd->GetID ()) < 0;
	}

	m_wndAdd.EnableWindow (bEnableAddButton);
}
//**********************************************************************
void CBCGPRibbonCustomizePage::AddCustomCategory (LPCTSTR lpszName,
												  const CList<UINT, UINT>& lstIDS)
{
	ASSERT (lpszName != NULL);
	ASSERT (GetSafeHwnd () == NULL);

	CBCGPRibbonCustomCategory* pCategory = new CBCGPRibbonCustomCategory;
	pCategory->m_strName = lpszName;

	pCategory->m_lstIDs.AddHead ((UINT)0);	// Separator
	pCategory->m_lstIDs.AddTail ((CList<UINT,UINT>*)&lstIDS);

	m_lstCustomCategories.AddTail (pCategory);
}

#endif // BCGP_EXCLUDE_RIBBON
