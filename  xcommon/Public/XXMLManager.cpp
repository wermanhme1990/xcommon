

#include "stdafx.h"
#include "XXMLManager.h"
//#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDomParser.hpp>
#ifdef _DEBUG
#pragma  comment (lib, "xerces-c_2D.lib")
#else
#pragma  comment (lib, "xerces-c_2.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LONG CHWXML::m_nRef = 0;
DOMImplementation *CHWXML::m_ptXMLImpl;
vector<XERCES_CPP_NAMESPACE::DOMDocument *> CHWXML::m_vDocumentContainer;
vector<DOMWriter *> CHWXML::m_vWriterContainer;

XERCES_CPP_NAMESPACE::DOMDocument* CHWXML::GetDocument(LPCWSTR strPath)
{
  CHWXML &tXMLImpl = CreateInstance();  
  return tXMLImpl.OpenDocument(strPath);
}
DOMWriter* CHWXML::GetWriter()
{
  CHWXML& tXMLImpl = CreateInstance();  
  return tXMLImpl.CreateWriter();
}
XERCES_CPP_NAMESPACE::DOMDocument * CHWXML::CreateDocument(LPCWSTR strPath)
{
  assert(m_ptXMLImpl);
  XERCES_CPP_NAMESPACE::DOMDocument *ptDocument = NULL;
  VERIFY(ptDocument = m_ptXMLImpl->createDocument());
  return ptDocument;
}
XERCES_CPP_NAMESPACE::DOMDocument * CHWXML::OpenDocument(LPCWSTR strPath)
{
  assert(m_ptXMLImpl);
  XERCES_CPP_NAMESPACE::DOMDocument *ptDocument = NULL;
  if (strPath)
  {    
    assert(m_ptXMLImpl);
    XERCES_CPP_NAMESPACE::XercesDOMParser *parser = new XercesDOMParser;
    parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
    parser->setDoNamespaces(TRUE);    // optional  
    parser->parse(strPath);  
    ptDocument = parser->getDocument();
    //delete parser;
    //parser = NULL;
  }
  if (!ptDocument)
  {
    VERIFY(ptDocument = m_ptXMLImpl->createDocument());
  }    
  return ptDocument;
}
XERCES_CPP_NAMESPACE::DOMDocument * CHWXML::NewDocument()
{
  assert(m_ptXMLImpl);
  XERCES_CPP_NAMESPACE::DOMDocument *ptDocument = NULL;
  VERIFY(ptDocument = m_ptXMLImpl->createDocument());  
  return ptDocument;
}
XERCES_CPP_NAMESPACE::DOMWriter * CHWXML::CreateWriter()
{
  assert(m_ptXMLImpl);
  DOMWriter *ptWriter = NULL;
  VERIFY(ptWriter = m_ptXMLImpl->createDOMWriter());
  return ptWriter;
}
CHWXML& CHWXML::CreateInstance()
{
  static CHWXML tXMLImpl;  
  return tXMLImpl;
}
BOOL CHWXML::WriteDocument( const XERCES_CPP_NAMESPACE::DOMDocument * ptDoc, 
                           const CXString& wstrPath)
{
  assert(ptDoc);  
  XERCES_CPP_NAMESPACE::DOMWriter* ptDomWrite = CHWXML::GetWriter();  
  assert(ptDomWrite);  
  LocalFileFormatTarget tFile(wstrPath.C_Str());
  ptDomWrite->setEncoding(L"UTF-8"); 
  ptDomWrite->writeNode(&tFile, *ptDoc);
  ptDomWrite->release();
  return TRUE;
}
BOOL CHWXML::ReleaseDocument( const XERCES_CPP_NAMESPACE::DOMDocument * pDocument)
{
  CHWXML &tXMLImpl = CreateInstance();  
  return tXMLImpl.ReleaseDocumentI(pDocument);  
}
BOOL CHWXML::ReleaseDocumentI( const XERCES_CPP_NAMESPACE::DOMDocument * pDocument)
{
  BOOL blRet = FALSE;
  vector<XERCES_CPP_NAMESPACE::DOMDocument *> ::iterator itvpDocument;
  itvpDocument = find(m_vDocumentContainer.begin(), m_vDocumentContainer.end(), pDocument);
  if (itvpDocument != m_vDocumentContainer.end())
  {
    (*itvpDocument)->release();
    *itvpDocument = NULL;
    m_vDocumentContainer.erase(itvpDocument);
    blRet = TRUE;
  }  
  return blRet;
}
void CHWXML::Clear()
{   
  for (vector<XERCES_CPP_NAMESPACE::DOMDocument *>::iterator itvDocument = m_vDocumentContainer.begin();
    itvDocument < m_vDocumentContainer.end(); itvDocument ++)
  {
    (*itvDocument)->release();
    (*itvDocument) = NULL;
  }
  m_vDocumentContainer.clear();

  for(vector<XERCES_CPP_NAMESPACE::DOMWriter *>::iterator itvWriter = m_vWriterContainer.begin();
    itvWriter < m_vWriterContainer.begin(); itvWriter ++)
  {
    (*itvWriter)->release();
    (*itvWriter) = NULL;
  }
  m_vWriterContainer.clear();
}
void CHWXML::Startup()
{
  if (m_nRef <= 0)
  {
    XMLPlatformUtils::Initialize();
    m_ptXMLImpl =  DOMImplementationRegistry::getDOMImplementation(L"Traversal");
    m_vDocumentContainer.clear();
    m_vWriterContainer.clear();
    m_nRef = 1;
  }  
  else
  {
    m_nRef ++;
  }
}
void CHWXML::Shutdown()
{
  if(m_nRef > 1)
  {
    assert(m_ptXMLImpl);
    m_nRef --;
  }
  else if (1 == m_nRef)
  {
    Clear();
    m_ptXMLImpl = NULL;
    XMLPlatformUtils::Terminate();
    m_nRef --;
  } 
  else if (m_nRef <= 0)
  {
    assert(!m_ptXMLImpl);
    m_nRef = 0;
  }  
}
BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement * ptElement, const XMLCh *const pxhAttrName, long nData)
{  
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%d", nData);
  return SetAttr(ptElement, pxhAttrName, awhBuffer);
}
BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement * ptElement, const XMLCh *const pxhAttrName, const wstring& wStr)
{
  return SetAttr(ptElement, pxhAttrName, wStr.c_str());
}
BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement * ptElement, const XMLCh *const pxhAttrName, DWORD dwData)
{  
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%X", dwData);
  return SetAttr(ptElement, pxhAttrName, awhBuffer);
}
BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const XMLCh *const pxhAttrName,  const char *pchData)
{    
  return SetAttr(ptElement, pxhAttrName, X(pchData));
}
BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const XMLCh *const pxhAttrName, double dbData)
{
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%f", dbData);
  return SetAttr(ptElement, pxhAttrName, awhBuffer);
}

