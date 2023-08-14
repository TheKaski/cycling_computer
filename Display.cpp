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
  //Function for showing given data on screen. Output will show max ammount of characters that can be fitted in the provided display at once
  int digit = 0;
  int dataIndex = 0;
  //Prints data to digits from left to right
  while(dataIndex < size )
  {
    //Return if display gets full before the end of data[]
    if(digit >= this->numOfDigits)
    {
      return;
    }
    //If the next character is dot append it to this digit
     if(data[dataIndex+1] == '.')
      {
        showDigit(this->digitPins[digit], data[dataIndex], true);
        dataIndex++; //Skip the dot in the data
      } else {
        showDigit(this->digitPins[digit], data[dataIndex]);
      }
      digit++; //Move to next digit
      dataIndex++; //Move to next data char
  }
}

void sevSegmentDisplay::setBrightnessPercentage(int brightness) {
  this->brightness = 10-(brightness / 10); //The brightness value is given as percentage which we convert to PWM delay value to control the brightness
                                           //by doing this the greater value for brightnes we give the shorter the delay between digits will be 
}
//      PRIVATE METHODS:
void sevSegmentDisplay::showDigit(int digitPin, char dataChar, bool hasDot){
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
  if(hasDot == true){
    digitalWrite(this->segmentPins[this->numOfSegments-1],HIGH);
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