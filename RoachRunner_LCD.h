#ifndef _ROACHRUNNERLCD_H_INCLUDED
#define _ROACHRUNNERLCD_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include "Digipot.h"

//Command Headers
#define clr    0x01  //0b00000001;   // Clear Display
#define rtn    0x02  //0b00000010;   // Return Home
#define ems    0x07  //0b00000111;   // Entry Mode Set
#define disp   0x0f  //b00001111;    // Display On/Off
#define crsr   0x1c  //0b00011100;   // Cursor or Display Shift
#define fcn    0x3c  //0b00111100;   // Function Set
#define cgram  0x7f  //0b01111111;   // Set CGRAM Address
#define ddram  0xff  //0b11111111;   // Set DDRAM Address
#define data   0xff  //0b11111111;   // Write Data to RAM

//Command Options
#define INCR       0x06  //0b10;     // Increment by 1
#define DECR       0x04  //0b00;     // Decrement by 1
#define SHIFT_EN   0x05  //0b1;      // Enable Full Display Shift
#define SHIFT_DIS  0x04  //0b0;      // Disable Full Display Shift
#define D_ON       0x0c  //0b100;    // Display On
#define D_OFF      0x08  //0b000;    // Display Off
#define C_ON       0x0a  //0b10;     // Cursor On
#define C_OFF      0x08  //0b00;     // Cursor Off
#define B_ON       0x09  //0b1;      // Blink On
#define B_OFF      0x08  //0b0;      // Blink Off
#define SC_ON      0x18  //0b1000;   // Set Cursor Control On
#define SC_OFF     0x10  //0b0000;   // Set Cursor Control Off
#define RIGHT      0x14  //0b100;    // Set Shift Direction Right
#define LEFT       0x10  //0b000;    // Set Shift Direction Left
#define DL_8       0x30  //0b10000;  // Data Length 8-Bit
#define DL_4       0x20  //0b00000;  // Data Length 4-Bit
#define N_2        0x28  //0b1000;   // Numbers of Display Line = 2, (1/16 Duty)
#define N_1        0x20  //0b0000;   // Numbers of Display Line = 1, (1/8 or 1/11 Duty)
#define F_11       0x24  //0b100;    // Font: 5x11
#define F_8        0x20  //0b000;    // Font: 5x8

class LCD {
public:
  LCD(int _cs_lcd, int _cs_pot, int _rs, int _inc, int _ud);
  LCD(int _cs_lcd, int _cs_pot, int _rs, int _inc, int _ud, int _upLED, int _downLED);
  void init(void);
  void contrast(int cont);
  void clear(void);
  void home(void);
  void command(byte val);
  void setCursor(bool csr, bool blnk);
  void writeData(unsigned char val);
  void writeRow(String txt, int r);
  void writeChar(char c, int r, int col);
  void print(String txt);
  void scroll(String txt, int r, int c, int x, int y);
  void makeExtraChars(void);
  void loadingBar(void);

  //Create a Digipot
  Digipot dpot;

  //Pin Assignments
  byte cs;
  byte cs_pot;
  byte rs;
  byte inc;
  byte ud;

  //Command Variables
  uint8_t ID;
  uint8_t SH;
  uint8_t D;
  uint8_t C;
  uint8_t B;
  uint8_t SC;
  uint8_t RL;
  uint8_t DL;
  uint8_t N;
  uint8_t F;

  //Command Variables Combined
  uint8_t fcn_set;
  uint8_t disp_set_1;
  uint8_t ems_set;
  uint8_t disp_set_2;
  uint8_t crsr_set;

  //Character Map
  uint8_t row0[20] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93};
  uint8_t row1[20] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3};
  uint8_t row2[20] = {0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7};
  uint8_t row3[20] = {0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7};

  //Special Characters
  uint8_t clock12 = 0x00;
  uint8_t clock3 = 0x01;
  uint8_t clock6 = 0x02;
  uint8_t clock9 = 0x03;
  uint8_t backArrow = 0x04;
  uint8_t ellipsis = 0x05;
  uint8_t menuUp = 0x06;
  uint8_t menuDown = 0x07;
  uint8_t arrowR = 0x7E;
  uint8_t arrowL = 0x7F;
  uint8_t degree = 0xDF;
  uint8_t alpha = 0xE0;
  uint8_t beta = 0xE2;
  uint8_t epsilon = 0xE3;
  uint8_t mew = 0xE4;
  uint8_t sigma = 0xE5;
  uint8_t rho = 0xE6;
  uint8_t sqrt = 0xE8;
  uint8_t theta = 0xF2;
  uint8_t infinity = 0xF3;
  uint8_t omega = 0xF4;
  uint8_t Sigma = 0xF6;
  uint8_t pi = 0xF7;
  uint8_t divideMath = 0xFD;
  uint8_t blank = 0xFE;
  uint8_t black = 0xFF;
  uint8_t x0 = 0x00;
  uint8_t x1 = 0x01;
  uint8_t x2 = 0x02;
  uint8_t x3 = 0x03;
  uint8_t x4 = 0x04;
  uint8_t x5 = 0x05;
  uint8_t x0r = 0xBA;
  uint8_t x1r = 0xDB;
  uint8_t x2r = 0x06;
  uint8_t x3r = 0x07;
  boolean symbolLib = false;
  boolean loadingLib = false;

  //Buffers
  uint8_t buf;
  uint8_t ch;
  uint8_t addr;
  uint8_t addr_com;
  int len;
  char blrb[18] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

  //Counters, Switch States, and Variables
  int brightness;
  int j;
  int con_init;
  int row_length;
  int dt;
  int row;
  int col;

};

#endif