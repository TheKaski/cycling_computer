/* display.h -- Library for display components

*/
#ifndef Display_h
#define Display_h
//#include "Arduino.h"

//Class for 7-segment displays
class sevSegmentDisplay {
    public:
    sevSegmentDisplay();
    void begin();
    /*
    Segment displays consist of N digits
    Therefore there must be atleast 1 or more digit pins
    and atleast 7 segmentpins and possible 8th decimal pin

    The functionality of the display will stay the same wheter there is
    1 or more segments
    */
    private:
        byte numDigits;
        byte digitPins[];
        byte segmentPins[];
};

#endif