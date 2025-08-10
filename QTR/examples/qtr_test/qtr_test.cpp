/*
Simple demo for reading a QTR sensor
*/

//Bring in lib
#include "QTR.h"

//Set sensor on pin
QTR test(5);

void setup(){
  Serial.begin(9600);//Start serial for report
}

void loop(){
  Serial.println(test.read_QTR()); //Read the sensor
  delay(200); //5Hz update rate
}
