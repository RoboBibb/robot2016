/*
 * tateUtils.cpp
 *
 *  Created on: 21/02/2016
 *      Author: tate
 */

#include "tateUtils.h"

#ifndef ACCELEROMETER_TURNING_CONSTANT
	#define ACCELEROMETER_TURNING_CONSTANT 2
#endif


//used to control a motors direction using 2 buttons (fwd & bkwd)
template <class MOTCTLR>
void utils::setMotorDirection(
	MOTCTLR& motor, //a reference to the motor controller
	Joystick& joystick, //the joystick to use
	const uint8_t& fwd, //forward button number
	const uint8_t& bkwd //backward button number
){
	if (joystick.GetRawButton(fwd) == joystick.GetRawButton(bkwd)) //both buttons pressed or neither pressed.
		motor.SetSpeed(0);
	else if (joystick.GetRawButton(fwd)) motor.SetSpeed(1);
	else motor.SetSpeed(-1); //bkwd
}

template <class MOTCTLR>
void utils::controlMotor(
		MOTCTLR& motor, //a reference to the motor controller
		Joystick& joystick, //the joystick to use
		const uint8_t& fwd, //forward button number
		const uint8_t& bkwd, //backward button number
		const bool& condition, //the safety condition
		const double& multiplier //make the motor run at partial power
){
	//multiplier can only decrease the value and/or reverse direction
	if (multiplier > 1 || multiplier < -1) multiplier = 1;
	if (condition) { //only move if safe
		// don't move if both buttons pressed or neither pressed.
		if (joystick.GetRawButton(fwd) == joystick.GetRawButton(bkwd))
			motor.SetSpeed(0);
		else if (joystick.GetRawButton(fwd)) motor.SetSpeed(1);
		else motor.SetSpeed(-1); //bkwd

	} else motor.SetSpeed(0); //not safe: don't move.
}


//uses an accelerometer to drive straight.
void utils::driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, float seconds, const float& moveValue){
	//changing order might increase runtime speed...
	for (; seconds > 0; seconds-=0.050) {
		switch (axis) {
			case x: drive.Drive(moveValue, accel.GetX() * ACCELEROMETER_TURNING_CONSTANT);break;
			case y: drive.Drive(moveValue, accel.GetY() * ACCELEROMETER_TURNING_CONSTANT);break;
			case z: drive.Drive(moveValue, accel.GetZ() * ACCELEROMETER_TURNING_CONSTANT);break;
			default: std::cerr <<"driveStraigt(): invalid axis given!"; //this should output the problem to the console
		}
		Wait(0.050);
	}
}
void utils::driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, bool (*condition)(void), const float& moveValue){
	switch (axis) {
		case x:
				while (condition())
					drive.Drive(moveValue, accel.GetX() * ACCELEROMETER_TURNING_CONSTANT);
				break;
		case y:
				while (condition())
					drive.Drive(moveValue, accel.GetY() * ACCELEROMETER_TURNING_CONSTANT);
				break;
		case z:
				while (condition())
					drive.Drive(moveValue, accel.GetZ() * ACCELEROMETER_TURNING_CONSTANT);
				break;

		default: std::cerr <<"driveStraigt(): invalid axis given!"; //this should output the problem to the console
	}
}

//built-in accelerometer (call the others)
void utils::driveStraignt(RobotDrive& drive, const Axis& axis, float seconds, const float& moveValue){
	BuiltInAccelerometer builtInAccel(Accelerometer::kRange_4G);
	utils::driveStraight(drive, builtInAccel, axis, seconds, moveValue);
}
void utils::driveStraignt(RobotDrive& drive, const Axis& axis, bool (*condition)(void), const float& moveValue){
	BuiltInAccelerometer builtInAccel(Accelerometer::kRange_4G);
	utils::driveStraight(drive, builtInAccel, axis, condition, moveValue);
}
