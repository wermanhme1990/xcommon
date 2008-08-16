# Microsoft Developer Studio Project File - Name="BCGCBPro" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BCGCBPro - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BCGCBPro.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BCGCBPro.mak" CFG="BCGCBPro - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BCGCBPro - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BCGCBPro - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BCGCBPro - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BCGCBPro - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BCGCBPro - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BCGCBPRO_" /D "BCG_NO_SIZINGBAR" /D "BCG_NO_OUTLOOKBAR" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\bin\BCGCBPRO940.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "BCGCBPro - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BCGCBPRO_" /D "BCG_NO_SIZINGBAR" /D "BCG_NO_OUTLOOKBAR" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\bin\BCGCBPRO940d.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BCGCBPro - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BCGCBPro___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BCGCBPro___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BCGCBPRO_" /D "BCG_NO_SIZINGBAR" /D "BCG_NO_OUTLOOKBAR" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_BCGCBPRO_" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\bin\BCGCBPRO940d.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\bin\BCGCBPRO940ud.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BCGCBPro - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BCGCBPro___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "BCGCBPro___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "ReleaseU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BCGCBPRO_" /D "BCG_NO_SIZINGBAR" /D "BCG_NO_OUTLOOKBAR" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "_BCGCBPRO_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\bin\BCGCBPRO940.dll"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\bin\BCGCBPRO940u.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "BCGCBPro - Win32 Release"
# Name "BCGCBPro - Win32 Debug"
# Name "BCGCBPro - Win32 Unicode Debug"
# Name "BCGCBPro - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BCGCBPro.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGCBPro.rc
# End Source File
# Begin Source File

SOURCE=.\Bcgglobals.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAnimCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAppBarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideDockBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBarContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBarContainerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseTabbedBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCalculator.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCalendarBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCalendarMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPColorBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPColorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPColorMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCommandManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPContextMenuManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPControlBarImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPControlRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPCustomizeMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDAOGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDBGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDlgImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDockBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDockBarRow.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDockingCBWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDockingControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDockManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDragFrameImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDrawManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDropDown.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDropDownList.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPDurationCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPEditListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPExCheckList.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPFontComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPFrameImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPFullScreenImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPGlobalUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPGridView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPHotSpotImageCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPImageEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPInplaceToolTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPIntelliSenseLB.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPIntelliSenseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyboardManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyboardPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMainClientAreaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMDIChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMDIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMiniFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMouseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMousePage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPMultiMiniFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPODBCGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOleCntrFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOleDocIPFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOleIPFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOleServerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBarDockingPane.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBarPane.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPowerColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPrintPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPropList.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPReBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRebarState.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRecentDockInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPReportView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSDPlaceMarkerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSelectSkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShellList.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShellManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShellTree.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShowAllButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSkinManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSmartDockingManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSmartDockingMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSound.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSpinButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTabbedControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTabbedToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTagManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTaskPaneMiniFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTasksPane.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTearOffManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarComboBoxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarCustomize.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarCustomizePages.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDropSource.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarEditBoxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarFontCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBarImages.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarMenuButtonsButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarSpinEditBoxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarSystemMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPToolTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPTooltipManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPURLLinkButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPUserTool.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPUserToolsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager2003.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager2007.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManagerVS2005.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManagerXP.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPWindowsManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPWinXPVisualManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonAppearanceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonsList.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonsTextList.cpp
# End Source File
# Begin Source File

SOURCE=.\CalendarPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdUsageCount.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPage1.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPage2.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ImagePaintArea.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyAssign.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuHash.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuImages.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertySheetCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RegPath.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolbarNameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackMouse.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BCGCBPro.h
# End Source File
# Begin Source File

SOURCE=.\BCGCBProInc.h
# End Source File
# Begin Source File

SOURCE=.\BCGCBProVer.h
# End Source File
# Begin Source File

