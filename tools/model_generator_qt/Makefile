#
# to build the GenConfig utility
#
## common defintions ##
PROJECT	= GenConfig 
COMNPATH = 
GUIDIR	= .\UI
QTROOT	= D:\Qt\4.8.6
VCROOT		= $(VCINSTALLDIR)
MSSDKROOT	= $(WINDOWSDKDIR)
cc			= cl.exe
link		= link.exe
#
!IFDEF	NODEBUG
OUTDIR	= .\Release
!ELSE
OUTDIR	= .\Debug
!ENDIF
#
OBJECTS	= $(OUTDIR)\GenConfig.obj	\
		  $(OUTDIR)\MainFrame.obj	\
		  $(OUTDIR)\moc_MainFrame.obj	\
		  $(OUTDIR)\qrc_GenConfig.obj	\
		  $(OUTDIR)\GenConfig.res \
		  $(OUTDIR)\AttributeType.obj $(OUTDIR)\SclParser.obj \
		  $(OUTDIR)\Utils.obj $(OUTDIR)\pugixml.obj

DEPENDS	= 

## target ##
all: $(OUTDIR) $(DEPENDS) $(OUTDIR)\$(PROJECT).exe

## cflags ##
ccommon		= -c
!IFDEF NODEBUG
cdebug		= -Ox
cvars		= -MT -D_MT
!ELSE
cvars		= -MTd -D_MTd
cdebug		= -Zi -Od -DDEBUG
!ENDIF

cflags		= $(ccommon) $(cdebug) $(cvars) \
			  -D_X86=1 -DWIN32 -D_WIN32 -D_CRT_SECURE_NO_WARNINGS \
			  -W3 /Zc:wchar_t

## flags for Qt
QTDEFINES	= -DQT_CORE_LIB -DQT_GUI_LIB -DQT_LARGEFILE_SUPPORT
QTINCPATH	= -I"$(QTROOT)\include\QtCore" -I"$(QTROOT)\include\QtGui" \
			  -I"$(QTROOT)\include" -I"$(QTROOT)\include\QtWidgets" \
			  -I"parser\pugixml" -I"parser"

incpath		= $(QTINCPATH)
cflags		= $(cflags) $(cvars) $(cdebug) $(incpath)

## create output directory ##
$(OUTDIR):
	IF NOT EXIST "$(OUTDIR)/$(NULL)" MKDIR "$(OUTDIR)"

### implicit rules
.SUFFIXES: .cpp .c

{.}.cpp{$(OUTDIR)}.obj:
	$(cc) $(cflags) /Fo"$(OUTDIR)\\" /Fp"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $<

{$(OUTDIR)}.cpp{$(OUTDIR)}.obj:
	$(cc) $(cflags) /Fo"$(OUTDIR)\\" /Fp"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $<

{.\parser}.cpp{$(OUTDIR)}.obj:
	$(cc) $(cflags) /Fo"$(OUTDIR)\\" /Fp"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $<

{.\parser\pugixml}.cpp{$(OUTDIR)}.obj:
	$(cc) $(cflags) /Fo"$(OUTDIR)\\" /Fp"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $<

## uic ##
$(GUIDIR)\uic_MainFrame.h : $(GUIDIR)\MainFrame.ui
	$(QTROOT)\bin\uic -o $(GUIDIR)\uic_MainFrame.h $(GUIDIR)\MainFrame.ui

## moc ##
$(OUTDIR)\moc_GenConfig.cpp : GenConfig.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_GenConfig.cpp GenConfig.h

$(OUTDIR)\moc_MainFrame.cpp : MainFrame.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_MainFrame.cpp MainFrame.h

## resource ##
$(OUTDIR)\qrc_GenConfig.cpp : .\GenConfig.qrc
	$(QTROOT)\bin\rcc -name GenConfig -o $(OUTDIR)\qrc_GenConfig.cpp .\GenConfig.qrc

$(OUTDIR)\GenConfig.res : .\GenConfig.rc
	rc.exe -fo $(OUTDIR)\GenConfig.res .\GenConfig.rc

## link ##
QTLIBPATH	= /LIBPATH:"$(QTROOT)\lib"
!IFDEF	NODEBUG
QTLIBS	= QtGui.lib QtCore.lib QtMain.lib
!ELSE
QTLIBS	= QtGuid.lib QtCored.lib QtMaind.lib
!ENDIF

WIN32LIBS	= kernel32.lib user32.lib gdi32.lib ws2_32.lib imm32.lib \
			  winmm.lib comdlg32.lib shell32.lib advapi32.lib \
			  winspool.lib shlwapi.lib

LIBS	= $(WIN32LIBS) $(QTLIBS) $(PRILIBS)
lflags	= $(lflags) $(QTLIBPATH) $(LIBS) $(libpath) /DEBUG

## target

$(OUTDIR)\$(PROJECT).exe : $(OBJECTS)
	$(link) $(lflags) $(ldebug) $(libs) $(OBJECTS) /SUBSYSTEM:windows /out:"$(OUTDIR)\$(PROJECT).exe"
#	mt.exe -manifest $(OUTDIR)\$(PROJECT).exe.manifest -outputresource:$(OUTDIR)\$(PROJECT).exe

## clean
clean:
	del $(OUTDIR)\*.obj
	del $(OUTDIR)\*.cpp
	del $(OUTDIR)\*.pdb
	del $(OUTDIR)\*.h
	del $(OUTDIR)\*.manifest
	del $(GUIDIR)\*.h
	del $(OUTDIR)\*.exp
	del $(OUTDIR)\*.ilk
	del $(OUTDIR)\$(PROJECT).exe
