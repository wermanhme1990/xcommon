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
CXReg::CXReg(HKEY hRootKey) : m_hRootKey(hRootKey)
{	
}
 /************************************************************************
 名称:	SetRootKey
 全称:	void CXReg::SetRootKey( HKEY hRootKey )
 类型:	public 
 返回值:	void: 
 功能:	 设置根键值
 参数:	HKEY hRootKey: 
 ************************************************************************/
void CXReg::Attach( HKEY hRootKey )
{
	assert(NULL != hRootKey);
	m_hRootKey = hRootKey;
}

CXReg::~CXReg() //在析构函数中关闭打开注册表句柄
{
	m_hRootKey = NULL;
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
	HKEY hKey = NULL;
	if(ERROR_SUCCESS != RegOpenKeyEx (m_hRootKey, pszPath, 0L, KEY_ALL_ACCESS, &hKey))
	{	
		blRet = FALSE;		
	}
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return blRet;
}




/*================================================================ 
* 函数名：    VerifyValue
* 参数：      (LPCTSTR pszValue)
* 功能描述:   判断指定的键名是否等于某个值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::IsEqual(LPCTSTR pszKey, LPCTSTR pszValue, DWORD dwValue)
{
	DWORD dwTemp;
	if (!Read(pszKey, pszValue, dwTemp))
	{
		return FALSE;
	}		
	return dwTemp == dwValue;
}
BOOL CXReg::IsEqual(LPCTSTR pszKey, LPCTSTR pszValue,LPCTSTR lpszString)
{
	CXStringT str;
	if (!Read(pszKey, pszValue, str))
	{
		return FALSE;
	}	
	return str.CompareNoCase(lpszString);	
}


/*================================================================ 
* 函数名：    CreateKey
* 参数：      (HKEY hRootKey, LPCTSTR pszPath)
* 功能描述:   创建路径
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
HKEY CXReg::CreateKey (LPCTSTR pszPath, REGSAM sam)
{
	HKEY hKey = NULL;
	DWORD dwTmp;		
	RegCreateKeyEx (m_hRootKey, pszPath, 0L, NULL, REG_OPTION_NON_VOLATILE, sam, 
		NULL, &hKey, &dwTmp);	
	return hKey;
}


/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, DWORD dwVal)
* 功能描述:   写入DWORD值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, LPCTSTR lpszValue, DWORD dwVal)
{
	BOOL blRet = FALSE;
	HKEY hKey = NULL;
	if (!lpszKeyName)
	{
		return FALSE;
	}
	VERIFY(hKey = CreateKey(lpszKeyName));	
	blRet = (ERROR_SUCCESS == ::RegSetValueEx (hKey, lpszValue, 0L, REG_DWORD,
		(const BYTE*) &dwVal, sizeof(DWORD)));
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return blRet;
}


/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, LPCTSTR pszData)
* 功能描述:   写入字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR pszData)
{
	BOOL blRet = FALSE;
	HKEY hKey = NULL;
	if (!lpszKeyName)
	{
		return FALSE;
	}
	VERIFY(hKey = CreateKey(lpszKeyName));
	blRet = (ERROR_SUCCESS == RegSetValueEx (hKey, lpszValue, 0L, REG_SZ, (const BYTE*) pszData,
		(LONG)(_tcslen(pszData) + 1) * sizeof(TCHAR)));	
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return blRet;
}
/*================================================================ 
* 函数名：    Write
* 参数：      (LPCTSTR lpszKeyName, BYTE *pbData, long nSiz)
* 功能描述:   写入二进制
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Write (LPCTSTR lpszKeyName, LPCTSTR pszValue, const BYTE *pbData, DWORD dwSize)
{
	BOOL blRet = FALSE;
	HKEY hKey = NULL;
	if (!lpszKeyName)
	{
		return FALSE;
	}
	VERIFY(hKey = CreateKey(lpszKeyName));
	blRet = ERROR_SUCCESS == RegSetValueEx (hKey, pszValue, 0L, REG_BINARY,	(const BYTE*)pbData,
		dwSize);
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return blRet;
}
/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, DWORD& dwVal) 第2个参数通过引用传递，可以在函数中修改实参
* 功能描述:   读取DWORD值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, LPCTSTR lpszValue, DWORD& dwVal)
{
	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;	
	HKEY hSubKey = NULL;
	BOOL blRet = FALSE;
	hSubKey = OpenKey(lpszKeyName, KEY_READ);
	if (!hSubKey)
	{
		return FALSE;
	}
	if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, lpszValue, NULL, &dwType, (BYTE *) &dwDest,
		&dwSize) && REG_DWORD == dwType)
	{
		dwVal = dwDest;
		blRet = TRUE;
	}
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	return blRet;
}


/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, CString& sVal) 第2个参数通过引用传递，可以在函数中修改实参
* 功能描述:   读取字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, LPCTSTR lpszValue, CXString& sVal)
{	
	DWORD dwType;
	DWORD dwSize = REG_BufferMaxLen;
	TCHAR szString[REG_BufferMaxLen];
	HKEY hSubKey = NULL;
	BOOL blRet = FALSE;
	hSubKey = OpenKey(lpszKeyName, KEY_READ);
	if (!hSubKey)
	{
		return FALSE;
	}
	if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, lpszValue, NULL, &dwType, (BYTE *)szString, 
		&dwSize) && REG_SZ == dwType)
	{
		sVal = szString;
		blRet = TRUE;
	}
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	return blRet;
}

/*================================================================ 
* 函数名：    Read
* 参数：      (LPCTSTR lpszKeyName, BYTE *pbVal, long nBufferSize) 第2个参数通过指针传递，可以在函数中修改实参
* 功能描述:   读取字符串值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::Read (LPCTSTR lpszKeyName, LPCTSTR lpszValue, BYTE *&pbData, DWORD& dwSize)
{	
	assert(NULL != pbData && dwSize > 0);
	if (!pbData)
	{
		return FALSE;
	}
	DWORD dwType;
	HKEY hSubKey = NULL;
	BOOL blRet = FALSE;
	dwSize = 0;
	hSubKey = OpenKey(lpszKeyName, KEY_READ);
	if (!hSubKey)
	{
		return FALSE;
	}
	if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, lpszValue, NULL,	&dwType, NULL, &dwSize) &&
		REG_BINARY == dwType)
	{		
		if (dwSize)
		{
			VERIFY(pbData = new BYTE[dwSize]);
			VERIFY(ERROR_SUCCESS == RegQueryValueEx (hSubKey, lpszValue, NULL,	&dwType, pbData, &dwSize));
		}
		blRet = TRUE;		
	}
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	return blRet;
}

/*================================================================ 
* 函数名：    DeleteValue
* 参数：      (LPCTSTR pszValue) 
* 功能描述:   删除值
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
BOOL CXReg::DeleteValue( LPCTSTR pszKey, LPCTSTR pszValue )
{
	BOOL blRet = TRUE;
	HKEY hSubKey = NULL;
	if (!(hSubKey = OpenKey(pszKey)))
	{
		return TRUE;
	}
	if(ERROR_SUCCESS != ::RegDeleteValue(hSubKey, pszValue))		
	{
		blRet = FALSE;
	}
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	return blRet;
}
/*================================================================ 
* 函数名：    DeleteKey
* 参数：      (HKEY hRootKey, LPCTSTR pszPath) 
* 功能描述:   删除路径
* 返回值：    BOOL
* 作 者：     
================================================================*/ 
void CXReg::DeleteKey (LPCTSTR pszPath)
{	
	assert(NULL != pszPath);
	BOOL blRet = TRUE;	
	if (!pszPath)
	{		
		return;
	}
	HKEY hSubKey = OpenKey(pszPath);
	if (hSubKey)
	{
		DeletaKeyTree(hSubKey);
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}		
	if (ERROR_SUCCESS == RegDeleteKey(m_hRootKey, pszPath))
	{
		;		
	}
	return;
}
/*================================================================ 
* 函数名：    Close
* 参数：      
* 功能描述:   关闭注册表
* 返回值：    void
* 作 者：     
================================================================*/ 

