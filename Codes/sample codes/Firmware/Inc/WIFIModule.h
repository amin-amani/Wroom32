#ifndef __WIFI_MODULE_H
#define __WIFI_MODULE_H
#include "CircularBuffer.h"
#include <string.h>
void WIFIModuleInit();
 char WIFIGetChar();
void WIFIPutChar(char data);
void WIFIModuleSetWIFIName(char*ssid);
void WIFIModuleSetWIFIPassword(char*password);

#endif
