TEMPLATE = app
QT += xml 
TARGET = 

DEPENDPATH += . src
INCLUDEPATH += . src
LIBS += -lmosquittopp -lmosquitto -lcurl

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += main.cpp 

SOURCES += ConfigParse.cpp 
HEADERS += ConfigParse.h 

SOURCES += DataPoint.cpp
HEADERS += DataPoint.h

SOURCES += MosqConnect.cpp
HEADERS += MosqConnect.h

SOURCES += QuickSurf.cpp
HEADERS += QuickSurf.h

SOURCES += MessageParser.cpp
HEADERS += MessageParser.h
