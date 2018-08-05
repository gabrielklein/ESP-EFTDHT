//
// This class is used to get temperature from a DS18xx
//
#ifndef EFT_MDHT_H
#define EFT_MDHT_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "comm/IComm.h"
#include "comm/MQTT.h"
#include "comm/ServerUtil.h"
#include <DHT.h>
#include <DHT_U.h>
#include <DallasTemperature.h>

namespace eft {

class MDHT : public IComm {

public:
MDHT(int pin, int type);
~MDHT();
void setup(String nameAP, String nameFX, Config *config);
String commType();
void commFx(JsonObject *o, JsonArray *ja);
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
void loop();
void servTemp();
void servConfig();

float tempC;
float tempF;
float humidity;

private:
ESP8266WebServer *webServer;
MQTT *mqtt;
String nameFX;
Config *config;
uint8_t type;

bool isFarenheit = false;

unsigned long timeLastUpdate = 0;
int updateEveryS = 15;
DHT *dht;

};

}

#endif
