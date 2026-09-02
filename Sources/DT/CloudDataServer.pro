QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES *= QT_USE_QSTRINGBUILDER

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CloudDataPacket.cpp \
    CloudServices.cpp \
    CloudTalker.cpp \
    CloudTaskHandler.cpp \
    CmdParas.cpp \
    ConnectStatus.cpp \
    DataPacketData.cpp \
    InfoData.cpp \
    PLCRequest.cpp \
    PLCResponse.cpp \
    PLCTaskHandler.cpp \
    PLCTaskHandler2.cpp \
    PackingTools.cpp \
    PolData.cpp \
    SBData.cpp \
    SqliteTools.cpp \
    SqliteTools2.cpp \
    SubWorker/SubWorker2011.cpp \
    SubWorker/SubWorker2021.cpp \
    SubWorkerBase.cpp \
    Task.cpp \
    Worker.cpp \
    main.cpp \
    mainwindow.cpp \
    plc.cpp \
    uart.c

HEADERS += \
    CloudDataPacket.h \
    CloudServices.h \
    CloudTalker.h \
    CloudTaskHandler.h \
    CmdParas.h \
    ConnectStatus.h \
    DataAlign.h \
    DataPacketData.h \
    InfoData.h \
    PLCRequest.h \
    PLCResponse.h \
    PLCTaskHandler.h \
    PLCTaskHandler2.h \
    PackingTools.h \
    PolData.h \
    SBData.h \
    SqliteTools.h \
    SqliteTools2.h \
    SubWorker/SubWorker2011.h \
    SubWorker/SubWorker2021.h \
    SubWorkerBase.h \
    Task.h \
    Worker.h \
    mainwindow.h \
    plc.h \
    uart.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CloudDataServer.pro.user
