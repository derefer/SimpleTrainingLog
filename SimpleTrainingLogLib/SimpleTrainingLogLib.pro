TEMPLATE = lib
TARGET = SimpleTrainingLogLib

QT += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
CONFIG -= debug_and_release

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SIMPLETRAININGLOGLIB_LIBRARY

HEADERS += \
    DataHandler.h \
    Diagram.h \
    Exercise.h \
    ExerciseTable.h \
    ExportDialog.h \
    NewExerciseDialog.h \
    Place.h \
    PlacesDialog.h \
    SettingsDialog.h \
    Shoe.h \
    ShoesDialog.h \
    SimpleTrainingLogLibDecl.h \
    SimpleTrainingLogMainWindow.h \
    Sport.h \
    SportsDialog.h \
    StatisticsHandler.h \
    Weather.h \
    WeathersDialog.h

SOURCES += \
    DataHandler.cpp \
    Diagram.cpp \
    Exercise.cpp \
    ExerciseTable.cpp \
    ExportDialog.cpp \
    NewExerciseDialog.cpp \
    PlacesDialog.cpp \
    SettingsDialog.cpp \
    ShoesDialog.cpp \
    SimpleTrainingLogMainWindow.cpp \
    SportsDialog.cpp \
    StatisticsHandler.cpp \
    WeathersDialog.cpp

FORMS += \
    ExportDialog.ui \
    NewExerciseDialog.ui \
    PlacesDialog.ui \
    SettingsDialog.ui \
    ShoesDialog.ui \
    SimpleTrainingLogMainWindow.ui \
    SportsDialog.ui \
    WeathersDialog.ui

RESOURCES += \
    SimpleTrainingLog.qrc
