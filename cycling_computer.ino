
const int buttonPin = A2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

boolean clockEnabled = false; // Variable for  determining the clock state
boolean hasGoneLow = false; // Variable for detectinng the start of the clock

unsigned long int T1, T2; //Time values
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), speedSensorPulse, CHANGE);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // Start the clock when HIGH
  if (buttonState == HIGH and hasGoneLow == true) {
    //If clock is started take T2 and stop the clock
    if(clockEnabled == true){
      T2 = millis();
      clockEnabled = false;
      if(T2-T1 > 5){
        Serial.println(T2-T1); 
      }  
      hasGoneLow = false;
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
}

unsigned int speedSensorPulse() {

}

void calculateSpeedFromTime()
{
  
}
