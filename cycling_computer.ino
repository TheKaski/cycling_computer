
const int buttonPin = A2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

boolean clock_is_on = false; // Variable for  determining the clock state
boolean has_gone_low = false; // Variable for detectinng the start of the clock

unsigned long int T1, T2, timeDiff; //Time values
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}
void loop() {
  timeDiff = speedSensorPulse();
}

unsigned long int speedSensorPulse() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // Start the clock when HIGH
  if (buttonState == HIGH and has_gone_low == true) {
    //If clock is started take T2 and stop the clock
    if(clock_is_on == true){
      T2 = millis();
      clock_is_on = false;
      if(T2-T1 > 5){
        Serial.println(T2-T1); 
      }  
      has_gone_low = false;
    }
    T1 = millis();
    clock_is_on = true;
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    has_gone_low = true;
  }
<<<<<<< Updated upstream
}

void calculate_speed_from_time()
=======

  return T2-T1;
}

void calculateSpeedFromTime()
>>>>>>> Stashed changes
{
  
}
