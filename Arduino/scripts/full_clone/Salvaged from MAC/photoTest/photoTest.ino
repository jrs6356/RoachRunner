#define light_Hi 500    // value in no light <------
#define light_Lo 300    // value in full light <-----
#define photoRes_Pin A1
#define buzzer 13
int propRun = 0;
int light = 0;
void sunSpot(){
  while(propRun == 0){
    int photo = analogRead(photoRes_Pin);
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
    }                                    // function that determines which phase kiwi is in
    //if(propRun==1){                 // kiwi withholds permission for prop to run until outside of rocket
     // digitalWrite(propPin, HIGH);
    //}
  }
}
void setup() {
  Serial.begin(57600);
}
void loop(){
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
}
