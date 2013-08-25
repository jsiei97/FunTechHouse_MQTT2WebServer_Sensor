#include <QtCore>
#include <QtTest>

#include "DataPoint.h"
#include "ConfigParse.h"
#include "global.h"

class TestConfig : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void testDataPoint();
        void testParseFile_02();
};

void TestConfig::testDataPoint()
{
    DataPoint dp;
    QCOMPARE(dp.isOK(), false);

    {
        QString data("test01");
        dp.setName( data);
        QCOMPARE(dp.getName(), data);
    }
    QCOMPARE(dp.isOK(), false);

    {
        QString data("test02");
        dp.setBaseURL( data );
        QCOMPARE(dp.getBaseURL(), data);
    }
    QCOMPARE(dp.isOK(), false);

    {
        QString data("test03");
        dp.setDeviceId( data );
        QCOMPARE(dp.getDeviceId(), data);
    }
    QCOMPARE(dp.isOK(), false);

    {
        QString data("test05");
        dp.setMosqTopic( data );
        QCOMPARE(dp.getMosqTopic(), data);
    }
    QCOMPARE(dp.isOK(), false);

    {
        dp.setType( "sensor" );
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_SENSOR);
    }
    QCOMPARE(dp.isOK(), true);

}


void TestConfig::testParseFile_02()
{
    ConfigParse config("files/config02.xml");
    QList<DataPoint> *list;
    list = new QList<DataPoint>;

    QVERIFY(config.parse(list));

    QCOMPARE(config.getMqttServer(), QString("localhost"));
    QCOMPARE(config.getMqttAppName(),QString("FunTechHouse_MQTT2WebServer"));

    QCOMPARE(list->size(), 5);

    {
        DataPoint dp = list->at(0);
        QCOMPARE(dp.getName(),    QString("ref01"));
        QCOMPARE(dp.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(dp.getDeviceId(), QString("s=1"));
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_SENSOR );
        QCOMPARE(dp.getMosqTopic(), QString("FunTechHouse/Room1/Temperature"));
    }

    {
        DataPoint dp = list->at(1);
        QCOMPARE(dp.getName(),    QString("Shunt3"));
        QCOMPARE(dp.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(dp.getDeviceId(), QString("id=1"));
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_REGULATOR);
        QCOMPARE(dp.getMosqTopic(), QString("FunTechHouse/Pannrum/ElPanna"));
    }

    {
        DataPoint dp = list->at(2);
        QCOMPARE(dp.getName(),    QString("Shunt2"));
        QCOMPARE(dp.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(dp.getDeviceId(), QString("id=2"));
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_REGULATOR);
        QCOMPARE(dp.getMosqTopic(), QString("FunTechHouse/Pannrum/VMP"));
    }

    {
        DataPoint dp = list->at(3);
        QCOMPARE(dp.getName(),    QString("Shunt1"));
        QCOMPARE(dp.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(dp.getDeviceId(), QString("id=2"));
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_REGULATOR);
        QCOMPARE(dp.getMosqTopic(), QString("FunTechHouse/Pannrum/shunt"));
    }
    {
        DataPoint dp = list->at(4);
        QCOMPARE(dp.getName(),    QString("El"));
        QCOMPARE(dp.getBaseURL(), QString("http://localhost/test/index.php"));
        QCOMPARE(dp.getDeviceId(), QString("id=6"));
        QCOMPARE((int)dp.getType(), (int)DATAPOINT_METER);
        QCOMPARE(dp.getMosqTopic(), QString("FunTechHouse/el"));
    }

}

QTEST_MAIN(TestConfig)
#include "TestConfig.moc"
