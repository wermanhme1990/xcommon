/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   HWXString.h          
文件内容:    自定义String处理类
版本历史:    1.0
作者:        xuejuntao xuejuntao@hanwang.com.cn 2008/03/06
*********************************************************************/
#ifndef HWXUE_CString_H_INC
#define HWXUE_CString_H_INC
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Ext_Memory.h"
#include "HWXPackage.h"
#include <vector>
#include <iostream>

const WCHAR g_awhDelimit[5] = {L'\n', L'\r', L'\t', L' ', 0x0};
const CHAR  g_achDelimit[5] = {'\n', '\r', '\t', ' ', 0x0};
class CXStringW;
class CXStringA;


#ifdef UNICODE
typedef CXStringW CXStringT;
#else 
typedef CXStringA CXStringT;
#endif
typedef CXStringT CXString;

typedef CXStringT CHWXString;

class DLLXEXPORT CXStringW
{  
private:  
  CHWXPackage m_tPackage;  
  LONG m_nIncStep;
  LONG m_nLength;
public:
  // Constructor/Destructor
  CXStringW();  
  CXStringW(LPCWSTR); 
  explicit CXStringW(const WCHAR &);
  CXStringW(LPCWSTR, LONG nLen);
  CXStringW(const CXStringW &);   
  ~CXStringW();  
	void AssertValid() const;
  /************************************************************************
  名称:  Clear
  全称:  void CHWXString::Clear(  )
  类型:  public 
  返回值:  void: 
  功能:  清除字符 
  ************************************************************************/
  void Clear();

  /************************************************************************
  名称:  StrLen
  全称:  LONG CHWXString::StrLen(  ) const
  类型:  public 
  返回值:  LONG: 
  功能:  获取串长度 
  ************************************************************************/
  LONG StrLen() const;
  /************************************************************************
  名称:  IsEmpty
  全称:  bool CHWXString::IsEmpty(  ) const
  类型:  public 
  返回值:  bool: 
  功能:  是否为空 
  ************************************************************************/
  BOOL IsEmpty() const;

  /************************************************************************
  名称:  C_Str
  全称:  char * CHWXString::GetData(  ) const
  类型:  public 
  返回值:  char *: 
  功能:  获取内容
  ************************************************************************/
  LPCWSTR C_Str() const;


  WCHAR GetAt(LONG );  
  BOOL SetAt(LONG , WCHAR );
  //////////////////////////////////////////////////////////////////////////
  //翻转字符串
  void Reverse();
  //字符集定位
  LONG FirstBelong(LPCWSTR);
  LONG FirstNoBelong(LPCWSTR);
  BOOL IsBelong(LPCWSTR);
  BOOL IsNoBelong(LPCWSTR);
  /************************************************************************
  名称:  MakeLower
  全称:  void CHWXString::MakeLower(  )
  类型:  public 
  返回值:  void: 
  功能:  大小写变换 
  ************************************************************************/
  void MakeLower();
  void MakeUpper();

  /************************************************************************
  名称:  Compare
  全称:  bool CHWXString::Compare( const char *pchCompareString )
  类型:  public 
  返回值:  bool: 
  功能:  比较 
  参数:  const char * pchCompareString: 
  ************************************************************************/
  BOOL Compare(LPCWSTR) const;
  BOOL Compare(const CXStringW &) const;
  BOOL CompareNoCase(LPCWSTR) const;
  BOOL CompareNoCase(const CXStringW &) const;  

  /************************************************************************
  名称:  Find
  全称:  LONG CHWXString::Find( LONG nStartIndex, LPCWSTR )
  类型:  public 
  返回值:  LONG: 
  功能:  查找 
  参数:  LONG nStartIndex: 
  参数:  char * pchStringToFind: 
  ************************************************************************/
  LONG Find(LONG , LPCWSTR);
  LONG Find(LPCWSTR);
  LONG Find(LONG ,WCHAR);
  LONG Find(WCHAR );
  LONG FindNoCase(LONG , LPCWSTR);
  LONG FindNoCase(LPCWSTR);
  LONG FindNoCase(LONG , WCHAR);
  LONG FindNoCase(WCHAR);  

  LONG ReverseFind(LONG , WCHAR);
  LONG ReverseFind(WCHAR );
  //////////////////////////////////////////////////////////////////////////
  CXStringW Left(LONG );
  CXStringW ExcludeLeft(LONG ); // remove first nCount chars
  CXStringW Mid(LONG , LONG );
  CXStringW Right(LONG );
  //this为操作后数据
  void InternalLeft(LONG); 
  void InternalExcludeLeft(LONG);   
  void InternalMid(LONG, LONG); 
  void InternalRight(LONG); 

