# MDht

This module will collect temperature and humidity from a DHTxx module.

Should be supported: DHT11, DHT22, DHT21, AM2301

Perfect module for room temperature and humidity.

# Where to buy and how to install

You can buy a DHT22 on
https://www.aliexpress.com/item/AM2302-DHT22-Digital-Temperature-and-Humidity-Sensor-Module/32759901711.html

Put a resistance of 4700 Ohm between +3.3v and "signal"

Connect GND to GND of the ESP and VCC to +3.3v of the ESP. I suggest to connect the signal pin to D3.

# Integration

You can include the library using
```
#include "module/MDHT.h"

....

Then add the library to the hub
hub.add("dht", new eft::MDHT(D3, DHT22));

```

And add the following libraries (tip: in the lib_deps section of your platformio.ini)

*  DHT sensor library
*  Adafruit Unified Sensor

## C++ integration

Temperature is stored in
* dht.tempC;
* dht.tempF;

And humidity in
* dht.humidity;

# Core Web API

## /api/{name}

Get the current temperature.

```
{
  "temp": [
    {
      "type": 22,
      "humidity": 23.3,
      "tempC": 20,
      "tempF": 68
    }
  ]
}
```

Please note that temperature is updated in the loop() of the class every "updateEveryS" seconds.

## /api/{name}/config

Example: /api/temp/config?fahrenheit=0&updateEveryS=15

If you prefer to receive temperature in fahrenheit, set fahrenheit=1

Temperature is collected every updateEveryS seconds and sent to MQTT.

If you need more real time temperature, set it to a lower value.

```
{"time":194064,"config":{"temp.updateEveryS":"15","temp.fahrenheit":"0",}}
```


# MQTT

Temperature is sent to
{baseTopic}/{nameFX} (mybasetopic/dht/)

Example: (Temperature of 19.5°c, humidity of 22.90)
19.50,22.90
