//const char* ssid = "NET_2G2B7A87";
//const char* password = "2F2B7A87";
//const char* serverIP = "192.168.0.10";  // Substitua pelo endereço IP do seu servidor
//const int serverPort = 5000;            // Substitua pela porta do servidor Node.js
//sudo chmod a+rw /dev/ttyUSB0

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebSerial.h>
#include "config.h"
#include "helpers.h"

AsyncWebServer server(80);
long randNumber;


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(NETWORK_NAME, NETWORK_PASSWORD);

  otaUpdate();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting WiFi...");
  }

  httpRequests();

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();

  serialPrint("WiFi connected.");
  Serial.println("ESP32 IP address:");
  Serial.println(WiFi.localIP());
  serialPrint("Servidor HTTP iniciado.");
}

void otaUpdate() {
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname(ESP_NAME);
  ArduinoOTA.setPassword(ESP_PASSWORD);

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

void httpRequests() {
  server.on("/random-number", HTTP_GET, [](AsyncWebServerRequest* request) {
    randNumber = random(100);
    String espResponse = String(randNumber);
    request->send(200, "text/plain", espResponse.c_str());
    serialPrint(espResponse);
  });

  server.onNotFound(notFound);
}


void loop() {

  ArduinoOTA.handle();
}
