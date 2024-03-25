#include <CoDrone.h>

int FLAG; // The flag used for determining if you've activated the sequence
unsigned long StartTime; // Your start time used for checking how much time has passed

void setup() {
    CoDrone.begin(115200);
    CoDrone.pair();

    CoDrone.takeoff();
    CoDrone.goToHeight(750);
    delay(200);
}

void loop() {
    byte bt1 = digitalRead(11);
    byte bt8 = digitalRead(18);

    if (bt1) {
        CoDrone.emergencyStop();
        FLAG = 0; 
    }
 
    //If right sensor is triggered, start the timer to run the sequence
    //if (!bt1 && bt8) { 
        FLAG = 1;
        //CoDrone.Buzz(5000, 7);
        delay(200);
        StartTime = millis();
    //}
    
//----------------------------------------------------------------------

    if ( FLAG == 1) {
        // In the first 1.5 seconds, roll to left
        if (millis() - StartTime < 1000) {
            CoDrone.setRoll(0);
            CoDrone.setPitch(0);
            CoDrone.setThrottle(30);
            CoDrone.move();
        }

        // In the next 3 seconds pitch forward
        else if (millis() - StartTime < 3000) {
            CoDrone.setRoll(-60);
            CoDrone.setPitch(0);
            CoDrone.setThrottle(0);
            CoDrone.move();
        }

        else if (millis() - StartTime < 6000) {
            CoDrone.setRoll(0);
            CoDrone.setPitch(0);
            CoDrone.setThrottle(0);
            CoDrone.move();
        }
        
        // From 1.5 seconds to 3.5 seconds, pitch forward at 40% power
        else if (millis() - StartTime < 9000) {
            CoDrone.setRoll(0);
            CoDrone.setPitch(60);
            CoDrone.setThrottle(0);
            CoDrone.move();
        }

        // In the next 3 seconds pitch forward
        else if (millis() - StartTime < 13000) {
            CoDrone.setRoll(0);
            CoDrone.setPitch(0);
            CoDrone.setThrottle(0);
            CoDrone.move();
        }


        /*
        // Land if you have finished the sequence
        
        else if (FLAG == 1) { 
            CoDrone.land();
            FLAG = 0;
        }
        */
    }

}
