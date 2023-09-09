int sensorPinF = 2;
int sensorPinR = 3;
int speakerOut = 8;
int pulses = 100;

float t0 = millis();
float t1;
float dt;

float xi1 = 0.0;      // u(K)
float xi2 = 0.0;      // u(K-1)
float xi3 = 0.0;      // u(K-2)
float xOld1 = 0.0;    // y(K-1)
float xOld2 = 0.0;    // y(K-2)
float xnew = 0.0;     // y(K)

float yi1 = 0.0;      // u(K)
float yi2 = 0.0;      // u(K-1)
float yi3 = 0.0;      // u(K-2)
float yOld1 = 0.0;    // y(K-1)
float yOld2 = 0.0;    // y(K-2)
float ynew = 0.0;     // y(K)

float a1 = 1.0;       // filter coefficient
float a2 = -0.8773;   // filter coefficient
float a3 = 0.3111;    // filter coefficient
float b1 = 0.1084;    // filter coefficient
float b2 = 0.2169;    // filter coefficient
float b3 = 0.1084;    // filter coefficient

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speakerOut,OUTPUT);
}

void loop() {
  if(millis()<500000){
    t1 = millis();
    dt = t1-t0;
    if(dt > 10){
      xi3 = xi2;  // u(K-2) = u(K-1)
      xi2 = xi1;  // u(K-1) = u(K)
      xi1 = analogRead(sensorPinF); // u(K) = current voltage
      xnew = (1/a1)*(b1*xi1 + b2*xi2 + b3*xi3 - a2*xOld1 - a3*xOld2);
      xOld2 = xOld1;  // y(K-1) = y(K)
      xOld1 = xnew;   // y(K) = current output
      //Serial.print(xnew);
      //Serial.print("    ");

      yi3 = yi2;  // u(K-2) = u(K-1)
      yi2 = yi1;  // u(K-1) = u(K)
      yi1 = analogRead(sensorPinR); // u(K) = current voltage
      ynew = (1/a1)*(b1*yi1 + b2*yi2 + b3*yi3 - a2*yOld1 - a3*yOld2);
      yOld2 = yOld1;  // y(K-1) = y(K)
      yOld1 = ynew;   // y(K) = current output
      //Serial.println(ynew);
      t0 = millis();
    }
    float voltF = xnew * 5/1023;    // Voltage output from front sensor
    float voltR = ynew * 5/1023;    // Voltage output from rear sensor
    float dist;                     // declares variable for global distance
    float distF = -15 + 1/(.0026 + .0109 * voltF) + 40;   // linearizing/globalizing equation converting front sensor voltage to distance
    float distR = -15 + 1/(.0026 + .0109 * voltR) - 20;   // linearizing/globalizing equation converting rear sensor voltage to distance
    if(distF>70){
      dist = distF;
    }
    if((distF<70)&&(distF>60)){
      float w1 = pow(2,(60-distR));
      float w2 = 1 - w1;
      dist = distF*w2 + w1*distR;
    }
    if(distR<60){
      dist = distR;
    }
    Serial.println(dist);
    int pitch = (dist / 150)*200 + 1000;    // generates a frequency based on distance
    //Serial.println(pitch);
    for(int i = 0; i<pulses; i++){          // cycles through a short sound corresponding to the frequency
      digitalWrite(speakerOut, HIGH);
      delayMicroseconds(pitch);
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(pitch);
    }
  }
}
