// imgdoc.cpp : implementation of the CImageDoc class
//

#include "stdafx.h"
#include "app.h"

#include "imgdoc.h"
#include "frame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
	//{{AFX_MSG_MAP(CImageDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateIsOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_STRETCH_MODE, OnUpdateStretchMode)
	ON_COMMAND(ID_TRANSFORM_ELLIPSE, OnTransformEllipse)
	ON_COMMAND(ID_WINDOW_DUPLICATE, OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_CIMAGE_FLIP, OnFlip)
	ON_COMMAND(ID_CIMAGE_MIRROR, OnMirror)
	ON_COMMAND(ID_CIMAGE_NEGATIVE, OnNegative)
	ON_COMMAND(ID_CIMAGE_GREYSCALE, OnGreyscale)
	ON_UPDATE_COMMAND_UI(ID_CIMAGE_FLIP, OnUpdateIsOpen)
	ON_UPDATE_COMMAND_UI(ID_CIMAGE_MIRROR, OnUpdateIsOpen)
	ON_UPDATE_COMMAND_UI(ID_CIMAGE_NEGATIVE, OnUpdateIsOpen)
	ON_UPDATE_COMMAND_UI(ID_CIMAGE_GREYSCALE, OnUpdateIsOpen)   
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_ELLIPSE, OnUpdateIsOpen)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_DUPLICATE, OnUpdateIsOpen)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_STRETCH_MODE, OnStretchMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CImageDoc::CImageDoc() : CDocument(), m_image(), m_stretchMode(FALSE), m_UndoLevel(0)
{
	for (int i=0;i<MAX_UNDO_LEVELS;i++)
   {
      imageUndo[i] = new CImageObject;
   }
}

CImageDoc::~CImageDoc()
{
	for (int i=0;i<MAX_UNDO_LEVELS;i++) delete imageUndo[i];
}

void CImageDoc::DeleteContents() 
{
   m_image.Close();
	CDocument::DeleteContents();
}

BOOL CImageDoc::OnNewDocument()
{
	BOOL bOK = CDocument::OnNewDocument();
   if (bOK)
   {
   }
	return bOK;
}

/////////////////////////////////////////////////////////////////////////////
// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageDoc commands

/////////////////////////////////////////////////////////////////////////////
// CImageDoc serialization

void CImageDoc::Serialize(CArchive& ar)
{
   m_image.Serialize(ar);
}

CFile* CImageDoc::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
	// The CImage library is implemented using regular C Runtime file I/O.
   // Thus we need to provide a FILE* handle.
   CStdioFile* pFile = new CStdioFile;
	ASSERT(pFile != NULL);
   if (!pFile->Open(lpszFileName, nOpenFlags | CFile::typeBinary, pError))
	{
		delete pFile; pFile = NULL;
	}
	return pFile;
}

BOOL CImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
   BOOL bOK = CDocument::OnOpenDocument(lpszPathName);
   if ((!bOK) && (0 != strlen(m_image.GetLastError())))
   {
	   AfxMessageBox(CString(m_image.GetLastError()));
   }
	return bOK;
}

BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   BOOL bOK = CDocument::OnSaveDocument(lpszPathName);
   if (!bOK)
   {
	   AfxMessageBox(CString(m_image.GetLastError()));
   }
	return bOK;
}

BOOL CImageDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
{
	if (!m_image.IsOpen())
		return FALSE;

	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty())
		bSaveAs = TRUE;
   else if (!CApp::GetApp().GetWritableType(m_image.GetFileType()))
		bSaveAs = TRUE;

	if (bSaveAs)
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			int iBad = newName.FindOneOf(_T(" #%;/\\"));    // dubious filename
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
         newName += CApp::GetApp().GetExtFromType(m_image.GetFileType());
		}

		int nDocType = m_image.GetFileType();
		if (!CApp::GetApp().PromptForFileName(newName, 
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &nDocType))
		{
			return FALSE;       // don't even try to save
		}
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			TRY 
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	EndWaitCursor();
	if (bReplace)
	{
		// Reset the title and change the document name
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName);       // must be set
	}
	else // SaveCopyAs
	{
		SetModifiedFlag(bModified);
	}
	return TRUE;        // success
}

