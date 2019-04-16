#include <QtTest>
#include <QDebug>
#include "../../../include/TCPServer.h"
// add necessary includes here
QByteArray TX,RX;
class TCPServerTest : public QObject
{
    Q_OBJECT

public:
    TCPServerTest();
    ~TCPServerTest();
    static void SPI(uint8_t*txd,uint8_t*rxd,int len)
    {
TX=QByteArray::fromRawData((const char*)txd,len);
        qDebug()<<"call"<<TX;

    }

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

    void test_SetSSID();
    void test_SendTCP();
};

TCPServerTest::TCPServerTest()
{

}

TCPServerTest::~TCPServerTest()
{

}

void TCPServerTest::initTestCase()
{
    TCPServerInit(SPI);

}

void TCPServerTest::cleanupTestCase()
{

}

void TCPServerTest::test_case1()
{
SetPassword("amin");
qDebug()<<QString::fromLatin1(TX);

}
void TCPServerTest::test_SetSSID()
{
    TX.clear();
SetSSID("wifiname");
qDebug()<<TX.toHex();

}
void TCPServerTest::test_SendTCP()
{
    TX.clear();
SendTCP("tcpsend",20);
qDebug()<<TX.toHex();

}

QTEST_APPLESS_MAIN(TCPServerTest)

#include "tst_tcpservertest.moc"
