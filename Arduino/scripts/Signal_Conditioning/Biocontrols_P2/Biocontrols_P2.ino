#define encoder0PinA 2
#define encoder0PinB 3

#define button 2 %CONFLICT

%-------Encoder Input--------
volatile int encoder0Pos = 0;
int tmp_Pos = 1;

boolean A_set;
boolean B_set;
int a = 1;
int i;
int t0;
int t1;
int dt;
int count = 10000;
float loopSpeed;
%----------------------------

%--------Motor Output---------
int pot = A0;

int onLED = 5;
int offLED = 6;

int in1 = 7;
int in2 = 8;
int ena = 9;

int state = 0;
int potVal;
int rotSpeed = 0;
%------------------------------


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

  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);   % Declare these as interrupt routines?

  Serial.begin(9600);
}

void loop() {
  motorOut();
  delay(100);
}

void buttonState(){
  state = !state;
  Serial.println("Flip");
}

void motorOut(){
  potVal = analogRead(pot);
  if(state==0){
    rotSpeed = 0;
    digitalWrite(offLED,HIGH);
  }
  else{
    rotSpeed = map(potVal, 0, 1023, 0 , 255);
    digitalWrite(offLED,LOW);
  }
  trueSpeed = encoderIn();
  desiredSpeed = (rotSpeed/255)*(100*2*3.14/60)*.025;
  error = desiredSpeed - trueSpeed;
  y = error*kp + error/kd;
  analogWrite(onLED, rotSpeed);
  analogWrite(ena, rotSpeed);
  //Serial.print(potVal);
  Serial.print(rotSpeed);
  Serial.print("  ");
  Serial.println(state);
}
void encoderIn(){
  if(1==a){
    t0 = micros();  
    for(i = 0; i<=count; i++){  
      if (digitalRead(encoder0PinA) == HIGH) {
          A_set = true;
          if (!B_set) {
            encoder0Pos = encoder0Pos + 1;
          }
        }
      if (digitalRead(encoder0PinA) == LOW) {
          A_set = false;
        }
      }
      t1 = micros();
      dt = t1 - t0;
      loopSpeed = float(dt)/float(count);
      Serial.println(dt);
      Serial.println(loopSpeed);
      a = 0;
  }
}
