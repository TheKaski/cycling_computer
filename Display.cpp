/* display.cpp -- Library for all the  display components */
#include "Display.h"
#include "Segments.h"
//7-Segment Display start:
//      PUBLIC METHODS
sevSegmentDisplay::sevSegmentDisplay(int numOfDigits, int* digitPins, int numOfSegments, int* segmentPins) {
    //Number of pins and segments given
    this->numOfDigits = numOfDigits;
    this->numOfSegments = numOfSegments;
    this->digitPins = digitPins;
    this->segmentPins = segmentPins; //If the decimal segment is defined 8 segments can be used
}
//Set the pins ready to be used
void sevSegmentDisplay::begin() {
  //Setup the display to be used
  setupDisplay();
}

void sevSegmentDisplay::show(char data[], int size) {
  //The display will show information by turning on the digits one by one
  //Go through the data NOTE: longer than 4 or shorter than 4 chars should be able to give
 
  for(int i = 0; i < size; i++)
  { 
      if(i>this->numOfDigits)
      {
        return;
      }
      showDigit(this->digitPins[i], data[i]);
      //delay(1);
  }
}
void sevSegmentDisplay::setBrightnessPercentage(int brightness) {
  this->brightness = 10-(brightness / 10); //The brightness value is given as percentage which we convert to PWM delay value to control the brightness
                                           //by doing this the greater value for brightnes we give the shorter the delay between digits will be 
}
//      PRIVATE METHODS:
void sevSegmentDisplay::showDigit(int digitPin, char dataChar){
  //show digit will turn on the segments for the specified character and then light up the full digit
  //This function will use specified charactertable to define the segment
  uint8_t charToSegments = SevenSegmentASCII[dataChar-32];  //ASCII table starts from space (ascii value 32) 

  //Turn on the segments
  for(int i = 0; i < this->numOfSegments; i++)
  {
    int bitValue = bitRead(charToSegments, i); //NOTE: Bit read reads from least-significant(rightmost bit) to most significant
    int pin = this->segmentPins[i];

    if(bitValue == 1)
    {
      digitalWrite(pin,HIGH); //Turn on the segment
    } else {
      digitalWrite(pin,LOW); //Turn off the segment
    } 
  }
  // Turn on the digit by setting the digitPin LOW 
  digitalWrite(digitPin, LOW);
  delay(7);
  digitalWrite(digitPin, HIGH);

  //Turn off the segments
  for(int i = 0; i < this->numOfSegments; i++)
  {
    int pin = this->segmentPins[i];
    digitalWrite(pin, LOW); //Turn off the segment
  }
}

void sevSegmentDisplay::setupDisplay()
{
   //Set the digitPins  as OUTPUTS:
  for(int pin = 0; pin < this->numOfDigits; pin++){
    pinMode(this->digitPins[pin], OUTPUT);
    digitalWrite(this->digitPins[pin], HIGH);
  }
  //Set the segmentPins as OUTPUTS:
  for(int pin = 0; pin < this->numOfSegments; pin++){
    pinMode(this->segmentPins[pin], OUTPUT);
    digitalWrite(this->segmentPins[pin], LOW);
  }
}

//7-Segment Display end

//Other displays:TODO