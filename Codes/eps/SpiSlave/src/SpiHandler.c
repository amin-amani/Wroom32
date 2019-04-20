#include "../include/SpiHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


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
void StartProcessData(SpiHandler * self,char*data,int len)
{
//qDebug()<<"ps data:";
    packet=(SPIPacketType*)data;
    //SPIPacketType *packet=(SPIPacketType*)data;

    if(packet->Command<NUMBER_OF_FUNCTIONS){
    self->LastError=self->FunctionList[packet->Command](packet->Data,packet->Datalen);
    self->LastCommand=packet->Command;

    if(self->LastCommand==7)
    {

//            qDebug()<<"sender sum="<<packet->sum;
//             qDebug()<<"sender data="<<QByteArray::fromRawData(packet->Data,12).toHex();
//        packet->sum= CalcSum((uint8_t*)packet->Data,12);

//       qDebug()<<"sender data="<<QByteArray::fromRawData(packet->Data,12).toHex();

        StatusPacket.sum= CalcCRC((uint8_t*)StatusPacket.Data,12);

      // qDebug()<<"sender data="<<QByteArray::fromRawData(StatusPacket.Data,12).toHex();
     //  qDebug()<<"sender crc="<<StatusPacket.sum;
       return;
    }
    StatusPacket.Command=self->LastCommand;
    StatusPacket.Data[0]=(char)self->LastError;

    StatusPacket.sum= CalcCRC((uint8_t*)StatusPacket.Data,12);

  // qDebug()<<"sender data="<<QByteArray::fromRawData(StatusPacket.Data,12).toHex();
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
uint8_t SPIGetStatus(char*data,int len)
{
return 0;
}
//==============================================================================
uint8_t WIFIRead(char*data,int len)
{

memcpy((char*)&StatusPacket.Data[0],(char*)&WIFIReceivedPacket.Data[0],sizeof(SPIPacketType));

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
    StatusPacket.StartOfPacket=0x7e;
    StatusPacket.EndOfPacket=0x7e;
    StatusPacket.Datalen=0xff;



}
