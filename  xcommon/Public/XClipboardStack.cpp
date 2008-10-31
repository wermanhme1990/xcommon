/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XClipboardStack.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/09/25
*********************************************************************/
#include "stdafx.h"
#include "XClipboardStack.h"


CXClipboardStack::tagClipboardDataPiece::tagClipboardDataPiece()
{
	m_nFormat = 0;	
	memset(m_szFormatName, 0, sizeof(m_szFormatName));
	m_hData = NULL;
}
CXClipboardStack::tagClipboardDataPiece::~tagClipboardDataPiece()
{
	m_nFormat = 0;	
	memset(m_szFormatName, 0, sizeof(m_szFormatName));
	m_hData = NULL;	
}
void CXClipboardStack::tagClipboardDataPiece::Release()
{	
	if (m_hData)
	{			
		::GlobalFree( m_hData );
		m_hData = NULL;
	}	
	m_nFormat = 0;	
	memset(m_szFormatName, 0, sizeof(m_szFormatName));
}
CXClipboardStack::CXClipboardStack()
{

}

CXClipboardStack::~CXClipboardStack()
{		
	while(!m_stkClipboard.empty())
	{
		std::list<LPXClipboardDataPiece> *plstTop = NULL;
		plstTop = m_stkClipboard.top();			
		if (plstTop)
		{
			std::list<LPXClipboardDataPiece>::iterator itlstTravel = plstTop->begin();
			while (itlstTravel != plstTop->end())
			{
				LPXClipboardDataPiece& pClipboardData = *itlstTravel;
				assert(pClipboardData);		
				if (pClipboardData)
				{
					pClipboardData->Release();
					delete pClipboardData;
					pClipboardData = NULL;
				}					 
				itlstTravel ++;
			}
			delete plstTop;
			plstTop = NULL;
		}			
		m_stkClipboard.pop();			
	}		
}

BOOL CXClipboardStack::Push()
{	
	if( ! ::OpenClipboard(NULL) )
	{		
		return FALSE;
	}

	std::list<LPXClipboardDataPiece> *plstThisTime = NULL;
	VERIFY(plstThisTime = new std::list<LPXClipboardDataPiece>);
	UINT uiFormat = 0;
	while(uiFormat = ::EnumClipboardFormats(uiFormat))
	{
		LPXClipboardDataPiece lpXData = NULL;			
		VERIFY(lpXData = new XClipboardDataPiece);		
		lpXData->m_nFormat = uiFormat;		
		if( uiFormat <= 14 )
		{
			lpXData->m_szFormatName[0] = 0;
		}
		else if( !GetClipboardFormatName(uiFormat, lpXData->m_szFormatName, MAX_PATH))
		{
			lpXData->m_szFormatName[0] = 0;
		}		
		HANDLE hMem = ::GetClipboardData(uiFormat);
		if( hMem)
		{			
			LONG   nSize = (LONG)::GlobalSize(hMem);
			LPVOID pMem = ::GlobalLock(hMem);
			lpXData->m_hData   = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, nSize );
			LPVOID pNewMem = ::GlobalLock( lpXData->m_hData );
			memcpy(pNewMem, pMem, nSize);
			::GlobalUnlock(hMem);
			::GlobalUnlock(lpXData->m_hData);
		}				
		if (lpXData)
		{
			plstThisTime->push_back(lpXData);
		}		
	}	
	m_stkClipboard.push(plstThisTime);
	::CloseClipboard();
	return TRUE;
} 

BOOL CXClipboardStack::Pop()
{
	if (m_stkClipboard.empty())
	{
		return FALSE;
	}	
	if(!::OpenClipboard(NULL))
		return FALSE;

	::EmptyClipboard();		
	std::list<LPXClipboardDataPiece> *plstTop = NULL;
	plstTop = m_stkClipboard.top();
	UINT uiFormat = 0;
	if (plstTop)
	{
		std::list<LPXClipboardDataPiece>::iterator itlstTravel = plstTop->begin();
		while (itlstTravel != plstTop->end())
		{
			LPXClipboardDataPiece& pClipboardData = *itlstTravel;
			assert(pClipboardData);		
			if (pClipboardData)
			{
				UINT uiFormat = pClipboardData->m_nFormat;
				if( pClipboardData->m_szFormatName[0])
				{
					UINT uiFormatTmp = RegisterClipboardFormat( pClipboardData->m_szFormatName );
					if( uiFormatTmp > 0 ) 
					{
						uiFormat = uiFormatTmp;
					}
				}
				::SetClipboardData( uiFormat, pClipboardData->m_hData);
				delete pClipboardData;
				pClipboardData = NULL;
			}		
			itlstTravel ++;
		}
		delete plstTop;
		plstTop = NULL;
	}
	::CloseClipboard();	
	m_stkClipboard.pop();
	return TRUE;
}
BOOL CXClipboardStack::Empty()
{
	return m_stkClipboard.empty();
}
