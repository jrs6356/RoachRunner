#define button 2

int pot = A0;

int onLED = 5;
int offLED = 6;

int in1 = 7;
int in2 = 8;
int ena = 9;

int state = 0;
int potVal;
int rotSpeed = 0;

void setup() {
  pinMode(ena,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(pot,INPUT);
  //pinMode(button,INPUT);
  pinMode(onLED,OUTPUT);
  pinMode(offLED,OUTPUT);
  
  digitalWrite(onLED,LOW);
  digitalWrite(offLED,HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  attachInterrupt(digitalPinToInterrupt(button), buttonState, FALLING);

  Serial.begin(9600);
}

void loop() {
  tempFunc();
  delay(100);
}

void buttonState(){
  state = !state;
  Serial.println("Flip");
}

void tempFunc(){
  potVal = analogRead(pot);
  if(state==0){
    rotSpeed = 0;
    digitalWrite(offLED,HIGH);
  }
  else{
    rotSpeed = map(potVal, 0, 1023, 0 , 255);
    digitalWrite(offLED,LOW);
  }
  analogWrite(onLED, rotSpeed);
  analogWrite(ena, rotSpeed);
  //Serial.print(potVal);
  Serial.print(rotSpeed);
  Serial.print("  ");
  Serial.println(state);
}



/*
#define enA 9
#define in1 6
#define in2 7
#define button 4
int rotDirection = 0;
int pressed = false;
void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
void loop() {
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
  // Read button - Debounce
  if (digitalRead(button) == true) {
    pressed = !pressed;
  }
  while (digitalRead(button) == true);
  delay(20);
  // If button is pressed - change rotation direction
  if (pressed == true  & rotDirection == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    rotDirection = 1;
    delay(20);
  }
  // If button is pressed - change rotation direction
  if (pressed == false & rotDirection == 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    rotDirection = 0;
    delay(20);
  }
}
*/
