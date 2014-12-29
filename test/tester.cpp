#include <QtTest/QtTest>

class Tester: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
    void toLower();
};

void Tester::toUpper()
{
    QString str = "Hello";
    //QVERIFY(str.toUpper() == "HELLO");
    QCOMPARE(str.toUpper(), QString("HELLO"));  // QCOMPARE has more details
}

void Tester::toLower()
{
    QString str = "Hello";
    QVERIFY(str.toLower() == "hello");
}

QTEST_MAIN(Tester)
#include "tester.moc"
