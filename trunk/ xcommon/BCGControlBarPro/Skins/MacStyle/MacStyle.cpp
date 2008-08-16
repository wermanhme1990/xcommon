// MacStyle.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MacStyle.h"
#include "MacStyleSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMacStyleApp

BEGIN_MESSAGE_MAP(CMacStyleApp, CWinApp)
	//{{AFX_MSG_MAP(CMacStyleApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacStyleApp construction

CMacStyleApp::CMacStyleApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMacStyleApp object

CMacStyleApp theApp;

CRuntimeClass* __stdcall BCGCBProGetSkinClass (int iIndex)
{
	switch (iIndex)
	{
	case 0:
		return RUNTIME_CLASS (CMacStyleSkin);

	// TODO: return other skin classes here
	
	}

	return NULL;
}
//*************************************************************************************
int __stdcall BCGCBProGetSkinCount ()
{
	return 1;
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBProGetSkinName (int iIndex)
{
	switch (iIndex)
	{
	case 0:
		return _T("Mac Style Skin");

	// TODO: return other skin names here
	
	}

	return NULL;
}
//*************************************************************************************
BOOL __stdcall BCGCBProGetSkinVersion(int& iVersionMajor,int& iVersionMinor)
{
	iVersionMajor = _BCGCBPRO_VERSION_MAJOR;
	iVersionMinor = _BCGCBPRO_VERSION_MINOR;

	return TRUE;
}
//*************************************************************************************
BOOL __stdcall BCGCBProIsUNICODE()
{
#ifdef _UNICODE
	return TRUE;
#else
	return FALSE;
#endif
}
//*************************************************************************************
BOOL __stdcall BCGCBProPreviewSkin (CDC* pDC, int iIndex, CRect rectPreview)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState ())

	switch (iIndex)
	{
	case 0:
		{
			CBitmap bmpPreview;
			bmpPreview.LoadBitmap (IDB_MACSTYLESKIN_PREVIEW);

			pDC->DrawState (rectPreview.TopLeft (), rectPreview.Size (), 
							&bmpPreview, DSS_NORMAL);
		}
		return TRUE;

	// TODO: preview other skins here
	
	}

	return FALSE;
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBProGetSkinAuthor (int /*iIndex*/)
{
	return _T("BCGSoft, BCGSoft");
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBProGetSkinAuthorURL (int /*iIndex*/)
{
	return _T("http://www.bcgsoft.com");
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBProGetSkinAuthorMail (int /*iIndex*/)
{
	return _T("skins@bcgsoft.com");
}
//*************************************************************************************
int __stdcall BCGCBProGetSkinUpdate ()
{
	return 1;
}
