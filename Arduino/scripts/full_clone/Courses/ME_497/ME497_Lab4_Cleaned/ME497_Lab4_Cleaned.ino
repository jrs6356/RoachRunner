/*
 * Jackson Sizer
 * jrs6356
 * Mechatronics Lab 4
 * Code for Part 1
 * 
 * This code utilizes modules borrowed from Arduino examples "Sweep.ino" and "Fade.ino"
 */

#include <Servo.h>      // include the servo library

Servo myservo;          // create servo object to control a servo, 12 servo objects can be created on most boards

//---------------------------------------------------------------------------------------------
int part = 2;           // insert number of part from report to run that portion of code (from 1-5)
int letter = 2;         // insert letter of part from report to run that portion of code (a=1, b=2)
//---------------------------------------------------------------------------------------------

int pos = 0;            // variable to store the servo position
int loops = 1;          // counter for tracking number of loops the code runs through (which translates into a number of servo sweeps)
int led = 10;           // declares pin for LED
int brightness = 0;     // declares initial brightness value for LED
int fadeAmount = 5;     // declares interval by which LED changes brightness

void setup() {          // initialize code
  myservo.attach(9);    // atteches the servo on pin 9 to the servo object
  pinMode(led,OUTPUT);  // declares the LED as an output pin
  Serial.begin(9600);   // establishes communication with serial monitor
}

void loop() {           // start looping
  if(2==part){          // checks to see which part of report you want to check out
    part2();            // runs that part of report
  }                     //  ||
  else if(3==part){     //  ||
    part3();            //  ||
  }                     //  ||
  else if(4==part){     // ditto
    part4();            //  ||
  }                     //  ||
  else if(5==part){     // _||_
    part5();            // \  /
  }                     //  \/
}
void part2(){                                     // declares the function for the second part of the report
  if(1==letter){                                  // checks to see which subsection you want to check out
    while(1){                                     // creates self contained loop
      for (pos = 0; pos <= 180; pos += 1) {       // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);                       // tell servo to go to position in variable 'pos'
        delay(15);                                // waits 15ms for the servo to reach the position
      }     // for
      for (pos = 180; pos >= 0; pos -= 1) {       // goes from 180 degrees to 0 degrees
        myservo.write(pos);                       // tell servo to go to position in variable 'pos'
        delay(15);                                // waits 15ms for the servo to reach the position
      }     // for
    }     // while
  }     // if
  else if(2==letter){                             // checks to see which subsection you want to check out
    while(1){                                     // creates self contained loop
      for (pos = 0; pos <= 180; pos += 1) {       // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);                       // tell servo to go to position in variable 'pos'
        delay(15);                                // waits 15ms for the servo to reach the position
        if((45==pos)||(90==pos)||(135==pos)){     // waits until servo is at an angle multiple of 45 degrees
          delay(300);                             // pauses for a bit
        }   // if
      }   // for
      for (pos = 180; pos >= 0; pos -= 1) {       // goes from 180 degrees to 0 degrees
        myservo.write(pos);                       // tell servo to go to position in variable 'pos'
        delay(15);                                // waits 15ms for the servo to reach the position
        if((45==pos)||(90==pos)||(135==pos)){     // waits until servo is at an angle multiple of 45 degrees
          delay(300);                             // pauses for a bit
        }   // if
      }   // for
    }   // while
  }   // if
}   // void
void part3(){                                 // declares function for 3rd part of lab
  while(1){                                   // creates self contained loop
    float t0 = millis();                      // grabs start time of loop in milliseconds
    Serial.print("Start time (millis): ");    // prints start time of loop to serial monitor
    Serial.println(t0);                       // prints actual value of start time to serial monitor
    for(loops = 1; loops<=15; loops+=1){      // runs 15 loops (or 30 sweeps)
      for (pos = 0; pos <= 180; pos += 1) {   // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(5);                             // waits 5ms for the servo to reach the position
      }   // for
      for (pos = 180; pos >= 0; pos -= 1) {   // goes from 180 degrees to 0 degrees
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(5);                             // waits 5ms for the servo to reach the position
      }   // for
      Serial.print("Loop number: ");          // print the current loop number to the serial monitor
      Serial.println(loops);                  // print the actual value of the current loop number to the serial monitor
    }   // for
    float t1 = millis();                      // grabs current time since function servo looping started
    float dt = t1-t0;                         // takes differences between times
    float servoSpeed = 2000*loops/(dt);       // uses differences in times to calculate slew rate
    Serial.print("End time (millis): ");      // print that stuff
    Serial.println(dt);                       // print the end time
    Serial.print("Servo speed is: ");         // print that stuff
    Serial.print(servoSpeed);                 // print the servo speed
    Serial.println(" sweeps/sec");            // print that stuff
  }   // while
}   // void
void part4(){                                 // declares the function for the fourth part of the lab
  while(1){                                   // creates self contained loop
    float t0 = millis();                      // grabs current time
    Serial.print("Start time (millis): ");    // prints that
    Serial.println(t0);                       // prints start time
    for(loops = 1; loops<=15; loops+=1){      // starts 15 cycle loop (30 sweeps)
      for (pos = 0; pos <= 180; pos += 1) {   // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(5);                             // waits 15ms for the servo to reach the position
      }     // for
      for (pos = 180; pos >= 0; pos -= 1) {   // goes from 180 degrees to 0 degrees
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(5);                             // waits 15ms for the servo to reach the position
      }     // for
      Serial.print("Loop number: ");          // prints that
      Serial.println(loops);                  // prints current loop number
    }     // for
    float t1 = millis();                      // grabs time at end of 30 sweeps
    float dt = t1-t0;                         // calculates time difference
    float servoSpeed = 2000*loops/(dt);       // calculates slew rate accomodating for half sweeps
    Serial.print("End time (millis): ");      // prints that
    Serial.println(t1);                       // prints time difference
    Serial.print("Servo speed is: ");         // prints that
    Serial.print(servoSpeed);                 // prints servo slew rate
    Serial.println(" sweeps/sec");            // prints that
  }   // while
}   // void
void part5(){                                 // declares function for 5th section of lab
  while(1){                                   // creates self contained loop
      for (pos = 0; pos <= 180; pos += 1) {   // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(15);                            // waits 15ms for the servo to reach the position
      }
      for (pos = 180; pos >= 0; pos -= 1) {   // goes from 180 degrees to 0 degrees
        myservo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(15);                            // waits 15ms for the servo to reach the position
      }
      analogWrite(led,brightness);            // sends PWM wave to led (voltage controlled)
      brightness += fadeAmount;               // adjusts the brightness of the led to dim
      if (brightness <= 0 || brightness >=255){   // keeps brightness within an operable range
        fadeAmount = -fadeAmount;             // reverses sign of fadeamount to keep brightness ossilating
      }   // if
  }     // while
}     // void