BOOL CXXMLManager::SetAttr(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const XMLCh *const pxhName,  const XMLCh *pxhAttr)
{
  assert(ptElement);  
  assert(pxhName && pxhAttr);
  BOOL blRet = TRUE;
  ptElement->setAttribute(pxhName, pxhAttr);
  return blRet;
}
DOMElement *CXXMLManager::AppendChildElement(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const XMLCh *const pxhTag)
{
  DOMElement *ptRetElement = NULL;
  if (ptElement)
  {
    if (0 == XMLString::compareIString(ptElement->getTagName(), pxhTag))
    {
      ptRetElement = ptElement;
    }
    else
    {
      VERIFY(ptRetElement = CreateElement(pxhTag));
      ptElement->appendChild(ptRetElement);
    }
  }
  else
  {
    VERIFY(ptRetElement = CreateElement(pxhTag));
    m_pDOMDocument->appendChild(ptRetElement);
  }    
  return ptRetElement;
}
DOMElement *CXXMLManager::AppendChildElement(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const wstring& pxhTag)
{
  DOMElement *ptRetElement = NULL;
  if (ptElement)
  {
    if (0 == XMLString::compareIString(ptElement->getTagName(), pxhTag.c_str()))
    {
      ptRetElement = ptElement;
    }
    else
    {
      VERIFY(ptRetElement = CreateElement(pxhTag.c_str()));
      ptElement->appendChild(ptRetElement);
    }
  }
  else
  {
    VERIFY(ptRetElement = CreateElement(pxhTag.c_str()));
    m_pDOMDocument->appendChild(ptRetElement);
  }    
  return ptRetElement;
}
DOMText *CXXMLManager::AppendChildText(XERCES_CPP_NAMESPACE::DOMElement *ptElement, const XMLCh *const pxhData)
{
  XERCES_CPP_NAMESPACE::DOMText *ptSubText = NULL;
  VERIFY(ptSubText = CreateTextElement(pxhData));
  ptElement->appendChild(ptSubText);
  return ptSubText;
}
DOMText *CXXMLManager::AppendChildText(XERCES_CPP_NAMESPACE::DOMElement *ptElement, double dbData)
{
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%f", dbData);  
  return AppendChildText(ptElement, awhBuffer);
}
DOMText *CXXMLManager::AppendChildText(XERCES_CPP_NAMESPACE::DOMElement *ptElement, LONG nData)
{
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%d", nData);  
  return AppendChildText(ptElement, awhBuffer);
}
DOMText *CXXMLManager::AppendChildText(XERCES_CPP_NAMESPACE::DOMElement *ptElement, DWORD dwData)
{
  WCHAR awhBuffer[50];
  Mem_ZeroMemory(awhBuffer, sizeof(awhBuffer));
  swprintf(awhBuffer, L"%X", dwData);
  return AppendChildText(ptElement, awhBuffer);
}
XERCES_CPP_NAMESPACE::DOMElement * CXXMLManager::CreateElement( const XMLCh *const pxhTag)
{
  assert(pxhTag);
  assert(m_pDOMDocument);
  if (m_pDOMDocument)
  {
    return m_pDOMDocument->createElement(pxhTag);
  }
  else
  {
    return NULL;
  }
}
DOMElement * CXXMLManager::CreateElement( LPCWSTR szBaseKeyName, LPCWSTR szKey)
{
  CXString strBase = szBaseKeyName;  
  strBase.Trim(TEXT('/'));    
  DOMElement *pBase = Locate(m_pDOMDocument, strBase);
  if (!pBase)
  {    
    return NULL;
  }
  return CreateElement(pBase, szKey);
}
DOMElement * CXXMLManager::CreateElement(DOMElement *pParent, LPCWSTR szKey)
{
  assert(pParent);  
  CXString strKey = szKey;
  strKey.Trim(TEXT('/'));    
  if (strKey.IsEmpty())
  {
    return pParent;
  }    
  vector<CXString> vstrSplit;
  VERIFY(strKey.Token('/', vstrSplit)); 
  DOMElement *pXMLKey = pParent;
  for (LONG nIndex = 0; nIndex < (LONG)vstrSplit.size(); nIndex ++)
  {
    VERIFY(pXMLKey = AppendChildElement(pXMLKey, vstrSplit[nIndex]));  
  }
  return pXMLKey;  
}
XERCES_CPP_NAMESPACE::DOMText *CXXMLManager::CreateTextElement(const XMLCh *const pxhText)
{
  assert(pxhText);
  assert(m_pDOMDocument);
  if (m_pDOMDocument)
  {
    return m_pDOMDocument->createTextNode(pxhText);
  }
  else
  {
    return NULL;
  }
}

