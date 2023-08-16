/* display.cpp -- Library for all the  display components */
#include "Display.h"
#include "Segments.h"

//7-Segment Display start:
//      PUBLIC METHODS
sevSegmentDisplay::sevSegmentDisplay(int numOfDigits, int* digitPins, int numOfSegments, int* segmentPins) 
{
    this->numOfDigits = numOfDigits;  
    this->numOfSegments = numOfSegments;
    this->digitPins = digitPins;
    this->segmentPins = segmentPins; //If the decimal segment is defined 8 segments can be used
}
//Set the pins ready to be used
void sevSegmentDisplay::begin() 
{
  setupDisplay();
  startupAnimation();
  
}

void sevSegmentDisplay::show(char data[], int size, int delaytime) 
{
  //Function for showing given data on screen. Output will show max ammount of characters that can be fitted in the provided display at once
  bool refresh = false;
  int T1 = millis();
  while(refresh == false) //THIS IS A TEST IMPLEMENTATION FOR LIMITING THE SCREEN REFRESHRATE  BY SHOWING SAME DATA FOR THE TIME OF DELAYTIME
  {
    int digit = 0;
    int dataIndex = 0;
    //Prints data to digits from left to right
    while(dataIndex < size )  
    {
      //Return if display gets full before the end of data[]
      if(digit >= this->numOfDigits)
      {
        break;
      }

      //If the next character is dot append it to this digit and skip the dot next round
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
    int T2 = millis();
    if(T2-T1 > delaytime) 
    {
      refresh = true;
    } 
  }
}

void sevSegmentDisplay::setBrightnessPercentage(int brightness) 
//THIS FUNCTION IS CURRENTLY OUT OF USE DUE PROBLEMS WITH THE BRIGHTNESS OFF THE DISPLAY COMPONENT
{
  this->brightness = 10-(brightness / 10); //The brightness value is given as percentage which we convert to PWM delay value to control the brightness
                                           //by doing this the greater value for brightnes we give the shorter the delay between digits will be 
}
//      PRIVATE METHODS:
void sevSegmentDisplay::showDigit(int digitPin, char dataChar, bool hasDot)
{
  //show digit will turn on the segments for the specified character and then light up the full digit
  //This function will use specified charactertable to translate characters to segments
  uint8_t charToSegments = SevenSegmentASCII[dataChar-32];  //get the segmentcode with the given datachar
  
  //Turn the segments on
  turnONSegments(charToSegments, hasDot);
  //Turn on the digit by setting the digitPin LOW 
  digitalWrite(digitPin, LOW); //The digit lights up when segments are HIGH and digit is LOW
  delay(6); //Delay for seeing the light
  digitalWrite(digitPin, HIGH);
  //Turn the segments off
  turnOFFSegments();
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

void sevSegmentDisplay::turnONSegments(uint8_t segments, bool hasDot)
//Function for turning on specified segments on the display
{
  //Turn on the segments
  for(int i = 0; i < this->numOfSegments; i++)
  {
    int bitValue = bitRead(segments, i); //NOTE: Bit read reads from least-significant(rightmost bit) to most significant
    int pin = this->segmentPins[i];

    if(bitValue == 1)
    {
      digitalWrite(pin,HIGH); //Turn on the segment
    } else {
      digitalWrite(pin,LOW); //Turn off the segment
    } 
  }
  //Turn on the dot segment when hasdot is set to True and nummber of segments is greater than 7 (8th segment is the decimal point)
  if(hasDot == true && this->numOfSegments > 7){
    digitalWrite(this->segmentPins[this->numOfSegments-1],HIGH);
  }
}

void sevSegmentDisplay::turnOFFSegments()
//Function for clearing the screen by turning OFF all the segments on the display
{
  //Turn off the segments
  for(int i = 0; i < this->numOfSegments; i++)
  {
    int pin = this->segmentPins[i];
    digitalWrite(pin, LOW); //Turn off this segment
  }
}

void sevSegmentDisplay::startupAnimation()
//Function for showing certain data sequence on display mainly in the beginning of the program UNNECESSARY BUT FUN
{
  char dataChar = '-'; //Animation character
  uint8_t charToSegments = SevenSegmentASCII[dataChar-32];
  //Turn on all the digits
  for(int pin = 0; pin < this->numOfDigits; pin++){
    digitalWrite(this->digitPins[pin], LOW);
  }
  //Turn on the character segments
  turnONSegments(charToSegments);
  delay(400);
  //Turn off and back on all the digits with a delay
  for(int pin = 0; pin < this->numOfDigits; pin++){
    digitalWrite(this->digitPins[pin], HIGH);
    delay(200);
    digitalWrite(this->digitPins[pin], LOW);
  }
  //Togle the segments on and off with delays
  turnOFFSegments();
  delay(200);
  turnONSegments(charToSegments);
  delay(200);
  turnOFFSegments();
  delay(100);
  turnONSegments(charToSegments);
  delay(200);
  turnOFFSegments();
  //Toggle the digits back off
  for(int pin = 0; pin < this->numOfDigits; pin++){
    digitalWrite(this->digitPins[pin], HIGH);
  }
}

//7-Segment Display end

//Other displays:TODO