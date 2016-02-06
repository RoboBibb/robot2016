/**
 * What the code does:
 * - Multiple autonomous codes. It should work.
 * - uses a robotDrive object to move the robot using tank drive based off input from the joystick
 * - reports video feed from a USB camera to the smartdashboard
 * - fills the air tank using the compressor's member function "SetClosedLoopControl()"
 * - setMotorDirection() member function to control a motor using 2 buttons.
 * - prints kobe to terminal when trigger pulled
 * - switches gears using buttons 11 & 12
 *
 *Rules for editing:
 * - put a comment with your name stating what you have changed so that I can make the changes to the actual code.
 * - I recommend you install eclipse to take full advantage of it's syntax highlighting, error detection, and integrated documentation. if you need a text editor.
 * - Include lots of comments
 * - a great person once said "good code explains itself" : use self-explanatory names, syntax, etc. and write code that is easy to read.
 * - KISS = Keep It Simple Stupid
 * - Don't change something that will work unless performance would be improved. (even so, remember KISS)
 */
//I could probably use pragma once... 
#ifndef ROBOT_H
#define ROBOT_H

#include "WPILib.h"

class Robot: public IterativeRobot{

public:
    Robot(); //constructor must be public

private:

    //robot parts (initialized by constructor):
    RobotDrive myRobot; // drive train
	DoubleSolenoid gearShifter, shooterPiston; //piston used to switch gears
	Joystick stick; // only 1 joystick (for now)
	Compressor airPump;
	Talon shooterElevator, inAndOut;
	DigitalInput shooterUpLim, shooterDownLim, shooterInLim;

	//member variables:
	bool m_kobe = 1, m_isHighGear = 0;

	//these are for the autonomous code chooser (smart-dashboard integration)
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser = new SendableChooser();
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom1 = "Auto-Code1";
	std::string autoSelected;

	//custom function(s)
	template <typename MOT>
	void setMotorDirection(MOT &motor, Joystick &joystick, const unsigned int& fwd, const unsigned int& bkwd);

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
