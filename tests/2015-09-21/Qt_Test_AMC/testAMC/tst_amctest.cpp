#include <QString>
#include <QtTest>

class AMCTest : public QObject
{
    Q_OBJECT

public:
    AMCTest();

private Q_SLOTS:
    void testCase1();
};

AMCTest::AMCTest()
{
}

void AMCTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(AMCTest)

#include "tst_amctest.moc"
