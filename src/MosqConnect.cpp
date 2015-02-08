/**
 * @file MosqConnect.cpp
 * @author Johan Simonsson
 * @brief Mosquitto interface
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

#include <cstdio>
#include <cstring>

#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QDateTime>
#include <QUrl>

#include <mosquittopp.h>

#include "MosqConnect.h"
#include "Mosq2QuickSurf.h"
#include "DataPoint.h"
#include "MessageParser.h"


MosqConnect::MosqConnect(const char *id, const char *host, int port, QList<DataPoint> *list) : mosquittopp(id)
{
    this->list = list;
    int keepalive = 60;

    // Connect immediately.
    connect(host, port, keepalive);
};

MosqConnect::~MosqConnect()
{
}

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

    /// @todo create topic2url function

    for(int i=0; i<list->size(); i++)
    {
        DataPoint dp = list->at(i);
        if(topic == dp.getMosqTopic())
        {
            qDebug() << "Mess from" << dp.getName() << mess << topic;
            if(!Mosq2QuickSurf::send(&dp, mess))
            {
                qDebug() << "Error, FAILED to send mess to webserver:" 
                    << dp.getName() << mess << topic;

            }
        }
    }
}

void MosqConnect::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    printf("Subscription succeeded. mid=%d qos=%d granter_qos=%d\n", mid, qos_count, *granted_qos);
}

