# Microsoft Developer Studio Project File - Name="BCGCBDotNetExampleStatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BCGCBDotNetExampleStatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BCGCBDotNetExampleStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BCGCBDotNetExampleStatic.mak" CFG="BCGCBDotNetExampleStatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BCGCBDotNetExampleStatic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BCGCBDotNetExampleStatic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BCGCBDotNetExampleStatic - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "ReleaseStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BCGCBPRO_STATIC_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\BCGCBPro\Styles\Office 2007 (luna - blue)\\" /d "NDEBUG" /d "_AFXDLL" /d "_BCGCBPRO_STATIC_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../bin/BCGCBDotNetExampleS.exe"

!ELSEIF  "$(CFG)" == "BCGCBDotNetExampleStatic - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "DebugStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BCGCBPRO_STATIC_" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\BCGCBPro\Styles\Office 2007 (luna - blue)\\" /d "_DEBUG" /d "_AFXDLL" /d "_BCGCBPRO_STATIC_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin/BCGCBDotNetExampleSD.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BCGCBDotNetExampleStatic - Win32 Release"
# Name "BCGCBDotNetExampleStatic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AppLookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExample.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExample.rc
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExampleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExampleView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassTreeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicHelpView.cpp
# End Source File
# Begin Source File

SOURCE=.\EnhMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\FileViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FindComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GotoLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsPages.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertiesViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceView.cpp
# End Source File
# Begin Source File

SOURCE=.\StartView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AppLookDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExample.h
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExampleDoc.h
# End Source File
# Begin Source File

SOURCE=.\BCGCBDotNetExampleView.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClassTreeWnd.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewBar.h
# End Source File
# Begin Source File

SOURCE=.\CustomEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DynamicHelpView.h
# End Source File
# Begin Source File

SOURCE=.\EnhMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\FileViewBar.h
# End Source File
# Begin Source File

SOURCE=.\FindComboBox.h
# End Source File
# Begin Source File

SOURCE=.\GotoLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsPages.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\OutputView.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesViewBar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourceView.h
# End Source File
# Begin Source File

SOURCE=.\StartView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBox.h
# End Source File
# Begin Source File

SOURCE=.\UndoBar.h
# End Source File
# Begin Source File

SOURCE=.\UndoListBox.h
# End Source File
# Begin Source File

SOURCE=.\WatchBar.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Group "Examples"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\res\Examples\bcgpedit-cpp.xml"
# End Source File
# Begin Source File

SOURCE=".\res\Examples\bcgpedit-web.xml"
# End Source File
# Begin Source File

SOURCE=.\res\Examples\example.asp
# End Source File
# Begin Source File

SOURCE=.\res\Examples\Example.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\BCGCBDotNetExample.ico
# End Source File
# Begin Source File

SOURCE=.\res\BCGCBDotNetExampleDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\BCGDevStudioExample.ico
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\build.bmp
# End Source File
# Begin Source File

SOURCE=.\res\build24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\class_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\class_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\classview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\classview_hc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cpp.ico
# End Source File
# Begin Source File

SOURCE=.\res\director.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dynamich.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dynamich24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dynamichelp_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\file_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\filevie24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fileview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\img_inte_hc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\intellisense.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menuimages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menuimages24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionsimages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\output_bar.ico
# End Source File
# Begin Source File

SOURCE=.\res\output_bar_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\pointer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pointer1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prop24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prop_bar_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\properti.bmp
# End Source File
# Begin Source File

SOURCE=.\res\properti.ico
# End Source File
# Begin Source File

SOURCE=.\res\res_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\res_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\resource.bmp
# End Source File
# Begin Source File

SOURCE=.\res\resource24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\solution.bmp
# End Source File
# Begin Source File

SOURCE=.\res\solution24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sort.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sort24.bmp
# End Source File
# Begin Source File

SOURCE=".\res\start-hot.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\start16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\textbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbox_.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbox_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\tools_page1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page1hc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page2hc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page3hc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\watch_bar.ico
# End Source File
# Begin Source File

SOURCE=.\res\watch_bar_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\web.ico
# End Source File
# Begin Source File

SOURCE=.\res\workspc.bmp
# End Source File
# Begin Source File

SOURCE=.\xptheme.bin
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
