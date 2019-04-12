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

   void (*FunctionList[2])(char *data,int len);
   void (*SPIDataReadyCallback)(char*data,int len);
   void (*WIFIDataReadyCallback)(char*data,int len);
   //void (*WIFIDataReadyCallback)(char*data,int len);
   void (*ProcessData)(SpiHandler * self,char*data,int len);
   
};
//struct SpiHandler;  // forward declared for encapsulation

// SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len));  // equivalent to "new Point(x, y)"
 SpiHandler* CreateSpiHandler();
 void SpiHandlerInit(SpiHandler*self);
#endif
