/* display.cpp -- Library for all the  display components */
#include "Display.h"
#include "Segments.h"
//7-Segment Display start:
sevSegmentDisplay::sevSegmentDisplay(int numOfDigits, int* digitPins, int numOfSegments, int* segmentPins) {
    //Number of pins and segments given
    this->numOfDigits = numOfDigits;
    this->numOfSegments = numOfSegments;
    this->digitPins = digitPins;
    this->segmentPins = segmentPins; //If the decimal segment is defined 8 segments can be used
}
//Set the pins ready to be used
void sevSegmentDisplay::begin() {
  //Set the digitPins  as OUTPUTS:
  for(int Pin = 0; Pin < this->numOfDigits; Pin++){
    pinMode(this->digitPins[Pin], OUTPUT);
    digitalWrite(this->digitPins[Pin], LOW); 
    digitalWrite(this->digitPins[Pin], HIGH); //Turn off
  }
  //Set the segmentPins as OUTPUTS:
  for(int Pin = 0; Pin < this->numOfSegments; Pin++){
    pinMode(this->digitPins[Pin], OUTPUT);
    digitalWrite(this->segmentPins[Pin], LOW); //Turn off
  }
}

void sevSegmentDisplay::showDigit(int digitPin, char dataChar){
  //show digit will turn on the segments for the specified character and then light up the full digit
  //This function will use specified charactertable to define the segment

  uint8_t charToSegments = SevenSegmentASCII[dataChar-32];  //ASCII table starts from space (ascii value 32) 

  //Turn on the segments
  for(int i = 0; i < 8; i++)
  {
    int bitValue = bitRead(charToSegments, i); //NOTE: Bit read reads from least-significant(rightmost bit) to most significant
    if(bitValue == 1)
    {
      int pin = this->segmentPins[i];
      digitalWrite(pin,HIGH); //Turn on the segment
    } 
  }
  // Turn on the digit by setting the digitPin LOW
  digitalWrite(digitPin, LOW);
  delay(1); //Delay to see the light
  digitalWrite(digitPin, HIGH); //Turn off

  //Turn off the segments
  for(int i = 0; i < 8; i++)
  {
    int bitValue = bitRead(charToSegments, i);
    if(bitValue == 1)
    {
      int pin = this->segmentPins[i];
      digitalWrite(pin, LOW); //Turn off the segment
  
    } 
  }
}
void sevSegmentDisplay::show(char data[], int size) {
  //The display will show information by turning on the digits one by one
  //Go through the data NOTE: longer than 4 or shorter than 4 chars should be able to give
  for(int i = 0; i < this->numOfDigits; i++)
  { 
      if(i>=size)
      {
        showDigit(this->digitPins[i], ' ');
        delay(this->brightness);
      }
      else {
        showDigit(this->digitPins[i], data[i]);
        delay(this->brightness);
      }
  }
}
void sevSegmentDisplay::setBrightnessPercentage(int brightness) {
  this->brightness = 10-(brightness / 10); //The brightness value is given as percentage which we convert to PWM delay value to control the brightness
                                           //by doing this the greater value for brightnes we give the shorter the delay between digits will be 
}
//7-Segment Display end

//Other displays:TODO