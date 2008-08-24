/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRegistryManager.h					
文件内容:	    用于管理系统配置文件 支持注册表与ini同时
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/03/14
*********************************************************************/

#include "stdafx.h"
#include "XRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//CXString CXRegistryManager::m_strPath;
/////////////////////////////////////////////////////////////////////////////
// CHKey - helper automatically closes open HKEY
#define SAFE_CLOSEKEY(hKey)  if (hKey) {::RegCloseKey(hKey),hKey = NULL;}



/////////////////////////////////////////////////////////////////////////////
// CXRegistryManager

CXRegistryManager::CXRegistryManager(HKEY hKeyBase/*= HKEY_CURRENT_USER*/)
	: m_hAppKey(NULL), m_hKeyBase(hKeyBase)	
	, m_lResult(ERROR_SUCCESS)
  , m_strPath(), m_nMode(0)
{
	assert(hKeyBase == HKEY_CURRENT_USER || hKeyBase == HKEY_LOCAL_MACHINE);	
}
CXRegistryManager::CXRegistryManager(LPCTSTR strPath)
: m_hAppKey(NULL), m_hKeyBase(NULL)
, m_lResult(ERROR_SUCCESS), m_strPath(strPath)
{
  assert(strPath);
	SetMode(Mode_INI);
}
CXRegistryManager::CXRegistryManager( HKEY hKeyBase /*= HKEY_CURRENT_USER*/, LPCTSTR lpszKey)
: m_hAppKey(NULL), m_hKeyBase(hKeyBase)
, m_lResult(ERROR_SUCCESS)
, m_strPath(lpszKey)
{
	assert(hKeyBase == HKEY_CURRENT_USER || hKeyBase == HKEY_LOCAL_MACHINE);
	SetMode(Mode_REG);
}
CXRegistryManager::~CXRegistryManager()
{
	SAFE_CLOSEKEY(m_hAppKey);
}


HKEY CXRegistryManager::GetSectionKey(LPCTSTR lpszSection, REGSAM samDesired)
{
	assert(lpszSection != NULL);
	HKEY hSectionKey = NULL;
	DWORD dwTmp = 0;
	__try
	{
		if (!lpszSection)
		{
			__leave;	
		}
		if (!m_hAppKey)
		{
			m_lResult = ::RegCreateKeyEx(m_hKeyBase, m_strPath, 0L, REG_NONE,
				REG_OPTION_NON_VOLATILE, samDesired, NULL, &m_hAppKey, &dwTmp);	
			if (m_lResult != ERROR_SUCCESS || !m_hAppKey)
			{
				__leave;
			}
		}

		if (!m_hAppKey)
		{
			__leave;	
		}
		m_lResult = ::RegCreateKeyEx(m_hAppKey, lpszSection, 0, REG_NONE,	REG_OPTION_NON_VOLATILE,
			samDesired, NULL, &hSectionKey, &dwTmp);
		if (m_lResult != ERROR_SUCCESS)
		{
			__leave;
		}
	}
	__finally
	{
		 ;
	}

	return hSectionKey;
}


BOOL CXRegistryManager::WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
																					 LPBYTE pData, const LONG& nBytes)
{
	assert(lpszSection);	
	BOOL blRet = FALSE;
	HKEY hSection = NULL;
	__try
	{
		if (!lpszSection || nBytes <= 0)
		{
			__leave;
		}
		if (Mode_REG == m_nMode)
		{		
			if(!(hSection = GetSectionKey(lpszSection)))
			{
				__leave;
			}	
			if (ERROR_SUCCESS == (m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_BINARY,
				pData, nBytes)))
			{
				blRet = TRUE;		
				__leave;
			}							
		}
		else if (Mode_INI == m_nMode)
		{
			LPTSTR lpszBuffer = NULL;
			VERIFY(lpszBuffer = (LPTSTR)_alloca(nBytes * 2 + 1));
			LONG i;
			for (i = 0; i < (LONG)nBytes; i ++)
			{
				lpszBuffer[i * 2] = (TCHAR)((pData[i] & 0x0F) + 'A'); 
				lpszBuffer[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); 
			}
			lpszBuffer[i * 2] = 0;
			assert(!m_strPath.IsEmpty());
			blRet = WriteProfileString(lpszSection, lpszEntry, lpszBuffer);	
			__leave;
		}	
	}
	__finally
	{
		SAFE_CLOSEKEY(hSection);
	}
	return blRet;
}

