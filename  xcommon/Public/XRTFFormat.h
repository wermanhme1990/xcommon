/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRTFFormat.h					
文件内容:	  制作简单RTF格式
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/31
*********************************************************************/




#ifndef X_RTFFORMAT
#define X_RTFFORMAT

#include <afx.h>
#include <list>
#include <stack>

using namespace std;
#pragma  warning(disable : 4786)


class CRTFBuilder;
class CStringManip;
class CIntManip;
class CLONGManip;

typedef CRTFBuilder& (*RTFSM_PFUNC	)     (CRTFBuilder&);								
typedef CRTFBuilder& (*RTFSM_STRINGPFUNC)	(CRTFBuilder&, const CString&); 
typedef CRTFBuilder& (*RTFSM_INTPFUNC)	  (CRTFBuilder&, const int&);
typedef CRTFBuilder& (*RTFSM_LONGPFUNC)	  (CRTFBuilder&, const LONG&);	
typedef CRTFBuilder& (*RTFSM_BOOLPFUNC)	  (CRTFBuilder&, const bool&);			
typedef CRTFBuilder& (*RTFSM_CONTROLPFUNC)(CRTFBuilder&, CRichEditCtrl&);		



class CBoolString
{
private:
  BOOL		m_b;
  CString m_strOn;
  CString m_strOff;
public:
  CBoolString(CString strOn, CString strOff = TEXT(""))
  {
    m_strOn = strOn;
    m_strOff = strOff;
    m_b = FALSE;
  }

  void operator=(BOOL b) 
  { 
    m_b = b; 
  }
  operator CString() const
  { 
    return m_b ? m_strOn : m_strOff; 
  }
};

class CTextAttributes
{
protected:
  int			m_nFontSize;
  CBoolString  m_bsBold, m_bsUnderline, m_bsItalic, m_bsStrike;
  LONG		m_nFontNumber, m_nColorFground, m_nColorBground;
public:
  CTextAttributes() : m_bsBold(TEXT("\\b")), m_bsUnderline(TEXT("\\ul"))
    , m_bsItalic(TEXT("\\i")), m_bsStrike(TEXT("\\Strike")) 
  {
    m_nColorBground = m_nColorFground = m_nFontNumber = m_nFontSize = 0;
    m_bsBold = FALSE;
  }

  operator CString() const
  {
    CString s;
    s.Format(TEXT("\\plain%s%s%s%s\\f%d\\fs%d\\cb%d\\cf%d "),	(CString)m_bsBold,
      (CString)m_bsUnderline, (CString)m_bsItalic, (CString)m_bsStrike, 
      m_nFontNumber, m_nFontSize , m_nColorBground, m_nColorFground);
    return s;
  }
  friend class CRTFBuilder;
};



class CFontList: public list<CString>
{
public:
  operator CString() const
  { 
    CString s(TEXT("{\\fonttbl"));    
    CString s2;
    LONG nCount = 0;
    for (const_iterator i = begin(); i != end(); i ++)
    {      
      s2.Format(TEXT("{\\f%d %s;}"), nCount ++, (*i));
      s += s2;
    }
    s += TEXT('}');
    return s;
  }

  void Add(const CString& s)
  { push_back(s); }
};

class CColorList : public list<COLORREF>
{
public:
  LONG Add(const COLORREF& c)
  { 
    push_back (c); 
    return (LONG)size() -1;
  }
  LONG Find(const COLORREF& c)
  {
    LONG n = 0;
    for (iterator i = begin(); i != end(); i++, n++)
    {
      COLORREF cComp(*i);
      if (cComp == c)
      {
        return n;
      }
    }
    return -1;
  }

  operator CString() const
  {
    CString s(TEXT("{\\colortbl"));    
    for (const_iterator i = begin(); i != end(); i ++)
    {
      COLORREF c(*i);
      LONG	r((c & 0x000000ff)),
        g((c >> 8) & 0x000000ff),
        b((c >> 16) & 0x000000ff);
      CString s2;
      s2.Format(TEXT("\\Red%d\\Green%d\\Blue%d;"), r ,g, b);
      s += s2;
    }
    s += TEXT('}');
    return s;
  }
};

	

class DLLXEXPORT CManip
{
protected:
  LPVOID	m_pFunc;
  CString	m_strVal;
  int			m_iInt;  
  bool		m_bVal;
  LONG    m_nLong;

public:
  virtual CRTFBuilder& Run (CRTFBuilder&) = 0;
  CManip()
  { 
    m_pFunc =  NULL; 

    m_iInt = 0; 
    m_strVal = TEXT(""); 
    m_nLong = 0;
    m_iInt = 0;
    m_bVal = FALSE;
  }

  CManip(LPVOID	p, const CString& s)
  {
    m_pFunc = p;
    m_strVal = s;
  }

  CManip(LPVOID	p , const int& n)
  {
    m_pFunc = p;
    m_iInt = n;
  }

  CManip(LPVOID p, const bool& b)
  {
    m_pFunc = p;
    m_bVal = b;
  }
  CManip(LPVOID p, const LONG& b)
  {
    m_pFunc = p; 
    m_nLong = b;
  }
};


