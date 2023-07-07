
// Setting pins
const int speedSensor = A2;  // speedSensorPin
const int ledPin = 13;  // the number of the LED pin

// Setting clock variables
boolean clockEnabled = false;  // Variable for determining the clock state
boolean hasGoneLow = false;  // Variable for detectinng the start of the clock
unsigned long int T1, T2, timeDiff;  // Time values

// Setting other variables
int speedSensorState = 0;  // variable for reading the speedSensor status

// User settings:
float wheelSize = 27.5; // Wheel size in inches
float wheelCircumference = wheelSize * 3.1415926535897932384626433832795 * 2.54; // Wheel circumference in cm


void setup() {
  Serial.begin(9600);  // Starting serial communication

  // Print user info to serial
  Serial.print("Wheel circumference in cm: ");
  Serial.println(wheelCircumference);

  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output:
  pinMode(speedSensor, INPUT); // initialize the speedSensor pin as an input:
}

void loop() {
  timeDiff = speedSensorPulse();
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

