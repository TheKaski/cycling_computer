#include "Display.h"
#include <avr/dtostrf.h>
// Setting pins
const int speedSensor = 3; 
const int navButton = 4;

// Setting clock variables
bool clockEnabled = false;  // Variable for determining the clock state
unsigned long int T1, T2, timeDiff, clockTime, lastClockTime;  // Time values

// Setting display variable:
int navIndex = 0; //The default value is 0 resultting in rendering the speed value
// User settings:
float wheelSize = 27.5; // Wheel size in inches
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m
int screenRefreshRate = 1000; // Screen refresh rate in ms
int timeOut = 8000; //Timeout for detecting stopped state

// Calculated values:
float speed = 0;  // Speed in km/h
float distance = 0;  // distance in km
float time_s = 0;  // time in seconds
float avgSpeed = 0;  // average speed in km/h

//Setup display components:   
const int numOfDigits = 4;
const int numOfSegments = 8;      
int digitPins[numOfDigits] = {11, 8, 7, 12}; // {Dig1, Dig2, Dig3, Dig4} 
int segmentPins[numOfSegments] = {10, 6, 17, 15, 14, 9, 18, 16}; // { A, B, C, D, E, F, G, DP}
// Initialize the display
sevSegmentDisplay segmentDisplay(numOfDigits, digitPins, numOfSegments, segmentPins);
void setup() {
  Serial.begin(9600);  // Starting serial communication

  //Print user info to serial
  Serial.print("Wheel circumference in m: ");
  Serial.println(wheelCircumference);

  pinMode(speedSensor, INPUT); // Initialize the speedSensor pin as an input:
  pinMode(navButton, INPUT); // Initialize the navbutton as input 

  //Attatch interrupt to SpeedSensor Pin
  attachInterrupt(digitalPinToInterrupt(speedSensor), speedSensorInterrupt, LOW);

  //Start the display and set the brightness to 100%
  segmentDisplay.begin();
  segmentDisplay.setBrightnessPercentage(100);
}
void loop() {

  lastClockTime = clockTime;
  clockTime = millis();
  //Serial.println(clockTime - lastClockTime);
  updateDisplay(); // Call the update function for the display
  //Serial.println(timeDiff);
  //Serial.println(digitalRead(speedSensor));
  checkTime();

  // Check wheter navigation button was pressed:
  int navButtonState = digitalRead(navButton);
  Serial.println(navButtonState);
  if(navButtonState == HIGH) {
    Serial.println("Changing the view");
    // Change what we want to show on screen:
    if(navIndex == 2) {
      navIndex = 0;
      return;
    }
    navIndex = navIndex+1;
  }

}
// Function for updating what is shown on the screen.
// Everything shown depends on the values of the variables and from the currently selected 
// variable. The default value is 0 which is the current speed and will always be shown if push button
// is not in use.
void updateDisplay() {
  // Check the navigationIndex to render the correct variable on screen 
  switch (navIndex) {
    case 0:
      // Display avgSpeed
      displayValue(speed);
      break;
    case 1:
      // Display speed
      displayValue(avgSpeed);
      break;
    case 2:
      // Display distance
      displayValue(distance);
      break;
    default:
      // Handle unexpected state (optional)
      break;
  }

}
// Function for displaying given value on the display:
void displayValue(float value)
{
  char result[8];
  dtostrf(value, 6, 2, result);
  segmentDisplay.show(result, 6);
}

//Function for checking elapsed time used to detect wheter wheel has stopped
void checkTime()
{
  //Clock needs to be in enabled state
  if(clockEnabled)
  {
    T2 = millis();
    //If elapsedTime is greater than timeout we set the speed to zero
    if(T2-T1 > timeOut)
    {
     speed = 0;
    }
  }
}
void printToSerial(unsigned int long value) {
    Serial.println(value);
}

float calculateSpeed(unsigned long int spinTime) {

  // Calculates speed by dividing wheel circumference by time in seconds and converting to km/h
  speed = wheelCircumference / (spinTime / 1000.0) * 3.6; // Speed in km/h
  return speed;

} 
//This is a interrupt function which will take Time T1 or T2 based on clock status
void speedSensorInterrupt() {
  //Take end time when clock status is enabled
  if(clockEnabled)
  {
    T2 = millis();
    timeDiff = T2-T1;
    //If the timeDifference is greater then 1ms calculate speed from it
    if( timeDiff > 1 )
    {
      clockEnabled = false;
      calculateSpeed(timeDiff); //Update the speed value
    }
  }
  //Take T1 Always when clock status is not enabled (Start new clock every time)
  T1 = millis();
  clockEnabled = true;
}

