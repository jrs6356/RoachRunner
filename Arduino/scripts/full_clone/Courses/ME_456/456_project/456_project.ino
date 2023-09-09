/*------------------------------------------------------------------
Go
Objective   : How to Use goToHeight function
syntax      : goToHeight("height")
description : goToHeight example is using goToHeight() for fly certain height
              You can set 100 ~ 2000 boundary 
              Distance is checked from IR distance sensor next to Optical flow sensor(bottom )
-------------------------------------------------------------------*/
/*
// go for hoop(e)
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.calibrate();
  CoDrone.goToHeight(750);			    // Fly 1000mm away from bottom and hover
  CoDrone.go(LEFT,1);           	 	// Go forward at 50% power for 1 second
  CoDrone.goToHeight(750);	     		// Fly 400mm away from bottom and hover
  CoDrone.go(FORWARD,2);			       	// Go forward at 50% power for 1 second
  CoDrone.goToHeight(750);          // Fly 400mm away from bottom and hover
  CoDrone.go(FORWARD,2);              // Go forward at 50% power for 1 second
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}
*/

 //go for goal

 #include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.calibrate();
  CoDrone.go(FORWARD,4);              // Go forward at 50% power for 1 second
  CoDrone.hover(3);
  CoDrone.go(LEFT,2);               // Go forward at 50% power for 1 second
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}
// */
