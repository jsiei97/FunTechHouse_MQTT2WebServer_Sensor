FunTechHouse MQTT2WebServer Sensor
==================================

Moves temperature data from a mqtt server (like mosquitto) to a old school web server aka web services.

Usage, build and then run from the command line with arg -c.

Today it moves data that is put into a MQTT broker with 
FunTechHouse RoomTemperature onto a webserver with http get.

Depends on
==========
1. Mosquitto c++ lib 1.0.2 and newer
2. libcurl (apt-get install libcurl4-openssl-dev)

