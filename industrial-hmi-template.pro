QT       += core gui sql network  #qml
#xlsx
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#disable-desktop|android-embedded|!isEmpty(CROSS_COMPILE)|qnx {
#    DEFINES += MAIN_QML=\\\"basic-b2qt.qml\\\"
#} else {
#    DEFINES += MAIN_QML=\\\"Basic.qml\\\"
#}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/BLL/BBase.cpp \
    Sources/BLL/BCalcSetup.cpp \
    Sources/BLL/BComArgs.cpp \
    Sources/BLL/BFactor.cpp \
    Sources/BLL/BHistoryData.cpp \
    Sources/BLL/BLocalService.cpp \
    Sources/BLL/BStatisticalData.cpp \
    Sources/BLL/BSysConfig.cpp \
    Sources/BLL/BSysDictionary.cpp \
    Sources/BLL/BUnitSetup.cpp \
    Sources/BLL/BUser.cpp \
    Sources/DAL/dal.cpp \
    Sources/DT/RelayControl.cpp \
    Sources/DT/SamplerState.cpp \
    Sources/DT/SelfDetection.cpp \
    Sources/DT/SubWorker/SubWorker1063.cpp \
    Sources/DT/SubWorker/SubWorker1064.cpp \
    Sources/DT/SubWorker/SubWorker1072.cpp \
    Sources/DT/SubWorker/SubWorker2012.cpp \
    Sources/DT/SubWorker/SubWorker2021.cpp \
    Sources/DT/SubWorker/SubWorker2031.cpp \
    Sources/DT/SubWorker/SubWorker2051.cpp \
    Sources/DT/SubWorker/SubWorker2062.cpp \
    Sources/DT/SubWorker/SubWorker2081.cpp \
    Sources/DT/SubWorker/SubWorker3011.cpp \
    Sources/DT/SubWorker/SubWorker3012.cpp \
    Sources/DT/SubWorker/SubWorker3013.cpp \
    Sources/DT/SubWorker/SubWorker3014.cpp \
    Sources/DT/SubWorker/SubWorker3015.cpp \
    Sources/DT/SubWorker/SubWorker3016.cpp \
    Sources/DT/SubWorker/SubWorker3017.cpp \
    Sources/DT/SubWorker/SubWorker3018.cpp \
    Sources/DT/SubWorker/SubWorker3019.cpp \
    Sources/DT/SubWorker/SubWorker3020.cpp \
    Sources/DT/SubWorker/SubWorker3021.cpp \
    Sources/DT/SubWorker/SubWorker3044.cpp \
    Sources/DT/SubWorker/SubWorker3080.cpp \
    Sources/DT/SubWorker/SubWorker3085.cpp \
    Sources/DT/SubWorker/SubWorker3086.cpp \
    Sources/Tools/CScreenSaver.cpp \
    Sources/Tools/JsonParse.cpp \
    Sources/Tools/datatype.cpp \
    Sources/Tools/feeddog.c \
    Sources/Tools/key.c \
    Sources/Tools/keyBoard.cpp \
    Sources/Tools/net.c \
    Sources/Tools/ping.cpp \
    Sources/Tools/set_sys_time.c \
    Sources/Tools/sysutil.cpp \
    Sources/DT/CloudDataPacket.cpp \
    Sources/DT/CloudServices.cpp \
    Sources/DT/CloudTalker.cpp \
    Sources/DT/CloudTaskHandler.cpp \
    Sources/DT/CmdParas.cpp \
    Sources/DT/ConnectStatus.cpp \
    Sources/DT/DataPacketData.cpp \
    Sources/DT/InfoData.cpp \
    Sources/DT/PLCRequest.cpp \
    Sources/DT/PLCResponse.cpp \
    Sources/DT/PLCTaskHandler2.cpp \
    Sources/DT/PackingTools.cpp \
    Sources/DT/PolData.cpp \
    Sources/DT/SBData.cpp \
    Sources/DT/SqliteTools.cpp \
    Sources/DT/SubWorker/SubWorker1000.cpp \
    Sources/DT/SubWorker/SubWorker1011.cpp \
    Sources/DT/SubWorker/SubWorker1012.cpp \
    Sources/DT/SubWorker/SubWorker1013.cpp \
    Sources/DT/SubWorker/SubWorker1061.cpp \
    Sources/DT/SubWorker/SubWorker1062.cpp \
    Sources/DT/SubWorker/SubWorker2011.cpp \
    Sources/DT/SubWorker/SubWorker2061.cpp \
    Sources/DT/SubWorkerBase.cpp \
    Sources/DT/Task.cpp \
    Sources/DT/Worker.cpp \
    Sources/DT/plc.cpp \
    Sources/DT/uart.c \
    dialogdiagram.cpp \
    dialogsetupai.cpp \
    dialogsetupaicheck.cpp \
    dialogsetupcom.cpp \
    dialogsetupcommfactor.cpp \
    dialogsetupdataup.cpp \
    dialogsetupfacsetting.cpp \
    dialogsetupinternet.cpp \
    dialogsetupnet.cpp \
    dialogsetuppwd.cpp \
    dialogsetuprelay.cpp \
    dialogsetuprs232detection.cpp \
    dialogsetuprs485detection.cpp \
    dialogsetupscreensavetime.cpp \
    dialogsetupserver.cpp \
    dialogsetupsite.cpp \
    dialogsetupswitch.cpp \
    dialogsetuptimer.cpp \