BOOL CXRegistryManager::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	assert(lpszSection);
	BOOL blRet = FALSE;
	HKEY hSection = NULL;
	__try
	{	
		if (!lpszSection)
		{
			__leave;
		}
		if (Mode_REG == m_nMode)
		{					
			if (!(hSection =  GetSectionKey(lpszSection)))
			{
				__leave;
			}

			if(ERROR_SUCCESS == (m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_SZ,
				(LPBYTE)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR))))
			{
				blRet = TRUE;
				__leave;
			}
		}	
		else if (Mode_INI == m_nMode)
		{
			assert(!m_strPath.IsEmpty());
			assert(m_strPath.StrLen() < 4095); 
			blRet = ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
				m_strPath);
			__leave;
		}	
	}
	__finally
	{
		SAFE_CLOSEKEY(hSection);
	}
	return blRet;
}

UINT CXRegistryManager::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, const int& nDefault)
{
	assert(lpszSection != NULL);
	HKEY hSection = NULL;
	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof(DWORD);
	dwValue = nDefault;
	__try
	{
		if (!lpszSection)
		{
			__leave;
		}
		if (Mode_REG == m_nMode) 
		{
			if(!(hSection = GetSectionKey(lpszSection, KEY_READ)))
			{
				__leave;
			}
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
			if (m_lResult != ERROR_SUCCESS || dwType != REG_DWORD)
			{
				__leave;
			}		
			__leave;
		}
		else if (Mode_INI == m_nMode)
		{
			assert(!m_strPath.IsEmpty());
			return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,	m_strPath);
		}	
	}
	__finally
	{
		SAFE_CLOSEKEY(hSection);
	}
	return dwValue;
}

BOOL CXRegistryManager::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, const int& nValue)
{
	assert(lpszSection);
	assert(lpszEntry);
	static TCHAR szBuffer[MAX_PATH];
	HKEY hSection = NULL;
	BOOL blRet = FALSE;
	__try
	{
		if (!lpszSection)
		{
			__leave;
		}
		if (Mode_REG == m_nMode)
		{		
			if (!(hSection = GetSectionKey(lpszSection)))
			{
				__leave;
			}		
			if (ERROR_SUCCESS == (m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL,
				REG_DWORD, (LPBYTE)&nValue, sizeof(nValue))))
			{
				blRet = TRUE;
				__leave;
			}			
		}	
		else if (Mode_INI == m_nMode)
		{
			assert(!m_strPath.IsEmpty());		
			_stprintf_s(szBuffer, MAX_PATH, TEXT("%d"), nValue);
			blRet = ::WritePrivateProfileString(lpszSection, lpszEntry, szBuffer,	m_strPath);
			__leave;
		}	
	}
	__finally
	{
		SAFE_CLOSEKEY(hSection);
	}
	return blRet;
}
BYTE* CXRegistryManager::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
																					LONG& nBytes)
{
	assert(lpszSection != NULL);			
	BYTE *pData = NULL;
	HKEY hSection = NULL;
	nBytes = 0;
	if (!lpszSection)
	{
		return pData;
	}			
	if (Mode_REG == m_nMode)
	{
		DWORD dwType;
		if(hSection = GetSectionKey(lpszSection, KEY_READ))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, NULL, (LPDWORD)&nBytes);
			if (m_lResult == ERROR_SUCCESS && REG_BINARY == dwType)
			{
				if (nBytes > 0)
				{				
					VERIFY(pData = new BYTE[nBytes]);	
					memset(pData, 0, sizeof(*pData) * nBytes);
					if(ERROR_SUCCESS != (m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, 
						pData, (LPDWORD)&nBytes)))
					{
						SAFE_DELETE_AR(pData);
					}
				}	
			}			
		}
		SAFE_CLOSEKEY(hSection);
		
		return pData;
	}
	else if(Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());
		CXString strBuffer = GetProfileString(lpszSection, lpszEntry, TEXT(""));
		if (strBuffer.IsEmpty())
		{
			return pData;	
		}
		nBytes = (LONG)strBuffer.StrLen() * 2;	
		if (nBytes > 0)
		{
			VERIFY(pData = new BYTE[nBytes]);
			memset(pData, 0, sizeof(*pData) * nBytes);
			LONG i = 0;
			for (; i < nBytes; i += 2)
			{
				(pData)[i/2] = (BYTE)	(((strBuffer[i + 1] - 'A') << 4) + (strBuffer[i] - 'A'));
			}
		}			
		return pData;
	}	
	return pData;
}

