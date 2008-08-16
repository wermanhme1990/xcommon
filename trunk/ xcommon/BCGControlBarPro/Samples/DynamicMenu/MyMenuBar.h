// MyMenuBar.h: interface for the CMyMenuBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMENUBAR_H__9054C2C9_91D7_486B_802F_3EDB867F0BD7__INCLUDED_)
#define AFX_MYMENUBAR_H__9054C2C9_91D7_486B_802F_3EDB867F0BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyMenuBar : public CBCGPMenuBar  
{
public:
	CMyMenuBar();
	virtual ~CMyMenuBar();

	void AddSubMenu ();

protected:
	virtual BOOL RestoreOriginalstate ();
};

#endif // !defined(AFX_MYMENUBAR_H__9054C2C9_91D7_486B_802F_3EDB867F0BD7__INCLUDED_)
