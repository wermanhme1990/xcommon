/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	ThreadEx.h					
文件内容:	  简单的线程
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/04/30
*********************************************************************/
#ifndef X_THREADEX
#define X_THREADEX

#include "Ext_Type.h"

class DLLXEXPORT CXThread
{
public:
	CXThread();
	~CXThread();
	void Stop();
	BOOL Begin(AFX_THREADPROC, void *pvVariant);
	void TryExit();
	void SetDelay(unsigned long);
private:	
	CWinThread *m_pThread;
	CEvent *m_pEvent;
	unsigned long m_unDelay;
};


#endif//X_THREADEX