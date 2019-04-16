#include "../include/SpiHandler.h"
#include <stdlib.h>

SPIPacketType *packet;


//==============================================================================
void StartProcessData(SpiHandler * self,char*data,int len)
{

    SPIPacketType *packet=(SPIPacketType*)data;
    if(packet->Command<6)
    self->FunctionList[packet->Command](packet->Data,packet->Datalen);

}
//==============================================================================
SpiHandler* CreateSpiHandler()
{
    SpiHandler* result = (SpiHandler*) malloc(sizeof(SpiHandler));
    result->ProcessSPIData=StartProcessData;
    return result;
}

 //==============================================================================
void SpiHandlerInit(SpiHandler * self)
{
    self->FunctionList[0]=self->SPISendDataCallback;
    self->FunctionList[1]=self->WIFISendDataCallback;
    self->FunctionList[2]=self->WIFIReadDataCallback;
    self->FunctionList[3]=self->WIFISetSSIDCallback;
    self->FunctionList[4]=self->WIFISetPasswordCallback;
    self->FunctionList[5]=self->WIFIStartApCallback;
}
