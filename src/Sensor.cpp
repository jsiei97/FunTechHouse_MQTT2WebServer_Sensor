#include <QString>
#include <QDebug>

#include "Sensor.h"

Sensor::Sensor()
{
    this->name = "";
    this->baseURL = "";
    this->deviceId = "";
    this->sensorName = "";
    this->mosqTopic = "";
}

Sensor::Sensor(
        QString name,
        QString baseURL,
        QString deviceId,
        QString sensorName,
        QString mosqTopic)
{
    this->name = name;
    this->baseURL = baseURL;
    this->deviceId = deviceId;
    this->sensorName = sensorName;
    this->mosqTopic = mosqTopic;
}

bool Sensor::isOK()
{
    if( ( name.size()       > 0 ) &&
        ( baseURL.size()    > 0 ) &&
        ( deviceId.size()   > 0 ) &&
        ( sensorName.size() > 0 ) &&
        ( mosqTopic.size()  > 0 )
      )
    {
        return true;
    }
    print();
    return false;
}

void Sensor::print()
{
    qDebug() << "Sensordata"
        << name
        << baseURL
        << deviceId
        << sensorName
        << mosqTopic;
}


void Sensor::setName( QString name ){
    this->name = name.trimmed();
}
void Sensor::setBaseURL( QString baseURL ){
    this->baseURL = baseURL.trimmed();;
}
void Sensor::setDeviceId( QString deviceId ){
    this->deviceId = deviceId.trimmed();;
}
void Sensor::setSensorName( QString sensorName ){
    this->sensorName = sensorName.trimmed();;
}
void Sensor::setMosqTopic( QString mosqTopic ){
    this->mosqTopic = mosqTopic.trimmed();;
}

QString Sensor::getName(){
    return this->name;
}
QString Sensor::getBaseURL(){
    return this->baseURL;
}
QString Sensor::getDeviceId(){
    return this->deviceId;
}
QString Sensor::getSensorName(){
    return this->sensorName;
}
QString Sensor::getMosqTopic(){
    return this->mosqTopic;
}

