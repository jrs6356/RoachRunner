#include <CoDrone.h>

void setup() {
    CoDrone.begin(115200);
    CoDrone.pair();
}

void loop() {
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);

  if (PAIRING == true){  

    CoDrone.setYaw( -1 * CoDrone.AnalogScaleChange(analogRead(22)) );    
    CoDrone.setThrottle( CoDrone.AnalogScaleChange(analogRead(23)) );   
    CoDrone.setRoll( -1 * CoDrone.AnalogScaleChange(analogRead(24)) ); 
    CoDrone.setPitch( CoDrone.AnalogScaleChange(analogRead(25)) ); 
    CoDrone.move(ROLL, PITCH, YAW, THROTTLE);   

  }
  if(!bt1 && !bt4 && bt8){ 
    CoDrone.Buzz(2000,2);
    CoDrone.setRoll(50);
    CoDrone.move(2);
  } 
  if(bt1 && !bt4 && !bt8){ 
    CoDrone.Buzz(2000,2);
    CoDrone.setRoll(-50);
    CoDrone.move(2);
  } 
  if (!bt1 && bt4 && !bt8){
    CoDrone.emergencyStop();
    CoDrone.Buzz(2000,2);
  }
}
