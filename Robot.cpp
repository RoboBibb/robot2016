#include "Robot.h"

Robot::Robot() : //inline initializations:
    	myRobot(0, 1), //left0, right1
	gearShifter(0,1), shooterPiston(2,3), //solenoids
	driveStick(0), shootStick(1), //they want 2 joysicks now
	airPump(0), //compressor
	shooterElevator(2), //set elevation of the shooter
	inAndOut1(8), inAndOut2(9), //shooter motors
	shooterUpLim(0), shooterDownLim(1), shooterInLim(5) //limit switches
{
	myRobot.SetExpiration(0.1);
}


//used to control a motors direction using 2 buttons (fwd & bkwd)
// function template that allows control of any motor controller using 2 buttons.
// Austin and Prem should also be able to understand (some of) this function...
//ToDo: (in the future add a condition param)
template <class MOTCTLR>
void Robot::setMotorDirection(MOTCTLR& motor, Joystick& joystick, const unsigned int& fwd, const unsigned int& bkwd){
	if (joystick.GetRawButton(fwd) == joystick.GetRawButton(bkwd)) 
		motor.SetSpeed(0);
	else if (joystick.GetRawButton(fwd)) 
		motor.SetSpeed(1);
	else motor.SetSpeed(-1); //bkwd
}

void Robot::RobotInit(){
	//setup the auto-chooser:
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom1, (void*)&autoNameCustom1);

	SmartDashboard::PutData("Auto Modes", chooser);

	//get camera feed and post it to the smartdashboard (*NOTE: disabled until we get a camera)
	//CameraServer::GetInstance()->SetQuality(50);
	//CameraServer::GetInstance()->StartAutomaticCapture("cam0");//get camera name (cam0) from the roborio web interface

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
	//start filling the tank
	airPump.SetClosedLoopControl(true);

    //auto-choosing code
	autoSelected = *((std::string*)chooser->GetSelected());
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault); //use this for labview drive station

	//print the chosen autonomous code to the terminal
	std::cout <<"Autonomous has started...\nAuto selected: " <<autoSelected <<"\n";

	//run selected autonomous code
	if (autoSelected == autoNameCustom1) {
		//Custom Auto goes here
	} else {
		//Default Auto goes here
	}
}

void Robot::AutonomousPeriodic(){
	if (autoSelected == autoNameCustom1) {
		//Custom Auto goes here
	} else {
		//Default Auto goes here

		///The simplest (lame) autonomous code
		// if (NO_OBSTACLES_IN_THE_WAY) DRIVE FORWARD
		// else STOP
	}
}

void Robot::TeleopInit(){
	//start filling the tank
	airPump.SetClosedLoopControl(true);

	//tele-op has started.
	std::cout <<"Teleop has started...\n";

	//start out in low gear (50% CHANCE OF BEING CORRECT)
	gearShifter.Set(DoubleSolenoid::Value::kReverse);

}

void Robot::TeleopPeriodic(){

	//"all of these could be parallel processes."
	// -LabVIEW (2014-2015 RIP)

	//drive the robot
	myRobot.ArcadeDrive( -driveStick.GetY(), -driveStick.GetX(), false);

	//shift gears as according to buttons 11 & 12 (adjust upon request)
	if (driveStick.GetRawButton(11) && m_isHighGear) {
		gearShifter.Set(DoubleSolenoid::Value::kForward);
		std::cout <<"Low Gear\n";
		m_isHighGear = !m_isHighGear;
	} else if (driveStick.GetRawButton(12) && !m_isHighGear) {
		gearShifter.Set(DoubleSolenoid::Value::kReverse);
		std::cout <<"High Gear\n";
		m_isHighGear = !m_isHighGear;
	}

	//sebastian wants to test using a piston for the shooter
	if (shootStick.GetTrigger() && !shootStick.GetRawButton(2)) // fire the shooter
		shooterPiston.Set(DoubleSolenoid::Value::kReverse);
	else if (shootStick.GetTrigger() && shootStick.GetRawButton(2))
		std::cout <<"Warning: you cannot shoot and suck at the same time.\n";
	else shooterPiston.Set(DoubleSolenoid::Value::kForward);

	//adjust shooter's vertical angle
	/// the limits will likely have to be swapped (50% chance)
	// if it's safe to move the motor, run the code to do so
	if ((shootStick.GetRawButton(3) && shooterUpLim.Get()) != (shootStick.GetRawButton(5) && shooterDownLim.Get()))
		setMotorDirection(shooterElevator, shootStick, 3, 5);
	else //stop the motor if it isn't safe
		shooterElevator.SetSpeed(0);

	//intake and pre-fire controls (button 3 starts the shooter motors spinning)
	if ((shootStick.GetRawButton(11) && shooterInLim.Get()) || shootStick.GetRawButton(12))
			setMotorDirection(inAndOut1, shootStick, 11, 12); //set intake/fire
	else inAndOut2.SetSpeed(0);

    //this is because they want the colors to match on the motors
	if ((shootStick.GetRawButton(11) && shooterInLim.Get()) || shootStick.GetRawButton(12))
		setMotorDirection(inAndOut2, shootStick, 11, 12); //set intake/fire
	else inAndOut2.SetSpeed(0);

	//print "Kobe!!" to the terminal when we shoot (for good luck)
	/// this code makes it only print once for each time the trigger is pressed
	if (m_kobe && shootStick.GetTrigger()) {
		std::cout <<"Kobe!!\n";
		m_kobe = false;
	} else if (!shootStick.GetTrigger()) {
		m_kobe = true;
	}
}

void Robot::TestInit(){
	std::cout <<"Testing mode enabled...\nCurrently doing: (NULL)"; //as though there was a purpose for testing mode...
}

void Robot::TestPeriodic(){	lw->Run(); }

START_ROBOT_CLASS(Robot)
