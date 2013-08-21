#include <QString>
#include <QRegExp>
#include <QDebug>

#include "RoomTemperatureDataParser.h"

RoomTemperatureDataParser::RoomTemperatureDataParser(QString data)
{
    this->data = data;

    alarm = false;
    value = "";
}

bool RoomTemperatureDataParser::parse()
{
    if(data.size() == 0)
    {
        return false;
    }

    bool hit = false;
    QRegExp rxData("temperature=([-0-9.]+)");
    QRegExp rxAlarm("Alarm: (.*)");

    if (rxAlarm.indexIn(data) != -1)
    {
        alarm = true;
        //qDebug() << "Got a alarm: " << rxAlarm.cap(1);
        value = rxAlarm.cap(1);
        hit = true;
    }
    else if (rxData.indexIn(data) != -1)
    {
        alarm = false;
        //qDebug() << "Data=" << rxData.cap(1);
        value =  rxData.cap(1);
        hit = true;
    }

    return hit;
}

bool RoomTemperatureDataParser::isAlarm()
{
    return alarm;
}

QString RoomTemperatureDataParser::getValue()
{
    return value;
}
