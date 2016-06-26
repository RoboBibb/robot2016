#include "Robot.h"


// constructor
Robot::Robot() : // member initializations (constructor)
	myRobot(1, 2), //left0, right1
	gearShifter(2, 3), //solenoids
	driveCtl(0), shootCtl(1), //xbox360 controllers
	airPump(0), //compressor
	inAndOut(4), //shooter motors
	accel(Accelerometer::kRange_4G) // the accelerometer in the roboRIO (not used...)
{
	myRobot.SetExpiration(0.1);
}

// on startup
void Robot::RobotInit(){
	//setup the auto-chooser:
	chooser->AddDefault(autoStopAtObstacle, (void*) &autoStopAtObstacle);
	chooser->AddObject(autoLowBar, (void*) &autoLowBar);
	chooser->AddObject(autoSeeSaws, (void*) &autoSeeSaws);

	SmartDashboard::PutData("Auto Modes", chooser);

	//get camera feed and post it to the smartdashboard
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");// camera name in the web interface

	// Dream big XD
	std::cout <<"Hello world!\n\n I\'ve got a plan for world domination.\nXDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n" <<std::endl;
}


/**Notes on autonomous chooing code:
 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
 * below the Gyro
 *
 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
 * If using the SendableChooser make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit(){

	// start filling the tank
	airPump.SetClosedLoopControl(true);

	// auto-choosing code
	autoSelected = *((std::string*)chooser->GetSelected()); // for C++/Java smartdashboard
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoStopAtObstacle); //use this for labview smartdashboard

	// print the chosen autonomous code to the console
	std::cout <<"Autonomous has begun!\nAuto selected: "
				<<autoSelected <<std::endl;

	// disable safety on drive-train
	myRobot.SetSafetyEnabled(false);
	
	// jerk back-and-forth to knock shooter down
	myRobot.Drive(0.75f, 0);
	Wait(0.125);
	myRobot.Drive(-0.75f, 0);
	Wait(0.125);

	// drive up to wall
	myRobot.Drive(1, 0); // full forward 1.5seconds
	Wait(2);

	// stop
	myRobot.Drive(0.0f, 0);

	/* extended, untested, and unused :P

	// turn
	myRobot.Drive(0.0f, 0.5f);
	Wait(0.25);

	// drive up to goal
	myRobot.Drive(0.75f, 0);
	Wait(0.5);

	// stop
	myRobot.Drive(0.0f, 0);

	// shoot
	inAndOut.SetSpeed(1);
	*/

/* requires ultrasonic sensor:
	sonar.SetAutomaticMode(true); // turns on automatic mode
	if (autoSelected == autoLowBar) {
		// drive until the low bar flap thing
		while (sonar.GetRangeInches() < STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.5f, 0);

		// drive through the low-bar
		myRobot.ArcadeDrive(0, -0.25f);
		Wait(3);

		// drive until the wall on the other side
		while (sonar.GetRangeInches() < STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.5f, 0);

		// turn approximatly 90 degrees right. (towards goal)
		myRobot.ArcadeDrive(0, -0.2f);
		Wait(0.5);	//adjust wait period

		// start driving forward again
		myRobot.ArcadeDrive(0.5f, 0.5f);

		// start spinning the shooter
		inAndOut1.SetSpeed(1);
		inAndOut2.SetSpeed(1);
		//Wait(0.75);

		// drive up to the goal
		while (sonar.GetRangeInches() < STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.5f, 0);

		// shoot the ball [into the goal].
		shooterPiston.Set(DoubleSolenoid::Value::kReverse);

		// stop spinning the shooter
		inAndOut1.SetSpeed(0);
		inAndOut2.SetSpeed(0);

		// stop moving (wait until teleop starts)
		myRobot.ArcadeDrive(0.0f, 0.0f);
		
	} else if (autoSelected == autoSeeSaws) {
		// autonomous code to go over the see-saws
		///TODO:
		/// - drive up to the see-saws
		/// - use the shooter to lower the see-saws (might not be possible)
		/// - drive over the see-saws
		/// - proceed to emulate the low-bar autonomous code.
	} else {
		// Default Auto goes here
	}
	*/
}

void Robot::AutonomousPeriodic(){
	/* requires ultrasonic sensor :(
	if (autoSelected == autoLowBar) {}
	else if (autoSelected == autoSeeSaws) {}
	else { // default autonomous code
		// drive forward and stop 3 feet in front of the vertical obstacle.
		// the robot and field are built in inches, so it's probably best not to use metric =(
		if (sonar.GetRangeInches() > STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.3f, 0);
		else myRobot.Drive(-0.5f, 0);
	} */
	
	
}


