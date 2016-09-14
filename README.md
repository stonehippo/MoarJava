MoarJava
========

A simple, connected weight monitoring gadget using an ESP8266 and [Adafruit IO](https://io.adafruit.com) over MQTT.

About MoarJava
--------------

- Built using a few simple components
  - ESP8266 board — I'm using an [Adafruit Huzzah ESP8266](https://www.adafruit.com/product/2471), but other boards based on this chip will likely work
  - A force sensitive resistor
- Developed using [Platform.io](https://platform.io)

Notes
-----

The ESP8266 ADC pin can only take 0-1V input. To make sure this is all it gets, I'm reading from the FSR via a resistor divider:

```
3.3V->FSR->R1[10K]->ADC->R2[3.9K]->GND
```
