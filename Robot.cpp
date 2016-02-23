#include "Robot.h"


// some important constants...
#define STOPPING_DISTANCE_INCHES 36 // 3 feet


Robot::Robot() : //inline initializations:
	myRobot(0, 1), //left0, right1
	gearShifter(0,1), shooterPiston(2,3), //solenoids
	driveCtl(0), shootCtl(1), //they want 2 joysicks now
	airPump(0), //compressor
	shooterElevator(2), //set elevation of the shooter
	inAndOut1(8), inAndOut2(9), //shooter motors
	sonar(2, 2), //ultrasonic range-finder
	/// they want potentiometers instead of limit switches :(
	shooterArmPot(0/*Analog input port number*/, 90/*degrees of rotation*/, 0/*initial rotation*/),
	accel(Accelerometer::kRange_4G) // the accelerometer in the roboRIO
{
	myRobot.SetExpiration(0.1);
}


void Robot::RobotInit(){
	//setup the auto-chooser:
	chooser->AddDefault(autoStopAtObstacle, (void*)&autoStopAtObstacle);
	chooser->AddObject(autoLowBar, (void*)&autoLowBar);
	chooser->AddObject(autoSeeSaws, (void*)&autoSeeSaws);

	SmartDashboard::PutData("Auto Modes", chooser);

	//get camera feed and post it to the smartdashboard (*NOTE: disabled until we get a camera)
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");// camera name in the web interface

	std::cout <<"Hello world!" <<std::endl; // :P

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
	autoSelected = *((std::string*)chooser->GetSelected()); //4 C++/Java smartdashboard
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoStopAtObstacle); //use this for labview drive station

	// print the chosen autonomous code to the console
	std::cout <<"World domination ..." <<std::endl
			<<"Autonomous has begun!" <<std::endl
			<<"Auto selected: " <<autoSelected <<std::endl;


	// not quite sure what this does... but it's needed.
	sonar.SetAutomaticMode(true); // turns on automatic mode

	// disable safety on drive-train
	myRobot.SetSafetyEnabled(false);

	if (autoSelected == autoLowBar) {
		// drive until the low bar flap thing
		while (sonar.GetRangeInches() < STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.5f, ACCELEROMETER_TURNING_CONSTANT * accel.GetX());

		// drive through the low-bar
		myRobot.ArcadeDrive(0, -0.25f);
		utils::driveStraight(myRobot, utils::Axis::x, 3.0f, -0.25f); //drive straight for 3 seconds

		// drive until the wall on the other side
		while (sonar.GetRangeInches() < STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.5f, ACCELEROMETER_TURNING_CONSTANT * accel.GetX());

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
			myRobot.Drive(-0.5f, ACCELEROMETER_TURNING_CONSTANT * accel.GetX());

		// shoot the ball [into the goal].
		shooterPiston.Set(DoubleSolenoid::Value::kReverse);

		// stop spinning the shooter
		inAndOut1.SetSpeed(0);
		inAndOut2.SetSpeed(0);

		// stop moving (wait until tele-op starts)
		myRobot.ArcadeDrive(0.0f, 0.0f);

	} else if (autoSelected == autoSeeSaws) {
		// autonomous code to go over the see-saws

		///TODO:
		/// drive up to the see-saws
		/// use the shooter to lower the see-saws (might not be possible)
		/// drive over the see-saws
		/// proceed to emulate the low-bar autonomous code.

	} else { //default autonomous code
		//Default Auto goes here
	}
}

void Robot::AutonomousPeriodic(){
	if (autoSelected == autoLowBar) {
		//autonomous code to go through the low bar
	} else if (autoSelected == autoSeeSaws) {
		//autonomous code to go over the see-saws
	} else {//default autonomous code
		//drive forward and stop 3 feet in front of the vertical obstacle.
		//the robot and field are built in inches, so it's probably best not to use metric =(
		if (sonar.GetRangeInches() > STOPPING_DISTANCE_INCHES)
			myRobot.Drive(-0.3f, ACCELEROMETER_TURNING_CONSTANT * accel.GetX());
		else myRobot.Drive(-0.5f, ACCELEROMETER_TURNING_CONSTANT * accel.GetX());
	}
}

void Robot::TeleopInit(){
	//start filling the tank
	airPump.SetClosedLoopControl(true);

	//tele-op has started.
	std::cout <<"Tele-op has started - Awaiting your command\n";

	//start out in low gear
	gearShifter.Set(DoubleSolenoid::Value::kForward);

}

