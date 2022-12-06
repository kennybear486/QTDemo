#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T21:38:33
#
#-------------------------------------------------

QT       += core gui network
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myTcpTools
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    mytcpserver.cpp \
    mytcpclient.cpp

HEADERS  += frmmain.h \
    mytcpserver.h \
    mytcpclient.h \
    myhelper.h

FORMS    += frmmain.ui

DESTDIR=bin
MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj

win32:RC_FILE=main.rc

RESOURCES += \
    main.qrc
