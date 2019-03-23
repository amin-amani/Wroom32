

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
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SPI.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

static const int spiClk = 500000; // 1 MHz
//static const int spiClk = 1000000; // 1 MHz
//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
WiFiServer server(80);
 char pass[30];
  char ssid[20]; 
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
byte GetSSID(byte *response)
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
  return 0;
    }
  packetLen=SpiSendByte(0xff);
  delay(100);
  //sprintf(temp,"packet len=%d \n",packetLen);
  //Serial.println(temp);
  
  if(packetLen>=100 || packetLen<3){Serial.println("invalid len\n");return 0;}
  SpiSendByte(0xff);//read command
  delay(100);

  SpiReadBytes(response,packetLen-4);
  response[packetLen-4]=0;

    
 return 1;
  }  
//=====================================================================================
byte  GetWifiPassword(byte *response)
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
  return 0;
    }
  packetLen=SpiSendByte(0xff);
  delay(100);
  //sprintf(temp,"packet len=%d \n",packetLen);
  //Serial.println(temp);
  
  if(packetLen>=100 || packetLen<3){Serial.println("invalid len\n");return 0;}
  SpiSendByte(0xff);//read command
  delay(100);

  SpiReadBytes(response,packetLen-4);
  response[packetLen-4]=0;
  return 1;
  
  }
//=====================================================================================
byte WaitForWifiSettings()
{

 
 if(!GetSSID((byte*)ssid))return 0;
 if(!GetWifiPassword((byte*)pass))return 0;
 return 1;
 
  
}
//=====================================================================================

void WIFIInit()
{
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, pass);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");  
  
  }

//=====================================================================================

void WIFIClientHandler()
{
  
    WiFiClient client = server.available();   // listen for incoming clients
int ledFlag=1;
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    digitalWrite(LED_BUILTIN, HIGH); 
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        SpiSendByte(c);
        ledFlag=!ledFlag;
        digitalWrite(LED_BUILTIN, ledFlag);              

      }//if client avaliable
    }//while client connected
    // close the connection:
    client.stop();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Client Disconnected.");
  }
  
  
  }

  
//=====================================================================================

void WIFIClientHandler2()
{
  
    WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  
  
  }
  
//=====================================================================================
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
  SpiInit();
  while(!WaitForWifiSettings())
  {
  delay(1000);
  }

  Serial.println("wifi setings completed\n");
  Serial.println(ssid);
  Serial.println(pass);
    WIFIInit();
 

}
//=====================================================================================
// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
WIFIClientHandler();
  //free ssid;
  //sprintf(temp,"ssid=%s\n",(char*)ssid);


}
