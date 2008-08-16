#if !defined(AFX_BCGCBPROAPPWIZARDAW_H__D2612C5F_6106_4A7E_8855_2201E8244247__INCLUDED_)
#define AFX_BCGCBPROAPPWIZARDAW_H__D2612C5F_6106_4A7E_8855_2201E8244247__INCLUDED_

// BCGCBProAppWizardaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see BCGCBProAppWizard.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CBCGCBProAppWizardAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
	virtual CAppWizStepDlg* Back(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);

protected:
	CDialogChooser* m_pChooser;
};

void HideHelpButton (CAppWizStepDlg* pDlg);

// This declares the one instance of the CBCGCBProAppWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global BCGCBProAppWizardaw.  (Its definition is in BCGCBProAppWizardaw.cpp.)
extern CBCGCBProAppWizardAppWiz BCGCBProAppWizardaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCBPROAPPWIZARDAW_H__D2612C5F_6106_4A7E_8855_2201E8244247__INCLUDED_)
