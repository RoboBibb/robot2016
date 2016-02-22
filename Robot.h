/**
 *Rules for editing:
 * - put a comment with your name stating what you have changed so that I can make the changes to the actual code.
 * - I recommend you install eclipse to take full advantage of it's syntax highlighting, error detection, and integrated documentation. if you need a text editor.
 * - Include lots of comments
 * - a great person once said "good code explains itself" : use self-explanatory names, syntax, etc. and write code that is easy to read.
 * - KISS = Keep It Simple Stupid
 * - Don't change something that will work unless performance would be improved. (even so, remember KISS)
 */

#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h> //The C library is good...

#include "WPILib.h" // robot stuff

#include "tateUtils.h" // custom functions and stuff

class Robot: public IterativeRobot{

public:
    Robot(); //constructor must be public

private:

    //robot parts (initialized by constructor):
    RobotDrive myRobot; // drive train
	DoubleSolenoid gearShifter, shooterPiston; //piston used to switch gears
	Joystick driveCtl, shootCtl; // only 1 joystick (for now)
	Compressor airPump;
	Talon shooterElevator, inAndOut1, inAndOut2;
	DigitalInput shooterUpLim, shooterDownLim, shooterInLim;
	Ultrasonic sonar;
	AnalogPotentiometer shooterArmPot; //pot used to determine the rough angle of the shooter.
	BuiltInAccelerometer accel; //the accelerometer in the RoboRIO

	//member variables:
	bool m_kobe = true, m_isHighGear = false;

	//these are for the autonomous code chooser (smart-dashboard integration)
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser = new SendableChooser();
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

	void DisabledInit(){
		std::cout <<"\n\nGoodbye cruel world\n\t-Sir Kobe\n";
	}
};


#endif
