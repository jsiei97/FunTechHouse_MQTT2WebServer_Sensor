#ifndef  __CONFIGPARSE_H
#define  __CONFIGPARSE_H

#include "Sensor.h"

class ConfigParse
{
    private:
        QString configfile;

        QString mqttServer;
        QString mqttAppName;

    public:
        ConfigParse(QString configfile);
        bool parse(QList<Sensor> *list);

        QString getMqttServer();
        QString getMqttAppName();
};

#endif  // __CONFIGPARSE_H
