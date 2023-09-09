float A = 3.03;
float B = -5;
float angle;
float voltage;
float reading;

void setup() {
  // put your setup code here, to run once:

  //Initialize baud rate
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Read Analog 0
  reading = analogRead(A0);
  // Convert serial bin numbers to voltages
  voltage = reading * (3.3/ 1023.0);

  //Calculate angle
  angle = asin(A*voltage+B)*57.29578;


  //Print angle
  Serial.println(angle);
  
  //Wait 1 ms
  //delay (1);

}
