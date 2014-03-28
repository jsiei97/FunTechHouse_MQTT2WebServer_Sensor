/**
 * @file ConfigParse.cpp
 * @author Johan Simonsson  
 * @brief Read and parse config file
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
#include <QList>
#include <QFile>
#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "ConfigParse.h"
#include "DataPoint.h"
#include "global.h"

ConfigParse::ConfigParse(QString configfile)
{
    this->configfile = configfile;
    mqttServer = "";
    mqttAppName = "";
}

QString ConfigParse::getMqttServer()
{
    return mqttServer;
}

QString ConfigParse::getMqttAppName()
{
    return mqttAppName;
}

bool ConfigParse::parse(QList<DataPoint> *list)
{

    bool ret = true;
    /*
       for(int i=1; i<6; i++)
       {
       QString name("DataPoint");
       name.append(QString("%1").arg(i));
       qDebug() << name;
       list->append(DataPoint(name,  "b", "c", "d", "e"));
       }
       */

    QDomDocument doc("configfile");
    QFile file(configfile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error can't open file" << configfile;
        return false;
    }

    QString domErr;
    int domErrLine;
    int domErrCol;
    if (!doc.setContent(&file, false, &domErr, &domErrLine, &domErrCol))
    {
        file.close();
        qDebug() << "Error can't parse file" << configfile;
        qDebug() << " " << domErr << domErrLine << domErrCol;
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    for(QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if(e.tagName() == "mqtt_server")
            {
                mqttServer = e.text().trimmed();
            }
            if(e.tagName() == "mqtt_appname")
            {
                mqttAppName = e.text().trimmed();
            }
            if(e.tagName() == "datapoint")
            {
                DataPoint dp;
                //qDebug() << qPrintable(e.tagName()); // the node really is an element.
                if(e.hasAttribute("name"))
                {
                    qDebug() << qPrintable(e.tagName()) << qPrintable(e.attribute("name"));
                    dp.setName( e.attribute("name") );
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("baseurl");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        dp.setBaseURL( tagE.text() );
                    }
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("id");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        dp.setDeviceId( tagE.text() );
                    }
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("type");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        if(!dp.setType( tagE.text() ))
                        {
                            qDebug() << "Error: not a valid type:" << tagE.text();
                            ret = false;
                        }
                    }
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("topic");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        dp.setMosqTopic( tagE.text() );
                    }
                }


                if(dp.isOK())
                {
                    list->append(dp);
                }
            }
        }
        qDebug() << "";
    }


    if(mqttServer.size() == 0)
    {
        qDebug() << "Error missing config mqtt_server";
        return false;
    }
    if(mqttAppName.size() == 0)
    {
        qDebug() << "Error missing config mqtt_appname";
        return false;
    }

    return ret;
}
