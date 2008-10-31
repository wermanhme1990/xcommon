/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   HWXPackage.h          
文件内容:  数据包定义 可以用做内存的管理 
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/
#include "stdafx.h"
#include "HWXPackage.h"

#include "Ext_Memory.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#define PACKAGE_Mem_Inc    (256)
//////////////////////////////////////////////////////////////////////////
CXPackage::CXPackage(BOOL blAlign)
{
  m_pbBuffer = NULL;
  m_dwSize = 0;
  m_dwCapacity = CalcUsedBytes(m_dwSize);
  m_dwIncStep = PACKAGE_Mem_Inc;
  m_bl32BitAlign = blAlign;
}

CXPackage::CXPackage(const void *pvContent, DWORD dwSize, BOOL blAlign)
{
  m_pbBuffer = NULL;
  m_dwSize = 0;
  m_dwCapacity = CalcUsedBytes(m_dwSize);
  m_dwIncStep = PACKAGE_Mem_Inc;
  m_bl32BitAlign = blAlign;
  ResetEx(pvContent, dwSize);  
}

CXPackage::CXPackage(const CXPackage &tB)
{
  m_pbBuffer = NULL;
  m_dwSize = 0;
  m_dwCapacity = CalcUsedBytes(m_dwSize);
  m_dwIncStep = PACKAGE_Mem_Inc;
  m_bl32BitAlign = tB.m_bl32BitAlign;
  if (!tB.IsEmpty())
  {
    ResetEx(tB.m_pbBuffer, tB.m_dwSize);
  }  
}
//////////////////////////////////////////////////////////////////////////
CXPackage::~CXPackage()
{
  Release();
}

//释放缓存区
void CXPackage::Release()
{
  Mem_FreeMemory((void **)&m_pbBuffer);
  m_dwSize = 0;
  m_dwCapacity = 0;
}
//清空里面元素(不释放内存)
void CXPackage::Clear()
{
#if defined(_DEBUG) || defined(DEBUG)
  {
    if (m_dwCapacity)
    {
      assert(m_pbBuffer);
      Mem_ZeroMemory(m_pbBuffer, m_dwCapacity);
    }  
  }
#endif  
  m_dwSize = 0;  
}
//////////////////////////////////////////////////////////////////////////
//重新设置数据
BOOL CXPackage::Reset(const void *pvContent, DWORD dwSize)
{
  assert(pvContent);
  assert(m_pbBuffer != pvContent);
  Clear();
  return Push(pvContent, dwSize);  
}

BOOL CXPackage::ResetEx(const void *pvContent, DWORD dwSize)
{//为了防止内存的浪费  这里拷贝有效的内存块
  assert(pvContent);  
  assert(m_pbBuffer != pvContent);
  BOOL blRet = TRUE;
  
  if (pvContent)
  {   
    DWORD dwNewSize = CalcUsedBytes(dwSize);
    if(FALSE != Mem_ResizeMemory((void **)&m_pbBuffer, dwNewSize))
    {  
        Mem_CopyMemory(m_pbBuffer, (void *)pvContent, dwSize);
        m_dwSize = dwSize;
        m_dwCapacity = dwNewSize;      
    }
    else
    {      
      blRet = FALSE;
    }
  }
  else
  {    
    Release();
  }
  return blRet;
}
//////////////////////////////////////////////////////////////////////////
//尾部Push追加数据
BOOL CXPackage::Push(const void *pvContent, DWORD dwSize)
{
  BOOL blRet = TRUE;
  assert(pvContent);  
  if (NULL == pvContent)
  {
    blRet = FALSE;
    goto _Exit;
  }
  VERIFY(FALSE != (blRet = Reserve(m_dwSize + dwSize)));
  assert(m_dwCapacity >= m_dwSize + dwSize);
  Mem_CopyMemory(m_pbBuffer + m_dwSize, (void *)pvContent, dwSize);
  m_dwSize += dwSize;

_Exit:
  return blRet;
}
BOOL CXPackage::Push(const CXPackage *ptB)
{
  assert(ptB);

  BOOL blRet = TRUE;
  if (!ptB)
  {
    blRet = FALSE;
    goto _Exit;
  }
  if (ptB->IsEmpty())
  {
    goto _Exit;
  }
  blRet = Push(ptB->m_pbBuffer, ptB->m_dwSize);
_Exit:
  return blRet;
}
//////////////////////////////////////////////////////////////////////////
//头部Push追加数据
BOOL CXPackage::Push_Front(const void *pvContent, DWORD dwSize)
{
  assert(pvContent);
  BOOL blRet = TRUE;
    
  if (NULL == pvContent)
  {
    blRet = FALSE;
    goto _Exit;
  }
  VERIFY(FALSE != (blRet = Reserve(m_dwSize + dwSize)));
  assert(m_dwCapacity >= m_dwSize + dwSize);
  
  Mem_CopyMemory(m_pbBuffer + dwSize, m_pbBuffer, m_dwSize);
  Mem_CopyMemory(m_pbBuffer, (void *)pvContent, dwSize);
  m_dwSize += dwSize;
  
_Exit:
  return blRet;
}
BOOL CXPackage::Push_Front(const CXPackage *ptB)
{
  assert(ptB);
  BOOL blRet = TRUE;
  if (!ptB)
  {
    blRet = FALSE;
    goto _Exit;
  }
  if (ptB->IsEmpty())
  {
    goto _Exit;
  }
  blRet = Push_Front(ptB->m_pbBuffer, ptB->m_dwSize);
_Exit:
  return blRet;
}
//////////////////////////////////////////////////////////////////////////
//弹出数据
BOOL CXPackage::Pop(DWORD dwSize, void *pvVariant)
{
  BOOL blRet = TRUE;  
  assert(dwSize >= 0);
  if (0 == dwSize)
  {
    goto _Exit;
  }  
  else if (dwSize > m_dwSize)
  {
    dwSize = m_dwSize;
  }
  assert(m_dwSize >= dwSize);
  if (pvVariant)
  {
    Mem_CopyMemory(pvVariant, m_pbBuffer + m_dwSize - dwSize, dwSize);
  }
#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(m_pbBuffer + m_dwSize - dwSize, dwSize);
	}
