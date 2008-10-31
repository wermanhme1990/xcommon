/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRegistryManager.h					
文件内容:	    用于管理系统配置文件 支持注册表与ini同时
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/03/14
*********************************************************************/

#pragma  once
#include "Ext_Type.h"
#include "HWXString.h"
#include <afxtempl.h>
#include <vector>
using namespace std;

enum XRegMode
{
	Mode_INI = 0x1,
	Mode_REG = 0x2
};
class DLLXEXPORT CXRegistryManager
{	
public:
	CXRegistryManager(HKEY hKeyBase = HKEY_CURRENT_USER); 
	CXRegistryManager(HKEY hKeyBase, LPCTSTR = TEXT("Software\\Hanvon")); 
	CXRegistryManager(LPCTSTR);
	virtual ~CXRegistryManager();
protected:
	HKEY     m_hKeyBase;  // The default is HKEY_CURRENT_USER.
	HKEY		 m_hAppKey;		// SubKey	
	CXString m_strPath;		// if Mode_INI then Name of an .INI file if Mode_REG then Path of registry settings.
	LONG		 m_nMode;
public:	
	void SetMode(LONG);
	LONG GetMode();
	void Set(LPCTSTR strINIFileName, LONG = Mode_INI, HKEY hKeyBase = 0);
	CXString& GetPath();	
	
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, const int& nValue);	
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, const int& nDefault);

	BOOL WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, const LONG& nBytes);
	LPBYTE GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LONG& pBytes);

	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	CXString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);

	BOOL WriteProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, const POINT& Value);
	POINT GetProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, const POINT& Default);

	BOOL WriteProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, const RECT& Value);
	RECT GetProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, const RECT& Default);

	BOOL WriteProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, const SIZE& Value);
	SIZE GetProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, const SIZE& Default);

	BOOL WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, const double& Value);
	double GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, const double& Default);

	BOOL WriteProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, const DWORD& Value);
	DWORD GetProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, const DWORD& Default);

	BOOL WriteProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, const COLORREF& Value);
	COLORREF GetProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, const COLORREF& crDefault);	
	
	
	LONG EnumValues(LPCTSTR lpszSection, CMap<CXString, LPCTSTR, DWORD, DWORD&>*  mapItems, 
    vector<CXString>* arrayNames);
	LONG EnumKeys(LPCTSTR lpszSection, vector<CXString> & arrayKeys);
	BOOL DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey);
	BOOL DeleteValue(LPCTSTR lpszSection, LPCTSTR lpszKey);
	LONG GetErrorCode() const;
	CXString GetErrorMessage() const;

protected:	
	virtual HKEY GetSectionKey(LPCTSTR lpszSection, REGSAM samDesired = KEY_WRITE | KEY_READ);

private:	
	BOOL RecurseDeleteKey(HKEY hKey, LPCTSTR lpszKey);
	LONG m_lResult;
};

//////////////////////////////////////////////////////////////////////
