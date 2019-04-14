#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H

#define EXAMPLE_WIFI_SSID "espwifi"
#define EXAMPLE_WIFI_PASS ""
#define PORT 8888
//-----------------------------------------------------------------------------------------
void (*TCPServerNewDataReceivedCallback)(char*data,int len);
void (*TCPNewClientConnectedCallback)();
//=================================================================================================================
void TCPServerStartListen();
//=================================================================================================================
int TCPServerSendData(char *data,int len);
//=================================================================================================================
//void TCPServerInit(void);
 void TCPServerInit(char*ssid,char*password);
//=================================================================================================================
#endif
