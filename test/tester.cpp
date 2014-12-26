#include <QtTest/QtTest>

class Tester: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};

void Tester::toUpper()
{
    QString str = "Hello";
    //QVERIFY(str.toUpper() == "HELLO");
    QCOMPARE(str.toUpper(), QString("HELLO"));  // QCOMPARE has more details
}

QTEST_MAIN(Tester)
#include "tester.moc"