CXString CXRegistryManager::GetProfileString(LPCTSTR lpszSection, 
                                             LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	HKEY hSection = NULL;
	CXString strValue(lpszDefault);
	TCHAR szBuffer[4096];
	memset(szBuffer, 0, sizeof(szBuffer));
	if (Mode_REG == m_nMode)
	{		
		DWORD dwType, dwCount = 0;
		if (hSection = GetSectionKey(lpszSection, KEY_READ))

		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);
			if ((m_lResult == ERROR_SUCCESS && dwType == REG_SZ))
			{
				m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,	
					(LPBYTE)strValue.GetBuffer(dwCount / sizeof(TCHAR) + 1), &dwCount);
				strValue.ReleaseBuffer();
			}					
		}	
		SAFE_CLOSEKEY(hSection);
		return strValue;		
	}
	else if(Mode_INI == m_nMode)
	{
		TCHAR chNil = '\0';
		assert(!m_strPath.IsEmpty());
		if (!lpszDefault)
		{
			lpszDefault = &chNil;
		}		
		strValue = lpszDefault;
		if (::GetPrivateProfileString(lpszSection, lpszEntry,
			lpszDefault, szBuffer, sizeof(szBuffer), m_strPath) > 0)
		{
			strValue =  szBuffer;
		}			
		return strValue;
	}	
	return strValue;
}

LONG CXRegistryManager::EnumValues(LPCTSTR lpszSection, 
                                  CMap<CXString, LPCTSTR, DWORD, DWORD&>* mapItems,
									vector<CXString> * pvArrayNames)
{
	static TCHAR szValue[1024];
	DWORD dwLen = 1024;
	assert(lpszSection != NULL);
	HKEY hSection = NULL;
	LONG nCount = 0;
	DWORD dwType;

	if (!(hSection = GetSectionKey(lpszSection, KEY_READ)))			
	{
		return 0;
	}				
	while (::RegEnumValue(hSection, nCount ++, szValue, &dwLen,
		NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
	{
		if (mapItems)
		{
			mapItems->SetAt(szValue, dwType);
		}
		if (pvArrayNames) 
		{
			pvArrayNames->push_back(szValue);
		}
		memset(szValue, 0, sizeof(szValue));
		dwLen = 1024;
	}
	SAFE_CLOSEKEY(hSection);

	return -- nCount;
}

LONG CXRegistryManager::EnumKeys(LPCTSTR lpszSection, vector<CXString> & vstrKeys)
{
	static TCHAR szValue[1024] = {0};
	assert(lpszSection != NULL);
	HKEY hSection = NULL;	
	DWORD dwLen = 1024;
	LONG nCount = 0;
	if (hSection = GetSectionKey(lpszSection, KEY_READ))
	{
		while (::RegEnumKeyEx(hSection, nCount ++, szValue, &dwLen,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			vstrKeys.push_back(szValue);
			dwLen = 1024;		
		}	
	}	
	SAFE_CLOSEKEY(hSection);
	return nCount > 0 ?  -- nCount : nCount;
}

BOOL CXRegistryManager::RecurseDeleteKey(HKEY hKey, LPCTSTR lpszKey)
{
	HKEY hSubKey = NULL;
	FILETIME time;
	DWORD dwSize = 1024;
	static TCHAR szBuffer[1024];
	
	if (ERROR_SUCCESS != (m_lResult = ::RegOpenKeyEx(hKey, lpszKey, 0,
		KEY_READ | KEY_WRITE, &hSubKey)))
	{
		return FALSE;
	}	
	while (::RegEnumKeyEx(hSubKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time) == ERROR_SUCCESS)
	{		
		if (!RecurseDeleteKey(hSubKey, szBuffer))
		{
			return FALSE;
		}
		dwSize = 1024;
		szBuffer[0] = 0;
	}
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	m_lResult = ::RegDeleteKey(hKey, lpszKey);
	return m_lResult == ERROR_SUCCESS;
}

BOOL CXRegistryManager::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	HKEY hSection = NULL;	
	if (!(hSection = GetSectionKey(lpszSection)))
	{
		return FALSE;
	}
	return (RecurseDeleteKey(hSection, lpszKey) == ERROR_SUCCESS);
}

BOOL CXRegistryManager::DeleteValue(LPCTSTR lpszSection, LPCTSTR lpszValue)
{
	HKEY hSection = NULL;	
	BOOL blReturn = FALSE;
	if (hSection = GetSectionKey(lpszSection, KEY_ALL_ACCESS))
	{
		blReturn = ERROR_SUCCESS == (m_lResult = ::RegDeleteValue(hSection, (LPTSTR)lpszValue));
	}	
	SAFE_CLOSEKEY(hSection);
	return m_lResult;
}

BOOL CXRegistryManager::WriteProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					const POINT& Value)
{
	HKEY hSection = NULL;
	assert(lpszSection != NULL);
	BOOL blReturn = FALSE;
	if (Mode_REG == m_nMode)
	{		
		if (hSection = GetSectionKey(lpszSection))
		{
			if (ERROR_SUCCESS == (m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_BINARY,
				(LPBYTE)&Value, 
				sizeof(Value))))
			{
				blReturn = TRUE;					
			}	
		}				

		SAFE_CLOSEKEY(hSection);
		return blReturn;
		
	}
	else if (Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());
		CXString str;
		str.Format(_T("%i,%i"), Value.x, Value.y);
		blReturn = m_lResult = ::WriteProfileString(lpszSection, lpszEntry, str);
	}		
	return blReturn;
}

