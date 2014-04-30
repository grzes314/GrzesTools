TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    bignum.cpp \
    tests/BigNumTest.cpp


LIBS += -L/usr/lib64/ -lgtest

INCLUDEPATH += /usr/include/gtest
DEPENDPATH += /usr/include/gtest

HEADERS += \
    bignum.h