##    dialogsetupwatchdog.cpp \
    dialogsetupwatchdog.cpp \
    dialogsetupwqs.cpp \
    formlogin.cpp \
    framediagram.cpp \
    framehistorydata.cpp \
    framerealtimedata.cpp \
    framerebootcover.cpp \
    framesetup.cpp \
    framesetupcomm.cpp \
    #framesetupcontrol.cpp \
    framesetupother.cpp \
    framesetuptimer.cpp \
    framesysinfodata.cpp \
    framethermo.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Headers/BLL/BBase.h \
    Headers/BLL/BCalcSetup.h \
    Headers/BLL/BComArgs.h \
    Headers/BLL/BFactor.h \
    Headers/BLL/BHistoryData.h \
    Headers/BLL/BLocalService.h \
    Headers/BLL/BStatisticalData.h \
    Headers/BLL/BSysConfig.h \
    Headers/BLL/BSysDictionary.h \
    Headers/BLL/BUnitSetup.h \
    Headers/BLL/BUser.h \
    Headers/DAL/dal.h \
    Headers/Model/MBase.h \
    Headers/Model/MCalcSetup.h \
    Headers/Model/MComArgs.h \
    Headers/Model/MConfig.h \
    Headers/Model/MFactor.h \
    Headers/Model/MStatisticalData.h \
    Headers/Model/MUnitSetup.h \
    Headers/Model/MUser.h \
    Headers/Tools/CScreenSaver.h \
    Headers/Tools/JsonParse.h \
    Headers/Tools/keyBoard.h \
    Sources/DT/RelayControl.h \
    Sources/DT/SelfDetection.h \
    Sources/DT/SubWorker/SubWorker2012.h \
    Sources/DT/SubWorker/SubWorker2021.h \
    Sources/DT/SubWorker/SubWorker2062.h \
    Sources/DT/SubWorker/SubWorker2081.h \
    Sources/DT/SubWorker/SubWorker3016.h \
    Sources/DT/SubWorker/SubWorker3017.h \
    Sources/DT/SubWorker/SubWorker3018.h \
    Sources/DT/SubWorker/SubWorker3019.h \
    Sources/DT/SubWorker/SubWorker3020.h \
    Sources/DT/SubWorker/SubWorker3021.h \
    Sources/DT/SubWorker/SubWorker3044.h \
    Sources/DT/SubWorker/SubWorker3080.h \
    Sources/DT/SubWorker/SubWorker3085.h \
    Sources/DT/SubWorker/SubWorker3086.h \
    Sources/Tools/datatype.h \
    Sources/Tools/feeddog.h \
    Sources/Tools/key.h \
    Sources/Tools/net.h \
    Sources/Tools/ping.h \
    Sources/Tools/set_sys_time.h \
    Sources/Tools/sysutil.h \
    Sources/DT/PLCAction.h \
    Sources/DT/SamplerState.h \
    Sources/DT/SubWorker/SubWorker2031.h \
    Sources/DT/SubWorker/SubWorker2051.h \
    Sources/DT/SubWorker/SubWorker3011.h \
    Sources/DT/SubWorker/SubWorker3012.h \
    Sources/DT/SubWorker/SubWorker3013.h \
    Sources/DT/SubWorker/SubWorker3014.h \
    Sources/DT/SubWorker/SubWorker3015.h \
    Sources/DT/CloudDataPacket.h \
    Sources/DT/CloudServices.h \
    Sources/DT/CloudTalker.h \
    Sources/DT/CloudTaskHandler.h \
    Sources/DT/CmdParas.h \
    Sources/DT/ConnectStatus.h \
    Sources/DT/Task.h \
    Sources/DT/Worker.h \
    Sources/DT/DataAlign.h \
    Sources/DT/DataPacketData.h \
    Sources/DT/InfoData.h \
    Sources/DT/PLCRequest.h \
    Sources/DT/PLCResponse.h \
    Sources/DT/PLCTaskHandler2.h \
    Sources/DT/PackingTools.h \
    Sources/DT/PolData.h \
    Sources/DT/SBData.h \
    Sources/DT/SqliteTools.h \
    Sources/DT/SubWorkerBase.h \
    Sources/DT/SubWorker/SubWorker1000.h \
    Sources/DT/SubWorker/SubWorker1011.h \
    Sources/DT/SubWorker/SubWorker1012.h \
    Sources/DT/SubWorker/SubWorker1013.h \
    Sources/DT/SubWorker/SubWorker1061.h \
    Sources/DT/SubWorker/SubWorker1062.h \
    Sources/DT/SubWorker/SubWorker2011.h \
    Sources/DT/SubWorker/SubWorker2061.h \
    Sources/DT/SubWorker/SubWorker1063.h \
    Sources/DT/SubWorker/SubWorker1064.h \
    Sources/DT/SubWorker/SubWorker1072.h \
    Sources/DT/plc.h \
    Sources/DT/uart.h \
    dialogdiagram.h \
    dialogsetupai.h \
    dialogsetupaicheck.h \
    dialogsetupcom.h \
    dialogsetupcommfactor.h \
    dialogsetupdataup.h \
    dialogsetupfacsetting.h \
    dialogsetupinternet.h \
    dialogsetupnet.h \
    dialogsetuppwd.h \
    dialogsetuprelay.h \
    dialogsetuprs232detection.h \
    dialogsetuprs485detection.h \
    dialogsetupscreensavetime.h \
    dialogsetupserver.h \
    dialogsetupsite.h \
    dialogsetupswitch.h \
    dialogsetuptimer.h \
