#ifndef __TCPSERVER_H
#define __TCPSERVER_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef TESTUNIT
#include<qdebug.h>
#endif

#define   TCPSendDataCommand 1
#define   TCPReadDataCommand 2
#define   WIFISSIDCommand 3
#define   WIFIPasswordCommand 4
#define   WIFIStartAPCommand 5
#define   WIFITCPReadCommand 7
#define   SPIPrintCommand 8
#define START_END_FRAME 0x7e
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
//amin@amin-pc:~/Desktop/Wroom32/Codes/M4/firmware-spi-master$ sudo stm32flash -b 115200 -w /home/amin/Desktop/Wroom32/Codes/M4/firmware-spi-master/.pioenvs/black_f407vg/firmware.bin /dev/ttyUSB0 -v
 void (*SpiSendFunction)(uint8_t*,uint8_t*,int );


 //================================================================================================

 uint8_t CalcSum(uint8_t *data,int len)
 {
    uint8_t sum=0;
    int i;
    for(i=0;i<len;i++){

        sum^=data[i];
    }
    return sum;
 }

 //================================================================================================
 uint8_t GetStatus(uint8_t command)
 {
     uint8_t empty[20];
     uint8_t rxd[20];
     memset(empty,0,sizeof(empty));
     empty[1]=6;
     SpiSendFunction(empty,rxd,sizeof (SPIPacketType));
     if(rxd[1]!=command)return 1;
     return rxd[4];
 }
 //=========================================================================================
void TCPServerInit(void (*SpiSendFunctionCallback)(uint8_t*,uint8_t*,int ) )
{
SpiSendFunction=SpiSendFunctionCallback;
}
//=========================================================================================
uint8_t SendTCP(char*data,uint8_t len)
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=TCPSendDataCommand;
    packet.Datalen=(len<=sizeof (packet.Data))?len:sizeof (packet.Data);
    memset(packet.Data,0,sizeof (packet.Data));
    memcpy(packet.Data,data,packet.Datalen);
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    return GetStatus(TCPSendDataCommand);
}
//=========================================================================================
uint8_t ReadTCP(char*data,uint8_t len)
{
    SPIPacketType packet;
    uint8_t empty[20];
    uint8_t rxd[20];
     uint8_t response[20];

    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=7;
    packet.Datalen=(len<=sizeof (packet.Data))?len:sizeof (packet.Data);
    memset(packet.Data,0,sizeof (packet.Data));
    memcpy(packet.Data,data,packet.Datalen);
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    memset(empty,0,sizeof(empty));
    empty[1]=6;
    SpiSendFunction(empty,response,sizeof (SPIPacketType));
    memcpy(data,&response[4],12);
    uint8_t sum=CalcSum((uint8_t*) data,12);
    //qDebug()<<"respose data="<<QByteArray::fromRawData((char*)response,20).toHex();
    //qDebug()<<"reciver sum="<<sum<<" d="<<QString::number(response[sizeof(SPIPacketType)-2]&0xff,10);

    if(sum==response[sizeof(SPIPacketType)-2])return 0;
   return 1;


}
//=========================================================================================
uint8_t SetSSID(char*ssid)
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=WIFISSIDCommand;
    packet.Datalen=(strlen(ssid)<=sizeof (packet.Data))?strlen(ssid):sizeof (packet.Data);
    memset(packet.Data,0,sizeof (packet.Data));
    memcpy(packet.Data,ssid,packet.Datalen);
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    return GetStatus(WIFISSIDCommand);
}

//=========================================================================================
uint8_t SetPassword(char*pass)
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=WIFIPasswordCommand;
    packet.Datalen=(strlen(pass)<=sizeof (packet.Data))?strlen(pass):sizeof (packet.Data);
    memset(packet.Data,0,sizeof (packet.Data));
    memcpy(packet.Data,pass,packet.Datalen);
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    return GetStatus(WIFIPasswordCommand);
}
//=========================================================================================
uint8_t SPIPrint(char*message)
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=SPIPrintCommand;
    packet.Datalen=(strlen(message)<=sizeof (packet.Data))?strlen(message):sizeof (packet.Data);
    memset(packet.Data,0,sizeof (packet.Data));
    memcpy(packet.Data,message,packet.Datalen);
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    return 0;
//    return GetStatus(WIFIPasswordCommand);
}
//=========================================================================================
uint8_t StartAp()
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=WIFIStartAPCommand;
    packet.Datalen=0;
    memset(packet.Data,0,sizeof (packet.Data));
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
    return GetStatus(WIFIStartAPCommand);
}
//=========================================================================================
#endif
