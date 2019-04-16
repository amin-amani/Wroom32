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
void TCPServerInit(void (*SpiSendFunctionCallback)(uint8_t*,uint8_t*,int ) )
{
SpiSendFunction=SpiSendFunctionCallback;
}
void SendTCP(char*data,int len)
{
SPIPacketType packet;
uint8_t rxd[20];
packet.StartOfPacket=0x7e;
packet.EndOfPacket=0x7e;
packet.Command=TCPSendDataCommand;
packet.Datalen=len;
memcpy(packet.Data,data,len);
SpiSendFunction((uint8_t*)&packet,rxd,sizeof (packet));
}


void SetSSID(char*ssid)
{
 uint8_t txd[20]={0x7e,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t rxd[20];

memcpy(txd+4,ssid,strlen(ssid));
txd[3]=strlen(ssid);
SpiSendFunction(txd,rxd,sizeof(txd));


}

void SetPassword(char*pass)
{
 uint8_t txd[20]={0x7e,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t rxd[20];
memcpy(txd+4,pass,strlen(pass));
txd[3]=strlen(pass);
SpiSendFunction(txd,rxd,sizeof(txd));

}

void StartAp()
{
 uint8_t txd[20]={0x7e,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t rxd[20];


SpiSendFunction(txd,rxd,20);

}

#endif
