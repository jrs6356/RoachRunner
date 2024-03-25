void setup() {
  Serial.begin(9600);
  while(!Serial);
}
void loop() {
  derivatives();
  while(true);  
}
void derivatives(){
  String eq;
  int a;
  int b;
  int c;
  int d;
  int powA;
  int powB;
  int powC;
  int powD;
  char s[200];
  
  a = 4;
  b = -2;
  c = 1;
  d = 3;
  powA = 4;
  powB = 3;
  powC = 2;
  powD = 1;
  
  sprintf(s, "%dx^%d + %dx^%d + %dx^%d + %dx^%d", a, powA, b, powB, c, powC, d, powD);
  Serial.println(s);
}
