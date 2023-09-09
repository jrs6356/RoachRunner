const byte ud_button = 2;
const byte cs_button = 3;
int ud_h = 4;
int ud_l = 5;
int cs_h = 6;
int cs_l = 7;

volatile byte ud_state_h = LOW;
volatile byte ud_state_l = HIGH;
volatile byte ud_states = HIGH;
volatile byte cs_state_h = LOW;
volatile byte cs_state_l = HIGH;
volatile byte cs_states = HIGH;

void setup() {
  pinMode(ud_h, OUTPUT);
  pinMode(ud_l, OUTPUT);
  pinMode(cs_h, OUTPUT);
  pinMode(cs_l, OUTPUT);
  //pinMode(ud_button, INPUT_PULLUP);
  //pinMode(cs_button, INPUT_PULLUP);
  digitalWrite(ud_h, ud_state_h);
  digitalWrite(ud_l, ud_state_l);
  digitalWrite(cs_h, cs_state_h);
  digitalWrite(cs_l, cs_state_l);
  
  attachInterrupt(digitalPinToInterrupt(ud_button), ud_state, HIGH);
  attachInterrupt(digitalPinToInterrupt(cs_button), cs_state, HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  setPins();
}
void ud_state(){
  ud_states = !ud_states;
}

void cs_state(){
  cs_states = !cs_states;
}

void setPins(){
  if(ud_states==1){
    ud_state_h = HIGH;
    ud_state_l = LOW;
  }
  else{
    ud_state_h = LOW;
    ud_state_l = HIGH;
  }
  if(cs_states==1){
    cs_state_h = HIGH;
    cs_state_l = LOW;
  }
  else{
    cs_state_h = LOW;
    cs_state_l = HIGH;
  }
  digitalWrite(ud_h, ud_state_h);
  digitalWrite(ud_l, ud_state_l);
  digitalWrite(cs_h, cs_state_h);
  digitalWrite(cs_l, cs_state_l);
  Serial.print(ud_states);Serial.print("    ");
  Serial.print(ud_state_h);Serial.print("   ");
  Serial.print(ud_state_l);Serial.print("       ");
  Serial.print(ud_states);Serial.print("    ");
  Serial.print(cs_state_h);Serial.print("   ");
  Serial.println(cs_state_l);
  delay(100);
}
