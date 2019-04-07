QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_t1.cpp \
    ../../src/Calculator.c \


HEADERS += ../../include/Calculator.h