  //操作符重载  
  operator LPCWSTR () const;
  WCHAR operator[](LONG );
  CXStringW &operator =(LPCWSTR);  
  CXStringW &operator =(const CXStringW &);  
  CXStringW &operator +=(const CXStringW &);  
  CXStringW &operator +=(LPCWSTR);
  CXStringW &operator +=(const WCHAR &);  

  CXStringW operator +(const CXStringW &);
  CXStringW operator +(LPCWSTR);
  CXStringW operator +(const WCHAR & );

  //friend CXStringW operator+(const CXStringW &, const CXStringW &);
  //friend CXStringW operator+(const CXStringW &, LPCWSTR);
  friend CXStringW operator+(LPCWSTR, const CXStringW &);
  friend CXStringW operator+(const WCHAR &, const CXStringW &);

  BOOL operator ==(const CXStringW &) const;
  BOOL operator!=(const CXStringW &) const;
  BOOL operator ==(LPCWSTR) const;
  BOOL operator!=(LPCWSTR)const;
  BOOL operator==(const WCHAR) const;
  BOOL operator!=(const WCHAR) const;

  friend BOOL operator==(LPCWSTR, const CXStringW &);
  friend BOOL operator!=(LPCWSTR, const CXStringW &);
  friend BOOL operator!=(const WCHAR, const CXStringW &);  
  friend BOOL operator==(const WCHAR, const CXStringW &);
  /************************************************************************
  名称:  TrimLeft
  全称:  void CHWXString::TrimLeft( WCHAR ch )
  类型:  public 
  返回值:  void: 
  功能:  修剪两遍特殊字符 
  参数:  char ch: 
  ************************************************************************/
  void TrimLeft(const WCHAR );
  void TrimLeft(LPCWSTR = NULL);
  void TrimRight(const WCHAR );
  void TrimRight(LPCWSTR = NULL);
  void Trim(const WCHAR );
  void Trim(LPCWSTR = NULL);  

  /************************************************************************
  名称:  GetNumber
  全称:  LONG CHWXString::GetNumber(  )
  类型:  public 
  返回值:  LONG: 
  功能:  过滤字符串中的数字 
  ************************************************************************/
  LONG  GetNumber();
  float  GetFloat();
  LONG  GetHex();
  /************************************************************************
  名称:  ReplaceChar
  全称:  void CHWXString::ReplaceChar( char ch, char sReplacementChar )
  类型:  public 
  返回值:  void: 
  功能:  替换操作 
  参数:  char ch: 
  参数:  char sReplacementChar: 
  ************************************************************************/
  void ReplaceChar(const WCHAR , const WCHAR );
  void ReplaceCharWithString(const WCHAR , LPCWSTR); // szReplacementString==NULL allowed  
  void ReplaceCharWithString(const WCHAR , const CXStringW &);

	void Remove( const WCHAR);
  /************************************************************************
  名称:  Format
  全称:  void Format(const char *pchFormat, ...)
  类型:  Public 
  返回值:  void: 
  功能:  格式化字符串 
  参数:  const char *pchFormat:    参考printf
  ************************************************************************/
  void Format(LPCWSTR, ...);
  void FormatEx(LPCWSTR, ...);


  /************************************************************************
  名称:	Token
  全称:	BOOL CHWXString::Token( const TCHAR *, std::vector<CHWXString> & )
  类型:	public 
  返回值:	BOOL: 
  功能:	  将字符串分割成字段输出 
  参数:	const TCHAR *: 字段的分割标记
  参数:	std::vector<CHWXString> &: 
  ************************************************************************/
  BOOL Token(LPCWSTR, std::vector<CXStringW> &) const;
  BOOL Token(const WCHAR , std::vector<CXStringW> &) const;

  friend std::ostream &operator<<(std::ostream &, const CXStringW &a);
  //////////////////////////////////////////////////////////////////////////
  static CXStringW From(LPCSTR);
  static CXStringW From(LPCWSTR);
  static CXStringW From(LONG);
  static CXStringW From(DWORD);
  static CXStringW From(double);

  LPWSTR GetBuffer( const DWORD& dwSize);  
  void ReleaseBuffer();
protected:  
  /************************************************************************
  名称:  Reset
  全称:  void CHWXString::Reset( const char *pchSource )
  类型:  private 
  返回值:  void: 
  功能:  内容重新赋值 
  参数:  const char * pchSource: 
  ************************************************************************/
  void Reset(LPCWSTR);
  void Reset(LPCWSTR, LONG nLen);
  void Reset(const CXStringW &);

