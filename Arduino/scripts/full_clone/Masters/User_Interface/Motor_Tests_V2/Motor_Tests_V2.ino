byte gui_sw = 2;
byte gui_b = 3;
byte gui_a = 4;
byte motor_b = 5;
byte motor_a = 6;
byte fg = 8;
byte pwm = 9;
byte dirPin = 14;

int speedOut = 0;
unsigned long speedInB = 0;
unsigned long speedInB1 = 0;
double speedIn = 0.0;
bool dirOut = true;

volatile int dir = 0;
volatile int dirM = 0;
volatile int enc_val = 0;
volatile int enc_valM = 0;
volatile bool bounce = false;
volatile bool sw_dir = 0;
volatile bool scroll = false;
volatile bool scrollM = false;
volatile bool a_state = false;
volatile bool b_state = false;
volatile bool a_stateM = false;
volatile bool b_stateM = false;
volatile bool fg_flag = false;
volatile bool meas_flag = false;
volatile bool meas_flag2 = false;
bool pid = true;

volatile unsigned long t = 0;
volatile unsigned long t_up = 0;
volatile unsigned long t_dn = 0;
volatile unsigned long pulse_dur = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long dt = 0;
long x1 = 0;
long x2 = 0;
long dx = 0;
double dx_b = 0.0;
double v_r = 0.0;
double v_rpm = 0.0;
double v_t = 25.0;
double v[10] = {0,0,0,0,0,0,0,0,0,0};
double v_rpm_max = 159.0;
double v_tm = 0.0;
double v_e = 0.0;
double v_e1 = 0.0;
double dv_e = 0.0;
double y_m = 0.0;
double y_m_frac = 0.0;
unsigned int y_s = 65535;
double kd = 0.01;
double kp = 0.1;
double ki = 30.0;
float r = 2.2;
float pi_val = 3.14159265358979;
int pd_N = 0;

void setup() {
  Serial.begin(250000);
  while(!Serial){}
  pinMode(fg, INPUT_PULLUP);
  pinMode(pwm, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(gui_a, INPUT_PULLUP);
  pinMode(gui_b, INPUT_PULLUP);
  pinMode(motor_a, INPUT_PULLUP);
  pinMode(motor_b, INPUT_PULLUP);
  pinMode(gui_sw, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(gui_a), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_b), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_sw), ensw, RISING);
  attachInterrupt(digitalPinToInterrupt(motor_a), encAm, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_b), encBm, CHANGE);
  attachInterrupt(digitalPinToInterrupt(fg), fg_isr, CHANGE);
  
  digitalWrite(dirPin,HIGH);
  analogWriteResolution(16);

  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  a_stateM = digitalRead(motor_a);
  b_stateM = digitalRead(motor_b);

  analogWrite(pwm,y_s);
  //testFunc2();
}

void loop() {
  readGUI2();
  //readGUI();
  //readMotor();
  //motorIn();
  //checkSpeed();
  //if(pid){
    //checkSpeed();
    //motorOut();
  //}
  //checkSpeed();
  //info();
  //delay(10);
}

void testFunc(){
  uint32_t z = 0b10110110110010100011111000001101; // 1011 | 0110 | 1100 | 1010 | 0011 | 1110 | 0000 | 1101
  for(int i=0;i<32;++i){
    if((i>0)&&((i%4)==0)){
      Serial.print(" | ");
    }
    Serial.print(1&(z>>(31-i)));
  }
  Serial.print("\n");
}

void testFunc2(){
  uint16_t x = 0b1100101001110110; // 1100 | 1010 | 0111 | 0110
  uint16_t y = 0b0011001111010111; // 0011 | 0011 | 1101 | 0111
  uint32_t z = (x << 16) | y;      // 1100 | 1010 | 0111 | 0110 | 0011 | 0011 | 1101 | 0111
  Serial.println("1100 | 1010 | 0111 | 0110");
  print16(x);
  Serial.println("\n\n0011 | 0011 | 1101 | 0111");
  print16(y);
  Serial.println("\n\n1100 | 1010 | 0111 | 0110 | 0011 | 0011 | 1101 | 0111");
  print32(z);
  Serial.print("\n");
}

void print16(uint16_t _x){
  for(int i=0;i<16;++i){
    if((i>0)&&((i%4)==0)){
      Serial.print(" | ");
    }
    Serial.print(1&(_x>>(15-i)));
  }
}

void print32(uint32_t _x){
  for(int i=0;i<32;++i){
    if((i>0)&&((i%4)==0)){
      Serial.print(" | ");
    }
    Serial.print(1&(_x>>(31-i)));
  }
}

