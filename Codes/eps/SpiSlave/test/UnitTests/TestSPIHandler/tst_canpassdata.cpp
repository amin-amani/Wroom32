#include <QtTest>
#include "../../../include/SpiHandler.h"
#include "../../../src/SpiHandler.c"

// add necessary includes here
QString CalledFunction="";
class CanPassData : public QObject
{
    Q_OBJECT
int val=0;
public:
    SpiHandler *handler;
    CanPassData();
    ~CanPassData();

    static uint8_t spiSendEvent(uint8_t *data, int len);
    static uint8_t WIFISendEvent(uint8_t *data, int len);
    static uint8_t WIFIReadEvent(uint8_t *data, int len);
    static uint8_t WIFISetSSIDEvent(uint8_t *data, int len);
    static uint8_t WIFISetPasswordEvent(uint8_t *data, int len);
    static uint8_t WIFIStartApEvent(uint8_t *data, int len);



public slots:

private slots:
    void initTestCase();
    void cleanupTestCase();
    void Test_WIFISendPacket();
    void Test_SPIPacket();
    void Test_SetSSIDPacket();
    void Test_SetPassPacket();
    void Test_StartApPacket();

    void Test_WIFIReadPacket();

};
//===============================================================================================

CanPassData::CanPassData()
{

}
//===============================================================================================

CanPassData::~CanPassData()
{

}
//===============================================================================================

void CanPassData::initTestCase()
{

    handler=CreateSpiHandler();
    handler->SPISendDataCallback=spiSendEvent;
    handler->WIFISendDataCallback=WIFISendEvent;
    handler->WIFIReadDataCallback=WIFIReadEvent;
    handler->WIFISetSSIDCallback=WIFISetSSIDEvent;
    handler->WIFISetPasswordCallback=WIFISetPasswordEvent;
    handler->WIFIStartApCallback=WIFIStartApEvent;
}
//===============================================================================================

void CanPassData::cleanupTestCase()
{

}
//===============================================================================================

uint8_t CanPassData::spiSendEvent(uint8_t*data,int len)
{
//qDebug()<<"callback";
CalledFunction="SPI";
}
//===============================================================================================

uint8_t CanPassData::WIFISendEvent(uint8_t*data,int len)
{
//qDebug()<<"callback";
CalledFunction="WIFI";
}
//===============================================================================================

uint8_t CanPassData::WIFIReadEvent(uint8_t*data,int len)
{
//qDebug()<<"callback";
CalledFunction="wifiread";
}
//===============================================================================================

uint8_t CanPassData::WIFISetSSIDEvent(uint8_t *data, int len)
{
    //qDebug()<<"callback";
    CalledFunction="setssid";
}
//===============================================================================================

uint8_t CanPassData::WIFISetPasswordEvent(uint8_t *data, int len)
{
   // qDebug()<<"callback";
    CalledFunction="setpass";
}
//===============================================================================================

uint8_t CanPassData::WIFIStartApEvent(uint8_t *data, int len)
{

   // qDebug()<<"callback";
    CalledFunction="startap";
}
//===============================================================================================
void CanPassData::Test_WIFISendPacket()
{
//QSKIP("This macro caus skip test");
uint8_t packet[20]={0x7e,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,2);
QVERIFY(CalledFunction=="WIFI");

}

//===============================================================================================

void CanPassData::Test_SetSSIDPacket()
{
    uint8_t packet[20]={0x7e,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="setssid");
}
//===============================================================================================

void CanPassData::Test_SetPassPacket()
{
    uint8_t packet[20]={0x7e,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="setpass");
}
//===============================================================================================

void CanPassData::Test_StartApPacket()
{
    uint8_t packet[20]={0x7e,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="startap");
}
//===============================================================================================
void CanPassData::Test_SPIPacket()
{
uint8_t packet[20]={0x7e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,2);
QVERIFY(CalledFunction=="SPI");

}
//===============================================================================================
void CanPassData::Test_WIFIReadPacket()
{
      //QSKIP("This macro caus skip test");
    memcpy(WIFIReceivedPacket.Data,"itswifi data",12);
uint8_t packet[18]={0x7e,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x7e};
uint8_t readStatusRequest[18]={0x7e,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x7e};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,18);
handler->ProcessSPIData(handler,readStatusRequest,18);
qDebug()<< "status packet="<<QByteArray::fromRawData((char*) StatusPacket.Data,12);
QVERIFY(strncmp("itswifi data",(char*)StatusPacket.Data,12)==0);

}
//===============================================================================================

QTEST_APPLESS_MAIN(CanPassData)

#include "tst_canpassdata.moc"
