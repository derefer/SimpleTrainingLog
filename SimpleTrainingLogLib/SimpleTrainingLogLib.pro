TEMPLATE = lib
TARGET = SimpleTrainingLogLib

QT += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
CONFIG -= debug_and_release

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SIMPLETRAININGLOGLIB_LIBRARY

HEADERS += \
    SimpleTrainingLogLibDecl.h \
    ktrainer.h \
    newexercisedialog.h \
    exercisetable.h \
    dataelements.h \
    exportdialog.h \
    statisticshandler.h \
    settingsdialog.h \
    placesdialog.h \
    shoesdialog.h \
    sportsdialog.h \
    weathersdialog.h \
    diagram.h \
    exercisedataparser_yacc.h

SOURCES += \
    ktrainer.cpp \
    newexercisedialog.cpp \
    exercisetable.cpp \
    exportdialog.cpp \
    statisticshandler.cpp \
    dataelements.cpp \
    settingsdialog.cpp \
    placesdialog.cpp \
    shoesdialog.cpp \
    sportsdialog.cpp \
    weathersdialog.cpp \
    diagram.cpp \
    exercisedataparser_lex.cpp \
    exercisedataparser_yacc.cpp

FORMS +=

# I disabled this, since it doesn't work anymore...
# Manual generation is necessary:
# flex -Pexercisedataparser --nounistd -o exercisedataparser_lex.cpp ./exercisedataparser.l
# bison -d -o exercisedataparser_yacc.cpp -l -p exercisedataparser -b exercisedataparser ./exercisedataparser.y
#QMAKE_LEX = flex
#QMAKE_YACC = bison
#QMAKE_YACCFLAGS = -d -o exercisedataparser_yacc.cpp -l
#YACCSOURCES = exercisedataparser.y
#LEXSOURCES = exercisedataparser.l
