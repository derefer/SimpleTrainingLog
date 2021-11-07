TEMPLATE = app
TARGET = KTrainer

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

HEADERS += \
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
    main.cpp \
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

RESOURCES = application.qrc

DISTFILES += TODO

# Disable this, since it doesn't work...
# Manual generation is necessary:
# flex -Pexercisedataparser --nounistd -o exercisedataparser_lex.cpp ./exercisedataparser.l
# bison -d -o exercisedataparser_yacc.cpp -l -p exercisedataparser -b exercisedataparser ./exercisedataparser.y
#QMAKE_LEX = flex
#QMAKE_YACC = bison
#QMAKE_YACCFLAGS = -d -o exercisedataparser_yacc.cpp -l
#YACCSOURCES = exercisedataparser.y
#LEXSOURCES = exercisedataparser.l
