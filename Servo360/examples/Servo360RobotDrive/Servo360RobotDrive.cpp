/*
 * Using the robot version of Servo360 to control
 * two robot drive motors, which keep track of left and 
 * right so that the forward direction corresponds to a 
 * positive speed, and vise versa.
 */
#include "Servo.h"
#include "Servo360.h"

/*
 * Create an object to handle the motors
 *  The argument 'ROBOT' is provided to let the
 *  library know that you will specify right and
 *  left motors specifically- that way + and - 
 *  correspond to forward and backward, respectively
 */
Servo360 motors(9,10,ROBOT);

void setup() {
  //Initialize the connection to the motors
  motors.connect();
}

void loop() {
  /*
   * Set the speeds of the motors, loop through a couple movements
   * Both 50% forward for a second 
   */
  motors.move(50,50);
  delay(1000);
  
  motors.move(50,-50);
  delay(1000);
  
  motors.move(-50,50);
  delay(1000);
  
  motors.move(-50,-50);
  delay(1000);

}
