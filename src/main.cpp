#include <QDebug>
#include <QFile>
#include <getopt.h> /* getopt_long */

#include "Sensor.h"
#include "ConfigParse.h"
#include "MosqConnect.h"

void print_usage()
{
    qDebug("Usage");
    qDebug(" -c / --config [file]");
    qDebug("   What config file to use");
    exit(0);
}


int main(int argc, char *argv[])
{
    qDebug() << "FunTechHouse_Mosq2WebService_Sensor";

    int verbose = 0;
    QFile configFile;

    //A letter with : after means it would like a arg
    const char* short_options = "c:htv";
    const struct option long_options[] = {
        { "help",    0, NULL, 'h'},
        { "verbose", 0, NULL, 'v'},
        { "config",  1, NULL, 'c'},
        { "test",    0, NULL, 't'},
        { NULL,      0, NULL, 0}
    };

    int next_option;

    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
        //printf("next_option %x, %c (%d)\n", next_option, next_option, next_option);

        switch(next_option)
        {
            case -1 :
                {
                    break;
                }
            case 'h':
                {
                    print_usage();
                    break;
                }
            case 'v':
                {
                    verbose++;
                    break;
                }
            case 'c':
                {
                    configFile.setFileName( QString(optarg) );
                    break;
                }
            case 't':
                {
                    //test the config file so it is ok in some way...
                    break;
                }
            default:
                {
                    printf("Error\n");
                    abort();
                    break;
                }
        }
    }while(next_option != -1);


    if(verbose != 0)
    {
        qDebug() << "Verbose level" << verbose;
        //enable info prints in some way...
    }

    if(!configFile.exists())
    {
        qDebug() << "Error Can't find config file" << configFile.fileName();
        exit(1);
    }

    ConfigParse config(configFile.fileName());
    QList<Sensor> *list;
    list = new QList<Sensor>;

    if(!config.parse(list))
    {
        qDebug() << "Error Parse config failed";
        exit(1);
    }

    if(list->size() == 0)
    {
        qDebug() << "Error Kind off empty config" << configFile.fileName();
        exit(1);
    }

    if(verbose > 0)
    {
        qDebug() << "";
        qDebug() << "Configfile contains sensors:" << list->size();

        for(int i=0; i<list->size(); i++)
        {
            Sensor sensor = list->at(i);
            sensor.print();
        }
        qDebug() << "";
    }

	class MosqConnect *mqtt;
	int rc;

	mosqpp::lib_init();

	mqtt = new MosqConnect(
            config.getMqttAppName().toAscii(), 
            config.getMqttServer().toAscii(), 
            1883, 
            list
            );
	
	while(1){
		rc = mqtt->loop();
		if(rc){
			mqtt->reconnect();
		}
	}

	mosqpp::lib_cleanup();


}