void info(){
  Serial.print(speedInB);
  Serial.print("\t");
  Serial.print(speedIn);
  Serial.print(" rpm\t");
  Serial.print(x1);
  Serial.print("\t");
  Serial.print(x2);
  Serial.print("\t");
  Serial.print(dx);
  Serial.print("\t");
  Serial.print(t1);
  Serial.print("\t");
  Serial.print(t2);
  Serial.print("\t");
  Serial.print(dt);
  Serial.print("\t|\t");
  
  Serial.print(v_r);
  Serial.print("\t");
  Serial.print(v_rpm);
  Serial.print("\t");
  Serial.print(v_tm);
  Serial.print("\t|\t");
  
  Serial.print(v_e);
  Serial.print("\t");
  Serial.print(dv_e);
  Serial.print("\t|\t");
  
  Serial.print(y_m);
  Serial.print("\t");
  Serial.print(y_m_frac);
  Serial.print("\t");
  Serial.println(y_s);
}

void motorIn(){
  x1 = x2;
  t1 = t2;
  speedInB = 0;
  //delay(50);
  x2 = enc_valM;
  t2 = t;
  for(byte j=0;j<8;++j){
    speedInB += duePulseIn();
  }
  speedIn = 8.0*60.0*1000000.0/(45.0*6.0*2.0*(double)speedInB);
  dx = x2 - x1;
  dx_b = -2.0*pi_val*r*((double)dx)/(64.0*4.0);
  dt = t2 - t1;
  v_r = ((double)dx/(double)dt)*1000000.0;
  v_rpm = 60.0*v_r/(64.0*4.0);
  v_tm = v_r*2.0*pi_val*r/(64.0*4.0);
  v_e1 = v_e;
  v_e = v_t - v_tm;
  dv_e = 1000000.0*(v_e-v_e1)/dt;
  y_m = kp*v_e + kd*dv_e + ki*dx_b;
  y_m_frac = 1.0 - (y_m + v_t)/(v_rpm_max*2.0*pi_val*r/60.0);
  y_s = (unsigned int)(y_m_frac*65535.0);
  if(y_s>65535){
    y_s = 65535;
  }
  //speedOut = abs(enc_val);
}
void motorOut(){
  if(pd_N>5){
    analogWrite(pwm,y_s);
  }
  else{
    pd_N += 1;
  }
}

void checkSpeed(){
  for(int i=0;i<9;i++){
    v[9-i] = v[8-i];
  }
  v[0] = v_e;
  double _v_ave = 0.0;
  for(int i=0;i<10;i++){
    _v_ave = _v_ave + v[i];
  }
  _v_ave = abs(_v_ave/10.0);
  Serial.println(_v_ave);
  if(_v_ave<0.5){
    pid = false;
  }
  else{
    pid = true;
  }
}

uint32_t duePulseIn(){
  meas_flag = true;
  while(!fg_flag){}
  pulse_dur = t_dn - t_up;
  fg_flag = false;
  return pulse_dur;
}

void readGUI(){
  if(scroll){
    scroll = false;
    enc_val = enc_val + dir;
  }
  if(bounce){
    bounce = false;
    dirOut = !dirOut;
    digitalWrite(dirPin,dirOut);
  }
}

void readGUI2(){
  if(scroll){
    scroll = false;
    y_s = y_s + 100*dir;
    if(y_s>65535){
      y_s = 65535;
    }
    else if(y_s<0){
      y_s = 0;
    }
    Serial.println(y_s);
    analogWrite(pwm,y_s);
  }
}

void readMotor(){
  if(scrollM){
    scrollM = false;
    enc_valM = enc_valM + dirM;
  }
}

void encA(){
  //enc_f = 'A';
  if(!scroll){
    a_state = !a_state;
    if(a_state == b_state){
      dir = -1;
    }
    else{
      dir = 1;
    }
    scroll = true;
  }
}

void encB(){
  if(!scroll){
    //enc_f = 'B';
    b_state = !b_state;
    if(a_state == b_state){
      dir = 1;
    }
    else{
      dir = -1;
    }
    scroll = true;
  }
}

void encAm(){
  t = micros();
  a_stateM = !a_stateM;
  if(a_stateM == b_stateM){
    dirM = -1;
  }
  else{
    dirM = 1;
  }
  enc_valM += dirM;
}

void encBm(){
  t = micros();
  b_stateM = !b_stateM;
  if(a_stateM == b_stateM){
    dirM = 1;
  }
  else{
    dirM = -1;
  }
  enc_valM += dirM;
}

void ensw(){
  if(!bounce){
    bounce = true;
  }
}

void fg_isr(){
  if(meas_flag2){
    t_dn = micros();
    fg_flag = true;
    meas_flag2 = false;
  }
  if(meas_flag){
    t_up = micros();
    meas_flag2 = true;
    meas_flag = false;
  }
}
