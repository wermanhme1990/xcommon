#if !defined HWXUE_XREG_H
#define HWXUE_XREG_H

/************************************************************************ 
* 文件名：    HWXReg.h
* 文件描述：  注册表读写 
* 创建人：    
************************************************************************/ 
#include "HWXString.h"
class DLLXEXPORT CXReg
{
public:
	CXReg(HKEY hRootKey = HKEY_LOCAL_MACHINE); //构造函数带有默认参数
	virtual ~CXReg();

	BOOL VerifyValue(LPCTSTR, LPCTSTR);
	BOOL VerifyKey (LPCTSTR pszPath); 
	void Attach(HKEY hRootKey);	

	BOOL DeleteValue (LPCTSTR lpszKey, LPCTSTR pszValue);
	void DeleteKey (LPCTSTR pszPath);	

	BOOL Write (LPCTSTR lpszKey, LPCTSTR lpszValue, DWORD dwVal);
	BOOL Write (LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR pszVal);
	BOOL Write (LPCTSTR lpszKey, LPCTSTR lpszValue, const BYTE *pbVal, DWORD dwSize);

	BOOL Read (LPCTSTR lpszKey, LPCTSTR lpszValue, DWORD& dwVal);
	BOOL Read (LPCTSTR lpszKey, LPCTSTR lpszValue, CXString& sVal);
	BOOL Read (LPCTSTR lpszKey, LPCTSTR lpszValue, BYTE *&pbVal, DWORD &dwSize);

	BOOL IsEqual(LPCTSTR lpszKey, LPCTSTR pszValue,DWORD dw);
	BOOL IsEqual(LPCTSTR lpszKey, LPCTSTR pszValue,LPCTSTR lpsz);

	BOOL RestoreKey(LPCTSTR lpszKey, LPCTSTR lpFileName);
	BOOL SaveKey(LPCTSTR lpszKey, LPCTSTR lpFileName);
private:	
	void DeletaKeyTree(HKEY);
	HKEY OpenKey(LPCTSTR lpszKey, REGSAM = KEY_ALL_ACCESS);
	HKEY CreateKey (LPCTSTR lpszKey, REGSAM	= KEY_ALL_ACCESS);
protected:		
	HKEY    m_hRootKey;   //保存根键句柄
};

#endif
