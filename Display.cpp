/* display.cpp -- Library for all the  display components */
#include "Display.h"
#include "Segments.h"
//7-Segment Display start:
sevSegmentDisplay::sevSegmentDisplay(int numOfDigits, int digitPins[], int numOfSegments, int segmentPins[]) {
    //Number of pins and segments given
    this->numOfDigits = numOfDigits;
    this->numOfSegments = numOfSegments;
    this->digitPins[ numOfDigits ] = *digitPins;
    this->segmentPins[ numOfSegments ] = *segmentPins; //If the decimal segment is defined 8 segments can be used
}
//Set the pins ready to be used
void sevSegmentDisplay::begin() {
  //Set the digitPins  as OUTPUTS:
  for(int Pin = 0; Pin < this->numOfDigits; Pin++){
    pinMode(this->digitPins[Pin], OUTPUT);
  }
  //Set the segmentPins as OUTPUTS:
  for(int Pin = 0; Pin < this->numOfSegments; Pin++){
    pinMode(this->digitPins[Pin], OUTPUT);
  }
}

void showDigit(int digitPin, char dataChar){
  //show digit will turn on the segments for the specified character and then light up the full digit
  //This function will use specified charactertable to define the segments
  uint8_t charToSegments = SevenSegmentASCII[dataChar];
  

  
}

void sevSegmentDisplay::show(char data[]) {
  //The display will show information by turning on the digits one by one

}
//7-Segment Display end

//Other displays:TODO