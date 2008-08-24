/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XFlash.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/24
*********************************************************************/

#ifndef HWX_FLASH_H
#define HWX_FLASH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXFlash : public CWnd
{
protected:
	DECLARE_DYNCREATE(CXFlash)
public:	
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,	const RECT& rect,
		CWnd* pParentWnd, UINT nID,	CCreateContext* pContext = NULL);

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,	const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,	BSTR bstrLicKey = NULL);
	// Attributes
protected:
	CLSID const& GetClsid();
// Operations
public:
	LONG GetReadyState();
	LONG GetTotalFrames();
	BOOL GetPlaying();
	void SetPlaying(BOOL bNewValue);
	LONG GetQuality();
	void SetQuality(LONG nNewValue);
	LONG GetScaleMode();
	void SetScaleMode(LONG nNewValue);
	LONG GetAlignMode();
	void SetAlignMode(LONG nNewValue);
	LONG GetBackgroundColor();
	void SetBackgroundColor(LONG nNewValue);
	BOOL GetLoop();
	void SetLoop(BOOL bNewValue);
	CString GetMovie();
	void SetMovie(LPCTSTR lpszNewValue);
	LONG GetFrameNum();
	void SetFrameNum(LONG nNewValue);
	void SetZoomRect(LONG left, LONG top, LONG right, LONG bottom);
	void Zoom(LONG factor);
	void Pan(LONG x, LONG y, LONG mode);
	void Play();
	void Stop();
	void Back();
	void Forward();
	void Rewind();
	void StopPlay();
	void GotoFrame(LONG FrameNum);
	LONG CurrentFrame();
	BOOL IsPlaying();
	LONG PercentLoaded();
	BOOL FrameLoaded(LONG FrameNum);
	LONG FlashVersion();
	CString GetWMode();
	void SetWMode(LPCTSTR lpszNewValue);
	CString GetSAlign();
	void SetSAlign(LPCTSTR lpszNewValue);
	BOOL GetMenu();
	void SetMenu(BOOL bNewValue);
	CString GetBase();
	void SetBase(LPCTSTR lpszNewValue);
	CString GetScale();
	void SetScale(LPCTSTR lpszNewValue);
	BOOL GetDeviceFont();
	void SetDeviceFont(BOOL bNewValue);
	BOOL GetEmbedMovie();
	void SetEmbedMovie(BOOL bNewValue);
	CString GetBGColor();
	void SetBGColor(LPCTSTR lpszNewValue);
	CString GetQuality2();
	void SetQuality2(LPCTSTR lpszNewValue);
	void LoadMovie(LONG layer, LPCTSTR url);
	void TGotoFrame(LPCTSTR target, LONG FrameNum);
	void TGotoLabel(LPCTSTR target, LPCTSTR label);
	LONG TCurrentFrame(LPCTSTR target);
	CString TCurrentLabel(LPCTSTR target);
	void TPlay(LPCTSTR target);
	void TStopPlay(LPCTSTR target);
	void SetVariable(LPCTSTR name, LPCTSTR value);
	CString GetVariable(LPCTSTR name);
	void TSetProperty(LPCTSTR target, LONG property, LPCTSTR value);
	CString TGetProperty(LPCTSTR target, LONG property);
	void TCallFrame(LPCTSTR target, LONG FrameNum);
	void TCallLabel(LPCTSTR target, LPCTSTR label);
	void TSetPropertyNum(LPCTSTR target, LONG property, double value);
	double TGetPropertyNum(LPCTSTR target, LONG property);
	double TGetPropertyAsNumber(LPCTSTR target, LONG property);
	CString GetSWRemote();
	void SetSWRemote(LPCTSTR lpszNewValue);
	CString GetFlashVars();
	void SetFlashVars(LPCTSTR lpszNewValue);
	CString GetAllowScriptAccess();
	void SetAllowScriptAccess(LPCTSTR lpszNewValue);
	CString GetMovieData();
	void SetMovieData(LPCTSTR lpszNewValue);
	LPUNKNOWN GetInlineData();
	void SetInlineData(LPUNKNOWN newValue);
	BOOL GetSeamlessTabbing();
	void SetSeamlessTabbing(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}

#endif //HWX_FLASH_H
