# Microsoft Developer Studio Generated NMAKE File, Based on freepy.dsp
!IF "$(CFG)" == ""
CFG=freepy - Win32 Debug
!MESSAGE No configuration specified. Defaulting to freepy - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "freepy - Win32 Release" && "$(CFG)" != "freepy - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freepy.mak" CFG="freepy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freepy - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freepy - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "freepy - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\freepy.dll" "$(OUTDIR)\freepy.bsc"

!ELSE 

ALL : "$(OUTDIR)\freepy.dll" "$(OUTDIR)\freepy.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\freepy.obj"
	-@erase "$(INTDIR)\freepy.res"
	-@erase "$(INTDIR)\freepy.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\handle.obj"
	-@erase "$(INTDIR)\handle.sbr"
	-@erase "$(INTDIR)\HZlib.obj"
	-@erase "$(INTDIR)\HZlib.sbr"
	-@erase "$(INTDIR)\HZmodei.obj"
	-@erase "$(INTDIR)\HZmodei.sbr"
	-@erase "$(INTDIR)\HZmodenormal.obj"
	-@erase "$(INTDIR)\HZmodenormal.sbr"
	-@erase "$(INTDIR)\HZmodephtoch.obj"
	-@erase "$(INTDIR)\HZmodephtoch.sbr"
	-@erase "$(INTDIR)\HZmodeu.obj"
	-@erase "$(INTDIR)\HZmodeu.sbr"
	-@erase "$(INTDIR)\HZmodev.obj"
	-@erase "$(INTDIR)\HZmodev.sbr"
	-@erase "$(INTDIR)\HZspecial.obj"
	-@erase "$(INTDIR)\HZspecial.sbr"
	-@erase "$(INTDIR)\HZutil.obj"
	-@erase "$(INTDIR)\HZutil.sbr"
	-@erase "$(INTDIR)\imm.obj"
	-@erase "$(INTDIR)\imm.sbr"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\uicand.obj"
	-@erase "$(INTDIR)\uicand.sbr"
	-@erase "$(INTDIR)\uicomp.obj"
	-@erase "$(INTDIR)\uicomp.sbr"
	-@erase "$(INTDIR)\uistatus.obj"
	-@erase "$(INTDIR)\uistatus.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\freepy.bsc"
	-@erase "$(OUTDIR)\freepy.dll"
	-@erase "$(OUTDIR)\freepy.exp"
	-@erase "$(OUTDIR)\freepy.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\freepy.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\freepy.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freepy.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\freepy.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\handle.sbr" \
	"$(INTDIR)\HZlib.sbr" \
	"$(INTDIR)\HZmodei.sbr" \
	"$(INTDIR)\HZmodenormal.sbr" \
	"$(INTDIR)\HZmodephtoch.sbr" \
	"$(INTDIR)\HZmodeu.sbr" \
	"$(INTDIR)\HZmodev.sbr" \
	"$(INTDIR)\HZspecial.sbr" \
	"$(INTDIR)\HZutil.sbr" \
	"$(INTDIR)\imm.sbr" \
	"$(INTDIR)\Process.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\uicand.sbr" \
	"$(INTDIR)\uicomp.sbr" \
	"$(INTDIR)\uistatus.sbr"

"$(OUTDIR)\freepy.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib imm32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\freepy.pdb" /machine:I386 /def:".\freepy.def"\
 /out:"$(OUTDIR)\freepy.dll" /implib:"$(OUTDIR)\freepy.lib" 
DEF_FILE= \
	".\freepy.def"
LINK32_OBJS= \
	"$(INTDIR)\freepy.obj" \
	"$(INTDIR)\freepy.res" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\handle.obj" \
	"$(INTDIR)\HZlib.obj" \
	"$(INTDIR)\HZmodei.obj" \
	"$(INTDIR)\HZmodenormal.obj" \
	"$(INTDIR)\HZmodephtoch.obj" \
	"$(INTDIR)\HZmodeu.obj" \
	"$(INTDIR)\HZmodev.obj" \
	"$(INTDIR)\HZspecial.obj" \
	"$(INTDIR)\HZutil.obj" \
	"$(INTDIR)\imm.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\uicand.obj" \
	"$(INTDIR)\uicomp.obj" \
	"$(INTDIR)\uistatus.obj"

"$(OUTDIR)\freepy.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\freepy.dll" "$(OUTDIR)\freepy.bsc"

!ELSE 

