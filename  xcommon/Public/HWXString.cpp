/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   HWXString.h          
文件内容:    自定义String处理类
版本历史:    1.0
作者:        xuejuntao xuejuntao@hanwang.com.cn 2008/03/06
*********************************************************************/
#include "StdAfx.h"
#include "HWXString.h"
#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <Windows.h>
#include <AtlBase.h>
#include <AtlConv.h>

#define    HWXString_Len_Inc    (256)
const WCHAR g_whEndMark  = L'\0';
const CHAR  g_chEndMark  = '\0';
//////////////////////////////////////////////////////////////////////////
// CXStringW
//////////////////////////////////////////////////////////////////////////
CXStringW::CXStringW() 
{   
	Init();
	Clear();
}
CXStringW::CXStringW(LPCWSTR pwhSource) 
{ 
	Init();
	Reset(pwhSource);  
}
CXStringW::CXStringW(const WCHAR& whSource) 
{ 
	Init();
	WCHAR awhBuffer[2];
	awhBuffer[0] = whSource;
	awhBuffer[1] = g_whEndMark;
	Reset(awhBuffer);  
}
CXStringW::CXStringW(LPCWSTR pwhSource, LONG nLen) 
{ 	
	Init();
	Reset(pwhSource, nLen);  
}
CXStringW::CXStringW(const CXStringW &xstrSource) 
{   
	Init();
	Reset(xstrSource.C_Str());
}
//////////////////////////////////////////////////////////////////////////
void CXStringW::Init()
{	
	m_nLength = 0;
	m_nIncStep = HWXString_Len_Inc; 
	
	m_tPackage.SetIncStep(m_nIncStep * sizeof(WCHAR));	
	Reserve(m_nIncStep);	
}
//////////////////////////////////////////////////////////////////////////
CXStringW::~CXStringW()
{

#if defined(_DEBUG) || defined(DEBUG)
	{
		Clear();
	}
#endif  

}

