/*
This library simplifies servo management by combining the storage
of the servo objects under one main object, converting the 0-180
degree control scheme into a -100 to 100 form, to make continuous
rotation servos more intuitive, and providing a robot-specific 
version which keeps the right and left motors in mind specifically
so you don't have to try and remember which needs positive values
to move forward, and which needs negative ones.

*/

#include "Arduino.h"

#include "Servo.h"
#include "Servo360.h"


/*
Each constructor creates slots for all servos to remove ambiguity
when calling the singular unified 'move' command without a bunch of
'#ifdef's. 
*/

Servo360::Servo360(int servoPin1,int servoPin2,int servoPin3,int servoPin4){
	//	Create records for each servo (of which there are up to 4)
	//  The pin and center offset for each is created below, and
	//  we create all 4 now, regardless of how many are actually \
	// 	required, to simplify the setup.
	_serv1Pin = servoPin1;
	_serv2Pin = servoPin2;
	_serv3Pin = servoPin3;
	_serv4Pin = servoPin4;
	_center1 = 90;
	_center2 = 90;
	_center3 = 90;
	_center4 = 90;
	//	Hidden variable registering whether in robot or normal mode
	// 	which is specified in the constructor. This allows for there
	//	to be only one method per function, not	a separate method 
	//	for each mode.
	mode = 0; 
}

Servo360::Servo360(int left, int right, String robot){
	//	Robot mode initializer- keeps 'left' and 'right' servos
	//  oriented to make driving easier by automatically making
	// 	positive speeds move the wheels in the forward direction
	_leftPin = left;
	_rightPin = right;
	_centerLeft = 90;
	_centerRight = 90;
	mode = 1; //Robot mode, as per the above comment on the mode variable
}

void Servo360::connect(){	
	//	Function to do all the attaching at once, split by
	//	mode as each version has a slightly different naming
	//	convention.
	if (mode == 0){
		//	Normal mode. Always have at least one servo, but have to
		//	check other servos to see if the user supplied pins
		//	for them. We don't attach if they aren't present
		_serv1.attach(_serv1Pin);
		if (_serv2Pin != -1){
			_serv2.attach(_serv2Pin);}
		if (_serv3Pin != -1){
			_serv3.attach(_serv3Pin);}
		if (_serv4Pin != -1){
			_serv4.attach(_serv4Pin);}
	}

	if (mode == 1){
		//	Robot mode- no checks, always have left and right
		_servLeft.attach(_leftPin);
		_servRight.attach(_rightPin);
	}
}

void Servo360::move(int i, int pos){
	//	Set a servo to a speed
	// 	In the functions below, we us a map from a range of -100 to 100 input onto a range of the center +/- 30
	// 	rather than the full +/- 90 range for two reasons: first, +/-90 would work strangely with changing center points,
	// 	and would require adjusting to account for the aysmmetric upper and lower ranges, but more importantly,
	// 	because the apparent speed under testing observations did not appear to significantly vary for deviations
	// 	from the center point of more than approximately 30 steps- to make the range spread more visible,
	// 	we restricted the range to +/- 30
	if (mode == 0){ //Normal mode
		if (i == 1){
			int posto = map(pos,-100,100,_center1+30,_center1-30);
			_serv1.write(posto);}
		if (i == 2){
			int posto = map(pos,-100,100,_center2+30,_center2-30);
			_serv2.write(posto);}
		if (i == 3){
			int posto = map(pos,-100,100,_center3+30,_center3-30);
			_serv3.write(posto);}
		if (i == 4){
			int posto = map(pos,-100,100,_center4+30,_center4-30);
			_serv4.write(posto);}
	}
	else{ //	Robot mode
		//	Two maps which convert to 'proper' speeds and also set the directions
		//	notice that the converted value is reversed for the left motor (-1*i).
		//	This is because the left and right motors are flipped. Conventionally,
		//	we make the right motor 'positive' forward, so the left has to be
		// the negative of the supplied value
		int postoLeft = map(-1*i,-100,100,_centerLeft+30,_centerLeft-30);
		int postoRight = map(pos,-100,100,_centerRight+30,_centerRight-30);
		_servLeft.write(postoLeft);
		_servRight.write(postoRight);
	}
}

void Servo360::center(int i, int center){
	//	Set the 'center' positions to an offset value for trimming,
	//	the position '90' is ostensibly the middle point, or zero speed,
	//	for the motors, but the actual zero speed point can vary. This
	//	function lets you adjust that to make control smooth- all 
	//	speeds are relative to the 'center' zero-speed point.
	if (mode == 0){ //Normal mode uses i as an index to pick which to change
		if (i == 1){
			_center1 = 90 + center;}
		if (i == 2){
			_center2 = 90 + center;}
		if (i == 3){
			_center3 = 90 + center;}
		if (i == 4){
			_center4 = 90 + center;}
	}
	if (mode == 1){//Robot mode uses i as the left offset and center as the right, since there's always two of them
		_centerLeft = i;
		_centerRight = center;
	}
}

