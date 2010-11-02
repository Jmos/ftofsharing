CONFIG += release plugin
TEMPLATE = lib
HEADERS += gnupgpplugin.h
SOURCES += gnupgpplugin.cpp
TARGET = $$qtLibraryTarget(gnupgp_plugin)
DESTDIR = ../
QT += core
