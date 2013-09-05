#include <QtCore>
#include <QtTest>

#include "MessageParser.h"
#include "global.h"

class TestDataParser : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void testParse_data();
        void testParse();
};

void TestDataParser::testParse_data()
{
    QTest::addColumn<QString>("data");
    QTest::addColumn<int>("type");
    QTest::addColumn<bool>("isAlarm");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("setpoint");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("active");

    QTest::newRow("normal data 01") << "temperature=54.34" << (int)DATAPOINT_SENSOR << false << "54.34" << "" << "" << "";

    QTest::newRow("normal data 02") << "temperature=22.3"  << (int)DATAPOINT_SENSOR << false << "22.3" << "" << "" << "";
    QTest::newRow("normal data 03") << "temperature=9.0"   << (int)DATAPOINT_SENSOR << false << "9.0" << "" << "" << "";
    QTest::newRow("normal data 04") << "temperature=-5.5"  << (int)DATAPOINT_SENSOR << false << "-5.5" << "" << "" << "";
    QTest::newRow("normal data 05") << "temperature=-15.4" << (int)DATAPOINT_SENSOR << false << "-15.4" << "" << "" << "";

    QTest::newRow("Alarm 01") 
        << "Alarm: High temperature=26.9 level=25.0" 
        << (int)DATAPOINT_SENSOR 
        << true 
        << "High temperature=26.9 level=25.0" << "" << "" << "";
    QTest::newRow("Alarm 02") 
        << "Alarm: Low temperature=21.9 level=22.0" 
        << (int)DATAPOINT_SENSOR
        << true 
        << "Low temperature=21.9 level=22.0" << "" << "" << "";



    QTest::newRow("meter 01") << "energy=1606.353 kWh" << (int)DATAPOINT_METER << false << "1606.353" << "" << "" << "";
    QTest::newRow("meter 02") <<  "energy=983.522 kWh" << (int)DATAPOINT_METER << false <<  "983.522" << "" << "" << "";
    QTest::newRow("meter 03") <<   "energy=23.001 kWh" << (int)DATAPOINT_METER << false <<   "23.001" << "" << "" << "";
    QTest::newRow("meter 04") <<    "energy=2.105 kWh" << (int)DATAPOINT_METER << false <<    "2.105" << "" << "" << "";

    QTest::newRow("meter 05") << "energy=5671606.353 kWh" << (int)DATAPOINT_METER << false << "5671606.353" << "" << "" << "";


    QTest::newRow("regul 01") << "value=55.69 ; setpoint=60.00 ; output=000%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "55.69" << "60.00" << "000" << "";

    QTest::newRow("regul 02") << "value=54.97 ; setpoint=60.00 ; output=033%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "54.97" << "60.00" << "033" << "";

    QTest::newRow("regul 03") << "value=-10.21 ; setpoint=-35.00 ; output=070%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "-10.21" << "-35.00" << "070" << "";

    QTest::newRow("regul 04") << "value=102.30 ; setpoint=25.56 ; output=100%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "102.30" << "25.56" << "100" << "";

    QTest::newRow("regul 05") << "value=102.30 ; setpoint=25.56 ; output=100% ; active=60%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "102.30" << "25.56" << "100" << "60";

    QTest::newRow("regul 06") << "value=102.30 ; setpoint=25.56 ; output=100% ; active=000%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "102.30" << "25.56" << "100" << "000";

    QTest::newRow("regul 07") << "value=102.30 ; setpoint=25.56 ; output=100% ; active=100%" 
        << (int)DATAPOINT_REGULATOR << false 
        << "102.30" << "25.56" << "100" << "100";

    QTest::newRow("Regul Alarm 01") 
        << "Alarm: Low ; value=13.56 ; alarm=45.67 ; setpoint=50 ; output=100%"
        << (int)DATAPOINT_REGULATOR 
        << true 
        << "Low ; value=13.56 ; alarm=45.67 ; setpoint=50 ; output=100%" << "" << "" << "";

    QTest::newRow("Regul Alarm 02") 
        << "Alarm: High ; value=40.00 ; alarm=20.00 ; setpoint=50.00 ; output=000%"
        << (int)DATAPOINT_REGULATOR 
        << true 
        << "High ; value=40.00 ; alarm=20.00 ; setpoint=50.00 ; output=000%" << "" << "" << "";

}

void TestDataParser::testParse()
{
    QFETCH(QString,data);
    QFETCH(int, type);
    DataPointType dataType = (DataPointType)type;
    QFETCH(bool,isAlarm);
    QFETCH(QString,value);
    QFETCH(QString,setpoint);
    QFETCH(QString,output);
    QFETCH(QString,active);

    //qDebug() << __FILE__ << __LINE__ << data << isAlarm << value;

    MessageParser parser(data, dataType);
    QCOMPARE(parser.parse(), true);

    switch ( dataType )
    {
        case DATAPOINT_SENSOR:
            QCOMPARE(parser.isAlarm(), isAlarm);
            QCOMPARE(parser.getValue(), value);
            break;
        case DATAPOINT_REGULATOR:
            QCOMPARE(parser.isAlarm(), isAlarm);
            QCOMPARE(parser.getValue(), value);
            if(!parser.isAlarm())
            {
                QCOMPARE(parser.getSetpoint(), setpoint);
                QCOMPARE(parser.getOutput(), output);
                QCOMPARE(parser.getActive(), active);
            }
            QCOMPARE(parser.getActive().isEmpty(), active.isEmpty());
            break;
        case DATAPOINT_METER:
            QCOMPARE(parser.isAlarm(), isAlarm);
            QCOMPARE(parser.getValue(), value);
            break;
        case DATAPOINT_NOT_VALID:
        default :
            QFAIL("Not a valid type to test..");
            break;
    }

}

QTEST_MAIN(TestDataParser)
#include "TestDataParser.moc"
