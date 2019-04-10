QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_canpassdata.cpp \
    ../../../src/SpiHandler.c

HEADERS += \
    ../../../include/SpiHandler.h
