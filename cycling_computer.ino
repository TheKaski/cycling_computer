
// Setting pins
const int speedSensor = 2;  // speedSensorPin

// Setting clock variables
boolean clockEnabled = false;  // Variable for determining the clock state
unsigned long int T1, T2, timeDiff;  // Time values

// User settings:
float wheelSize = 27.5; // Wheel size in inches
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m
int screenRefreshRate = 1000; // Screen refresh rate in ms

// Calculated values:
float speed = 0;  // Speed in km/h
float distance = 0;  // distance in km
float time_s = 0;  // time in seconds
float avgSpeed = 0;  // average speed in km/h


void setup() {
  Serial.begin(9600);  // Starting serial communication

  while(! Serial); //Wait for serial connection for debugging 

  // Print user info to serial
  Serial.print("Wheel circumference in m: ");
  Serial.println(wheelCircumference);

  pinMode(speedSensor, INPUT); // initialize the speedSensor pin as an input:

  //Attatch interrupt to SpeedSensor Pin
  attachInterrupt(digitalPinToInterrupt(speedSensor), speedSensorInterrupt, RISING);
}

void loop() {

  // Calculate speed
  calculateSpeed(timeDiff);

  printToSerial(speed); // print speed to serial
  delay(screenRefreshRate);
  
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
    clockEnabled = false;
  }

  //Take T1 Always when clock status is not enabled (Start new clock every time)
  T1 = millis();
  clockEnabled = true;
}

