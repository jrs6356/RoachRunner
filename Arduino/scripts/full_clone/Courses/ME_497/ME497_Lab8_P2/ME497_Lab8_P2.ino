#include "DualMC33926MotorShield.h" //declares p
#define encoder0PinA 2
#define encoder0PinB 3

volatile int encoder0Pos = 0;
int tmp_Pos = 1;

boolean A_set;
boolean B_set;

float t1;
float t2 = 0;
float dt;

DualMC33926MotorShield md;

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}

void setup()
{
  Serial.begin(500000);
  Serial.println("Dual MC33926 Motor Shield");
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);
  
  md.init();
}

void loop(){
  for (int i = 0; i <= 200; i++)
  {
    t1 = micros()/1000000.0;
    //dt = t1 - t2;
    //t2 = micros()/1000000.0;
    if (tmp_Pos != encoder0Pos) {
    Serial.print(t1,6);Serial.print("    "); Serial.println(encoder0Pos, DEC);
    tmp_Pos = encoder0Pos;
    }
    md.setM1Speed(400);
    stopIfFault();
    delay(2);
  }
  while(1){
  md.setM1Speed(200);
  }
}


void doEncoderA() {

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
void doEncoderB() {

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
