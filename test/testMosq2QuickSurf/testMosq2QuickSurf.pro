CONFIG += qtestlib debug
TEMPLATE = app
TARGET = 
DEFINES += private=public

# Test code
DEPENDPATH += . stub
INCLUDEPATH += . stub
SOURCES += TestMosq2QuickSurf.cpp
SOURCES += QuickSurf.cpp


# Code to test
DEPENDPATH  += ../../src/
INCLUDEPATH += ../../src/
SOURCES += Mosq2QuickSurf.cpp
SOURCES += MessageParser.cpp
SOURCES += DataPoint.cpp


