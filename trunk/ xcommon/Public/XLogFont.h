
/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XLogFont.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/18
*********************************************************************/
#ifndef HWX_LOGFONT_H
#define HWX_LOGFONT_H
class CXLogFont : public LOGFONT
{
public:
	CXLogFont();
	CXLogFont(const LOGFONT& logfont); //<combine CXLogFont::CXLogFont>
	const CXLogFont& operator=(const CXLogFont&);
	const CXLogFont& operator =(const LOGFONT&);
public:
	void operator=(LOGFONT& logfont);
	BOOL operator<(const CXLogFont&);

public:
	DWORD dwType;			// Used to hold the font type, i.e. TT_FONT, DEVICE_FONT.
};

#endif