/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XClipboardStack.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/09/25
*********************************************************************/
#ifndef HWX_CLIPBOARDSTACK_H
#define HWX_CLIPBOARDSTACK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ext_Type.h"
#include <list>
#include <stack>
/*
#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12
#define CF_UNICODETEXT      13
#define CF_ENHMETAFILE      14

#if(WINVER >= 0x0400)
#define CF_HDROP            15
#define CF_LOCALE           16
#define CF_MAX              17
#endif // WINVER >= 0x0400

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E
*/

class CXClipboardStack
{
public:
	BOOL Push();
	BOOL Pop();
	BOOL Empty();
public:
	CXClipboardStack();
	~CXClipboardStack();
protected:
	typedef struct tagClipboardDataPiece
	{
		UINT   m_nFormat;
		TCHAR  m_szFormatName[MAX_PATH];
		HANDLE m_hData;
		tagClipboardDataPiece();
		~tagClipboardDataPiece();
		void Release();
	}XClipboardDataPiece, *LPXClipboardDataPiece;
	
	std::stack<std::list<LPXClipboardDataPiece> *> m_stkClipboard;
};

#endif 
