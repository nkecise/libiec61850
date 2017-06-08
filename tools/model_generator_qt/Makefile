#
# to build the GenConfig utility
#
## common defintions ##
PROJECT	= IEC61850Server 
COMNPATH = 
GUIDIR	= .\UI
QTROOT	= D:\Qt\4.8.6
IEC61850ROOT = ..\..
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
		  $(OUTDIR)\AttributeType.obj $(OUTDIR)\SclParser.obj $(OUTDIR)\Error.obj \
		  $(OUTDIR)\Utils.obj $(OUTDIR)\pugixml.obj \
		  $(OUTDIR)\IedTreeWidget.obj \
		  $(OUTDIR)\moc_IedTreeWidget.obj \
		  $(OUTDIR)\61850Handler.obj \
		  $(OUTDIR)\CentralWidget.obj \
		  $(OUTDIR)\moc_CentralWidget.obj \
		  $(OUTDIR)\SclValidate.obj \
		  $(OUTDIR)\SGCBHandler.obj \
		  $(OUTDIR)\FileSvcHandler.obj

DEPENDS	= $(GUIDIR)\uic_centralListWidget.h

## target ##
all: $(OUTDIR) $(DEPENDS) $(OUTDIR)\$(PROJECT).exe

## cflags ##
ccommon		= -c
!IFDEF NODEBUG
cdebug		= -Od
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
IEC61850INCPATH = -I"$(IEC61850ROOT)/config" \
		-I"$(IEC61850ROOT)/src/common/inc" \
		-I"$(IEC61850ROOT)/src/goose" \
		-I"$(IEC61850ROOT)/src/hal/inc" \
		-I"$(IEC61850ROOT)/src/iec61850/inc" \
		-I"$(IEC61850ROOT)/src/iec61850/inc_private" \
		-I"$(IEC61850ROOT)/src/mms/inc" \
		-I"$(IEC61850ROOT)/src/mms/inc_private" \
		-I"$(IEC61850ROOT)/src/mms/iso_mms/asn1c" \
		-I"$(IEC61850ROOT)/src/logging" \
		-I"$(IEC61850ROOT)/src/vs"

incpath		= $(QTINCPATH) $(IEC61850INCPATH)
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
$(GUIDIR)\uic_centralListWidget.h : $(GUIDIR)\centralListWidget.ui
	$(QTROOT)\bin\uic -o $(GUIDIR)\uic_centralListWidget.h $(GUIDIR)\centralListWidget.ui

## moc ##
$(OUTDIR)\moc_GenConfig.cpp : GenConfig.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_GenConfig.cpp GenConfig.h

$(OUTDIR)\moc_MainFrame.cpp : MainFrame.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_MainFrame.cpp MainFrame.h

$(OUTDIR)\moc_IedTreeWidget.cpp : IedTreeWidget.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_IedTreeWidget.cpp IedTreeWidget.h

$(OUTDIR)\moc_CentralWidget.cpp : CentralWidget.h
	$(QTROOT)\bin\moc $(QTDEFINES) $(incpath) -DWIN32 -o $(OUTDIR)\moc_CentralWidget.cpp CentralWidget.h

## resource ##
$(OUTDIR)\qrc_GenConfig.cpp : .\GenConfig.qrc
	$(QTROOT)\bin\rcc -name GenConfig -o $(OUTDIR)\qrc_GenConfig.cpp .\GenConfig.qrc

$(OUTDIR)\GenConfig.res : .\GenConfig.rc
	rc.exe -fo $(OUTDIR)\GenConfig.res .\GenConfig.rc

## link ##
QTLIBPATH	= /LIBPATH:"$(QTROOT)\lib" /LIBPATH:"$(IEC61850ROOT)\lib"
!IFDEF	NODEBUG
QTLIBS	= QtGui.lib QtCore.lib QtMain.lib iec61850.lib
!ELSE
QTLIBS	= QtGuid.lib QtCored.lib QtMaind.lib iec61850d.lib
!ENDIF

WIN32LIBS	= kernel32.lib user32.lib gdi32.lib ws2_32.lib imm32.lib \
			  winmm.lib comdlg32.lib shell32.lib advapi32.lib \
			  winspool.lib shlwapi.lib

LIBS	= $(WIN32LIBS) $(QTLIBS)
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
