/**
 * @file main.cpp
 * @author Johan Simonsson
 * @brief Main
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
#include <QFile>
#include <QFileInfo>

#include <errno.h>
#include <getopt.h>
#include <linux/kernel.h> // for struct sysinfo
#include <linux/unistd.h> // for _syscallX macros/related stuff
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#include "DataPoint.h"
#include "ConfigParse.h"
#include "MosqConnect.h"

void sigusr2(int signo, siginfo_t *info, void *extra);
void sigterm(int signo, siginfo_t *info, void *extra);

void print_usage()
{
    qDebug("Usage");
    qDebug(" -c / --config [file]");
    qDebug("   What config file to use");
    exit(0);
}

pid_t pidMain = 0;
pid_t pidMosq = 0;
long  mosqUptime = 0;


long get_uptime()
{
    struct sysinfo s_info;
    int error;
    error = sysinfo(&s_info);
    if(error != 0)
    {
        printf("code error = %d\n", error);
        return -1;
    }
    return s_info.uptime;
}

int main(int argc, char *argv[])
{
    qDebug() << "FunTechHouse_Mosq2WebService_dp";

    //
    //First check all the args
    //

    int verbose = 0;
    bool testConfig = false;
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
                break;
            case 'h':
                print_usage();
                break;
            case 'v':
                verbose++;
                break;
            case 'c':
                configFile.setFileName( QString(optarg) );
                break;
            case 't':
                verbose++;
                testConfig = true;
                break;
            default:
                printf("Error\n");
                abort();
                break;
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

    //
    // Then parse the configfile so we have something to work with.
    //
    ConfigParse config(configFile.fileName());
    QList<DataPoint> *list;
    list = new QList<DataPoint>;

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
        qDebug() << "Configfile contains DataPoints:" << list->size();

        for(int i=0; i<list->size(); i++)
        {
            DataPoint dp = list->at(i);
            dp.print();
        }
        qDebug() << "";
    }

    if(testConfig)
    {
        exit(0);
    }


    //
    //Let's create the signal reciver for the ET phone home pings
    //
    //MetodikTest/qt/unixSignals_kill
    //MetodikTest/C/fork/sigusr1.c

    //qDebug() << "SIGUSR2 .... " << SIGUSR2;

    {
        //Add sigqueue for SIGUSR2
        struct sigaction action;

        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = &sigusr2;

        if (sigaction(SIGUSR2, &action, NULL) == -1) {
            perror("sigusr: sigaction");
            _exit(1);
        }
    }

    //
    // What is this my pid, this will be used by the forked processes to phone home
    //
    pidMain = getpid();
    //qDebug() << "Master/main pid" << pidMain;

    pid_t forkvalue = fork();
    if(0 == forkvalue)
    {
        pidMosq = getpid();
        qDebug() << "Child/MQTT :" << "Master:" << pidMain << "Mosq:" << pidMosq;

        class MosqConnect *mqtt;
        int rc;

        mosqpp::lib_init();

        mqtt = new MosqConnect(
                config.getMqttAppName().toAscii(),
                config.getMqttServer().toAscii(),
                1883,
                list
                );

        while(1)
        {
            rc = mqtt->loop();
            if(rc)
            {
                mqtt->reconnect();
            }

            sleep(1);
            union sigval value;
            value.sival_int = 30;
            sigqueue(pidMain, SIGUSR2, value);
        }
        mosqpp::lib_cleanup();
        qDebug() << "Child/MQTT : Done";
        exit(1);
    }
    else
    {
        //qDebug() << "Master/main:" << getpid() << forkvalue;
        pidMosq = forkvalue;
    }

    qDebug() << "Master/main :" << "Master:" << pidMain << "Mosq:" << pidMosq;


    {
        //Add sigqueue for SIGTERM
        struct sigaction action;

        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = &sigterm;

        if (sigaction(SIGTERM, &action, NULL) == -1) {
            perror("sigterm: sigaction");
            _exit(1);
        }
    }


    //Wait for the other process to become stable
    sleep(10);

    while(1)
    {
        long time = get_uptime();

        if( (time-mosqUptime) > 120 )
        {
            printf("No phone home, lets kill pid: %d\n", pidMosq);
            kill(pidMosq, SIGKILL);
            sleep(1);
            exit(1);
        }

        sleep(1);
    }
}

void sigterm(int signo, siginfo_t *info, void *extra)
{
    //http://www.kernel.org/doc/man-pages/online/pages/man2/sigaction.2.html
    void* pval = info->si_value.sival_ptr;

    int   val = info->si_value.sival_int;
    pid_t pid = info->si_pid; // Sending process ID
    uid_t uid = info->si_uid; // Real user ID of sending process

    printf("%s:%d : Signal %d, value %d pid=%d uid=%d \n", __func__, __LINE__, signo, val, pid, uid);

    printf("%s:%d : send kill to pid=%d \n", __func__, __LINE__, pidMosq);
    kill(pidMosq, SIGKILL);
    sleep(1);
    printf("%s:%d : About to exit, pid=%d \n", __func__, __LINE__, pidMain);
    exit(1);
}

void sigusr2(int signo, siginfo_t *info, void *extra)
{
    //http://www.kernel.org/doc/man-pages/online/pages/man2/sigaction.2.html
    void* pval = info->si_value.sival_ptr;

    int   val = info->si_value.sival_int;
    pid_t pid = info->si_pid; // Sending process ID
    uid_t uid = info->si_uid; // Real user ID of sending process

    //printf("%s:%d : Signal %d, value %d pid=%d uid=%d \n", __func__, __LINE__, signo, val, pid, uid);

    if(pid == pidMosq)
    {
        printf("%s:%d : pidMosq is alive\n", __func__, __LINE__);
        mosqUptime = get_uptime();
    }
}

