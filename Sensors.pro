#-------------------------------------------------
#
# Project created by QtCreator 2014-03-20T09:53:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sensors
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AddDialog.cpp \
    EditDialog.cpp \
    Sensor.cpp \
    SettingDialog.cpp

HEADERS  += MainWindow.h \
    AddDialog.h \
    EditDialog.h \
    Sensor.h \
    SettingDialog.h

FORMS    += MainWindow.ui \
    AddDialog.ui \
    EditDialog.ui \
    SettingDialog.ui
