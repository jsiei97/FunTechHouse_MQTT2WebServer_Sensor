#include <QtCore>
#include <QtTest>

#include "Mosq2QuickSurf.h"
#include "global.h"

class TestMosq2QuickSurf : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void testParse_data();
        void testParse();
};

void TestMosq2QuickSurf::testParse_data()
{
    QTest::addColumn<QString>("data");
    QTest::addColumn<int>("type");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("url");

    QTest::newRow("alarm") << "Alarm: error error" 
        << (int)DATAPOINT_SENSOR << "ref"
        << "http://x/y.php?1=2&Larm=ref\%20error\%20error";

    QTest::newRow("test 01") << "temperature=15.4" 
        << (int)DATAPOINT_SENSOR << "ref"
        << "http://x/y.php?1=2&ref=15.4";

    QTest::newRow("test 02") << "energy=10.353 kWh" 
        << (int)DATAPOINT_METER << "ref"
        << "http://x/y.php?1=2&ref=10.353";

    QTest::newRow("test 03") << "value=102.30 ; setpoint=25.56 ; output=100% ; active=60%"
        << (int)DATAPOINT_REGULATOR << "ref"
        << "http://x/y.php?1=2&ref_Supply=102.30&ref_SetPoint=25.56&ref_Actuator=100&ref_Activator=60";

    QTest::newRow("test 04") << "temperature=55.44 ; rh=33.66\%" 
        << (int)DATAPOINT_RH << "ref03"
        << "http://x/y.php?1=2&ref03=55.44&ref04=33.66"; 

    QTest::newRow("test 05") << "temperature=55.44 ; rh=33.66\%" 
        << (int)DATAPOINT_RH << "ref13"
        << "http://x/y.php?1=2&ref13=55.44&ref14=33.66"; 
    //For now rh becomes the next number if name ends with 2 digits
}


extern QString my_global_url;
void TestMosq2QuickSurf::testParse()
{
    QFETCH(QString,data);
    QFETCH(int, type);
    DataPointType dataType = (DataPointType)type;
    QFETCH(QString,name);
    QFETCH(QString,url);

    DataPoint dp(name, "http://x/y.php", "1=2", dataType, "test");

    QVERIFY(Mosq2QuickSurf::send(&dp, data));

    QCOMPARE(url, my_global_url);
}


QTEST_MAIN(TestMosq2QuickSurf)
#include "TestMosq2QuickSurf.moc"
