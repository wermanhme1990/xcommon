# Microsoft Developer Studio Project File - Name="Office 2007 (silver)" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Office 2007 (silver) - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "style.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "style.mak" CFG="Office 2007 (silver) - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Office 2007 (silver) - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Office 2007 (silver) - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Office 2007 (silver) - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\bin"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /Gf /Gy /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# SUBTRACT BASE MTL /mktyplib203
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /nodefaultlib /NOENTRY
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /pdb:none /machine:I386 /nodefaultlib /out:"..\..\..\bin\BCGPStyle2007Silver.dll" /NOENTRY

!ELSEIF  "$(CFG)" == "Office 2007 (silver) - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\bin"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /Gf /Gy /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# SUBTRACT BASE MTL /mktyplib203
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /nodefaultlib /NOENTRY
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /pdb:none /machine:I386 /nodefaultlib /out:"..\..\..\bin\BCGPStyle2007Silver.dll" /NOENTRY

!ENDIF 

# Begin Target

# Name "Office 2007 (silver) - Win32 Debug"
# Name "Office 2007 (silver) - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Style.rc
# End Source File
# Begin Source File

SOURCE=.\Style.rc2
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Style_res.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Group "AppCaption"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Back.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Close.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Close_S.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Maximize.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Maximize_S.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Minimize.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Minimize_S.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Restore.bmp
# End Source File
# Begin Source File

SOURCE=.\AppCaption\SysBtn_Restore_S.bmp
# End Source File
# End Group
# Begin Group "ComboBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ComboBox\Btn.bmp
# End Source File
# End Group
# Begin Group "MainWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainWnd\Border.bmp
# End Source File
# Begin Source File

SOURCE=.\MainWnd\BorderCaption.bmp
# End Source File
# End Group
# Begin Group "MenuBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MenuBar\Btn.BMP
# End Source File
# End Group
# Begin Group "PopupBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PopupBar\Border.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\Gripper.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\Item_Back.bmp
# End Source File
# Begin Source File

SOURCE=.\PopupBar\Item_Marker_C.bmp
# End Source File
# Begin Source File

SOURCE=.\PopupBar\Item_Marker_R.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\MenuButton.bmp
# End Source File
# Begin Source File

SOURCE=.\PopupBar\MenuHighlight.bmp
# End Source File
# Begin Source File

SOURCE=.\PopupBar\MenuHighlightDisabled.bmp
# End Source File
# Begin Source File

SOURCE=.\PopupBar\ResizeBar.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\ResizeBar_HV.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\ResizeBar_V.BMP
# End Source File
# Begin Source File

SOURCE=.\PopupBar\Tear.bmp
# End Source File
# End Group
# Begin Group "StatusBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StatusBar\Back.BMP
# End Source File
# Begin Source File

SOURCE=.\StatusBar\Back_Ext.BMP
# End Source File
# Begin Source File

SOURCE=.\StatusBar\PaneBorder.BMP
# End Source File
# Begin Source File

SOURCE=.\StatusBar\SizeGrip.bmp
# End Source File
# End Group
# Begin Group "Tab"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Tab\3D.bmp
# End Source File
# Begin Source File

SOURCE=.\Tab\Flat.BMP
# End Source File
# End Group
# Begin Group "TaskPane"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TaskPane\ScrollBtn.bmp
# End Source File
# End Group
# Begin Group "ToolBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ToolBar\Border.BMP
# End Source File
# Begin Source File

SOURCE=.\ToolBar\Btn.bmp
# End Source File
# End Group
# Begin Group "Ribbon"

# PROP Default_Filter ""
# Begin Group "Buttons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Buttons\Check.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Default_Icon.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Default_Image.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Default_QAT.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Default_QAT_Icon.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Group_F.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Group_L.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Group_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Group_S.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_F_C.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_F_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_L_C.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_L_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_M_C.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\GroupMenu_M_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Launch.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Launch_Icon.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Main.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Main_Panel.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Menu_H_C.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Menu_H_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Menu_V_C.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Menu_V_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Normal_B.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Normal_S.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Palette_B.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Palette_M.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\Palette_T.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Buttons\StatusPane.bmp
# End Source File
# End Group
# Begin Group "Category_Context"

# PROP Default_Filter ""
# Begin Group "Red"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Red\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Red\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Red\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Red\Tab.bmp
# End Source File
# End Group
# Begin Group "Orange"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Orange\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Orange\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Orange\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Orange\Tab.bmp
# End Source File
# End Group
# Begin Group "Yellow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Yellow\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Yellow\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Yellow\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Yellow\Tab.bmp
# End Source File
# End Group
# Begin Group "Green"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Green\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Green\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Green\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Green\Tab.bmp
# End Source File
# End Group
# Begin Group "Blue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Blue\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Blue\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Blue\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Blue\Tab.bmp
# End Source File
# End Group
# Begin Group "Indigo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Indigo\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Indigo\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Indigo\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Indigo\Tab.bmp
# End Source File
# End Group
# Begin Group "Violet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Violet\Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Violet\Caption.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Violet\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Violet\Tab.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Panel_Back_B.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Panel_Back_T.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Context\Separator.bmp
# End Source File
# End Group
# Begin Group "Slider"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Slider\Minus.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Slider\Plus.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Slider\Thumb.bmp
# End Source File
# End Group
# Begin Group "Progress"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ribbon\Progress\Back.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Progress\Infinity.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Progress\Normal.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Progress\Normal_Ext.BMP
# End Source File
# End Group
# Begin Source File

SOURCE=.\Ribbon\Border_Floaty.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Border_QAT.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Caption_QA.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Caption_QA_Glass.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Back.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Tab.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Category_Tab_Sep.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Panel_Back_B.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Panel_Back_T.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Panel_Main.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Panel_Main_Border.BMP
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Panel_QAT.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribbon\Separator.bmp
# End Source File
# End Group
# Begin Group "Outlook"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OutlookWnd\BarBack.BMP
# End Source File
# Begin Source File

SOURCE=.\OutlookWnd\BtnPage.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Style.xml
# End Source File
# End Group
# End Target
# End Project
