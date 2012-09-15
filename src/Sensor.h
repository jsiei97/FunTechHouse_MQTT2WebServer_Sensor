#ifndef  __SENSOR_H
#define  __SENSOR_H

#include <QString>

class Sensor
{
    private:
        QString name;
        QString baseURL;
        QString deviceId;
        QString sensorName;

        QString mosqTopic;

    public:
        Sensor();
        Sensor( QString name,
                QString baseURL,
                QString deviceId,
                QString sensorName,
                QString mosqTopic);

        bool isOK();

        void print();

        void setName( QString name );
        void setBaseURL( QString baseURL );
        void setDeviceId( QString deviceId );
        void setSensorName( QString sensorName );
        void setMosqTopic( QString mosqTopic );

        QString getName();
        QString getBaseURL();
        QString getDeviceId();
        QString getSensorName();
        QString getMosqTopic();
};

#endif  // __SENSOR_H
