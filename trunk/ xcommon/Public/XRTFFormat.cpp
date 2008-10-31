/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRTFFormat.cpp					
文件内容:	  制作简单RTF格式
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/31
*********************************************************************/
#include "StdAfx.h"
#include "XRtfFormat.h"

#include <HWXString.h>
#include <GlobalEx.h>
//
// GLOBAL STREAM MANIPULATORS
CRTFBuilder& Write(CRTFBuilder& r, CRichEditCtrl&	c) throw ()
{																					 
  r.Write(c);
  return r ;
}

CRTFBuilder& Size(CRTFBuilder&r, const LONG& n) throw ()
{																					 
  r.Size(n);
  return r ;
}

CRTFBuilder& doBold(CRTFBuilder& r, const BOOL& b)
{
  r.Bold(b);
  return r ;
}

CRTFBuilder& Strike(CRTFBuilder& r, const bool& b)
{
  r.Strike(b);
  return r ;
}

CRTFBuilder& Italic(CRTFBuilder& r, const bool& b)
{
  r.Italic(b);
  return r ;
}

CRTFBuilder& Underline(CRTFBuilder& r, const bool& b)
{
  r.Underline(b);
  return r ;
}

CRTFBuilder& Font(CRTFBuilder&	r, const LONG& n)
{
  CString strFont;
  strFont.Format(TEXT("%d"), n);
  r.Font(strFont);
  return r ;
}

CRTFBuilder& Color(	CRTFBuilder&r,const LONG& n)
{
  r.Color((COLORREF)n);
  return r ;
}

CRTFBuilder& BackColor(CRTFBuilder&r,const LONG& n)
{
  r.BackColor((COLORREF)n);
  return r ;
}

CRTFBuilder& AddColor(CRTFBuilder&	r,const LONG&	n)
{
  r.AddColor((COLORREF)n);
  return r ;
}

CRTFBuilder& Font(CRTFBuilder&	r, const CString&	s)
{
  r.Font(s);
  return r ;
}

CRTFBuilder& AddFont(	CRTFBuilder&	r, const CString&	s)
{
  r.AddFont(s);
  return r ;
}


CControlManip Write(CRichEditCtrl& c) throw ()
{
  return CControlManip(&Write, c);
}

CLONGManip Size(const LONG& n) throw()
{
  return CLONGManip(&Size, n);
}

CLONGManip Font(const LONG& n) throw()
{
  return CLONGManip(&Font, n);
}

CLONGManip Color(const LONG& n) throw()
{
  return CLONGManip(&Color, n);
}

CLONGManip BackColor(const LONG& n) throw()
{
  return CLONGManip(&BackColor, n);
}

CLONGManip AddColor(const LONG &n) throw()
{
  return CLONGManip(&AddColor, n);
}

CStringManip Font(LPCTSTR s) throw()
{
  return CStringManip(&Font, s);
}

CStringManip AddFont(LPCTSTR s) throw()
{
  return CStringManip(&AddFont, s);
}

CBOOLManip Bold(const bool& b) throw()
{
  return CBOOLManip((RTFSM_BOOLPFUNC)&doBold, b);
}

CBOOLManip Strike(const bool& b) throw()
{
  return CBOOLManip(	&Strike, b);
}

CBOOLManip Italic(const bool& b) throw()
{
  return CBOOLManip(&Italic, b);
}

CBOOLManip Underline(const bool& b) throw()
{
  return CBOOLManip(&Underline, b);
}

CRTFBuilder& operator << (	CRTFBuilder&	b, RTFSM_PFUNC	f) 
{
  return f(b);
}


CRTFBuilder& operator << (	CRTFBuilder&	b, CManip&	f) 
{
  return f.Run(b);
}

//CRTFBuilder& 
//operator << (	CRTFBuilder&		b, 
//							CControlManip&	f) 
//{
//	return f.Run(b);
//}

CRTFBuilder& Normal(CRTFBuilder& b)
{
  b.Normal();
  return b ;
}

CRTFBuilder& Black(CRTFBuilder& b)
{
  b.Black();
  return b ;
}

CRTFBuilder& Push(CRTFBuilder& b)
{
  b.Push();
  return b ;
}

CRTFBuilder& Pull(CRTFBuilder& b)
{
  b.Pull();
  return b ;
}

CRTFBuilder& Red(CRTFBuilder& b)
{
  b.Red();
  return b ;
}

CRTFBuilder& Green(CRTFBuilder& b)
{
  b.Green();
  return b ;
}

CRTFBuilder& Blue(CRTFBuilder& b)
{
  b.Blue();
  return b ;
}

CRTFBuilder& Bold(CRTFBuilder& b)
{
  b.Bold();
  return b ;
}

CRTFBuilder& Strike(CRTFBuilder& b)
{
  b.Strike();
  return b ;
}

CRTFBuilder& Italic(CRTFBuilder& b)
{
  b.Italic();
  return b ;
}

CRTFBuilder& Underline(CRTFBuilder& b)
{
  b.Underline();
  return b ;
}


CRTFBuilder::CRTFBuilder()
{
  m_fontList.Add("Arial");
  m_colorList.Add(RGB(0,0,0) );
  m_colorList.Add(RGB(255,0,0) );
  m_colorList.Add(RGB(0,255,0) );
  m_colorList.Add(RGB(0,0,255) );
  Size(35);
}

