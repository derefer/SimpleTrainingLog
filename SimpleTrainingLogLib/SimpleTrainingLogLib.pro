TEMPLATE = lib
TARGET = SimpleTrainingLogLib

QT += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
CONFIG -= debug_and_release

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SIMPLETRAININGLOGLIB_LIBRARY

HEADERS += \
    Diagram.h \
    ExerciseTable.h \
    ExportDialog.h \
    NewExerciseDialog.h \
    PlacesDialog.h \
    SettingsDialog.h \
    ShoesDialog.h \
    SimpleTrainingLogLibDecl.h \
    SimpleTrainingLogMainWindow.h \
    SportsDialog.h \
    StatisticsHandler.h \
    WeathersDialog.h \
    DataElements.h

SOURCES += \
    Diagram.cpp \
    ExerciseTable.cpp \
    ExportDialog.cpp \
    NewExerciseDialog.cpp \
    PlacesDialog.cpp \
    SettingsDialog.cpp \
    ShoesDialog.cpp \
    SimpleTrainingLogMainWindow.cpp \
    SportsDialog.cpp \
    StatisticsHandler.cpp \
    WeathersDialog.cpp \
    DataElements.cpp

FORMS +=
