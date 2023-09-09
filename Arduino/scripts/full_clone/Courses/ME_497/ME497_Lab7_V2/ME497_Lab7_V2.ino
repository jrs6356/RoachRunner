#include "DualMC33926MotorShield.h"   //include motor driver library
#define encoder0PinA 2                // define interrupt pin a
#define encoder0PinB 3                // define interrupt pin b

volatile int encoder0Pos = 0;         // initial position of encoder
int tmp_Pos = 1;                      // stores previous position of encoder

boolean A_set;                        // check for direction of rotation
boolean B_set;                        // check for direction of rotation

DualMC33926MotorShield md;            // names the motor

void stopIfFault()                    // function to stop running if things go wrong
{
  if (md.getFault())                  // check for fault
  {
    Serial.println("fault");          // print fault
    while(1);                         // lock code
  }
}

void setup()
{
  Serial.begin(500000);             // establish comm
  Serial.println("Dual MC33926 Motor Shield");      
  pinMode(encoder0PinA, INPUT); // declare encoder pin as input
  pinMode(encoder0PinB, INPUT); // declare encoder pin as input

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);     // declare interrupt

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);     // declare interrupt
  
  md.init();    // initialize motor
}

void loop()
{
  for (int i = 0; i <= 400; i++) // ramp up
  {
    if (tmp_Pos != encoder0Pos) {   //check if encoder pos has changed
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC); // print encoder pos
    tmp_Pos = encoder0Pos;    // update with current pos
    }
    md.setM1Speed(i);   // set speed  of motor
    stopIfFault();      // check for fault
    if (abs(i)%200 == 100) // check value of speed
    {
      Serial.print("M1 current: ");   //print
      Serial.println(md.getM1CurrentMilliamps());   //print current recieved
    }
    delay(2);
  }
  for (int i = 400; i >= -400; i--)   //ramp down and repeat
  {
    if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -400; i <= 0; i++)   //ramp up and repeat
  {
    if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }

  for (int i = 0; i <= 400; i++)    //ramp up and repeat
  {
    if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 400; i >= -400; i--)   // ramp down and repeat
  {
    if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -400; i <= 0; i++)   // ramp up and repeat
  {
    if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
}
void doEncoderA() {   //interrupt routine moving forward

  // Low to High transition?
  if (digitalRead(encoder0PinA) == HIGH) {
    A_set = true;
    if (!B_set) {
      encoder0Pos = encoder0Pos + 1;
    }
  }

  // High-to-low transition?
  if (digitalRead(encoder0PinA) == LOW) {
    A_set = false;
  }

}


// Interrupt on B changing state
void doEncoderB() {     //interrupt routine moving backward

  // Low-to-high transition?
  if (digitalRead(encoder0PinB) == HIGH) {
    B_set = true;
    if (!A_set) {
      encoder0Pos = encoder0Pos - 1;
    }
  }

  // High-to-low transition?
  if (digitalRead(encoder0PinB) == LOW) {
    B_set = false;
  }
}
