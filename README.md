## Introduction

This project aims to offer way to turn your Arduino board into a basic cycling speedometer with the following measurement features implemented: Current speed, Average speed, Travelled distance and Cadence sensor. Additionally this project also offers 3D models for 3D printed parts including: casing of the board, casing of the sensor, holder for magnets. This code was implemented for the board: Arduino RP2040 Nano Connect with BLE support, but other boards should also be compatitible with the code with minor adjustments. Check the Hardware section for more details about my specific build.

## Contributors

<a href="https://github.com/TheKaski/cycling_computer/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=TheKaski/cycling_computer" />
</a>

## Supported Features

- **Speed Sensor**
- **Cadence Sensor**
- **7/8-Segment Display**
- **Current Speed**
- **Average Speed**
- **Travelled Distance**

## Goals and outcomes of this project

The goals for this project were to:

1. Find interesting use case for Arduino board and develop a solution for that use case
2. Learn more about Arduino, C++ and electronics
3. Learn more about 3D printing and designing.

Outcome of this project is that I learned a lot about working with Arduino and electronics. I also learned a lot more about C++ as I decided to write the 7-segment display class from scratch. My skills on 3D printing and design were also improved by the project. I am particularily happy with the snap on bottom and top of the casing and the design which allows to print separately the bottom, casing and the top to make it easier to fit other components.

## How to use this project:

You can use this project by cloning this repo:

```
git clone https://github.com/TheKaski/cycling_computer.git
```

Open the file cycling_computer.ino with the Arduino IDE and upload to your board.
The Arduino codefile cycling_computer.ino uses the custom made display class for the segment display, however with small adjustments you can also configure the code to use any other display class if you want to. You can also try to develop your own display class inside the Display.cpp and .h files if you want to learn about Arduino, C++, Display libraries and Github contributions. However this is not expected by the nature of this project being only a hobby project. You can adjust the .ino file according to your needs relatively easily.

## Hardware used

Board: Arduino RP2040 Nano Connect

Battery: 9V battery

Hall effect sensor: Bipolar hall effect sensor SS441A

Display: Four digit segment display with decimal points(8-segments) 5611AS

PushButton: Momentary 4-pin mini pushbutton

Resistors(Ω): 10K, 680, 220

Magnets: 2pcs small neodymium magnets

3D filament: PLA

## Libraries required:
- ArduinoBLE
- avr/dtostrf
