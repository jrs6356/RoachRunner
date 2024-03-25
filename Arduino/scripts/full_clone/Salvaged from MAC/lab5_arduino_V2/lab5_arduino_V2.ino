#include <Servo.h>          // Include servo library
Servo myservo_ball;
Servo myservo_cam;

int wait=1;
int center=0;
int temp=0;
int midpoint=127; //midpoint of camera
int error=0; //difference between midpoint and center
int last_error=0;
int rate_error=0;
float currentTime=0;
float previousTime=0;
float PD=0;
float kp=5;        //PD values
float kd=10;
float delta=0;
float currentPos=90;
float nextPos=0;
float ballPos=90;
int sign = 1;
float dt = 0;
int span = 10;
float now;


void setup(){
  Serial.begin(9600);
  myservo_ball.attach(9); //tennis ball servo
  myservo_cam.attach(6);  //camera servo
  while (wait==1){
    Serial.println(10101);
    if(Serial.available()){
      if(Serial.read()>=253){
        wait=0;
        Serial.println(111);
        
      }
    }
  }
  now = millis();
}

void loop(){
  //Serial.println(333);
  follow();
  //ball();
  //delay(15);
}

void follow(){
  //record center position of tennis ball (center) without random -1
  temp = Serial.read();
  if (temp>=0){
  center=temp;
  //Serial.println(center);
  //delay(50); 
  }
  
  //tennis ball servo 
  dt = millis()-now;
  if(dt>span){
    now = millis();
    ball();
  }
  
  
  //PD controller
  error=midpoint-center;
  rate_error=(error-last_error)/(currentTime-previousTime);
  PD=kp*error+kd*rate_error;
  //Serial.println(PD);
  //What is max value of PD? may need changing
  //delta=map(PD,-700,700,-.5,.5);  //map output of PD controller to change in angle value (delta) between 0 and 10 degrees
  delta=(PD/700)*.5;
  nextPos=currentPos-delta;
  if(nextPos>180){
    nextPos=180;
  }
  if(nextPos<0){
    nextPos=0;
  }
  //Serial.println('      ');
  Serial.println(nextPos);
  myservo_cam.write(nextPos);
  currentPos=nextPos;
  //delay(5);
  
  
  last_error=error;
  previousTime=currentTime;
  currentTime=millis();
}

void ball(){
  if(ballPos==40){
    sign = 1;
  }
  if(ballPos==140){
    sign = -1;
  }
  ballPos = ballPos+.5*sign;
  myservo_ball.write(ballPos);
  //Serial.println(ballPos);
}
  

