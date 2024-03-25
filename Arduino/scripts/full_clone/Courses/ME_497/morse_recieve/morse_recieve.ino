/*
 * Jackson R. Sizer
 * jrs6356
 * 1/23/2018
 * Section 007
 * 
 * ME 497 - Mechatronics: Lab 1, morse_recieve
 */
int led = 2;        // Declares pin for Repeater LED
int reciever = 3;   // Declares reciever pin for signal from Transmitter
int error = 4;      // Declares return pin from Repeater back to Transmitter

void setup() {              // Initialize connection with board
  Serial.begin(9600);       // Opens communication with Serial Monitor
  pinMode(led,OUTPUT);      // Declares led pin as output
  pinMode(reciever,INPUT);  // Declares pin as input pin
  pinMode(error,OUTPUT);    // Declares pin as output pin
}

void loop() {               // Starts looping
  recieve();                // Calls recieve function
}
void recieve(){                       // Declares recieve function
  int state = digitalRead(reciever);  // Reads state of signal sent from Transmitter and declares it as an integer
  digitalWrite(led,state);            // Sets LED to the observed state
  digitalWrite(error,state);          // Returns signal to Transmitter with matched state
}

