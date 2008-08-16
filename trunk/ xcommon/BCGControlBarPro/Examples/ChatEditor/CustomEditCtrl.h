// CustomEditCtrl.h: interface for the CCustomEditCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMEDITCTRL_H__3BED6DCA_EFF3_4B30_8473_75259525814A__INCLUDED_)
#define AFX_CUSTOMEDITCTRL_H__3BED6DCA_EFF3_4B30_8473_75259525814A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomEditCtrl : public CBCGPEditCtrl  
{
public:
	CCustomEditCtrl();
	virtual ~CCustomEditCtrl();

	virtual void UpdateScrollBars ();

protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CUSTOMEDITCTRL_H__3BED6DCA_EFF3_4B30_8473_75259525814A__INCLUDED_)
