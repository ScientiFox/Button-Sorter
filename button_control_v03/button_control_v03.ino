/*
Button sorter contoller Firmware

Reads QTR sensors, controls directional sweeping armature,
and manages inputs from plate position and edge and center
tracking sensors

*/

//Import servo and QTR controller libraries
#include <Servo.h>
#include "QTR.h"

//Create servo objects
Servo myservo; //Button collector armature
Servo sweep; //Sorting armature

//Initia; positions for servos on init
int sweep_pos = 95;
int pos = 83;

//Control message character
char s;

//Activate QTR sensor
QTR button_IR(2);

void setup() { 
  //Start serial
  Serial.begin(9600);

  //Start collector servo and sent to init position
  myservo.attach(9);
  myservo.write(pos); 

  //Start sweep servo and sent to init position
  sweep.attach(11);
  sweep.write(sweep_pos);
  
  //end effector position, and contact sensor controls
  pinMode(7,OUTPUT);
  pinMode(10,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  digitalWrite(10,HIGH); //Sensor bias voltage
  digitalWrite(7,HIGH);

  //Controls for H-bridge motor controller for rail and position sensors
  pinMode(6,OUTPUT); //rail control
  pinMode(5,OUTPUT); //rail control
  pinMode(4,INPUT); //edge button
  pinMode(3,INPUT); //edge button
  pinMode(8,INPUT); //center hall sensor

} 

// 
int Ar1,Ar2,D1,tcount;

void loop() {

  //Tick counter and 0.1s smoothness delay
  tcount+=1;
  delay(100);
  
  //If serial commands on the port
  if (Serial.available()){
    s = Serial.read(); //Read the commans

    if (s == 'a'){ //Send to left side of rail
      while (1-digitalRead(4)){ //While side button not pressed
        digitalWrite(6,HIGH); //Set H-bridge to slide left
        digitalWrite(5,LOW);}
      digitalWrite(6,LOW); //Turn off motor
      digitalWrite(5,LOW);
      }
    
    if (s == 'b'){ //Send to right side of rail
      while (1-digitalRead(3)){ //While side button not pressed
        digitalWrite(6,LOW); //Set H-bridge to slide right
        digitalWrite(5,HIGH);}
      digitalWrite(6,LOW); //Turn off motor
      digitalWrite(5,LOW);}

    if (s == 'c'){ //Send to center position
      while (digitalRead(8)){ //While middle hall-effect sensor high
        digitalWrite(6,HIGH); //Slide left
        digitalWrite(5,LOW);
        if (digitalRead(4)){ //If you hit the left side, go back right
          digitalWrite(5,HIGH);
          digitalWrite(6,LOW);
          while (digitalRead(8)){} //carry on until seeing sensor
        }
      }
      while (1-digitalRead(8)){ //Move left until at edge of hall effect field
        digitalWrite(6,LOW);
        digitalWrite(5,HIGH);}
      digitalWrite(6,LOW); //Turn off motor
      digitalWrite(5,LOW);}
      
    if (s == 'e'){ //Move to the ID plate position
      while (digitalRead(12)){ //While not at the ID plate sensor, slide left
        digitalWrite(5,HIGH);
        digitalWrite(6,LOW);}
      while (1-digitalRead(12)){ //Move to edge of sensor field
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);}
      digitalWrite(6,LOW); //Turn off motor
      digitalWrite(5,LOW);}
    
    if (s == 'd'){ //Lower end effector
      end_effector_down();
      delay(500);} //Vibration delay
    
    if (s == 'u'){ //Raise end effector
      end_effector_up();
      delay(500);} //Vibration delay

    if (s == 'l'){ //Set end effector to mid position
      end_effector_center();
      delay(500);} //Vibration delay
      
    if (s == 'p'){ //Print status
      Serial.print("a");
      Serial.print(digitalRead(4)); //Left side sensor value
      Serial.print(",");
      Serial.print("b");
      Serial.print(digitalRead(3)); //Right side sensor valure
      Serial.print(",");
      Serial.print("c");
      Serial.print(digitalRead(8)); //Center sensor
      Serial.print(",");
      Serial.print("u");
      Serial.print(digitalRead(10)); //End effector top sensor
      Serial.print(",");
      Serial.print("d");
      Serial.print(digitalRead(13)); //End effector bottom sensor
      Serial.print(",");
      Serial.print("e");
      Serial.print(digitalRead(12)); //Id plate position sensor
      Serial.print(",");
      Serial.print("h");
      Serial.print(pos); //Sweep armature position
      Serial.print(",");
      Serial.print("r");
      Serial.print(button_IR.read_QTR()); //QTR sensor reading
      Serial.print(",");
      Serial.print("t");
      Serial.print(tcount); //Tick counter
      Serial.println();}
  
    if (s == 't'){ //CCW turn for bottom-down button
      sweep.write(sweep_pos+10);}
  
    if (s == 'r'){ //CW for top-down button
      sweep.write(sweep_pos-12);}

    if (s == 's'){ //Return sweep arm to home location
      sweep.write(sweep_pos+10); //CCW turn
      while (analogRead(A5) > 100){} //wait till entering sensor field
      while (analogRead(A5) < 100){} //wait till edge of sensor field
      sweep.write(sweep_pos-10); //reverse
      while (analogRead(A5) > 100){} //wait until entering sensor field
      sweep.write(sweep_pos);} //Stop movement
      
    if (s == 'w'){ //Delay command, utility
      delay(100);}
  }
  
}

void end_effector_down(){
  //Function to lower the end effector
  myservo.write(pos); //Write the current position value
  while (digitalRead(13)&(pos <= 160)){ //While down button not pressed, and position within limit
    pos++; //Increment position
    myservo.write(pos); //Write new position
    delay(50); //Smoothness delay
    }
}

void end_effector_up(){
  //Function to raise the end effector
  myservo.write(pos); //Write current position
  while (digitalRead(10)&(pos >= 45)){ //While top button not pressed and position within limit
    pos--; //Decrement position
    myservo.write(pos); //Write new position
    delay(50); //Smoothness delay
  }
}

void end_effector_center(){
  //Function to set end effector to mid position
  myservo.write(pos); //Write current position
  while (digitalRead(13)&(pos < 130)){ //While middle sensor not active and under position limit
    pos++; //Increment position
    myservo.write(pos); //Write new position
    delay(50);} //Smoothness delay
  while (digitalRead(13)&(pos > 130)){ //While middle sensor not active and over position limit
    pos--; //Decrement position
    myservo.write(pos); //Write new position
    delay(50);} //Smoothness delay
}



