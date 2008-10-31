/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   HWXPackage.h          
文件内容:  数据包定义 可以用做内存的管理 
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/
#ifndef HWXUE_DBG_TPackage_H_Inc
#define HWXUE_DBG_TPackage_H_Inc
#include "Ext_Type.h"

class DLLXEXPORT CXPackage
{   
private:
  DWORD m_dwCapacity;
  DWORD m_dwSize;
  DWORD m_dwIncStep;
  BOOL m_bl32BitAlign;
  BYTE *m_pbBuffer;  
public:
  //////////////////////////////////////////////////////////////////////////
  CXPackage(BOOL = TRUE);
  CXPackage(const void *, DWORD, BOOL = TRUE);
  ~CXPackage();
  CXPackage(const CXPackage &);
  //设置每次增长量 根据实际情况设置增长量 免得不停的申请新内存 影响速度 默认是 PACKAGE_Mem_Inc BYTE
  void SetIncStep(DWORD);
  DWORD GetSize() const;
  DWORD GetCapacity() const;
  const void *GetData() const;
  //////////////////////////////////////////////////////////////////////////
  //当作缓存处理
  BYTE *GetBuffer(DWORD);
  BOOL ReleaseBuffer(DWORD);
  //在原缓存的基础上增加 释放后 原缓存内容不变 只会增量
  BYTE *GetBufferEx(DWORD);
  BOOL ReleaseBufferEx(DWORD);
  //////////////////////////////////////////////////////////////////////////
  //重新设置buffer内容 buffer容量比预期的大
  BOOL Reset(const void *, DWORD);
  //Buffer容量刚好
  BOOL ResetEx(const void *, DWORD);
  
  //////////////////////////////////////////////////////////////////////////
  //清空里面元素(不释放内存)
  void Clear();
  //释放资源 
  void Release();
  //////////////////////////////////////////////////////////////////////////
  //BOOL
  BOOL IsEmpty() const;  
  BOOL IsNull() const;
  //////////////////////////////////////////////////////////////////////////
  //用堆栈 尾部追加
  BOOL Push(const void *, DWORD);
  BOOL Push(const CXPackage *);
  //////////////////////////////////////////////////////////////////////////
  //头部Push追加数据
  BOOL Push_Front(const void *, DWORD);
  BOOL Push_Front(const CXPackage *);
  //////////////////////////////////////////////////////////////////////////
  //弹出数据  
  BOOL Pop(DWORD , void *pvVariant = NULL);
  BOOL Pop(DWORD , CXPackage* );
  //////////////////////////////////////////////////////////////////////////
  //头部弹出数据
  BOOL Pop_Front(DWORD , void *pvVariant = NULL);
  BOOL Pop_Front(DWORD , CXPackage* );
  //////////////////////////////////////////////////////////////////////////
  void Shrink();
  //////////////////////////////////////////////////////////////////////////
  //删除元素
  BOOL Erase(DWORD, DWORD);
  //////////////////////////////////////////////////////////////////////////
  //插入元素
  BOOL Insert(DWORD , const void *pvVariant, DWORD);
  //////////////////////////////////////////////////////////////////////////
  //操作重载
  BOOL operator==(const CXPackage& ) const;
  const CXPackage  &operator=(const CXPackage& );
  const CXPackage  &operator+=(const CXPackage& );
  const CXPackage  operator+(const CXPackage& ) const;
  operator const BYTE *() const;  
  operator const TCHAR *() const;  
private:
  //////////////////////////////////////////////////////////////////////////
  //预设一定容量 如何已经有内存分配 则不进行处理
  BOOL Reserve(DWORD);
  inline DWORD CalcUsedBytes( DWORD dwBits);
};

typedef CXPackage CHWXPackage;

#endif//HWXUE_DBG_TPackage_H_Inc