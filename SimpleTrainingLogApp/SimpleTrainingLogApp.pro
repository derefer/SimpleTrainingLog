TEMPLATE = app
TARGET = SimpleTrainingLogApp

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

DEPENDPATH += ../SimpleTrainingLogLib

INCLUDEPATH += ../SimpleTrainingLogLib

LIBS += -L../SimpleTrainingLogLib -lSimpleTrainingLogLib

HEADERS +=

SOURCES += \
    main.cpp

RESOURCES = SimpleTrainingLog.qrc
