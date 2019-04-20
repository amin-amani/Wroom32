#ifndef __SPI__HANDLER
#define __SPI__HANDLER
#include <stdint.h>


#define NUMBER_OF_FUNCTIONS 8

#ifndef THIS_TYPE
#define THIS_TYPE

typedef struct 
{
  
  uint8_t StartOfPacket;
  uint8_t Command;
  uint8_t FrameNumber;
  uint8_t Datalen;
  char Data[12];
  uint8_t sum;
  uint8_t EndOfPacket;
   
}SPIPacketType;

#endif
extern SPIPacketType StatusPacket;

typedef struct SpiHandler SpiHandler;
SPIPacketType WIFIReceivedPacket;

 struct  SpiHandler {
    uint8_t LastCommand;
    uint8_t LastError;

   uint8_t (*FunctionList[NUMBER_OF_FUNCTIONS])(char *data,int len);
   uint8_t (*SPISendDataCallback)(char*data,int len);
   uint8_t (*WIFISendDataCallback)(char*data,int len);//event to send wifi
   uint8_t (*WIFIReadDataCallback)(char*data,int len);
   uint8_t (*WIFISetSSIDCallback)(char*data,int len);
   uint8_t (*WIFISetPasswordCallback)(char*data,int len);
   uint8_t (*WIFIStartApCallback)(char*data,int len);
   void (*ProcessSPIData)(SpiHandler * self,char*data,int len);
 //  uint8_t SPIGetStatus(SpiHandler * self,char*data,int len);
   
};
//struct SpiHandler;  // forward declared for encapsulation

// SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len));  // equivalent to "new Point(x, y)"
 SpiHandler* CreateSpiHandler();
 void SpiHandlerInit(SpiHandler*self);
#endif
