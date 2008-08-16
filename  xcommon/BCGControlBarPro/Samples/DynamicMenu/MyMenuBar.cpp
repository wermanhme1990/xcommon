// MyMenuBar.cpp: implementation of the CMyMenuBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DynamicMenu.h"
#include "MyMenuBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMenuBar::CMyMenuBar()
{

}

CMyMenuBar::~CMyMenuBar()
{

}

BOOL CMyMenuBar::RestoreOriginalstate ()
{
	if (!CBCGPMenuBar::RestoreOriginalstate ())
	{
		return FALSE;
	}

	AddSubMenu ();
	return TRUE;
}

void CMyMenuBar::AddSubMenu ()
{
	CMenu menu;
	menu.LoadMenu (IDR_DYNAMIC_MENU); 

	InsertButton (CBCGPToolbarMenuButton (0, menu, -1, _T ("&DynamicMenu")));

	AdjustLayout();
	AdjustSizeImmediate ();
}
