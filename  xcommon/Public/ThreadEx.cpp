/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	ThreadEx.cpp
文件内容:	  简单的线程
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/04/30
*********************************************************************/
#include "StdAfx.h"
#include "Threadex.h"
#include <assert.h>
CXThread::CXThread()
{	
	m_pEvent = NULL;
	m_pThread = NULL;
	m_unDelay = 0;
}
CXThread::~CXThread()
{
	Stop();
}

void CXThread::Stop()
{
	if (NULL != m_pEvent)
	{
		m_pEvent->SetEvent();
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);//等待线程结束
		delete m_pThread;
		m_pThread = NULL;
		delete m_pEvent;
		m_pEvent = NULL;
	}
	else
	{
		assert(NULL == m_pThread);
	}
}

BOOL CXThread::Begin(AFX_THREADPROC pvCallBackFunction, void *pvVariant)
{
	assert(NULL != pvCallBackFunction);	
	if (NULL == pvCallBackFunction)
	{
		return FALSE;
	}
	if(NULL != m_pEvent)
	{//防止线程还没有结束 就再begin运行
		assert(NULL != m_pThread);
		Stop();		
	}
	assert(NULL == m_pThread);
	m_pEvent = new CEvent(FALSE, TRUE);		
	
	m_pThread = AfxBeginThread((AFX_THREADPROC)pvCallBackFunction, pvVariant, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	if (NULL != m_pThread)
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}

	return(NULL != m_pThread);	
}

void CXThread::TryExit()
{
	if (NULL == m_pEvent)
	{//只有当前线程启动之后才可以判断是否可以结束
		return;
	}	
	if(WAIT_OBJECT_0 == WaitForSingleObject(m_pEvent->m_hObject, m_unDelay))   
	{	
		m_pEvent->ResetEvent();		
		AfxEndThread(NULL);			
	}
}

void CXThread::SetDelay(unsigned long unDelay)
{
	m_unDelay = unDelay;
}