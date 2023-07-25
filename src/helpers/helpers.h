#ifndef helpers_h
#define helpers_h

#include "Arduino.h"
#include <WebSerial.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

void serialPrint(String msg);
void recvMsg(uint8_t* data, size_t len);
void notFound(AsyncWebServerRequest *request);

#endif