CRTFBuilder::~CRTFBuilder()
{

}


void CRTFBuilder::Push()
{
  m_attrStack.push(m_attr);
}

void CRTFBuilder::Pull()
{
  m_attr = m_attrStack.top();
  m_attrStack.pop();
}

void CRTFBuilder::Color(const COLORREF& c)
{
  LONG n (m_colorList.Find(c));
  m_attr.m_nColorFground= n < 0 ? m_colorList.Add(c) :	n ;
}

void CRTFBuilder::BackColor(const COLORREF& c)
{
  LONG n (m_colorList.Find(c));
  m_attr.m_nColorBground= n < 0 ? m_colorList.Add(c) : n ;
}

void CRTFBuilder::Black()
{
  m_attr.m_nColorFground = 0 ;
}



void CRTFBuilder::Red()
{
  m_attr.m_nColorFground = 1 ;
}

void CRTFBuilder::Green()
{
  m_attr.m_nColorFground = 2 ;

}

void CRTFBuilder::Blue()
{
  m_attr.m_nColorFground = 3 ;
}

void CRTFBuilder::Size(const LONG& n)
{
  m_attr.m_nFontSize = n ;
}


void CRTFBuilder::Font(const CString& strFont)
{

  int nCount =  0 ;
  for (list<CString>::iterator i = m_fontList.begin(); i != m_fontList.end(); i++, nCount++)
  {
    if ((*i) == strFont)
    {
      m_attr.m_nFontNumber = nCount ;
      return ;
    }
  }
  //not there, lets Add it
  m_fontList.Add(strFont);
  m_attr.m_nFontNumber = nCount ;
}


void CRTFBuilder::Bold(const BOOL& b)
{
  m_attr.m_bsBold = b ;
}

void CRTFBuilder::Strike(const BOOL& b)
{
  m_attr.m_bsStrike = b ;
}

void CRTFBuilder::Italic(const BOOL& b)
{
  m_attr.m_bsItalic = b ;
}

void CRTFBuilder::Underline(const BOOL& b)
{
  m_attr.m_bsUnderline = b ;
}

void CRTFBuilder::Normal()
{
  Bold(false);
  Italic(false);
  Underline(false);
  Strike(false);
}
/*

static DWORD CALLBACK EditStreamCallBack(	DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  CString *pstr = (CString *)dwCookie;
  if(LONG(pstr->GetLength() * sizeof(TCHAR)) < cb)
  {
    TRACE(TEXT("Len = %d\n"), pstr->GetLength());
    *pcb = pstr->GetLength() * sizeof(TCHAR);
    memcpy(pbBuff, (LPCTSTR)*pstr, *pcb);
    pstr->Empty();
  }
  else
  {
    *pcb = cb;
    memcpy(	pbBuff, (LPCTSTR)*pstr, *pcb);
    *pstr = pstr->Right(pstr->GetLength() - (cb / sizeof(TCHAR)));
  }
  return 0;
}*/

static DWORD CALLBACK EditStreamCallBack(	DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  CXStringA *pstr = (CXStringA*)dwCookie;
  if(pstr->StrLen() < cb)
  {   
    *pcb = pstr->StrLen();
    memcpy(pbBuff, (LPCSTR)*pstr, *pcb);
    pstr->Clear();
  }
  else
  {
    *pcb = cb;
    memcpy(	pbBuff, (LPCSTR)*pstr, *pcb);
    *pstr = pstr->Right(pstr->StrLen() - cb);
  }
  return 0;
}
void CRTFBuilder::Write(CRichEditCtrl& c)
{
  m_string += TEXT('}');  
  LPCSTR strBuffer = TCharToChar(m_string);
  CXStringA strMbcs = strBuffer;
  if (strBuffer)
  {
    Mem_FreeMemory((void **)&strBuffer);
  }
  EDITSTREAM es = {(DWORD)&strMbcs, 0, EditStreamCallBack };
  c.StreamIn(SF_RTF | SFF_SELECTION, es);
  m_string.Empty();
}



CRTFBuilder& CRTFBuilder::operator+=(LPCTSTR p)
{
  CString s(p) , s2 ;
  for (int i = 0 ; i < s.GetLength(); i ++)
  {
    if (s[i]==TEXT('\n'))
    {
      s2 += (CString)TEXT("\r\n") += TEXT("\\par ");//\\par " ;
    }
    else
    {
      s2 += s[i];
    }
  }
  m_string += (CString) TEXT("{\\rtf1\\ansi\\ansicpg936\\uc2\\deff0\\deftab720") 
    += (CString)m_fontList  += (CString)m_colorList += (CString)m_attr += s2;

  return *this ;
}

CRTFBuilder&CRTFBuilder::operator << (LPCTSTR p)
{
  *this+=p ;
  return *this ;
}

CRTFBuilder&CRTFBuilder::operator << (int n)
{
  CString s ;
  s.Format(TEXT("%d"), n);
  *this += (LPCTSTR)s ;
  return *this ;
}


CRTFBuilder&CRTFBuilder::operator >> (CRichEditCtrl& e)
{
  Write(e);
  return *this ;
}