#endif
  m_dwSize -= dwSize;
  assert(0 <= m_dwSize);
_Exit:

  return blRet;
}
BOOL CXPackage::Pop(DWORD dwSize, CXPackage* ptB)
{  
  assert(ptB);
  BOOL blRet = TRUE;
  if (0 == dwSize)
  {
    goto _Exit;
  }  
  else if (dwSize > m_dwSize)
  {
    dwSize = m_dwSize;
  }  
  assert(m_dwSize >= dwSize);
  if (FALSE == (blRet = ptB->Push(m_pbBuffer + m_dwSize - dwSize, dwSize)))
  {
    goto _Exit;
  }

#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(m_pbBuffer + m_dwSize - dwSize, dwSize);
	}
#endif
  m_dwSize -= dwSize;
  assert(0 <= m_dwSize);
_Exit:
  
  return blRet;
}
//////////////////////////////////////////////////////////////////////////
BOOL CXPackage::Pop_Front(DWORD dwSize, void *pvVariant)
{
  BOOL blRet = TRUE;
  
  assert(dwSize <= m_dwSize);
  if (0 == dwSize)
  {
    goto _Exit;
  }  
  else if (dwSize > m_dwSize)
  {
    dwSize = m_dwSize;
  }
  if (pvVariant)
  {
    Mem_CopyMemory(pvVariant, m_pbBuffer, dwSize);
  }
  if (dwSize != m_dwSize)
  {
    Mem_CopyMemory(m_pbBuffer, m_pbBuffer + dwSize, m_dwSize - dwSize);
  }
  
#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(m_pbBuffer + m_dwSize - dwSize, dwSize);
	}
#endif
  m_dwSize -= dwSize;
  assert(0 <= m_dwSize);
