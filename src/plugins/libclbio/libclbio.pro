TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../app
DESTDIR = ../../../plugins
TARGET = $$qtLibraryTarget(clbio)
HEADERS += ioplugin.h
SOURCES += ioplugin.cpp

OTHER_FILES += \
    io.json
