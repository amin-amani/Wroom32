#ifndef __SPI__HANDLER
#define __SPI__HANDLER
#include <stdint.h>

typedef struct 
{
  
  uint8_t StartOfPacket;
  uint8_t Command;
  uint8_t FrameNumber;
  uint8_t Datalen;
  char Data[12];
  uint8_t CRC;
  uint8_t EndOfPacket;
   
}SPIPacketType;

typedef struct SpiHandler SpiHandler;

 struct  SpiHandler {

   void (*FunctionList[6])(char *data,int len);
   void (*SPISendDataCallback)(char*data,int len);
   void (*WIFISendDataCallback)(char*data,int len);//event to send wifi
   void (*WIFIReadDataCallback)(char*data,int len);
   void (*WIFISetSSIDCallback)(char*data,int len);
   void (*WIFISetPasswordCallback)(char*data,int len);
   void (*WIFIStartApCallback)(char*data,int len);
   void (*ProcessSPIData)(SpiHandler * self,char*data,int len);
   
};
//struct SpiHandler;  // forward declared for encapsulation

// SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len));  // equivalent to "new Point(x, y)"
 SpiHandler* CreateSpiHandler();
 void SpiHandlerInit(SpiHandler*self);
#endif
