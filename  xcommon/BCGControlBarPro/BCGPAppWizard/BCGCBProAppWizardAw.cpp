// BCGCBProAppWizardaw.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBProAppWizard.h"
#include "BCGCBProAppWizardaw.h"
#include "chooser.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CBCGCBProAppWizardAppWiz::InitCustomAppWiz()
{
	// Create a new dialog chooser; CDialogChooser's constructor initializes
	//  its internal array with pointers to the steps.
	m_pChooser = new CDialogChooser;

	// At first, we don't know the total number of steps, since there are two
	//  possible "tracks" (MDI/SDI app and dialog-based app).
	SetNumberOfSteps(-1);

	// Inform AppWizard of the languages we support
	SetSupportedLanguages(_T("English [United States] (APPWZENU.DLL);0x40904b0"));

	m_Dictionary.SetAt (_T("MENUBAR"), _T("1"));
	m_Dictionary.SetAt (_T("TOOLBAR_CUSTOMIZATION"), _T("1"));
	m_Dictionary.SetAt (_T("KEYBOARD_CUSTOMIZATION"), _T("1"));
	m_Dictionary.SetAt (_T("CONTEXT_MENU_CUSTOMIZATION"), _T("1"));
	m_Dictionary.SetAt (_T("ALT_CUSTOMIZATION"), _T("1"));
	m_Dictionary.SetAt (_T("RU_MENUS"), _T("1"));
	m_Dictionary.SetAt (_T("OFFICE2003_LOOK"), _T("1"));
	m_Dictionary.SetAt (_T("TOOLBAR_256_COLORS"), _T("1"));
	m_Dictionary.SetAt (_T("WORKSPACEBAR"), _T("1"));
	m_Dictionary.SetAt (_T("OUTPUTBAR"), _T("1"));
	m_Dictionary.SetAt (_T("AUTO_HIDE"), _T("1"));
	m_Dictionary.SetAt (_T("VISUAL_MANAGER"), _T("1"));

	m_Dictionary.SetAt (_T("workspace_ifile"), _T("WorkspaceBar"));
	m_Dictionary.SetAt (_T("workspace_hfile"), _T("WorkspaceBar"));
	m_Dictionary.SetAt (_T("WORKSPACEBAR_CLASS"), _T("CWorkspaceBar"));

	m_Dictionary.SetAt (_T("workspace2_ifile"), _T("WorkspaceBar2"));
	m_Dictionary.SetAt (_T("workspace2_hfile"), _T("WorkspaceBar2"));
	m_Dictionary.SetAt (_T("WORKSPACEBAR_CLASS2"), _T("CWorkspaceBar2"));

	m_Dictionary.SetAt (_T("outputbar_ifile"), _T("OutputBar"));
	m_Dictionary.SetAt (_T("outputbar_hfile"), _T("OutputBar"));
	m_Dictionary.SetAt (_T("OUTPUTBAR_CLASS"), _T("COutputBar"));

	m_Dictionary.SetAt (_T("taskpane_ifile"), _T("TaskPane"));
	m_Dictionary.SetAt (_T("taskpane_hfile"), _T("TaskPane"));
	m_Dictionary.SetAt (_T("TASKPANE_CLASS"), _T("CTaskPane"));
}

// This is called just before the custom AppWizard is unloaded.
void CBCGCBProAppWizardAppWiz::ExitCustomAppWiz()
{
	// Deallocate memory used for the dialog chooser
	ASSERT(m_pChooser != NULL);
	delete m_pChooser;
	m_pChooser = NULL;

	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
//  or "Next" on one of the custom AppWizard's steps.
CAppWizStepDlg* CBCGCBProAppWizardAppWiz::Next(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Next(pDlg);
}

// This is called when the user clicks "Back" on one of the custom
//  AppWizard's steps.
CAppWizStepDlg* CBCGCBProAppWizardAppWiz::Back(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Back(pDlg);
}

void CBCGCBProAppWizardAppWiz::CustomizeProject(IBuildProject* pProject)
{
	// TODO: Add code here to customize the project.  If you don't wish
	//  to customize project, you may remove this virtual override.
	
	// This is called immediately after the default Debug and Release
	//  configurations have been created for each platform.  You may customize
	//  existing configurations on this project by using the methods
	//  of IBuildProject and IConfiguration such as AddToolSettings,
	//  RemoveToolSettings, and AddCustomBuildStep. These are documented in
	//  the Developer Studio object model documentation.

	// WARNING!!  IBuildProject and all interfaces you can get from it are OLE
	//  COM interfaces.  You must be careful to release all new interfaces
	//  you acquire.  In accordance with the standard rules of COM, you must
	//  NOT release pProject, unless you explicitly AddRef it, since pProject
	//  is passed as an "in" parameter to this function.  See the documentation
	//  on CCustomAppWiz::CustomizeProject for more information.
}


// Here we define one instance of the CBCGCBProAppWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global BCGCBProAppWizardaw.
CBCGCBProAppWizardAppWiz BCGCBProAppWizardaw;

void HideHelpButton (CAppWizStepDlg* pDlg)
{
	CWnd* pWnd = pDlg->GetParent ();
	if (pWnd == NULL)
	{
		return;
	}

	CWnd* pButtons [5];

	pButtons [0] = pWnd->GetDlgItem (0x01b9);	// back
	pButtons [1] = pWnd->GetDlgItem (0x0001);	// next
	pButtons [2] = pWnd->GetDlgItem (0x01b8);	// finish
	pButtons [3] = pWnd->GetDlgItem (0x0002);	// cancel
	pButtons [4] = pWnd->GetDlgItem (0xe146);	// help

	int i = 0;

	for (i = 0; i < 5; i++)
	{
		if (pButtons [i]->GetSafeHwnd () == NULL)
		{
			ASSERT (FALSE);
			return;
		}
	}

	if (!pButtons [4]->IsWindowVisible ())
	{
		return;
	}

	CRect rectHelp;
	pButtons [4]->GetWindowRect (rectHelp);

	CRect rectCancel;
	pButtons [3]->GetWindowRect (rectCancel);

	const int dx = rectHelp.left - rectCancel.left;

	pButtons [4]->ShowWindow (SW_HIDE);

	for (i = 0; i < 4; i++)
	{
		CRect rectBtn;
		pButtons [i]->GetWindowRect (rectBtn);

		pWnd->ScreenToClient (&rectBtn);

		rectBtn.OffsetRect (dx, 0);

		pButtons [i]->SetWindowPos (NULL, rectBtn.left, rectBtn.top, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