POINT CXRegistryManager::GetProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					const POINT& tDefault)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	POINT tPointRet = tDefault;	
	if (Mode_REG == m_nMode)
	{				
		HKEY hSection = NULL;	
		DWORD dwType, dwCount = 0;
		if (hSection = GetSectionKey(lpszSection, KEY_READ))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,NULL, &dwCount);

			if (m_lResult == ERROR_SUCCESS && dwType == REG_BINARY && dwCount == sizeof(tPointRet))
			{
				m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,	
					(LPBYTE)&tPointRet, &dwCount);				
			}		
		}		
		SAFE_CLOSEKEY(hSection);	
		return tPointRet;
	}
	else
	{
		assert(!m_strPath.IsEmpty());

		CXString str = GetProfileString(lpszSection, lpszEntry, TEXT(""));
		if (!str.IsEmpty())
		{
			_tscanf_s(str, _T("%ld,%ld"), &tPointRet.x, &tPointRet.y);	
		}			
	}  
	return tPointRet;
}

BOOL CXRegistryManager::WriteProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																				 const RECT& rtDefault)
{
	assert(lpszSection != NULL);
	BOOL blReturn = FALSE;
	if (Mode_REG == m_nMode)
	{		
		HKEY hSection = NULL;
		if (hSection = GetSectionKey(lpszSection))
		{
			if(ERROR_SUCCESS == (m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_BINARY,
				(LPBYTE)&rtDefault, sizeof(rtDefault))))
			{
				blReturn = TRUE;				
			}	
		}
		SAFE_CLOSEKEY(hSection);
		return blReturn;
	}
	else if (Mode_INI == m_nMode)
	{
		CXString str;
		str.Format(_T("%i,%i,%i,%i"), rtDefault.left, rtDefault.top, rtDefault.right, rtDefault.bottom);
		return blReturn = WriteProfileString(lpszSection, lpszEntry, str);
	}
	return blReturn;
}

