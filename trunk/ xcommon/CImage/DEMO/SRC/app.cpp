// app.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "app.h"

#include "frame.h"
#include "imgframe.h"
#include "imgdoc.h"
#include "imgview.h"
#include "imgview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static const DocType MOD_doctypes[] =
{
	{ TRUE, TRUE , _T("BMP files" ), _T("bmp") },
#ifdef _DEBUG
   { TRUE, TRUE , _T("GIF files" ), _T("gif") },
#else
   { TRUE, FALSE, _T("GIF files" ), _T("gif") },
#endif
	{ TRUE, TRUE , _T("JPEG files"), _T("jpg") },
	{ TRUE, TRUE , _T("PNG files" ), _T("png") },
	{ TRUE, TRUE , _T("ICO files" ), _T("ico") },
	{ TRUE, TRUE , _T("TIF files" ), _T("tif") }
};
C_ASSERT(ENUM_cimage_format_enumcount == _countof(MOD_doctypes));

/////////////////////////////////////////////////////////////////////////////
// CApp
BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDR_IMAGE_BMP, OnImageFromBitmap)
	ON_COMMAND(IDR_IMAGE_JPEG, OnLoadJpegResource)
	ON_COMMAND(IDR_IMAGE_ICON, OnLoadIconResource)
	ON_COMMAND(IDR_IMAGE_GIF, OnLoadGifResource)
	ON_COMMAND(IDR_IMAGE_PNG, OnLoadPngResource)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction
CApp::CApp()
{
	m_nFilterIndex = ENUM_cimage_format_jpeg;	//default file extension
	m_nDocCount=1;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

/*static*/ CApp  CApp::m_app;
/*static*/ CApp& CApp::GetApp(void)
{ 
   return m_app; 
}

/////////////////////////////////////////////////////////////////////////////
// CApp initialization
BOOL CApp::InitInstance()
{
   SetRegistryKey(_T("troelsk"));
	// Standard initialization
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(9);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_IMAGE_TYPE,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CImageFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageView));
	demoTemplate = pDocTemplate;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CFrame* pMainFrame = new CFrame;
	if (!pMainFrame->LoadFrame(CFrame::IDR))
   {
		delete pMainFrame;
      return FALSE;
   }
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
    // Alter behaviour to not open window immediately. NO!
   //cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);
   
   if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) 
      cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable open from command line
	//if (m_lpCmdLine != "") OpenDocumentFile(m_lpCmdLine); NO!
   //m_pMainWnd->PostMessage(WM_COMMAND, IDR_IMAGE_GIF);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// App command to run the dialog
void CApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX, m_pMainWnd).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
void CApp::OnFileOpen() 
{
	// prompt the user (with all document templates)
	CString newName;
	int nType = ENUM_cimage_format_bmp;
	if (!PromptForFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, &nType))
		return; // open cancelled
	OpenDocumentFile(newName);
}
/////////////////////////////////////////////////////////////////////////////
// prompt for file name - used for open and save as
// static function called from app
BOOL CApp::PromptForFileName(CString& fileName, UINT nIDSTitle, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title(_T("Open bitmap file"));

	dlgFile.m_ofn.Flags |= dwFlags;
//	dlgFile.m_ofn.Flags &= ~OFN_SHOWHELP;

	int nIndex = m_nFilterIndex;
	if (!bOpenFileDialog){
		int nDocType = (pType != NULL) ? *pType : ENUM_cimage_format_bmp;
		nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
		if (nIndex == -1)
			nIndex = 0;
		ASSERT(nIndex != -1);
		nIndex++;
	}
	dlgFile.m_ofn.nFilterIndex = nIndex + 1;
	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(GetTypeFromIndex(nIndex, bOpenFileDialog));
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		

	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet){
		if (bOpenFileDialog)
			m_nFilterIndex = dlgFile.m_ofn.nFilterIndex - 1;
		if (pType != NULL){
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
		}
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
int CApp::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<_countof(MOD_doctypes);i++){
		if (bOpenFileDialog ? MOD_doctypes[i].bRead : MOD_doctypes[i].bWrite)
      {
			if (i == nDocType)
				return nCnt;
			nCnt++;
		}
	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////
int CApp::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	return bOpenFileDialog ? MOD_doctypes[nIndex].bRead : MOD_doctypes[nIndex].bWrite;
}

