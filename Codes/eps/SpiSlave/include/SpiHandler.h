#ifndef __SPI__HANDLER
#define __SPI__HANDLER
#include <stdint.h>


#define NUMBER_OF_FUNCTIONS 9

#ifndef THIS_TYPE
#define THIS_TYPE

typedef struct 
{
  
  uint8_t StartOfPacket;
  uint8_t Command;
  uint8_t FrameNumber;
  uint8_t Datalen;
  uint8_t Data[12];
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

   uint8_t (*FunctionList[NUMBER_OF_FUNCTIONS])(uint8_t*data,int len);
   uint8_t (*SPISendDataCallback)(uint8_t*data,int len);
   uint8_t (*WIFISendDataCallback)(uint8_t*data,int len);//event to send wifi
   uint8_t (*WIFIReadDataCallback)(uint8_t*data,int len);
   uint8_t (*WIFISetSSIDCallback)(uint8_t*data,int len);
   uint8_t (*WIFISetPasswordCallback)(uint8_t*data,int len);
   uint8_t (*WIFIStartApCallback)(uint8_t*data,int len);
   void (*ProcessSPIData)(SpiHandler * self,uint8_t*data,int len);
 //  uint8_t SPIGetStatus(SpiHandler * self,char*data,int len);
   
};
//struct SpiHandler;  // forward declared for encapsulation

// SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len));  // equivalent to "new Point(x, y)"
 SpiHandler* CreateSpiHandler();
 void SpiHandlerInit(SpiHandler*self);
#endif
