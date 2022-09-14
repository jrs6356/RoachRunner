#include "ST7565.h"

int ledPin =  13;    // LED connected to digital pin 13
const byte a1 = 5;
const byte b1 = 4;
const byte sw = 3;
const byte bt1 = 11;
const byte bt2 = 12;

uint8_t k = 0x19;
byte alpha = 0b00000000;
volatile int dir = 0;
volatile bool scroll = false;
volatile bool bounce = false;
volatile bool bounce2 = false;
volatile bool bounce3 = false;
volatile int a_state = false;
volatile int b_state = false;

// the LCD backlight is connected up to a pin so you can turn it on & off
//#define BACKLIGHT_LED 4

// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 6 - LCD reset (RST)
// pin 5 - LCD chip select (CS)
ST7565 glcd(9, 8, 52, 53, 51);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

// a bitmap of a 16x16 fruit icon
const static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

void set_brightness(uint8_t val) {
    Serial.print("\t val:\t");Serial.print(val);
    glcd.st7565_command(CMD_SET_VOLUME_FIRST);
    alpha = CMD_SET_VOLUME_SECOND | (val & 0x3f);
    Serial.print("\t");printBinary(alpha);Serial.println("");
    glcd.st7565_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
    //glcd.st7565_command(alpha);
}

void hold_config(){
  while(!bounce){
    if(scroll){
      scroll = false;
      if(dir==-1){
        if(k==0){
          k=0;
        }
        else{
          k = k - 1;
        }
      }
      else if(dir==1){
        if(k==64){
          k=64;
        }
        else{
          k = k + 1;
        }
      }
      Serial.print("Brightness: ");Serial.print(k, DEC);Serial.print("\t");Serial.print(k, HEX);Serial.print("\t");printBinary(k);Serial.println("");
      set_brightness(k);
      glcd.clear();
      testdrawchar();
      glcd.display();
    }
  }
  bounce = false;
}

void setup()   {                
  Serial.begin(115200);
  pinMode(a1, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(a1), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b1), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sw), ensw, RISING);
  attachInterrupt(digitalPinToInterrupt(bt1), but, RISING);
  attachInterrupt(digitalPinToInterrupt(bt2), but2, RISING);
  Serial.println("Brightness: DEC HEX     BIN");
#ifdef __AVR__
  Serial.print(freeRam());
#endif

  // turn on backlight
  //pinMode(BACKLIGHT_LED, OUTPUT);
  //digitalWrite(BACKLIGHT_LED, LOW);

  // initialize and set the contrast to 0x18
  Serial.println("Step 1");
  glcd.clear();
  glcd.begin(k);//0x18);
  while(!bounce2){}
  Serial.println("Step 2");
  bounce2=false;
  //glcd.clear();
  testdrawchar();
  glcd.display(); // show splashscreen
  while(!bounce){
    if(bounce2){
      delay(2000);
      glcd.clear();
      delay(2000);
      testdrawchar();
      glcd.display();
      bounce2 = false;
    }
  }
  bounce = false;
  hold_config();
  delay(2000);
  glcd.clear();
  
  // draw a single pixel
  glcd.setpixel(10, 10, BLACK);
  glcd.display();        // show the changes to the buffer
  hold_config();
  delay(2000);
  glcd.clear();

  // draw many lines
  testdrawline();
  glcd.display();       // show the lines
  hold_config();
  delay(2000);
  glcd.clear();

  // draw rectangles
  testdrawrect();
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw multiple rectangles
  testfillrect();
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw mulitple circles
  testdrawcircle();
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw a black circle, 10 pixel radius, at location (32,32)
  glcd.fillcircle(32, 32, 10, BLACK);
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw the first ~120 characters in the font
  testdrawchar();
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw a string at location (0,0)
  glcd.drawstring(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation");
  glcd.display();
  hold_config();
  delay(2000);
  glcd.clear();

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  
}

void loop()                     
{}

#ifdef __AVR__
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 
#endif

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  randomSeed(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(128);
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      glcd.drawbitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
    }
    glcd.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      glcd.drawbitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, 0);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > 64) {
  icons[f][XPOS] = random(128);
  icons[f][YPOS] = 0;
  icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}


void testdrawchar(void) {
  for (uint8_t i=0; i < 168; i++) {
    glcd.drawchar((i % 21) * 6, i/21, i);
  }    
}

void testdrawcircle(void) {
  for (uint8_t i=0; i<64; i+=2) {
    glcd.drawcircle(63, 31, i, BLACK);
  }
}


void testdrawrect(void) {
  for (uint8_t i=0; i<64; i+=2) {
    glcd.drawrect(i, i, 128-i, 64-i, BLACK);
  }
}

void testfillrect(void) {
  for (uint8_t i=0; i<64; i++) {
      // alternate colors for moire effect
    glcd.fillrect(i, i, 128-i, 64-i, i%2);
  }
}

void testdrawline() {
  for (uint8_t i=0; i<128; i+=4) {
    glcd.drawline(0, 0, i, 63, BLACK);
  }
  for (uint8_t i=0; i<64; i+=4) {
    glcd.drawline(0, 0, 127, i, BLACK);
  }

  glcd.display();
  delay(1000);

  for (uint8_t i=0; i<128; i+=4) {
    glcd.drawline(i, 63, 0, 0, WHITE);
  }
  for (uint8_t i=0; i<64; i+=4) {
    glcd.drawline(127, i, 0, 0, WHITE);
  }
}

void encA(){
  scroll = true;
  a_state = digitalRead(a1);
  b_state = digitalRead(b1);
  if(a_state==HIGH){
    if(b_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
  else{
    if(b_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
}

void encB(){
  scroll = true;
  a_state = digitalRead(a1);
  b_state = digitalRead(b1);
  if(b_state==HIGH){
    if(a_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
  else{
    if(a_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
}

void ensw(){
  if(!bounce){
    bounce = true;
  }
}

void printBinary(byte inByte)
{
  for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
}

void but(){
  if(!bounce2){
    bounce2 = true;
  }
}

void but2(){
  bounce3 = !bounce3;
  if(bounce3){
    glcd.st7565_command(CMD_SET_DISP_NORMAL);
  }
  else{
    glcd.st7565_command(CMD_SET_DISP_REVERSE);
  }
}
