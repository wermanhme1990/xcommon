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

public:
	BOOL VerifyKey (LPCTSTR pszPath); 
	BOOL VerifyValue (LPCTSTR pszValue);
	void SetRootKey(HKEY hRootKey);	
	BOOL CreateKey (LPCTSTR pszPath);
	void Close();

	BOOL DeleteValue (LPCTSTR pszValue);
	BOOL DeleteKey (LPCTSTR pszPath);
	BOOL DeleteKey();

	BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);
	BOOL Write (LPCTSTR pszKey, const BYTE *pbVal, DWORD dwSize);

	BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	BOOL Read (LPCTSTR pszKey, CXString& sVal);
	BOOL Read (LPCTSTR pszKey, BYTE *pbVal, DWORD dwSize);

	BOOL IsEqual(LPCTSTR pszValue,int nn);
	BOOL IsEqual(LPCTSTR pszValue,DWORD dw);
	BOOL IsEqual(LPCTSTR pszValue,LPCTSTR lpsz);

	BOOL RestoreKey(LPCTSTR lpFileName);
	BOOL SaveKey(LPCTSTR lpFileName);
private:
	void DeletaKeyTree(HKEY);
protected:	
	HKEY 	  m_hSubKey;    //保存打开的子键句柄
	HKEY    m_hRootKey;   //保存根键句柄
};

#endif