BOOL CXXMLManager::Write(const CXString& wstrPath)
{
  BOOL blRet = FALSE;
  XERCES_CPP_NAMESPACE::DOMDocument *ptDoc = m_pDOMDocument; 
  blRet = CHWXML::WriteDocument(m_pDOMDocument, wstrPath);
  return blRet;

}
DOMElement* CXXMLManager::GetFirstSubElement( DOMElement *ptParent,const wstring& wstrTagName)
{
  DOMElement *ptRet = NULL;
  if (ptParent)
  {    
    ptRet = dynamic_cast<DOMElement *>(ptParent->getFirstChild());
  }
  else
  {
    ptRet = dynamic_cast<DOMElement *>(m_pDOMDocument->getFirstChild());
  }
  return ptRet;
}
CXXMLManager::~CXXMLManager()
{
  //Write(m_strXMLFileName);
  m_pDOMDocument = NULL;
  CHWXML::Startup();
}
/////////////////////////////////////////////////////////////////////////////
// CXXMLManager
CXXMLManager::CXXMLManager() : m_strXMLFileName()
{
  m_pDOMDocument = NULL;  
  CHWXML::Startup();
}
BOOL CXXMLManager::GetAttr( XERCES_CPP_NAMESPACE::DOMElement *ptElement, 
                           const XMLCh *const pxhAttrName, 
                                         wstring& wstrAttr )
{
  assert(ptElement);  
  assert(pxhAttrName);
  BOOL blRet = TRUE;
  const XMLCh * pXMLCh = ptElement->getAttribute(pxhAttrName);
  if (pXMLCh)
  {
    wstrAttr = pXMLCh;    
  }
  else
  {
    blRet = FALSE;
  }
  return blRet;
}
BOOL CXXMLManager::GetAttr( XERCES_CPP_NAMESPACE::DOMElement *ptElement, 
                            const XMLCh *const pxhAttrName, 
                            CXString& wstrAttr )
{
  assert(ptElement);  
  assert(pxhAttrName);
  BOOL blRet = TRUE;
  const XMLCh * pXMLCh = ptElement->getAttribute(pxhAttrName);
  if (pXMLCh)
  {
    wstrAttr = pXMLCh;    
  }
  else
  {
    blRet = FALSE;
  }
  return blRet;
}
BOOL CXXMLManager::GetAttr( XERCES_CPP_NAMESPACE::DOMElement *ptElement, 
                           const XMLCh *const pxhAttrName, 
                           LONG& nLong )
{
  assert(ptElement);  
  assert(pxhAttrName);
  BOOL blRet = TRUE;
  const XMLCh * pXMLCh = ptElement->getAttribute(pxhAttrName);
  CXString strContent;
  if (pXMLCh)
  {
    strContent = pXMLCh;  
    nLong = strContent.GetNumber();
  }
  else
  {
    blRet = FALSE;
  }
  return blRet;
}
BOOL CXXMLManager::GetAttr( XERCES_CPP_NAMESPACE::DOMElement *ptElement, 
                           const XMLCh *const pxhAttrName, 
                           DWORD& dwLong )
{
  assert(ptElement);  
  assert(pxhAttrName);
  BOOL blRet = TRUE;
  const XMLCh * pXMLCh = ptElement->getAttribute(pxhAttrName);
  CXString strContent;
  if (pXMLCh)
  {
    strContent = pXMLCh;  
    dwLong = strContent.GetHex();
  }
  else
  {
    blRet = FALSE;
  }
  return blRet;
}
BOOL CXXMLManager::GetAttr( XERCES_CPP_NAMESPACE::DOMElement *ptElement, 
                           const XMLCh *const pxhAttrName, 
                           double& dbData )
{
  assert(ptElement);  
  assert(pxhAttrName);
  BOOL blRet = TRUE;
  const XMLCh * pXMLCh = ptElement->getAttribute(pxhAttrName);
  CXStringW strContent;
  if (pXMLCh)
  {
    strContent = pXMLCh;  
    dbData = strContent.GetFloat();
  }
  else
  {
    blRet = FALSE;
  }
  return blRet;
}
DOMNodeList * CXXMLManager::GetChildByName( DOMElement *pParent, const XMLCh *const strName)
{
  if (!pParent)
  {
    return m_pDOMDocument->getElementsByTagName(strName);
  }
  else
  {
    return pParent->getElementsByTagName(strName);
  }  
}


