// demo.h : main header file for the DEMO application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See demo.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	CApp();

// Attributes
protected:
   static CApp m_app;

   int m_nFilterIndex;
public:
	int m_nDocCount;	//number of documents
	CMultiDocTemplate *demoTemplate;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Operations
public:
   static CApp& GetApp(void);

   BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	BOOL GetWritableType(int nDocType);

// Implementation

	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
   afx_msg void OnFileOpen() ;
	afx_msg void OnImageFromBitmap();
	afx_msg void OnLoadJpegResource();
	afx_msg void OnLoadIconResource();
	afx_msg void OnLoadGifResource();
	afx_msg void OnLoadPngResource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   friend class CFrame;
};

struct DocType
{
public:
	BOOL bRead;
	BOOL bWrite;
	LPCTSTR description;
	LPCTSTR ext;
};

/////////////////////////////////////////////////////////////////////////////
