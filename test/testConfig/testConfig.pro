CONFIG += qtestlib debug
TEMPLATE = app
QT += xml 
TARGET = 
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestConfig.cpp

# Code to test
DEPENDPATH  += ../../src/
INCLUDEPATH += ../../src/
SOURCES += ConfigParse.cpp
SOURCES += DataPoint.cpp
