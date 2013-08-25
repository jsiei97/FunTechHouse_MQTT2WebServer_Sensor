#ifndef  __CONFIGPARSE_H
#define  __CONFIGPARSE_H

#include "DataPoint.h"

class ConfigParse
{
    private:
        QString configfile;

        QString mqttServer;
        QString mqttAppName;

    public:
        ConfigParse(QString configfile);
        bool parse(QList<DataPoint> *list);

        QString getMqttServer();
        QString getMqttAppName();
};

#endif  // __CONFIGPARSE_H
