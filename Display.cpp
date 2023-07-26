/* display.cpp -- Library for all the  display components */
#include "Display.h"

//7-Segment Display start:
sevSegmentDisplay::sevSegmentDisplay(int numOfDigits, int digitPins[], int segmentPins[], bool decimalSegment) {
    this->numDigits = numOfDigits;
    this->digitPins[ numOfDigits ] = digitPins;
    this->segmentPins[decimalSegment ? 8 : 7] = segmentPins; //If the decimal segment is defined 8 segments can be used
}
//Set the pins ready to be used
void sevSegmentDisplay::begin() {
}

void sevSegmentDisplay::setValue(int value) {
}
//7-Segment Display end

//Other displays:TODO