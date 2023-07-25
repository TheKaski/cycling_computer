
// Setting pins
const int speedSensor = 2;  // speedSensorPin
const int ledPin = 13;  // the number of the LED pin

// Setting clock variables
boolean clockEnabled = false;  // Variable for determining the clock state
unsigned long int T1, T2, timeDiff;  // Time values

// User settings:
float wheelSize = 27.5; // Wheel size in inches
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m

// Calculated values:
float speed;  // Speed in km/h
float distance;  // distance in km
float time;  // time in seconds
float avgSpeed;  // average speed in km/h


void setup() {
  Serial.begin(9600);  // Starting serial communication

  while(! Serial); //Wait for serial connection for debugging 

  // Print user info to serial
  Serial.print("Wheel circumference in m: ");
  Serial.println(wheelCircumference);

  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output:
  pinMode(speedSensor, INPUT); // initialize the speedSensor pin as an input:

  //Attatch interrupt to SpeedSensor Pin
  attachInterrupt(digitalPinToInterrupt(speedSensor), speedSensorInterrupt, RISING);
}

void loop() {
  //timeDiff = speedSensorPulse(); //Non InterruptVersion
  printToSerial(timeDiff);   
  
}

void printToSerial(unsigned int long value) {
  if ( value > 100 ) { //100 ms 27.5 inch wheel max speed is 78..km/h 
    Serial.println(value);
  }
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

