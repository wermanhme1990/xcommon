; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mdibars.h"
LastPage=0

ClassCount=6
Class1=CChildFrame
Class2=CMainFrame
Class3=CMDIBarsApp
Class4=CAboutDlg
Class5=CMDIBarsDoc
Class6=CMDIBarsView

ResourceCount=7
Resource1=IDR_MAINFRAME
Resource2=IDR_TOOLBAR1
Resource3=IDR_POPUP_TOOLBAR
Resource4=IDR_POPUP_MDI_TOOLBAR
Resource5=IDR_MDIBARTYPE
Resource6=IDR_CONTEXT_MENU
Resource7=IDD_ABOUTBOX

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMDIBarsApp]
Type=0
BaseClass=CWinApp
HeaderFile=MDIBars.h
ImplementationFile=MDIBars.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MDIBars.cpp
ImplementationFile=MDIBars.cpp
LastObject=CAboutDlg

[CLS:CMDIBarsDoc]
Type=0
BaseClass=CDocument
HeaderFile=MDIBarsDoc.h
ImplementationFile=MDIBarsDoc.cpp

[CLS:CMDIBarsView]
Type=0
BaseClass=CView
HeaderFile=MDIBarsView.h
ImplementationFile=MDIBarsView.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_COMMAND_1
Command2=ID_COMMAND2
Command3=ID_COMMAND3
CommandCount=3

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_MDIBARTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_STATUS_BAR
Command16=ID_WINDOW_NEW
Command17=ID_WINDOW_CASCADE
Command18=ID_WINDOW_TILE_HORZ
Command19=ID_WINDOW_ARRANGE
Command20=ID_APP_ABOUT
CommandCount=20

[MNU:IDR_CONTEXT_MENU]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
CommandCount=3

[MNU:IDR_POPUP_TOOLBAR]
Type=1
Class=?
Command1=ID_VIEW_TOOLBAR
Command2=ID_VIEW_CHILDBAR
Command3=ID_VIEW_CUSTOMIZE
CommandCount=3

[MNU:IDR_POPUP_MDI_TOOLBAR]
Type=1
Class=?
Command1=ID_VIEW_CHILDBAR
Command2=ID_VIEW_CUSTOMIZE
CommandCount=2

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

