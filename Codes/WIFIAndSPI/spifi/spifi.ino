

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

static const int spiClk = 500000; // 1 MHz
//static const int spiClk = 1000000; // 1 MHz
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
    //vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0)); 
  }
//=====================================================================================
byte SpiSendByte(byte data)
{
  byte result=0;   

      
digitalWrite(5, LOW); //pull SS slow to prep other end for transfer        
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  result=vspi->transfer(data);    
  delay(80);
    vspi->endTransaction(); 
     digitalWrite(5, HIGH); //pull ss high to signify end of data transfer


return result;

  }
//=====================================================================================
 void SpiSendBytes(byte *data,byte*recivedData,int len)
{
  byte result=0;   


  for(int i=0;i<len;i++)
    {  
      
digitalWrite(5, LOW); //pull SS slow to prep other end for transfer     

    
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

  recivedData[i]=vspi->transfer(data[i]);    
  delay(80);
    vspi->endTransaction(); 
     digitalWrite(5, HIGH); //pull ss high to signify end of data transfer


  } 

    
  

  

  }
  //=====================================================================================
 void SpiReadBytes(byte*recivedData,int len)
{
  byte result=0;
  for(int i=0;i<len;i++){
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  
  recivedData[i]=vspi->transfer(0xff);  
  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
    delay(80);
  }
  }
//=====================================================================================
void GetSSID(byte *response)
{
  byte packet[6]={0x7e,6,1,0,0,0x7e};

  byte answer[6];
  char temp[20];
  int packetLen=0;
  byte sof=0;
  SpiSendBytes(packet,answer,6);
  delay(100);
  sof=SpiSendByte(0xff);//dummy
  delay(100);
  sof=SpiSendByte(0xff);
  if(0x7e!=sof)
  {
  sprintf(temp,"invalid=%d\n",sof);
  Serial.println(temp);
  return;
    }
  packetLen=SpiSendByte(0xff);
  delay(100);
  //sprintf(temp,"packet len=%d \n",packetLen);
  //Serial.println(temp);
  
  if(packetLen>=100 || packetLen<3){Serial.println("invalid len\n");return;}
  SpiSendByte(0xff);//read command
  delay(100);

  SpiReadBytes(response,packetLen-4);
  response[packetLen-4]=0;

    
 
  }  
//=====================================================================================
void  GetWifiPassword(byte *response)
{
  byte packet[6]={0x7e,6,2,0,0,0x7e};

  byte answer[6];
  char temp[20];
  int packetLen=0;
  byte sof=0;
  SpiSendBytes(packet,answer,6);
  delay(100);
  sof=SpiSendByte(0xff);//dummy
  delay(100);
  sof=SpiSendByte(0xff);
  if(0x7e!=sof)
  {
  sprintf(temp,"invalid=%d\n",sof);
  Serial.println(temp);
  return;
    }
  packetLen=SpiSendByte(0xff);
  delay(100);
  //sprintf(temp,"packet len=%d \n",packetLen);
  //Serial.println(temp);
  
  if(packetLen>=100 || packetLen<3){Serial.println("invalid len\n");return;}
  SpiSendByte(0xff);//read command
  delay(100);

  SpiReadBytes(response,packetLen-4);
  response[packetLen-4]=0;

    
 
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
  char pass[30];
  char ssid[20]; 
  GetSSID((byte*)ssid);
  GetWifiPassword((byte*)pass);
  //free ssid;
  //sprintf(temp,"ssid=%s\n",(char*)ssid);
  Serial.println(ssid);
    Serial.println(pass);
  delay(1000);
}