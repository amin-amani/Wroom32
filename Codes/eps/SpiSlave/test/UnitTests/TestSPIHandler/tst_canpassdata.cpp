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
private slots:
    void initTestCase();
    void cleanupTestCase();
    void Test_SPIPacket();

    void Test_WIFIPacket();
};

CanPassData::CanPassData()
{

}

CanPassData::~CanPassData()
{

}

void CanPassData::initTestCase()
{

    handler=CreateSpiHandler();
}

void CanPassData::cleanupTestCase()
{

}
void CanPassData::spiSendCallback(char*data,int len)
{
qDebug()<<"callback";
CalledFunction="SPI";
}
void CanPassData::WIFISendCallback(char*data,int len)
{
qDebug()<<"callback";
CalledFunction="WIFI";
}

void CanPassData::Test_WIFIPacket()
{
      //QSKIP("This mkacro cause skip test");
char packet[20]={0x7e,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
handler->SPIDataReadyCallback=spiSendCallback;
handler->WIFIDataReadyCallback=WIFISendCallback;
SpiHandlerInit(handler);
CalledFunction="";

handler->ProcessData(handler,packet,2);
QVERIFY(CalledFunction=="WIFI");

}
void CanPassData::Test_SPIPacket()
{
char packet[20]={0x7e,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
handler->SPIDataReadyCallback=spiSendCallback;
handler->WIFIDataReadyCallback=WIFISendCallback;
SpiHandlerInit(handler);
CalledFunction="";
handler->ProcessData(handler,packet,2);
QVERIFY(CalledFunction=="SPI");

}

QTEST_APPLESS_MAIN(CanPassData)

#include "tst_canpassdata.moc"
