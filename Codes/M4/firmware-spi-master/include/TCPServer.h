#ifndef __TCPSERVER_H
#define __TCPSERVER_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define   TCPSendDataCommand 1
#define   TCPReadDataCommand 2
#define   WIFISSIDCommand 3
#define   WIFIPasswordCommand 4
#define   WIFIStartAPCommand 5
#define START_END_FRAME 0x7e
    
typedef struct 
{
  
  uint8_t StartOfPacket;
  uint8_t Command;
  uint8_t FrameNumber;
  uint8_t Datalen;
  char Data[12];
  uint8_t Cksum;
  uint8_t EndOfPacket;
   
}SPIPacketType;

//amin@amin-pc:~/Desktop/Wroom32/Codes/M4/firmware-spi-master$ sudo stm32flash -b 115200 -w /home/amin/Desktop/Wroom32/Codes/M4/firmware-spi-master/.pioenvs/black_f407vg/firmware.bin /dev/ttyUSB0 -v
 void (*SpiSendFunction)(uint8_t*,uint8_t*,int );
//=========================================================================================
void TCPServerInit(void (*SpiSendFunctionCallback)(uint8_t*,uint8_t*,int ) )
{
SpiSendFunction=SpiSendFunctionCallback;
}
//=========================================================================================
void SendTCP(char*data,uint8_t len)
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
}
//=========================================================================================
void SetSSID(char*ssid)
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
}
//=========================================================================================
void SetPassword(char*pass)
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
}
//=========================================================================================
void StartAp()
{
    SPIPacketType packet;
    uint8_t rxd[20];
    packet.StartOfPacket=START_END_FRAME;
    packet.EndOfPacket=START_END_FRAME;
    packet.Command=WIFIStartAPCommand;
    packet.Datalen=0;
    memset(packet.Data,0,sizeof (packet.Data));
    SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
}
//=========================================================================================
#endif
