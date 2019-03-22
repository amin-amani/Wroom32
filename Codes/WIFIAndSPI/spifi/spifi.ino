

/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 * 
 * Logic analyser capture is in the same folder as this example as
 * "multiple_bus_output.png"
 * 
 * created 30/04/2018 by Alistair Symonds
 */
#include <SPI.h>

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
//=====================================================================================
void SpiInit()
{
  vspi = new SPIClass(VSPI);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Start SpiTest");
  //alternatively route through GPIO pins of your choice
  // vspi->begin(0, 2, 4, 33); //SCLK, MISO, MOSI, SS
  vspi->begin(18, 19, 23, 5); //SCLK, MISO, MOSI, SS
  pinMode(5, OUTPUT); //VSPI SS  
  }
//=====================================================================================
byte SpiSendByte(byte data)
{
  byte result=0;
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  result=vspi->transfer(data);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
  return result;
  }
//=====================================================================================
 void SpiSendBytes(byte *data,byte*recivedData,int len)
{
  byte result=0;   
  for(int i=0;i<len;i++)
    { 
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer

  recivedData[i]=vspi->transfer(data[i]);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
    
  
  vspi->endTransaction();
  delay(80);
 }
  }
  //=====================================================================================
 void SpiReadBytes(byte*recivedData,int len)
{
  byte result=0;
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  for(int i=0;i<len;i++)
  recivedData[i]=vspi->transfer(0xff);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();

  }
//=====================================================================================
void GetSSID()
{
  byte packet[6]={0x7e,6,1,0,0,0x7e};
   byte response[100];
  byte answer[6];
  char temp[20];
  int packetLen=0;
  byte sof=0;
  SpiSendBytes(packet,answer,6);
  //delay(100);
 // sof=SpiSendByte(0xff);
//  if(0x7e!=sof)
//  {
//  sprintf(temp,"invalid=%d\n",sof);
//  Serial.println(temp);
//  return;
//    }
//  packetLen=SpiSendByte(0xff);
//  
//  sprintf(temp,"packet len=%d\n",packetLen);
//  Serial.println(temp);
//  SpiReadBytes(response,packetLen-3);
//   
   Serial.println("Read Ok...\n");
    
 
  }  
//=====================================================================================
void  GetWifiPassword()
{
  byte packet[6]={0x7e,6,2,0,0,0x7e};
  byte answer[6];
  SpiSendBytes(packet,answer,6);
 
  }
//=====================================================================================
void WaitForWifiSettings()
{

  
}
//=====================================================================================
void setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  SpiInit();
 // WaitForWifiSettings();

}
//=====================================================================================
// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
  GetSSID();
  delay(1000);
}
