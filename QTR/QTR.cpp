/*
A small library to read from the QTR compact IR distance sensor

It looks like it measures the bounce time of IR in uS, but that's not feasible!
Instead, the IR beam charges a capacitor based on the reflection intensity, which is
inverse square proportional to distance, then we measure the capacitor discharge time.
*/

#include "Arduino.h"
#include "QTR.h"

//Base object needs only a pin
QTR::QTR(int pin){
  _pin = pin;}

//Read the sensor
int QTR::read(){

  pinMode(_pin,OUTPUT); //Make the read pin and output
  digitalWrite(_pin,HIGH); //Set on
  delay(1); //Give a ms to charge the capacitor

  pinMode(_pin,INPUT); //make input
  sV = micros(); //set microsecond timer

  while((digitalRead(_pin))&&(millis()-time<10)){} //Wait for discharge
  sV = micros() - sV; //Calculate uS to discharge
  delay(1); //debounce delay

  if (sV >= 0){return (int)(sV/100);} //Return scaled time to discharge as reading
  else{return -1;} // Return -1 if overflow (too far, no distance read)
}
