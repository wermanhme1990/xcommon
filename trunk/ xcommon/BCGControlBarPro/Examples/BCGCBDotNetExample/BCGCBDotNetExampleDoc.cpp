// BCGCBDotNetExampleDoc.cpp : implementation of the CBCGCBDotNetExampleDoc class
//

#include "stdafx.h"
#include "BCGCBDotNetExample.h"

#include "BCGCBDotNetExampleDoc.h"
#include "BCGCBDotNetExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDoc

IMPLEMENT_DYNCREATE(CBCGCBDotNetExampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CBCGCBDotNetExampleDoc, CDocument)
	//{{AFX_MSG_MAP(CBCGCBDotNetExampleDoc)
	ON_COMMAND(ID_DUMMY_BUILD, OnDummyBuild)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_DUMMY_EXECUTE, OnDummyExecute)
	ON_COMMAND(ID_DUMMY_GO, OnDummyGo)
	ON_COMMAND(ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, OnDummySelectActiveConfiguration)
	ON_COMMAND(ID_DUMMY_CLEAN, OnDummyClean)
	ON_COMMAND(ID_DUMMY_REBUILD_ALL, OnDummyRebuildAll)
	ON_COMMAND(ID_TOOLS_MACRO, OnToolsMacro)
	ON_COMMAND(ID_NEW_DIALOG, OnNewDialog)
	ON_COMMAND(ID_NEW_ACCELERATOR, OnNewAccelerator)
	ON_COMMAND(ID_NEW_BITMAP, OnNewBitmap)
	ON_COMMAND(ID_NEW_CURSOR, OnNewCursor)
	ON_COMMAND(ID_NEW_ICON, OnNewIcon)
	ON_COMMAND(ID_NEW_MENU, OnNewMenu)
	ON_COMMAND(ID_NEW_STRING_TABLE, OnNewStringTable)
	ON_COMMAND(ID_NEW_TOOLBAR, OnNewToolbar)
	ON_COMMAND(ID_NEW_VERSION, OnNewVersion)
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, OnDummySelectActiveConfiguration)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDoc construction/destruction

CBCGCBDotNetExampleDoc::CBCGCBDotNetExampleDoc()
{
	m_bTitleMark = FALSE;
	m_uiExampleID = 0;
	m_uiExampleXMLSettingsID = 0;
}

CBCGCBDotNetExampleDoc::~CBCGCBDotNetExampleDoc()
{
}

BOOL CBCGCBDotNetExampleDoc::OnNewDocument() 
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	CommonInit ();
	return TRUE;
}

BOOL CBCGCBDotNetExampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CommonInit ();
	return TRUE;
}

void CBCGCBDotNetExampleDoc::CommonInit ()
{
	if (GetDocTemplate () == theApp.m_pDocTemplateCpp)
	{
		m_uiExampleID = ID_EXAMPLE_CPP;
		m_uiExampleXMLSettingsID = IDR_EDITOR_XML_SETTINGS_CPP;
		return;
	}

	if (GetDocTemplate () == theApp.m_pDocTemplateWeb)
	{
		m_uiExampleID = ID_EXAMPLE_WEB;
		m_uiExampleXMLSettingsID = IDR_EDITOR_XML_SETTINGS_WEB;
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDoc serialization

void CBCGCBDotNetExampleDoc::Serialize(CArchive& ar)
{
	for (POSITION pos = GetFirstViewPosition (); pos != NULL;)
	{
		CBCGCBDotNetExampleView* pView = DYNAMIC_DOWNCAST (CBCGCBDotNetExampleView,
			GetNextView (pos));

		if (pView != NULL)
		{
			pView->SerializeRaw(ar);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDoc diagnostics

#ifdef _DEBUG
void CBCGCBDotNetExampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBCGCBDotNetExampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDoc commands

void CBCGCBDotNetExampleDoc::OnDummyBuild() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnDummyCompile() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnDummyExecute() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnDummyGo() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnDummySelectActiveConfiguration() 
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL && 
				CBCGPToolBar::IsLastCommandFromButton (pCombo))
			{
				pSrcCombo = pCombo;
			}
		}
	}

	if (pSrcCombo != NULL)
	{
		ASSERT_VALID (pSrcCombo);

		LPCTSTR lpszSelItem = pSrcCombo->GetItem ();
		CString strSelItem = (lpszSelItem == NULL) ? _T("") : lpszSelItem;

		AfxMessageBox (strSelItem);
	}
	else
	{
		AfxMessageBox (_T("Show \"Set Active Configuration\" dialog...."));
	}
}

void CBCGCBDotNetExampleDoc::OnDummyClean() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnDummyRebuildAll() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnToolsMacro() 
{
	// TODO: Add your command handler code here
	
}

void CBCGCBDotNetExampleDoc::OnNewDialog() 
{
   AfxMessageBox(_T("New Dialog"));
}

void CBCGCBDotNetExampleDoc::OnNewAccelerator() 
{
	AfxMessageBox(_T("New Accelerator"));
}

void CBCGCBDotNetExampleDoc::OnNewBitmap() 
{
	AfxMessageBox(_T("New Bitmap"));
}

void CBCGCBDotNetExampleDoc::OnNewCursor() 
{
	AfxMessageBox(_T("New Cursor"));
}

void CBCGCBDotNetExampleDoc::OnNewIcon() 
{
	AfxMessageBox(_T("New Icon"));
}

void CBCGCBDotNetExampleDoc::OnNewMenu() 
{
	AfxMessageBox(_T("New Menu"));
}

void CBCGCBDotNetExampleDoc::OnNewStringTable() 
{
	AfxMessageBox(_T("New StringTable"));
}

void CBCGCBDotNetExampleDoc::OnNewToolbar() 
{
   AfxMessageBox(_T("New Toolbar"));
}

void CBCGCBDotNetExampleDoc::OnNewVersion() 
{
	AfxMessageBox(_T("New Version"));
}

void CBCGCBDotNetExampleDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strTitle(lpszTitle);
	
	if (IsModified())
	{
		if (!m_bTitleMark)
		{
			m_bTitleMark = TRUE;
			strTitle += _T(" *");
		}
	}
	else
	{
		if (m_bTitleMark)
		{
			strTitle.ReleaseBuffer(strTitle.GetLength() - 2);
			m_bTitleMark = FALSE;
		}
	}
	
	CDocument::SetTitle(strTitle.GetBuffer(0));
}

void CBCGCBDotNetExampleDoc::CheckTitle() 
{
	if (IsModified() ^ m_bTitleMark)
	{
		SetTitle(GetTitle());
	}	
}

