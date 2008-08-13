
#ifndef X_MEMORYMANAGER
#define X_MEMORYMANAGER

#include "Ext_Type.h"


typedef void*(*XMALLOC)(size_t nSize);
typedef void(*XFREE)(void * pvVoid);


class DLLXEXPORT CXMemoryManager
{
protected:
  XMALLOC m_pfMalloc;
  XFREE m_pfFree;
public: 
  CXMemoryManager();
  CXMemoryManager(XMALLOC, XFREE);
  virtual ~CXMemoryManager();
  void* Alloc(size_t);
  void  Free(void *);
  void Init(XMALLOC, XFREE);
  static CXMemoryManager&GetInstance();
  
};


#endif//X_MEMORYMANAGER



