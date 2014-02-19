neospi
======

Neopixels via AVR (ATtiny85) SPI

ATtiny85 using the USI as SPI slave to read pixel values.  Example is pixl.ino arduino project to drive 4 pixels.

This "fixes" the problem where the neopixels have tight timing constraints.

512 bytes of ram can handle 160+ pixels!

It also shows how to use the USI as an SPI slave.

TODO: slave select
TODO: eeprom and flash stores images, will display out of reset or via simple commands.
