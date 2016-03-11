#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h> //The C library is good...

#include "WPILib.h" // robot stuff


class Robot: public IterativeRobot {

public:
    Robot(); //constructor must be public

private:

    //robot parts (initialized by constructor):
    RobotDrive myRobot; // drive train
	DoubleSolenoid gearShifter, shooterPiston; //piston used to switch gears
	Joystick driveCtl, shootCtl; // only 1 joystick (for now)
	Compressor airPump;
	Talon shooterElevator1,shooterElevator2, inAndOut1, inAndOut2;
	Ultrasonic sonar;
	// back to limit switches
	DigitalInput shooterUpLim, shooterDownLim, shooterInLim; // shooterInLim might not be used....
	BuiltInAccelerometer accel; //the accelerometer in the RoboRIO


	//data members:
	bool m_kobe = true, m_isHighGear = false;

	//these are for the autonomous code chooser (smart-dashboard integration)
	LiveWindow *lw = LiveWindow::GetInstance();
	//SendableChooser *chooser = new SendableChooser();
	const std::string autoStopAtObstacle = "stop at first vertical obstacle";
	const std::string autoLowBar = "go under low bar (might not work)";
	const std::string autoSeeSaws  = "go over see-saws (Don\'t use)";
	std::string autoSelected;


	//inherited from IterativeRobot
	void RobotInit(); //run once on startup

	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestInit();
	void TestPeriodic();

	void DisabledInit()
		{ std::cout <<"\n\nGoodbye cruel world\n\t-Sir Kobe\n"; }
};

namespace utils {
	/// remove 'ghost-input' resulting from inaccurate joysticks
	inline float removeGhost(const float& val){
		return (val > 0.15f || val < -0.15f) ? val : 0.0f;
	}
}

#endif