#define EPSILON (0.0000001)

/*static*/int CImageDoc::ComputePixel(float x, float y, float &x1, float &y1)
{
  double r, nn;

  if (x==0 && y==0) {
	 x1 = x;
	 y1 = y;
	 return 1;
  }

  nn = sqrt(x*x + y*y);
  r =  (fabs(x) > fabs(y)) ? fabs(nn/x): fabs(nn/y);

  x1 = (float)(r*x);
  y1 = (float)(r*y);

  return 1;
}
/////////////////////////////////////////////////////////////////////////////
void CImageDoc::ComputeNewImage(void)
{
  CWaitCursor wait;

  const float xmid = (float) (m_image.GetWidth ()/2.0);
  const float ymid = (float) (m_image.GetHeight()/2.0);
  const float ar   = (float) (m_image.GetHeight())/(float)(m_image.GetWidth());
  
  DWORD x, y, x1, y1;
  float fx, fy;
  CImage image2(m_image);
  for (y = 0; y < m_image.GetHeight(); y++) 
  {
	 for (x=0; x < m_image.GetWidth(); x++)
    {
		ComputePixel(ar*(x-xmid), y-ymid, fx, fy);
		x1 = (int)(xmid+fx/ar);
		y1 = (int)(ymid+fy);

		if (m_image.IsInside(x1, y1))
		  image2.SetPixelColor(x, y, m_image.GetPixelColor(x1, y1));
		else
		  image2.SetPixelColor(x, y,RGB(255,255,255));
	 }
  }
  m_image.CImage::operator=(image2);
  UpdateAllViews(NULL);
}

void CImageDoc::OnUpdateIsOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_image.IsOpen());
}
/////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
//	pCmdUI->Enable((image != NULL)); //&& theApp.GetWritableType(image->GetFileType()));
}

void CImageDoc::OnUpdateStretchMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_stretchMode);
	pCmdUI->Enable(TRUE);
}

void CImageDoc::OnTransformEllipse() 
{
	SubmitUndo();
	ComputeNewImage();
	UpdateAllViews(NULL);
}

void CImageDoc::OnWindowDuplicate() 
{
	CImageDoc* pDoc=(CImageDoc*)CApp::GetApp().demoTemplate->OpenDocumentFile(NULL); 

	if (NULL != pDoc)
   {
      pDoc->m_image.Copy(m_image);
		CString s;
		s.Format(_T("Copy %d of %s"),CApp::GetApp().m_nDocCount++,GetTitle());
		pDoc->SetTitle(s);
		pDoc->UpdateAllViews(NULL, ENUM_hint_newimage);
	}
}

void CImageDoc::OnEditCopy() 
{
   HANDLE hDIB = GetImage().CopyToHandle(NULL);

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) 
   {
		if(::EmptyClipboard()) 
      {
			if (::SetClipboardData(CF_DIB,hDIB) == NULL ) 
         {
				AfxMessageBox( _T("Unable to set Clipboard data") );
			}
		}
      ::CloseClipboard();
	}
}

void CImageDoc::OnFlip() 
{
	if (m_image.IsOpen())
   {
		SubmitUndo();
	   if (GetImage().Flip())
      {
		   UpdateAllViews(NULL);
	   }
   }
}

void CImageDoc::OnMirror() 
{
	if (m_image.IsOpen())
   {
		SubmitUndo();
	   if (GetImage().Mirror())
      {
		   UpdateAllViews(NULL);
	   }
   }
}

