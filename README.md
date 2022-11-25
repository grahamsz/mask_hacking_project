# mask_hacking_project

This repository contains code for hacking the [Liovode Programmable Facemask](https://amzn.to/3giLWTn) to work with an ESP32

Unfortunately, the microcontroller and batter for the mask itself are set in an epoxy block and cannot be readily extracted. However the LED Matrix display can be hooked up to an ESP32 micro-controller as shown.

```
     MASK PIN       ESP PIN
     VCC            3V3
     DATA           TX2
     GND            GND
```

