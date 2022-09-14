const byte a1 = 5;
const byte b1 = 4;
const byte sw = 3;

volatile int k = 0;
volatile int j = 0;
volatile int dir = 0;
int pos = 0;
int push = 0;
volatile bool bounce = false;

void setup() {
  Serial.begin(115200);
  pinMode(a1, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(a1), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b1), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sw), ensw, RISING);
  Serial.println("New Encoder");
  delay(500);
}

void loop() {
  if(push !=j){
    push = j;
    Serial.print("Click: ");
    Serial.println(push);
    delay(500);
    bounce = false;   
  }
  if(pos != k){
    pos = k;
    Serial.print("Direction: ");
    Serial.print(dir);
    Serial.print("  Position: ");
    Serial.println(pos);
  }
  delay(5);
}

void encA(){
  if(digitalRead(a1) == true){
    if(digitalRead(b1) == true){
      dir = -1;
      k--;
    }
    else{
      dir = 1;
      k++;
    }
  }
  else{
    if(digitalRead(b1) == true){
      dir = 1;
      k++;
    }
    else{
      dir = -1;
      k--;
    }
  }
}

void encB(){
  if(digitalRead(b1) == true){
    if(digitalRead(a1) == true){
      dir = 1;
      k++;
    }
    else{
      dir = -1;
      k--;
    }
  }
  else{
    if(digitalRead(a1) == true){
      dir = -1;
      k--;
    }
    else{
      dir = 1;
      k++;
    }
  }
}

void ensw(){
  if(bounce == false){
    bounce = true;
    j=j+1;
  }
}
