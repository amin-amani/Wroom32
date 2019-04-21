#include "../include/SpiHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifdef TESTUNIT
#include<QDebug>
#include<QByteArray>
#endif


SPIPacketType *packet;
SPIPacketType StatusPacket;


uint8_t CalcCRC(uint8_t *data,int len)
{
   uint8_t sum=0;
   int i;
   for(i=0;i<len;i++){

       sum^=data[i];
   }
   return sum;
}

//==============================================================================
void StartProcessData(SpiHandler * self,uint8_t *data,int len)
{
    packet=(SPIPacketType*)data;

    
    if(packet->Command<NUMBER_OF_FUNCTIONS){
        
     self->LastError=self->FunctionList[packet->Command](packet->Data,packet->Datalen);
     StatusPacket.sum= CalcCRC((uint8_t*)StatusPacket.Data,12);

     if(packet->Command==7 || self->LastCommand==7){
        self->LastCommand=packet->Command;
        StatusPacket.Command=self->LastCommand;
        return;
    }
    StatusPacket.Data[0]=self->LastError;
    self->LastCommand=packet->Command;
    StatusPacket.Command=self->LastCommand;

    }
}
//==============================================================================
SpiHandler* CreateSpiHandler()
{
    SpiHandler* result = (SpiHandler*) malloc(sizeof(SpiHandler));
    result->ProcessSPIData=StartProcessData;
    return result;
}
//==============================================================================
uint8_t SPIGetStatus(uint8_t*data,int len)
{

return 0;
}
//==============================================================================
uint8_t WIFIRead(uint8_t*data,int len)
{


memcpy(&StatusPacket.Data[0],&WIFIReceivedPacket.Data[0],sizeof(StatusPacket.Data));
return 0;
}
//==============================================================================
uint8_t SPIPrintFunction(uint8_t*data,int len)
{

printf("%s",data);
return 0;
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
    self->FunctionList[6]=SPIGetStatus;
    self->FunctionList[7]=WIFIRead;
     self->FunctionList[8]=SPIPrintFunction;
    StatusPacket.StartOfPacket=0x7e;
    StatusPacket.EndOfPacket=0x7e;
    StatusPacket.Datalen=0xff;



}
