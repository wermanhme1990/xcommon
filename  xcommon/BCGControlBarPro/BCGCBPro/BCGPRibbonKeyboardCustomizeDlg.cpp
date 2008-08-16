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
// BCGPRibbonKeyboardCustomizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPLocalResource.h"
#include "BCGPRibbonKeyboardCustomizeDlg.h"
#include "BCGPKeyHelper.h"
#include "BCGPToolBar.h"
#include "BCGPKeyboardManager.h"
#include "BCGPMultiDocTemplate.h"
#include "BCGPBaseRibbonElement.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonCategory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef BCGP_EXCLUDE_RIBBON

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonKeyboardCustomizeDlg dialog

CBCGPRibbonKeyboardCustomizeDlg::CBCGPRibbonKeyboardCustomizeDlg(
	CBCGPRibbonBar* pRibbonBar, CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CBCGPRibbonKeyboardCustomizeDlg::IDD, pParent),
	m_wndCommandsList (pRibbonBar, FALSE /* Don't include separator */)
{
	//{{AFX_DATA_INIT(CBCGPRibbonKeyboardCustomizeDlg)
	m_strDesrcription = _T("");
	m_strAssignedTo = _T("");
	//}}AFX_DATA_INIT

	ASSERT_VALID (pRibbonBar);

	m_pRibbonBar = pRibbonBar;
	m_pParentFrame = m_pRibbonBar->GetTopLevelFrame ();

	m_hAccelTable = NULL;
	m_lpAccel = NULL;
	m_nAccelSize = 0;
	m_pSelTemplate = NULL;
	m_pSelButton = NULL;
	m_pSelEntry = NULL;
	m_pLocaRes = NULL;
}
//*************************************************************************************
CBCGPRibbonKeyboardCustomizeDlg::~CBCGPRibbonKeyboardCustomizeDlg()
{
	if (m_lpAccel != NULL)
	{
		delete [] m_lpAccel;
	}
}
//*************************************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGPRibbonKeyboardCustomizeDlg)
	DDX_Control(pDX, IDC_BCGBARRES_ASSIGNED_TO_TITLE, m_wndAssignedToTitle);
	DDX_Control(pDX, IDC_BCGBARRES_NEW_SHORTCUT_KEY, m_wndNewKey);
	DDX_Control(pDX, IDC_BCGBARRES_VIEW_TYPE, m_wndViewTypeList);
	DDX_Control(pDX, IDC_BCGBARRES_VIEW_ICON, m_wndViewIcon);
	DDX_Control(pDX, IDC_BCGBARRES_REMOVE, m_wndRemoveButton);
	DDX_Control(pDX, IDC_BCGBARRES_CURRENT_KEYS_LIST, m_wndCurrentKeysList);
	DDX_Control(pDX, IDC_BCGBARRES_COMMANDS_LIST, m_wndCommandsList);
	DDX_Control(pDX, IDC_BCGBARRES_CATEGORY, m_wndCategoryList);
	DDX_Control(pDX, IDC_BCGBARRES_ASSIGN, m_wndAssignButton);
	DDX_Text(pDX, IDC_BCGBARRES_COMMAND_DESCRIPTION, m_strDesrcription);
	DDX_Text(pDX, IDC_BCGBARRES_ASSIGNED_TO, m_strAssignedTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBCGPRibbonKeyboardCustomizeDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CBCGPRibbonKeyboardCustomizeDlg)
	ON_BN_CLICKED(IDC_BCGBARRES_ASSIGN, OnAssign)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_CATEGORY, OnSelchangeCategory)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_COMMANDS_LIST, OnSelchangeCommandsList)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_CURRENT_KEYS_LIST, OnSelchangeCurrentKeysList)
	ON_BN_CLICKED(IDC_BCGBARRES_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_BCGBARRES_RESET_SHORTCUTS, OnResetAll)
	ON_CBN_SELCHANGE(IDC_BCGBARRES_VIEW_TYPE, OnSelchangeViewType)
	ON_EN_UPDATE(IDC_BCGBARRES_NEW_SHORTCUT_KEY, OnUpdateNewShortcutKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonKeyboardCustomizeDlg message handlers

INT_PTR CBCGPRibbonKeyboardCustomizeDlg::DoModal() 
{
	m_pLocaRes = new CBCGPLocalResource;
	return CBCGPDialog::DoModal();
}
//*************************************************************************************
BOOL CBCGPRibbonKeyboardCustomizeDlg::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();

	if (m_pLocaRes != NULL)
	{
		delete m_pLocaRes;
		m_pLocaRes = NULL;
	}

	ASSERT (g_pKeyboardManager != NULL);
	ASSERT_VALID (m_pRibbonBar);

	//---------------------------------
	// Initialize commands by category:
	//---------------------------------	
	CBCGPRibbonCategory* pMainCategory = m_pRibbonBar->GetMainCategory ();
	if (pMainCategory != NULL)
	{
		ASSERT_VALID (pMainCategory);
		m_wndCategoryList.AddString (pMainCategory->GetName ());
	}
	
	for (int i = 0; i < m_pRibbonBar->GetCategoryCount (); i++)
	{
		m_wndCategoryList.AddString (
			m_pRibbonBar->GetCategory (i)->GetName ());
	}

	if (m_wndCategoryList.GetCount () > 0)
	{
		m_wndCategoryList.SetCurSel (0);
		OnSelchangeCategory ();
	}

	//-------------------------------------------------------------------
	// Find all application document templates and fill menues combobox
	// by document template data:
	//------------------------------------------------------------------
	CDocManager* pDocManager = AfxGetApp ()->m_pDocManager;
	if (m_bAutoSet && pDocManager != NULL)
	{
		//---------------------------------------
		// Walk all templates in the application:
		//---------------------------------------
		for (POSITION pos = pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
		{
			CBCGPMultiDocTemplate* pTemplate = 
				(CBCGPMultiDocTemplate*) pDocManager->GetNextDocTemplate (pos);
			ASSERT_VALID (pTemplate);
			ASSERT_KINDOF (CDocTemplate, pTemplate);

			//-----------------------------------------------------
			// We are interessing CBCGPMultiDocTemplate objects with
			// the shared menu only....
			//-----------------------------------------------------
			if (!pTemplate->IsKindOf (RUNTIME_CLASS (CMultiDocTemplate)) ||
				pTemplate->m_hAccelTable == NULL)
			{
				continue;
			}

			//----------------------------------------------------
			// Maybe, the template with same ID is already exist?
			//----------------------------------------------------
			BOOL bIsAlreadyExist = FALSE;
			for (int i = 0; !bIsAlreadyExist && i < m_wndViewTypeList.GetCount (); i++)
			{
				CBCGPMultiDocTemplate* pListTemplate = 
					(CBCGPMultiDocTemplate*) m_wndViewTypeList.GetItemData (i);
				bIsAlreadyExist = pListTemplate != NULL &&
					pListTemplate->GetResId () == pTemplate->GetResId ();
			}

			if (!bIsAlreadyExist)
			{
				CString strName;
				pTemplate->GetDocString (strName, CDocTemplate::fileNewName);

				int iIndex = m_wndViewTypeList.AddString (strName);
				m_wndViewTypeList.SetItemData (iIndex, (DWORD_PTR) pTemplate);
			}
		}
	}

	//--------------------------
	// Add a default application:
	//--------------------------
	CFrameWnd* pWndMain = DYNAMIC_DOWNCAST (CFrameWnd, m_pParentFrame);
	if (pWndMain != NULL && pWndMain->m_hAccelTable != NULL)
	{
		CBCGPLocalResource locaRes;

		CString strName;
		strName.LoadString (IDS_BCGBARRES_DEFAULT_VIEW);

		int iIndex = m_wndViewTypeList.AddString (strName);
		m_wndViewTypeList.SetItemData (iIndex, (DWORD_PTR) NULL);

		m_wndViewTypeList.SetCurSel (iIndex);
		OnSelchangeViewType();
	}

	if (m_wndViewTypeList.GetCurSel () == CB_ERR)
	{
		m_wndViewTypeList.SetCurSel (0);
		OnSelchangeViewType();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnAssign()
{
	ASSERT (m_lpAccel != NULL);
	ASSERT (m_pSelButton != NULL);

	//-----------------------------------------------------------
	// Obtain a new acceleration entry from the keyboard control:
	//-----------------------------------------------------------
	ASSERT (m_wndNewKey.IsKeyDefined ());

	ACCEL* pAccel = (ACCEL*) m_wndNewKey.GetAccel ();
	ASSERT (pAccel != NULL);

	pAccel->cmd = (USHORT) m_pSelButton->GetID ();

	//----------------------------
	// Create a new entries array:
	//----------------------------
	LPACCEL lpAccelOld = m_lpAccel;

	m_lpAccel = new ACCEL [m_nAccelSize + 1];
	ASSERT (m_lpAccel != NULL);

	memcpy (m_lpAccel, lpAccelOld, sizeof (ACCEL) * m_nAccelSize);

	int listcount = m_wndCurrentKeysList.GetCount();
	for (int i = 0; i < m_nAccelSize; i ++)
	{
		for (int idx=0; idx<listcount; idx++)
		{
			if ( m_wndCurrentKeysList.GetItemData(idx) == (DWORD_PTR) &lpAccelOld [i] )
			{
				m_wndCurrentKeysList.SetItemData(idx, (DWORD_PTR) &m_lpAccel [i]);
				break;
			}
		}
	}

	m_lpAccel [m_nAccelSize ++] = *pAccel;

	delete [] lpAccelOld;

	g_pKeyboardManager->UpdateAcellTable (
		m_pSelTemplate, m_lpAccel, m_nAccelSize);

	AddKeyEntry (&m_lpAccel [m_nAccelSize - 1]);

	m_wndNewKey.ResetKey ();
	OnUpdateNewShortcutKey ();

	m_wndCommandsList.SetFocus ();
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnSelchangeCategory() 
{
	UpdateData ();

	int nIndex = m_wndCategoryList.GetCurSel ();

	CBCGPRibbonCategory* pCategory = NULL;

	if (m_pRibbonBar->GetMainCategory () != NULL)
	{
		nIndex--;

		if (nIndex < 0)
		{
			pCategory = m_pRibbonBar->GetMainCategory ();
		}
	}

	if (pCategory == NULL)
	{
		pCategory = m_pRibbonBar->GetCategory (nIndex);
	}

	m_wndCommandsList.FillFromCategory (pCategory);

	m_wndNewKey.EnableWindow (FALSE);

	if (m_wndCommandsList.GetCount () > 0)
	{
		m_wndCommandsList.SetCurSel (0);
		OnSelchangeCommandsList ();
	}
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnSelchangeCommandsList() 
{
	m_strDesrcription.Empty ();
	m_wndCurrentKeysList.ResetContent ();
	OnSelchangeCurrentKeysList ();

	int iIndex = m_wndCommandsList.GetCurSel ();
	if (iIndex == LB_ERR)
	{
		m_pSelButton = NULL;
		m_wndNewKey.EnableWindow (FALSE);

		UpdateData (FALSE);
		return;
	}

	//-------------------------
	// Set command description:
	//-------------------------
	m_pSelButton = (CBCGPBaseRibbonElement*) m_wndCommandsList.GetItemData (iIndex);
	ASSERT_VALID (m_pSelButton);

	CFrameWnd* pParent = GetParentFrame ();
	if (pParent != NULL && pParent->GetSafeHwnd () != NULL)
	{
		pParent->GetMessageString (m_pSelButton->GetID (),
					m_strDesrcription);
	}

	//--------------------------------------------
	// Fill keys associated with selected command:
	//--------------------------------------------
	if (m_lpAccel != NULL)
	{
		for (int i = 0; i < m_nAccelSize; i ++)
		{
			if (m_pSelButton->GetID () == m_lpAccel [i].cmd)
			{
				AddKeyEntry (&m_lpAccel [i]);
			}
		}
	}

	m_wndNewKey.EnableWindow ();
	UpdateData (FALSE);
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnSelchangeCurrentKeysList() 
{
	int iIndex = m_wndCurrentKeysList.GetCurSel ();
	if (iIndex == LB_ERR)
	{
		m_pSelEntry = NULL;
		m_wndRemoveButton.EnableWindow (FALSE);

		return;
	}
	
	m_pSelEntry = (LPACCEL) m_wndCurrentKeysList.GetItemData (iIndex);
	ASSERT (m_pSelEntry != NULL);

	m_wndRemoveButton.EnableWindow ();
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnRemove() 
{
	ASSERT (m_pSelEntry != NULL);
	ASSERT (m_lpAccel != NULL);

	//----------------------------
	// Create a new entries array:
	//----------------------------
	LPACCEL lpAccelOld = m_lpAccel;

	m_lpAccel = new ACCEL [m_nAccelSize - 1];
	ASSERT (m_lpAccel != NULL);

	int iNewIndex = 0;
	for (int i = 0; i < m_nAccelSize; i ++)
	{
		if (m_pSelEntry != &lpAccelOld [i])
		{
			m_lpAccel [iNewIndex ++] = lpAccelOld [i];

			int listcount = m_wndCurrentKeysList.GetCount();
			for (int idx=0; idx<listcount; idx++)
			{
				if ( m_wndCurrentKeysList.GetItemData(idx) == (DWORD_PTR) &lpAccelOld [i] )
				{
					m_wndCurrentKeysList.SetItemData(idx, (DWORD_PTR) &m_lpAccel [iNewIndex-1]);
					break;
				}
			}
		}
	}

	delete [] lpAccelOld;
	m_nAccelSize --;

	g_pKeyboardManager->UpdateAcellTable (
		m_pSelTemplate, m_lpAccel, m_nAccelSize);

	OnSelchangeCommandsList ();
	m_wndCommandsList.SetFocus ();
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnResetAll() 
{
	{
		CBCGPLocalResource locaRes;

		CString str;
		str.LoadString (IDS_BCGBARRES_RESET_KEYBOARD);

		if (AfxMessageBox (str, MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}

	g_pKeyboardManager->ResetAll ();
	
	//---------------------------------------------
	// Send notification to application main frame:
	//---------------------------------------------
	if (m_pParentFrame != NULL)
	{
		m_pParentFrame->SendMessage (BCGM_RESETKEYBOARD);
	}

	OnSelchangeViewType();
	OnSelchangeCommandsList ();
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnSelchangeViewType() 
{
	m_hAccelTable = NULL;
	m_pSelTemplate = NULL;

	if (m_lpAccel != NULL)
	{
		delete [] m_lpAccel;
		m_lpAccel = NULL;
	}

	int iIndex = m_wndViewTypeList.GetCurSel ();
	if (iIndex == CB_ERR)
	{
		m_wndViewIcon.SetIcon (NULL);
		return;
	}

	HICON hicon = NULL;

	CBCGPMultiDocTemplate* pTemplate = 
			(CBCGPMultiDocTemplate*) m_wndViewTypeList.GetItemData (iIndex);
	if (pTemplate != NULL)
	{
		ASSERT_VALID (pTemplate);

		hicon = AfxGetApp ()->LoadIcon (pTemplate->GetResId ());
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		CFrameWnd* pWndMain = DYNAMIC_DOWNCAST (CFrameWnd, m_pParentFrame);
		if (pWndMain != NULL)
		{
			hicon = (HICON)(LONG_PTR) GetClassLongPtr (*pWndMain, GCLP_HICON);
			m_hAccelTable = pWndMain->m_hAccelTable;
		}
	}

	if (hicon == NULL)
	{
		hicon = ::LoadIcon(NULL, IDI_APPLICATION);
	}

	m_wndViewIcon.SetIcon (hicon);

	ASSERT (m_hAccelTable != NULL);

	m_nAccelSize = ::CopyAcceleratorTable (m_hAccelTable, NULL, 0);

	m_lpAccel = new ACCEL [m_nAccelSize];
	ASSERT (m_lpAccel != NULL);

	::CopyAcceleratorTable (m_hAccelTable, m_lpAccel, m_nAccelSize);
	m_pSelTemplate = pTemplate;

	OnSelchangeCommandsList ();
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::AddKeyEntry (LPACCEL pEntry)
{
	ASSERT (pEntry != NULL);

	CBCGPKeyHelper helper (pEntry);
	
	CString str;
	helper.Format (str);

	int iIndex = m_wndCurrentKeysList.AddString (str);
	m_wndCurrentKeysList.SetItemData (iIndex, (DWORD_PTR) pEntry);
}
//******************************************************************
void CBCGPRibbonKeyboardCustomizeDlg::OnUpdateNewShortcutKey() 
{
	ASSERT_VALID (m_pRibbonBar);

	ACCEL* pAccel = (ACCEL*) m_wndNewKey.GetAccel ();
	ASSERT (pAccel != NULL);

	m_strAssignedTo.Empty ();
	m_wndAssignedToTitle.ShowWindow (SW_HIDE);
	m_wndAssignButton.EnableWindow (FALSE);

	if (m_wndNewKey.IsKeyDefined ())
	{
		ASSERT (m_lpAccel != NULL);

		BOOL bIsAlreadyDefined = FALSE;

		for (int i = 0; !bIsAlreadyDefined && i < m_nAccelSize; i ++)
		{
			const BYTE fRelFlags = FCONTROL | FALT | FSHIFT | FVIRTKEY;

			if (pAccel->key == m_lpAccel [i].key &&
				(pAccel->fVirt & fRelFlags) == (m_lpAccel [i].fVirt & fRelFlags))
			{
				CBCGPBaseRibbonElement* pCmd = m_pRibbonBar->FindByID (m_lpAccel [i].cmd, FALSE);
				if (pCmd != NULL)
				{
					m_strAssignedTo = pCmd->GetText ();

					if (m_strAssignedTo.IsEmpty ())
					{
						pCmd->UpdateTooltipInfo ();
						m_strAssignedTo = pCmd->GetToolTipText ();
					}
				}
				else
				{
					m_strAssignedTo = _T("????");

					CString strText;
					strText.LoadString (m_lpAccel [i].cmd);

					if (!strText.IsEmpty ())
					{
						AfxExtractSubString (m_strAssignedTo, strText, 1, '\n');
					}
				}

				bIsAlreadyDefined = TRUE;
			}
		}

		if (!bIsAlreadyDefined)
		{
			CBCGPLocalResource locaRes;
			m_strAssignedTo.LoadString (IDP_BCGBARRES_UNASSIGNED);
			
			m_wndAssignButton.EnableWindow ();
		}

		m_wndAssignedToTitle.ShowWindow (SW_SHOW);
	}

	UpdateData (FALSE);
}

#endif	// BCGP_EXCLUDE_RIBBON