##    dialogsetupwatchdog.h \
    dialogsetupwatchdog.h \
    dialogsetupwqs.h \
    formlogin.h \
    framediagram.h \
    framehistorydata.h \
    framerealtimedata.h \
    framerebootcover.h \
    framesetup.h \
    framesetupcomm.h \
    #framesetupcontrol.h \
    framesetupother.h \
    framesetuptimer.h \
    framesysinfodata.h \
    framethermo.h \
    mainwindow.h

FORMS += \
    dialogdiagram.ui \
    dialogsetupai.ui \
    dialogsetupaicheck.ui \
    dialogsetupcom.ui \
    dialogsetupcommfactor.ui \
    dialogsetupdataup.ui \
    dialogsetupfacsetting.ui \
    dialogsetupinternet.ui \
    dialogsetupnet.ui \
    dialogsetuppwd.ui \
    dialogsetuprelay.ui \
    dialogsetuprs232detection.ui \
    dialogsetuprs485detection.ui \
    dialogsetupscreensavetime.ui \
    dialogsetupserver.ui \
    dialogsetupsite.ui \
    dialogsetupswitch.ui \
    dialogsetuptimer.ui \
    dialogsetupwatchdog.ui \
    dialogsetupwqs.ui \
    formlogin.ui \
    framediagram.ui \
    framehistorydata.ui \
    framerealtimedata.ui \
    framerebootcover.ui \
    framesetup.ui \
    framesetupcomm.ui \
    #framesetupcontrol.ui \
    framesetupother.ui \
    framesetuptimer.ui \
    framesysinfodata.ui \
    framethermo.ui \
    mainwindow.ui

TRANSLATIONS += \
    industrial-hmi-template_zh_CN.ts

UI_DIR=./UI


#INCLUDEPATH += /opt/qwt-6.2.0/src/
#INCLUDEPATH += /opt/qwt-6.2.0/classincludes/
#LIBS += -L/opt/qwt-6.2.0/lib -lqwt

INCLUDEPATH += /usr/include/qwt
LIBS += -lqwt-qt5

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images.qrc
