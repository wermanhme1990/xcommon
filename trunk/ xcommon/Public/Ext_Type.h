#ifndef HWXUE_EXT_TYPE_H_INC
#define HWXUE_EXT_TYPE_H_INC

#include <assert.h>
#include <wtypes.h>
#include <CRTDbg.h>
#include <tchar.h>
#ifdef XEXPORT
#define DLLXEXPORT   __declspec(dllexport) 
#else
#define DLLXEXPORT   //__declspec(dllimport) 
#endif

typedef     short             SHORT;
typedef     SHORT*            LPSHORT;
typedef     long              LONG;
typedef     LONG*             LPLONG;
typedef     void *            LPVOID;
typedef     char              CHAR;
typedef     const CHAR *      LPCSTR;
typedef     CHAR*             LPCHAR;
//typedef     unsigned LONG     ULONG;
//////////////////////////////////////////////////////////////////////////
#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef     unsigned char       BYTE;
typedef     BYTE*             LPBYTE;
#endif 
//////////////////////////////////////////////////////////////////////////
#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef     unsigned short    WORD;
typedef     WORD*             LPWORD;
#endif 
//////////////////////////////////////////////////////////////////////////
#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef     unsigned long     DWORD;
typedef     DWORD*            LPDWORD;
#endif // !_DWORD_DEFINED
//////////////////////////////////////////////////////////////////////////
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef     unsigned short    WCHAR;
#endif
//////////////////////////////////////////////////////////////////////////
#ifndef _WCHAR_DEFINED
#define _WCHAR_DEFINED
typedef     wchar_t           WCHAR;
typedef     const WCHAR *     LPCWSTR;  
typedef     WCHAR *           LPWSTR;
#endif 
//////////////////////////////////////////////////////////////////////////
typedef  TCHAR *  LPTSTR;
typedef  const TCHAR *LPCTSTR;

#ifndef BOOL
typedef int     BOOL;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE 
#define TRUE      1
#endif

#ifndef FALSE
#define FALSE     0
#endif

#ifndef NULL
#define NULL      0
#endif

#ifndef VOID
#define VOID      void
#endif

#ifndef HANDLE
typedef void*     HANDLE; 
#endif



#ifndef   _TERROR_DEFINED
#define   _TERROR_DEFINED
typedef   LONG    TError;  
#endif


#ifndef VERIFY
#ifdef _DEBUG
#define VERIFY(f)          assert((f))//_ASSERT((f))
#else
#define VERIFY(f)          ((void)(f))
#endif    
#endif
#define verify   VERIFY
#ifndef _abs
#define _abs(a) (((a)>0)?(a):(-(a)))
#endif

#ifndef _max
#define _max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef  _min
#define _min(a,b) ((a)<(b)?(a):(b))
#endif

#define _swap(a,b) {(a)^=(b);(b)^=(a);(a)^=(b);}

#ifndef _min3
#define  _min3(a, b, c) (_min(_min((a), (b)), (c)))
#endif

#ifndef _max3
#define  _max3(a, b, c) (_max(_max((a), (b)), (c)))
#endif

#define MIN_CHAR        (0x80)        
#define MAX_CHAR        (0x7f)        
#define MIN_SHORT       (0x8000)      
#define MAX_SHORT       (0x7fff)      
#define MIN_LONG        (0x80000000)  
#define MAX_LONG        (0x7fffffff)  
#define MAX_BYTE        (0xff) 
#define MIN_WORD        (0x0)
#define MAX_WORD        (0xffff)  
#define MIN_DWORD       (0x0)
#define MAX_DWORD       (0xffffffff) 

#ifndef MAX_PATH
#define MAX_PATH          (260)
#endif
#define MAX_DRIVE         (3)  
#define MAX_DIR           (256) 
#define MAX_FNAME         (256) 
#define MAX_EXT           (256) 
#ifndef MAX_NAME
#define MAX_NAME          MAX_PATH
#endif

#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD)(a) & 0xff))   | ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))
#endif
//笔画结束标记

//pot数据包头长度
#define    X_HEADERSIZE_POT        (0x6)


//高级结构
#ifndef _T_Point_
#define _T_Point_
typedef struct tagTPoint
{
  SHORT  x;
  SHORT  y;
} TPoint, *LPTPoint;
#endif

typedef struct tagCnd4
{
  WORD  m_wDistance;  
  WORD  m_wIndex;
} TCnd4, *LPTCnd4;

typedef struct tagCnd8
{
  int    m_nIndex;
  int    m_nDistance;
}TCnd8, *LPTCnd8;
//函数参数前 表示传入, 传出
#define HWX_IN    
#define HWX_OUT   
#define HWX_IN_OUT


#ifndef PIE
#define     PIE         (3.14159265358979323846)
#define     PIE_2       (1.57079632679489661923)
#define     PIE_4       (0.785398163397448309616)
#endif

#define HW_StrokeEndMark  (-1)


//#define SAFE_DELETE(ptr)
#undef SAFE_DELETE
#define SAFE_DELETE(ptr) \
	if (ptr)\
{ \
	delete ptr;\
	ptr = NULL; \
}

//#define SAFE_DELETE_AR(ptr)
#undef SAFE_DELETE_AR
#define SAFE_DELETE_AR(ptr) \
	if (ptr) \
{ \
	delete [] ptr; \
	ptr = NULL; \
}

//#define SAFE_DELETE_HICON(hIcon)
#undef SAFE_DELETE_HICON
#define SAFE_DELETE_HICON(hIcon) \
	if (hIcon) \
{\
	::DestroyIcon(hIcon); \
	hIcon = NULL;\
}


//#define SAFE_DELETE_OBJ(obj)
#undef SAFE_DELETE_OBJ
#define SAFE_DELETE_OBJ(obj) \
	if (obj.GetSafeHandle()) \
	{\
		obj.DeleteObject();\
	}

#undef SAFE_FREE_LIBRARY(hModel)
#define SAFE_FREE_LIBRARY(hModel) \
	if (hModel)\
	{\
		::FreeLibrary(hModel);\
		hModel = NULL;\
	}
#endif//HWXUE_EXT_TYPE_H_INC

