TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../app
DESTDIR = ../../plugins
TARGET = $$qtLibraryTarget(clb_math)
HEADERS += mathplugin.h
SOURCES += mathplugin.cpp
