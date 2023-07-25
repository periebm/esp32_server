#include "helpers.h"

void serialPrint(String msg){
  Serial.println(msg);
  WebSerial.println(msg);
}

void recvMsg(uint8_t* data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}