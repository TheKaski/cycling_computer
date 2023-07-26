/* display.h -- Library for display components
*/
#ifndef Display_h
#define Display_h
#include "Arduino.h"

//7-segment Display start:
class sevSegmentDisplay {
    public:
    sevSegmentDisplay(int numOfDigits, int digitPins[], int segmentPins[], bool decimalSegment=false);
    void begin();
    void setValue(int value);
    /*
    Segment displays consist of N digits
    Therefore there must be atleast 1 or more digit pins
    and atleast 7 segmentpins and possible 8th decimal pin 
    NOTE: The display can also be commonCathode or commonAnode which might affect the circuit and use of this library
          TO handle this latertwo separate tables for characters need to be developped
    The functionality of the display will stay the same wheter there is
    1 or more segments
    */
    private:
        int numDigits;
        int *digitPins[];
        int *segmentPins[];
};
//7-Segment Display end

//Other displays:TODO
#endif