void CXXMLManager::Detach()
{
  if (m_pDOMDocument)
  {
    m_pDOMDocument->release();
  }
  m_pDOMDocument = NULL;
  return;
}
BOOL CXXMLManager::Attach( LPCTSTR strPath)
{
  assert(strPath);  
  m_strXMLFileName = strPath;
  Detach();
  m_pDOMDocument = CHWXML::GetDocument(strPath);
  return !m_pDOMDocument;
}
DOMElement* CXXMLManager::Locate(DOMElement *pParent, const CXString& strKey)
{
  if (strKey.IsEmpty())
  {
    return dynamic_cast<DOMElement *>(m_pDOMDocument);
  }  
  DOMElement *pXMLRoot = pParent;
  if (!pXMLRoot)
  {
    return Locate(m_pDOMDocument, strKey);
  }
  vector<CXString> vstrSplit;
  VERIFY(strKey.Token('/', vstrSplit)); 
  assert(pXMLRoot);
  for (LONG nIndex = 0; nIndex < (LONG)vstrSplit.size(); nIndex ++)
  {
    DOMNodeList *pXMLNodeLst = pXMLRoot->getElementsByTagName(vstrSplit[nIndex]);
    if (!pXMLNodeLst || pXMLNodeLst->getLength() < 1)
    {
      VERIFY(pXMLRoot = AppendChildElement(pXMLRoot, vstrSplit[nIndex]));
    }
    else
    {
      VERIFY(pXMLRoot = dynamic_cast<DOMElement *>(pXMLNodeLst->item(0)));
    }
  }
  return pXMLRoot;
}
DOMElement* CXXMLManager::Locate(DOMElement *pParent, const vector<CXString>& vstrKey)
{
  assert(pParent);
  if (vstrKey.empty())
  {
    return pParent;
  } 
  DOMElement *pXMLRoot = pParent;
  assert(pXMLRoot);
  for (LONG nIndex = 0; nIndex < (LONG)vstrKey.size(); nIndex ++)
  {
    DOMNodeList *pXMLNodeLst = pXMLRoot->getElementsByTagName(vstrKey[nIndex]);
    if (!pXMLNodeLst || pXMLNodeLst->getLength() < 1)
    {
      VERIFY(pXMLRoot = AppendChildElement(pXMLRoot, vstrKey[nIndex]));
    }
    else
    {
      VERIFY(pXMLRoot = dynamic_cast<DOMElement *>(pXMLNodeLst->item(0)));
    }
  }
  return pXMLRoot;
}
DOMElement* CXXMLManager::Locate(XERCES_CPP_NAMESPACE::DOMDocument *pDocument, const CXString& strKey)
{
  if (strKey.IsEmpty())
  {
    return dynamic_cast<DOMElement *>(m_pDOMDocument);
  }
  vector<CXString> vstrSplit;
  VERIFY(strKey.Token('/', vstrSplit)); 
  if (!pDocument)
  {
    pDocument = m_pDOMDocument;
  }
  if (vstrSplit.empty())
  {
    return NULL;
  }
  DOMElement *pXMLRoot = NULL;
  DOMNodeList *pXMLNodeLst = pDocument->getElementsByTagName(vstrSplit[0]);
  if (!pXMLNodeLst || pXMLNodeLst->getLength() < 1)
  {
    VERIFY(pXMLRoot = AppendChildElement(pXMLRoot, vstrSplit[0]));
  }
  else
  {
    VERIFY(pXMLRoot = dynamic_cast<DOMElement *>(pXMLNodeLst->item(0)));
  }
  vstrSplit.erase(vstrSplit.begin());
  return Locate(pXMLRoot, vstrSplit);
}
DOMElement* CXXMLManager::Locate(const CXString& strKey)
{  
  return Locate(m_pDOMDocument, strKey);
}


