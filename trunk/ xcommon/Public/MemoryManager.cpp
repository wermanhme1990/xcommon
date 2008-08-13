

#include "stdafx.h"
#include "MemoryManager.h"

void* CXMemoryManager::Alloc( size_t nSize)
{
  LPVOID pvMem = NULL;
  if (!m_pfMalloc)
  {
    return pvMem;
  }
  pvMem = m_pfMalloc(nSize);
  if (pvMem)
  {
#if defined(DEBUG)  || defined(_DEBUG)
		{
			Mem_ZeroMemory(pvMem, (LONG)nSize);
		}
#endif
  }
  return pvMem;
}
void CXMemoryManager::Free( void * pvVoid)
{
  if(!m_pfFree || !pvVoid)
  {
    return;
  }
#if defined(DEBUG)  || defined(_DEBUG)
	{
		LONG nSize = (LONG)_msize(pvVoid);
		Mem_ZeroMemory(pvVoid, nSize);
	}
#endif
  m_pfFree(pvVoid);
}
CXMemoryManager::CXMemoryManager()
{
  m_pfMalloc = NULL;
  m_pfFree = NULL;
}
CXMemoryManager::CXMemoryManager( XMALLOC pfMalloc, XFREE pfFree)
{
  assert(pfMalloc && pfFree);
  m_pfMalloc = pfMalloc;
  m_pfFree = pfFree;
}
CXMemoryManager::~CXMemoryManager()
{
  m_pfMalloc = NULL;
  m_pfFree = NULL;
}
CXMemoryManager& CXMemoryManager::GetInstance()
{
  static CXMemoryManager tMemoryManager;
  return tMemoryManager;
}
void CXMemoryManager::Init(XMALLOC pfMalloc, XFREE pfFree)
{ 
  assert(pfMalloc && pfFree);
  m_pfMalloc = pfMalloc;
  m_pfFree = pfFree;
}