TEMPLATE = lib
CONFIG += plugin
QT -= gui
INCLUDEPATH += ../../app
DESTDIR = ../../../plugins
TARGET = $$qtLibraryTarget(clbstr)
HEADERS += strplugin.h
SOURCES += strplugin.cpp