BOOL CXXMLManager::GetAttribute( LPCWSTR szBaseKeyName, LPCWSTR szKey,
                                LPCWSTR szAttributeName, CXString & strValue,
                                LPCWSTR szDefatuleValue)
{
  CXString strBase = szBaseKeyName;  
  strBase.Trim(TEXT('/'));    
  DOMElement *pBase = Locate(m_pDOMDocument, strBase);
  if (!pBase)
  {
    strValue = szDefatuleValue;
    return FALSE;
  }
  return GetAttribute(pBase, szKey, szAttributeName, strValue, szDefatuleValue);  
}
BOOL CXXMLManager::GetAttribute(DOMElement *pParent, LPCWSTR szKey,
                                LPCWSTR szAttributeName, CXString & strValue,
                                LPCWSTR szDefatuleValue)
{
  assert(pParent);  
  CXString strKey = szKey;
  strKey.Trim(TEXT('/'));
  if (strKey.IsEmpty())
  {
    strValue = szDefatuleValue;
    return FALSE;
  }
  DOMElement *pBase = pParent;
  if (!pBase)
  {
    strValue = szDefatuleValue;
    return FALSE;
  }
  DOMElement *pKey = Locate(pBase, strKey);
  if (!pKey)
  {
    strValue = szDefatuleValue;
    return FALSE;
  }
  return GetAttribute(pKey, szAttributeName, strValue, szDefatuleValue); 
}
BOOL CXXMLManager::GetAttribute(DOMElement *pParent, LPCWSTR szAttributeName, 
                                CXString & strValue, LPCWSTR szDefatuleValue)
{
  assert(pParent);  
  DOMElement *pKey = pParent;
  if (!pKey)
  {
    strValue = szDefatuleValue;
    return FALSE;
  }
  if (pKey->hasAttribute(szAttributeName))
  {
    GetAttr(pKey, szAttributeName, strValue);    
  }
  else
  {    
    strValue = szDefatuleValue;
    if (!SetAttr(pKey, szAttributeName, strValue))
    {
      return FALSE;
    }    
  } 
  return TRUE;
}

