#include <QtTest>
#include <QDebug>
#include "../../../include/TCPServer.h"
#include "../../../../../eps/SpiSlave/include/SpiHandler.h"
#include "../../../../../eps/SpiSlave/src/SpiHandler.c"

// add necessary includes here
//================================================================================================

class CommunicationTest : public QObject
{
    Q_OBJECT
public:


    CommunicationTest();
    ~CommunicationTest();
    static void SPIEvent(uint8_t *tx, uint8_t *rx, int len);
    static uint8_t WIFISetPasswordEvent(char*data,int len);
    static uint8_t WIFISetSSIDEvent(char*data,int len);
    static uint8_t WIFIStartAPEvent(char*data,int len);
    static uint8_t WIFISendTCPEvent(char*data,int len);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_SetPassword();
    void test_SetSSID();
    void test_StartAP();
    void test_SendTCP();
    void test_ReadTCP();

signals:
    void DataReady();


};
//================================================================================================

CommunicationTest::CommunicationTest()
{

}
//================================================================================================

CommunicationTest::~CommunicationTest()
{

}
//================================================================================================
void CommunicationTest::SPIEvent(uint8_t *tx, uint8_t *rx, int len)
{
//qDebug()<<"spi call";
SpiHandler *esp32;
esp32=CreateSpiHandler();
esp32->WIFISetPasswordCallback=WIFISetPasswordEvent;
esp32->WIFISetSSIDCallback=WIFISetSSIDEvent;
esp32->WIFIStartApCallback=WIFIStartAPEvent;
esp32->WIFISendDataCallback=WIFISendTCPEvent;
SpiHandlerInit(esp32);
memcpy(rx,&StatusPacket,sizeof(SPIPacketType));
esp32->ProcessSPIData(esp32,(char*)tx,len);
free( esp32);
//qDebug()<<QByteArray::fromRawData((char*)rx,len).toHex();

}
//================================================================================================
uint8_t CommunicationTest::WIFISetPasswordEvent(char *data, int len)
{
qDebug()<<"esp set pass";

return 0;
}
//================================================================================================

uint8_t CommunicationTest::WIFISetSSIDEvent(char *data, int len)
{
    qDebug()<<"esp set ssid";

    return 0;
}
//================================================================================================

uint8_t CommunicationTest::WIFIStartAPEvent(char *data, int len)
{
    qDebug()<<"esp start ap";

    return 0;
}
//================================================================================================

uint8_t CommunicationTest::WIFISendTCPEvent(char *data, int len)
{
    qDebug()<<"esp send tcp";

    return 0;
}
//================================================================================================
void CommunicationTest::initTestCase()
{

}
//================================================================================================
void CommunicationTest::cleanupTestCase()
{

}
//================================================================================================
void CommunicationTest::test_SetPassword()
{
TCPServerInit(SPIEvent);
uint8_t error=SetPassword("PASSWORD");
qDebug()<<"and what:"<<error;
QVERIFY(error==0);

}
//================================================================================================

void CommunicationTest::test_SetSSID()
{
    TCPServerInit(SPIEvent);
    uint8_t error=SetSSID("SSID");
    qDebug()<<"and what:"<<error;
    QVERIFY(error==0);
}
//================================================================================================

void CommunicationTest::test_StartAP()
{
    TCPServerInit(SPIEvent);
    uint8_t error=StartAp();
    qDebug()<<"and what:"<<error;
    QVERIFY(error==0);
}
//================================================================================================

void CommunicationTest::test_SendTCP()
{
    TCPServerInit(SPIEvent);
    uint8_t error=SendTCP("salam",5);
    qDebug()<<"and what:"<<error;
    QVERIFY(error==0);
}
//================================================================================================

void CommunicationTest::test_ReadTCP()
{ char data[20];
    TCPServerInit(SPIEvent);
     uint8_t error=0;
     strcpy(WIFIReceivedPacket.Data,"abcdefghijkl");
     error=ReadTCP(data,sizeof(data));
     qDebug()<<data<<QByteArray::fromRawData(data,12).toHex();

    qDebug()<<"and what:"<<error<<QString::number(error,16);
    QVERIFY(error==0);
    QVERIFY(strncmp(data,"abcdefghijkl",12)==0);
}
//================================================================================================
QTEST_APPLESS_MAIN(CommunicationTest)

#include "tst_communicationtest.moc"
