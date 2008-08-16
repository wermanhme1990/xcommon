# Microsoft Developer Studio Project File - Name="BCGPMSOffice2007Demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BCGPMSOffice2007Demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BCGPMSOffice2007DemoStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BCGPMSOffice2007DemoStatic.mak" CFG="BCGPMSOffice2007Demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BCGPMSOffice2007Demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BCGPMSOffice2007Demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BCGPMSOffice2007Demo - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "ReleaseStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BCGCBPRO_STATIC_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\BCGCBPro\Styles\Office 2007 (luna - blue)\\" /d "NDEBUG" /d "_AFXDLL" /d "_BCGCBPRO_STATIC_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\..\bin/BCGPMSOffice2007DemoS.exe"

!ELSEIF  "$(CFG)" == "BCGPMSOffice2007Demo - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "DebugStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BCGCBPRO_STATIC_" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\BCGCBPro\Styles\Office 2007 (luna - blue)\\" /d "_DEBUG" /d "_AFXDLL" /d "_BCGCBPRO_STATIC_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"..\..\bin/BCGPMSOffice2007DemoSD.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BCGPMSOffice2007Demo - Win32 Release"
# Name "BCGPMSOffice2007Demo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BCGPMSOffice2007Demo.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMSOffice2007Demo.rc
# End Source File
# Begin Source File

SOURCE=.\BCGPMSOffice2007DemoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMSOffice2007DemoView.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourcePage.cpp
# End Source File
# Begin Source File

SOURCE=.\RibbonTooltipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BCGPMSOffice2007Demo.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMSOffice2007DemoDoc.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMSOffice2007DemoView.h
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OptionsPage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourcePage.h
# End Source File
# Begin Source File

SOURCE=.\RibbonTooltipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BCGPMSOffice2007Demo.ico
# End Source File
# Begin Source File

SOURCE=.\res\BCGPMSOffice2007Demo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\BCGPMSOffice2007DemoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Buttons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filelarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filesmall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\font.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Insert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertIllustrations.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertIllustrationsLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertLinks.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertLinksLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertPages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertPagesLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertTable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertTableLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InsertTextLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LargeButtons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\largeicons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Mailings.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MailingsLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PageLayout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PageLayoutLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\paragraph.bmp
# End Source File
# Begin Source File

SOURCE=.\res\References.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ReferencesLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shape1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shape7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smallicons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\styles.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tasks.bmp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256.bmp
# End Source File
# Begin Source File

SOURCE=".\res\tooltip-clipboard.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\tooltip-fontdlg.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\tooltip-paragraph.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\View.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ViewLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ViewPat_1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ViewPat_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ViewPat_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Write.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WriteFont.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WriteFont16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WriteLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WriteParagraph.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\xptheme.xml
# End Source File
# End Target
# End Project