_Exit:
  
  return blRet;
}
BOOL CXPackage::Pop_Front(DWORD dwSize, CXPackage *ptB)
{
  BOOL blRet = TRUE;
  
  assert(dwSize <= m_dwSize);
  assert(ptB);
  if (0 == dwSize)
  {
    goto _Exit;
  }
  else if (dwSize > m_dwSize)
  {
    dwSize = m_dwSize;
  }
  if (FALSE == (blRet = ptB->Push(m_pbBuffer, dwSize)))
  {
    goto _Exit;
  }  
  if (dwSize != m_dwSize)
  {
    Mem_CopyMemory(m_pbBuffer, m_pbBuffer + dwSize, m_dwSize - dwSize);
  }
  
#if defined(_DEBUG) || defined(DEBUG)
	{
		Mem_ZeroMemory(m_pbBuffer + m_dwSize - dwSize, dwSize);
	}
#endif
  m_dwSize -= dwSize;
  assert(0 <= m_dwSize);
  
_Exit:
  
  return blRet;
}
//////////////////////////////////////////////////////////////////////////
//收缩多余内存
void CXPackage::Shrink()
{  
  if (m_dwSize != m_dwCapacity)
  {    
    if (!IsEmpty())
    {
      DWORD dwShrinkedSize = CalcUsedBytes(m_dwSize);
      if (FALSE != Mem_ResizeMemory((void **)&m_pbBuffer, dwShrinkedSize))
      {
        m_dwCapacity = dwShrinkedSize;
      }  
    }
    else
    {
      Release();
    }
  }
  return;
}
//////////////////////////////////////////////////////////////////////////
void CXPackage::SetIncStep(DWORD dwStep)
{
  if (0 == dwStep)
  {
    m_dwIncStep = PACKAGE_Mem_Inc;
  }
  else
  {
    m_dwIncStep = dwStep;
  }
}
//////////////////////////////////////////////////////////////////////////
DWORD CXPackage::GetSize() const
{
  return m_dwSize;
}
DWORD CXPackage::GetCapacity() const
{
  return m_dwCapacity;  
}
const void *CXPackage::GetData() const
{
  return (const void *)m_pbBuffer;
}
BOOL CXPackage::IsEmpty() const
{
  return (0 == m_dwSize);
}
BOOL CXPackage::IsNull() const
{
  return (!m_pbBuffer);
}
//////////////////////////////////////////////////////////////////////////
//operator
BOOL CXPackage::operator==(const CXPackage& tB) const
{  
  return (m_dwSize == tB.m_dwSize && FALSE != Mem_EqualMemory(m_pbBuffer, tB.m_pbBuffer, m_dwSize));  
}
const CXPackage& CXPackage::operator=(const CXPackage& tRight)
{//为了防止内存的浪费  这里不进行完全相等 只拷贝有效的内存块
  //可能出现自己等于自己的情况 此时realloc分配可能出现随机值
  assert(&tRight != this);
  if (&tRight != this)
  {//自己等于自己
    if (!tRight.IsEmpty())
    {
      ResetEx(tRight.m_pbBuffer, tRight.m_dwSize);
    }
    else
    {
      Release();
    }
  }  
  return *this;
}
const CXPackage& CXPackage::operator+=(const CXPackage& tRight)
{//为了防止内存的浪费  这里不进行完全相等 只拷贝有效的内存块    
  if (!tRight.IsEmpty())
  {
    Push(tRight.m_pbBuffer, tRight.m_dwSize);
  }
  
  return *this;
}
const CXPackage CXPackage::operator+(const CXPackage& tRight) const
{  
  CXPackage tRet(*this);
  tRet.Push(&tRight);    
  return tRet;
}
CXPackage::operator const BYTE *() const
{    
  return (const BYTE *)m_pbBuffer;
}
CXPackage::operator const TCHAR *() const 
{    
  return (const TCHAR *)m_pbBuffer;
}
//////////////////////////////////////////////////////////////////////////
///删除元素
BOOL CXPackage::Erase(DWORD dwStart, DWORD dwSize)
{
  BOOL blRet = TRUE;
  DWORD dwNewSize = 0;
  
  assert(dwStart < m_dwSize);  
  assert(dwStart + dwSize <= m_dwSize);

  if (dwStart >= m_dwSize)
  {
    blRet = FALSE;
    goto _Exit;
  }
  if (0 == dwSize)
  {
    goto _Exit;
  }
  if (dwStart + dwSize >= m_dwSize)
  {
    dwSize = m_dwSize - dwStart;
  }
  assert(dwStart + dwSize <= m_dwSize);
  if (dwStart + dwSize < m_dwSize)
  {//结尾还留有元素
    Mem_CopyMemory(m_pbBuffer + dwStart, m_pbBuffer + (dwStart + dwSize), m_dwSize - (dwStart + dwSize));
  }
  dwNewSize = (long)m_dwSize - (long)dwSize;
#if defined(_DEBUG) || defined(DEBUG)
  {
    Mem_ZeroMemory(m_pbBuffer + dwNewSize, m_dwSize - dwNewSize);
  }
#endif
  m_dwSize = dwNewSize;
_Exit:
  return blRet;
}

