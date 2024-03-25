/*
 * Jackson R. Sizer
 * jrs6356
 * 1/23/2018
 * Section 007
 * 
 * ME 497 - Mechatronics: Lab 1, morse_code
 */
//Declare globals between here and setup

//------------MESSAGE---------------
String message = "bird"; //This is the message that will be communicated. Lowercase letters, numbers, and spaces are acceptable. Uppercase and special characters are unacceptable.

//--------------PINS----------------
int led1 = 2;         // This is the LED activated by the transmitter board
int transmitter = 3;  // This is the pin out to the repeater
int reciever = 4;     // This is the pin for the signal sent from the repeater back to the transmitter
int transStat = 5;    // This allocates an input pin to read the status of the signal sent along the transmitter pin (read status of an output)

//----CONSTANTS/SCALARS/COUNTERS----
int errorNum = 0;     // Creates an initial value for the number of errors detected between the two boards
int counter = 0;      // This counter allows morse() to read through the message one character at a time
int tu = 100;         // Time unit
int tu1 = 1*tu;       // Length of a dot or time between dash and dot within a letter
int tu3 = 3*tu;       // Length of a dash or time between letters
int tu7 = 4*tu;       // Modified length of a space. A space is 7 time units, but there is already a 3 time unit pause after each letter, so only 4 time units are needed to complete a space

void setup() {                  // Where values are initialized
  pinMode(led1,OUTPUT);         // Allows to send signal out to LED
  pinMode(transmitter,OUTPUT);  // Allows to send signal out to Repeater
  pinMode(reciever,INPUT);      // Allows to read signal returned from Repeater
  pinMode(transStat,INPUT);     // Allows to read signal sent to LED as input
  Serial.begin(9600);           // Allows communication with Serial Monitor at 9600 Hz
}

