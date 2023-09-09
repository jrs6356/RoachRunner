#include <RedBot.h>
RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);

int buttonPin = 12;
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
int lCount;
int rCount;

float t1;
float t0 = 0;
float dt;

void setup(){
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop(void){
  if (digitalRead(buttonPin) == LOW)
      {
        encoder.clearEnc(BOTH);
        motors.drive(250);
      }
  t1 = millis()/1000.0;
  dt = t1 - t0;
  lCount = encoder.getTicks(LEFT);
  rCount = encoder.getTicks(RIGHT);
  if(.001<dt){
    Serial.print(t1,4);
    Serial.print("\t");
    Serial.print(lCount);
    Serial.print("\t");
    Serial.println(rCount);
  }
}

