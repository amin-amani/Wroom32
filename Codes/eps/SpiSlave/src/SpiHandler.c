#include "../include/SpiHandler.h"
#include <stdlib.h>

SPIPacketType *packet;


//==============================================================================
void StartProcessData(SpiHandler * self,char*data,int len)
{

    SPIPacketType *packet=(SPIPacketType*)data;
    if(packet->Command==1 || packet->Command==2)
    self->FunctionList[packet->Command-1](data,len);

}
//==============================================================================
SpiHandler* CreateSpiHandler()
{
    SpiHandler* result = (SpiHandler*) malloc(sizeof(SpiHandler));
    result->ProcessData=StartProcessData;


    return result;
}

 //==============================================================================
void SpiHandlerInit(SpiHandler * self)
{
    self->FunctionList[0]=self->SPIDataReadyCallback;
    self->FunctionList[1]=self->WIFIDataReadyCallback;
}
