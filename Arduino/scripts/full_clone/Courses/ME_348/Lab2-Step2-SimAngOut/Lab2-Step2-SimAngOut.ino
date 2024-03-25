

int KitchenPin = 6; // Kitchen Light connected to pin  6 (LED 1)
int LivingRoomPin = 8; // Living Room Light connected to pin  8 (LED 2)
int OvenPin = 12; // Red LED for Oven connected to pin  12 (LED 3)


void setup() {
// put your setup code here, to run once:
// Set baud rate
Serial.begin(9600);
// Define each Pin as input or output.
pinMode(KitchenPin, OUTPUT); // sets the KitchenPin to be an output
pinMode(LivingRoomPin, OUTPUT); // sets the LivingRoomPin to be an output
pinMode(OvenPin, OUTPUT); // sets the OvenPin to be an output
}

void loop() {
   // Set digital output signal as 1 = HIGH.
  digitalWrite(LivingRoomPin, HIGH);
  digitalWrite(OvenPin, HIGH);
  
  // put your main code here, to run repeatedly:
  for(int brightness = 0; brightness <=255; brightness ++)
  {
  analogWrite(KitchenPin, brightness);
  delay(20);
  }

   for(int brightness = 255; brightness >=0; brightness --)
  {
  analogWrite(KitchenPin, brightness);
  delay(20);
  }

 
}
