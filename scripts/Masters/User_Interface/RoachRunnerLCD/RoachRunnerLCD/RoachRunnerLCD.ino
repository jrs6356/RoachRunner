#include <SPI.h>

#define lcd_CS1B 51
#define lcd_RST 53
#define lcd_A0 52
//#define lcd_SCL
//#define lcd_SDA
#define button1 10
#define button2 11

#define COMMAND (true)
#define DATA (false)

int push1 = 0;
int push2 = 0;
int flip1 = 0;
int flip2 = 0;

typedef enum{
  disp_on = 0xAF,
  disp_off = 0xAE,
  disp_strt_ln_set = 0x40,
  pg_addr_set = 0xB0,
  col_addr_set_ub = 0x10,
  col_addr_set_lb = 0x00,
  adc_sel_nor = 0xA0,
  adc_sel_rev = 0xA1,
  disp_nor = 0xA6,
  disp_rev = 0xA7,
  disp_all_pt_nor = 0xA4,
  disp_all_pt_all = 0xA5,
  bias_19 = 0xA2,
  bias_17 = 0xA3,
  rmw = 0xE0,
  end_disp = 0xEE,
  reset_disp = 0xE2,
  com_o_md_sel_nor = 0xC0,
  com_o_md_sel_rev = 0xC8,
  pwr_ctrl_set = 0x28,
  vo_reg_int_res_rat_set = 0x20,
  el_vol_md_set = 0x81,
  el_vol_reg_set = 0x00,
  slp_md_set_sl = 0xAC,
  slp_md_set_nor = 0xAD,
  bstr_rat_set_234x = 0xF8,
  bstr_rat_set_5x = 0x01,
  bstr_rat_set_6x = 0x03,
}commands;

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(1);
  }
  SPI.begin();
  pinMode(lcd_CS1B, OUTPUT);
  pinMode(lcd_A0, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(lcd_RST, OUTPUT);
  
  digitalWrite(lcd_CS1B, LOW);
  digitalWrite(lcd_A0, LOW);
  
  digitalWrite(lcd_RST, LOW);
  delay(500);
  digitalWrite(lcd_RST, HIGH);
  
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
  delay(1);
  
  write8(bias_17);
  write8(adc_sel_nor);
  write8(com_o_md_sel_nor);
  write8(disp_strt_ln_set);
  write8(vo_reg_int_res_rat_set);
  write8(el_vol_md_set);
  write8(pwr_ctrl_set);
}

void loop(){
  write8(disp_all_pt_all);
  Serial.println("All points on");
  delay(300);
  write8(disp_on);
  Serial.println("Display on");
  delay(300);
  write8(disp_off);
  Serial.println("Display off");
  delay(300);
  
  write8(disp_all_pt_nor);
  Serial.println("All points off");
  delay(300);
  write8(disp_on);
  Serial.println("Display on");
  delay(300);
  write8(disp_off);
  Serial.println("Display off");
  delay(300);
}

//void loop() {
//  push1 = digitalRead(button1);
//  push2 = digitalRead(button2);
//  if (push1 == 1){
//    Serial.println("Push");
//    button_switch1();
//    delay(500);
//  }
//  if (push2 == 1){
//    Serial.println("Push");
//    button_switch2();
//    delay(500);
//  }
//}

void button_switch1(){
  if (flip1 == 0){
    flip1 = 1;
    Serial.println("On");
    write8(disp_on);
    //digitalWrite(lcd_RST, HIGH);
  }
  else{
    flip1 = 0;
    Serial.println("Off");
    digitalWrite(lcd_RST, LOW);
    write8(disp_off);
  }
}

void button_switch2(){
  if (flip2 == 0){
    flip2 = 1;
    Serial.println("Show All Points");
    write8(disp_all_pt_all);
  }
  else{
    flip2 = 0;
    Serial.println("All Points Off");
    write8(disp_all_pt_nor);
  }
}

void write8(byte value){
  digitalWrite(lcd_CS1B, LOW);
  //SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
  //SPI.transfer(reg & 0x7F);
  SPI.transfer(value);
  Serial.println(value);
  //SPI.endTransaction();
  digitalWrite(lcd_CS1B, HIGH);
}
