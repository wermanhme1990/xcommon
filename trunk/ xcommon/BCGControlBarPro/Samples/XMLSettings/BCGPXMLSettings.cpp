// BCGPXMLSettings.cpp: implementation of the CBCGPXMLSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPXMLSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPXMLSettings, CBCGPRegistry)

CBCGPXMLNode	CBCGPXMLSettings::m_Local;
CBCGPXMLNode	CBCGPXMLSettings::m_User;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPXMLSettings::CBCGPXMLSettings()
{
	m_pCurrNode = NULL;

	if (m_Local.m_strName.IsEmpty ())
	{
		m_Local.m_strName = _T("LOCAL_MACHINE");
	}

	if (m_User.m_strName.IsEmpty ())
	{
		m_User.m_strName = _T("CURRENT_USER");
	}
}

CBCGPXMLSettings::~CBCGPXMLSettings()
{
}

BOOL CBCGPXMLSettings::VerifyKey (LPCTSTR /*pszPath*/)
{
	ASSERT (FALSE);	// TODO
	return TRUE;
}

BOOL CBCGPXMLSettings::VerifyValue (LPCTSTR /*pszValue*/)
{
	ASSERT (FALSE);	// TODO
	return TRUE;
}

BOOL CBCGPXMLSettings::CreateKey (LPCTSTR pszPath)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (pszPath != NULL);

	CString strPath = pszPath;
	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}

	CBCGPXMLNode* pNode = GetTree ();

	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGPXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			pChild = new CBCGPXMLNode;
			pChild->m_strName = strSubKey;
			pNode->AddChild (pChild);
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	m_pCurrNode = pNode;
	return TRUE;
}

BOOL CBCGPXMLSettings::Open (LPCTSTR pszPath)
{
	ASSERT (pszPath != NULL);

	CBCGPXMLNode* pNode = GetTree ();

	m_sPath = pszPath;

	CString strPath = pszPath;
	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}

	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGPXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			m_pCurrNode = NULL;
			return FALSE;
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	m_pCurrNode = pNode;
	return TRUE;
}
//***************************************************************************************
void CBCGPXMLSettings::Close()
{
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, int iVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CString str;
	str.Format (_T("%d"), iVal);

	return WriteTag (pszKey, str);
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, DWORD dwVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CString str;
	str.Format (_T("%d"), dwVal);

	return WriteTag (pszKey, str);
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	return WriteTag (pszKey, pszData);
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, CObject& obj)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			obj.Serialize (ar);
			ar.Flush ();
		}

#if _MSC_VER >= 1300
		ULONGLONG dwDataSize = file.GetLength ();
#else
		DWORD dwDataSize = file.GetLength ();
#endif
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, CObject* pObj)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);
			ar << pObj;
			ar.Flush ();
		}

#if _MSC_VER >= 1300
		ULONGLONG dwDataSize = file.GetLength ();
#else
		DWORD dwDataSize = file.GetLength ();
#endif
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, CWordArray& wcArray)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << wcArray.GetSize ();
			for (int i = 0; i < wcArray.GetSize (); i ++)
			{
				ar << wcArray [i];
			}

			ar.Flush ();
		}

#if _MSC_VER >= 1300
		ULONGLONG dwDataSize = file.GetLength ();
#else
		DWORD dwDataSize = file.GetLength ();
#endif
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write(LPCTSTR pszKey, const CRect& rect)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << rect;
			ar.Flush ();
		}

#if _MSC_VER >= 1300
		ULONGLONG dwDataSize = file.GetLength ();
#else
		DWORD dwDataSize = file.GetLength ();
#endif
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	const int iMaxChars = 20;
	CDWordArray dwcArray;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, lpPoint->x);
	dwcArray.SetAt(1, lpPoint->y);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
#if _MSC_VER >= 1300
	ULONGLONG dwLen = file.GetLength ();
#else
	const DWORD dwLen = file.GetLength ();
