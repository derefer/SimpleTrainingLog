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
    ExerciseDataParser_yacc.h \
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
    ExerciseDataParser_lex.cpp \
    ExerciseDataParser_yacc.cpp \
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

# I disabled this, since it doesn't work anymore...
# Manual generation is necessary:
# flex -PExerciseDataParser --nounistd -o ExerciseDataParser_lex.cpp ./ExerciseDataParser.l
# bison -d -o ExerciseDataParser_yacc.cpp -l -p ExerciseDataParser -b ExerciseDataParser ./ExerciseDataParser.y
#QMAKE_LEX = flex
#QMAKE_YACC = bison
#QMAKE_YACCFLAGS = -d -o ExerciseDataParser_yacc.cpp -l
#YACCSOURCES = ExerciseDataParser.y
#LEXSOURCES = ExerciseDataParser.l
