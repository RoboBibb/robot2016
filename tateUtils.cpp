/*
 * tateUtils.cpp
 *
 *  Created on: 21/02/2016
 *      Author: tate
 */

#include "tateUtils.h"


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
void utils::driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, float seconds){
	for (; seconds > 0; seconds-=0.050) {



	}
}
void utils::driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, const bool& condition);
void utils::driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, bool (*condition)(void));

//built-in accelerometer (call the others)
void utils::driveStraignt(RobotDrive& drive, const Axis& axis, const float& seconds){
	BuiltInAccelerometer builtInAccel(Accelerometer::kRange_4G);
	utils::driveStraight(drive, builtInAccel, axis, seconds);
}
void utils::driveStraight(RobotDrive& drive, const Axis& axis, const bool& condition){
	BuiltInAccelerometer builtInAccel(Accelerometer::kRange_4G);
	utils::driveStraight(drive, builtInAccel, axis, condition);
}
void utils::driveStraignt(RobotDrive& drive, const Axis& axis, bool (*condition)(void)){
	BuiltInAccelerometer builtInAccel(Accelerometer::kRange_4G);
	utils::driveStraight(drive, builtInAccel, axis, condition);
}
