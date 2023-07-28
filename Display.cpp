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

void sevSegmentDisplay::showDigit(int digitPin, char dataChar){
  //show digit will turn on the segments for the specified character and then light up the full digit
  //This function will use specified charactertable to define the segments
  char data = '0';
  Serial.println(int(data));
  uint8_t charToSegments = SevenSegmentASCII[18];
  int segmentPins[] = {12,10,6,8,7,11,5,9}; //{ A, B, C, D, E, F, G, DP}
  //Turn on the segments
  for(int i = 0; i < 8; i++)
  {
    int bitValue = bitRead(charToSegments, i); //NOTE: Bit read reads from least-significant(rightmost bit) to most significant
    Serial.print(bitValue);
    if(bitValue == 1)
    {
      int pin = segmentPins[i];
      digitalWrite(pin,HIGH); //Turn on the segment
    } 
  }
  Serial.println();
  // Turn on the digit
  digitalWrite(digitPin, HIGH);
  delay(100);
  digitalWrite(digitPin, LOW);
  delay(100);

  //Turn off the segments
  for(int i = 0; i < 8; i++)
  {
    int bitValue = bitRead(charToSegments, i);
    if(bitValue == 1)
    {
      int pin = segmentPins[i];
      digitalWrite(pin, LOW); //Turn off the segment
    } 
  }

}

void sevSegmentDisplay::show(char data[]) {
  //The display will show information by turning on the digits one by one
  int digitPins2[] = {21, 2, 3, 4};

  //Go through the data NOTE: longer than 4 or shorter than 4 chars should be able to give
  for(int i = 0; i < 4; i++)
  {
    showDigit(digitPins2[i], data[i]);
  }

}
//7-Segment Display end

//Other displays:TODO