
#ifndef __SPI__HANDLER
#define __SPI__HANDLER

typedef struct  SpiHandler {
   int x;
   int y;
   
   void (*SendToSPI)(char*data,int len);
   void (*SendToWIFI)(char*data,int len);
   
}SpiHandler;
//struct SpiHandler;  // forward declared for encapsulation
void DataReceived(SpiHandler * self,char*data,int len);
 SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len));  // equivalent to "new Point(x, y)"
 
#endif