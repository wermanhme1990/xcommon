// TBListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "toolboxdemo.h"
#include "TBListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT CTBData::s_ClpFormat = 0;
CString CTBData::s_ClpFormatName = _T("BCGPToolBoxClpFmt");

static CTBData TBData;

CTBData::CTBData ()
{
	if (GetClipboardFormat () == 0)
	{
		s_ClpFormat = (CLIPFORMAT)
			::RegisterClipboardFormat (GetClipboardFormatName ());
	}
}

CLIPFORMAT CTBData::GetClipboardFormat ()
{
	return s_ClpFormat;
}

LPCTSTR CTBData::GetClipboardFormatName ()
{
	return s_ClpFormatName;
}

BOOL CTBData::IsClipboardFormatAvailable (
	COleDataObject* pDataObject)
{
	ASSERT (pDataObject != NULL);
	return (pDataObject->IsDataAvailable (GetClipboardFormat ()));
}

BOOL CTBData::SerializeTo (CFile& file, CBCGPToolBoxButton* pTool, int nPage, int nTool)
{
	CArchive Archive (&file, CArchive::store);

	CBCGPToolBarImages* pImages = pTool->GetImageList ();
	ASSERT_VALID (pImages);

	CImageList imageList;
	imageList.Create (pImages->GetImageSize ().cx, pImages->GetImageSize ().cy,
		ILC_COLOR32 | ILC_MASK, 1, 0);

	imageList.Add (pImages->ExtractIcon (pTool->GetImageIndex ()));

	CString strName = pTool->GetLabel ();
	BOOL bResult = Serialize (Archive, strName, imageList, nPage, nTool);

	Archive.Close();
	return bResult;
}

BOOL CTBData::SerializeFrom (CFile& file, CString& strLabel, CImageList& imageList,
							 int& nPage, int& nTool)
{
	CArchive Archive (&file, CArchive::load);

	BOOL bResult = Serialize (Archive, strLabel, imageList, nPage, nTool);

	Archive.Close();
	return bResult;
}

BOOL CTBData::Serialize (CArchive& ar, CString& strLabel, CImageList& imageList,
						 int& nPage, int& nTool)
{
	if (ar.IsStoring ())
	{
		ar << strLabel;
		ar << nPage;
		ar << nTool;
		imageList.Write (&ar);
	}
	else
	{
		ar >> strLabel;
		ar >> nPage;
		ar >> nTool;
		imageList.Read (&ar);
	}

	return TRUE;
}



CTBDropTarget::CTBDropTarget()
	:  m_pCtrl (NULL)
{
}

CTBDropTarget::~CTBDropTarget()
{
}

BOOL CTBDropTarget::Register (CTBListCtrl* pCtrl)
{
	ASSERT_VALID (pCtrl);
	ASSERT (pCtrl != m_pCtrl);

	if (pCtrl != m_pCtrl)
	{
		if (COleDropTarget::Register (pCtrl))
		{
			m_pCtrl = pCtrl;
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

DROPEFFECT CTBDropTarget::OnDragEnter(CWnd* /*pWnd*/, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	ASSERT_VALID (m_pCtrl);

	if (!CTBData::IsClipboardFormatAvailable (pDataObject))
	{
		return DROPEFFECT_NONE;
	}

	if (m_pCtrl != NULL)
	{
		ASSERT_VALID (m_pCtrl);
		return m_pCtrl->OnDragEnter(pDataObject, dwKeyState, point);
	}

	return DROPEFFECT_NONE;
}

void CTBDropTarget::OnDragLeave(CWnd* /*pWnd*/) 
{
	ASSERT_VALID (m_pCtrl);

	if (m_pCtrl != NULL)
	{
		ASSERT_VALID (m_pCtrl);
		m_pCtrl->OnDragLeave ();
	}
}

DROPEFFECT CTBDropTarget::OnDragOver(CWnd* /*pWnd*/, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	ASSERT_VALID (m_pCtrl);

	if (!CTBData::IsClipboardFormatAvailable (pDataObject))
	{
		return DROPEFFECT_NONE;
	}

	if (m_pCtrl != NULL)
	{
		ASSERT_VALID (m_pCtrl);
		return m_pCtrl->OnDragOver(pDataObject, dwKeyState, point);
	}

	return DROPEFFECT_NONE;
}

DROPEFFECT CTBDropTarget::OnDropEx(CWnd* /*pWnd*/, 
							COleDataObject* pDataObject, 
							DROPEFFECT dropEffect, 
							DROPEFFECT /*dropList*/, CPoint point) 
{
	ASSERT_VALID (m_pCtrl);

	if (!CTBData::IsClipboardFormatAvailable (pDataObject))
	{
		return DROPEFFECT_NONE;
	}

	if (m_pCtrl != NULL)
	{
		ASSERT_VALID (m_pCtrl);
		return m_pCtrl->OnDrop(pDataObject, dropEffect, point) ?
				dropEffect : DROPEFFECT_NONE;
	}

	return DROPEFFECT_NONE;
}

/////////////////////////////////////////////////////////////////////////////
// CTBListCtrl

CTBListCtrl::CTBListCtrl()
{
}

CTBListCtrl::~CTBListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTBListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTBListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTBListCtrl message handlers


BOOL CTBListCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if (dropEffect != DROPEFFECT_COPY)
	{
		return FALSE;
	}

	ASSERT (pDataObject != NULL);
	ASSERT (CTBData::IsClipboardFormatAvailable (pDataObject));

	try
	{
		CFile* pFile = pDataObject->GetFileData (CTBData::GetClipboardFormat ());
		if (pFile == NULL)
		{
			return FALSE;
		}

		int nTool = -1;
		int nPage = -1;
		CString strLabel;

		CImageList	imageList;

		BOOL bRes = CTBData::SerializeFrom (*pFile, strLabel, imageList, nPage, nTool);

		delete pFile;

		int nImage = m_ImageList.Add (imageList.ExtractIcon (0));
		int nItem = InsertItem (GetItemCount (), strLabel, nImage);

		if (nItem != -1)
		{
			CString str;

			str.Format (_T("%d"), nPage);
			SetItemText (nItem, 1, str);

			str.Format (_T("%d"), nTool);
			SetItemText (nItem, 2, str);
		}

		return bRes;
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CTBListCtrl::OnDrop. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}

	return FALSE;
}

DROPEFFECT CTBListCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return OnDragOver(pDataObject, dwKeyState, point);
}

void CTBListCtrl::OnDragLeave()
{
}

DROPEFFECT CTBListCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT (pDataObject != NULL);

	if (CTBData::IsClipboardFormatAvailable (pDataObject))
	{
		return DROPEFFECT_COPY;
	}

	return DROPEFFECT_NONE;
}

void CTBListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	m_DropTarget.Register (this);

	m_ImageList.Create (16, 16, ILC_MASK | ILC_COLOR32, 1, 1);
	SetImageList (&m_ImageList, LVSIL_NORMAL);
	SetImageList (&m_ImageList, LVSIL_SMALL);
}
