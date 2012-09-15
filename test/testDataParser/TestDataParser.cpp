#include <QtCore>
#include <QtTest>

#include "RoomTemperatureDataParser.h" 

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
    QTest::addColumn<bool>("isAlarm");
    QTest::addColumn<QString>("value");

    QTest::newRow("normal data 01") << "temperature=54.34" << false << "54.34";
    QTest::newRow("normal data 02") << "temperature=22.3"  << false << "22.3";
    QTest::newRow("normal data 03") << "temperature=9.0"   << false << "9.0";
    QTest::newRow("normal data 04") << "temperature=-5.5"  << false << "-5.5";
    QTest::newRow("normal data 05") << "temperature=-15.4" << false << "-15.4";

    QTest::newRow("Alarm 01") 
        << "Alarm: High temperature=26.9 level=25.0" << true 
        << "High temperature=26.9 level=25.0";
    QTest::newRow("Alarm 02") 
        << "Alarm: Low temperature=21.9 level=22.0" << true 
        << "Low temperature=21.9 level=22.0";
}

void TestDataParser::testParse()
{
    QFETCH(QString,data);
    QFETCH(bool,isAlarm);
    QFETCH(QString,value);

    qDebug() << data << isAlarm << value;
    //QCOMPARE(sensor.isOK(), false);

    RoomTemperatureDataParser parser(data);

    QCOMPARE(parser.parse(), true);
    QCOMPARE(parser.isAlarm(), isAlarm);
    QCOMPARE(parser.getValue(), value);

}

QTEST_MAIN(TestDataParser)
#include "TestDataParser.moc"