SOURCE=.\Bcgglobals.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAccessibility.h
# End Source File
# Begin Source File

SOURCE=.\BCGPADOGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAnimCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAppBarWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideDockBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAutoHideToolBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBarContainer.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBarContainerManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseControlBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseTabbedBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseTabWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseToolBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCalculator.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCalendarBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCalendarMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCaptionMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPColorBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPColorButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPColorDialog.h
# End Source File
# Begin Source File

SOURCE=.\BCGPColorMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCommandManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPContextMenuManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPControlBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPControlBarImpl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPControlRenderer.h
# End Source File
# Begin Source File

SOURCE=.\BCGPCustomizeMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDAOGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDBGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDialog.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDlgImpl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDockBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDockBarRow.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDockingCBWrapper.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDockingControlBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDockManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDragFrameImpl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDrawManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDropDown.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDropDownList.h
# End Source File
# Begin Source File

SOURCE=.\BCGPDurationCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPEdit.h
# End Source File
# Begin Source File

SOURCE=.\BCGPEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPEditListBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPEditView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPExCheckList.h
# End Source File
# Begin Source File

SOURCE=.\BCGPFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\BCGPFontComboBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPFrameImpl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPFullScreenImpl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPGlobalUtils.h
# End Source File
# Begin Source File

SOURCE=.\BCGPGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPGridView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPHelpids.h
# End Source File
# Begin Source File

SOURCE=.\BCGPHotSpotImageCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPImageEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPInplaceToolTipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPIntelliSenseLB.h
# End Source File
# Begin Source File

SOURCE=.\BCGPIntelliSenseWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyboardManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyboardPage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyHelper.h
# End Source File
# Begin Source File

SOURCE=.\BCGPKeyMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPLocalResource.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMainClientAreaWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMaskEdit.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMDIChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMDIFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMenuPage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMiniFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMouseManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMousePage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMultiDocTemplate.h
# End Source File
# Begin Source File

SOURCE=.\BCGPMultiMiniFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPODBCGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOleCntrFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOleDocIPFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOleIPFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOleServerDoc.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOptionsPage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlineParser.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBarDockingPane.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookBarPane.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPOutlookWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupMenuBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPopupWindow.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPowerColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPrintPreviewView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPropList.h
# End Source File
# Begin Source File

SOURCE=.\BCGPReBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRebarState.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRecentDockInfo.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRegistry.h
# End Source File
# Begin Source File

SOURCE=.\BCGPReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPReportView.h
# End Source File
# Begin Source File

SOURCE=.\bcgprores.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSDPlaceMarkerWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSelectSkinDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShellList.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShellManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShellTree.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShowAllButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSkinEntry.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSkinManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSlider.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSmartDockingManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSmartDockingMarker.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSound.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSpinButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTabbedControlBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTabbedToolbar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTabView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTabWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTagManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTaskPaneMiniFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTasksPane.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTearOffManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarComboBoxButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarCustomize.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarCustomizePages.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDropSource.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarEditBoxButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarFontCombo.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBarImages.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarMenuButtonsButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarSpinEditBoxButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolbarSystemMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolsPage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPToolTipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPTooltipManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPURLLinkButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPUserTool.h
# End Source File
# Begin Source File

SOURCE=.\BCGPUserToolsManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager2003.h
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManager2007.h
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManagerVS2005.h
# End Source File
# Begin Source File

SOURCE=.\BCGPVisualManagerXP.h
# End Source File
# Begin Source File

SOURCE=.\BCGPWindowsManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPWinXPVisualManager.h
# End Source File
# Begin Source File

SOURCE=.\BCGPWorkspace.h
# End Source File
# Begin Source File

SOURCE=.\ButtonAppearanceDlg.h
# End Source File
# Begin Source File

SOURCE=.\ButtonsList.h
# End Source File
# Begin Source File

SOURCE=.\ButtonsTextList.h
# End Source File
# Begin Source File

