/* display.h -- Library for display components
*/
#ifndef Display_h
#define Display_h
#include "Arduino.h"

//7-segment Display start:
class sevSegmentDisplay {
    public:
    sevSegmentDisplay(int numOfDigits, int* digitPins, int numOfSegments, int* segmentPins);
    void begin();
    void show(char data[], int size, int delaytime=500);
    void setBrightnessPercentage(int brightness);
    /*
    Segment displays consist of N digits
    Therefore there must be atleast 1 or more digit pins
    and atleast 7 segmentpins and possible 8th decimal pin 
    NOTE: The display can also be commonCathode or commonAnode which might affect the circuit and use of this library
          TO handle this latertwo separate tables for characters need to be developped
    The functionality of the display will stay the same wheter there is
    1 or more digits
        A
5      ---
6   F |   | B
7     | G |
8      ---
9   E |   | C
10    |   |
11     ---  .dp
12      D
    */
    private:
        void showDigit(int digitPin, char dataChar, bool hasDot=false);
        void setupDisplay();
        void turnONSegments(uint8_t segmentCode, bool hasDot);
        void turnOFFSegments();
        int numOfDigits; //Number of digits on the display
        int numOfSegments; //Number of segments on the display

        int* digitPins; //Pins defined for the display in the alphabetical order + dp pin first
                          //If only 7-segments are given dp will be left out
        int* segmentPins; //Pins for the different segments of the display. Ordered from left to right 

        int brightness = 0; //Variable for controlling the brightness of the screen default set to 1

};
//7-Segment Display end

//Other displays:TODO
#endif