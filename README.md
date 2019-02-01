# Simple RGB Strip Controller
## Introduction
Simple RGB Strip Controller is as the name implies, a very simple RGB strip controller and is built on the Arduino platform.  This controller accepts very simple commands via the Arduino's serial port to change the color output on the RGB strip connected to it.  This controller uses the PWM outputs of the Arduino to change the color of the LEDs and is designed to work with simple 12V 5050 RGB LED strips.

## Project Description
This project consists of an Arduino Sketch to configure the Arduino as a Simple RGB LED Controller.  This sketch uses the [Arduino-SerialCommand](https://github.com/kroimon/Arduino-SerialCommand) library for command processing so you will need to download and install this library in your Arduino libraries directory before the project can be bilt.

## Wiring Diagram
Below is a wiring diagram for this project showing how to hook up the Arduino to the RGB LED Strips.

INSERT DIAGRAM HERE

### Theory of Operation
As should be obvious the Arduino cannot drive the RGB LED strips directly.  Instead, the Arduino drives the gates of three low RDSon N-Channel MOSFETs whose channels are connected between the RGB LEDS and ground.  When the PWM output is high, the MOSFET turns on allowing current to flow through the LED strip.  When the PWM output is low, the MOSFET turns off thus stopping the flow of current through the LEDs.  The brightness of the LEDS is controlled by changing the pulse width of the PWM output.

### Gate Resistors
In theory the gate of a MOSFET draws no current so in theory there should be no problem connecting the outputs of the Arduino directly to the gates of the MOSFETs.  However, you will notice in the diagram that there are resistors between the Arduino pins and the gates of the MOSFETs.  In reality, when the signal into the gate goes high there is significant current flow as the gate of the MOSFET is essentially a capacitor and current must flow to charge it to turn the MOSFET on.  These resistors limit this current flow so that it does not exceed the current output capability of the Arduino.  These resistors also prevent the MOSFETs from turning on too fast which can produce oscillations and create other undesired effects in the system.
