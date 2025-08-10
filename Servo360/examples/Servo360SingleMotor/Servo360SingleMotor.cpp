/*
 * Controlling a single servo motor with the Servo360
 * Library
 */
#include "Servo.h"
#include "Servo360.h"

/*
 *Create a servo360 object to make controlling 
 * the motor more simple
 */
Servo360 motor(9);

void setup() {
  motor.connect(); //Initialize the connection to the servos
}


void loop() {

  motor.move(1,20); 
  
}