BOOL CXReg::RestoreKey(LPCTSTR lpszKey, LPCTSTR lpFileName)
{	
	assert(lpFileName);
	BOOL blRet = FALSE;
	HKEY hKey = NULL;
	if (!(hKey = OpenKey(lpszKey)))
	{
		return FALSE;
	}	
	if(ERROR_SUCCESS == RegRestoreKey(hKey, lpFileName, REG_WHOLE_HIVE_VOLATILE))
	{
		blRet = TRUE;		
	}
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return blRet;
}

BOOL CXReg::SaveKey(LPCTSTR lpszKey, LPCTSTR lpFileName)
{	
	assert(lpFileName);
	BOOL blRet = FALSE;
	HKEY hKey = NULL;
	if (!(hKey = OpenKey(lpszKey)))
	{
		return FALSE;
	}	
	if(ERROR_SUCCESS == (blRet = RegSaveKey(hKey, lpFileName, NULL)))
	{
		blRet = TRUE;		
	}
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}	
	return blRet;
}
void  CXReg::DeletaKeyTree(HKEY hKey) 
{   
	DWORD   dwCount = 0;   
	TCHAR	szSubkeyName[128];   
	HKEY   hKeySub; 
   
	if (!hKey)
	{
		return;
	}
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
HKEY CXReg::OpenKey( LPCTSTR pthPath, REGSAM samReg)
{
	HKEY hKey = NULL;
	if (!pthPath)
	{
		return hKey;
	}
	RegOpenKeyEx (m_hRootKey, pthPath, 0L, samReg, &hKey);		
	return hKey;
}
/*================================================================ 
* 函数名：    VerifyValue
* 参数：      (LPCTSTR pszValue)
* 功能描述:   判断给定的值是否存在 （请先调用VerifyKey，然后在使用该函数）
* 返回值：    BOOL
* 作 者：     
================================================================*/ 

BOOL CXReg::VerifyValue( LPCTSTR pthKey, LPCTSTR pthValue)
{
	BOOL blExist = FALSE;
	HKEY subKey = NULL;
	if (!pthKey || !pthValue)
	{
		blExist = FALSE;
		return blExist;
	}	
	if(ERROR_SUCCESS != RegOpenKeyEx (m_hRootKey, pthKey, 0L,
		KEY_ALL_ACCESS, &subKey))
	{	
		blExist = FALSE;
		return blExist;
	}
	if(ERROR_SUCCESS == RegQueryValueEx(subKey, pthValue, NULL,
		NULL, NULL, NULL))
	{
		blExist = FALSE;
		return blExist;
	}
	if (subKey)
	{
		RegCloseKey(subKey);   
		subKey = NULL;
	}
	return blExist;
}
