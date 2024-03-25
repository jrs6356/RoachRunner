#define encoder0PinA 2
#define encoder0PinB 3

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

void setup() {
  // put your setup code here, to run once:
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  Serial.begin (9600);
}

void loop() {
    // put your main code here, to run repeatedly:
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
