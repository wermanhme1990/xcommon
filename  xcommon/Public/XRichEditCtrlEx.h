
#ifndef X_RICHEDITCTRLEX
#define X_RICHEDITCTRLEX

#if _MSC_VER >= 1000
#pragma once
#endif

#include "Ext_Type.h"

class _AFX_RICHEDITEX_STATE
{
public:
  _AFX_RICHEDITEX_STATE() ;
  virtual ~_AFX_RICHEDITEX_STATE();
public:

  HINSTANCE m_hInstRichEdit20;
} ;


BOOL PASCAL AfxInitRichEditEx() ;


class DLLXEXPORT CXRichEditCtrlEx : public CRichEditCtrl
{
  ////////////////////////////////////////////////////////////////////////////
  // Construction
  ////////////////////////////////////////////////////////////////////////////
	DECLARE_DYNAMIC(CXRichEditCtrlEx)
public:

  CXRichEditCtrlEx() ;
  virtual ~CXRichEditCtrlEx() ;

  ////////////////////////////////////////////////////////////////////////////
  // Operations
  ////////////////////////////////////////////////////////////////////////////

public:

  virtual BOOL AutoURLDetect( BOOL in_fEnable ) ;

  ////////////////////////////////////////////////////////////////////////////
  // ClassWizard generated virtual function overrides
  ////////////////////////////////////////////////////////////////////////////

  //{{AFX_VIRTUAL( CXRichEditCtrlEx )
public:

  virtual BOOL Create(DWORD dwStyle, const RECT& rcRect, CWnd* pParentWnd, LONG nID = -1) ;
  //}}AFX_VIRTUAL

  ////////////////////////////////////////////////////////////////////////////
  // Generated message map functions
  ////////////////////////////////////////////////////////////////////////////
  void Enable( BOOL b = TRUE )
  {
    ModifyStyle(b ? WS_DISABLED : 0 , b ?  0 : WS_DISABLED, 0) ; 
  }
  void Disable( BOOL b = FALSE )
  {
    Enable( b ) ;
  }
  void ReadOnly( BOOL b = TRUE )
  { 
    SetReadOnly( b ) ;
  }
  void Writable( BOOL b = TRUE )
  {
    ReadOnly( !b ) ; 
  }
protected:
  //{{AFX_MSG( CXRichEditCtrlEx )
  //}}AFX_MSG 
  DECLARE_MESSAGE_MAP()
} ;


inline BOOL CXRichEditCtrlEx::AutoURLDetect( BOOL in_fEnable )
{ 
  ASSERT(::IsWindow( m_hWnd ) ) ; 
  return ( BOOL )::SendMessage( m_hWnd, EM_AUTOURLDETECT, in_fEnable, 0 ) ;
}


#endif
