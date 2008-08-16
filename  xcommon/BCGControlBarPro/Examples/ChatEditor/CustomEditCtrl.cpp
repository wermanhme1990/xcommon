// CustomEditCtrl.cpp: implementation of the CCustomEditCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomEditCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomEditCtrl::CCustomEditCtrl()
{
	m_nMaxScrollWidth = 80;
	m_bCopyRTFToClipboard = TRUE;
}

CCustomEditCtrl::~CCustomEditCtrl()
{

}

BEGIN_MESSAGE_MAP(CCustomEditCtrl, CBCGPEditCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CCustomEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	UpdateScrollBars ();
	return 0;
}

void CCustomEditCtrl::UpdateScrollBars ()
{
	CBCGPEditCtrl::UpdateScrollBars ();

	ShowScrollBar (SB_HORZ, FALSE); // To disable horizontal scrollbar
}
