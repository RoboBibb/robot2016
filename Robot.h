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
	DoubleSolenoid gearShifter; // piston used to switch gears
	Joystick driveCtl, shootCtl; // xbox360 controllers
	Compressor airPump; // compressor
	Talon inAndOut; // shooter motor
	BuiltInAccelerometer accel; // accelerometer in the RoboRIO


	// these are for the [unused] autonomous code chooser (smart-dashboard integration)
	LiveWindow* lw = LiveWindow::GetInstance();
	SendableChooser* chooser = new SendableChooser();
	const std::string autoStopAtObstacle = "stop at first vertical obstacle";
	const std::string autoLowBar = "go under low bar";
	const std::string autoSeeSaws  = "go over see-saws";
	std::string autoSelected;


	// functions inherited from the IterativeRobot base-class:
	void RobotInit(); //run once on startup

	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	// robot whines if I don't overload these.
	// apparently it's okay if these are inline...
	void TestInit()
		{ std::cout <<"Testing mode enabled...\nCurrently doing: (NULL)" <<std::endl; }
	void TestPeriodic()
		{ lw->Run(); }

	void DisabledInit()
		{ std::cout <<"\n\nI will be ready for my next summoning.\n\t-Sir Sophagus" <<std::endl; }

	
};

/// a few useful functions
namespace utils {

	/// remove 'ghost-input' resulting fqrom inaccurate joysticks
	inline float removeGhost(const float& val){
		return (val > 0.15f || val < -0.15f) ? val : 0.0f;
	}


	/// a linear approach to preventing brownout (this has errors)
	float linReduceBrownout(const float& limit, const float& current, float& past){
		/// limit = maximum ammount of change per cycle
		/// current = the most recent value coming from input
		/// past = the value returned by this function in the last frame

		// null or ghost input doesn't affect robot (also good for breaking)
		if (utils::removeGhost(current) == 0.0f) return 0.0f;

		float change = current - past;

		if (change > 0) { // increase speed
			if (change > limit) // too much change
				return (past += limit);
			// nominal change
			return (past = current);

		} else { // decrease speed
			if (change < -limit) // too much change
				return (past -= limit);
			// nominal change
			return (past = current);
		}
	}

	// an exponential approach to preventing brownout
	float expReduceBrownout(const float& current, float& past){
		return ((past = ((past + utils::removeGhost(current)) / 2)) >= 0) ?
			sqrt(past) : -sqrt(-past);
	}

}

#endif
