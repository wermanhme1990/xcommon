
#include "StdAfx.h"
#include "_Object.h"

_XObject::_XObject()
{
  
}
_XObject::~_XObject()
{
  
}
void* PASCAL _XObject::operator new(size_t nSize)
{ 
	LPVOID pVoid = ::operator new(nSize); 
	if (pVoid)
	{
		memset(pVoid, 0, nSize);
	}
  return pVoid;
}

void* PASCAL _XObject::operator new(size_t, void* p)
{
  return p; 
}
void*PASCAL _XObject::operator new[](size_t nSize)
{
	LPVOID pVoid = ::operator new(nSize);
	if (pVoid)
	{
		memset(pVoid, 0, nSize);
	}
  return pVoid;
}
void* PASCAL _XObject::operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	LPVOID pVoid = ::operator new(nSize);
	if (pVoid)
	{
		memset(pVoid, 0, nSize);
	}
  return pVoid;
}
void* PASCAL _XObject::operator new[](size_t nSize, LPCSTR lpszFileName, int nLine)
{
	LPVOID pVoid = ::operator new(nSize);
	if (pVoid)
	{
		memset(pVoid, 0, nSize);
	}
  return pVoid;
}
void PASCAL _XObject::operator delete(void* p)
{ 
  if (p)
  {
    ::operator delete(p);
    p = NULL;
  }   
}
void PASCAL _XObject::operator delete(void* p, void*)
{ 
  if (p)
  {
    ::operator delete(p); 
    p = NULL;
  }
  
}
void PASCAL _XObject::operator delete[](void* p)
{
  if (p)
  {
		::operator delete(p);
    p = NULL;
  }
}
void PASCAL _XObject::operator delete(void *p, LPCSTR lpszFileName, int nLine)
{ 
	if (p)
	{
		::operator delete(p);
		p = NULL;
	}	
}
void PASCAL _XObject::operator delete[](void *p, LPCSTR lpszFileName, int nLine)
{
	if (p)
	{
		::operator delete(p);
		p = NULL;
	}	
}