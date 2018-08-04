#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T19:01:59
#
#-------------------------------------------------

QT       += core gui sql

include (QtXlsx\src\xlsx\qtxlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Produkto_Gamyba
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    productproduction.cpp \
    exceldata.cpp \
    eipfilehandler.cpp \
    fileselect.cpp \
    fileselection.cpp \
    progressbar.cpp \
    productfields.cpp \
    currentproductdata.cpp \
    filedata.cpp

HEADERS += \
        mainwindow.h \
    productproduction.h \
    exceldata.h \
    eipfilehandler.h \
    fileselect.h \
    fileselection.h \
    progressbar.h \
    productfields.h \
    currentproductdata.h \
    filedata.h

FORMS += \
        mainwindow.ui \
    fileselection.ui \
    progressbar.ui
