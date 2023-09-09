
/*
  Jackson Sizer
  jrs6356
  1/16/2018
  Section 001P 
 
  Heartbeat
  Blinks an LED to resemble a heartbeat (short on short off medium on long off)
  Repeats 10 times.
 
  This code utilizes "Blink", a preloaded example code in the Arduino library.
 */
 
int led = 10;
int loops = 1;
int brightness = 255;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
int lifespan = 0;

void setup() {                
  pinMode(led, OUTPUT);     
  Serial.begin(9600);     //allows me to write to the Serial monitor
}

void loop() {
    heartbeat();
}
void heartbeat(){
  brightness = 255;
  if(100 > lifespan){
    lifespan = lifespan + 1;    // increases value of lifespan until out of range of if condition
    Serial.println(lifespan);
    for(loops = 1; loops<=50; loops+=1){
      analogWrite(led, brightness);
      brightness = brightness - fadeAmount;
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
      delay(10);
    }
    analogWrite(led, 0);
    delay(100);
    brightness = 255;
    for(loops = 1; loops<=50; loops+=1){
      analogWrite(led, brightness);
      brightness = brightness - fadeAmount;
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
      delay(30);
    }
    analogWrite(led, 0);
    delay(500);
    /*
    digitalWrite(led, HIGH);    // turn the LED on (HIGH is the voltage level)
    delay(100);                 // wait for half a second
    digitalWrite(led, LOW);     // turn the LED off by making the voltage LOW
    delay(100);                 // wait for half a second
  
    digitalWrite(led, HIGH);    // turn the LED on (HIGH is the voltage level)
    delay(500);                // wait for a second
    digitalWrite(led, LOW);     // turn the LED off by making the voltage LOW
    delay(1000);                // wait for a second and half 
    */
  }
}

