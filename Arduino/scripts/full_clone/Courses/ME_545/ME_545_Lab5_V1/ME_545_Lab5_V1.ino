#include <Servo.h>

Servo myservo_ball;
Servo myservo_cam;
int pos = 30;
int ref = 125;

int kp = 1;
int kd = 1;
int ki = 1;


void setup() {
  myservo_ball.attach(9);
  myservo_cam.attach(6);
  myservo_ball.write(pos);
  myservo_cam.write(pos);
  Serial.begin(9600);

}

void loop() {
  tracker();

}

void tracker(){
  int error = ref - comm();
  cam_pos = kp + kd*x + ki/x;
  
}

int comm(){
  int val = Serial.read()
  if(val>0){
    int ang = val;
  }
}

