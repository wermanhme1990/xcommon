#pragma once
#include <windows.h>
#include <vector>
class CProcessMainWndArray 
{
    DWORD m_dwProcessId;
    std::vector<HWND> m_vHwnd;
public:
    CProcessMainWndArray(DWORD = 0);
    ~CProcessMainWndArray();
protected:
    static BOOL __stdcall EnumProc(HWND hWnd, LPARAM lParam);
protected:
    DWORD GetCurProcessId();
    std::vector<HWND>* GetMainHwnd();
public:
    BOOL Empty();
    LONG Count() const;
    HWND operator[](LONG );
		void Attach(DWORD );
		void Detach();
};