; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "demo.h"
LastPage=0

ClassCount=7
Class1=CApp
Class2=CRotateDialog
Class3=CFrame
Class4=CImageDoc
Class5=CImageFrame
Class6=CImageView
Class7=CMDIFaceWnd

ResourceCount=5
Resource1=IDR_IMAGE_TYPE
Resource2=IDD_ROTATE
Resource3=IDR_MAINFRAME
Resource4=LMFC_IDR_PREVIEW
Resource5=IDD_ABOUTBOX

[CLS:CApp]
Type=0
BaseClass=CWinApp
HeaderFile=app.h
ImplementationFile=app.cpp
LastObject=AFX_ID_PREVIEW_CLOSE

[CLS:CRotateDialog]
Type=0
BaseClass=CDialog
HeaderFile=DlgRotate.h
ImplementationFile=DlgRotate.cpp

[CLS:CFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=frame.h
ImplementationFile=frame.cpp
Filter=T
VirtualFilter=fWC
LastObject=CFrame

[CLS:CImageDoc]
Type=0
BaseClass=CDocument
HeaderFile=imgdoc.h
ImplementationFile=imgdoc.cpp
Filter=N
VirtualFilter=DC
LastObject=CImageDoc

[CLS:CImageFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=imgframe.h
ImplementationFile=imgframe.cpp

[CLS:CImageView]
Type=0
BaseClass=CScrollView
HeaderFile=imgview.h
ImplementationFile=imgview.cpp
Filter=C
VirtualFilter=VWC
LastObject=CImageView

[CLS:CMDIFaceWnd]
Type=0
BaseClass=CWnd
HeaderFile=mdiface.h
ImplementationFile=mdiface.cpp

[DLG:IDD_ROTATE]
Type=1
Class=CRotateDialog
ControlCount=4
Control1=IDC_EDIT,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352

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
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_EDIT_PASTE
Command7=IDR_IMAGE_BMP
Command8=IDR_IMAGE_JPEG
Command9=IDR_IMAGE_ICON
Command10=IDR_IMAGE_PNG
Command11=IDR_IMAGE_GIF
Command12=ID_VIEW_TOOLBAR
Command13=ID_VIEW_STATUS_BAR
Command14=ID_APP_ABOUT
CommandCount=14

[MNU:IDR_IMAGE_TYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_REDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_STRETCH_MODE
Command18=IDR_IMAGE_BMP
Command19=IDR_IMAGE_JPEG
Command20=IDR_IMAGE_ICON
Command21=IDR_IMAGE_PNG
Command22=IDR_IMAGE_GIF
Command23=ID_TRANSFORM_ELLIPSE
Command24=ID_CIMAGE_MIRROR
Command25=ID_CIMAGE_FLIP
Command26=ID_CIMAGE_NEGATIVE
Command27=ID_CIMAGE_GREYSCALE
Command28=ID_CIMAGE_ROTATE
Command29=ID_WINDOW_NEW
Command30=ID_WINDOW_CASCADE
Command31=ID_WINDOW_TILE_HORZ
Command32=ID_WINDOW_ARRANGE
Command33=ID_WINDOW_DUPLICATE
Command34=ID_APP_ABOUT
CommandCount=34

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE_AS
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
Command14=ID_EDIT_UNDO
CommandCount=14

[ACL:LMFC_IDR_PREVIEW]
Type=1
Class=?
Command1=AFX_ID_PREVIEW_ZOOMIN
Command2=AFX_ID_PREVIEW_CLOSE
Command3=AFX_ID_PREVIEW_NEXT
Command4=AFX_ID_PREVIEW_PREV
Command5=AFX_ID_PREVIEW_ZOOMOUT
CommandCount=5

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDCANCEL,button,1342373889
Control4=IDC_STATIC,static,1342308480
Control5=IDC_STATIC,static,1342308480

