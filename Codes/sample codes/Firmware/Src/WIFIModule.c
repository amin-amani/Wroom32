/*
 * WIFIModule.c
 *
 *  Created on: Mar 23, 2019
 *      Author: amin
 */
#include "WIFIModule.h"
CircularBuffer WIFIBuffer;
int ResponseIndex;
char Response[50];
char SSID[30];
char Password[30];
void WIFIModuleInit()
{
	WIFIBuffer=CircularBufferCreate(6);
	SSID[0]=0;
	Password[0]=0;

}
void SendSSID()
{int i=0;

	ResponseIndex=0;

	Response[0]=0x7e;
	Response[1]=8;
	Response[2]=1;
//	Response[3]='W';
//	Response[4]='I';
//	Response[5]='F';
//	Response[6]='I';
for(i=0;i<strlen(SSID);i++)
{
Response[i+3]=SSID[i];
}
	Response[i+4]=0x7e;


}
void SendPassword()
{

	int i;
	ResponseIndex=0;

	Response[0]=0x7e;
	Response[1]=12;
	Response[2]=2;
	for(i=0;i<strlen(Password);i++)
	{
	Response[i+3]=Password[i];
	}
		Response[i+4]=0x7e;
//	Response[3]='P';
//	Response[4]='A';
//	Response[5]='S';
//	Response[6]='S';
//	Response[7]='W';
//	Response[8]='O';
//	Response[9]='R';
//	Response[10]='D';
	//Response[11]=0x7e;

}
 char WIFIGetChar()
{
	 char result;
	result=Response[ResponseIndex];
	if(ResponseIndex<=sizeof(Response))ResponseIndex++;
	return result;
}
void WIFIPutChar(char data)
{
	uint8_t commandBuffer[6];
	CircularBufferPush(WIFIBuffer,&data, 1 );
	CircularBufferRead(WIFIBuffer,CircularBufferGetDataSize(WIFIBuffer),&commandBuffer[0]);
	if(commandBuffer[0]==0x7e && commandBuffer[5]==0x7e ){
	if(commandBuffer[2]==1)SendSSID();
	if(commandBuffer[2]==2)SendPassword();

	}
}
void WIFIModuleSetWIFIName(char*ssid)
{
strncpy(SSID,ssid,strlen (ssid));
}
void WIFIModuleSetWIFIPassword(char*password)
{
	strncpy(Password,password,strlen (password));
}
