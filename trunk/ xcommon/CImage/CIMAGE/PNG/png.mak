# Microsoft Developer Studio Generated NMAKE File, Based on png.dsp
!IF "$(CFG)" == ""
CFG=png - Win32 Release
!MESSAGE No configuration specified. Defaulting to png - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "png - Win32 Release" && "$(CFG)" != "png - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "png.mak" CFG="png - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "png - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "png - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "png - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "$(OUTDIR)\png.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Png.obj"
	-@erase "$(INTDIR)\Pngerror.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\Pngmem.obj"
	-@erase "$(INTDIR)\Pngpread.obj"
	-@erase "$(INTDIR)\Pngread.obj"
	-@erase "$(INTDIR)\Pngrio.obj"
	-@erase "$(INTDIR)\Pngrtran.obj"
	-@erase "$(INTDIR)\Pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\Pngtest.obj"
	-@erase "$(INTDIR)\Pngtrans.obj"
	-@erase "$(INTDIR)\Pngwio.obj"
	-@erase "$(INTDIR)\Pngwrite.obj"
	-@erase "$(INTDIR)\Pngwtran.obj"
	-@erase "$(INTDIR)\Pngwutil.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\zlib" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Png.obj" \
	"$(INTDIR)\Pngerror.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\Pngmem.obj" \
	"$(INTDIR)\Pngpread.obj" \
	"$(INTDIR)\Pngread.obj" \
	"$(INTDIR)\Pngrio.obj" \
	"$(INTDIR)\Pngrtran.obj" \
	"$(INTDIR)\Pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\Pngtest.obj" \
	"$(INTDIR)\Pngtrans.obj" \
	"$(INTDIR)\Pngwio.obj" \
	"$(INTDIR)\Pngwrite.obj" \
	"$(INTDIR)\Pngwtran.obj" \
	"$(INTDIR)\Pngwutil.obj"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "png - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "$(OUTDIR)\png.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Png.obj"
	-@erase "$(INTDIR)\Pngerror.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\Pngmem.obj"
	-@erase "$(INTDIR)\Pngpread.obj"
	-@erase "$(INTDIR)\Pngread.obj"
	-@erase "$(INTDIR)\Pngrio.obj"
	-@erase "$(INTDIR)\Pngrtran.obj"
	-@erase "$(INTDIR)\Pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\Pngtest.obj"
	-@erase "$(INTDIR)\Pngtrans.obj"
	-@erase "$(INTDIR)\Pngwio.obj"
	-@erase "$(INTDIR)\Pngwrite.obj"
	-@erase "$(INTDIR)\Pngwtran.obj"
	-@erase "$(INTDIR)\Pngwutil.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I "..\zlib" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Png.obj" \
	"$(INTDIR)\Pngerror.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\Pngmem.obj" \
	"$(INTDIR)\Pngpread.obj" \
	"$(INTDIR)\Pngread.obj" \
	"$(INTDIR)\Pngrio.obj" \
	"$(INTDIR)\Pngrtran.obj" \
	"$(INTDIR)\Pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\Pngtest.obj" \
	"$(INTDIR)\Pngtrans.obj" \
	"$(INTDIR)\Pngwio.obj" \
	"$(INTDIR)\Pngwrite.obj" \
	"$(INTDIR)\Pngwtran.obj" \
	"$(INTDIR)\Pngwutil.obj"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "png - Win32 Release" || "$(CFG)" == "png - Win32 Debug"
SOURCE=.\Png.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNG_C=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Png.obj" : $(SOURCE) $(DEP_CPP_PNG_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNG_C=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Png.obj" : $(SOURCE) $(DEP_CPP_PNG_C) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngerror.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGER=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngerror.obj" : $(SOURCE) $(DEP_CPP_PNGER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGER=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngerror.obj" : $(SOURCE) $(DEP_CPP_PNGER) "$(INTDIR)"


!ENDIF 

SOURCE=.\pngget.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGGE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\pngget.obj" : $(SOURCE) $(DEP_CPP_PNGGE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGGE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\pngget.obj" : $(SOURCE) $(DEP_CPP_PNGGE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngmem.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGME=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngmem.obj" : $(SOURCE) $(DEP_CPP_PNGME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGME=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngmem.obj" : $(SOURCE) $(DEP_CPP_PNGME) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngpread.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGPR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngpread.obj" : $(SOURCE) $(DEP_CPP_PNGPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGPR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngpread.obj" : $(SOURCE) $(DEP_CPP_PNGPR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngread.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGRE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngread.obj" : $(SOURCE) $(DEP_CPP_PNGRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGRE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngread.obj" : $(SOURCE) $(DEP_CPP_PNGRE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngrio.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGRI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrio.obj" : $(SOURCE) $(DEP_CPP_PNGRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGRI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrio.obj" : $(SOURCE) $(DEP_CPP_PNGRI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngrtran.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGRT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrtran.obj" : $(SOURCE) $(DEP_CPP_PNGRT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGRT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrtran.obj" : $(SOURCE) $(DEP_CPP_PNGRT) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngrutil.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGRU=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrutil.obj" : $(SOURCE) $(DEP_CPP_PNGRU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGRU=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngrutil.obj" : $(SOURCE) $(DEP_CPP_PNGRU) "$(INTDIR)"


!ENDIF 

SOURCE=.\pngset.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGSE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\pngset.obj" : $(SOURCE) $(DEP_CPP_PNGSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGSE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\pngset.obj" : $(SOURCE) $(DEP_CPP_PNGSE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngtest.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGTE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngtest.obj" : $(SOURCE) $(DEP_CPP_PNGTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGTE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngtest.obj" : $(SOURCE) $(DEP_CPP_PNGTE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngtrans.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGTR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngtrans.obj" : $(SOURCE) $(DEP_CPP_PNGTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGTR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngtrans.obj" : $(SOURCE) $(DEP_CPP_PNGTR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngwio.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGWI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwio.obj" : $(SOURCE) $(DEP_CPP_PNGWI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGWI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwio.obj" : $(SOURCE) $(DEP_CPP_PNGWI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngwrite.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGWR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwrite.obj" : $(SOURCE) $(DEP_CPP_PNGWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGWR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwrite.obj" : $(SOURCE) $(DEP_CPP_PNGWR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngwtran.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGWT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwtran.obj" : $(SOURCE) $(DEP_CPP_PNGWT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGWT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwtran.obj" : $(SOURCE) $(DEP_CPP_PNGWT) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pngwutil.c

!IF  "$(CFG)" == "png - Win32 Release"

DEP_CPP_PNGWU=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwutil.obj" : $(SOURCE) $(DEP_CPP_PNGWU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "png - Win32 Debug"

DEP_CPP_PNGWU=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\png.h"\
	".\pngconf.h"\
	

"$(INTDIR)\Pngwutil.obj" : $(SOURCE) $(DEP_CPP_PNGWU) "$(INTDIR)"


!ENDIF 


!ENDIF 

