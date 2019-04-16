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

    static void spiSendCallback(char *data, int len);
    static void WIFISendCallback(char *data, int len);
    static void WIFIReadCallback(char *data, int len);
    static void WIFISetSSIDCallback(char *data, int len);
    static void WIFISetPasswordCallback(char *data, int len);
    static void WIFIStartApCallback(char *data, int len);
public slots:

private slots:
    void initTestCase();
    void cleanupTestCase();
    void Test_SPIPacket();

    void Test_WIFISendPacket();
    void Test_WIFIReadPacket();
    void Test_SetSSIDPacket();
    void Test_SetPassPacket();
    void Test_StartApPacket();

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
    handler->SPISendDataCallback=spiSendCallback;
    handler->WIFISendDataCallback=WIFISendCallback;
    handler->WIFIReadDataCallback=WIFIReadCallback;
    handler->WIFISetSSIDCallback=WIFISetSSIDCallback;
    handler->WIFISetPasswordCallback=WIFISetPasswordCallback;
    handler->WIFIStartApCallback=WIFIStartApCallback;
}
//===============================================================================================

void CanPassData::cleanupTestCase()
{

}
//===============================================================================================

void CanPassData::spiSendCallback(char*data,int len)
{
qDebug()<<"callback";
CalledFunction="SPI";
}
//===============================================================================================

void CanPassData::WIFISendCallback(char*data,int len)
{
qDebug()<<"callback";
CalledFunction="WIFI";
}
//===============================================================================================

void CanPassData::WIFIReadCallback(char*data,int len)
{
qDebug()<<"callback";
CalledFunction="wifiread";
}
//===============================================================================================

void CanPassData::WIFISetSSIDCallback(char *data, int len)
{
    qDebug()<<"callback";
    CalledFunction="setssid";
}
//===============================================================================================

void CanPassData::WIFISetPasswordCallback(char *data, int len)
{
    qDebug()<<"callback";
    CalledFunction="setpass";
}
//===============================================================================================

void CanPassData::WIFIStartApCallback(char *data, int len)
{
    qDebug()<<"callback";
    CalledFunction="startap";
}
//===============================================================================================
void CanPassData::Test_WIFISendPacket()
{
//QSKIP("This macro caus skip test");
char packet[20]={0x7e,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,2);
QVERIFY(CalledFunction=="WIFI");

}
//===============================================================================================
void CanPassData::Test_WIFIReadPacket()
{
      //QSKIP("This macro caus skip test");
char packet[20]={0x7e,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,2);
QVERIFY(CalledFunction=="wifiread");

}
//===============================================================================================

void CanPassData::Test_SetSSIDPacket()
{
    char packet[20]={0x7e,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="setssid");
}
//===============================================================================================

void CanPassData::Test_SetPassPacket()
{
    char packet[20]={0x7e,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="setpass");
}
//===============================================================================================

void CanPassData::Test_StartApPacket()
{
    char packet[20]={0x7e,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    SpiHandlerInit(handler);
    CalledFunction="";
    handler->ProcessSPIData(handler,packet,2);
    QVERIFY(CalledFunction=="startap");
}
//===============================================================================================
void CanPassData::Test_SPIPacket()
{
char packet[20]={0x7e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessSPIData(handler,packet,2);
QVERIFY(CalledFunction=="SPI");

}
//===============================================================================================

QTEST_APPLESS_MAIN(CanPassData)

#include "tst_canpassdata.moc"
