TEMPLATE = app
TARGET = SimpleTrainingLogTests

QT += core gui testlib # Include QtTest to use SignalSpy(), QTest::mouseClick(), etc.
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14
CONFIG += testcase # Creates check target in Makefile.
CONFIG -= debug_and_release
CONFIG += console

INCLUDEPATH += ../SimpleTrainingLogLib

LIBS += -L../SimpleTrainingLogLib -lSimpleTrainingLogLib -lgtest -lgmock

HEADERS += \
    QtTypePrinters.h

SOURCES += \
    main.cpp \
    SimpleTrainingLogTests.cpp
