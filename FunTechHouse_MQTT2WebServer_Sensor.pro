TEMPLATE = app
QT += xml 
TARGET = 

DEPENDPATH += . src
INCLUDEPATH += . src
LIBS += -lmosquittopp -lmosquitto -lcurl

# Input
SOURCES += main.cpp 

SOURCES += ConfigParse.cpp 
HEADERS += ConfigParse.h 

SOURCES += Sensor.cpp
HEADERS += Sensor.h

SOURCES += MosqConnect.cpp
HEADERS += MosqConnect.h

SOURCES += QuickSurf.cpp
HEADERS += QuickSurf.h

SOURCES += RoomTemperatureDataParser.cpp
HEADERS += RoomTemperatureDataParser.h
