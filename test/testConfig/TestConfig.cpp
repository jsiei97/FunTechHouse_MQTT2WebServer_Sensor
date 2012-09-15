#include <QtCore>
#include <QtTest>

#include "Sensor.h"
#include "ConfigParse.h"

class TestConfig : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void testSensor();
        void testParseFile();
};

void TestConfig::testSensor()
{
    Sensor sensor;
    QCOMPARE(sensor.isOK(), false);
    {
        QString data("test01");
        sensor.setName( data);
        QCOMPARE(sensor.getName(), data);
    }
    QCOMPARE(sensor.isOK(), false);
    {
        QString data("test02");
        sensor.setBaseURL( data );
        QCOMPARE(sensor.getBaseURL(), data);
    }
    QCOMPARE(sensor.isOK(), false);
    {
        QString data("test03");
        sensor.setDeviceId( data );
        QCOMPARE(sensor.getDeviceId(), data);
    }
    QCOMPARE(sensor.isOK(), false);
    {
        QString data("test04");
        sensor.setSensorName( data );
        QCOMPARE(sensor.getSensorName(), data);
    }
    QCOMPARE(sensor.isOK(), false);
    {
        QString data("test05");
        sensor.setMosqTopic( data );
        QCOMPARE(sensor.getMosqTopic(), data);
    }
    QCOMPARE(sensor.isOK(), true);

}
void TestConfig::testParseFile()
{
    //QCOMPARE(10.0, filter.getValue());

    ConfigParse config("files/config01.xml");
    QList<Sensor> *list;
    list = new QList<Sensor>;

    config.parse(list);

    QCOMPARE(config.getMosqServer(), QString("localhost"));
    QCOMPARE(config.getAppName(),    QString("FunTechHouse_MQTT2WebServer_Sensor__01"));

    QCOMPARE(list->size(), 3);


    {
        Sensor sensor = list->at(0);
        QCOMPARE(sensor.getName(),    QString("sensor01"));
        QCOMPARE(sensor.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(sensor.getDeviceId(), QString("id=1"));
        QCOMPARE(sensor.getSensorName(), QString("ref01"));
        QCOMPARE(sensor.getMosqTopic(), QString("FunTechHouse/Room1/Temperature"));
    }

    {
        Sensor sensor = list->at(1);
        QCOMPARE(sensor.getName(),    QString("sensor02"));
        QCOMPARE(sensor.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(sensor.getDeviceId(), QString("id=2"));
        QCOMPARE(sensor.getSensorName(), QString("ref02"));
        QCOMPARE(sensor.getMosqTopic(), QString("FunTechHouse/Room2/Temperature"));
    }

    {
        Sensor sensor = list->at(2);
        QCOMPARE(sensor.getName(),    QString("sensor03"));
        QCOMPARE(sensor.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(sensor.getDeviceId(), QString("id=3"));
        QCOMPARE(sensor.getSensorName(), QString("ref03"));
        QCOMPARE(sensor.getMosqTopic(), QString("FunTechHouse/Room3/Temperature"));
    }

}

QTEST_MAIN(TestConfig)
#include "TestConfig.moc"
