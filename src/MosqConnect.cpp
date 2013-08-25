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
#include "DataPoint.h"
#include "MessageParser.h"


MosqConnect::MosqConnect(const char *id, const char *host, int port, QList<DataPoint> *list) : mosquittopp(id)
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
            DataPoint dp = list->at(i);
            qDebug() << "subscribe" << dp.getMosqTopic();
            subscribe(NULL, dp.getMosqTopic().toAscii());
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
        DataPoint dp = list->at(i);
        if(topic == dp.getMosqTopic())
        {
            qDebug() << "Mess from" << dp.getName() << mess << topic;

            QString url = dp.getBaseURL();
            url.append("?");
            url.append(dp.getDeviceId());

            //qDebug() << "URL" << url;

            //is this a alarm or normal data?
            MessageParser parser(mess, dp.getType());
            bool dataOK = false;
            if(parser.parse())
            {
                if(parser.isAlarm())
                {
                    //Send a alarm
                    url.append("&Larm=");
                    QString alarm(dp.getName());
                    alarm.append(" ");
                    alarm.append(parser.getValue());
                    url.append( QUrl::toPercentEncoding(alarm, "", " ") );
                    qDebug() << "Alarm url:" << url;

                    dataOK = true;
                }
                else
                {
                    url.append("&");
                    url.append(dp.getName());
                    if( parser.getType() == DATAPOINT_REGULATOR ||
                        parser.getType() == DATAPOINT_REGULATOR )
                    {
                        url.append("_Supply");
                    }
                    url.append("=");
                    url.append(parser.getValue());

                    if(parser.getType() == DATAPOINT_REGULATOR)
                    {
                        url.append("&");
                        url.append(dp.getName());
                        url.append("_SetPoint");
                        url.append("=");
                        url.append(parser.getSetpoint());

                        url.append("&");
                        url.append(dp.getName());
                        url.append("_Actuator");
                        url.append("=");
                        url.append(parser.getOutput());
                    }

                    dataOK = true;
                }
            }

            if(dataOK)
            {
                bool result = false;
                int  retry = 3;
                do
                {
                    /// @todo Write to pipe, and wait for ack signal
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