void Robot::TeleopPeriodic(){
	//drive the robot
	myRobot.ArcadeDrive(-driveCtl.GetRawAxis(1), -driveCtl.GetRawAxis(0));

	//shift gears a==low b==high
	if (driveCtl.GetRawButton(1) && m_isHighGear) { // a
		gearShifter.Set(DoubleSolenoid::Value::kForward);
		std::cout <<"Low Gear" <<std::endl;
		m_isHighGear = !m_isHighGear;
	} else if (driveCtl.GetRawButton(2) && !m_isHighGear) { // b
		gearShifter.Set(DoubleSolenoid::Value::kReverse);
		std::cout <<"High Gear" <<std::endl;
		m_isHighGear = !m_isHighGear;
	}

	/*adjust shooter's vertical angle
	/// the limits will likely have to be swapped (50% chance)
	// if it's safe to move the motor, run the code to do so
	utils::controlMotor(shooterElevator, shootStick, 3, 5, ((shootStick.GetRawButton(3)&&shooterUpLim.Get()) != (shootStick.GetRawButton(5)&&shooterDownLim.Get())));
	//if ((shootStick.GetRawButton(3) && shooterUpLim.Get()) != (shootStick.GetRawButton(5) && shooterDownLim.Get()))
	//	setMotorDirection(shooterElevator, shootStick, 3, 5);
	//else //stop the motor if it isn't safe
	//	shooterElevator.SetSpeed(0);*/

	/* These two blobs serve the same purpose.
	//intake and pre-fire controls (button 3 starts the shooter motors spinning)
	controlMotor(inAndOut1, shootStick, 11, 12, ((shootStick.GetRawButton(11)&&shooterInLim.Get()) || shootStick.GetRawButton(12)));
	//if ((shootStick.GetRawButton(11) && shooterInLim.Get()) || shootStick.GetRawButton(12))
	//		setMotorDirection(inAndOut1, shootStick, 11, 12); //set intake/fire
	//else inAndOut2.SetSpeed(0);
    //this is because they want the wire-colors to match on the motors
	utils::controlMotor(inAndOut2, shootCtl, 11, 12, ((shootCtl.GetRawButton(11)&&shooterInLim.Get()) || shootCtl.GetRawButton(12)));
	//if ((shootStick.GetRawButton(11) && shooterInLim.Get()) || shootStick.GetRawButton(12))
	//	setMotorDirection(inAndOut2, shootStick, 11, 12); //set intake/fire
	//else inAndOut2.SetSpeed(0);
	*/

	// adjust shooter's vertical elevation using the D-pad
	if (shootCtl.GetPOV() > 90 && shootCtl.GetPOV() < 270 && shooterArmPot.Get() <= 75) // D-pad down
		shooterElevator.SetSpeed(-0.5f);
	else if(shootCtl.GetPOV() < 90 && shootCtl.GetPOV() > 270 && shooterArmPot.Get() >= 0) // D-pad up
		shooterElevator.SetSpeed(-0.5f);
	else shooterElevator.SetSpeed(0);


	//intake and pre-fire controls
	if (shootCtl.GetRawAxis(3) && !shootCtl.GetRawAxis(2)){ // pre-shoot
		inAndOut1.SetSpeed(1);
		inAndOut2.SetSpeed(1);
	} else if (shootCtl.GetRawAxis(2) && !shootCtl.GetRawAxis(3)){ // intake
		inAndOut1.SetSpeed(-0.75f);
		inAndOut2.SetSpeed(-0.75f);
	} else {
		inAndOut1.SetSpeed(0);
		inAndOut2.SetSpeed(0);
	}

	// We're using a piston for the shooter :P
	if (shootCtl.GetRawAxis(3) > 0.95f) // fire the shooter
		shooterPiston.Set(DoubleSolenoid::Value::kReverse);
	else shooterPiston.Set(DoubleSolenoid::Value::kForward); // retract the shooter

	// rumble both controllers when firing and when switching gears.
	if (shootCtl.GetRawAxis(3) > 0.95f) { // rumble for fire (kobe)
		driveCtl.SetRumble(driveCtl.kLeftRumble, 1);
		driveCtl.SetRumble(driveCtl.kRightRumble, 1);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 1);
		shootCtl.SetRumble(shootCtl.kRightRumble, 1);
	} else if (driveCtl.GetRawButton(1) || driveCtl.GetRawButton(2)) { // rumble for gear-shift
		driveCtl.SetRumble(driveCtl.kLeftRumble, 1);
		driveCtl.SetRumble(driveCtl.kRightRumble, 1);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 1);
		shootCtl.SetRumble(shootCtl.kRightRumble, 1);
	} else { // no rumble
		driveCtl.SetRumble(driveCtl.kLeftRumble, 0);
		driveCtl.SetRumble(driveCtl.kRightRumble, 0);
		shootCtl.SetRumble(shootCtl.kLeftRumble, 0);
		shootCtl.SetRumble(shootCtl.kRightRumble, 0);
	}

	//print "Kobe!!" to the terminal when we shoot (for good luck)
	if (m_kobe && shootCtl.GetRawAxis(3) > 0.95f) {
		std::cout <<"Kobe!!" <<std::endl;
		m_kobe = false;
	} else if (shootCtl.GetRawAxis(3) <= 0.95f) {
		m_kobe = true;
	}

}

void Robot::TestInit(){
	std::cout <<"Testing mode enabled...\nCurrently doing: {NULL}" <<std::endl;
}

void Robot::TestPeriodic(){	lw->Run(); }

START_ROBOT_CLASS(Robot)
