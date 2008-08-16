// SampleUserTool.cpp: implementation of the CSampleUserTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTools.h"
#include "SampleUserTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CSampleUserTool, CBCGPUserTool, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSampleUserTool::CSampleUserTool()
{
}

CSampleUserTool::~CSampleUserTool()
{
}

void CSampleUserTool::Serialize (CArchive& ar)
{
	CBCGPUserTool::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_strProp1;
		ar >> m_strProp2;
	}
	else
	{
		ar << m_strProp1;
		ar << m_strProp2;
	}
}

BOOL CSampleUserTool::Invoke ()
{
	CString str;
	str.Format (_T("Invoke user tool '%s'. \r\n\r\nParameters:\r\nParam1: %s\r\nParam2: %s"),
				m_strLabel, m_strProp1, m_strProp2);

	AfxMessageBox (str);
	return TRUE;
}