SOURCE=.\CalendarPopup.h
# End Source File
# Begin Source File

SOURCE=.\CmdUsageCount.h
# End Source File
# Begin Source File

SOURCE=.\ColorPage1.h
# End Source File
# Begin Source File

SOURCE=.\ColorPage2.h
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeButton.h
# End Source File
# Begin Source File

SOURCE=.\ImagePaintArea.h
# End Source File
# Begin Source File

SOURCE=.\KeyAssign.h
# End Source File
# Begin Source File

SOURCE=.\MenuHash.h
# End Source File
# Begin Source File

SOURCE=.\MenuImages.h
# End Source File
# Begin Source File

SOURCE=.\PropertySheetCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RegPath.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarNameDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrackMouse.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\appClock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\appClockM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\appUpDown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BCGCBPro.rc2
# End Source File
# Begin Source File

SOURCE=.\res\browse.bmp
# End Source File
# Begin Source File

SOURCE=.\res\browse32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calendar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calendar_icons.bmp
# End Source File
# Begin Source File

SOURCE=".\res\color-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\copy-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\copy-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\copy-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\delete-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\delete-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\delete-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\deletecolumn.cur
# End Source File
# Begin Source File

SOURCE=".\res\down-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\down-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ellipse-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\fill-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\font-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\hand-pro.cur"
# End Source File
# Begin Source File

SOURCE=".\res\help-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\help-pro.ico"
# End Source File
# Begin Source File

SOURCE=".\res\help-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\line-pro.cur"
# End Source File
# Begin Source File

SOURCE=.\res\logo24.bmp
# End Source File
# Begin Source File

SOURCE=".\res\mail-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\menuimg-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\menuimg-pro24.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\move-pro.cur"
# End Source File
# Begin Source File

SOURCE=.\res\movetab.cur
# End Source File
# Begin Source File

SOURCE=".\res\new-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\new-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\nomovetab.cur
# End Source File
# Begin Source File

SOURCE=".\res\palette-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\palette-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\pen-pro.cur"
# End Source File
# Begin Source File

SOURCE=.\res\picker.bmp
# End Source File
# Begin Source File

SOURCE=".\res\print-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\print-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PrintPreview-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PrintPreview-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\rect-pro.cur"
# End Source File
# Begin Source File

SOURCE=.\res\ribbon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RibbonPrintL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RibbonPrintS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDbottom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDCenter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDleft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDmiddle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDright.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDtop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\selectrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\taskpane.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskpane32.bmp
# End Source File
# Begin Source File

SOURCE=".\res\tool-pro.ico"
# End Source File
# Begin Source File

SOURCE=".\res\up-pro.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\up-pro32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\url-pro.bmp"
# End Source File
# End Group
# Begin Group "Calendar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCGPAppointment.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointment.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentDS.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentDS.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentProperty.h
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPAppointmentStorage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerClockIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerClockIcons.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerDropSource.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerDropSource.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerManagerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerManagerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerManagerView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerManagerView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrint.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintDay.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintDay.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintDual.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintDual.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintMonth.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerPrintWeek.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerView.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerView.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewDay.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewDay.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewMonth.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewWeek.h
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewWorkWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPPlannerViewWorkWeek.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrence.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrence.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrenceRule.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrenceRule.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrenceRules.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRecurrenceRules.h
# End Source File
# End Group
# Begin Group "Ribbon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCGPBaseRibbonElement.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPBaseRibbonElement.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonButtonsGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonButtonsGroup.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCategory.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCategory.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonColorButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonComboBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCommandsListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCommandsListBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCustomizePage.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonCustomizePage.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonEdit.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonFloaty.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonFloaty.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonKeyboardCustomizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonKeyboardCustomizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonLabel.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonMainPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonMainPanel.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPaletteButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPaletteButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPanel.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPanelMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonPanelMenu.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonQuickAccessToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonQuickAccessToolbar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonSlider.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonStatusBarPane.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPRibbonStatusBarPane.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
