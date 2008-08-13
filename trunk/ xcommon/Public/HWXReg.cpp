#include "stdafx.h"
#include "HWXReg.h"
#include <tchar.h>
#include <assert.h>
#define REG_BufferMaxLen		(256)
/*================================================================ 
* 函数名：    CXReg
* 参数：      (HKEY hRootKey) 
* 说明：      如果构造函数不带参数，则使用默认的参数，m_hRootKey被初始化
			  为HKEY_LOCAL_MACHINE, 如果带有参数则 m_hRootKey为指定的值
================================================================*/ 
CXReg::CXReg(HKEY hRootKey) :m_hSubKey(NULL)
{
	m_hRootKey = hRootKey; 
}
 /************************************************************************
 名称:	SetRootKey
 全称:	void CXReg::SetRootKey( HKEY hRootKey )
 类型:	public 
 返回值:	void: 
 功能:	 设置根键值
 参数:	HKEY hRootKey: 
 ************************************************************************/
void CXReg::SetRootKey( HKEY hRootKey )
{
	assert(NULL != hRootKey);
	m_hRootKey = hRootKey;
}

CXReg::~CXReg() //在析构函数中关闭打开注册表句柄
{
	Close();
}

/*================================================================ 
* 函数名：    VerifyKey
* 参数：      (HKEY hRootKey, LPCTSTR pszPath) 
* 功能描述:   判断给定的路径是否存在 (兼有打开的功能)
			  如果第一个参数为NULL，则使用默认的根键。
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::VerifyKey (LPCTSTR pszPath)
{
 	BOOL blRet = TRUE;
	if (NULL != m_hSubKey)
	{
		RegCloseKey(m_hSubKey);
		m_hSubKey = NULL;
	}
	if(ERROR_SUCCESS != RegOpenKeyEx (m_hRootKey, pszPath, 0L,
										KEY_ALL_ACCESS, &m_hSubKey))
	{	
		blRet = FALSE;
		goto Err;
	}
Err:
	return blRet;
}


/*================================================================ 
* 函数名：    VerifyValue
* 参数：      (LPCTSTR pszValue)
* 功能描述:   判断给定的值是否存在 （请先调用VerifyKey，然后在使用该函数）
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::VerifyValue (LPCTSTR pszValue)
{
	BOOL blRet = FALSE;
	if(ERROR_SUCCESS == RegQueryValueEx(m_hSubKey, pszValue, NULL,
		NULL, NULL, NULL))
	{
		blRet = TRUE;
		goto Err;
	}
Err:
	return blRet;
}

/*================================================================ 
* 函数名：    VerifyValue
* 参数：      (LPCTSTR pszValue)
* 功能描述:   判断指定的键名是否等于某个值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::IsEqual(LPCTSTR pszValue,DWORD dwValue)
{
	DWORD dwTemp;
	Read(pszValue, dwTemp);
	if(dwTemp == dwValue)
		return TRUE;
	return FALSE;
}
BOOL CXReg::IsEqual(LPCTSTR pszValue,LPCTSTR lpszString)
{
	CXStringT str;
	Read(pszValue, str);
	if(0 == str.CompareNoCase(lpszString))
		return TRUE;
	return FALSE;
}


/*================================================================ 
* 函数名：    CreateKey
* 参数：      (HKEY hRootKey, LPCTSTR pszPath)
* 功能描述:   创建路径
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::CreateKey (LPCTSTR pszPath)
{
	DWORD dwTmp;	
	if (m_hSubKey)
	{
		RegCloseKey(m_hSubKey);
		m_hSubKey = NULL;
	}
	if(ERROR_SUCCESS == RegCreateKeyEx (m_hRootKey, pszPath, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hSubKey, &dwTmp))
		return TRUE;

	return FALSE;
}


/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, DWORD dwVal)
* 功能描述:   写入DWORD值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, DWORD dwVal)
{
	return ::RegSetValueEx (m_hSubKey, lpszKeyName, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}


/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, LPCTSTR pszData)
* 功能描述:   写入字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, LPCTSTR pszData)
{
	if(ERROR_SUCCESS == RegSetValueEx (m_hSubKey, lpszKeyName, 0L, REG_SZ,
		(const BYTE*) pszData, (LONG)(_tcslen(pszData) + 1) * sizeof(TCHAR)))
	{
		return TRUE;
	}	
	return FALSE;
}
/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, BYTE *pbData, long nSiz)
* 功能描述:   写入二进制
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, const BYTE *pbData, DWORD dwSize)
{
	assert(NULL != pbData && dwSize > 0);	
	if(ERROR_SUCCESS == RegSetValueEx (m_hSubKey, lpszKeyName, 0L, REG_BINARY,
		(const BYTE*)pbData, dwSize))
	{
		return TRUE;
	}	
	return FALSE;
}
/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, DWORD& dwVal) 第2个参数通过引用传递，可以在函数中修改实参
* 功能描述:   读取DWORD值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, DWORD& dwVal)
{
	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;	

	if(ERROR_SUCCESS == RegQueryValueEx (m_hSubKey, (LPCTSTR) lpszKeyName, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize))
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}


/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, CString& sVal) 第2个参数通过引用传递，可以在函数中修改实参
* 功能描述:   读取字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, CXString& sVal)
{

	DWORD dwType;
	DWORD dwSize = REG_BufferMaxLen;
	TCHAR szString[REG_BufferMaxLen];

	if(ERROR_SUCCESS == RegQueryValueEx (m_hSubKey, lpszKeyName, NULL,
		&dwType, (BYTE *)szString, &dwSize))
	{
		sVal = szString;
		return TRUE;
	}
	return FALSE;
}

/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, BYTE *pbVal, long nBufferSize) 第2个参数通过指针传递，可以在函数中修改实参
* 功能描述:   读取字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, BYTE *pbData, DWORD dwSize)
{	
	assert(NULL != pbData && dwSize > 0);
	DWORD dwType;
	if(ERROR_SUCCESS == RegQueryValueEx (m_hSubKey, lpszKeyName, NULL,
		&dwType, pbData, &dwSize))
	{		
		return TRUE;
	}
	return FALSE;
}

/*================================================================ 
* 函数名：    DeleteValue
* 参数：      (LPCTSTR pszValue) 
* 功能描述:   删除值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::DeleteValue (LPCTSTR pszValue)
{
	if(ERROR_SUCCESS == ::RegDeleteValue(m_hSubKey, pszValue))		
		return TRUE;
	else
		return FALSE;
}

/*================================================================ 
* 函数名：    DeleteKey
* 参数：      (HKEY hRootKey, LPCTSTR pszPath) 
* 功能描述:   删除路径
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::DeleteKey (LPCTSTR pszPath)
{	
	assert(NULL != pszPath);
	BOOL blRet = TRUE;
	HKEY hOldSubKey;
	if (!pszPath)
	{
		blRet = FALSE;
		goto Exit_;
	}
	hOldSubKey = m_hSubKey;
	if(FALSE == VerifyKey(pszPath))
	{		
		blRet = FALSE;
		goto Exit_;
	}
	DeletaKeyTree(m_hSubKey);
	m_hSubKey = hOldSubKey;		
	if (ERROR_SUCCESS == RegDeleteKey(m_hRootKey, pszPath))
	{
		blRet = TRUE;		
	}
Exit_:
	return blRet;
}
BOOL CXReg::DeleteKey()
{	
	assert(NULL != m_hSubKey);
	BOOL blRet = FALSE;
	TCHAR szKeyPath[REG_BufferMaxLen];
	DWORD dwBufferLen = REG_BufferMaxLen;
	if (ERROR_SUCCESS != (blRet = RegQueryInfoKey(m_hSubKey, szKeyPath, &dwBufferLen, 
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)))
	{
		goto Exit_;
	}
	DeletaKeyTree(m_hSubKey);		
	if (ERROR_SUCCESS != RegDeleteKey(m_hRootKey, szKeyPath))
	{
		goto Exit_;	
	}
	blRet = TRUE;
	m_hSubKey = NULL;
Exit_:
	return blRet;
}

/*================================================================ 
* 函数名：    Close
* 参数：      
* 功能描述:   关闭注册表
* 返回值：    void
* 作 者：     
================================================================*/ 
void CXReg::Close()
{
	if (m_hSubKey)
	{
		::RegCloseKey (m_hSubKey);
		m_hSubKey = NULL;
	}
}
BOOL CXReg::RestoreKey(LPCTSTR lpFileName)
{
	assert(m_hSubKey);
	assert(lpFileName);
	BOOL blRet = TRUE;
	
	if(ERROR_SUCCESS != RegRestoreKey(m_hSubKey, lpFileName, REG_WHOLE_HIVE_VOLATILE))
	{
		blRet = FALSE;
		goto Err;
	}
Err:
	return blRet;
}

