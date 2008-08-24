/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XFlash.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/24
*********************************************************************/
#include "stdafx.h"
#include "XFlash.h"

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CXFlash, CWnd)

LONG CXFlash::GetReadyState()
{
	LONG result;
	InvokeHelper(DISPID_READYSTATE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LONG CXFlash::GetTotalFrames()
{
	LONG result;
	InvokeHelper(0x7c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CXFlash::GetPlaying()
{
	BOOL result;
	InvokeHelper(0x7d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetPlaying(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LONG CXFlash::GetQuality()
{
	LONG result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CXFlash::SetQuality(LONG nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LONG CXFlash::GetScaleMode()
{
	LONG result;
	InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CXFlash::SetScaleMode(LONG nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LONG CXFlash::GetAlignMode()
{
	LONG result;
	InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CXFlash::SetAlignMode(LONG nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x79, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LONG CXFlash::GetBackgroundColor()
{
	LONG result;
	InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CXFlash::SetBackgroundColor(LONG nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CXFlash::GetLoop()
{
	BOOL result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetLoop(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CXFlash::GetMovie()
{
	CString result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetMovie(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LONG CXFlash::GetFrameNum()
{
	LONG result;
	InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CXFlash::SetFrameNum(LONG nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CXFlash::SetZoomRect(LONG left, LONG top, LONG right, LONG bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 left, top, right, bottom);
}

void CXFlash::Zoom(LONG factor)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 factor);
}

void CXFlash::Pan(LONG x, LONG y, LONG mode)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, mode);
}

void CXFlash::Play()
{
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::Stop()
{
	InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::Back()
{
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::Forward()
{
	InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::Rewind()
{
	InvokeHelper(0x74, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::StopPlay()
{
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CXFlash::GotoFrame(LONG FrameNum)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FrameNum);
}

LONG CXFlash::CurrentFrame()
{
	LONG result;
	InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CXFlash::IsPlaying()
{
	BOOL result;
	InvokeHelper(0x81, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LONG CXFlash::PercentLoaded()
{
	LONG result;
	InvokeHelper(0x82, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CXFlash::FrameLoaded(LONG FrameNum)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x83, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FrameNum);
	return result;
}

LONG CXFlash::FlashVersion()
{
	LONG result;
	InvokeHelper(0x84, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CXFlash::GetWMode()
{
	CString result;
	InvokeHelper(0x85, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetWMode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x85, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetSAlign()
{
	CString result;
	InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetSAlign(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CXFlash::GetMenu()
{
	BOOL result;
	InvokeHelper(0x87, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetMenu(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x87, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CXFlash::GetBase()
{
	CString result;
	InvokeHelper(0x88, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetBase(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x88, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetScale()
{
	CString result;
	InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetScale(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x89, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CXFlash::GetDeviceFont()
{
	BOOL result;
	InvokeHelper(0x8a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetDeviceFont(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CXFlash::GetEmbedMovie()
{
	BOOL result;
	InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetEmbedMovie(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CXFlash::GetBGColor()
{
	CString result;
	InvokeHelper(0x8c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetBGColor(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetQuality2()
{
	CString result;
	InvokeHelper(0x8d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetQuality2(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void CXFlash::LoadMovie(LONG layer, LPCTSTR url)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x8e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 layer, url);
}

void CXFlash::TGotoFrame(LPCTSTR target, LONG FrameNum)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, FrameNum);
}

void CXFlash::TGotoLabel(LPCTSTR target, LPCTSTR label)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x90, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, label);
}

LONG CXFlash::TCurrentFrame(LPCTSTR target)
{
	LONG result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x91, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		target);
	return result;
}

CString CXFlash::TCurrentLabel(LPCTSTR target)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x92, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		target);
	return result;
}

void CXFlash::TPlay(LPCTSTR target)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x93, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target);
}

void CXFlash::TStopPlay(LPCTSTR target)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target);
}

void CXFlash::SetVariable(LPCTSTR name, LPCTSTR value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 name, value);
}

CString CXFlash::GetVariable(LPCTSTR name)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x98, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		name);
	return result;
}

void CXFlash::TSetProperty(LPCTSTR target, LONG property, LPCTSTR value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(0x99, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, property, value);
}

CString CXFlash::TGetProperty(LPCTSTR target, LONG property)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x9a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		target, property);
	return result;
}

void CXFlash::TCallFrame(LPCTSTR target, LONG FrameNum)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x9b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, FrameNum);
}

void CXFlash::TCallLabel(LPCTSTR target, LPCTSTR label)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x9c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, label);
}

void CXFlash::TSetPropertyNum(LPCTSTR target, LONG property, double value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_R8;
	InvokeHelper(0x9d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, property, value);
}

double CXFlash::TGetPropertyNum(LPCTSTR target, LONG property)
{
	double result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x9e, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		target, property);
	return result;
}

double CXFlash::TGetPropertyAsNumber(LPCTSTR target, LONG property)
{
	double result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xac, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		target, property);
	return result;
}

CString CXFlash::GetSWRemote()
{
	CString result;
	InvokeHelper(0x9f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetSWRemote(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetFlashVars()
{
	CString result;
	InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetFlashVars(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xaa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetAllowScriptAccess()
{
	CString result;
	InvokeHelper(0xab, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetAllowScriptAccess(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xab, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CXFlash::GetMovieData()
{
	CString result;
	InvokeHelper(0xbe, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXFlash::SetMovieData(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xbe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPUNKNOWN CXFlash::GetInlineData()
{
	LPUNKNOWN result;
	InvokeHelper(0xbf, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void CXFlash::SetInlineData(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xbf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CXFlash::GetSeamlessTabbing()
{
	BOOL result;
	InvokeHelper(0xc0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CXFlash::SetSeamlessTabbing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}
BOOL CXFlash::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/ )
{
	return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}
BOOL CXFlash::Create( LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist /*= NULL*/, BOOL bStorage /*= FALSE*/, BSTR bstrLicKey /*= NULL*/ )
{
	return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey);
}
CLSID const& CXFlash::GetClsid()
{
	static CLSID const clsid
		= { 0xd27cdb6e, 0xae6d, 0x11cf, { 0x96, 0xb8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
	return clsid;
}