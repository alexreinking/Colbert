TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../app
DESTDIR = ../../../plugins
TARGET = $$qtLibraryTarget(clbmath)
HEADERS += mathplugin.h
SOURCES += mathplugin.cpp

OTHER_FILES += \
    math.json
