// FileDialogDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FileDialogDemo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "FileDialogDemoDoc.h"
#include "FileDialogDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp

BEGIN_MESSAGE_MAP(CFileDialogDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CFileDialogDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp construction

CFileDialogDemoApp::CFileDialogDemoApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFileDialogDemoApp object

CFileDialogDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp initialization

BOOL CFileDialogDemoApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Samples"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_FILEDITYPE,
		RUNTIME_CLASS(CFileDialogDemoDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFileDialogDemoView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//----Exmaple----//
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		CString strCaption;
		strCaption.LoadString (AFX_IDS_APP_TITLE);

		CBCGPFileDialog dlg (strCaption, TRUE);

		CImageList newItemImages;
		newItemImages.Create (32, 32, ILC_MASK, 0, 1);
		newItemImages.Add (LoadIcon (IDI_ICON_DOC));
		newItemImages.Add (LoadIcon (IDI_ICON_TEXT));
		newItemImages.Add (LoadIcon (IDI_ICON_WRITE));

		dlg.SetNewImagesList (&newItemImages);

		CBitmap bmpLogo;
		bmpLogo.LoadBitmap (IDB_LOGO);
		dlg.SetLogoBitmap (&bmpLogo);

		CString strItem;

		strItem.LoadString (IDS_WINWORD6_DOC);
		dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 0);

		strItem.LoadString (IDS_TEXT_DOC);
		dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 1);

		strItem.LoadString (IDS_WRITE_DOC);
		dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 2);

		HICON hiconOpenDlg = LoadIcon (IDR_MAINFRAME);
		dlg.SetDlgIcon (hiconOpenDlg);

		BOOL bOk = (dlg.DoModal () == IDOK);

		::DestroyIcon (hiconOpenDlg);

		if (bOk)
		{
			switch (dlg.GetPage ())
			{
			case CBCGPFileDialog::BCGFileNew:
				{
					pDocTemplate->OpenDocumentFile(NULL);
					if (m_pMainWnd == NULL)
					{
						return FALSE;
					}
				}
				break;

			case CBCGPFileDialog::BCGFileOpen:
				if (!OpenDocumentFile (dlg.GetPathName ()))
				{
					return FALSE;
				}
				break;

			case CBCGPFileDialog::BCGFileRecent:
				if (!OpenDocumentFile (dlg.GetRecentFilePath ()))
				{
					return FALSE;
				}
				break;

			default:
				ASSERT (FALSE);
				return FALSE;
			}
		}
	}
	else if (!ProcessShellCommand(cmdInfo))
	{
			return FALSE;
	}

	//----Exmaple----//

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp message handlers

int CFileDialogDemoApp::ExitInstance() 
{
	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}

void CFileDialogDemoApp::OnAppAbout()
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}


void CFileDialogDemoApp::PreLoadState ()
{

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}


/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp message handlers

void CFileDialogDemoApp::OnFileOpen() 
{
	// prompt the user (with all document templates)
	CString newName;
	if (!PromptForFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE))
		return; // open cancelled

	OpenDocumentFile(newName);
}

//----Exmaple----//

void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL CFileDialogDemoApp::PromptForFileName(CString& fileName, UINT nIDSTitle,
	DWORD dwFlags, BOOL bOpenFileDialog)
{
	CString title;

	VERIFY(title.LoadString(nIDSTitle));
	CBCGPFileDialog dlgFile (title, FALSE);

	dlgFile.m_ofn.Flags |= dwFlags;

	CString strFilter;
	CString strDefault;

	// do for all doc template
	BOOL bFirst = TRUE;
	for (POSITION pos = m_pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
	{
		CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate (pos);
		ASSERT_VALID (pTemplate);

		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
			bFirst ? &strDefault : NULL);
		bFirst = FALSE;
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = (int) dlgFile.DoModal();
	fileName.ReleaseBuffer();

	if (nResult != IDOK)
	{
		return FALSE;
	}

	switch (dlgFile.GetPage ())
	{
	case CBCGPFileDialog::BCGFileOpen:
		fileName = dlgFile.GetPathName ();
		break;

	case CBCGPFileDialog::BCGFileRecent:
		fileName = dlgFile.GetRecentFilePath ();
		break;

	default:
		ASSERT (FALSE);
	}

	return TRUE;
}

//----Exmaple----//
