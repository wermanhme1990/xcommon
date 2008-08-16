// SideBarDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SideBarDemo.h"
#include "SideBarDemoDlg.h"
#include "SidebarWnd.h"
#include "CustomVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoApp

BEGIN_MESSAGE_MAP(CSideBarDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CSideBarDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoApp construction

CSideBarDemoApp::CSideBarDemoApp()
{
	m_nLook = 0;			// Custom look
	m_bShowOptionsOnStartup = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSideBarDemoApp object

CSideBarDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoApp initialization

BOOL CSideBarDemoApp::InitInstance()
{
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

	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Samples"));
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	SetRegistryBase (_T("Settings"));

	m_bShowOptionsOnStartup = GetInt ("ShowOptionsOnStartup", TRUE);

	SetLook (GetInt ("Look", 0));

	// Create side bar:
	m_pwndAppBar = new CSidebarWnd ();
	ASSERT_VALID (m_pwndAppBar);

	//----------------------------
	// Create a new appbar window:
	//----------------------------
	CRect rect (0, 0, 200, 400);
	if (!m_pwndAppBar->Create (NULL, rect, m_pMainWnd))
	{
		TRACE0 ("Can't create the appbar.\n");
	}

	m_pMainWnd = m_pwndAppBar;

	if (!m_pwndAppBar->LoadState (NULL))
	{
		m_pwndAppBar->SetSide (ABE_RIGHT); // default state
	}

	m_pwndAppBar->ShowWindow (SW_SHOW);
	m_pwndAppBar->UpdateWindow ();

	if (m_bShowOptionsOnStartup)
	{
		CSideBarDemoDlg dlg (m_pwndAppBar);
		dlg.DoModal();
	}

	return TRUE;
}

int CSideBarDemoApp::ExitInstance() 
{
	WriteInt ("Look", m_nLook);
	WriteInt ("ShowOptionsOnStartup", m_bShowOptionsOnStartup);

	m_pwndAppBar->SaveState (NULL);

	BCGCBProCleanUp();
	m_pwndAppBar->DestroyWindow ();
	delete m_pwndAppBar;

	return CWinApp::ExitInstance();
}

void CSideBarDemoApp::SetLook (int nLook)
{
	m_nLook = nLook;

	switch (m_nLook)
	{
	case 0:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CCustomVisualManager));
		break;

	case 1:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		break;

	case 2:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;
	}

	if (m_pwndAppBar != NULL)
	{
		m_pwndAppBar->OnChangeLook ();
	}
}
