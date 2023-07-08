
// Setting pins
const int speedSensor = A2;  // speedSensorPin
const int ledPin = 13;  // the number of the LED pin

// Setting clock variables
boolean clockEnabled = false;  // Variable for determining the clock state
boolean hasGoneLow = false;  // Variable for detecting the start of the clock
unsigned long int T1, T2, timeDiff;  // Time values

// Setting other variables
int speedSensorState = 0;  // variable for reading the speedSensor status

// User settings:
float wheelSize = 27.5; // Wheel size in inches
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54 / 100; // Wheel circumference in m

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
  if ( value > 5 ) {
    Serial.println(value);
  }
}
unsigned long int speedSensorPulse() {
  unsigned long int diff;
  // read the state of the pushbutton value:
  speedSensorState = digitalRead(speedSensor);
  // Start the clock when HIGH
  if (speedSensorState == HIGH and hasGoneLow == true) {
    //If clock is started take T2 and stop the clock
    if(clockEnabled == true){
      T2 = millis();
      clockEnabled = false;
      hasGoneLow = false;
      diff = T2-T1;
    }
    T1 = millis();
    clockEnabled = true;
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    hasGoneLow = true;
  }
  return diff;
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

