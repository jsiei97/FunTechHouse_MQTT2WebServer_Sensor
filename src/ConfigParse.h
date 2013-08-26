/**
 * @file ConfigParse.h
 * @author Johan Simonsson
 * @brief Read and parse config file
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

#ifndef  __CONFIGPARSE_H
#define  __CONFIGPARSE_H

#include "DataPoint.h"

class ConfigParse
{
    private:
        QString configfile;

        QString mqttServer;
        QString mqttAppName;

    public:
        ConfigParse(QString configfile);
        bool parse(QList<DataPoint> *list);

        QString getMqttServer();
        QString getMqttAppName();
};

#endif  // __CONFIGPARSE_H
