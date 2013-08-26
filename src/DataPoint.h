/**
 * @file DataPoint.h
 * @author Johan Simonsson
 * @brief A datapoint read from config
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

#ifndef  __DATAPOINT_H
#define  __DATAPOINT_H

#include <QString>
#include "global.h"

class DataPoint
{
    private:
        QString name;
        QString baseURL;
        QString deviceId;
        DataPointType type;

        QString mosqTopic;

    public:
        DataPoint();
        DataPoint( QString name,
                QString baseURL,
                QString deviceId,
                DataPointType type,
                QString mosqTopic);

        bool isOK();

        void print();

        void setName( QString name );
        void setBaseURL( QString baseURL );
        void setDeviceId( QString deviceId );
        bool setType( QString type );

        void setMosqTopic( QString mosqTopic );

        QString getName();
        QString getBaseURL();
        QString getDeviceId();
        DataPointType getType();
        QString getMosqTopic();
};

#endif  // __DATAPOINT_H
