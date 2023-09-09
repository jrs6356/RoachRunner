int speakerOut = 8;
#include "pitches.h"

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speakerOut,OUTPUT);

}

void loop() {
  annoy();
}
void sing(){
  for(int thisNote = 0; thisNote < 8; thisNote++){
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);

    noTone(8);
  }
  delay(500);
}
void annoy(){
  // put your main code here, to run repeatedly:
  int pulses = 1000;
  int pitch = random(700) + 50;
  for(int i = 0; i<pulses; i++){
    digitalWrite(speakerOut, HIGH);
    delayMicroseconds(pitch);
    digitalWrite(speakerOut, LOW);
    delayMicroseconds(pitch);
    Serial.println(pitch);
  }
  int wait = random(5000) + 1000;
  delay(wait);
}

