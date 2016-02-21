/*
 * tateUtils.h
 *
 *  Created on: 21/02/2016
 *      Author: tate
 *
 *  Just a few commonly used functions and utilities in a convienient utils namespace.
 *  These could help simplify your code if you know what they mean...
 *
 */

#ifndef TATEUTILS_H_
#define TATEUTILS_H_

#include "WPILib.h"

namespace utils {

	template <typename MOTCTLR>
	void setMotorDirection(
		MOTCTLR& motor, //a reference to the motor controller
		Joystick& joystick, //the joystick to use
		const uint8_t& fwd, //forward button number
		const uint8_t& bkwd //backward button number
	);

	template <class MOTCTLR>
	void controlMotor(
		MOTCTLR& motor, //a reference to the motor controller
		Joystick& joystick, //the joystick to use
		const uint8_t& fwd, //forward button number
		const uint8_t& bkwd, //backward button number
		const bool& condition = true, //the safety condition
		const double& multiplier = 1
	);


	typedef enum Axis {x, y, z}; //makes the code easier to read

	//uses an accelerometer to drive straight.
	void driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, float seconds);
	void driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, const bool& condition);
	void driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, bool (*condition)(void));

	//built-in accelerometer (call the others)
	void driveStraignt(RobotDrive& drive, const Axis& axis, float seconds);
	void driveStraight(RobotDrive& drive, const Axis& axis, const bool& condition);
	void driveStraignt(RobotDrive& drive, const Axis& axis, bool (*condition)(void));
}

#endif
