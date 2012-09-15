#ifndef  __ROOMTEMPERATUREDATAPARSER_H
#define  __ROOMTEMPERATUREDATAPARSER_H

#include <QString>

class RoomTemperatureDataParser
{
    private:
        QString data;
        bool alarm;
        QString value;

    public:
        RoomTemperatureDataParser(QString data);

        bool parse();
        bool isAlarm();
        QString getValue();

};

#endif  // __ROOMTEMPERATUREDATAPARSER_H
