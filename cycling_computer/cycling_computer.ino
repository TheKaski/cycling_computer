#include "Display.h"
#include <avr/dtostrf.h>
#include <ArduinoBLE.h>

// Active sensors
const bool speedSensorActive = true;
const bool cadenceSensorActive = false;

// Pins
const int speedSensor = 5; //Adjust these values based on your circuit
const int navButton = 4;
const int cadenceSensor = 0; //CadenceSensor can be configured by using this pinvariable.

// Clock variables for speed
bool clockEnabled = false;  // Variable for determining the speedSensor clock state
volatile unsigned long int T1, T2, timeDiff; // Time values used by the speedsensor

// Clock variables for cadence
bool cadenceClockEnabled = false;
volatile unsigned long int cadenceT1, cadenceT2; // Time values used by the cadenceSensor

// Navigation indexes:
volatile int navIndex = 0; 
const int maxIndex = 2; // adjust this variable based on how many variables you want to show on screen.

// Bicycle  specific settings:
const float wheelSize = 27.5; // Wheel size in inches you want to adjust this based on your bike
const float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m calculated based on your wheelsize

// Additional settings
const int screenRefreshRate = 1000; // Screen refresh rate in ms
const int timeOut = 8000; // Timeout for detecting stopped state which will set the current speed back to 0

// Metrics:
volatile float speed = 0;  // Speed in km/h
volatile float distance = 0;  // distance tracked in meters
volatile float avgSpeed = 0;  // average speed in km/h
volatile float time_s = 0;  // time in seconds is unused for now.
volatile float cadence = 0;
volatile unsigned long rotationsCount = 0; // keeping a record of the recorded rotations to calculate average speed and the travelled distance
volatile unsigned long crankRotationsCount = 0;

// NavButton:
bool buttonPressed = false;
unsigned long buttonPressTime = 0;

//Setup display variables:   
const int numOfDigits = 4;
const int numOfSegments = 8;      
int digitPins[numOfDigits] = {6, 9, 10, 12}; // {Dig1, Dig2, Dig3, Dig4} 
int segmentPins[numOfSegments] = {7, 11, 15, 17, 18, 8, 14, 16}; // { A, B, C, D, E, F, G, DP}

// BLE Service:
BLEService speedCadenceService("1816"); // Standard Cycling Speed and Cadence Service
BLECharacteristic speedCadenceMeasurementChar("2A5B", BLENotify, 11); // Measurement characteristics
BLEDevice central;

