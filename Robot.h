#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h> // constant width integers (ie- int8_t)
#include "WPILib.h" // robot stuff


class Robot: public IterativeRobot {

public:
    Robot(); //constructor must be public

private:
    //robot parts:
    RobotDrive myRobot; // drive train
	DoubleSolenoid gearShifter, shooterPiston; // piston used to switch gears
	Joystick driveCtl, shootCtl; // x-box controllers
	Compressor airPump; // compressor
	Talon /*shooterElevator1, shooterElevator2,*/ inAndOut;
	//Ultrasonic sonar; //rnage-finder
	//DigitalInput shooterUpLim, shooterDownLim, shooterInLim; // limit-switches
	BuiltInAccelerometer accel; // accelerometer in the RoboRIO

	//these are for the autonomous code chooser (smart-dashboard integration)
	LiveWindow* lw = LiveWindow::GetInstance();
	SendableChooser *chooser = new SendableChooser();
	const std::string autoStopAtObstacle = "stop at first vertical obstacle";
	const std::string autoLowBar = "go under low bar (might not work)";
	const std::string autoSeeSaws  = "go over see-saws (Don\'t use)";
	std::string autoSelected;


	// functions inherited from the IterativeRobot base-class:
	void RobotInit(); //run once on startup

	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestInit()
		{ std::cout <<"Testing mode enabled...\nCurrently doing: (NULL)" <<std::endl; }
	void TestPeriodic()
		{ lw->Run(); }

	void DisabledInit()
		{ std::cout <<"\n\nGoodbye cruel world\n\t-Sir Kobe" <<std::endl; }
};

namespace utils {

	/// remove 'ghost-input' resulting fqrom inaccurate joysticks
	inline float removeGhost(const float& val){
		return (val > 0.15f || val < -0.15f) ? val : 0.0f;
	}


	/// a linear approach to preventing brownout (this has errors)
	float linReduceBrownout(const float& limit, const float& current, float& past){
		// limit = maximum ammount of chage per frame
		// current = the most recent value coming from input
		// past = the value returned by this function in the last frame

		// null or ghost input doesn't affect robot
		if (removeGhost(current) == 0.0f) return 0.0f;

		float change = current - past;

		if (change > 0) {// forward
			if (change > limit) { // too much change
				past += limit;
				return past;
			}
			// nominal change
			past = current;
			return current;

		} else { //reverse
			if (change < -limit) { // too much change
				past -= limit;
				return past;
			}
			// nominal change
			past = current;
			return current;
		}
	}

	// an exponential approach to preventing brownout
	inline float expReduceBrownout(const float& current, float& past){
		return (((past = ((past + utils::removeGhost(current)) / 2)) > 0) ?
			sqrt(past / 2) : -sqrt(-(past / 2))
		);
	}

}

#endif
