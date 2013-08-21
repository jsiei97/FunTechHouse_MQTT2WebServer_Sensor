#ifndef  __MOSQCONNECT_H
#define  __MOSQCONNECT_H

#include <mosquittopp.h>
#include "Sensor.h"

class MosqConnect : public mosqpp::mosquittopp
{
    private:
        QList<Sensor> *list;

	public:
		MosqConnect(const char *id, const char *host, int port, QList<Sensor> *list);
		~MosqConnect();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

#endif  // __MOSQCONNECT_H
