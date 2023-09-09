int IRsensor = A0;  // declares sensor pin
float t;            // variable for current time
float dt;           // check condition to sample on a discrete interval
float st = 10;     // sample time
float voltage;      // voltage from sensor
float u1 = 0;       // u(K)
float u2 = 0;       // u(K-1)
float u3 = 0;       // u(K-2)
float yOld1 = 0;    // y(K-1)
float yOld2 = 0;    // y(K-2)
float ynew = 0;     // y(K)
float a1 = 1.0;     // filter coefficient
float a2 = -1.764;  // filter coefficient
float a3 = 0.789;   // filter coefficient
float b1 = 0.0063;  // filter coefficient
float b2 = 0.0125;  // filter coefficient
float b3 = 0.0063;  // filter coefficient
float tscaled;      // time scaled up to seconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // establish comm with serial monitor
  pinMode(IRsensor,INPUT);  // declare sensor as input
}

void loop() {
  while(10>tscaled){    // controls loop to read sensor for 10 seconds
    voltage = float(analogRead(IRsensor))*5.0/1024.0;   // grabs voltage of sensor and scales it appropriately
    t = millis();   // grabs current time
    dt = 0;         // resets time duration
    while(dt<st){   // makes code wait until sampling time has passed
      dt = millis() - t;    // updates time duration until time duration equal sampling time
    }   //while
    u3 = u2;  // u(K-2) = u(K-1)
    u2 = u1;  // u(K-1) = u(K)
    u1 = voltage; // u(K) = current voltage
    ynew = (1/a1)*(b1*u1 + b2*u2 + b3*u3 - a2*yOld1 - a3*yOld2);    // rearranged transfer function to calculate current output
    tscaled = t/1000.0;   // scales current time to seconds
    Serial.print(tscaled);Serial.print("  "); // prints time and a tab
    Serial.print(voltage);Serial.print("  "); // prints unfiltered sensor data and a tab
    Serial.println(ynew);   // prints filtered sensor data
    yOld2 = yOld1;  // y(K-1) = y(K)
    yOld1 = ynew;   // y(K) = current output
  }   // while
}   // void

