// BCGPIEDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BCGPIEDemo.h"

#include "MainFrm.h"
#include "BCGPIEDemoDoc.h"
#include "BCGPIEDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoApp

BEGIN_MESSAGE_MAP(CBCGPIEDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CBCGPIEDemoApp)
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
// CBCGPIEDemoApp construction

CBCGPIEDemoApp::CBCGPIEDemoApp()
{
	m_bBackgroundImage = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBCGPIEDemoApp object

CBCGPIEDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoApp initialization

BOOL CBCGPIEDemoApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
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
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Examples"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not present yet
	InitKeyboardManager();

	// Initialize favorites:
	m_Favorites.Load ();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBCGPIEDemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBCGPIEDemoView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

void CBCGPIEDemoApp::LoadCustomState ()
{
	m_bBackgroundImage = GetInt (_T("BackgroundImage"), FALSE);
}

void CBCGPIEDemoApp::SaveCustomState ()
{
	WriteInt (_T("BackgroundImage"), m_bBackgroundImage);
}

// App command to run the dialog
void CBCGPIEDemoApp::OnAppAbout()
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}

int CBCGPIEDemoApp::ExitInstance() 
{
	BCGCBProCleanUp ();
	return CWinApp::ExitInstance();
}
