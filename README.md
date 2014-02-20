neospi
======

Neopixels via AVR (ATtiny85) SPI

ATtiny85 using the USI as SPI slave to read pixel values.  Example is pixl.ino arduino project to drive 4 pixels using digital 2 as the chip/slave select.

This "fixes" the problem where the neopixels have tight timing constraints.

512 bytes of ram can handle 160+ pixels!

It also shows how to use the USI as an SPI slave.

Based loosely on https://github.com/cpldcpu/light_ws2812 (just some of the assembly lines)

TODO: eeprom and flash stores images, will display out of reset or via simple commands.
