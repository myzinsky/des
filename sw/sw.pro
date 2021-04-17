include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++2a -stdlib=libc++ -fcoroutines-ts

HEADERS += tst_eventQueue.h \
    descoroutine.h
HEADERS += tst_kernel.h
HEADERS += signal.h
HEADERS += kernel.h
HEADERS += event.h
HEADERS += eventQueue.h

SOURCES += event.cpp
SOURCES += eventQueue.cpp
SOURCES += kernel.cpp
SOURCES += main.cpp
