#include <Arduino.h>
#include <Servo.h>

#define ROBOT "robot"

class Servo360{
	public:
		//	Two constructors: one for the up to 4 normal servos, one for robot mode
		//	with designated right and left motors
		Servo360(int servoPin1,int servoPin2 = -1,int servoPin3 = -1,int servoPin4 = -1);
		Servo360(int left, int right, String robot);
		//	The basic functions- connect sets up the motors, move changes speeds, and
		//	center changes the zero-speed points
		void connect();
		void move(int pos,int i);
		void move(int left, int right, String robot);
		void center(int center, int i);
		//	The servo objects themselves which the deeper servo library needs to
		//	control movement
		Servo _serv1;
		Servo _serv2;
		Servo _serv3;
		Servo _serv4;
		Servo _servLeft;
		Servo _servRight;
	private:
		//	In the private variables, we have all the pin locations, the zero
		//	speed values (default 90), and the mode variable, controlling whether
		//	the object is in robot mode.
		int _serv1Pin;
		int _center1;
		int _serv2Pin;
		int _center2;
		int _serv3Pin;
		int _center3;
		int _serv4Pin;
		int _center4;
		int _leftPin;
		int _rightPin;
		int _centerRight;
		int _centerLeft;
		int mode;
};

