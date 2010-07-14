# -------------------------------------------------
# Project created by QtCreator 2009-04-24T16:29:06
# -------------------------------------------------
QT -= gui
TARGET = Colbert
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    scanner.cpp \
    parser.cpp \
    codetreenodes.cpp \
    interpretor.cpp
HEADERS += scanner.h \
    token.h \
    scope.h \
    parser.h \
    codetreenodes.h \
    symbols.h \
    interpretor.h
