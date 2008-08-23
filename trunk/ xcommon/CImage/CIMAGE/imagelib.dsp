# Microsoft Developer Studio Project File - Name="imagelib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=imagelib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "imagelib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "imagelib.mak" CFG="imagelib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "imagelib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "imagelib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "imagelib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\imagelib"
# PROP Intermediate_Dir "..\Release\imagelib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /MD /W4 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_MBCS" /YX /FD /GF /c
# SUBTRACT CPP /nologo
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\LIB\imagelib.lib"

!ELSEIF  "$(CFG)" == "imagelib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\imagelib"
# PROP Intermediate_Dir "..\Debug\imagelib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /FR /YX /FD /GZ /GF /c
# SUBTRACT CPP /nologo
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\LIB\imagelib_d.lib"

!ENDIF 

# Begin Target

# Name "imagelib - Win32 Release"
# Name "imagelib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SRC\cimage.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imabmp.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imagif.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imaicon.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imajpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imapng.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\imatiff.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\INCLUDE\cimage.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\cimage.inl
# End Source File
# Begin Source File

SOURCE=.\SRC\imabmp.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imagif.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imaicon.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imajpeg.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imapng.h
# End Source File
# Begin Source File

SOURCE=.\SRC\imatiff.h
# End Source File
# Begin Source File

SOURCE=.\SRC\iterator.h
# End Source File
# End Group
# Begin Group "JPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\JPEG\JCAPIMIN.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCAPISTD.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCCOEFCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCCOLOR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCDCTMGR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCHUFF.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCINIT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCMAINCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCMARKER.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCMASTER.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCOMAPI.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCPARAM.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCPHUFF.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCPREPCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCSAMPLE.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JCTRANS.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDAPIMIN.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDAPISTD.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDATADST.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDATASRC.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDCOEFCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDCOLOR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDDCTMGR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDHUFF.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDINPUT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDMAINCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDMARKER.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDMASTER.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDMERGE.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDPHUFF.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDPOSTCT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDSAMPLE.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JDTRANS.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JERROR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JFDCTFLT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JFDCTFST.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JFDCTINT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JIDCTFLT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JIDCTFST.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JIDCTINT.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JIDCTRED.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JMEMMGR.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JMEMNOBS.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JQUANT1.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JQUANT2.C
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\JPEG\JUTILS.C
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PNG\png.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngerror.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pnggccrd.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngget.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngmem.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngpread.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngread.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngrio.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngrtran.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngrutil.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngset.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngtest.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngtrans.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngvcrd.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngwio.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngwrite.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngwtran.c
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PNG\pngwutil.c
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "TIFF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TIFF\fax3sm_winnt.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_aux.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_c.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_close.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_codec.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_compress.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_dir.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_dirinfo.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_dirread.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_dirwrite.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_dumpmode.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_error.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_fax3.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_flush.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_getimage.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_jpeg.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_luv.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_lzw.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_next.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_open.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_packbits.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_pixarlog.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_predict.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_print.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_read.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_strip.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_swab.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_thunder.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_tile.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_version.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_warning.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_write.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# Begin Source File

SOURCE=.\TIFF\tif_zip.c
# ADD CPP /D "AVOID_WIN32_FILEIO" /YX
# End Source File
# End Group
# Begin Source File

SOURCE=.\cimage.html
# End Source File
# End Target
# End Project