ALL : "$(OUTDIR)\freepy.dll" "$(OUTDIR)\freepy.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\freepy.obj"
	-@erase "$(INTDIR)\freepy.res"
	-@erase "$(INTDIR)\freepy.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\handle.obj"
	-@erase "$(INTDIR)\handle.sbr"
	-@erase "$(INTDIR)\HZlib.obj"
	-@erase "$(INTDIR)\HZlib.sbr"
	-@erase "$(INTDIR)\HZmodei.obj"
	-@erase "$(INTDIR)\HZmodei.sbr"
	-@erase "$(INTDIR)\HZmodenormal.obj"
	-@erase "$(INTDIR)\HZmodenormal.sbr"
	-@erase "$(INTDIR)\HZmodephtoch.obj"
	-@erase "$(INTDIR)\HZmodephtoch.sbr"
	-@erase "$(INTDIR)\HZmodeu.obj"
	-@erase "$(INTDIR)\HZmodeu.sbr"
	-@erase "$(INTDIR)\HZmodev.obj"
	-@erase "$(INTDIR)\HZmodev.sbr"
	-@erase "$(INTDIR)\HZspecial.obj"
	-@erase "$(INTDIR)\HZspecial.sbr"
	-@erase "$(INTDIR)\HZutil.obj"
	-@erase "$(INTDIR)\HZutil.sbr"
	-@erase "$(INTDIR)\imm.obj"
	-@erase "$(INTDIR)\imm.sbr"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\uicand.obj"
	-@erase "$(INTDIR)\uicand.sbr"
	-@erase "$(INTDIR)\uicomp.obj"
	-@erase "$(INTDIR)\uicomp.sbr"
	-@erase "$(INTDIR)\uistatus.obj"
	-@erase "$(INTDIR)\uistatus.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\freepy.bsc"
	-@erase "$(OUTDIR)\freepy.dll"
	-@erase "$(OUTDIR)\freepy.exp"
	-@erase "$(OUTDIR)\freepy.ilk"
	-@erase "$(OUTDIR)\freepy.lib"
	-@erase "$(OUTDIR)\freepy.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\freepy.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\freepy.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freepy.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\freepy.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\handle.sbr" \
	"$(INTDIR)\HZlib.sbr" \
	"$(INTDIR)\HZmodei.sbr" \
	"$(INTDIR)\HZmodenormal.sbr" \
	"$(INTDIR)\HZmodephtoch.sbr" \
	"$(INTDIR)\HZmodeu.sbr" \
	"$(INTDIR)\HZmodev.sbr" \
	"$(INTDIR)\HZspecial.sbr" \
	"$(INTDIR)\HZutil.sbr" \
	"$(INTDIR)\imm.sbr" \
	"$(INTDIR)\Process.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\uicand.sbr" \
	"$(INTDIR)\uicomp.sbr" \
	"$(INTDIR)\uistatus.sbr"

"$(OUTDIR)\freepy.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib imm32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\freepy.pdb" /debug /machine:I386 /def:".\freepy.def"\
 /out:"$(OUTDIR)\freepy.dll" /implib:"$(OUTDIR)\freepy.lib" /pdbtype:sept 
DEF_FILE= \
	".\freepy.def"
LINK32_OBJS= \
	"$(INTDIR)\freepy.obj" \
	"$(INTDIR)\freepy.res" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\handle.obj" \
	"$(INTDIR)\HZlib.obj" \
	"$(INTDIR)\HZmodei.obj" \
	"$(INTDIR)\HZmodenormal.obj" \
	"$(INTDIR)\HZmodephtoch.obj" \
	"$(INTDIR)\HZmodeu.obj" \
	"$(INTDIR)\HZmodev.obj" \
	"$(INTDIR)\HZspecial.obj" \
	"$(INTDIR)\HZutil.obj" \
	"$(INTDIR)\imm.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\uicand.obj" \
	"$(INTDIR)\uicomp.obj" \
	"$(INTDIR)\uistatus.obj"

"$(OUTDIR)\freepy.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "freepy - Win32 Release" || "$(CFG)" == "freepy - Win32 Debug"
SOURCE=.\freepy.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_FREEP=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\freepy.obj"	"$(INTDIR)\freepy.sbr" : $(SOURCE) $(DEP_CPP_FREEP)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_FREEP=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\freepy.obj"	"$(INTDIR)\freepy.sbr" : $(SOURCE) $(DEP_CPP_FREEP)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\freepy.rc
DEP_RSC_FREEPY=\
	".\freepy.ico"\
	".\Penindic.ico"\
	".\status.bmp"\
	

"$(INTDIR)\freepy.res" : $(SOURCE) $(DEP_RSC_FREEPY) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\global.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_GLOBA=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) $(DEP_CPP_GLOBA)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_GLOBA=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) $(DEP_CPP_GLOBA)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\handle.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HANDL=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\handle.obj"	"$(INTDIR)\handle.sbr" : $(SOURCE) $(DEP_CPP_HANDL)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HANDL=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\handle.obj"	"$(INTDIR)\handle.sbr" : $(SOURCE) $(DEP_CPP_HANDL)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\HZlib.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZLIB=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZlib.obj"	"$(INTDIR)\HZlib.sbr" : $(SOURCE) $(DEP_CPP_HZLIB)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZLIB=\
	".\HZfreepy.h"\
	

