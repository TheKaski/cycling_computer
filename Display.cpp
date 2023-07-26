/* display.cpp -- Library for all the  display components */
#include "Display.h"

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
void sevSegmentDisplay::show() {
}
//7-Segment Display end

//Other displays:TODO