#include <cstdio>
#include <cstring>

#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QDateTime>
#include <QUrl>

#include <mosquittopp.h>

#include "MosqConnect.h"
#include "QuickSurf.h"
#include "Sensor.h"
#include "RoomTemperatureDataParser.h"


MosqConnect::MosqConnect(const char *id, const char *host, int port, QList<Sensor> *list) : mosquittopp(id)
{
    this->list = list;
    int keepalive = 60;

    // Connect immediately. 
    connect(host, port, keepalive);
};

void MosqConnect::on_connect(int rc)
{
    printf("Connected with code %d.\n", rc);
    if(rc == 0){
        // Only attempt to subscribe on a successful connect. 

        for(int i=0; i<list->size(); i++)
        {
            Sensor sensor = list->at(i);
            qDebug() << "subscribe" << sensor.getMosqTopic();
            subscribe(NULL, sensor.getMosqTopic().toAscii());
        }
    }
}

void MosqConnect::on_message(const struct mosquitto_message *message)
{
    /*
       struct mosquitto_message{
       int mid;
       char *topic;
       void *payload;
       int payloadlen;
       int qos;
       bool retain;
       };
       */

    //Move from "void* with payloadlen" to a QString.
    char* messData = (char*)malloc(sizeof(char)*(message->payloadlen+1));
    memcpy(messData, message->payload, message->payloadlen);
    messData[message->payloadlen] = '\0';

    //printf("Message %s - %s.\n", message->topic, messData);

    QString mess = QString(messData);
    free(messData);

    QString topic = QString(message->topic);

    //qDebug() << "\nNew message:" << (QDateTime::currentDateTime()).toString("hh:mm:ss") << topic << mess;

    for(int i=0; i<list->size(); i++)
    {
        Sensor sensor = list->at(i);
        if(topic == sensor.getMosqTopic())
        {
            qDebug() << "Mess from" << sensor.getName() << mess << topic;

            QString url = sensor.getBaseURL();
            url.append("?");
            url.append(sensor.getDeviceId());

            qDebug() << "URL" << url;

            //is this a alarm or normal data?
            RoomTemperatureDataParser parser(mess);
            bool dataOK = false;
            if(parser.parse())
            {
                if(parser.isAlarm())
                {
                    //Send a alarm
                    url.append("&Larm=");
                    QString alarm(sensor.getName());
                    alarm.append(" ");
                    alarm.append(parser.getValue());
                    url.append( QUrl::toPercentEncoding(alarm, "", " ") );
                    qDebug() << "Alarm url:" << url;

                    dataOK = true;
                }
                else
                {
                    url.append("&");
                    url.append(sensor.getSensorName());
                    url.append("=");
                    url.append(parser.getValue());

                    dataOK = true;
                }
            }

            if(dataOK)
            {
                bool result = false;
                int  retry = 3;
                do
                {
                    result = QuickSurf::doSurf(url);
                    if(result == false)
                    {
                        qDebug() << "Error: QuickSurf failed: retry" << retry;
                    }
                    retry--;
                } while(result == false && retry != 0);
            }
        } 
    }
}

void MosqConnect::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    printf("Subscription succeeded. mid=%d qos=%d granter_qos=%d\n", mid, qos_count, *granted_qos);
}

