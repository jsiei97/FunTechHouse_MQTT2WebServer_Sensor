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