void CImageDoc::OnNegative() 
{
	if (m_image.IsOpen())
   {
		SubmitUndo();
      if (GetImage().Negative())
      {
		   UpdateAllViews(NULL);
	   }
   }
}

void CImageDoc::OnGreyscale() 
{
	if (m_image.IsOpen())
   {
		SubmitUndo();
      if (GetImage().GreyScale())
      {
		   UpdateAllViews(NULL);
	   }
   }
}

void CImageDoc::UpdateStatusBar()
{
	if (m_image.IsOpen())
   {
		CString s;
		s.Format(_T("(%dx%dx%d)"),m_image.GetWidth(),m_image.GetHeight(),m_image.GetBPP());
		CStatusBar& statusBar = ((CFrame*)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
		statusBar.SetPaneText(2, s);
	}
}

void CImageDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UndoLevel>0);
   
   CString s;
	s.Format(_T("Undo (%d)\tCtrl+Z"),m_UndoLevel);
	pCmdUI->SetText(s);

/*
   static CString strFormat;
   if (( pCmdUI->m_pMenu != NULL) && (!strFormat.IsEmpty()))
   {
      pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, str, MF_BYCOMMAND);
   }
   if (!strFormat.IsEmpty())
   {
      CString s;
	   s.Format(strFormat,m_UndoLevel);
	   pCmdUI->SetText(s);
   }
*/	
}

void CImageDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_UndoLevel<(MAX_UNDO_LEVELS))&& (imageUndo[m_UndoLevel]->IsOpen()));
}

void CImageDoc::OnEditUndo() 
{
	m_UndoLevel--;
	CImageObject* tmp = new CImageObject;
   m_image.Detach(tmp);
	imageUndo[m_UndoLevel]->Detach(&m_image);
   delete imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;

	UpdateAllViews(NULL,ENUM_hint_newimage);
	UpdateStatusBar();
}

void CImageDoc::OnEditRedo() 
{
	CImageObject* tmp = new CImageObject;
   m_image.Detach(tmp);
	imageUndo[m_UndoLevel]->Detach(&m_image);
   delete imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;
	m_UndoLevel++;

	UpdateAllViews(NULL,ENUM_hint_newimage);
	UpdateStatusBar();
}

void CImageDoc::SubmitUndo()
{
	if (m_UndoLevel>=MAX_UNDO_LEVELS)
   {	// Max Undo reached
		CImageObject* tmp = imageUndo[0];			// discard the early undo
      tmp->Close();
		for(int i=1;i<MAX_UNDO_LEVELS;i++)
      {
			imageUndo[i-1]=imageUndo[i]; //shift the history
		}
		imageUndo[MAX_UNDO_LEVELS-1]=tmp;	// clear the undo slot
		m_UndoLevel=MAX_UNDO_LEVELS-1;  // reposition at last level
	}
	// we must clear the "redo" history when a new action is performed
	for (int i=m_UndoLevel;i<MAX_UNDO_LEVELS;i++)
   {
		if (imageUndo[i]->IsOpen())
      {
			imageUndo[i]->Close();
		}
	}
	// save the actual image in the undo history
	imageUndo[m_UndoLevel]->Copy(m_image);
	m_UndoLevel++;
}

void CImageDoc::SetTitle(LPCTSTR lpszTitle) 
{
   CString str;
   if (m_strPathName.IsEmpty())
   {
      str = lpszTitle;
   }
   else
   {
      str = m_strPathName;
   }
	CDocument::SetTitle(str);
}

void CImageDoc::OnStretchMode()
{
   m_stretchMode = !m_stretchMode;
	UpdateAllViews(NULL);
}

BOOL CImageDoc::Rotate(double angle)
{
	SubmitUndo();
	CWaitCursor wc;
	BOOL bOK = m_image.Rotate(angle);
   if (bOK)
   {
	   UpdateAllViews(NULL,ENUM_hint_newimage);
	   UpdateStatusBar();
   }
   return bOK;
}

