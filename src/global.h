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
    DATAPOINT_REGULATOR,     ///< value, setpoint and output. And alarm
    DATAPOINT_METER          ///< A meter i.e. a electric meter with a incemental reading.
} DataPointType;


#endif  // __GLOBAL_H
