#include "MDHT.h"


namespace eft {

MDHT::MDHT(int pin, int type) {
        this->type = type;
        this->dht = new DHT(pin, type);
};

MDHT::~MDHT() {
        delete(dht);
};

void MDHT::setup(String nameAP, String nameFX, Config *config) {
        this->nameFX = nameFX;
        this->config = config;

        isFarenheit = config->getBool(nameFX + ".fahrenheit", false);
        updateEveryS = config->getInt(nameFX + ".updateEveryMS", 15);
        if (updateEveryS<2) {
                updateEveryS = 2;
        }

};

String MDHT::commType() {
        return "MDHT";
};

void MDHT::commFx(JsonObject *o, JsonArray *ja) {

        (*o)["description"] = "Support for DHTxx temperature and humidity modules";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "View temperature and humidity of the DHTxx module";
        }

        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX+"/config";
                x["desc"] = "Configure the DHTxx";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "fahrenheit", "boolean", "Do we display temperature in fahrenheit instead of celsius");
                CommUtil::addFXParam(&t, "updateEveryS", "number", "Upload every x seconds");

        }


};


void MDHT::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {

        this->webServer = webServer;

        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servTemp();
                });

        webServer->on("/api/"+nameFX+"/config", HTTP_GET, [&] () {
                        this->servConfig();
                });

        this->mqtt = mqtt;
        this->dht->begin();
};


/**
 * Get list of supported timezones.
 */
void MDHT::servConfig() {

        bool doSave = false;

        if (webServer->hasArg("fahrenheit")) {

                isFarenheit = Config::isTrue(webServer->arg("fahrenheit"), false);

                config->setBool(nameFX + ".fahrenheit", isFarenheit);
                doSave = true;
        }

        if (webServer->hasArg("updateEveryS")) {

                String v = webServer->arg("updateEveryS");
                updateEveryS = v.toInt();
                if (updateEveryS<2) {
                        updateEveryS = 2;
                }

                config->setInt(nameFX + ".updateEveryS", updateEveryS);
                doSave = true;
        }

        if (doSave) {
                config->saveConfig();
        }

        config->servConfig(nameFX + ".fahrenheit", nameFX + ".updateEveryS");

};


void MDHT::servTemp() {


        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        JsonArray& tzo = root.createNestedArray("temp");
        JsonObject& x = tzo.createNestedObject();

        x["type"] = type;
        if (!isnan(humidity))
                x["humidity"] = humidity;
        if (!isnan(tempC))
                x["tempC"] = tempC;
        if (!isnan(tempF))
                x["tempF"] = tempF;


        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};

void MDHT::loop() {

        // Sleep
        long d = millis()-timeLastUpdate;
        if (d>=0 && d<updateEveryS*1000) {
                return;
        }

        // We should have the new temperature after 1 secondo
        timeLastUpdate = millis();

        //dht->read();
        humidity = dht->readHumidity();
        tempC = dht->readTemperature();
        tempF =  dht->readTemperature(true);
        //DallasTemperature::toFahrenheit(dhtTemp)

        if (!isnan(humidity) && !isnan(tempC)) {
                if (isFarenheit) {
                        this->mqtt->publish(nameFX, "",  String(tempF)+String(",")+String(humidity));
                }
                else {
                        this->mqtt->publish(nameFX, "",  String(tempC)+String(",")+String(humidity));
                }
        }

};


}