RECT CXRegistryManager::GetProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry,
																			 const RECT& rtDefault)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	CRect rtReturn = rtDefault;
	if (Mode_REG == m_nMode)
	{	
		HKEY hSection = NULL;
		hSection = GetSectionKey(lpszSection, KEY_READ);
		DWORD dwType, dwCount = 0;
		m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,NULL, &dwCount);

		if (!(m_lResult != ERROR_SUCCESS || dwType != REG_BINARY || dwCount != sizeof(rtReturn)))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,	
				(LPBYTE)&rtReturn, &dwCount);
		}		
		SAFE_CLOSEKEY(hSection);
		return rtReturn;
	}
	else if(Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());		
		CXString str = GetProfileString(lpszSection, lpszEntry,  TEXT(""));
		if (!str.IsEmpty())
		{
			_tscanf_s(str, _T("%ld,%ld,%ld,%ld"),
				&rtReturn.left, &rtReturn.top, &rtReturn.right, &rtReturn.bottom);
		}		
		return rtReturn;
	}
	return rtReturn;
}

BOOL CXRegistryManager::WriteProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																				 const SIZE& tValue)
{
	assert(lpszSection != NULL);
	BOOL blReturn = FALSE;
	if (Mode_REG == m_nMode)
	{	
		HKEY hSection = NULL;
		hSection = GetSectionKey(lpszSection);		
		m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)&tValue, sizeof(tValue));

		if (m_lResult == ERROR_SUCCESS)
		{
			blReturn = TRUE;
		}
		SAFE_CLOSEKEY(hSection);
		return blReturn;
	}
	else if (Mode_INI == m_nMode)
	{
		CXString str;
		str.Format(_T("%i,%i"), tValue.cx, tValue.cy);		
		return blReturn = WriteProfileString(lpszSection, lpszEntry, str);;
	}
	return blReturn;
}

SIZE CXRegistryManager::GetProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																				const SIZE& szResult)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	SIZE tRet = szResult;
	if (Mode_REG == m_nMode)
	{	
		HKEY hSection = NULL;
		DWORD dwType, dwCount = 0;
		hSection = GetSectionKey(lpszSection, KEY_READ);		
		m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);

		if (!(m_lResult != ERROR_SUCCESS || REG_BINARY != dwType || dwCount != sizeof(tRet)))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,(LPBYTE)&tRet, &dwCount);
		}		
		SAFE_CLOSEKEY(hSection);				
		return tRet;
	}
	else if(Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());		
		CXString str = GetProfileString(lpszSection, lpszEntry,  TEXT(""));
		if (!str.IsEmpty())
		{
			_tscanf_s(str, TEXT("%d,%d"), &tRet.cx, &tRet.cy);
		}		
		return tRet;
	}
	return tRet;
}

BOOL CXRegistryManager::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					 const double& dbValue)
{
	assert(lpszSection != NULL);
	BOOL blReturn = FALSE;
	if (Mode_REG == m_nMode)
	{		
		HKEY hSection = NULL;
		hSection = GetSectionKey(lpszSection);
		m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)&dbValue, sizeof(dbValue));

		if (m_lResult == ERROR_SUCCESS)
		{
			blReturn	= TRUE;
		}
		SAFE_CLOSEKEY(hSection);
		return blReturn;
	}
	else if (Mode_INI == m_nMode)
	{
		LPBYTE pData = (LPBYTE) &dbValue;
		UINT nBytes = sizeof(double);
		LPTSTR lpsz = (LPTSTR)_alloca(sizeof(TCHAR) * (nBytes * 2 + 1));
		LONG i = 0;
		for (; i < (LONG)nBytes; i ++)
		{
			lpsz[i * 2] = (TCHAR)((pData[i] & 0x0F) + 'A'); 
			lpsz[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); 
		}
		lpsz[i * 2] = 0;

		assert(!m_strPath.IsEmpty());		
		return blReturn = WriteProfileString(lpszSection, lpszEntry, lpsz);
	}
	return blReturn;
}

