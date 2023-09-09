
int x ;
int t ;

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
int x = 0;
int t = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

while (t <2000){

int initial_value = analogRead(A0);
int filtered_value = analogRead(A1);
float initial_voltage = initial_value * (5.0 / 1023.0);
float filtered_voltage = filtered_value * (5.0 / 1023.0);
Serial.print(initial_voltage);
Serial.print(" ");
Serial.println(filtered_voltage);
t=t+1;
delay(1);

}
}
