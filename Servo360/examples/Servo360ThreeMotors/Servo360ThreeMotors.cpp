/*
 * Illustrates how to use multiple servos with one
 * Servo360 object
 */
#include "Servo.h"
#include "Servo360.h"

/*
 *Create a Servo360 object to make controlling 
 * the motors more simple
 */
Servo360 motors(9,10,11);

void setup() {
  motors.connect();
}

void loop() {
  motors.move(1,50); //Set first motor to 50% of max forward speed
  motors.move(2,10); //Set second motor to 10% of max forward speed
  motors.move(3,-40); //Set third motor to 40% of max reverse speed
}
