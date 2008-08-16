// StateToFile.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "StateToFile.h"

#include "MainFrm.h"
#include "StateToFileDoc.h"
#include "StateToFileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp

BEGIN_MESSAGE_MAP(CStateToFileApp, CWinApp)
	//{{AFX_MSG_MAP(CStateToFileApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp construction

CStateToFileApp::CStateToFileApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CStateToFileApp object

CStateToFileApp theApp;

CString CStateToFileApp::g_strRegistryBase = _T ("Settings");

/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp initialization

BOOL CStateToFileApp::InitInstance()
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

	SetRegistryBase (g_strRegistryBase);

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStateToFileDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CStateToFileView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	LoadCustomState ();

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp message handlers

int CStateToFileApp::ExitInstance() 
{
	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}

void CStateToFileApp::ImportExportRegFile (LPCTSTR lpszCommandLine)
{
	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;
	memset(&siStartInfo, 0, sizeof(siStartInfo));
	memset(&piProcInfo, 0, sizeof(piProcInfo));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow = (WORD)SW_SHOW;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW;


	// export the selected file into registry using command line
	if (CreateProcess (NULL, (LPTSTR) lpszCommandLine, 
							NULL, NULL, FALSE, 0, 
							NULL, NULL, &siStartInfo, &piProcInfo))
	{
		WaitForSingleObject (piProcInfo.hProcess, 5000); // don't lock up if something happens with regedit
	}
}

void CStateToFileApp::OnAppAbout()
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}


void CStateToFileApp::PreLoadState ()
{

	GetMouseManager()->AddView (IDR_MAINFRAME, _T("My view"), IDR_MAINFRAME);

	// TODO: add another views and windows were mouse double click
	// customization is required

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}


/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp message handlers
