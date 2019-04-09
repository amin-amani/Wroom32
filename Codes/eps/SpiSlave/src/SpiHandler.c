#include "SpiHandler.h"
#include <stdlib.h>
//==============================================================================
void DataReceived(SpiHandler * self,char*data,int len)
{
char dt[2];
if(len>0) self->SendToSPI(dt,2);
}

 //==============================================================================
 SpiHandler* CreateSpiHandler(void (*callback)(char*data,int len) ) 
 {
   SpiHandler* result = (SpiHandler*) malloc(sizeof(SpiHandler));
   //SpiHandler__init(result, x, y);
  result->SendToSPI=callback;
   return result;

}
//==============================================================================