  //////////////////////////////////////////////////////////////////////////
  void Reserve(LONG nLen);
  //////////////////////////////////////////////////////////////////////////

  void Init();

  //////////////////////////////////////////////////////////////////////////
  BOOL Strcat(LPCWSTR);
  BOOL Strcat(const CXStringW &);
  BOOL Strcat(LPCWSTR , LONG nDstLen);  
};

class DLLXEXPORT CXStringA
{  
private:  
  CHWXPackage m_tPackage;  
  LONG m_nIncStep;
  LONG m_nLength;
public:
  // Constructor/Destructor
  CXStringA();  
  CXStringA(LPCSTR);  
  explicit CXStringA(const CHAR&);
  CXStringA(LPCSTR, LONG nLen);
  CXStringA(const CXStringA &);   
  ~CXStringA();  

	void AssertValid() const;
  /************************************************************************
  名称:  Clear
  全称:  void CHWXString::Clear(  )
  类型:  public 
  返回值:  void: 
  功能:  清除字符 
  ************************************************************************/
  void Clear();

  /************************************************************************
  名称:  StrLen
  全称:  LONG CHWXString::StrLen(  ) const
  类型:  public 
  返回值:  LONG: 
  功能:  获取串长度 
  ************************************************************************/
  LONG StrLen() const;
  /************************************************************************
  名称:  IsEmpty
  全称:  bool CHWXString::IsEmpty(  ) const
  类型:  public 
  返回值:  bool: 
  功能:  是否为空 
  ************************************************************************/
  BOOL IsEmpty() const;

  /************************************************************************
  名称:  C_Str
  全称:  char * CHWXString::GetData(  ) const
  类型:  public 
  返回值:  char *: 
  功能:  获取内容
  ************************************************************************/
  LPCSTR C_Str() const;


  CHAR GetAt(LONG );  
  BOOL SetAt(LONG , CHAR );
  //////////////////////////////////////////////////////////////////////////
  //翻转字符串
  void Reverse();
  //字符集定位
  LONG FirstBelong(LPCSTR);
  LONG FirstNoBelong(LPCSTR);
  BOOL IsBelong(LPCSTR);
  BOOL IsNoBelong(LPCSTR);
  /************************************************************************
  名称:  MakeLower
  全称:  void CHWXString::MakeLower(  )
  类型:  public 
  返回值:  void: 
  功能:  大小写变换 
  ************************************************************************/
  void MakeLower();
  void MakeUpper();

  /************************************************************************
  名称:  Compare
  全称:  bool CHWXString::Compare( const char *pchCompareString )
  类型:  public 
  返回值:  bool: 
  功能:  比较 
  参数:  const char * pchCompareString: 
  ************************************************************************/
  BOOL Compare(LPCSTR) const;
  BOOL Compare(const CXStringA &) const;
  BOOL CompareNoCase(LPCSTR) const;
  BOOL CompareNoCase(const CXStringA &) const;  

  /************************************************************************
  名称:  Find
  全称:  LONG CHWXString::Find( LONG nStartIndex, LPCWSTR )
  类型:  public 
  返回值:  LONG: 
  功能:  查找 
  参数:  LONG nStartIndex: 
  参数:  char * pchStringToFind: 
  ************************************************************************/
  LONG Find(LONG , LPCSTR);
  LONG Find(LPCSTR);
  LONG Find(LONG ,CHAR);
  LONG Find(CHAR );
  LONG FindNoCase(LONG , LPCSTR);
  LONG FindNoCase(LPCSTR);
  LONG FindNoCase(LONG , CHAR);
  LONG FindNoCase(CHAR);  

  LONG ReverseFind(LONG , CHAR);
  LONG ReverseFind(CHAR );
  //////////////////////////////////////////////////////////////////////////
  CXStringA Left(LONG );
  CXStringA ExcludeLeft(LONG ); // remove first nCount chars
  CXStringA Mid(LONG , LONG );
  CXStringA Right(LONG );
  //this为操作后数据
  void InternalLeft(LONG); 
  void InternalExcludeLeft(LONG);   
  void InternalMid(LONG, LONG); 
  void InternalRight(LONG); 

  //操作符重载  
  operator LPCSTR () const;
  CHAR operator[](LONG );
  CXStringA &operator =(LPCSTR);  
  CXStringA &operator =(const CXStringA &);  
  CXStringA &operator +=(const CXStringA &);  
  CXStringA &operator +=(LPCSTR);
  CXStringA &operator +=(const CHAR&);  