class DLLXEXPORT CStringManip : public CManip
{
public:
  CStringManip(RTFSM_STRINGPFUNC p,	const CString& s= TEXT("")) : CManip ((LPVOID)p, s) 
  {

  };

  CRTFBuilder& Run(CRTFBuilder& b)
  {		
    return ( (RTFSM_STRINGPFUNC) m_pFunc) (b, 	m_strVal); 
  }
};

class DLLXEXPORT CControlManip : public CManip
{
protected:
  CRichEditCtrl& m_control;

public:
  CControlManip(RTFSM_CONTROLPFUNC p,	
    CRichEditCtrl& c) :	m_control(c) , 
    CManip ((LPVOID)p, (CString)"") 
  {

  };

  CRTFBuilder& Run(CRTFBuilder& b)
  {	
    return ((RTFSM_CONTROLPFUNC) m_pFunc)(b, 	m_control); 
  }
};

class DLLXEXPORT CIntManip : public CManip
{

public:
  CIntManip(RTFSM_INTPFUNC p,	const int& n = 0) : CManip ((LPVOID)p, n)
  {

  };

  CRTFBuilder& Run(CRTFBuilder& b)
  {		
    return ((RTFSM_INTPFUNC) m_pFunc)(b, 	m_iInt); 
  }
};

class DLLXEXPORT CLONGManip: public CManip
{

public:
  CLONGManip(RTFSM_LONGPFUNC p,	const LONG& n = 0) : CManip ((LPVOID)p, n)
  {

  };

  CRTFBuilder& Run(CRTFBuilder& b)
  {	
    return ((RTFSM_LONGPFUNC) m_pFunc)(b, m_nLong);
  }
};

class DLLXEXPORT CBOOLManip: public CManip
{
public:
  CBOOLManip(RTFSM_BOOLPFUNC p,	const bool& b) : CManip ((LPVOID)p, b) 
  {

  };
  CRTFBuilder& Run(CRTFBuilder& b)
  {		
    return ((RTFSM_BOOLPFUNC)m_pFunc)(b, m_bVal);
  }
};

class DLLXEXPORT CRTFBuilder
{
protected:
  CString		m_string;
  CTextAttributes		m_attr;
  CFontList					m_fontList;
  CColorList				m_colorList;
  stack<CTextAttributes> m_attrStack;

public:
  void  Bold (const BOOL& b = TRUE);
  void  Strike(const BOOL& b = TRUE);
  void	Italic(const BOOL& b = TRUE);
  void	Underline(const BOOL& b = TRUE);
  void  Normal();
  void  Size (const LONG& n);
  void  Font (const CString& i);
  void  Black();
  void  Blue();
  void  Green();
  void  Red();
  void  Color(const COLORREF&); 
  void  BackColor(const COLORREF&); 

  void  Push();
  void  Pull();

  CString & operator+=(CString& s);
  CRTFBuilder();
  virtual ~CRTFBuilder();

  void AddFont(const CString& s)
  {	
    m_fontList.Add(s); 
  }
  void AddColor(COLORREF c)
  { 
    m_colorList.Add(c); 
  }
  CRTFBuilder& operator+=(LPCTSTR p); 
  operator CString() const
  { 
    return m_string;
  }
  void Write (CRichEditCtrl&);
  LONG ColorCount() const
  { 
    return m_colorList.size(); 
  }
public: 
  CRTFBuilder& operator << (LPCTSTR);
  CRTFBuilder& operator << (int);
  CRTFBuilder& operator >> (CRichEditCtrl&);

  friend DLLXEXPORT CRTFBuilder& Normal	(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Push		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Pull		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Black		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Red			(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Green		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Blue		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Bold		(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Strike	(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Italic	(CRTFBuilder&);
  friend DLLXEXPORT CRTFBuilder& Underline(CRTFBuilder&);
};



DLLXEXPORT CControlManip		Write			(CRichEditCtrl&);
DLLXEXPORT CLONGManip			Normal		(const LONG& = 0);
DLLXEXPORT CLONGManip			Push			(const LONG& = 0);
DLLXEXPORT CLONGManip			Pull			(const LONG& = 0);
DLLXEXPORT CLONGManip			Size			(const LONG&);
DLLXEXPORT CLONGManip			Color			(const LONG&);
DLLXEXPORT CLONGManip			BackColor	(const LONG&);
DLLXEXPORT CLONGManip			AddColor	(const LONG&);
DLLXEXPORT CLONGManip			Font			(const LONG&);
DLLXEXPORT CStringManip		Font		  (LPCTSTR);
DLLXEXPORT CStringManip		AddFont	  (LPCTSTR);
DLLXEXPORT CBOOLManip			Bold		  (const bool&);
DLLXEXPORT CBOOLManip			Strike	  (const bool&);
DLLXEXPORT CBOOLManip			Italic	  (const bool&);
DLLXEXPORT CBOOLManip			Underline	(const bool&);

DLLXEXPORT CRTFBuilder& operator << (CRTFBuilder&, RTFSM_PFUNC);
DLLXEXPORT CRTFBuilder& operator << (CRTFBuilder&, CManip&);

/////////////////////////////////////////////////////////////////////////////

//}}AFX_INSERT_LOCATION}}

#endif //X_RTFFORMAT