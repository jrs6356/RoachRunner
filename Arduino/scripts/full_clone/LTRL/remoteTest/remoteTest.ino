#include <RedBot.h>

RedBotSensor lvert = RedBotSensor(A0);  //speed stick
RedBotSensor rvert = RedBotSensor(A5);  //turn stick

float vel = 0;    // scalar corresponding to percent power
int JSdir;        // joystick direction
int lspeed = 0;   // speed of left motor
int rspeed = 0;   // speed of right motor

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Readings:");
  Serial.println("L Select\tL Horz\tL Vert\tR Select\tR Horz\tR Vert");
}

void loop() {
  theta();    // run motor speed calculation function
  //Serial.print(vel,6);Serial.print("\t");Serial.print(JSdir);Serial.print("\t");    //debug print
  Serial.print("1\t");Serial.print(lspeed);Serial.print("\t");                        //sends motor output via xbee
  Serial.print("2\t");Serial.println(rspeed);                                         //sends motor output via xbee
  delay(100);
}
void theta(){     //motor speed calculation function
  vel = ((lvert.read()/2.0) - 262.0)/262.0;   //produces a scalar value in range -1<x<1 from speed joystick position
  JSdir = rhorz.read()-502;       // reads direction joystick and normalizes readings about 0
  if(0>JSdir){                    // checks if joystick is to the left
    rspeed = -vel*(JSdir + 255);  // interpolates to find rspeed (scaled)
    lspeed = vel*255;             // maintains left at full speed (scaled)
  }
  else if(0<JSdir){               // checks if joystick is to the right
    rspeed = -vel*255;            // maintains right at full speed (scaled)
    lspeed = -vel*(JSdir - 255);  // interpolates to find lspeed (scaled)
  }
  else{       // if joystick is in neutral position
    //Serial.println("straight");
    lspeed = vel*255;     // full speed left scaled
    rspeed = -vel*255;    // full speed right scaled
  }
}
void sensorCheck(){
  //Serial.print(lsel.read());
  //Serial.print("\t");
  Serial.print(lhorz.read());
  Serial.print("\t");
  Serial.print(lvert.read());
  Serial.print("\t");
  //Serial.print(rsel.read());
  //Serial.print("\t");
  Serial.print(rhorz.read());
  Serial.print("\t");
  Serial.println(rvert.read());
  delay(100);
}

