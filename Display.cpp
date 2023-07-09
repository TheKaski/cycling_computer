/* display.cpp -- Library for display components

*/
#include "Display.h"

sevSegmentDisplay::sevSegmentDisplay() {

}
void sevSegmentDisplay::begin(byte numOfDigits, byte digitPins[], byte segmentPins[]) {
    this.numDigits = numOfDigits;
    this.digitPins = digitPins;
    this.segmentPins = segmentPins;
}