  CXStringA operator +(const CXStringA &);
  CXStringA operator +(LPCSTR);
  CXStringA operator +(const CHAR );

  //friend CXStringA operator+(const CXStringA &, const CXStringA &);
  //friend CXStringA operator+(const CXStringA &, LPCSTR);
  friend CXStringA operator+(LPCSTR, const CXStringA &);
  friend CXStringA operator+(const CHAR &, const CXStringA&);

  BOOL operator ==(const CXStringA &) const;
  BOOL operator!=(const CXStringA &) const;
  BOOL operator ==(LPCSTR) const;
  BOOL operator!=(LPCSTR)const;
  BOOL operator==(const CHAR) const;
  BOOL operator!=(const CHAR) const;

  friend BOOL operator==(LPCSTR, const CXStringA &);
  friend BOOL operator!=(LPCSTR, const CXStringA &);
  friend BOOL operator!=(const CHAR, const CXStringA &);  
  friend BOOL operator==(const CHAR, const CXStringA &);
  /************************************************************************
  名称:  TrimLeft
  全称:  void CHWXString::TrimLeft( WCHAR ch )
  类型:  public 
  返回值:  void: 
  功能:  修剪两遍特殊字符 
  参数:  char ch: 
  ************************************************************************/
  void TrimLeft(const CHAR );
  void TrimLeft(LPCSTR = NULL);
  void TrimRight(const CHAR );
  void TrimRight(LPCSTR = NULL);
  void Trim(const CHAR );
  void Trim(LPCSTR = NULL);  

  /************************************************************************
  名称:  GetNumber
  全称:  LONG CHWXString::GetNumber(  )
  类型:  public 
  返回值:  LONG: 
  功能:  过滤字符串中的数字 
  ************************************************************************/
  LONG  GetNumber();
  float  GetFloat();
  LONG  GetHex();
  /************************************************************************
  名称:  ReplaceChar
  全称:  void CHWXString::ReplaceChar( char ch, char sReplacementChar )
  类型:  public 
  返回值:  void: 
  功能:  替换操作 
  参数:  char ch: 
  参数:  char sReplacementChar: 
  ************************************************************************/
  void ReplaceChar(const CHAR , const CHAR );
  void ReplaceCharWithString(const CHAR , LPCSTR); // szReplacementString==NULL allowed  
  void ReplaceCharWithString(const CHAR , const CXStringA &);
	
	void Remove(const CHAR);
  /************************************************************************
  名称:  Format
  全称:  void Format(const char *pchFormat, ...)
  类型:  Public 
  返回值:  void: 
  功能:  格式化字符串 
  参数:  const char *pchFormat:    参考printf
  ************************************************************************/
  void Format(LPCSTR, ...);
  void FormatEx(LPCSTR, ...);


  /************************************************************************
  名称:	Token
  全称:	BOOL CHWXString::Token( const TCHAR *, std::vector<CHWXString> & )
  类型:	public 
  返回值:	BOOL: 
  功能:	  将字符串分割成字段输出 
  参数:	const TCHAR *: 字段的分割标记
  参数:	std::vector<CHWXString> &: 
  ************************************************************************/
  BOOL Token(LPCSTR, std::vector<CXStringA> &) const;
  BOOL Token(const CHAR , std::vector<CXStringA> &) const;

  friend std::ostream &operator<<(std::ostream &, const CXStringA &a);
  static CXStringA  From(LPCWSTR);
  static CXStringA  From(LPCSTR);
  static CXStringA From(LONG);
  static CXStringA From(DWORD);
  static CXStringA From(double);

  LPSTR GetBuffer(const DWORD&);
  void ReleaseBuffer();
protected:  
  /************************************************************************
  名称:  Reset
  全称:  void CHWXString::Reset( const char *pchSource )
  类型:  private 
  返回值:  void: 
  功能:  内容重新赋值 
  参数:  const char * pchSource: 
  ************************************************************************/
  void Reset(LPCSTR);
  void Reset(LPCSTR, LONG nLen);
  void Reset(const CXStringA &);

  //////////////////////////////////////////////////////////////////////////
  void Reserve(LONG nLen);
  //////////////////////////////////////////////////////////////////////////

  void Init();

  //////////////////////////////////////////////////////////////////////////
  BOOL Strcat(LPCSTR);
  BOOL Strcat(const CXStringA &);
  BOOL Strcat(LPCSTR , LONG nDstLen);  
};


#endif //HWXUE_CString_H_INC


