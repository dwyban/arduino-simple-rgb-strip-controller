# Simple RGB Strip Controller
## Introduction
Simple RGB Strip Controller is as the name implies, a very simple RGB strip controller and is built on the Arduino platform.  This controller accepts very simple commands via the Arduino's serial port to change the color output on the RGB strip connected to it.  This controller uses the PWM outputs of the Arduino to change the color of the LEDs and is designed to work with simple 12V 5050 RGB LED strips.


## Project Description
This project consists of an Arduino Sketch to configure the Arduino as a Simple RGB LED Controller.  This sketch uses the [ArduinoSerialCommand](https://github.com/scogswell/ArduinoSerialCommand) library for command processing so you will need to download and install this library in your Arduino libraries directory before the project can be bilt.


## Parts List
* Arduino Nano
* 3x 5kOhm Resitors
* 3x Low RDSon N-Channel MOSFET (I used [NXP PSMN022-30PL](https://www.digikey.com/product-detail/en/nexperia-usa-inc/PSMN022-30PL127/1727-5893-ND/2606361))
* Hookup Wire
* Breadboard
* 12V 5050 RGB LED Strips

## Wiring Diagram
Below is a wiring diagram for this project showing how to hook up the Arduino to the RGB LED Strips.

INSERT DIAGRAM HERE

### Theory of Operation
As should be obvious the Arduino cannot drive the RGB LED strips directly.  Instead, the Arduino drives the gates of three low RDSon N-Channel MOSFETs whose channels are connected between the RGB LEDS and ground.  When the PWM output is high, the MOSFET turns on allowing current to flow through the LED strip.  When the PWM output is low, the MOSFET turns off thus stopping the flow of current through the LEDs.  The brightness of the LEDS is controlled by changing the pulse width of the PWM output.

### Gate Resistors
In theory the gate of a MOSFET draws no current so in theory there should be no problem connecting the outputs of the Arduino directly to the gates of the MOSFETs.  However, you will notice in the diagram that there are resistors between the Arduino pins and the gates of the MOSFETs.  In reality, when the signal into the gate goes high there is significant current flow as the gate of the MOSFET is essentially a capacitor and current must flow to charge it to turn the MOSFET on.  These resistors limit this current flow so that it does not exceed the current output capability of the Arduino.  These resistors also prevent the MOSFETs from turning on too fast which can produce oscillations and create other undesired effects in the system.


## Commands
Commands are sent to the controller as text strings and must be terminated with a carriage return character '\r'.  Commands consist of the command itself followed by any parameters.  Command parameters can be in the following formats.

| Parameter | Description |
| --------- | ----------- |
| &lt;0xRRGGBB&gt; |The RGB color value given as a series of hex digits.  For example "0099FF" |
| &lt;NR1&gt; | Floating point decimal number.  For example "1.25" |


The controller supports the following commands:

| Command | Description | Example |
| ------- | ----------- | ------- |
| C &lt;0xRRGGBB&gt; | Color: Immidiately sets the color to the passed value. | "C AA00FF\r" |
| CF &lt;0xRRGGBB&gt; | Color Fade: Fades the color to the passed value.  The length of fade is set by the fade time. | "CF 2299AA\r" |
| FT &lt;NR1&gt; | Fade Time: Sets the amount of time in seconds for a fade to complete. | "FT 2.0\r" will set the fade time to 2 seconds |
  
