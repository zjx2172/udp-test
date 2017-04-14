TEMPLATE = app
TARGET = UDP-Test
DESTDIR = ./output
QT += core multimedia network gui
CONFIG += debug console qt thread
DEFINES += QT_DLL QT_NETWORK_LIB QT_MULTIMEDIA_LIB NIX32 Linux
INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
    ../DCSSoftware-2.4.3/NET/include \
    ../DCSSoftware-2.4.3/STD
LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += ../DCSSoftware-2.4.3/NET/libNET.a
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += ./intermediate
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += \
    udp-test.h \
    sender.h
SOURCES += main.cpp \
    udp-test.cpp \
    sender.cpp
#RESOURCES += udp-test.qrc
target.path = /home/olog
INSTALLS += target
