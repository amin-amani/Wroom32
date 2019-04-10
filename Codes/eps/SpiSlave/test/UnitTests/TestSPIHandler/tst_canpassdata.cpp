#include <QtTest>
#include "../../../include/SpiHandler.h"
#include "../../../src/SpiHandler.c"

// add necessary includes here

class CanPassData : public QObject
{
    Q_OBJECT

public:
    CanPassData();
    ~CanPassData();

    static void callback(char *data, int len);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

CanPassData::CanPassData()
{

}

CanPassData::~CanPassData()
{

}

void CanPassData::initTestCase()
{

}

void CanPassData::cleanupTestCase()
{

}
void CanPassData::callback(char*data,int len)
{
qDebug()<<"callback";

}
void CanPassData::test_case1()
{
SpiHandler *handler;
handler=CreateSpiHandler(callback);

DataReceived(handler,"somting",2);


}

QTEST_APPLESS_MAIN(CanPassData)

#include "tst_canpassdata.moc"
