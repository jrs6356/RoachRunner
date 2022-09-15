#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;

int continue_button = 11;
int back_button = 12;

int page = 1;

void setup() {
  Serial.begin(115200);
  pinMode(continue_button, INPUT_PULLUP);
  pinMode(back_button, INPUT_PULLUP);
  freeRam();
}

void loop() {
  switch(page){
    case 1:
      page = page1();
      break;
    case 2:
      page = page2();
      break;
    case 3:
      page = page3();
      break;
  }
}

int page1(){
  Serial.println("-------------------------------------------------------");
  Serial.println("Page 1");
  freeRam();
  while(1){
    if(digitalRead(continue_button) == true){
      return 2;
    }else{}
  }
}

int page2(){
  Serial.println("-------------------------------------------------------");
  Serial.println("Page 2");
  freeRam();
  while(1){
    if(digitalRead(continue_button) == true){
      return 3;
    }
    else if(digitalRead(back_button) == true){
      return 1;
    }else{}
  }
}

int page3(){
  Serial.println("-------------------------------------------------------");
  Serial.println("Page 3");
  freeRam();
  while(1){
    if(digitalRead(back_button) == true){
      return 2;
    }else{}
  }
}

void freeRam(){
  char *heapend=sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi=mallinfo();
  Serial.print("\nDynamic ram used: ");
  Serial.println(mi.uordblks);
  Serial.print("Program static ram used ");
  Serial.println(&_end - ramstart); 
  Serial.print("Stack ram used ");
  Serial.println(ramend - stack_ptr); 
  Serial.print("My guess at free mem: ");
  Serial.println(stack_ptr - heapend + mi.fordblks);
} 
