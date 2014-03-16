/**
 * @file Mosq2QuickSurf.h
 * @author Johan Simonsson
 * @brief From a MQTT Message to a QuickSurf URL.
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

#ifndef  __MOSQ2QUICKSURF_H
#define  __MOSQ2QUICKSURF_H


#include <QString>
#include "DataPoint.h"

class Mosq2QuickSurf
{
    private:
    public:
        static bool send(DataPoint* dp, QString data);
};

#endif  // __MOSQ2QUICKSURF_H