BOOL CXXMLManager::SetAttribute( LPCWSTR szBaseKeyName, LPCWSTR szKey, 
                                LPCWSTR szAttributeName, LPCWSTR strValue )
{
  CXString strBase = szBaseKeyName;
  CXString strKey = szKey;
  strBase.Trim(TEXT('/'));
  strKey.Trim(TEXT('/'));
  if (strKey.IsEmpty())
  {    
    return FALSE;
  }
  DOMElement *pBase = Locate(strBase);
  if (!pBase)
  {    
    return FALSE;
  }
  return SetAttribute(pBase, szKey, szAttributeName, strValue);  
}

BOOL CXXMLManager::SetAttribute(DOMElement *pParent, LPCWSTR szKey, 
                                LPCWSTR szAttributeName, LPCWSTR strValue )
{
  assert(pParent);  
  CXString strKey = szKey;  
  strKey.Trim(TEXT('/'));
  if (strKey.IsEmpty())
  {    
    return FALSE;
  }
  DOMElement *pBase = pParent;
  if (!pBase)
  {    
    return FALSE;
  }
  DOMElement *pKey = Locate(pBase, strKey);
  if (!pKey)
  {    
    return FALSE;
  }
  return SetAttribute(pKey, szAttributeName, strValue);
}

BOOL CXXMLManager::SetAttribute(DOMElement *pParent, LPCWSTR szAttributeName, LPCWSTR strValue )
{
  assert(pParent);   
  DOMElement *pKey = pParent;  
  if (!pKey)
  {    
    return FALSE;
  }

  if (!SetAttr(pKey, szAttributeName, strValue))
  {
    return FALSE;
  }
  return TRUE;
}
BOOL CXXMLManager::GetAttributeLong( LPCWSTR strBase , LPCWSTR strkey, LPCWSTR strAttr, 
                                    LONG & nLongData, LONG nDefaultData )
{
  WCHAR awhBuffer[256];
  swprintf(awhBuffer, L"%d", nDefaultData);
  CXString strData;
  if (GetAttribute(strBase, strkey, strAttr, strData, awhBuffer))
  {    
    nLongData = strData.GetNumber();
    return TRUE;
  }
  else
  {
    nLongData = nDefaultData;
    return FALSE;
  }
}
BOOL CXXMLManager::GetAttributeDWORD( LPCWSTR strBase , LPCWSTR strkey, LPCWSTR strAttr, 
                                    DWORD & dwLongData, DWORD dwDefaultData )
{
  WCHAR awhBuffer[256];
  swprintf(awhBuffer, L"%x", dwDefaultData);
  CXString strData;
  if (GetAttribute(strBase, strkey, strAttr, strData, awhBuffer))
  {    
    dwLongData = strData.GetHex();
    return TRUE;
  }
  else
  {
    dwLongData = dwDefaultData;
    return FALSE;
  }
}
BOOL CXXMLManager::GetAttributeDouble( LPCWSTR strBase , LPCWSTR strkey, LPCWSTR strAttr, 
                                     double & dbLongData, double dbDefaultData )
{
  WCHAR awhBuffer[256];
  swprintf(awhBuffer, L"%f", dbDefaultData);
  CXString strData;
  if (GetAttribute(strBase, strkey, strAttr, strData, awhBuffer))
  {    
    dbLongData = strData.GetFloat();
    return TRUE;
  }
  else
  {
    dbLongData = dbDefaultData;
    return FALSE;
  }
}
BOOL CXXMLManager::SetAttributeLong( LPCWSTR szBase, LPCWSTR szKey, LPCWSTR szAttr, const LONG &nData)
{
  WCHAR awhBuffer[50];
  swprintf(awhBuffer, L"%d", nData);
  return SetAttribute( szBase, szKey, szAttr, awhBuffer);
}
BOOL CXXMLManager::SetAttributeDWORD( LPCWSTR szBase, LPCWSTR szKey, LPCWSTR szAttr, const DWORD &dwData)
{
  WCHAR awhBuffer[50];
  swprintf(awhBuffer, L"%x", dwData);
  return SetAttribute( szBase, szKey, szAttr, awhBuffer);
}
BOOL CXXMLManager::SetAttributeDouble( LPCWSTR szBase, LPCWSTR szKey, LPCWSTR szAttr, const double &dbData)
{
  WCHAR awhBuffer[50];
  swprintf(awhBuffer, L"%f", dbData);
  return SetAttribute( szBase, szKey, szAttr, awhBuffer);
}
CXXMLManager & CXXMLManager::GetInstance()
{
 static CXXMLManager tXMLManager;
 return tXMLManager;
}