#include "Display.h"
#include <avr/dtostrf.h>
// Setting pins
const int speedSensor = 5; //Adjust these values based on your circuit needs
const int navButton = 4;
//const int cadenceSensor = 0; //CadenceSensor can be configured by using this pin.

// Setting clock variables
bool clockEnabled = false;  // Variable for determining the speedSensor clock state
unsigned long int T1, T2, timeDiff, clockTime, lastClockTime;  // Time values used by the speedsensor
bool cadenceClockEnabled = false;
unsigned long int cadenceT1, cadenceT2; // Time values used by the cadenceSensor

// Setting displayindex variable:
int navIndex = 0; 
const int maxIndex = 2; // adjust this variable based on how many variables you want to show on screen.

// User specific settings:
float wheelSize = 27.5; // Wheel size in inches you want to adjust this based on your bike
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m calculated based on your wheelsize
int screenRefreshRate = 1000; // Screen refresh rate in ms
int timeOut = 8000; // Timeout for detecting stopped state which will set the current speed back to 0

// Calculated tracked values:
float speed = 0;  // Speed in km/h
float distance = 0;  // distance tracked in meters
float avgSpeed = 0;  // average speed in km/h
float time_s = 0;  // time in seconds is unused for now.
float cadence = 0;
unsigned long rotationsCount = 0; // keeping a record of the recorded rotations to calculate average speed and the travelled distance

//Setup display components:   
const int numOfDigits = 4;
const int numOfSegments = 8;      
int digitPins[numOfDigits] = {6, 9, 10, 12}; // {Dig1, Dig2, Dig3, Dig4} 
int segmentPins[numOfSegments] = {7, 11, 15, 17, 18, 8, 14, 16}; // { A, B, C, D, E, F, G, DP}

// Initialize the display
sevSegmentDisplay segmentDisplay(numOfDigits, digitPins, numOfSegments, segmentPins);
void setup() {
  Serial.begin(9600);  // Starting serial communication

  //Print user info to serial
  Serial.print("Wheel circumference in m: ");
  Serial.println(wheelCircumference);

  pinMode(speedSensor, INPUT); // Initialize the speedSensor pin as an input:
  pinMode(navButton, INPUT); // Initialize the navbutton as input 
  //pinMode(cadenceSensor, INPUT); //Uncomment this line if you want the cadence sensor to be configured with or instead of the speed sensor

  //Attatch interrupt to SpeedSensor ( and cadence sensor ) 
  attachInterrupt(digitalPinToInterrupt(speedSensor), speedSensorInterrupt, LOW);
  //attachInterrupt(digitalPinToInterrupt(speedSensor), cadenceSensorInterrupt, LOW); //Uncomment this line for the cadence

  //Start the display and set the brightness to 100%
  segmentDisplay.begin();
  segmentDisplay.setBrightnessPercentage(100);
}
void loop() {

  lastClockTime = clockTime;
  clockTime = millis();
  updateDisplayValue(); // Call the update function for the display
  checkTime();

  // Check wheter navigation button was pressed:
  int navButtonState = digitalRead(navButton);

  if(navButtonState == HIGH) {
    Serial.println("Changing the view");

    // Change what we want to show on screen:
    if(navIndex == maxIndex) {
      navIndex = 0;
      displayNavigationBar(navIndex);
      return;
    }
    navIndex = navIndex+1;
    displayNavigationBar(navIndex);
  }

}
// Function for updating what value shown on the screen.
void updateDisplayValue() {
  switch (navIndex) {
    case 0:
      // Display avgSpeed
      displayValue(speed);
      break;
    case 1:
      // Display the current speed
      displayValue(avgSpeed);
      break;
    case 2:
      // Display the current distance travelled
      // The distance is only calculated when we are showing it on the screen 
      distance = rotationsCount * wheelCircumference;
      displayValue(distance / 1000); // Convert meters to km and display the result
      break;
    // Add more variables to be shown for example cadence
    default:
      break;
  }

}
// Function for displaying given value on the display with correct dataformat:
void displayValue(float value)
{
  char result[6];
  dtostrf(value, 6, 2, result);
  segmentDisplay.show(result, 6);
}

// Function for displaying a simple navindex on the screen based on navIndex.
// This is a workaround for now due to the way the ShowFunction is defined.
// There might be need to update the display library so that it would allow to show strings directly
void displayNavigationBar(int navIndex)
{
  char navBar[4];
  // Populate the navBar array based on navIndex
  for (int i = 0; i < 4; i++) {
    if (i <= navIndex) {
      navBar[i] = '_'; // Set the position indicated by navIndex to '_'
    } else {
      navBar[i] = ' '; // Set other positions to ' ' (space)
    }
  }
  // Show the navigation bar on the segment display
  segmentDisplay.show(navBar, 4);
  delay(200);
  return;

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

// Function for calculating and returning the speed in km/h based on time measurement in seconds
float calculateSpeed(unsigned long int spinTime) {

  // Calculates speed by dividing wheel circumference by time in seconds and converting to km/h
  float calculatedSpeed = wheelCircumference / (spinTime / 1000.0) * 3.6; // Speed in km/h
  return calculatedSpeed;

}
// Function for calculating rpm/cadence based on time in seconds
float calculateCadence(unsigned long int rotationTime) {
  // Calculate the rpm by dividing 60 seconds with the rotationTime
  float rpm = 60.0 / rotationTime;
  return rpm;
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

      // Update the number of rotations
      rotationsCount++;
      float new_speed = calculateSpeed(timeDiff); //Update the speed value
      speed = new_speed;
      // To keep up the average speed we implement a running  average formula to avoid overflow of the sum of speed values and constant large number division
      avgSpeed = avgSpeed + (new_speed-avgSpeed) / rotationsCount;
    }
  }
  //Take T1 Always when clock status is not enabled (Start new clock every time)
  T1 = millis();
  clockEnabled = true;
}

//This is an interrupt function which will measure the time of one rotation and calculate an RPM value after every rotation to produce cadence estimate
void cadenceSensorInterrupt() {
  //Take end time when clock status is enabled
  if(cadenceClockEnabled)
  {
    cadenceT2 = millis();
    float deltaTime = cadenceT2-cadenceT1;
    //If the timeDifference is greater then 1ms calculate speed from it
    if( deltaTime > 1 )
    {
      clockEnabled = false;
      // calculate the rpm based on the deltaTime:
      cadence = calculateCadence(deltaTime);
    }
  }
  //Take T1 Always when clock status is not enabled (Start new clock every time)
  cadenceT1 = millis();
  cadenceClockEnabled = true;
}

