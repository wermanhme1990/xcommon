

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "StdAfx.h"
#include "XRichEditCtrlEx.h"
_AFX_RICHEDITEX_STATE::_AFX_RICHEDITEX_STATE()
{
  m_hInstRichEdit20 = NULL ;
}

_AFX_RICHEDITEX_STATE::~_AFX_RICHEDITEX_STATE()
{
  if( m_hInstRichEdit20 != NULL )
  {
    ::FreeLibrary( m_hInstRichEdit20 ) ;
  }
}

_AFX_RICHEDITEX_STATE _afxRichEditStateEx ;

BOOL PASCAL AfxInitRichEditEx()
{
  if( ! ::AfxInitRichEdit() )
  {
    return FALSE ;
  }

  _AFX_RICHEDITEX_STATE* l_pState = &_afxRichEditStateEx ;

  if( l_pState->m_hInstRichEdit20 == NULL )
  {
    l_pState->m_hInstRichEdit20 = LoadLibrary(TEXT("RICHED20.DLL")) ;
  }

  return l_pState->m_hInstRichEdit20 != NULL ;
}

IMPLEMENT_DYNAMIC(CXRichEditCtrlEx, CRichEditCtrl)
CXRichEditCtrlEx::CXRichEditCtrlEx()
{
}

CXRichEditCtrlEx::~CXRichEditCtrlEx()
{
}

BEGIN_MESSAGE_MAP( CXRichEditCtrlEx, CRichEditCtrl)

  //{{AFX_MSG_MAP( CXRichEditCtrlEx )
  //}}AFX_MSG_MAP

END_MESSAGE_MAP()

BOOL CXRichEditCtrlEx::Create( DWORD dwStyle, const RECT& rcRect, CWnd*	pParentWnd, LONG nID)
{
  if(!::AfxInitRichEditEx())
  {
    return FALSE ;
  }
  LONG nCtrlID = _max(nID, 0);
  CWnd* l_pWnd = this;
  BOOL blRet = TRUE;
#ifdef UNICODE
  {
    blRet = l_pWnd->Create( _T( "RichEdit20W" ), NULL, dwStyle, rcRect, pParentWnd, nCtrlID) ;
  }
#else
  {
    blRet = l_pWnd->Create( _T( "RichEdit20A" ), NULL, dwStyle, rcRect, pParentWnd, nCtrlID) ;
  }
#endif
  return blRet;
}
