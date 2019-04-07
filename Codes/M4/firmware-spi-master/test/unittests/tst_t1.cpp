#include <QtTest>
#include "../../src/Calculator.c"
//#include "../../include/Calculator.h"

// add necessary includes here

class t1 : public QObject
{
    Q_OBJECT

public:
    t1();
    ~t1();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

t1::t1()
{

}

t1::~t1()
{

}

void t1::initTestCase()
{

}

void t1::cleanupTestCase()
{

}

void t1::test_case1()
{
QVERIFY(Add(1,2)==3);
}

QTEST_APPLESS_MAIN(t1)

#include "tst_t1.moc"
