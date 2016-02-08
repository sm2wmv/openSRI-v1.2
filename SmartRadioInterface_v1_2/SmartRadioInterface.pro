#-------------------------------------------------
#
# Project created by QtCreator 2013-12-06T18:11:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartRadioInterface
TEMPLATE = app \
           debug

SOURCES += main.cpp\
        mainwindow.cpp \
        commclass.cpp \
        dialogselectport.cpp \
        settings.cpp \
    misc.cpp \
    status.cpp \
    displayclass.cpp

HEADERS  += mainwindow.h \
            version.h \
            commclass.h \
            dialogselectport.h \
            commands.h \
            settings.h \
    misc.h \
    status.h \
    displayclass.h

FORMS    += mainwindow.ui \
            dialogselectport.ui
