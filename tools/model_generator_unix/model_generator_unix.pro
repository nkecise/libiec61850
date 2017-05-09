RESOURCES += \
    GenConfig.qrc

OTHER_FILES += \
    GenConfig.rc

HEADERS += \
    CentralWidget.h \
    GenConfig.h \
    IedTreeWidget.h \
    MainFrame.h \
    parser/AttributeType.h \
    parser/Error.h \
    parser/platform.h \
    parser/SclParser.h \
    parser/pugixml/pugiconfig.hpp \
    parser/pugixml/pugixml.hpp

SOURCES += \
    61850Handler.cpp \
    CentralWidget.cpp \
    GenConfig.cpp \
    IedTreeWidget.cpp \
    MainFrame.cpp \
    parser/AttributeType.cpp \
    parser/Error.cpp \
    parser/SclParser.cpp \
    parser/SclValidate.cpp \
    parser/Utils.cpp \
    parser/pugixml/pugixml.cpp

IEC61850ROOT = /home/kuixiaon/SWAP/libiec61850.git.nk/trunk
INCLUDEPATH += ./parser
INCLUDEPATH += ./parser/pugixml
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/config
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/common/inc
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/goose
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/hal/inc
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/iec61850/inc
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/iec61850/inc_private
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/mms/inc
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/mms/inc_private
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/mms/iso_mms/asn1c
INCLUDEPATH += /home/kuixiaon/SWAP/libiec61850.git.nk/trunk/src/logging

LIBS += ../../build/libiec61850.a