double CXRegistryManager::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					 const double& dbDefault)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	double dbReturn = dbDefault;
	if (Mode_REG == m_nMode)
	{		
		HKEY hSection = NULL;
		DWORD dwType, dwCount = 0;
		hSection = GetSectionKey(lpszSection, KEY_READ);			
		m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);

		if (!(m_lResult != ERROR_SUCCESS || dwType != REG_BINARY || dwCount != sizeof(double)))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)&dbReturn, &dwCount);
		}
		SAFE_CLOSEKEY(hSection);
		return dbReturn;
	}
	else if(Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());
		CXString str = GetProfileString(lpszSection, lpszEntry, TEXT(""));
		if (!str.IsEmpty())
		{
			assert(str.StrLen() % 2 == 0);
			LONG nLen = str.StrLen();

			LPBYTE pData = (LPBYTE)&dbReturn;
			LONG i;
			for (i = 0; i < nLen; i += 2)
			{
				(pData)[i / 2] = (BYTE)(((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
			}
		}		
		return dbReturn;
	}
	return dbReturn;
}

BOOL CXRegistryManager::WriteProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					const DWORD& dwValue)
{
	assert(lpszSection != NULL);
	BOOL blReturn = FALSE;
	if (Mode_REG == m_nMode)
	{
		HKEY hSection = NULL;
		hSection = GetSectionKey(lpszSection);
		m_lResult = ::RegSetValueEx(hSection, lpszEntry, NULL, REG_DWORD,
			(LPBYTE)dwValue, sizeof(dwValue));

		if (m_lResult == ERROR_SUCCESS)
		{
			blReturn = TRUE;
		}
		SAFE_CLOSEKEY(hSection);
		return blReturn;
	}
	else if (Mode_INI == m_nMode)
	{
		m_lResult = WriteProfileInt(lpszSection, lpszEntry, int(dwValue));
	}	 
	return m_lResult;
}

DWORD CXRegistryManager::GetProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, const DWORD& dwDefault)
{
	assert(lpszSection != NULL);
	assert(lpszEntry != NULL);
	DWORD dwReturn = dwDefault;
	if (Mode_REG == m_nMode)
	{
		HKEY hSection = NULL;
		DWORD dwType, dwCount;
		hSection  = GetSectionKey(lpszSection, KEY_READ);			
		m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType,	NULL, &dwCount);

		if (!(m_lResult != ERROR_SUCCESS || dwType != REG_DWORD || dwCount != sizeof(DWORD)))
		{
			m_lResult = ::RegQueryValueEx(hSection, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)&dwReturn, &dwCount);
		}		
		SAFE_CLOSEKEY(hSection);		
		return dwReturn;
	}
	else if(Mode_INI == m_nMode)
	{
		assert(!m_strPath.IsEmpty());

		CXString str = GetProfileString(lpszSection, lpszEntry, TEXT(""));
		if (!str.IsEmpty())
		{
			dwReturn = (DWORD)GetProfileInt(lpszSection, lpszEntry, 0);
		}		
		return dwReturn;
	}
	return dwReturn;
}

BOOL CXRegistryManager::WriteProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																					const COLORREF& crValue)
{
	return WriteProfileDword(lpszSection, lpszEntry, crValue);
}

COLORREF CXRegistryManager::GetProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
																				const COLORREF& crDefault)
{
	return GetProfileDword(lpszSection, lpszEntry, crDefault);
}

CXString CXRegistryManager::GetErrorMessage() const
{
	LPVOID lpMsgBuf = 0;

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetErrorCode(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
		);

	CXString csMessage((LPCTSTR)lpMsgBuf);	
	::LocalFree( lpMsgBuf );
	return csMessage;
}
void CXRegistryManager::Set(LPCTSTR strINIFileName, LONG nMode, HKEY hKeyBase) 
{
	/*
  TCHAR strShortPath[_MAX_PATH + 1];
	if (!HWXIsFileExist(strINIFileName))
	{
		HWXCreateFile(strINIFileName);
	}
  VERIFY(_MAX_PATH > GetShortPathName(strINIFileName, strShortPath, _MAX_PATH));
	*/
  m_strPath = strINIFileName;
	SetMode(nMode);
	m_hKeyBase = hKeyBase;
}
CXString& CXRegistryManager::GetPath() 
{
  return m_strPath;
}
LONG CXRegistryManager::GetErrorCode() const 
{
  return m_lResult;
}
void CXRegistryManager::SetMode(LONG nMode)
{
	m_nMode = nMode;
}
LONG CXRegistryManager::GetMode()
{
	return m_nMode;
}