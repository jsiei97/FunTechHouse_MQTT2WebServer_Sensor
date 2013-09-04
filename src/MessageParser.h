/**
 * @file MessageParser.h
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
        QString active;


    public:
        MessageParser(QString data, DataPointType type);

        bool parse();
        bool isAlarm();

        DataPointType getType();

        QString getValue();
        QString getSetpoint();
        QString getOutput();
        QString getActive();

};

#endif  // __MESSAGEPARSER_H
