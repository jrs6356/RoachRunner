//Pins
int photoRes_Pin = A4;  // can be changed to whichever pin is used
int prop_Pin = A3;      // not sure if this is handled by whoever wrote prop module
//Constants
int light = 0;          // corresponds to phase 1: kiwi is on and outside of rocket
int propRun = 0;
int light_Hi = 350;     // value in full light <------
int light_Lo = 675;     // value in low light  <------MAKE SURE TO CHANGE THESE VALUES!!!!! THESE ARE BOUNDS!

void setup(){
  pinMode(prop_Pin,OUTPUT);
  Serial.begin(9600);  //establishes connection w/ Teensey board...idk if you do the same thing w/ nano
  //while (! Serial);
}
void loop(){
  sunSpot();
  delay(1000);
}
void sunSpot(){
  Serial.println(light);
  if(propRun==0){
    inOrOut();                         // function that determines which phase kiwi is in
  }
  else if(propRun==1){                 // kiwi withholds permission for prop to run until outside of rocket
    prop();
  }
}
void inOrOut(){
  int photo = analogRead(photoRes_Pin);
  Serial.println(photo);               // uncomment to calibrate
  if(light==0){                        // kiwi turned on, net yet inside rocket
    if(photo>light_Hi){                     // kiwi identifies shift from light to darkness
      light = 1;
    }
  }
  else if(light==1){
    if(photo<light_Lo){                     // kiwi reading darkness inside rocket, waiting to be ejected
      light = 2;                       // kiwi identifies shift from dark to light
    }
  }
  else if(light==2){                   // kiwi determines it is outside of the rocket
    delay(3000);
    propRun = 1;                       // kiwi gives prop function permission to run
  }
}
void prop(){                           // takes control of prop
    digitalWrite(prop_Pin, HIGH);      // these few lines are for demo-ing code, meant to be replaced by propellor module
    delay(1000);
    digitalWrite(prop_Pin, LOW);
    delay(1000);
}
