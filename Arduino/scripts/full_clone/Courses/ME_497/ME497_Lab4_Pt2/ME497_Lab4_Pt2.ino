/*
 * Jackson Sizer
 * jrs6356
 * Mechatronics Lab 4
 * Code for Part 2
 */
int myServo = 9;              // declares pin for servo
int led = 10;                 // declares pin for led
int pos = 0;                  // variable to store the servo position
int pulse = 0;                // variable for pulse length
int brightness = 0;           // variable for LED brightness
int fadeAmount = 1;           // constant by which brightness changes. Set to 1 because must be an integer and must be less than 255. 

void setup() {                // initialize
  pinMode(myServo,OUTPUT);    // declares servo pin as output
  pinMode(led,OUTPUT);        // declares led pin as output
  Serial.begin(9600);         // establishes comm with serial monitor
}

void loop() {                               // loop
  for(pos = 0; pos <= 180; pos += 1){       // cycles through angle output from 0 to 180 degrees
    pulse = (pos*10) + 500;                 // creates a pulse length that corresponds to the desired angle
    digitalWrite(myServo,HIGH);             // set pin to high
    delayMicroseconds(pulse);               // keep pin high for a pulse length
    digitalWrite(myServo,LOW);              // turn pin off
    delay(5);                               // wait a bit
    brightness = brightness + fadeAmount;   // adjust LED to get bright
    analogWrite(led,brightness);            // send new brightness to LED
    Serial.println(brightness);             // print the current brightness to serial monitor.......for science?
  }   // for
  for(pos = 180; pos >= 0; pos -= 1){       // cycles through angle output from 180 to 0 degrees
    pulse = (pos*10) + 500;                 // creates pulse length that corresponds to the desired angle
    digitalWrite(myServo,HIGH);             // set pin to high
    delayMicroseconds(pulse);               // keep pin high for a pulse length
    digitalWrite(myServo,LOW);              // turn pin off
    delay(5);                               // wait a bit
    brightness = brightness - fadeAmount;   // adjust LED to dim
    analogWrite(led,brightness);            // send new brightness to LED
    Serial.println(brightness);             // do that
  }   // for
}   // void

