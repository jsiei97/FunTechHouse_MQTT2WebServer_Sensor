/**
 * @file Mosq2QuickSurf.cpp
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

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QRegExp>

#include "QuickSurf.h"
#include "Mosq2QuickSurf.h"
#include "DataPoint.h"
#include "MessageParser.h"

bool Mosq2QuickSurf::send(DataPoint* dp, QString data)
{
    QString url = dp->getBaseURL();
    url.append("?");
    url.append(dp->getDeviceId());

    //qDebug() << "URL" << url;

    MessageParser parser(data, dp->getType());
    if(!parser.parse())
    {
        qDebug() << "Failed to parse data:" << data << dp->getType();
        return false;
    }

    bool dataOK = false;

    //is this a alarm or normal data?
    if(parser.isAlarm())
    {
        //Send a alarm
        url.append("&Larm=");
        QString alarm(dp->getName());
        alarm.append(" ");
        alarm.append(parser.getValue());
        url.append( QUrl::toPercentEncoding(alarm, "", " ") );
        qDebug() << "Alarm url:" << url;

        dataOK = true;
    }
    else
    {
        url.append("&");
        url.append(dp->getName());
        if( parser.getType() == DATAPOINT_REGULATOR )
        {
            url.append("_Supply");
        }
        url.append("=");
        url.append(parser.getValue());

        if( parser.getType() == DATAPOINT_RH )
        {
            QString val2 = parser.getValue2();
            if(!val2.isEmpty())
            {
                QString name = dp->getName();

                QRegExp rxNum(".*(\\d{1,2})");
                int pos = rxNum.indexIn(name);
                //if(rxNum.captureCount() == 1)
                if (pos > -1)
                {
                    bool ok;
                    QString numStr = rxNum.cap(1);
                    int num = numStr.toInt(&ok, 10);
                    num++;

                    if(ok)
                    {
                        name.chop(numStr.size());
                        name.append( QString("%1").arg(num, 2, 10, QChar('0')) );

                        url.append("&");
                        url.append(name);
                        url.append("=");
                        url.append(val2);
                    }
                }
            }
        }


        if(parser.getType() == DATAPOINT_REGULATOR)
        {
            url.append("&");
            url.append(dp->getName());
            url.append("_SetPoint");
            url.append("=");
            url.append(parser.getSetpoint());

            url.append("&");
            url.append(dp->getName());
            url.append("_Actuator");
            url.append("=");
            url.append(parser.getOutput());

            if(!parser.getActive().isEmpty())
            {
                url.append("&");
                url.append(dp->getName());
                url.append("_Activator");
                url.append("=");
                url.append(parser.getActive());
            }
        }

        QString date = parser.getDate();
        if(!date.isEmpty())
        {
            url.append("&date=");
            url.append(date);
        }

        dataOK = true;
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

        return result;
    }

    return false;
}
