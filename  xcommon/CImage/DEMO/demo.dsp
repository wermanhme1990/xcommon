# Microsoft Developer Studio Project File - Name="demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=demo - Win32 DLL Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "demo.mak" CFG="demo - Win32 DLL Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 DLL Release" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 DLL Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "demo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\Demo"
# PROP Intermediate_Dir "..\Release\Demo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\lib\imagelib.lib ..\lib\zlib.lib /nologo /subsystem:windows /machine:I386 /out:"..\BIN/demo.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\Demo"
# PROP Intermediate_Dir "..\Debug\Demo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "_LMRTDLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\lib\imagelib_d.lib ..\lib\zlib_d.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\BIN/demo_d.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "demo___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "demo___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\Demo\Unicode"
# PROP Intermediate_Dir "..\Release\Demo\Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O1 /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GF /c
# ADD CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\BIN/demo.exe"
# ADD LINK32 ..\lib\imagelib.lib ..\lib\zlib.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\BIN/demou.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "demo___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "demo___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\Demo\Unicode"
# PROP Intermediate_Dir "..\Debug\Demo\Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GF /c
# ADD CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\BIN/demo_d.exe"
# ADD LINK32 ..\lib\imagelib_d.lib ..\lib\zlib_d.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\BIN/demou_d.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "demo___Win32_DLL_Release"
# PROP BASE Intermediate_Dir "demo___Win32_DLL_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\Demo\DLL"
# PROP Intermediate_Dir "..\Release\Demo\DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT BASE CPP /nologo
# ADD CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_IMAGEDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\BIN/demo.exe"
# ADD LINK32 ..\lib\imagedll.lib /nologo /subsystem:windows /machine:I386 /out:"..\BIN/demodll.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "demo___Win32_DLL_Debug"
# PROP BASE Intermediate_Dir "demo___Win32_DLL_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\Demo\DLL"
# PROP Intermediate_Dir "..\Debug\Demo\DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "_LMRTDLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT BASE CPP /nologo
# ADD CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_IMAGEDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\BIN/demo_d.exe"
# ADD LINK32 ..\lib\imagedll_d.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\BIN/demodll_d.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "demo___Win32_DLL_Unicode_Release"
# PROP BASE Intermediate_Dir "demo___Win32_DLL_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\Demo\Unicode\DLL"
# PROP Intermediate_Dir "..\Release\Demo\Unicode\DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT BASE CPP /nologo
# ADD CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_IMAGEDLL" /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\imagelib.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\BIN/demou.exe"
# ADD LINK32 ..\lib\imagedll.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\BIN/demodllu.exe"

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "demo___Win32_DLL_Unicode_Debug"
# PROP BASE Intermediate_Dir "demo___Win32_DLL_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\Demo\Unicode\DLL"
# PROP Intermediate_Dir "..\Debug\Demo\Unicode\DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_LMRTDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT BASE CPP /nologo
# ADD CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_IMAGEDLL" /FR /Yu"stdafx.h" /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\imagelib_d.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\BIN/demou_d.exe"
# ADD LINK32 ..\lib\imagedll_d.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\BIN/demodllu_d.exe"

!ENDIF 

# Begin Target

# Name "demo - Win32 Release"
# Name "demo - Win32 Debug"
# Name "demo - Win32 Unicode Release"
# Name "demo - Win32 Unicode Debug"
# Name "demo - Win32 DLL Release"
# Name "demo - Win32 DLL Debug"
# Name "demo - Win32 DLL Unicode Release"
# Name "demo - Win32 DLL Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\SRC\app.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\demo.rc
# End Source File
# Begin Source File

SOURCE=.\SRC\DlgRotate.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\frame.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imgdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imgframe.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imgobj.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imgview.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\mdiface.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CIMAGE\SRC\trans.c

!IF  "$(CFG)" == "demo - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 Unicode Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 Unicode Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Release"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Debug"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Unicode Release"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "demo - Win32 DLL Unicode Debug"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\SRC\app.h
# End Source File
# Begin Source File

SOURCE=.\SRC\helpers.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imgdoc.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imgframe.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imgobj.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imgobj.inl
# End Source File
# Begin Source File

SOURCE=.\SRC\imgview.h
# End Source File
# Begin Source File

SOURCE=.\SRC\mdiface.h
# End Source File
# Begin Source File

SOURCE=.\SRC\memdc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\bg.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\bg.gif
# End Source File
# Begin Source File

SOURCE=.\RES\bg.jpg
# End Source File
# Begin Source File

SOURCE=.\res\demo.ico
# End Source File
# Begin Source File

SOURCE=.\res\demo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\demoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\doc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\Explorer.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico2.bin
# End Source File
# Begin Source File

SOURCE=.\RES\mdiface.bmp
# End Source File
# Begin Source File

SOURCE=.\res\testbmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\trans.gif
# End Source File
# End Group
# End Target
# End Project
