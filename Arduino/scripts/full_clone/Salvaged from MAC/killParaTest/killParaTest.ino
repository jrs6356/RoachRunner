int para = 8;
int KILL = 9;
void setup(){
  Serial.begin(9600);
  pinMode(para,INPUT);
  pinMode(KILL,INPUT);
}
void loop(){
  whatValPara();
  whatValKILL();
  delay(1000);
}
void whatValPara(){
  boolean paraOpen = digitalRead(para);
  if(paraOpen == HIGH){
    Serial.println("Parachute is open");
  }
  else{
    Serial.println("Parachute not deployed");
  }
}
void whatValKILL(){
  boolean kill_hit = digitalRead(KILL);
  if(kill_hit == HIGH){
    Serial.println("That was easy");
  }
  else{
    Serial.println("<3");
  }
}
