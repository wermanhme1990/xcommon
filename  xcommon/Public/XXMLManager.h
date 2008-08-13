

#pragma  once
#include <map>

#include <xercesc/dom/DOM.hpp>
XERCES_CPP_NAMESPACE_USE
/////////////////////////////////////////////////////////////////////////////
class XStr
{
public : 
  XStr(const char* const toTranscode)
  {    
    m_pxhUnicodeForm = XMLString::transcode(toTranscode);
  }
  ~XStr()
  {
    XMLString::release(&m_pxhUnicodeForm);
  }  
  const XMLCh* UnicodeForm() const
  {
    return m_pxhUnicodeForm;
  }
private :  
  XMLCh*  m_pxhUnicodeForm;
};
#define X(pch) XStr(pch).UnicodeForm()

class CHWXML
{
private:
  
public:
  CHWXML()
  {   
    Startup();
  }
  ~CHWXML()
  {   
    Shutdown();  
  }
  static XERCES_CPP_NAMESPACE::DOMDocument*GetDocument(LPCWSTR = NULL);  
  static DOMWriter *GetWriter();
  static BOOL WriteDocument(const XERCES_CPP_NAMESPACE::DOMDocument *, const CXString& );
  static BOOL ReleaseDocument(const XERCES_CPP_NAMESPACE::DOMDocument *);
  static CHWXML& CreateInstance();
  static void Clear();
  static void Startup();
  static void Shutdown();
private:  
  XERCES_CPP_NAMESPACE::DOMDocument *CreateDocument(LPCWSTR = NULL);
  XERCES_CPP_NAMESPACE::DOMDocument *OpenDocument(LPCWSTR);
  XERCES_CPP_NAMESPACE::DOMDocument *NewDocument();
  XERCES_CPP_NAMESPACE::DOMWriter *CreateWriter(); 
  BOOL ReleaseDocumentI(const XERCES_CPP_NAMESPACE::DOMDocument *);
  static LONG m_nRef;
  static DOMImplementation *m_ptXMLImpl;
  static vector<XERCES_CPP_NAMESPACE::DOMDocument *> m_vDocumentContainer;
  static vector<DOMWriter *> m_vWriterContainer;
};


class CXXMLManager 
{
private:  
  XERCES_CPP_NAMESPACE::DOMDocument* m_pDOMDocument;
public:    
  virtual DOMElement *AppendChildElement(DOMElement *, const XMLCh *const);
  virtual DOMElement *AppendChildElement(DOMElement *, const wstring&);

  virtual DOMText *AppendChildText(DOMElement *, const XMLCh *const pxhData);
  virtual DOMText *AppendChildText(DOMElement *, double);
  virtual DOMText *AppendChildText(DOMElement *, LONG);
  virtual DOMText *AppendChildText(DOMElement *, DWORD);
  //读写XML需要的数据
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   const char *);  
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   const XMLCh *);
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   const wstring &);
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   LONG);
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   DWORD);
  virtual BOOL SetAttr(DOMElement *, const XMLCh *const ,   double);

  virtual BOOL GetAttr(DOMElement *, const XMLCh *const, wstring &);
  virtual BOOL GetAttr(DOMElement *, const XMLCh *const, CXString &);
  virtual BOOL GetAttr(DOMElement *, const XMLCh *const, LONG &);
  virtual BOOL GetAttr(DOMElement *, const XMLCh *const, DWORD &);
  virtual BOOL GetAttr(DOMElement *, const XMLCh *const, double &); 
  virtual DOMNodeList * GetChildByName( DOMElement *pParent, const XMLCh *const);

  
  BOOL GetAttributeLong(LPCWSTR , LPCWSTR , LPCWSTR , LONG & , LONG = 0);
  BOOL GetAttributeDWORD(LPCWSTR , LPCWSTR , LPCWSTR , DWORD & , DWORD = 0);
  BOOL GetAttributeDouble(LPCWSTR , LPCWSTR , LPCWSTR , double & , double = 0.0);

  BOOL SetAttributeLong(LPCWSTR , LPCWSTR , LPCWSTR , const LONG & );
  BOOL SetAttributeDWORD(LPCWSTR , LPCWSTR , LPCWSTR , const DWORD & );
  BOOL SetAttributeDouble(LPCWSTR , LPCWSTR , LPCWSTR , const double & );

  BOOL GetAttribute(LPCWSTR , LPCWSTR , LPCWSTR , CXString & , LPCWSTR = NULL);  
  BOOL SetAttribute(LPCWSTR , LPCWSTR , LPCWSTR , LPCWSTR );
  BOOL GetAttribute(DOMElement* , LPCWSTR , LPCWSTR , CXString & , LPCWSTR = NULL);  
  BOOL SetAttribute(DOMElement* , LPCWSTR , LPCWSTR , LPCWSTR );
  BOOL GetAttribute(DOMElement *, LPCWSTR , CXString & , LPCWSTR = NULL);
  BOOL SetAttribute(DOMElement* , LPCWSTR , LPCWSTR );

  DOMElement *CreateElement(LPCWSTR, LPCWSTR);
  DOMElement * CreateElement(DOMElement *, LPCWSTR );
  BOOL Write(const CXString&);
  virtual DOMElement* GetFirstSubElement( DOMElement *ptParent,const wstring& wstrTagName);
  DOMElement *CreateElement(const XMLCh *const );
  DOMText *CreateTextElement(const XMLCh *const ); 
  DOMElement* Locate(const CXString&);
  DOMElement* Locate(XERCES_CPP_NAMESPACE::DOMDocument *, const CXString& );
  DOMElement* Locate(XERCES_CPP_NAMESPACE::DOMElement *, const vector<CXString>& );
  DOMElement* Locate(XERCES_CPP_NAMESPACE::DOMElement *, const CXString& );
public:	
  static CXXMLManager &GetInstance();
  BOOL Attach(LPCTSTR);
  void Detach();
  CXXMLManager();	
	virtual ~CXXMLManager();
protected:
	wstring m_strXMLFileName; 
};