BOOL CXReg::SaveKey(LPCTSTR lpFileName)
{
	assert(m_hSubKey);
	assert(lpFileName);
	BOOL blRet = TRUE;
	if(ERROR_SUCCESS != (blRet = RegSaveKey(m_hSubKey, lpFileName, NULL)))
	{
		blRet = FALSE;
		goto Err;
	}
Err:	
	return blRet;
}
void  CXReg::DeletaKeyTree(HKEY hKey) 
{   
	DWORD   dwCount = 0;   
	TCHAR	szSubkeyName[128];   
	HKEY   hKeySub; 
    
	if(ERROR_SUCCESS != RegQueryInfoKey(hKey, 0, 0, 0, &dwCount, 0, 0, 0, 0, 0, 0, 0))   
	{   		
		return;   
	}   
	else if(dwCount)   
	{
		for(DWORD i = 0; i < dwCount; i ++)   
		{   		 
			if(ERROR_SUCCESS != RegEnumKey(hKey, 0, szSubkeyName, 128))   
			{   
				assert(0);  					
				return;   
			}   			
			if(ERROR_SUCCESS != RegOpenKey(hKey, szSubkeyName, &hKeySub))   
			{   
				assert(0);   					
				return;   
			}   
			DeletaKeyTree(hKeySub);//调用递归   			 
			if(ERROR_SUCCESS != RegDeleteKey(hKey, szSubkeyName))   
			{   
				assert(0);   					  
				return;   
			}   
			RegCloseKey(hKeySub);   
		}  
	}		
} 