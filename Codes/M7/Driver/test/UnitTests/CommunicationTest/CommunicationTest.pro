QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_communicationtest.cpp \
    ../../../../../eps/SpiSlave/src/SpiHandler.c

HEADERS += \
    ../../../include/TCPServer.h \
    ../../../../../eps/SpiSlave/include/SpiHandler.h