void Robot::TeleopInit(){
	//start filling the tank
	airPump.SetClosedLoopControl(true);

	//tele-op has started.
	std::cout <<"\nTele-op has started - Awaiting your command\n";

	//start out in low gear
	gearShifter.Set(DoubleSolenoid::Value::kForward);

	myRobot.SetSafetyEnabled(false);
}

void Robot::TeleopPeriodic(){

	// joystick data from previous cycle
	static struct coord { // `static` keeps this local variable in memory
		float x = 0, y = 0;
	} stick;

	//drive the robot
	myRobot.ArcadeDrive(
		-utils::expReduceBrownout(driveCtl.GetRawAxis(1), stick.y) * 0.9f,
		-utils::expReduceBrownout(driveCtl.GetRawAxis(0), stick.x) * 0.8f
	);


	static bool isHighGear = false; // default to low-gear

	// shift gears a=>low b=>high
	if (isHighGear && driveCtl.GetRawButton(1)) { // a
		gearShifter.Set(DoubleSolenoid::Value::kForward);
		std::cout <<"Switched to LOW Gear <<default" <<std::endl;
		isHighGear = !isHighGear;
	} else if (!isHighGear && driveCtl.GetRawButton(2)) { // b
		gearShifter.Set(DoubleSolenoid::Value::kReverse);
		std::cout <<"Switched to HIGH Gear >>" <<std::endl;
		isHighGear = !isHighGear;
	}

	/* now using a static shooter (no adjustments to angle)
	// adjust shooter's vertical elevation using the D-pad
	if (shootCtl.GetPOV() > 90 && shootCtl.GetPOV() < 270 && shooterDownLim.Get()) { // D-pad down
		shooterElevator1.SetSpeed(-0.5f);
		shooterElevator2.SetSpeed(0.5f);
	} else if (shootCtl.GetPOV() < 90 && shootCtl.GetPOV() > 270 && shooterUpLim.Get()) {// D-pad up
		shooterElevator1.SetSpeed(0.5f);
		shooterElevator2.SetSpeed(-0.5f);
	} else {
		shooterElevator1.SetSpeed(0);
		shooterElevator2.SetSpeed(0);
	} */


	// intake and pre-fire controls
	if (shootCtl.GetRawAxis(3) > shootCtl.GetRawAxis(2)) // shoot
		inAndOut.SetSpeed(-1);
	else if (shootCtl.GetRawAxis(3) < shootCtl.GetRawAxis(2)) // suck-in
		inAndOut.SetSpeed(0.9f);
	else inAndOut.SetSpeed(0); // freeze

	/* we removed the shooter piston
	if (shootCtl.GetRawAxis(3) > 0.95f) // fire the shooter
		shooterPiston.Set(DoubleSolenoid::Value::kReverse);
	else shooterPiston.Set(DoubleSolenoid::Value::kForward); // retract the shooter
	*/





	/// give some driver feedback:

	// we still say kobe, even for the low goal
	static bool kobe = true;

	// print "Kobe!!" to the console when we shoot (for good luck)
	if (kobe && (shootCtl.GetRawAxis(3) > 0.95f)) {
		std::cout <<"Kobe!!" <<std::endl;
		kobe = false;
	} else if (shootCtl.GetRawAxis(3) <= 0.95f) {
		kobe = true;
	}

	// rumble both controllers when firing and when switching gears (aids communication between drivers)
	if (shootCtl.GetRawAxis(3) > 0.2f) { // rumble for fire (kobe)
		driveCtl.SetRumble(driveCtl.kLeftRumble, 1.0f);
		driveCtl.SetRumble(driveCtl.kRightRumble, 1.0f);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 1.0f);
		shootCtl.SetRumble(shootCtl.kRightRumble, 1.0f);
	} else if (driveCtl.GetRawButton(1) || driveCtl.GetRawButton(2)) { // rumble for gear-shift
		driveCtl.SetRumble(driveCtl.kLeftRumble, 1.0f);
		driveCtl.SetRumble(driveCtl.kRightRumble, 1.0f);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 1.0f);
		shootCtl.SetRumble(shootCtl.kRightRumble, 1.0f);
	} else { // no rumble
		driveCtl.SetRumble(driveCtl.kLeftRumble, 0.0f);
		driveCtl.SetRumble(driveCtl.kRightRumble, 0.0f);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 0.0f);
		shootCtl.SetRumble(shootCtl.kRightRumble, 0.0f);
	}

}

// make it happen (macro defined in WPILib.h)
START_ROBOT_CLASS(Robot)
