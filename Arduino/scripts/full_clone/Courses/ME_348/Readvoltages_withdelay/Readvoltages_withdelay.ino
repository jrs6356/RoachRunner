int reading;
float voltage ;
int n;

// the setup routine runs once when you press reset:
void setup() {
  //Initialize baud rate
  Serial.begin(9600);
  n=1;
}


void loop() {
  // Read Analog 0
  reading = analogRead(A0);
  // Convert serial bin numbers to voltages
  voltage = reading * (3.3 / 1023.0);
  //Display
  Serial.print(n);
  Serial.print("\t");
  Serial.println(voltage,5);

  //waits 2 seconds
  delay(2000);

  //Incrememts counter
  n=n+1;
}
