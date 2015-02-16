#-------------------------------------------------
#
# Project created by QtCreator 2014-12-25T17:07:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rename2
TEMPLATE = app
RC_ICONS = icon.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    filemover.cpp \
    filenamegenerator.cpp \
    filesmanager.cpp \
    settings.cpp \
    maskeddirform.cpp

HEADERS  += mainwindow.h \
    filemover.h \
    filenamegenerator.h \
    filesmanager.h \
    settings.h \
    maskeddirform.h

FORMS    += mainwindow.ui \
    maskeddirform.ui

RESOURCES += \
    graphicresources.qrc
