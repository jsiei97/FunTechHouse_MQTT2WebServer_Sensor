/**
 * @file MessageParser.cpp
 * @author Johan Simonsson
 * @brief Parse message data from MQTT server
 */

/*
 * Copyright (C) 2013 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QString>
#include <QRegExp>
#include <QDebug>

#include "MessageParser.h"
#include "global.h"

MessageParser::MessageParser(QString data, DataPointType type)
{
    this->type = type;
    this->data = data;

    alarm = false;
    value = "";
}

bool MessageParser::parse()
{
    if(data.size() == 0)
    {
        return false;
    }

    bool hit = false;


    QRegExp rxData;
    switch ( this->type )
    {
        case DATAPOINT_SENSOR:
            rxData.setPattern("temperature=([-0-9.]+)");
            break;
        case DATAPOINT_REGULATOR:
            rxData.setPattern("value=([-0-9.]+) ; setpoint=([-0-9.]+) ; output=([0-9]+)%");
            break;
        case DATAPOINT_METER:
            rxData.setPattern("energy=([-0-9.]+) kWh");
            break;
        default :
            return false;
            break;
    }

    QRegExp rxAlarm("Alarm: (.*)");

    if (rxAlarm.indexIn(data) != -1)
    {
        alarm = true;
        //qDebug() << "Got a alarm: " << rxAlarm.cap(1);
        value = rxAlarm.cap(1);
        hit = true;
    }
    else
    {
        if (rxData.indexIn(data) != -1)
        {
            hit = true;
            alarm = false;
            //qDebug() << "Data=" << rxData.cap(1);
            value =  rxData.cap(1);
            if(3 == rxData.captureCount())
            {
                setpoint =  rxData.cap(2);
                output   =  rxData.cap(3);
            }
        }
    }

    return hit;
}

bool MessageParser::isAlarm()
{
    return alarm;
}

DataPointType MessageParser::getType()
{
    return this->type;
}
QString MessageParser::getValue()
{
    return value;
}

QString MessageParser::getSetpoint()
{
    return setpoint;
}
QString MessageParser::getOutput()
{
    return output;
}
