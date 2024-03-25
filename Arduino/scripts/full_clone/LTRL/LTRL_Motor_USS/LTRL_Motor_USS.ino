int trigPin = 8;
int echoPin = 7;
int MOTOR_IN1 = 9;
int MOTOR_IN2 = 10;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  for (int i=0; i<255; i++) {
    analogWrite(MOTOR_IN2, i);
    delay(10);
  }
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration/2) / 29.1;
  Serial.println(distance);
  if (distance < 4){
    for (int i=255; i>=0; i--) {
        analogWrite(MOTOR_IN2, i);
        delay(10);
    }
    digitalWrite(MOTOR_IN2, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTOR_IN1, i);
      delay(10);
    }
    while (distance < 4) {
      delay(1000);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      Serial.print("While        ");
      Serial.println(distance);
    }
    for (int i=255; i>=0; i--) {
      analogWrite(MOTOR_IN1, i);
      delay(10);
    }
    digitalWrite(MOTOR_IN1, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTOR_IN2, i);
      delay(10);
    }
  }
    analogWrite(MOTOR_IN2, 254);
    delay(100);
}
