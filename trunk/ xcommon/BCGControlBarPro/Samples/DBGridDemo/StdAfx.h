// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__42C8D8F3_2924_4FC0_A559_92622488EAEC__INCLUDED_)
#define AFX_STDAFX_H__42C8D8F3_2924_4FC0_A559_92622488EAEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <BCGCBProInc.h>			// BCGControlBar Pro

#pragma warning (push)
#pragma warning (disable : 4192)
#pragma warning (disable : 4146)

#import "C:\\WINDOWS\\ServicePackFiles\\i386\\msado15.dll" rename ( "BOF", "ADOBOF" ) rename ( "EOF", "ADOEOF" ) rename ( "BOF", "ADOBOF" )
#import "C:\\program files\\common files\\system\\ole db\\oledb32.dll"
using namespace ADODB;

#pragma warning( pop )

#include <BCGPADOGridCtrl.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__42C8D8F3_2924_4FC0_A559_92622488EAEC__INCLUDED_)
