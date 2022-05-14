# parking-light

A device to help you park within a good region in your garage (i.e., complicated tennis ball).

Basically, the device will turn on a light to indicate you are within a good region to park, and it will rapidly blink the light to indicate you are too close. If there is no movement after 30 seconds, the light turns off, and the distance is only checked every three seconds to save power.

Distance from the device to the vehicle is measured with an ultrasonic distance sensor. There are two buttons: one to set the "close enough" distance, and the other to set the "too close" distance. These distance settings are saved to flash memory so they persist after a power cycle.


## Hardware
- [Seeeduino XIAO microcontroller](https://www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html) (Arduino alternative)
- [HC-SR04 ultrasonic distance sensor](https://www.sparkfun.com/products/15569)
- Buttons: Any normally-open pushbuttons connected to GND will do.
- Light: Whatever you want, as long as you use an NPN transistor (or similar positive-triggered device) to turn it on; the microcontroller likely won't be able to supply enough current on its own to power the light.
- See [parking_light.ino](src/arduino/parking_light/parking_light.ino#L8) for pin assignments.
