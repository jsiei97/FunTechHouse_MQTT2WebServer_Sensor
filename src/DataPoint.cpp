#include <QString>
#include <QDebug>

#include "DataPoint.h"
#include "global.h"

DataPoint::DataPoint()
{
    this->name = "";
    this->baseURL = "";
    this->deviceId = "";
    this->type = DATAPOINT_NOT_VALID;
    this->mosqTopic = "";
}

DataPoint::DataPoint(
        QString name,
        QString baseURL,
        QString deviceId,
        DataPointType type,
        QString mosqTopic)
{
    this->name = name;
    this->baseURL = baseURL;
    this->deviceId = deviceId;
    this->type = type;
    this->mosqTopic = mosqTopic;
}

bool DataPoint::isOK()
{
    if( ( name.size()       > 0 ) &&
        ( baseURL.size()    > 0 ) &&
        ( deviceId.size()   > 0 ) &&
        ( type != DATAPOINT_NOT_VALID) &&
        ( mosqTopic.size()  > 0 )
      )
    {
        return true;
    }
    print();
    return false;
}

void DataPoint::print()
{
    QString typeName = "";
    switch ( this->type )
    {
        case DATAPOINT_SENSOR :
            typeName.append("sensor");
            break;
        case DATAPOINT_REGULATOR:
            typeName.append("regulator");
            break;
        case DATAPOINT_METER:
            typeName.append("meter");
            break;
        default :
            typeName.append("NOT VALID TYPE");
            break;
    }

    qDebug() << __FILE__ << __LINE__ << "DataPointdata"
        << name
        << baseURL
        << deviceId
        << typeName
        << mosqTopic;
}


void DataPoint::setName( QString name ){
    this->name = name.trimmed();
}
void DataPoint::setBaseURL( QString baseURL ){
    this->baseURL = baseURL.trimmed();
}
void DataPoint::setDeviceId( QString deviceId ){
    this->deviceId = deviceId.trimmed();
}
bool DataPoint::setType( QString type ){
    type = type.trimmed();
    bool ret = false;
    if(0==type.compare("sensor"))
    {
            this->type = DATAPOINT_SENSOR;
            ret = true;
    }
    else if(0==type.compare("regulator"))
    {
            this->type = DATAPOINT_REGULATOR;
            ret = true;
    }
    else if(0==type.compare("meter"))
    {
            this->type = DATAPOINT_METER;
            ret = true;
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << "Error: What is this?" << type;

    }

    return ret;
}
void DataPoint::setMosqTopic( QString mosqTopic ){
    this->mosqTopic = mosqTopic.trimmed();
}

QString DataPoint::getName(){
    return this->name;
}
QString DataPoint::getBaseURL(){
    return this->baseURL;
}
QString DataPoint::getDeviceId(){
    return this->deviceId;
}
DataPointType DataPoint::getType()
{
    return this->type;
}
QString DataPoint::getMosqTopic(){
    return this->mosqTopic;
}

