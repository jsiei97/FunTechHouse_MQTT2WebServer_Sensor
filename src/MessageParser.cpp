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
#include <QStringList>
#include <QRegExp>
#include <QDebug>

#include "MessageParser.h"
#include "global.h"

MessageParser::MessageParser(QString data, DataPointType type)
{
    this->type = type;
    this->data = data;

    alarm = false;
}

bool MessageParser::parse()
{
    if(data.size() == 0)
    {
        return false;
    }

    {
        QRegExp rxAlarm("Alarm: (.*)");
        if (rxAlarm.indexIn(data) != -1)
        {
            alarm = true;
            //qDebug() << "Got a alarm: " << rxAlarm.cap(1);
            value = rxAlarm.cap(1);
            if(!value.isEmpty())
            {
                return true;
            }
        }
    }
    alarm = false;


    QRegExp rxData;
    switch ( this->type )
    {
        case DATAPOINT_SENSOR:
            {
                rxData.setPattern("^temperature=([-0-9.]+)");
                if (rxData.indexIn(data) != -1)
                {
                    value =  rxData.cap(1);
                    return true;
                }
            }
            break;
        case DATAPOINT_RH:
            {
                rxData.setPattern("temperature=([-0-9.]+)");
                QRegExp rxRH("rh=([0-9.]+)\%");

                QStringList list = data.split(";", QString::SkipEmptyParts);
                for (int i = 0; i < list.size(); ++i)
                {
                    QString part = list.at(i).trimmed();
                    //qDebug() << part;

                    if (rxData.indexIn(part) != -1)
                    {
                        value =  rxData.cap(1);
                    }
                    if (rxRH.indexIn(part) != -1)
                    {
                        value2 =  rxRH.cap(1);
                    }
                }

                if( ! (value.isEmpty() || value2.isEmpty()) )
                {
                    return true;
                }
            }
            break;
        case DATAPOINT_REGULATOR:
            {
                QRegExp rxValue("value=([-0-9.]+)");
                QRegExp rxSetpoint("setpoint=([-0-9.]+)");
                QRegExp rxOutput("output=([-0-9.]+)%");
                QRegExp rxActive("active=([-0-9.]+)%");
                QStringList list = data.split(";", QString::SkipEmptyParts);
                for (int i = 0; i < list.size(); ++i)
                {
                    QString part = list.at(i).trimmed();
                    //qDebug() << part;

                    if (rxValue.indexIn(part) != -1)
                        value =  rxValue.cap(1);
                    else if (rxSetpoint.indexIn(part) != -1)
                        setpoint = rxSetpoint.cap(1);
                    else if (rxOutput.indexIn(part) != -1)
                        output = rxOutput.cap(1);
                    else if (rxActive.indexIn(part) != -1)
                        active = rxActive.cap(1);
                }

                //Manadatory data must be there!
                if( ! (value.isEmpty() || setpoint.isEmpty() || output.isEmpty()) )
                {
                    return true;
                }
            }
            break;
        case DATAPOINT_METER:
            {
                rxData.setPattern("energy=([-0-9.]+) kWh");
                if (rxData.indexIn(data) != -1)
                {
                    value =  rxData.cap(1);
                    return true;
                }
            }
            break;
        case DATAPOINT_METER_KWH:
            {
                //energy=29.072kWh time=2016-12-31T06:52:19Z
                rxData.setPattern("energy=([-0-9.]+)kWh time=([0-9]{4,4}-[0-9]{2,2}-[0-9]{2,2}T[0-9]{2,2}:[0-9]{2,2}:[0-9]{2,2}Z)");
                if (rxData.indexIn(data) != -1)
                {
                    value =  rxData.cap(1);
                    date  =  rxData.cap(2);
                    return true;
                }
            }
            break;
        default :
            return false;
            break;
    }

    return false;
}

bool MessageParser::isAlarm()
{
    return alarm;
}

DataPointType MessageParser::getType()
{
    return this->type;
}

QString MessageParser::getDate()
{
    return date;
}

QString MessageParser::getValue()
{
    return value;
}

QString MessageParser::getValue2()
{
    return value2;
}

QString MessageParser::getSetpoint()
{
    return setpoint;
}
QString MessageParser::getOutput()
{
    return output;
}

QString MessageParser::getActive()
{
    if(!active.isEmpty())
        return active;

    return "";
}
