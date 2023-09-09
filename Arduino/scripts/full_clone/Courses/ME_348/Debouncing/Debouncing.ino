/*  
THis code is modified from  http://www.arduino.cc/en/Tutorial/Debounce to include a non-debounced LED 
  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi
  modified 8 Sep 2020
  by Matthew Erdman

  This example code is in the public domain.
*/

int ButtonPin = 2;

int DebouncedPin = 12;
int NotDebouncedPin = 13;

int Debounced_State = HIGH;
int NotDebounced_State = HIGH;

int Debounced_ButtonState;
int NotDebounced_ButtonState;

int Debounced_LastState = LOW;
int NotDebounced_LastState = LOW;

unsigned long LastDebounceTime = 0;
unsigned long DebounceDelay = 50;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(ButtonPin, INPUT);
  pinMode(DebouncedPin, OUTPUT);
  pinMode(NotDebouncedPin, OUTPUT);

  digitalWrite(DebouncedPin, Debounced_State);
  digitalWrite(NotDebouncedPin, NotDebounced_State);
}

void loop() {
  // put your main code here, to run repeatedly:
  int CurrentReading = digitalRead(ButtonPin);

  if (CurrentReading != Debounced_LastState) {
    LastDebounceTime = millis();
  }

  if ((millis() - LastDebounceTime) > DebounceDelay) {
    if (CurrentReading != Debounced_ButtonState){
      Debounced_ButtonState = CurrentReading;
      if (Debounced_ButtonState == HIGH){
        Debounced_State = !Debounced_State;
      }
    }
  }



  if (CurrentReading != NotDebounced_ButtonState){
      NotDebounced_ButtonState = CurrentReading;
      if (NotDebounced_ButtonState == HIGH){
        NotDebounced_State =!NotDebounced_State;
      }
    }

  digitalWrite(DebouncedPin, Debounced_State);
  digitalWrite(NotDebouncedPin, NotDebounced_State);

  Debounced_LastState = CurrentReading;
  NotDebounced_LastState = CurrentReading;
  
}
