#include <QDebug>
#include <QString>
#include <QList>
#include <QFile>
#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "ConfigParse.h"
#include "Sensor.h"

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

bool ConfigParse::parse(QList<Sensor> *list)
{

    /*
       for(int i=1; i<6; i++)
       {
       QString name("Sensor");
       name.append(QString("%1").arg(i));
       qDebug() << name;
       list->append(Sensor(name,  "b", "c", "d", "e"));
       }
       */

    QDomDocument doc("configfile");
    QFile file(configfile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error can't open file";
        return false;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        qDebug() << "Error can't parse file";
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
            if(e.tagName() == "sensor")
            {
                Sensor sensor;
                //qDebug() << qPrintable(e.tagName()); // the node really is an element.
                if(e.hasAttribute("name"))
                {
                    //qDebug() << qPrintable(e.tagName()) << qPrintable(e.attribute("name"));
                    sensor.setName( e.attribute("name") );
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("baseurl");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        //qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        sensor.setBaseURL( tagE.text() );
                    }
                }
                {
                    QDomNodeList tagId = e.elementsByTagName("id");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        //qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        sensor.setDeviceId( tagE.text() );
                    }
                }

                {
                    QDomNodeList tagId = e.elementsByTagName("sensor_name");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        //qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        sensor.setSensorName( tagE.text() );
                    }
                }
                
                {
                    QDomNodeList tagId = e.elementsByTagName("topic");
                    for(int i=0; i<tagId.count(); i++)
                    {
                        QDomNode tag = tagId.at(i);
                        QDomElement tagE = tag.toElement();
                        //qDebug() << qPrintable(tagE.tagName()) << tagE.text();
                        sensor.setMosqTopic( tagE.text() );
                    }
                }


                if(sensor.isOK())
                {
                    list->append(sensor);
                }
            }
        }
        //qDebug() << "";
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

    return true;
}