CString CApp::GetExtFromType(int nDocType)
{
	return MOD_doctypes[nDocType].ext;
}

BOOL CApp::GetWritableType(int nDocType)
{
	return MOD_doctypes[nDocType].bWrite;
}

CString CApp::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<_countof(MOD_doctypes);i++){
		if (bOpenFileDialog && MOD_doctypes[i].bRead){
			str += MOD_doctypes[i].description;
			str += (TCHAR)NULL;
            str += _T("*");
			str += MOD_doctypes[i].ext;
			str += (TCHAR)NULL;
		} else if (!bOpenFileDialog && MOD_doctypes[i].bWrite) {
			str += MOD_doctypes[i].description;
			str += (TCHAR)NULL;
            str += _T("*");
			str += MOD_doctypes[i].ext;
			str += (TCHAR)NULL;
		}
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CApp commands

void CApp::OnImageFromBitmap() 
{
	HBITMAP handle = ::LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_BMP));
	if (handle == NULL)
   {
		AfxMessageBox(_T("Could not load handle from resource"));
		return;
	}

	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		CImage newImage(handle);
		if (!newImage.IsOpen()) 
      {
			AfxMessageBox(_T("Could not create CImage"));
			return;
		}
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("BITMAP Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}

void CApp::OnLoadIconResource() 
{
	CImage newImage;
	if (!newImage.LoadResource(FindResource(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_ICON),_T("IMAGES")),ENUM_cimage_format_icon))
   {
		AfxMessageBox(CString(newImage.GetLastError()));
		return;
	}
	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("ICON Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}

/*
void CApp::OnLoadIconResource() 
{
	HICON handle = ::LoadIcon(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_ICON));
	if (handle == NULL)
   {
		AfxMessageBox(_T("Could not load handle from resource"));
		return;
	}

	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		CImage newImage(handle);
		if (!newImage.IsOpen()) 
      {
			AfxMessageBox(_T("Could not create CImage"));
			return;
		}
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("ICON Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}
*/

/*
void CApp::OnLoadIconResource() 
{
	CImage newImage;
	if (!newImage.LoadResource(FindResource(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_ICON),RT_GROUP_ICON),ENUM_cimage_format_ICO))
   {
		AfxMessageBox(CString(newImage.GetLastError()));
		return;
	}
	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("Icon Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}
*/
void CApp::OnLoadJpegResource() 
{
	CImage newImage;
	if (!newImage.LoadResource(FindResource(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_JPEG),_T("IMAGES")),ENUM_cimage_format_jpeg))
   {
		AfxMessageBox(CString(newImage.GetLastError()));
		return;
	}
	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("JPEG Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}

void CApp::OnLoadGifResource() 
{
	CImage newImage;
	if (!newImage.LoadResource(FindResource(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_GIF),_T("IMAGES")),ENUM_cimage_format_gif))
   {
		AfxMessageBox(CString(newImage.GetLastError()));
		return;
	}
	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("GIF Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(NULL);
		doc->UpdateStatusBar();
	}
}

void CApp::OnLoadPngResource() 
{
	CImage newImage;
	if (!newImage.LoadResource(FindResource(m_hInstance,MAKEINTRESOURCE(IDR_IMAGE_PNG),_T("IMAGES")),ENUM_cimage_format_png))
   {
		AfxMessageBox(CString(newImage.GetLastError()));
		return;
	}
	CImageDoc *doc = (CImageDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc)
   {
		newImage.Detach(&doc->m_image);

		CString str;
		str.Format(_T("PNG Resource Image %d"),m_nDocCount++);
		doc->SetTitle(str);
		doc->UpdateAllViews(0);
		doc->UpdateStatusBar();
	}
}

/////////////////////////////////////////////////////////////////////////////
