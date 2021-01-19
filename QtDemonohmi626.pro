#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T15:03:07
#
#-------------------------------------------------
QT       += sql
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtDemo
TEMPLATE = app
DEFINES += QT_NO_STATEMACHINE

SOURCES += main.cpp \
    Bsp/Bsp_Time.cpp \
    Bsp/Bsp_Uart1.cpp \
    Pub/Pub_ComFpi.cpp \
    Pub/Pub_MyBus.cpp \
    Pub/Pub_Queue.cpp \
    Pub/Pub_Tmr.cpp \
    Pub/Pub_Alarm.cpp \
    App/App_ComFpi.cpp \
    App/App_Init.cpp\
    globalvar.cpp\
    sqlite.cpp \
    Bsp/Bsp_Fpga.cpp \
    Bsp/Bsp_Gpioctl.cpp \
    Bsp/Bsp_Can0.cpp \
    Bsp/Bsp_TCP.cpp \
    Pub/Pub_Qthread.cpp\
    Mod/mod_ioctrl.cpp \
    Mod/Mod_Method.cpp \
    Pro/pro_fpga.cpp \
    Mod/mod_mnt.cpp \
    Mod/mod_config.cpp \
    Mod/mod_detectdata.cpp \
    Pub/pub_caltemp.cpp \
    App/app_app.cpp \
    Mod/mod_circu.cpp \
    Pub/pub_MKSVacuometer_V1_0.cpp \
    Pub/Pub_CtlModJudge.cpp \
    Mod/mod_fpgaupdate.cpp \
    Mod/mod_EI.cpp \
    Mod/mod_Q0Driver.cpp \
    Bsp/Bsp_SPI1.cpp \
    Pro/pro_led.cpp



HEADERS  += Bsp/Bsp_Time.h \
    Bsp/Bsp_Uart1.h \
    Pub/Pub_ComFpi.h \
    Pub/Pub_MyBus.h \
    Pub/Pub_Queue.h \
    Pub/Pub_Tmr.h \
    Pub/Pub_Alarm.h \
    App/App_ComFpi.h \
    App/App_Init.h\
    typedefine.h\
    globalvar.h\
    sqlite.h \
    Bsp/Bsp_Fpga.h \
    Include.h \
    Bsp/Bsp_Include.h \
    Bsp/Bsp_Gpioctl.h \
    Bsp/Bsp_Can0.h \
    Bsp/Bsp_TCP.h \
    Pub/Pub_Qthread.h\
    Mod/mod_ioctrl.h \
    Mod/Mod_Method.h \
    Pro/pro_fpga.h \
    Mod/mod_mnt.h \
    Mod/mod_config.h \
    Mod/mod_vacuumgauge.h \
    Mod/mod_detectdata.h \
    Pub/pub_caltemp.h \
    App/app_app.h \
    Mod/mod_circu.h \
    Pub/pub_MKSVacuometer_V1_0.h \
    Pub/Pub_CtlModJudge.h \
    Mod/mod_fpgaupdate.h \
    Mod/mod_EI.h \
    Mod/mod_Q0Driver.h \
    Bsp/Bsp_SPI1.h \
    Pro/pro_led.h

