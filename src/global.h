/**
 * @file global.h
 * @author Johan Simonsson
 * @brief global variables and types
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

#ifndef  __GLOBAL_H
#define  __GLOBAL_H

extern pid_t pidMain; ///< This is the pid of the master/main process used to phone home.
extern pid_t pidMosq; ///< The pid for the MQTT/Mosq listner process
extern pid_t pidSurf; ///< The pid for the Quicksurf process

extern QString pipeSurfInput; ///< Filename for the data pipe that sends data into the surf process.

typedef enum
{
    DATAPOINT_NOT_VALID = 0, ///< The default not valid value.
    DATAPOINT_SENSOR,        ///< A value, and alarm
    DATAPOINT_RH,            ///< RH (Relative Humidity), and temperature with alarm.
    DATAPOINT_REGULATOR,     ///< value, setpoint and output. And alarm
    DATAPOINT_METER,         ///< A meter i.e. a electric meter with a incemental reading.
    DATAPOINT_METER_KWH      ///< A electric meter with timestamp
} DataPointType;


#endif  // __GLOBAL_H
