// WorkspaceObj.cpp: implementation of the CWorkspaceObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPOutlookDemo.h"
#include "WorkspaceObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkspaceObj::CWorkspaceObj (const CString& strName, const UINT uiCmd, 
							  const int iIconIndex,
							  const UINT uiDefaultNewAction) :
	m_strName (strName),
	m_uiCmd (uiCmd),
	m_iIconIndex (iIconIndex),
	m_uiDefaultNewAction (uiDefaultNewAction)
{
}

CWorkspaceObj::~CWorkspaceObj()
{

}
