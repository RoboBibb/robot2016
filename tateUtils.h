#ifndef TATEUTILS_H
#define TATEUTILS_H

#include "WPILib.h"

/*
#ifndef ACCELEROMETER_TURNING_CONSTANT
	#define ACCELEROMETER_TURNING_CONSTANT 2
#endif
*/

namespace utils {

	/*
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
		const double& multiplier = 1 //make the motor run at partial power
	);

	// DriveStraight function: great for driving over uneven terrain in autonomous.

	enum Axis {x, y, z}; //makes the code easier to read

	//uses an accelerometer to drive straight.
	void driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, float seconds, const float& moveValue = 1);
	void driveStraight(RobotDrive& drive, Accelerometer& accel, const Axis& axis, bool (*condition)(void), const float& moveValue = 1);

	//built-in accelerometer (call the others)
	void driveStraight(RobotDrive& drive, const Axis& axis, float seconds, const float& moveValue = 1);
	void driveStraight(RobotDrive& drive, const Axis& axis, bool (*condition)(void), const float& moveValue = 1);

	*/

	inline float removeGhost(const float& val){
		return (val > 0.15f || val < -0.15f) ? val : 0.0f;
	}

}

#endif
