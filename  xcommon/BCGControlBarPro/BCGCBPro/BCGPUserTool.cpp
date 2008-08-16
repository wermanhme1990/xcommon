//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// BCGPUserTool.cpp: implementation of the CBCGPUserTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGCBPro.h"
#include "BCGGlobals.h"
#include "BCGPLocalResource.h"
#include "BCGPUserTool.h"

#include "bcgprores.h"

IMPLEMENT_SERIAL(CBCGPUserTool, CObject, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPUserTool::CBCGPUserTool()
{
	m_uiCmdId = 0;
	m_hIcon = NULL;
}
//*****************************************************************************************
CBCGPUserTool::~CBCGPUserTool()
{
	DeleteIcon ();
}
//*****************************************************************************************
void CBCGPUserTool::Serialize (CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_strLabel;
		
		CString strCmd;
		ar >> strCmd;
		SetCommand (strCmd);

		ar >> m_strArguments;
		ar >> m_strInitialDirectory;
		ar >> m_uiCmdId;
	}
	else
	{
		ar << m_strLabel;
		ar << m_strCommand;
		ar << m_strArguments;
		ar << m_strInitialDirectory;
		ar << m_uiCmdId;
	}
}
//*****************************************************************************************
BOOL CBCGPUserTool::Invoke ()
{
	if (m_strCommand.IsEmpty ())
	{
		TRACE(_T("Empty command in user-defined tool: %d\n"), m_uiCmdId);
		return FALSE;
	}

	if (::ShellExecute (AfxGetMainWnd()->GetSafeHwnd (), NULL, m_strCommand,
		m_strArguments, m_strInitialDirectory, 
		SW_SHOWNORMAL) < (HINSTANCE) 32)
	{
		TRACE(_T("Can't invoke command: %s\n"), m_strCommand);
		return FALSE;
	}

	return TRUE;
}
//******************************************************************************
void CBCGPUserTool::DrawToolIcon (CDC* pDC, const CRect& rectImage)
{
	ASSERT_VALID (pDC);

	int cx = globalData.m_sizeSmallIcon.cx;
	int cy = globalData.m_sizeSmallIcon.cy;

	int x = max (0, (rectImage.Width () - cx) / 2);
	int y = max (0, (rectImage.Height () - cy) / 2);

	::DrawIconEx (pDC->GetSafeHdc (),
		rectImage.left + x, rectImage.top + y, m_hIcon,
		0, 0, 0, NULL, DI_NORMAL);
}
//******************************************************************************
BOOL CBCGPUserTool::CopyIconToClipboard ()
{
	CBCGPLocalResource locaRes;

	try
	{
		CWindowDC dc (NULL);

		int cx = globalData.m_sizeSmallIcon.cx;
		int cy = globalData.m_sizeSmallIcon.cy;

		//----------------------
		// Create a bitmap copy:
		//----------------------
		CDC memDCDest;
		memDCDest.CreateCompatibleDC (NULL);
		
		CBitmap bitmapCopy;
		if (!bitmapCopy.CreateCompatibleBitmap (&dc, cx, cy))
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			return FALSE;
		}

		CBitmap* pOldBitmapDest = memDCDest.SelectObject (&bitmapCopy);

		CRect rectIcon (0, 0, cx, cy);
		memDCDest.FillRect (rectIcon, &globalData.brBtnFace);

		DrawToolIcon (&memDCDest, rectIcon);

		memDCDest.SelectObject (pOldBitmapDest);

		if (!AfxGetMainWnd ()->OpenClipboard ())
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			return FALSE;
		}

		if (!::EmptyClipboard ())
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			::CloseClipboard ();
			return FALSE;
		}

		HANDLE hclipData = ::SetClipboardData (CF_BITMAP, bitmapCopy.Detach ());
		if (hclipData == NULL)
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			TRACE (_T("CBCGToolBar::OnBcgbarresCopyImage error. Error code = %x\n"), GetLastError ());
		}

		::CloseClipboard ();
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox (IDP_BCGBARRES_INTERLAL_ERROR);
	}

	return FALSE;
}
//******************************************************************************
void CBCGPUserTool::SetCommand (LPCTSTR lpszCmd)
{
	ASSERT (lpszCmd != NULL);

	if (m_strCommand != lpszCmd)
	{
		m_strCommand = lpszCmd;

		DeleteIcon ();
		m_hIcon = SetToolIcon ();
	}
}
//*****************************************************************************
HICON CBCGPUserTool::SetToolIcon ()
{
	CString strPath = m_strCommand;

	//--------------------------------------------
	// If the image path is not defined, found it:
	//--------------------------------------------
	if (strPath.Find (_T("\\")) == -1 &&
		strPath.Find (_T("/")) == -1 &&
		strPath.Find (_T(":")) == -1)
	{
		TCHAR lpszPath [MAX_PATH];
		CString strFileName = m_strCommand;

		if (::SearchPath (NULL, strFileName, NULL, MAX_PATH, 
			lpszPath, NULL) == 0)
		{
			return LoadDefaultIcon ();
		}

		strPath = lpszPath;
	}

	//----------------------------------------
	// Try to obtain a default icon from file:
	//----------------------------------------
	SHFILEINFO sfi;
	if (::SHGetFileInfo (strPath, 0, &sfi, sizeof(SHFILEINFO),
		SHGFI_ICON | SHGFI_SHELLICONSIZE | SHGFI_SMALLICON))
	{
		return sfi.hIcon;
	}

	return LoadDefaultIcon ();
}
//******************************************************************************
HICON CBCGPUserTool::LoadDefaultIcon ()
{
	if (globalData.m_hiconTool == NULL)
	{
		CBCGPLocalResource locaRes;

		globalData.m_hiconTool = (HICON) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE (IDI_BCGRES_TOOL),
			IMAGE_ICON,
			globalData.m_sizeSmallIcon.cx,
			globalData.m_sizeSmallIcon.cy,
			LR_SHARED);
	}

	return globalData.m_hiconTool;
}
//*******************************************************************************
void CBCGPUserTool::DeleteIcon ()
{
	if (m_hIcon != NULL && m_hIcon != globalData.m_hiconTool)
	{
		::DestroyIcon (m_hIcon);
	}

	m_hIcon = NULL;
}
