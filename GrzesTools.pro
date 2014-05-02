TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    sources/bignum.cpp \
    tests/BigNumTest.cpp


LIBS += -L/usr/lib64/ -lgtest

INCLUDEPATH += /usr/include/gtest
DEPENDPATH += /usr/include/gtest

HEADERS += \
    headers/bignum.h \
    headers/BigNumException.h

QMAKE_CXXFLAGS += -std=c++11 \
                  -pedantic