"$(INTDIR)\HZlib.obj"	"$(INTDIR)\HZlib.sbr" : $(SOURCE) $(DEP_CPP_HZLIB)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodei.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZMOD=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodei.obj"	"$(INTDIR)\HZmodei.sbr" : $(SOURCE) $(DEP_CPP_HZMOD)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZMOD=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodei.obj"	"$(INTDIR)\HZmodei.sbr" : $(SOURCE) $(DEP_CPP_HZMOD)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodenormal.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZMODE=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodenormal.obj"	"$(INTDIR)\HZmodenormal.sbr" : $(SOURCE)\
 $(DEP_CPP_HZMODE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZMODE=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodenormal.obj"	"$(INTDIR)\HZmodenormal.sbr" : $(SOURCE)\
 $(DEP_CPP_HZMODE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodephtoch.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZMODEP=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodephtoch.obj"	"$(INTDIR)\HZmodephtoch.sbr" : $(SOURCE)\
 $(DEP_CPP_HZMODEP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZMODEP=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodephtoch.obj"	"$(INTDIR)\HZmodephtoch.sbr" : $(SOURCE)\
 $(DEP_CPP_HZMODEP) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodeu.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZMODEU=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodeu.obj"	"$(INTDIR)\HZmodeu.sbr" : $(SOURCE) $(DEP_CPP_HZMODEU)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZMODEU=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodeu.obj"	"$(INTDIR)\HZmodeu.sbr" : $(SOURCE) $(DEP_CPP_HZMODEU)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodev.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZMODEV=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodev.obj"	"$(INTDIR)\HZmodev.sbr" : $(SOURCE) $(DEP_CPP_HZMODEV)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZMODEV=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZmodev.obj"	"$(INTDIR)\HZmodev.sbr" : $(SOURCE) $(DEP_CPP_HZMODEV)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\HZspecial.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZSPE=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZspecial.obj"	"$(INTDIR)\HZspecial.sbr" : $(SOURCE)\
 $(DEP_CPP_HZSPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZSPE=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZspecial.obj"	"$(INTDIR)\HZspecial.sbr" : $(SOURCE)\
 $(DEP_CPP_HZSPE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZutil.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_HZUTI=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\HZutil.obj"	"$(INTDIR)\HZutil.sbr" : $(SOURCE) $(DEP_CPP_HZUTI)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_HZUTI=\
	".\HZfreepy.h"\
	

"$(INTDIR)\HZutil.obj"	"$(INTDIR)\HZutil.sbr" : $(SOURCE) $(DEP_CPP_HZUTI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\imm.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_IMM_C=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\imm.obj"	"$(INTDIR)\imm.sbr" : $(SOURCE) $(DEP_CPP_IMM_C)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_IMM_C=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\imm.obj"	"$(INTDIR)\imm.sbr" : $(SOURCE) $(DEP_CPP_IMM_C)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\Process.c

"$(INTDIR)\Process.obj"	"$(INTDIR)\Process.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\subs.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_SUBS_=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\subs.obj"	"$(INTDIR)\subs.sbr" : $(SOURCE) $(DEP_CPP_SUBS_)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_SUBS_=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\subs.obj"	"$(INTDIR)\subs.sbr" : $(SOURCE) $(DEP_CPP_SUBS_)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\uicand.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_UICAN=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uicand.obj"	"$(INTDIR)\uicand.sbr" : $(SOURCE) $(DEP_CPP_UICAN)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_UICAN=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uicand.obj"	"$(INTDIR)\uicand.sbr" : $(SOURCE) $(DEP_CPP_UICAN)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\uicomp.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_UICOM=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uicomp.obj"	"$(INTDIR)\uicomp.sbr" : $(SOURCE) $(DEP_CPP_UICOM)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_UICOM=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uicomp.obj"	"$(INTDIR)\uicomp.sbr" : $(SOURCE) $(DEP_CPP_UICOM)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\uistatus.c

!IF  "$(CFG)" == "freepy - Win32 Release"

DEP_CPP_UISTA=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uistatus.obj"	"$(INTDIR)\uistatus.sbr" : $(SOURCE) $(DEP_CPP_UISTA)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

DEP_CPP_UISTA=\
	".\freepy.h"\
	".\HZfreepy.h"\
	".\Indicml.h"\
	".\Vksub.h"\
	

"$(INTDIR)\uistatus.obj"	"$(INTDIR)\uistatus.sbr" : $(SOURCE) $(DEP_CPP_UISTA)\
 "$(INTDIR)"


!ENDIF 


!ENDIF 

