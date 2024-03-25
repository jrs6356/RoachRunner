int IRsensor = A1;  // declares sensor pin (raw data)
int filter = A0;    // declares pin for filtered data
float t;            // variable for current time
float dt;           // check condition to sample on a discrete interval
float st = 10;     // sample time
float voltage;      // voltage from sensor
float voltageFilt;  // float corresponding to filtered output
float tscaled;      // time scaled up to seconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // establish comm with serial monitor
  pinMode(IRsensor,INPUT);  // declare sensor as input
  pinMode(filter,INPUT);    // delcare filtered data pin as input
}

void loop() {
  while(10>tscaled){    // controls loop to read sensor for 10 seconds
    voltage = float(analogRead(IRsensor))*5.0/1024.0;   // grabs voltage of sensor and scales it appropriately
    voltageFilt = float(analogRead(filter))*5.0/1024.0;
    t = millis();   // grabs current time
    dt = 0;         // resets time duration
    while(dt<st){   // makes code wait until sampling time has passed
      dt = millis() - t;    // updates time duration until time duration equal sampling time
    }   //while
    tscaled = t/1000.0;   // scales current time to seconds
    Serial.print(tscaled);Serial.print("  "); // prints time and a tab
    Serial.print(voltage);Serial.print("  "); Serial.println(voltageFilt);    // print raw and filtered data
  }   // while
}   // void

