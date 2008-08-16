// ActiveAccessibilityDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ActiveAccessibilityDemo.h"

#include "MainFrm.h"
#include "ActiveAccessibilityDemoDoc.h"
#include "ActiveAccessibilityDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoApp

BEGIN_MESSAGE_MAP(CActiveAccessibilityDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CActiveAccessibilityDemoApp)
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
// CActiveAccessibilityDemoApp construction

CActiveAccessibilityDemoApp::CActiveAccessibilityDemoApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CActiveAccessibilityDemoApp object

CActiveAccessibilityDemoApp theApp;

void (*pDllRegister)();

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoApp initialization

BOOL CActiveAccessibilityDemoApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx (&osvi);

	if ((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) || (osvi.dwMajorVersion < 5))
	{
		AfxMessageBox (_T("This demo requires Windows 2000 or higher"));
		return FALSE;
	}

	AfxEnableControlContainer();

	globalData.EnableAccessibilitySupport ();
	if (!globalData.IsAccessibilitySupport ())
	{
		AfxMessageBox(_T("Unable to locate BCGPOleAcc.dll. To see this demo properly, you need to put BCGPOleAcc.dll to your path and register it."));
		return FALSE;
	}

	//Register BCGAccecibility COM library
	HINSTANCE m_hinstDLL = LoadLibrary (_T("BCGPOleAcc.dll"));
	if (m_hinstDLL != NULL)
	{
		(FARPROC &)pDllRegister = ::GetProcAddress (m_hinstDLL, _T("DllRegisterServer"));
		pDllRegister ();
	}

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
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CActiveAccessibilityDemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CActiveAccessibilityDemoView));
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

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoApp message handlers

int CActiveAccessibilityDemoApp::ExitInstance() 
{
	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}

// App command to run the dialog
void CActiveAccessibilityDemoApp::OnAppAbout()
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}


void CActiveAccessibilityDemoApp::PreLoadState ()
{

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}


/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoApp message handlers
