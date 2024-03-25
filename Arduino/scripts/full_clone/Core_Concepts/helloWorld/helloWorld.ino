void setup() {
  // put your setup code here, to run once:

}

void loop() {
  speak();

}
void speak(){
  int pos = 5;
  if(pos == 0){
    Serial.print("Hello World!\n");
  }
  else if(pos>0){
    Serial.print("Blah blah blah\n");
  }
  delay(100);
  a = 1;
}

