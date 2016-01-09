#-------------------------------------------------
#
# Project created by QtCreator 2012-04-13T02:32:33
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = interface_qt
TEMPLATE = app

unix:INCLUDEPATH += "../processor/modeler"
unix:LIBS += -L../processor -lbuilder
SOURCES += main.cpp\
        mainwindow.cpp \
    glviewer.cpp \
    syntaxhighlighter.cpp

HEADERS  += mainwindow.h \
    glviewer.h \
    syntaxhighlighter.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

LIBS += -lglut
