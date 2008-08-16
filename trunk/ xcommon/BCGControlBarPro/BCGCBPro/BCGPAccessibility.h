//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

#ifndef __BCGPACCESSIBILITY_H
#define __BCGPACCESSIBILITY_H

struct BCGPACCDATA
{
	CWnd*   m_pWnd;
	WPARAM  m_wParam;
	LPARAM  m_lParam;
	int     m_nObjType;
	void*   m_objData;
};

struct BCGPACC_TASKINFO
{
	int	m_nTask;
	int m_nTaskGroup;
	CString m_strText;

	BCGPACC_TASKINFO ()
	{
		m_nTask = -1;
		m_nTaskGroup = -1;
	}
};

struct BCGPACC_PROPINFO
{
	BOOL    m_bGroup;
	BOOL    m_bEnable;
	CString m_strName;
	CString m_strValue;
};


typedef enum BCGP_ACC_TYPE
{
		ACC_POPUPMENU	= 1,
		ACC_TASKPANE	= 2,
		ACC_PROPLIST	= 3,
};

#ifndef WM_GETOBJECT
#define WM_GETOBJECT	    0x003D
#endif  

BCGCBPRODLLEXPORT extern UINT BCGM_ACCGETOBGECT;
BCGCBPRODLLEXPORT extern UINT BCGM_ACCHITTEST;

#ifndef OBJID_CLIENT
#define OBJID_CLIENT					0xFFFFFFFC
#endif

#ifndef	OBJID_WINDOW
#define OBJID_WINDOW					0x00000000
#endif

#ifndef CHILDID_SELF
#define CHILDID_SELF					0
#endif

#ifndef EVENT_SYSTEM_MENUEND
#define EVENT_SYSTEM_MENUEND            0x0005
#endif

#ifndef EVENT_SYSTEM_MENUPOPUPSTART
#define EVENT_SYSTEM_MENUPOPUPSTART     0x0006
#endif

#ifndef EVENT_SYSTEM_MENUPOPUPEND
#define EVENT_SYSTEM_MENUPOPUPEND       0x0007
#endif

#ifndef EVENT_OBJECT_FOCUS
#define EVENT_OBJECT_FOCUS              0x8005
#endif 

#ifndef EVENT_SYSTEM_MENUSTART
#define EVENT_SYSTEM_MENUSTART			0x0004
#endif

#endif	// __BCGPACCESSIBILITY_H