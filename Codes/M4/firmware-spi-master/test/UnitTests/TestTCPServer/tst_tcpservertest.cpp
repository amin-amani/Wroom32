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
    static void SPI(uint8_t*txd,uint8_t*rxd,int len);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_SetPassword();
    void test_SetBigPassword();
    void test_SetSSID();
    void test_SendTCP();

};

TCPServerTest::TCPServerTest()
{

}

TCPServerTest::~TCPServerTest()
{

}

void TCPServerTest::SPI(uint8_t *txd, uint8_t *rxd, int len)
{
    TX.append(QByteArray::fromRawData((const char*)txd,len));

}

void TCPServerTest::initTestCase()
{
    TCPServerInit(SPI);
    TX.clear();

}

void TCPServerTest::cleanupTestCase()
{

}

void TCPServerTest::test_SetPassword()
{
    TX.clear();
    SetPassword("amin");
    SPIPacketType *responsePcket=(SPIPacketType*)(TX.data());
    QVERIFY(strncmp(responsePcket->Data,"amin",12)==0);
}

void TCPServerTest::test_SetBigPassword()
{
    TX.clear();
    SetPassword("1234567890ABCDEFG");
    SPIPacketType *responsePcket=(SPIPacketType*)(TX.data());
    QVERIFY(strncmp(responsePcket->Data,"1234567890AB",12)==0);
    QVERIFY(responsePcket->Datalen==12);



}
void TCPServerTest::test_SetSSID()
{
    TX.clear();
    SetSSID("wifiname");
    SPIPacketType *responsePcket=(SPIPacketType*)(TX.data());
    QVERIFY(strncmp(responsePcket->Data,"wifiname",12)==0);
    QVERIFY(responsePcket->Datalen==8);

}
void TCPServerTest::test_SendTCP()
{
    TX.clear();
    SendTCP("1234567890AB",12);
    SPIPacketType *responsePcket=(SPIPacketType*)(TX.data());
    //qDebug()<<responsePcket->Data;
    QVERIFY(strncmp(responsePcket->Data,"1234567890AB",12)==0);
    //QVERIFY(strcmp(responsePcket->Data,"1234567890AB")==0);

}

QTEST_APPLESS_MAIN(TCPServerTest)

#include "tst_tcpservertest.moc"
