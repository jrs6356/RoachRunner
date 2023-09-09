int blueLED = A3;
int photoRes = A9;
int onboardLED = 13;
void setup(){
  pinMode(blueLED,OUTPUT);
  //pinMode(photoRes,OUTPUT);
  pinMode(onboardLED,OUTPUT);
  Serial.begin(9600);
  while (! Serial);
}
void loop(){
  digitalWrite(onboardLED,HIGH);
  delay(5);
  digitalWrite(onboardLED,LOW);
  inOrOut();
  delay(1.5);
}
void inOrOut(){
  int photo = analogRead(photoRes);
  Serial.println(photo);
  //serial.println(Photo)
  if(photo > 500){
    digitalWrite(blueLED,HIGH);
  }
  else if(photo <= 500){
    digitalWrite(blueLED,LOW);
  }
}