void CXStringW::AssertValid() const
{
#if defined(_DEBUG) || defined(DEBUG)
	{
		assert(m_nLength >= 0);
		assert(C_Str());
		assert(wcslen(C_Str()) == m_nLength);	
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
LPCWSTR CXStringW::C_Str() const
{
	return (LPCWSTR)m_tPackage.GetData();  
}
LONG CXStringW::StrLen() const
{
#if defined(_DEBUG) || defined(DEBUG)
	{
		LPCWSTR pwhString = C_Str();
		LONG nLen = (LONG)wcslen(pwhString);
		//assert(nLen == m_nLength);
	}
#endif  
	return m_nLength; 
}
//////////////////////////////////////////////////////////////////////////
void CXStringW::Clear()
{
	LPWSTR pwhString = (LPWSTR)C_Str();
	assert(NULL != pwhString);
#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(pwhString, (StrLen() + 1) * sizeof(WCHAR));
	}
#endif  
	m_nLength = 0;
	pwhString[m_nLength] = g_whEndMark;  
}
//////////////////////////////////////////////////////////////////////////
void CXStringW::Reserve(LONG nLen)
{
	assert(nLen > 0);  
	VERIFY(NULL != m_tPackage.GetBuffer(nLen * sizeof(WCHAR)));
}
BOOL CXStringW::IsEmpty() const
{
	return 0 == m_nLength;
}
void CXStringW::Reverse()
{  
#if _MSC_VER > 1000
	_wcsrev((LPWSTR)C_Str());
#else
	wcsrev((LPWSTR)C_Str());
#endif
}
//定位
LONG CXStringW::FirstBelong(LPCWSTR pwhChrSet)
{
	assert(NULL != pwhChrSet);
	assert(wcslen(pwhChrSet) > 0);
	if (wcslen(pwhChrSet) > 0)
	{
		return (LONG)wcscspn((LPWSTR)C_Str(), pwhChrSet);
	}
	else
	{
		return 0;
	}  
}
LONG CXStringW::FirstNoBelong(LPCWSTR pwhChrSet)
{
	assert(NULL != pwhChrSet);
	assert(wcslen(pwhChrSet) > 0);
	if (wcslen(pwhChrSet) > 0)
	{
		return (LONG)wcsspn((LPWSTR)C_Str(), pwhChrSet);
	}
	else
	{
		return 0;
	}
}
BOOL CXStringW::IsBelong(LPCWSTR pwhChrSet)
{
	return m_nLength == FirstNoBelong(pwhChrSet);
}
BOOL CXStringW::IsNoBelong(LPCWSTR pwhChrSet)
{
	return m_nLength == FirstBelong(pwhChrSet);
}
WCHAR CXStringW::GetAt(LONG nIndex)
{
	LPWSTR pwhString = (LPWSTR)C_Str();
	return nIndex < m_nLength ? pwhString[nIndex] : WCHAR(-1);
}

WCHAR CXStringW::operator[](LONG nIndex)
{
	return GetAt(nIndex);
}

BOOL CXStringW::SetAt(LONG nIndex, WCHAR whB)
{
	LPWSTR pwhString = (LPWSTR)C_Str();
	assert(NULL != pwhString);
	if (nIndex < m_nLength)
	{
		pwhString[nIndex] = whB;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXStringW::Strcat(LPCWSTR pwhA)
{
	if (pwhA)
	{
		LONG nLen = (LONG)wcslen(pwhA); 
		Reserve(m_nLength + nLen + 1);    
		LPWSTR pwhOrgString = (LPWSTR)C_Str();
#if _MSC_VER > 1000
		wcscat_s(pwhOrgString, m_nLength + nLen + 1, pwhA);
#else
		wcscat(pwhOrgString, pwhA);
#endif
		m_nLength += nLen; 
#if defined(_DEBUG) || defined(DEBUG)
		{
			LONG nNewLen = (LONG)wcslen(pwhOrgString);
			assert(nNewLen == m_nLength);
		}
#endif
	}    
	return TRUE;
}
BOOL CXStringW::Strcat(LPCWSTR pwhA, LONG nDstLen)
{
	assert(nDstLen > 0);
	assert(NULL != pwhA);  
	LONG nDstRealLen = (LONG)wcslen(pwhA);  
	assert(nDstLen <= nDstRealLen);  
	Reserve(m_nLength + nDstLen + 1);    
	LPWSTR pwhOrgString = (LPWSTR)C_Str();  
	Mem_CopyMemory(pwhOrgString + m_nLength, pwhA, nDstLen * sizeof(WCHAR));  
	pwhOrgString[m_nLength + nDstLen] = g_whEndMark;  
	m_nLength += nDstLen;
	assert(m_nLength == wcslen(pwhOrgString));
	return TRUE;
}
BOOL CXStringW::Strcat(const CXStringW& strA)
{      
	return Strcat(strA.C_Str());
}    
void CXStringW::Reset(LPCWSTR pwhB)
{  
	Clear();
	Strcat(pwhB);
}
void CXStringW::Reset(LPCWSTR pwhA, LONG nLen)
{
	Clear();  
	Strcat(pwhA, nLen);
}
void CXStringW::Reset(const CXStringW &strB)
{
	Reset(strB.C_Str());
}

CXStringW& CXStringW::operator =(LPCWSTR pwhB)
{
	Reset(pwhB);
	return *this;
}

CXStringW& CXStringW::operator =(const CXStringW &xstrSource)
{
	Reset(xstrSource);
	return *this;
}

void CXStringW::MakeLower()
{
	LPWSTR ptchString = (LPWSTR)C_Str();
#if _MSC_VER >= 1400
	_wcslwr_s(ptchString, m_nLength);  
#else
	wcslwr(ptchString); 
#endif
}

void CXStringW::MakeUpper()
{
	LPWSTR ptchString = (LPWSTR)C_Str();  
#if _MSC_VER >= 1400
	_wcsupr_s(ptchString, m_nLength);
#else
	wcsupr(ptchString);
#endif
}

BOOL CXStringW::Compare(LPCWSTR pwhCompareString) const
{
	assert(NULL != pwhCompareString);
	return pwhCompareString ? (0 == wcscmp(C_Str(), pwhCompareString)) : FALSE;
}

BOOL CXStringW::Compare(const CXStringW &xstrCompareString) const
{
	return Compare(xstrCompareString.C_Str());
}

BOOL CXStringW::CompareNoCase(LPCWSTR pwhCompareString) const
{
	assert(NULL != pwhCompareString);  
#if _MSC_VER >= 1400
	return pwhCompareString ? (0 == _wcsicmp(C_Str(), pwhCompareString)) : FALSE;
#else
	return pwhCompareString ? (0 == wcsicmp(C_Str(), pwhCompareString)) : FALSE;
#endif
}

BOOL CXStringW::CompareNoCase(const CXStringW &xstrA) const
{
	return CompareNoCase(xstrA.C_Str());
}

BOOL CXStringW::operator==(const CXStringW &xstrA) const
{
	return Compare(xstrA);
}
BOOL CXStringW::operator!=(const CXStringW &xstrA) const
{
	return !Compare(xstrA);
}
BOOL CXStringW::operator==(LPCWSTR pwhData) const
{
	return Compare(pwhData);
}
BOOL CXStringW::operator!=(LPCWSTR pwhData) const
{
	return !(Compare(pwhData));
}
BOOL CXStringW::operator==(const WCHAR whA) const
{
	WCHAR awhTmp[2];
	awhTmp[0] = whA;
	awhTmp[1] = g_whEndMark;
	return Compare(awhTmp);
}
BOOL CXStringW::operator!=(const WCHAR whA) const
{
	return !(*this == whA);
}
LONG CXStringW::Find(LONG nStartIndex, LPCWSTR pwhStringToFind) 
{
	LONG nLen = StrLen();
	assert(NULL != pwhStringToFind);
	assert(nStartIndex >= 0 && nStartIndex < nLen);
	LPWSTR pwhString;
	if (nStartIndex < 0)
	{
		nStartIndex = 0;
	}
	else if(nStartIndex > nLen)
	{
		nStartIndex = nLen;
	}
	pwhString = (LPWSTR)C_Str();
	LPWSTR ptchResult = wcsstr(pwhString + nStartIndex, pwhStringToFind);

	return NULL != ptchResult ? (LONG)(ptchResult - pwhString) : -1;
}
LONG CXStringW::Find(LPCWSTR pwhStringToFind) 
{
	return Find(0, pwhStringToFind);
}
LONG CXStringW::Find(LONG nStartIndex, WCHAR whChrToFind)
{
	WCHAR awhBuffer[2];
	awhBuffer[0] = whChrToFind;
	awhBuffer[1] = g_whEndMark;  
	return Find(nStartIndex, awhBuffer);
}
LONG CXStringW::Find(const WCHAR whChrToFind)
{
	return Find(0, whChrToFind);
}
LONG CXStringW::FindNoCase(LONG nStartIndex, LPCWSTR pwhStringToFind)
{  
	CXStringW xstrTempLower = *this;
	xstrTempLower.MakeLower();
	CXStringW xstrStringToFindLower = pwhStringToFind;
	xstrStringToFindLower.MakeLower();  
	return xstrTempLower.Find(nStartIndex, xstrStringToFindLower.C_Str());
}
LONG CXStringW::FindNoCase(LPCWSTR pwhStringToFind)
{  
	return FindNoCase(0, pwhStringToFind);
}
LONG CXStringW::FindNoCase(LONG nStartIndex, const WCHAR whChrToFind)
{
	WCHAR awhTemp[2];
	awhTemp[0] = whChrToFind;
	awhTemp[1] = g_whEndMark;  
	return FindNoCase(nStartIndex, awhTemp);
}
LONG CXStringW::FindNoCase(const WCHAR whChrToFind)
{
	return FindNoCase(0, whChrToFind);
}
LONG CXStringW::ReverseFind(LONG nStartIndex, const WCHAR whChr)
{
	LPWSTR pwhResult = NULL, pwhString = (LPWSTR)C_Str();
	assert(nStartIndex >= 0 && nStartIndex < m_nLength);

	pwhResult = wcsrchr(pwhString + nStartIndex, whChr);
	return NULL != pwhResult ? (LONG)(pwhResult - pwhString) : -1; 
}
LONG CXStringW::ReverseFind(const WCHAR whChr)
{
	return ReverseFind(0, whChr);
}
CXStringW CXStringW::Left(LONG nCount)
{  
	LPWSTR pwhString = (LPWSTR)C_Str();    
	assert(NULL != pwhString);
	assert(nCount > 0 && nCount <= m_nLength);  
	CXStringW xstrTmp(pwhString, nCount);    

	return xstrTmp;
}

CXStringW CXStringW::ExcludeLeft(LONG nCount)
{
	return Right(m_nLength - nCount);
}

CXStringW CXStringW::Mid(LONG nIndex, LONG nCount)
{
	assert(nCount >= 0);

	LPWSTR pwhString = (LPWSTR)C_Str();    
	assert(nIndex >= 0 && nIndex <= m_nLength - nCount && nCount > 0);
	CXStringW xstrData(pwhString + nIndex, nCount);

	return xstrData;
}

CXStringW CXStringW::Right(LONG nCount)
{
	return Mid(m_nLength - nCount, nCount);
}

void CXStringW::InternalLeft(LONG nCount)
{
	InternalMid(0, nCount);
}

void CXStringW::InternalExcludeLeft(LONG nCount)
{
	InternalRight(m_nLength - nCount);
}

void CXStringW::InternalMid(LONG nIndex, LONG nCount)
{
	LPWSTR pwhString = (LPWSTR)C_Str();  
	assert(nIndex >= 0 && nIndex + nCount <= m_nLength && nCount >= 0);
	if (0 != nIndex && nCount > 0)
	{
		Mem_CopyMemory(pwhString, pwhString + nIndex, nCount * sizeof(WCHAR));
	}  
	pwhString[nCount] = g_whEndMark;  
#if defined(_DEBUG) || defined(DEBUG)
	{
		if (nIndex + nCount < m_nLength)
		{
			Mem_ZeroMemory(pwhString + nCount, (m_nLength - nCount) * sizeof(WCHAR));
		}
	}  
#endif
	m_nLength = nCount;
}

void CXStringW::InternalRight(LONG nCount)
{
	InternalMid(m_nLength - nCount, nCount);
}

CXStringW::operator LPCWSTR() const
{ 
	return (LPCWSTR)C_Str(); 
}
CXStringW &CXStringW::operator+=(const CXStringW &xstrAdditionalString)
{
	Strcat(xstrAdditionalString);  
	return *this;
}

CXStringW &CXStringW::operator+=(LPCWSTR pwhAdditionalString)
{
	Strcat(pwhAdditionalString);
	return *this;
}

CXStringW &CXStringW::operator+=(const WCHAR& whAdditionalChar)
{
	WCHAR awhAdditionalString[2];
	awhAdditionalString[0] = whAdditionalChar;
	awhAdditionalString[1] = g_whEndMark;

	Strcat(awhAdditionalString);
	return *this;
}

CXStringW CXStringW::operator+(const CXStringW &ptchAdditionalString)
{
	CXStringW xstrTmp = *this;
	xstrTmp.Strcat(ptchAdditionalString);
	return xstrTmp;
}

CXStringW CXStringW::operator+(LPCWSTR pwhAdditionalString)
{
	CXStringW xstrTmp = *this;
	xstrTmp.Strcat(pwhAdditionalString);
	return xstrTmp;
}

CXStringW CXStringW::operator+(const WCHAR &whAdditionalChar)
{
	CXStringW xstrTmp = *this;
	xstrTmp += whAdditionalChar;
	return xstrTmp;
}

CXStringW operator+(LPCWSTR pwhA, const CXStringW &xstrB)
{
	CXStringW xstrTmp = pwhA;
	xstrTmp += xstrB;
	return xstrTmp;
}
CXStringW operator+(const WCHAR &whAdditionalChar, const CXStringW& xstrB)
{
	CXStringW xstrTmp(whAdditionalChar);
	xstrTmp += xstrB;
	return xstrTmp;
}
//////////////////////////////////////////////////////////////////////////
BOOL operator!=(LPCWSTR pwhA, const CXStringW &xstrB)
{    
	return !(xstrB == pwhA);
}

BOOL operator==(LPCWSTR pwhA, const CXStringW &xstrB)
{    
	return xstrB == pwhA;
}
BOOL operator!=(const WCHAR whA, const CXStringW &xstrB)
{    
	return !(xstrB == whA);
}
BOOL operator==(const WCHAR whA, const CXStringW &xstrB)
{
	return xstrB == whA;
}
//////////////////////////////////////////////////////////////////////////
void CXStringW::TrimLeft(const WCHAR whA)
{
	WCHAR awhTag[2];
	awhTag[0] = whA;
	awhTag[1] = g_whEndMark;
	TrimLeft(awhTag);
}
void CXStringW::TrimLeft(LPCWSTR pwhTag)
{
	//assert(NULL != pwhTag);
	if (IsEmpty())
	{
		return;
	}   
	LPWSTR pwhString = (LPWSTR)C_Str();
	assert(m_nLength > 0);
	LONG nAmount = 0;  
	if (!pwhTag)
	{
		pwhTag = g_awhDelimit;
	}
	nAmount = (LONG)wcsspn(pwhString, pwhTag);
	assert(nAmount <= m_nLength);
	InternalExcludeLeft(nAmount);
}
void CXStringW::TrimRight(const WCHAR whA)
{
	WCHAR awhTag[2];
	awhTag[0] = whA;
	awhTag[1] = g_whEndMark;
	TrimRight(awhTag);
}
void CXStringW::TrimRight(LPCWSTR pwhTag)
{
	if (IsEmpty())
	{
		return;
	}    
	LONG nAmount = 0;
	BOOL blContinue = TRUE;
	if (NULL == pwhTag)
	{
		pwhTag = g_awhDelimit;
	}

	for (LONG i = 0; i < m_nLength && blContinue; i++)
	{    
		if (NULL != wcschr(pwhTag, GetAt(m_nLength - i - 1)))
			nAmount ++;
		else
			blContinue = FALSE;
	}  
	if (!blContinue || nAmount > 0)
		InternalLeft(m_nLength - nAmount);
}
void CXStringW::Trim(const WCHAR whA)
{
	WCHAR awhTag[2];
	awhTag[0] = whA;
	awhTag[1] = g_whEndMark;
	Trim(awhTag);
}
void CXStringW::Trim(LPCWSTR pwhTag)
{
	TrimLeft(pwhTag);
	TrimRight(pwhTag);
}
LONG CXStringW::GetNumber()
{
	if (IsEmpty())
		return 0L;

	LONG nValue = 0;
	LPCWSTR pwhIndex = wcspbrk(C_Str(), L"0123456789.-+");
	if (pwhIndex) 
	{
#if _MSC_VER >= 1000
		swscanf_s(pwhIndex, L"%ld", &nValue); 
#else
		swscanf(pwhIndex, L"%ld", &nValue);
#endif
	}
	return nValue;
}

float CXStringW::GetFloat()
{
	if (IsEmpty())
		return 0.0f;

	float flValue = 0.0;
	LPCWSTR pwhIndex = wcspbrk(C_Str(), L"0123456789.-+");
	if (pwhIndex) 
	{
#if _MSC_VER >= 1000
		swscanf_s(pwhIndex, L"%f", &flValue); 
#else
		swscanf(pwhIndex, L"%f", &flValue); 
#endif
	}
	return flValue;
}

LONG CXStringW::GetHex()
{
	if (IsEmpty())
		return 0;

	LONG nValue = 0;
	LPCWSTR pwhIndex = wcspbrk(C_Str(), L"0123456789ABCDEFabcdef-+");
	if (pwhIndex) 
	{
#if _MSC_VER >= 1000
		wscanf_s(pwhIndex, L"%x", &nValue); 
#else
		wscanf(pwhIndex, L"%x", &nValue);
#endif
	}
	return nValue;
}
void CXStringW::ReplaceChar(const WCHAR A, const WCHAR B)
{ 
	LPWSTR strString = (LPWSTR)C_Str();
	assert(strString);
	if (!strString)
	{
		return;
	}	
	assert(wcslen(strString) == m_nLength);	
	while (*strString)
	{
		if (A == *strString)
		{
			*strString = B;
		}    		
		strString ++;
	}  
}

void CXStringW::ReplaceCharWithString(const WCHAR whA, LPCWSTR pwhStringB)
{
	if (IsEmpty())
		return;

	CXStringW xstrData;  
	LONG nSearchIndex = 0;
	LONG nFindIndex;
	LPWSTR pwhString = (LPWSTR)C_Str();
	while (nSearchIndex < m_nLength && (nFindIndex = Find(nSearchIndex, whA)) > -1)
	{      
		//assert (nFindIndex > nSearchIndex);
		//xstrData += Mid(nSearchIndex, nFindIndex - nSearchIndex);  
		if (nFindIndex != nSearchIndex)
		{
			xstrData.Strcat(pwhString + nSearchIndex, nFindIndex - nSearchIndex);
		}    
		xstrData += pwhStringB;

		nSearchIndex = ++ nFindIndex;     
	}
	//处理尾巴
	if (nSearchIndex < m_nLength)
		xstrData += pwhString + nSearchIndex;

	*this = xstrData;
}

void CXStringW::ReplaceCharWithString(const WCHAR whA, const CXStringW &xstrB)
{
	ReplaceCharWithString(whA, xstrB.C_Str());
}

void CXStringW::Format(LPCWSTR pwhFormat, ...) 
{  
	LPWSTR pwhBuffer = NULL;
	LONG nLen = 0;
	va_list argList;
	va_start(argList, pwhFormat);
	nLen = _vscwprintf(pwhFormat, argList);
	Reserve(nLen + 1);
	Clear();
	pwhBuffer = (LPWSTR)C_Str();
#if _MSC_VER >= 1000
	vswprintf_s(pwhBuffer, nLen + 1, pwhFormat, argList);  
#else
	_vswprintf(pwhBuffer, pwhFormat, argList);  
#endif
	va_end(argList);
	m_nLength = (LONG)wcslen(pwhBuffer);
}

void CXStringW::FormatEx(LPCWSTR pwhFormat, ...) 
{  
	LPWSTR pwhBuffer = NULL;
	va_list argList;
	LONG nLen = 0, nLenOrg = m_nLength;
	va_start(argList, pwhFormat);
	nLen = _vscwprintf(pwhFormat, argList);
	Reserve(nLen + nLenOrg + 1);
	pwhBuffer = (LPWSTR)C_Str();
#if _MSC_VER >= 1000
	vswprintf_s(pwhBuffer + nLenOrg, nLen, pwhFormat, argList); 
#else
	_vswprintf(pwhBuffer + nLenOrg, pwhFormat, argList); 
#endif
	va_end(argList);  
	m_nLength = (LONG)wcslen(pwhBuffer);
}

BOOL CXStringW::Token(LPCWSTR pwhDelimit, std::vector<CXStringW> &vString) const
{
	assert(pwhDelimit);
	BOOL blRet = TRUE;
	if (wcslen(pwhDelimit) < 1)
	{
		blRet = FALSE;
		return blRet;
	}
	CXStringW strTmp = *this;
	CXStringW strSubStr;
	LPCWSTR pwhToken = NULL;
	LPWSTR ptStr = (LPWSTR)strTmp.C_Str();
#if _MSC_VER >= 1000
	LPWSTR pwhContent = NULL;
	pwhToken = wcstok_s( ptStr, pwhDelimit, &pwhContent);
	while( NULL != pwhToken )
	{ 
		strSubStr = pwhToken;
		vString.push_back(strSubStr); 
		pwhToken = wcstok_s( NULL, pwhDelimit, &pwhContent);
	}  
#else
	pwhToken = wcstok( ptStr, pwhDelimit);
	while( NULL != pwhToken )
	{ 
		strSubStr = pwhToken;
		vString.push_back(strSubStr); 
		pwhToken = wcstok( NULL, pwhDelimit );
	} 
#endif
	return blRet;
}
BOOL CXStringW::Token(const WCHAR whDelimit, std::vector<CXStringW> &vString) const
{
	WCHAR awhDelimit[2];
	awhDelimit[0] = whDelimit;
	awhDelimit[1] = g_whEndMark;
	return Token(awhDelimit, vString);
}
CXStringW CXStringW::From( LPCSTR pchA)
{
	USES_CONVERSION;
	assert(pchA);  
	LPWSTR pwhB = A2W(pchA);
	return CXStringW(pwhB);   
}
CXStringW CXStringW::From( LPCWSTR pwhA)
{
	return CXStringW(pwhA);
}
CXStringW CXStringW::From( LONG nData)
{
	WCHAR awhBuffer[50];
#if _MSC_VER >= 1000
	swprintf_s(awhBuffer, 50, L"%d", nData);
#else
	swprintf(awhBuffer, L"%d", nData);
#endif
	return awhBuffer;
}
CXStringW CXStringW::From( DWORD dwData)
{
	WCHAR awhBuffer[50];
#if _MSC_VER >= 1000
	swprintf_s(awhBuffer, 50, L"%x", dwData);
#else
	swprintf(awhBuffer, L"%x", dwData);
#endif
	return awhBuffer;
}
CXStringW CXStringW::From( double dbData)
{
	WCHAR awhBuffer[50];
#if _MSC_VER >= 1000
	swprintf_s(awhBuffer, 50, L"%f", dbData);
#else
	swprintf(awhBuffer, L"%f", dbData);
#endif
	return awhBuffer;
}
LPWSTR CXStringW::GetBuffer( const DWORD& dwSize)
{
	Reserve(sizeof(WCHAR) * (dwSize + 1));
	return LPWSTR(m_tPackage.GetData());
}
void CXStringW::ReleaseBuffer()
{
	m_nLength = (LONG)wcslen(C_Str());
}
void CXStringW::Remove( const WCHAR chA)
{
	if (m_nLength <= 0)
	{
		return;
	}
	LPWSTR strBuff = (LPWSTR)C_Str();	
	LPCWSTR strIndex = strBuff;

	assert((LONG)wcslen(strBuff) == m_nLength);
	while(*strIndex != g_chEndMark)
	{
		if (*strIndex != chA)
		{
			*strBuff ++	= *strIndex;
		}
		strIndex ++;
	}
	*strBuff = g_whEndMark;
	m_nLength = (LONG)wcslen(C_Str());
}
std::ostream &operator<<(std::ostream & oStream, const CXStringW &xStr)
{
	oStream<<xStr.C_Str();
	return oStream;
}


//////////////////////////////////////////////////////////////////////////
// CXStringA
//////////////////////////////////////////////////////////////////////////
CXStringA::CXStringA() 
{   
	Init();
	Clear();
}
CXStringA::CXStringA(LPCSTR pwhSource) 
{ 
	Init();
	Reset(pwhSource);  
}
CXStringA::CXStringA(const CHAR& chSource) 
{ 
	Init();
	CHAR achBuffer[2];
	achBuffer[0] = chSource;
	achBuffer[1] = g_chEndMark;
	Reset(achBuffer);  
}
CXStringA::CXStringA(LPCSTR pwhSource, LONG nLen) 
{ 
	Init();
	Reset(pwhSource, nLen);  
}
CXStringA::CXStringA(const CXStringA &xstrSource) 
{   
	Init();
	Reset(xstrSource.C_Str());
}
//////////////////////////////////////////////////////////////////////////
void CXStringA::Init()
{
	m_nLength = 0;
	m_nIncStep = HWXString_Len_Inc;  
	m_tPackage.SetIncStep(m_nIncStep * sizeof(CHAR));
	Reserve(m_nIncStep);
}
//////////////////////////////////////////////////////////////////////////
CXStringA::~CXStringA()
{
#if defined(_DEBUG) || defined(DEBUG)
	{
		Clear();
	}
#endif  
}
void CXStringA::AssertValid() const
{
#if defined(_DEBUG) || defined(DEBUG)
	{
		assert(m_nLength >= 0);
		assert(C_Str());
		assert(strlen(C_Str()) == m_nLength);	
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
LPCSTR CXStringA::C_Str() const
{
	return (LPCSTR)m_tPackage.GetData();  
}
LONG CXStringA::StrLen() const
{
#ifdef _DEBUG
	{
		LPCSTR pwhString = C_Str();
		LONG nLen = (LONG)strlen(pwhString);
		assert(nLen == m_nLength);
	}
#endif  
	return m_nLength; 
}
//////////////////////////////////////////////////////////////////////////
void CXStringA::Clear()
{
	LPSTR pchString = (LPSTR)C_Str();
	assert(NULL != pchString);
#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(pchString, (StrLen() + 1) * sizeof(CHAR));
	}
#endif  
	m_nLength = 0;
	pchString[m_nLength] = g_chEndMark;  
}
//////////////////////////////////////////////////////////////////////////
void CXStringA::Reserve(LONG nLen)
{
	assert(nLen > 0);  
	VERIFY(NULL != m_tPackage.GetBuffer(nLen * sizeof(CHAR)));
}
BOOL CXStringA::IsEmpty() const
{
	return 0 == m_nLength;
}
void CXStringA::Reverse()
{  
#if _MSC_VER >= 1400
	_strrev((LPSTR)C_Str());
#else
	strrev((LPSTR)C_Str());
#endif
}
//定位
LONG CXStringA::FirstBelong(LPCSTR pchChrSet)
{
	assert(NULL != pchChrSet);
	assert(strlen(pchChrSet) > 0);
	if (strlen(pchChrSet) > 0)
	{
		return (LONG)strcspn((LPSTR)C_Str(), pchChrSet);
	}
	else
	{
		return 0;
	}  
}
LONG CXStringA::FirstNoBelong(LPCSTR pchChrSet)
{
	assert(NULL != pchChrSet);
	assert(strlen(pchChrSet) > 0);
	if (strlen(pchChrSet) > 0)
	{
		return (LONG)strspn((LPSTR)C_Str(), pchChrSet);
	}
	else
	{
		return 0;
	}
}
BOOL CXStringA::IsBelong(LPCSTR pchChrSet)
{
	return m_nLength == FirstNoBelong(pchChrSet);
}
BOOL CXStringA::IsNoBelong(LPCSTR pchChrSet)
{
	return m_nLength == FirstBelong(pchChrSet);
}
CHAR CXStringA::GetAt(LONG nIndex)
{
	LPSTR pwhString = (LPSTR)C_Str();
	return nIndex < m_nLength ? pwhString[nIndex] : CHAR(-1);
}

CHAR CXStringA::operator[](LONG nIndex)
{
	return GetAt(nIndex);
}

BOOL CXStringA::SetAt(LONG nIndex, CHAR chB)
{
	LPSTR pchString = (LPSTR)C_Str();
	assert(NULL != pchString);
	if (nIndex < m_nLength)
	{
		pchString[nIndex] = chB;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXStringA::Strcat(LPCSTR pchA)
{
	if (pchA)
	{
		LONG nLen = (LONG)strlen(pchA);  
		Reserve(m_nLength + nLen + 1);    
		LPSTR pwhOrgString = (LPSTR)C_Str();
#if _MSC_VER >= 1000
		strcat_s(pwhOrgString, m_nLength + nLen + 1, pchA);
#else
		strcat(pwhOrgString, pchA);
#endif
		m_nLength += nLen; 
#if defined(_DEBUG) || defined(DEBUG)
		{
			LONG nNewLen = (LONG)strlen(pwhOrgString);
			assert(nNewLen == m_nLength);
		}
#endif 
	}   
	return TRUE;
}
BOOL CXStringA::Strcat(LPCSTR pwhA, LONG nDstLen)
{
	assert(nDstLen > 0);
	assert(NULL != pwhA);  
	LONG nDstRealLen = (LONG)strlen(pwhA);  
	assert(nDstLen <= nDstRealLen);  
	Reserve(m_nLength + nDstLen + 1);    
	LPSTR pwhOrgString = (LPSTR)C_Str();  
	Mem_CopyMemory(pwhOrgString + m_nLength, pwhA, nDstLen * sizeof(CHAR));  
	pwhOrgString[m_nLength + nDstLen] = g_chEndMark;  
	m_nLength += nDstLen;
	assert(m_nLength == strlen(pwhOrgString));
	return TRUE;
}
BOOL CXStringA::Strcat(const CXStringA& strA)
{      
	return Strcat(strA.C_Str());
}    
void CXStringA::Reset(LPCSTR pchB)
{  
	Clear();
	Strcat(pchB);
}
void CXStringA::Reset(LPCSTR pchA, LONG nLen)
{
	Clear();  
	Strcat(pchA, nLen);
}
void CXStringA::Reset(const CXStringA &strB)
{
	Reset(strB.C_Str());
}

CXStringA& CXStringA::operator =(LPCSTR pchB)
{
	Reset(pchB);
	return *this;
}

CXStringA& CXStringA::operator =(const CXStringA &xstrSource)
{
	Reset(xstrSource);
	return *this;
}

void CXStringA::MakeLower()
{
	LPSTR ptchString = (LPSTR)C_Str();
#if _MSC_VER > 1000
	_strlwr_s(ptchString, m_nLength); 
#else
	strlwr(ptchString, m_nLength); 
#endif
}

void CXStringA::MakeUpper()
{
	LPSTR ptchString = (LPSTR)C_Str();  
#if _MSC_VER > 1000
	_strupr_s(ptchString, m_nLength);
#else
	strupr(ptchString);
#endif
}

BOOL CXStringA::Compare(LPCSTR pchCompareString) const
{
	assert(NULL != pchCompareString);
	return pchCompareString ? (0 == strcmp(C_Str(), pchCompareString)) : FALSE;
}

BOOL CXStringA::Compare(const CXStringA &xstrCompareString) const
{
	return Compare(xstrCompareString.C_Str());
}

BOOL CXStringA::CompareNoCase(LPCSTR pchCompareString) const
{
	assert(NULL != pchCompareString);  
#if _MSC_VER >= 1400
	return pchCompareString ? (0 == _stricmp(C_Str(), pchCompareString)) : FALSE;
#else
	return pchCompareString ? (0 == stricmp(C_Str(), pchCompareString)) : FALSE;
#endif
}

BOOL CXStringA::CompareNoCase(const CXStringA &xstrA) const
{
	return CompareNoCase(xstrA.C_Str());
}

BOOL CXStringA::operator==(const CXStringA &xstrA) const
{
	return Compare(xstrA);
}
BOOL CXStringA::operator!=(const CXStringA &xstrA) const
{
	return !Compare(xstrA);
}
BOOL CXStringA::operator==(LPCSTR pchData) const
{
	return Compare(pchData);
}
BOOL CXStringA::operator!=(LPCSTR pwhData) const
{
	return !(Compare(pwhData));
}
BOOL CXStringA::operator==(const CHAR chA) const
{
	CHAR achTmp[2];
	achTmp[0] = chA;
	achTmp[1] = g_chEndMark;
	return Compare(achTmp);
}
BOOL CXStringA::operator!=(const CHAR chA) const
{
	return !(*this == chA);
}
LONG CXStringA::Find(LONG nStartIndex, LPCSTR pchStringToFind) 
{
	LONG nLen = StrLen();
	assert(NULL != pchStringToFind);
	assert(nStartIndex >= 0 && nStartIndex < nLen);
	LPSTR pchString;
	if (nStartIndex < 0)
	{
		nStartIndex = 0;
	}
	else if(nStartIndex > nLen)
	{
		nStartIndex = nLen;
	}
	pchString = (LPSTR)C_Str();
	LPSTR ptchResult = strstr(pchString + nStartIndex, pchStringToFind);

	return NULL != ptchResult ? (LONG)(ptchResult - pchString) : -1;
}
LONG CXStringA::Find(LPCSTR pchStringToFind) 
{
	return Find(0, pchStringToFind);
}
LONG CXStringA::Find(LONG nStartIndex, CHAR chChrToFind)
{
	CHAR achBuffer[2];
	achBuffer[0] = chChrToFind;
	achBuffer[1] = g_chEndMark;  
	return Find(nStartIndex, achBuffer);
}
LONG CXStringA::Find(const CHAR chChrToFind)
{
	return Find(0, chChrToFind);
}
LONG CXStringA::FindNoCase(LONG nStartIndex, LPCSTR pwhStringToFind)
{  
	CXStringA xstrTempLower = *this;
	xstrTempLower.MakeLower();
	CXStringA xstrStringToFindLower = pwhStringToFind;
	xstrStringToFindLower.MakeLower();  
	return xstrTempLower.Find(nStartIndex, xstrStringToFindLower.C_Str());
}
LONG CXStringA::FindNoCase(LPCSTR pchStringToFind)
{  
	return FindNoCase(0, pchStringToFind);
}
LONG CXStringA::FindNoCase(LONG nStartIndex, const CHAR chChrToFind)
{
	CHAR achTemp[2];
	achTemp[0] = chChrToFind;
	achTemp[1] = g_chEndMark;  
	return FindNoCase(nStartIndex, achTemp);
}
LONG CXStringA::FindNoCase(const CHAR whChrToFind)
{
	return FindNoCase(0, whChrToFind);
}
LONG CXStringA::ReverseFind(LONG nStartIndex, const CHAR chChr)
{
	LPSTR pwhResult = NULL, pwhString = (LPSTR)C_Str();
	assert(nStartIndex >= 0 && nStartIndex < m_nLength);

	pwhResult = strrchr(pwhString + nStartIndex, chChr);
	return NULL != pwhResult ? (LONG)(pwhResult - pwhString) : -1; 
}
LONG CXStringA::ReverseFind(const CHAR chChr)
{
	return ReverseFind(0, chChr);
}
CXStringA CXStringA::Left(LONG nCount)
{  
	LPSTR pchString = (LPSTR)C_Str();    
	assert(NULL != pchString);
	assert(nCount > 0 && nCount <= m_nLength);  
	CXStringA xstrTmp(pchString, nCount);    

	return xstrTmp;
}

CXStringA CXStringA::ExcludeLeft(LONG nCount)
{
	return Right(m_nLength - nCount);
}

CXStringA CXStringA::Mid(LONG nIndex, LONG nCount)
{
	assert(nCount >= 0);

	LPSTR pchString = (LPSTR)C_Str();    
	assert(nIndex >= 0 && nIndex <= m_nLength - nCount && nCount > 0);
	CXStringA xstrData(pchString + nIndex, nCount);

	return xstrData;
}

CXStringA CXStringA::Right(LONG nCount)
{
	return Mid(m_nLength - nCount, nCount);
}

void CXStringA::InternalLeft(LONG nCount)
{
	InternalMid(0, nCount);
}

void CXStringA::InternalExcludeLeft(LONG nCount)
{
	InternalRight(m_nLength - nCount);
}

void CXStringA::InternalMid(LONG nIndex, LONG nCount)
{
	LPSTR pwhString = (LPSTR)C_Str();  
	assert(nIndex >= 0 && nIndex + nCount <= m_nLength && nCount >= 0);
	if (0 != nIndex && nCount > 0)
	{
		Mem_CopyMemory(pwhString, pwhString + nIndex, nCount * sizeof(CHAR));
	}  
	pwhString[nCount] = g_whEndMark;  
#if defined(_DEBUG) || defined(DEBUG)
	{
		if (nIndex + nCount < m_nLength)
		{
			Mem_ZeroMemory(pwhString + nCount, (m_nLength - nCount) * sizeof(CHAR));
		}
	}  
#endif
	m_nLength = nCount;
}

void CXStringA::InternalRight(LONG nCount)
{
	InternalMid(m_nLength - nCount, nCount);
}

CXStringA::operator LPCSTR() const
{ 
	return (LPCSTR)C_Str(); 
}
CXStringA &CXStringA::operator+=(const CXStringA &xstrAdditionalString)
{
	Strcat(xstrAdditionalString);  
	return *this;
}

CXStringA &CXStringA::operator+=(LPCSTR pchAdditionalString)
{
	Strcat(pchAdditionalString);
	return *this;
}

CXStringA &CXStringA::operator+=(const CHAR& chAdditionalChar)
{
	CHAR awhAdditionalString[2];
	awhAdditionalString[0] = chAdditionalChar;
	awhAdditionalString[1] = g_chEndMark;

	Strcat(awhAdditionalString);
	return *this;
}

CXStringA CXStringA::operator+(const CXStringA &pchAdditionalString)
{
	CXStringA xstrTmp = *this;
	xstrTmp.Strcat(pchAdditionalString);
	return xstrTmp;
}

CXStringA CXStringA::operator+(LPCSTR pchAdditionalString)
{
	CXStringA xstrTmp = *this;
	xstrTmp.Strcat(pchAdditionalString);
	return xstrTmp;
}

CXStringA CXStringA::operator+(const CHAR chAdditionalChar)
{
	CXStringA xstrTmp = *this;
	xstrTmp += chAdditionalChar;
	return xstrTmp;
}

CXStringA operator+(LPCSTR pwhA, const CXStringA &xstrB)
{
	CXStringA xstrTmp = pwhA;
	xstrTmp += xstrB;
	return xstrTmp;
}
CXStringA operator+(const CHAR &chAdditionalChar, const CXStringA& xstrB)
{
	CXStringA xstrTmp(chAdditionalChar);
	xstrTmp += xstrB;
	return xstrTmp;
}
//////////////////////////////////////////////////////////////////////////
BOOL operator!=(LPCSTR pwhA, const CXStringA &xstrB)
{    
	return !(xstrB == pwhA);
}
BOOL operator==(LPCSTR pwhA, const CXStringA &xstrB)
{    
	return xstrB == pwhA;
}
BOOL operator!=(const CHAR whA, const CXStringA &xstrB)
{    
	return !(xstrB == whA);
}
BOOL operator==(const CHAR whA, const CXStringA &xstrB)
{
	return xstrB == whA;
}
//////////////////////////////////////////////////////////////////////////
void CXStringA::TrimLeft(const CHAR chA)
{
	CHAR achTag[2];
	achTag[0] = chA;
	achTag[1] = g_chEndMark;
	TrimLeft(achTag);
}
void CXStringA::TrimLeft(LPCSTR pchTag)
{
	//assert(NULL != pchTag);
	if (IsEmpty())
	{
		return;
	}   
	LPSTR pwhString = (LPSTR)C_Str();
	assert(m_nLength > 0);
	LONG nAmount = 0;  
	if (!pchTag)
	{
		pchTag = g_achDelimit;
	}
	nAmount = (LONG)strspn(pwhString, pchTag);
	assert(nAmount <= m_nLength);
	InternalExcludeLeft(nAmount);
}
void CXStringA::TrimRight(const CHAR chA)
{
	CHAR achTag[2];
	achTag[0] = chA;
	achTag[1] = g_chEndMark;
	TrimRight(achTag);
}
void CXStringA::TrimRight(LPCSTR pchTag)
{
	if (IsEmpty())
	{
		return;
	}    
	LONG nAmount = 0;
	BOOL blContinue = TRUE;
	if (NULL == pchTag)
	{
		pchTag = g_achDelimit;
	}

	for (LONG i = 0; i < m_nLength && blContinue; i++)
	{    
		if (NULL != strchr(pchTag, GetAt(m_nLength - i - 1)))
			nAmount ++;
		else
			blContinue = FALSE;
	}  
	if (!blContinue || nAmount > 0)
		InternalLeft(m_nLength - nAmount);
}
void CXStringA::Trim(const CHAR chA)
{
	CHAR achTag[2];
	achTag[0] = chA;
	achTag[1] = g_chEndMark;
	Trim(achTag);
}
void CXStringA::Trim(LPCSTR pchTag)
{
	TrimLeft(pchTag);
	TrimRight(pchTag);
}
LONG CXStringA::GetNumber()
{
	if (IsEmpty())
		return 0L;

	LONG nValue = 0;
	LPCSTR pchIndex = strpbrk(C_Str(), "0123456789.-+");
	if (pchIndex) 
	{
#if _MSC_VER >= 1000
		sscanf_s(pchIndex, "%ld", &nValue); 
#else
		sscanf(pchIndex, "%ld", &nValue); 
#endif
	}
	return nValue;
}

float CXStringA::GetFloat()
{
	if (IsEmpty())
		return 0.0f;

	float flValue = 0.0;
	LPCSTR pwhIndex = strpbrk(C_Str(), "0123456789.-+");
	if (pwhIndex) 
	{
#if _MSC_VER >= 1000
		sscanf_s(pwhIndex, "%f", &flValue); 
#else
		sscanf(pwhIndex, "%f", &flValue); 
#endif
	}
	return flValue;
}

LONG CXStringA::GetHex()
{
	if (IsEmpty())
		return 0;

	LONG nValue = 0;
	LPCSTR pwhIndex = strpbrk(C_Str(), "0123456789ABCDEFabcdef-+");
	if (pwhIndex)
	{
#if _MSC_VER >= 1000
		sscanf_s(pwhIndex, "%x", &nValue); 
#else
		sscanf(pwhIndex, "%x", &nValue); 
#endif
	}
	return nValue;
}
void CXStringA::ReplaceChar(const CHAR A, const CHAR B)
{ 
	LPSTR strString = (LPSTR)C_Str();
	assert(strString);
	if (!strString)
	{
		return;
	}	
	assert(strlen(strString) == m_nLength);	
	while (*strString)
	{
		if (A == *strString)
		{
			*strString = B;
		}    		
		strString ++;
	}  
}

void CXStringA::ReplaceCharWithString(const CHAR chA, LPCSTR pwhStringB)
{
	if (IsEmpty())
		return;

	CXStringA xstrData;  
	LONG nSearchIndex = 0;
	LONG nFindIndex;
	LPSTR pwhString = (LPSTR)C_Str();
	while (nSearchIndex < m_nLength && (nFindIndex = Find(nSearchIndex, chA)) > -1)
	{      
		//assert (nFindIndex > nSearchIndex);
		//xstrData += Mid(nSearchIndex, nFindIndex - nSearchIndex);  
		if (nFindIndex != nSearchIndex)
		{
			xstrData.Strcat(pwhString + nSearchIndex, nFindIndex - nSearchIndex);
		}    
		xstrData += pwhStringB;

		nSearchIndex = ++ nFindIndex;     
	}
	//处理尾巴
	if (nSearchIndex < m_nLength)
		xstrData += pwhString + nSearchIndex;

	*this = xstrData;
}

void CXStringA::ReplaceCharWithString(const CHAR chA, const CXStringA &xstrB)
{
	ReplaceCharWithString(chA, xstrB.C_Str());
}

void CXStringA::Format(LPCSTR pchFormat, ...) 
{  
	LPSTR pwhBuffer = NULL;
	LONG nLen = 0;
	va_list argList;
	va_start(argList, pchFormat);
	nLen = _vscprintf(pchFormat, argList);
	Reserve(nLen + 1);
	Clear();
	pwhBuffer = (LPSTR)C_Str();
#if _MSC_VER >= 1000
	vsprintf_s(pwhBuffer, nLen + 1, pchFormat, argList);  
#else
	vsprintf(pwhBuffer, pchFormat, argList); 
#endif
	va_end(argList);
	m_nLength = (LONG)strlen(pwhBuffer);
}

void CXStringA::FormatEx(LPCSTR pchFormat, ...) 
{  
	LPSTR pchBuffer = NULL;
	va_list argList;
	LONG nLen = 0, nLenOrg = m_nLength;
	va_start(argList, pchFormat);
	nLen = _vscprintf(pchFormat, argList);
	Reserve(nLen + nLenOrg + 1);
	pchBuffer = (LPSTR)C_Str();
#if _MSC_VER >= 1000
	vsprintf_s(pchBuffer + nLenOrg, nLen + 1, pchFormat, argList); 
#else
	vsprintf(pchBuffer + nLenOrg, pchFormat, argList); 
#endif
	va_end(argList);  
	m_nLength = (LONG)strlen(pchBuffer);
}

BOOL CXStringA::Token(LPCSTR pchDelimit, std::vector<CXStringA> &vString) const
{
	assert(pchDelimit);
	BOOL blRet = TRUE;
	if (strlen(pchDelimit) < 1)
	{
		blRet = FALSE;
		return blRet;
	}
	CXStringA strTmp = *this;
	CXStringA strSubStr;
	LPCSTR pchToken = NULL;
	LPSTR ptStr = (LPSTR)strTmp.C_Str();
#if _MSC_VER >= 1000
	LPSTR pchContent;
	pchToken = strtok_s( ptStr, pchDelimit, &pchContent);
	while( NULL != pchToken )
	{ 
		strSubStr = pchToken;
		vString.push_back(strSubStr); 
		pchToken = strtok_s( NULL, pchDelimit, &pchContent);
	} 
#else

	pchToken = strtok( ptStr, pchDelimit);
	while( NULL != pchToken )
	{ 
		strSubStr = pchToken;
		vString.push_back(strSubStr); 
		pchToken = strtok( NULL, pchDelimit );
	} 
#endif
	return blRet;
}
BOOL CXStringA::Token(const CHAR chDelimit, std::vector<CXStringA> &vString) const
{
	CHAR achDelimit[2];
	achDelimit[0] = chDelimit;
	achDelimit[1] = g_chEndMark;
	return Token(achDelimit, vString);
}
CXStringA CXStringA::From( LPCWSTR pwhA)
{
	USES_CONVERSION;
	LPSTR pchB = W2A(pwhA);  
	return CXStringA(pchB);
}
CXStringA CXStringA::From( LPCSTR pchA)
{
	return CXStringA(pchA);
}
CXStringA CXStringA::From( LONG nData)
{
	CHAR achBuffer[50];
#if _MSC_VER >= 1000
	sprintf_s(achBuffer, 50, "%d", nData);
#else
	sprintf(achBuffer, "%d", nData);
#endif
	return achBuffer;
}
CXStringA CXStringA::From( DWORD dwData)
{
	CHAR achBuffer[50];
#if _MSC_VER >= 1000
	sprintf_s(achBuffer, 50, "%x", dwData);
#else
	sprintf(achBuffer, "%x", dwData);
#endif
	return achBuffer;
}
CXStringA CXStringA::From( double dbData)
{
	CHAR achBuffer[50];
#if _MSC_VER >= 1000
	sprintf_s(achBuffer, 50, "%f", dbData);
#else
	sprintf(achBuffer, "%f", dbData);
#endif
	return achBuffer;
}
LPSTR CXStringA::GetBuffer( const DWORD& dwSize)
{
	Reserve(dwSize + 1);
	return LPSTR(m_tPackage.GetData());
}
void CXStringA::ReleaseBuffer()
{
	m_nLength = (LONG)strlen(C_Str());
}
void CXStringA::Remove( const CHAR chA)
{
	if (m_nLength <= 0)
	{
		return;
	}
	LPSTR strBuff = (LPSTR)C_Str();	
	LPCSTR strIndex = strBuff;

	assert((LONG)strlen(strBuff) == m_nLength);
	while(*strIndex != g_chEndMark)
	{
		if (*strIndex != chA)
		{
			*strBuff ++	= *strIndex;
		}
		strIndex ++;
	}
	*strBuff = g_chEndMark;
	m_nLength = (LONG)strlen(C_Str());
}
std::ostream &operator<<(std::ostream & oStream, const CXStringA &xStr)
{
	oStream<<xStr.C_Str();
	return oStream;
}