// Initialize the display
sevSegmentDisplay segmentDisplay(numOfDigits, digitPins, numOfSegments, segmentPins);
void setup() {
  Serial.begin(9600);  // Start serial communication

  // Wait for BLE service to start:
  if (!BLE.begin()) {
    Serial.println("Starting ble service failed");
    while(1);
  }

  // Set device name and advertise the service
  BLE.setLocalName("SpeedCadenceSensor");
  BLE.setAdvertisedService(speedCadenceService);
  // Add characteristics to the service
  speedCadenceService.addCharacteristic(speedCadenceMeasurementChar);
  BLE.addService(speedCadenceService);
  // Start advertising
  BLE.advertise();

  //Print user info to serial
  Serial.print("Wheel circumference in m: ");
  Serial.println(wheelCircumference);

  pinMode(navButton, INPUT); // Initialize the navbutton as input 
 
  //Attatch interrupt to SpeedSensor if set to active:
  if(speedSensorActive) 
  {
    pinMode(speedSensor, INPUT); // Initialize the speedSensor pin as an input:
    attachInterrupt(digitalPinToInterrupt(speedSensor), speedSensorInterrupt, LOW);
  }
  
  //Attatch interrupt to CadenceSensor if set to active:
  if(cadenceSensorActive)
  {
    pinMode(cadenceSensor, INPUT);
    attachInterrupt(digitalPinToInterrupt(cadenceSensor), cadenceSensorInterrupt, LOW); //Uncomment this line for the cadence
  }

  //Start the display
  segmentDisplay.begin();
}
void loop() {

  // Check for BLE central device connection
  if (!central) {
    central = BLE.central();
    if (central) {
      Serial.print("Connected to central: ");
      Serial.println(central.address());
    }
  } else if (!central.connected()) {
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    central = BLEDevice(); // Reset central device
  }

  // Check wheter the wheel is stopped or not
  checkTime();
  // Show the correct variable on screen
  updateDisplayValue(); 
  //Handle any navigation buttonpresses:
  handleNavigationButton();

  //If the central device is connected, send data:
  if(central.connected()) {
    sendSpeedCadenceData();
  }
}
// Function for updating what value shown on the screen.
void updateDisplayValue() 
{
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

// Function for handling the navigation logic of the navButtonButton
void handleNavigationButton()
{

  // Here we "record" the button press when detected:
  if(digitalRead(navButton) == HIGH && !buttonPressed)
  {
    buttonPressed = true;
    buttonPressTime = millis();
    return;
  }
  // When button goes to LOW when it is pressed we take second time and estimate the result:
  if(digitalRead(navButton) == LOW && buttonPressed)
    {
      // Calculate how long was the press:
      unsigned long pressDuration = millis() - buttonPressTime;
      if(pressDuration < 1000 )
      { // Short press for navigation:
        handleNavigationPress();
      } else if (pressDuration > 2000)
      { // Long press is for resetting variables
        resetMetrics();
      }
      // Always set the flag back to false
      buttonPressed = false;
    }
}

void handleNavigationPress()
{
  if(navIndex == maxIndex) {
          navIndex = 0;
          displayNavigationBar(navIndex);
          return;
        }
        navIndex = navIndex+1;
        displayNavigationBar(navIndex);
}
//Function for resetting the metrics on runtime
// Used to clear for example metrics recorded before starting a ride (taking bike out and setting it up)
void resetMetrics()
{
  speed, distance, avgSpeed, time_s, cadence = 0;  
  rotationsCount, crankRotationsCount = 0;
  //Reset the clocks
  clockEnabled = false;
  cadenceClockEnabled = false;
  // Show the reset by re begining the diplay:
  segmentDisplay.begin();
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
    //If elapsedTime is greater than timeout value we set the speed back to zero
    if(T2-T1 > timeOut)
    {
     speed = 0;
    }
  }
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

//Interrupt function for the speedsensor. It will start a clock by taking time T1 when magnetic pulse is detected and set clockenabled to True
// When the clock is enabled and the pulse is detected second time the wheel has turned one full revolution and we take new time and calculate the difference.
// based on the difference the speed can be calculated and updated based on new value.
// The T2 value is then used as new value for T1 and the clock is enabled again.
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
    //when the clock is enabled use the T2 trigger time to start a new clock:
    T1 = T2;
    clockEnabled = true;
    return;
  }
  //Take T1 Always when clock status is not enabled
  T1 = millis();
  clockEnabled = true;
}

//Interrupt function for cadence sensor. Measures the time of one full revolution and estimates the cadence in rpm with calculateCadence function.
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
    crankRotationsCount++;
    cadenceT1 = cadenceT2;
    cadenceClockEnabled;
    return;
  }
  //Take T1 Always when clock status is not enabled (Start new clock when clock is not enabled)
  cadenceT1 = millis();
  cadenceClockEnabled = true;
}

// Functions for BLE communication:

//Function for sending data for Speedsensor, Cadence sensor or both when they are set to be active
//This function uses the BLE standard speedCadence characteristics to communicate to third party BLE fitnness applications
void sendSpeedCadenceData() {
  uint8_t flags = 0;
  int offset = 1; // Start at the second byte for data

  uint8_t speedCadenceData[11] = {0};

  if(speedSensorActive) {
    flags |= 0x01; // Set speed data present flag
    speedCadenceData[offset++] = (uint8_t)(rotationsCount & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((rotationsCount >> 8) & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((rotationsCount >> 16) & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((rotationsCount >> 24) & 0xFF);
    speedCadenceData[offset++] = (uint8_t)(T1 & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((T1 >> 8) & 0xFF);
  }

  if(cadenceSensorActive) {
    flags |= 0x02;
    speedCadenceData[offset++] = (uint8_t)(crankRotationsCount & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((crankRotationsCount >> 8) & 0xFF);
    speedCadenceData[offset++] = (uint8_t)(cadenceT1 & 0xFF);
    speedCadenceData[offset++] = (uint8_t)((cadenceT1 >> 8) & 0xFF);
  }

  // Set flags byte
  speedCadenceData[0] = flags;

  // Write data to characteristic
  speedCadenceMeasurementChar.writeValue(speedCadenceData, offset);

}

