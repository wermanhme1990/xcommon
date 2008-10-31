
#ifndef HWX__OBJECT_H
#define HWX__OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef HWNOTESR_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

class DLLEXPORT _XObject
{
protected:
  _XObject();
  ~_XObject();
public:
  void* PASCAL operator new(size_t nSize);
  void* PASCAL operator new(size_t, void* p);
  void* PASCAL operator new[](size_t);
  void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
  void* PASCAL operator new[](size_t nSize, LPCSTR lpszFileName, int nLine);
  void PASCAL operator delete(void* p);
  void PASCAL operator delete(void* p, void* pPlace);  
  void PASCAL operator delete[](void* p);
  void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
  void PASCAL operator delete[](void *p, LPCSTR lpszFileName, int nLine); 
  
};


#endif // HWX__OBJECT_H
