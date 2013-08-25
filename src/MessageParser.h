#ifndef  __MESSAGEPARSER_H
#define  __MESSAGEPARSER_H

#include <QString>
#include "global.h"

class MessageParser
{
    private:
        QString data;
        DataPointType type;

        bool alarm;
        QString value;
        QString setpoint;
        QString output;


    public:
        MessageParser(QString data, DataPointType type);

        bool parse();
        bool isAlarm();

        DataPointType getType();

        QString getValue();
        QString getSetpoint();
        QString getOutput();

};

#endif  // __MESSAGEPARSER_H 