void loop() {     // Begin looping
  morse();        // Calls the function that blinks the LED
  error();        // Calls the function that counts the number of errors
  //delay(500);   // Debugging delay to help read Serial Monitor
}
void error(){                               // Function for counting errors
  int sendStat = digitalRead(transStat);    // State of LED
  int repeatStat = digitalRead(reciever);   // State of Repeater LED
  //Serial.println(sendStat);               // Debugging print statement to monitor LED status
  //Serial.println(repeatStat);             // Debugging print statement to monitor Repeater status
  if(sendStat != repeatStat){               // Checks if there is an error
    errorNum = errorNum+1;                  // Adds a tally to the tally count of errors
    Serial.print("ERROR number ");          // Prints ERROR number to the Serial monitor
    Serial.println(errorNum);               // Prints the int error number to the previous line and adds a new line
  }
  else{
    //Serial.println("All good!");          // Debugging print statement
  }
}
void morse(){                                 // Function for blinking LED
  if(1>counter){
    Serial.print("This is the full message: "); // Prints that stuff in the quotes
    Serial.println(message);                    // Prints the message declared at the very start
  }
  int messageLength = message.length();       // Determines the length in characters of the message
  //Serial.println(messageLength);            // Debugging print statement
  if(messageLength > counter){                // Condition that moves through the message character by character
    int letter = message.charAt(counter);     // Extracts the current letter of the message
    counter = counter + 1;                    // Increases counter by 1
    Serial.print("Current letter: ");         // Prints that stuff
    if(97==letter){                           // Checks ASCII value of current character to see if correlates to ASCII "a"
      a();                                    // Calls function "a()"
      delay(tu3);                             // Dash length pause between letters
    }                                         //    ||
    else if(98==letter){                      //    ||
      b();                                    //    ||
      delay(tu3);                             //    ||
    }                                         //   Ditto
    else if(99==letter){                      //    ||
      c();                                    //    ||
      delay(tu3);                             //    ||
    }                                         //   _||_
    else if(100==letter){                     //   \  /
      d();                                    //    \/
      delay(tu3);
    }
    else if(101==letter){
      e();
      delay(tu3);
    }
    else if(102==letter){
      f();
      delay(tu3);
    }
    else if(103==letter){
      g();
      delay(tu3);
    }
    else if(104==letter){
      h();
      delay(tu3);
    }
    else if(105==letter){
      i();
      delay(tu3);
    }
    else if(106==letter){
      j();
      delay(tu3);
    }
    else if(107==letter){
      k();
      delay(tu3);
    }
    else if(108==letter){
      l();
      delay(tu3);
    }
    else if(109==letter){
      m();
      delay(tu3);
    }
    else if(110==letter){
      n();
      delay(tu3);
    }
    else if(111==letter){
      o();
      delay(tu3);
    }
    else if(112==letter){
      p();
      delay(tu3);
    }
    else if(113==letter){
      q();
      delay(tu3);
    }
    else if(114==letter){         //....letters.....
      r();
      delay(tu3);
    }
    else if(115==letter){
      s();
      delay(tu3);
    }
    else if(116==letter){
      t();
      delay(tu3);
    }
    else if(117==letter){
      u();
      delay(tu3);
    }
    else if(118==letter){
      v();
      delay(tu3);
    }
    else if(119==letter){
      w();
      delay(tu3);
    }
    else if(120==letter){
      x();
      delay(tu3);
    }
    else if(121==letter){
      y();
      delay(tu3);
    }
    else if(122==letter){
      z();
      delay(tu3);
    }
    else if(48==letter){        //.....numbers.....
      zero();
      delay(tu3);
    }
    else if(49==letter){
      one();
      delay(tu3);
    }
    else if(50==letter){
      two();
      delay(tu3);
    }
    else if(51==letter){
      three();
      delay(tu3);
    }
    else if(52==letter){
      four();
      delay(tu3);
    }
    else if(53==letter){
      five();
      delay(tu3);
    }
    else if(54==letter){
      six();
      delay(tu3);
    }
    else if(55==letter){
      seven();
      delay(tu3);
    }
    else if(56==letter){
      eight();
      delay(tu3);
    }
    else if(57==letter){
      nine();
      delay(tu3);
    }
    else if(32==letter){        // This is a space between words
      Serial.println(" ");
      delay(tu7);
    }
  }
}
void dash(){                        // This is a function for a dash length pulse
  //error();                          // Checks for error before lighting
  digitalWrite(led1,HIGH);          // Turns LED on
  digitalWrite(transmitter,HIGH);   // Turns wire to Repeater on
  delay(2);
  error();                          // Checks for error during lighting
  delay(tu3);                       // Holds for 3 time units
  digitalWrite(led1,LOW);           // Turns LED off
  digitalWrite(transmitter,LOW);    // Turns wire to Repeater off
  delay(2);
  error();                          // Checks for error after lighting
}
void dot(){                         // This is a function for a dot length pulse
  //error();                          // Checks for error before lighting
  digitalWrite(led1,HIGH);          // Turns LED on
  digitalWrite(transmitter,HIGH);   // Turns wire to Repeater on
  delay(2);
  error();                          // Checks for error during lighting
  delay(tu1);                       // Holds for 1 time unit
  digitalWrite(led1,LOW);           // Turns LED off
  digitalWrite(transmitter,LOW);    // Turns wire to Repeater off
  delay(2);
  error();                          // Checks for error after lighting
}
void a(){               // Function for morse translation for "a"
  Serial.println("a");  // Prints a
  dot();                // Calls dot function
  delay(tu1);           // 1 time unit pause between dot or dash
  dash();               // Calls dash function
}
void b(){               //    ||
  Serial.println("b");  //    ||
  dash();               //    ||
  delay(tu1);           //    ||
  dot();                //    ||
  delay(tu1);           //    ||
  dot();                //   Ditto
  delay(tu1);           //    ||
  dot();                //    ||
}                       //    ||
void c(){               //    ||
  Serial.println("c");  //   _||_
  dash();               //   \  /
  delay(tu1);           //    \/
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void d(){
  Serial.println("d");
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void e(){
  Serial.println("e");
  dot();
}
void f(){
  Serial.println("f");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void g(){
  Serial.println("g");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void h(){
  Serial.println("h");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void i(){
  Serial.println("i");
  dot();
  delay(tu1);
  dot();
}
void j(){
  Serial.println("j");
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void k(){
  Serial.println("k");
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void l(){
  Serial.println("l");
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void m(){
  Serial.println("m");
  dash();
  delay(tu1);
  dash();
}
void n(){
  Serial.println("n");
  dash();
  delay(tu1);
  dot();
}
void o(){
  Serial.println("o");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void p(){
  Serial.println("p");
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void q(){
  Serial.println("q");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void r(){
  Serial.println("r");
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void s(){
  Serial.println("s");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void t(){
  Serial.println("t");    //...letters...
  dash();
}
void u(){
  Serial.println("u");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void v(){
  Serial.println("v");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void w(){
  Serial.println("w");
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void x(){
  Serial.println("x");
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void y(){
  Serial.println("y");
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void z(){
  Serial.println("z");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void one(){
  Serial.println("1");      //...numbers...
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void two(){
  Serial.println("2");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void three(){
  Serial.println("3");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
void four(){
  Serial.println("4");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dash();
}
void five(){
  Serial.println("5");
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void six(){
  Serial.println("6");
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void seven(){
  Serial.println("7");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void eight(){
  Serial.println("8");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
  delay(tu1);
  dot();
}
void nine(){
  Serial.println("9");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dot();
}
void zero(){
  Serial.println("0");
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
  delay(tu1);
  dash();
}
