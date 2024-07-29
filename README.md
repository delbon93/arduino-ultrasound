# Ultrasound distance sensor

## Introduction

This project was my first serious foray into Arduino programming for sensors and other
electronic components. It was my attempt at getting started with integrating multiple
hardware components into a complete solution that could serve an actual purpose.

The final product is a distance sensor based on ultrasound echometry, i.e. measuring
the time a sound signal takes reach some object and be reflected back onto the sensor.
This resulting measurement then goes through unit conversion and smoothing, yielding
a distance value. 

This value is then displayed on a seven-segment display driven by a bitshift register to minimize pin usage.

## Project Components

The project makes use of the following components:

- Arduino UNO R3
- HC-SR04 Ultrasound Sensor
- SN74HC595N Bitshift Register
- 5461AS-1 Four Digit Seven-Segment Display

## Board Schematic

![Board Schematic](/docs/img/schematic.png)

## Notable Challenges

I set out with the intention of not using any libraries or headers for the hardware components, but writing my own instead to increase the learning effect of this project. This provided an interesting challenge in hardware abstraction and object oriented design in relation to electronic components.

This also meant I had to programm all the signals and timings myself, resorting to guides I found online, but primarily manuals and datasheets of the specific components that I used (see linked resources below).

Another interesting problem was the fact that the 5461AS seven-segment display - despite having four digits - only provides a single set of data lines for the individual segments of a digit. <br> Therefore, in order to show four digits independently from one another, they have to be controlled in turn, but fast enough to not be noticable.

## Resources

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Using the KYX-5461AS 4-digit 7-segment LED display with Arduino](https://thomas.bibby.ie/using-the-kyx-5461as-4-digit-7-segment-led-display-with-arduino/) by Thomas Bibby
- [74HC595 datasheet](https://www.ti.com/lit/ds/symlink/sn74hc595.pdf?ts=1721926001283&ref_url=https%253A%252F%252Feu.mouser.com%252F) by Texas Instruments
- [This document describing the HC-SR04 operation](https://www.mikrocontroller.net/attachment/218122/HC-SR04_ultraschallmodul_beschreibung_3.pdf) by KT-Elektronic (in German)
- [TinkerCAD](https://www.tinkercad.com) by Autodesk for the schematics