#endif
	LPBYTE lpbData = file.Detach ();
	BOOL bRes = Write (pszKey, lpbData, (UINT) dwLen);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Write (LPCTSTR pszKey, LPBYTE pData, UINT nBytes)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (pszKey != NULL);
	ASSERT (pData != NULL);
	ASSERT (AfxIsValidAddress (pData, nBytes, FALSE));

	LPTSTR lpszBuffer = new TCHAR [nBytes * 2 + 1];
	ASSERT (lpszBuffer != NULL);

	lpszBuffer [0] = 0;

	char lpszByte [3];
	int j = 0;

	for (UINT i = 0; i < nBytes; i++)
	{
		sprintf (lpszByte, "%02x", pData [i]);

		lpszBuffer [j++] = lpszByte [0];
		lpszBuffer [j++] = lpszByte [1];
	}

	lpszBuffer [j] = 0;

	BOOL bRes = WriteTag (pszKey, lpszBuffer);

	delete [] lpszBuffer;
	return bRes;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::WriteTag (LPCTSTR pszKey, LPCTSTR lpszBuffer)
{
	ASSERT (pszKey != NULL);
	ASSERT (lpszBuffer != NULL);

	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (m_pCurrNode == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT_VALID (m_pCurrNode);

	CString strKey = pszKey;
	strKey.Remove (_T(' '));

	CBCGPXMLNode* pNode = m_pCurrNode->FindChild (strKey);
	if (pNode == NULL)
	{
		pNode = new CBCGPXMLNode;
		pNode->m_strName = strKey;

		m_pCurrNode->AddChild (pNode);
	}

	pNode->m_strValue = lpszBuffer;
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read(LPCTSTR pszKey, int& iVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	iVal = _ttoi (lpszVal);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	dwVal = _ttol (lpszVal);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, CString& sVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		sVal.Empty ();
		return FALSE;
	}

	sVal = lpszVal;
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, CWordArray& wcArray)
{
	wcArray.SetSize (0);

	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		int iSize;
		ar >> iSize;

		wcArray.SetSize (iSize);
		for (int i = 0; i < iSize; i ++)
		{
			ar >> wcArray [i];
		}

		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGPXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read(LPCTSTR pszKey, CRect& rect)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		ar >> rect;
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGPXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		CDWordArray dwcArray;
		dwcArray.SetSize(5);
		dwcArray.Serialize (ar);
		lpPoint->x = dwcArray.GetAt(0);
		lpPoint->y = dwcArray.GetAt(1);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGPXMLSettings::Read ()!\n"));
	}

	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, BYTE** ppData, UINT* pBytes)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	int nLen = lstrlen (lpszVal);
	if ((nLen % 2) != 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	*pBytes = nLen / 2;
	*ppData = new BYTE [*pBytes];

	TCHAR szByte [3];
	szByte [2] = 0;

	int j = 0;
	for (int i = 0; i < nLen; i += 2)
	{
		szByte [0] = lpszVal [i];
		szByte [1] = lpszVal [i + 1];

		int nValue = 0;
		_stscanf (szByte, _T("%x"), &nValue);

		BYTE b = (BYTE) nValue;
		(*ppData) [j++] = b;
	}
	
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, CObject& obj)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		obj.Serialize (ar);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGPXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::Read (LPCTSTR pszKey, CObject*& pObj)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		ar >> pObj;

		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGPXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGPXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::DeleteValue (LPCTSTR pszValue)
{
	if (m_pCurrNode == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pCurrNode);

	for (POSITION pos = m_pCurrNode->m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGPXMLNode* pNode = m_pCurrNode->m_lstChildren.GetNext (pos);
		ASSERT_VALID (pNode);

		if (pNode->m_strName == pszValue)
		{
			m_pCurrNode->m_lstChildren.RemoveAt (posSave);
			delete pNode;
			return TRUE;
		}
	}

	return FALSE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::DeleteKey (LPCTSTR pszPath, BOOL bAdmin)
{
	if (m_bReadOnly)
	{
		return FALSE;
	}
	
	CBCGPXMLNode* pNode = bAdmin ? &m_Local : &m_User;

	ASSERT (pszPath != NULL);
	CString strPath = pszPath;

	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}
	
	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGPXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			return FALSE;
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	ASSERT_VALID (pNode->m_pParent);
	for (POSITION pos = pNode->m_pParent->m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGPXMLNode* pCurrNode = pNode->m_pParent->m_lstChildren.GetNext (pos);
		ASSERT_VALID (pCurrNode);

		if (pNode == pCurrNode)
		{
			pNode->m_pParent->m_lstChildren.RemoveAt (posSave);
			delete pNode;
			return TRUE;
		}
	}

	ASSERT(FALSE);
	return FALSE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::ReadSubKeys(CStringArray& /*SubKeys*/)
{
	ASSERT (FALSE);
	return FALSE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::ReadKeyValues(CStringArray& /*Values*/)
{
	ASSERT (FALSE);
	return FALSE;
}
//***************************************************************************************
LPCTSTR CBCGPXMLSettings::ReadTag (LPCTSTR pszKey) const
{
	ASSERT (pszKey != NULL);

	if (m_pCurrNode == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (m_pCurrNode);

	CString strKey = pszKey;
	strKey.Remove (_T(' '));

	CBCGPXMLNode* pNode = m_pCurrNode->FindChild (strKey);
	if (pNode == NULL)
	{
		return NULL;
	}

	return pNode->m_strValue;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::WriteXMLToFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName)
{
	CBCGPXMLNode& node = bIsLocalMachine ? m_Local : m_User;

	// Create XML buffer:
	CString strBuffer;
	if (node.WriteToBuffer (strBuffer, 0) <= 0)
	{
		return FALSE;
	}

	try
	{
		CStdioFile file;
		file.Open (lpszFileName, 
					CFile::modeWrite | CFile::modeCreate | CFile:: typeText);

		file.WriteString (strBuffer);
	}
	catch (CFileException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();

		return FALSE;
	}

	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLSettings::ReadXMLFromFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName)
{
	CString strBuffer;

	try
	{
		CStdioFile file;
		if (!file.Open (lpszFileName, CFile::modeRead))
		{
			TRACE(_T("File not found: %s"), lpszFileName);
			return FALSE;
		}

		CString str;

		while (file.ReadString (str))
		{
			strBuffer += str;
		}
	}
	catch (CFileException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();

		return FALSE;
	}

	CBCGPXMLNode& node = bIsLocalMachine ? m_Local : m_User;
	return node.ReadFromBuffer (strBuffer);
}
//***************************************************************************************
int CBCGPXMLNode::WriteToBuffer (CString& strBuffer, int iOffset)
{
	if (m_lstChildren.IsEmpty () && m_pParent != NULL)
	{
		CString strTagValue;
		strTagValue.Format (_T("<%s>\"%s\"</%s>\n"), m_strName, m_strValue, m_strName);

		strBuffer.Insert (iOffset, strTagValue);
		return strTagValue.GetLength ();
	}

	int iOffsetOrig = iOffset;

	CString strTagStart;
	strTagStart.Format (_T("<%s>\n"), m_strName);

	strBuffer.Insert (iOffset, strTagStart);
	iOffset += strTagStart.GetLength ();

	for (POSITION pos = m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		CBCGPXMLNode* pNode = m_lstChildren.GetNext (pos);
		ASSERT_VALID (pNode);

		iOffset += pNode->WriteToBuffer (strBuffer, iOffset);
	}

	CString strTagEnd;
	strTagEnd.Format (_T("</%s>\n"), m_strName);

	strBuffer.Insert (iOffset, strTagEnd);
	iOffset += strTagEnd.GetLength ();

	return iOffset - iOffsetOrig;
}
//***************************************************************************************
static BOOL ExcludeTag (CString& strBuffer, 
						LPCTSTR lpszTag,
						CString& strTag)
{
	const int iBufLen = strBuffer.GetLength ();

	CString strTagStart = _T('<');
	strTagStart += lpszTag;
	strTagStart += _T('>');

	const int iTagStartLen = strTagStart.GetLength ();

	int iStart = -1;

	int iIndexStart = strBuffer.Find (strTagStart);
	if (iIndexStart < 0)
	{
		return FALSE;
	}

	iStart = iIndexStart + iTagStartLen;

	CString strTagEnd = _T("</");
	strTagEnd += lpszTag;
	strTagEnd += _T('>');

	const int iTagEndLen = strTagEnd.GetLength ();

	int iIndexEnd =  -1;
	int nBalanse = 1;
	for (int i = iStart; i < iBufLen - iTagEndLen + 1; i ++)
	{
		if (strBuffer [i] != '<')
		{
			continue;
		}

		if (i < iBufLen - iTagStartLen &&
			_tcsncmp (strBuffer.Mid (i), strTagStart, iTagStartLen) == 0)
		{
			i += iTagStartLen - 1;
			nBalanse ++;
			continue;
		}

		if (_tcsncmp (strBuffer.Mid (i), strTagEnd, iTagEndLen) == 0)
		{
			nBalanse --;
			if (nBalanse == 0)
			{
				iIndexEnd = i;
				break;
			}

			i += iTagEndLen - 1;
		}
	}

	if (iIndexEnd == -1 || iStart > iIndexEnd)
	{
		return FALSE;
	}

	strTag = strBuffer.Mid (iStart, iIndexEnd - iStart);
	strTag.TrimLeft ();
	strTag.TrimRight ();

	strBuffer.Delete (iIndexStart, iIndexEnd + iTagEndLen - iIndexStart);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGPXMLNode::ReadFromBuffer (CString& strBuffer)
{
	m_strValue.Empty ();

	while (!m_lstChildren.IsEmpty ())
	{
		delete m_lstChildren.RemoveHead ();
	}

	if (strBuffer [0] != '<')
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int iTagEnd = strBuffer.Find ('>');
	if (iTagEnd < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_strName = strBuffer.Mid (1, iTagEnd - 1);

	CString strTag;
	if (!ExcludeTag (strBuffer, m_strName, strTag))
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (strTag.IsEmpty ())
	{
		return TRUE;
	}

	if (strTag [0] == '\"')
	{
		ASSERT (strTag [strTag.GetLength () - 1] == '\"');
		m_strValue = strTag.Mid (1, strTag.GetLength () - 2);

		return TRUE;
	}

	if (strTag [0] != '<')
	{
		ASSERT(FALSE);
		return FALSE;
	}

	while (!strTag.IsEmpty ())
	{
		CBCGPXMLNode* pChild = new CBCGPXMLNode;
		if (!pChild->ReadFromBuffer (strTag))
		{
			delete pChild;
			return FALSE;
		}

		AddChild (pChild);
	}

	return TRUE;
}
//***************************************************************************************
CBCGPXMLNode* CBCGPXMLSettings::GetTree () const
{
	return m_bAdmin ? &m_Local : &m_User;
}
