# ESP-EFTDHT

ESP-EFTDHT is a wrapper around the DHT11 and DHT22 modules to collect Humidity & Temperature information.

This library is an add-on to ESP-EFTCore.
https://github.com/gabrielklein/ESP-EFTCore

You can get more information on https://github.com/adafruit/DHT-sensor-library.

# Example

```c

// Include the DHT addon library
#include "module/MDHT.h"

// You need the hub
eft::Hub hub("MyNeopixelExample");

// In your setup function
void setup() {

  ...

  // You need to add the
  hub.add("dht", new eft::MDHT(D3, DHT22));

}

// In your loop function don't forget
void loop() {
        hub.loop();
}
```

For more information, see full documentation [here](doc/MDHT.md).