//////////////////////////////////////////////////////////////////////////
///插入元素
BOOL CXPackage::Insert(DWORD dwStart, const void *pvVariant, DWORD dwSize)
{  
  assert(pvVariant);  
  assert(dwStart <= m_dwSize);

  BOOL blRet = TRUE;
  CXPackage tTmp;
  if (dwSize <= 0)
  {
    goto _Exit;
  }
  if (dwStart > m_dwSize)
  {
    dwStart = m_dwSize;
  }  
  tTmp = *this;
  Clear();
  //一次内存分配
  VERIFY(Reserve(m_dwSize + dwSize));
  if (0 != dwStart)
  {
    if (FALSE == (blRet = tTmp.Pop_Front(dwStart, this)))
    {
      goto _Exit;
    }        
  }
  if (FALSE == (blRet = Push(pvVariant, dwSize)))
  {
    goto _Exit;
  }  
  if (!tTmp.IsEmpty())
  {
    if (FALSE == (blRet = Push(&tTmp)))
    {
      goto _Exit;
    }    
  }
_Exit:
  return blRet;
}

//////////////////////////////////////////////////////////////////////////
//预设一定空容量 如果已经达到这么大 则不进行处理 
BOOL CXPackage::Reserve(DWORD dwSize)
{  
  BOOL blRet = TRUE;
  DWORD dwExpMemSize;
  assert(m_dwSize <= m_dwCapacity);
  if (0 == dwSize)
  {
    dwSize = m_dwIncStep;
  }  
  if (dwSize < m_dwCapacity)
  {    
    goto _Exit;
  }
  
  dwExpMemSize = (dwSize + m_dwIncStep - 1) / m_dwIncStep * m_dwIncStep;
  dwExpMemSize = CalcUsedBytes(dwExpMemSize);	
  assert(dwExpMemSize >= m_dwIncStep);
  assert(m_dwCapacity <= dwExpMemSize);
  if (m_dwCapacity == dwExpMemSize)
  {
    goto _Exit;
  }
  if (FALSE != Mem_ResizeMemory((void **)&m_pbBuffer, dwExpMemSize))
  {
    m_dwCapacity = dwExpMemSize;
  }
  else
  {
    blRet = FALSE;
    goto _Exit;
  }   
_Exit:
  return blRet;
}
//在当前包最后增加定长的缓存区 返回此缓存区地址
BYTE *CXPackage::GetBufferEx(DWORD dwBufferSize)
{  
  BYTE *pbBuffer = NULL;
  if (FALSE == Reserve(m_dwSize + dwBufferSize))
  {
    goto Exit_;
  }
  pbBuffer = m_pbBuffer + m_dwSize;
Exit_:
  return pbBuffer;
}
//释放缓存区 将缓存区中nbuffersize的长度数据固化起来
BOOL CXPackage::ReleaseBufferEx(DWORD dwBufferSize)
{
  assert(dwBufferSize + m_dwSize <= m_dwCapacity);
  BOOL blRet = TRUE;
  if (!(dwBufferSize + m_dwSize <= m_dwCapacity))
  {
    blRet = FALSE;
    goto Exit_;
  }
  m_dwSize += dwBufferSize;
Exit_:
  return blRet;
}

//在当前包最后增加定长的缓存区 返回此缓存区地址
BYTE *CXPackage::GetBuffer(DWORD dwBufferSize)
{  	
  BYTE *pbBuffer = NULL;  
  if (FALSE == Reserve(dwBufferSize))
  {
    goto Exit_;
  }
  pbBuffer = m_pbBuffer;
Exit_:
  return pbBuffer;
}
//释放缓存区 将缓存区中nbuffersize的长度数据固化起来
BOOL CXPackage::ReleaseBuffer(DWORD dwBufferSize)
{
  assert(dwBufferSize <= m_dwCapacity);
  BOOL blRet = TRUE;
  if (!(dwBufferSize <= m_dwCapacity))
  {
    blRet = FALSE;
    goto Exit_;
  }
  m_dwSize = dwBufferSize;
Exit_:
  return blRet;
}
DWORD CXPackage::CalcUsedBytes( DWORD dwUsedBytes)
{
#if defined(_DEBUG) || defined(DEBUG)
  DWORD dwSize = dwUsedBytes;
#endif
  if( m_bl32BitAlign )
  { //if os is 64 bit , then maybe change code little.
    dwUsedBytes += dwUsedBytes > 0 ? (4 - (dwUsedBytes & 3) ) : 0;
  }
#if defined(_DEBUG) || defined(DEBUG)
  assert(dwUsedBytes >= dwSize);
#endif
  return dwUsedBytes;
}