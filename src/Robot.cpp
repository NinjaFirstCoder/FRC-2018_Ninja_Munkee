/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include "WPILib.h"
#include "Constants.h"

#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <TimedRobot.h>

#include <Joystick.h>
#include <RobotDrive.h>
#include <Drive/DifferentialDrive.h>
#include <Timer.h>
#include <CameraServer.h>

#include "ctre/Phoenix.h"
#include <Encoder.h>
#include <I2c.h>
#include <Victor.h>
#include <Talon.h>

#include <Timer.h>
#include <Servo.h>
#include <ctime>
#include <AnalogGyro.h>

#include <AHRS.h>

class Robot : public frc::TimedRobot {
public:
	/* setup the drive train
	 *
	 */
	 frc::Victor m_frontLeft{1};
	 frc::Victor m_midLeft{2};
	 frc::Victor m_rearLeft{3};
	 frc::SpeedControllerGroup m_left{m_frontLeft, m_midLeft, m_rearLeft};

	 frc::Victor m_frontRight{4};
	 frc::Victor m_midRight{5};
	 frc::Victor m_rearRight{6};
	 frc::SpeedControllerGroup m_right{m_frontRight, m_midRight, m_rearRight};

	 frc::DifferentialDrive m_drive{m_left, m_right};

	 /*
	  * Setup the controller
	  */
	 //frc::Joystick m_joystick{0};



	void RobotInit() {
		m_chooser.AddDefault(kAutoNameDefault, kAutoNameDefault);
		m_chooser.AddObject(kAutoNameCustom, kAutoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

		try {
		/***********************************************************************
		 * navX-MXP:
		 * - Communication via RoboRIO MXP (SPI, I2C, TTL UART) and USB.
		 * - See http://navx-mxp.kauailabs.com/guidance/selecting-an-interface.
		 *
		 * navX-Micro:
		 * - Communication via I2C (RoboRIO MXP or Onboard) and USB.
		 * - See http://navx-micro.kauailabs.com/guidance/selecting-an-interface.
		 *
		 * Multiple navX-model devices on a single robot are supported.
		 ************************************************************************/
			 navxgyro = new AHRS(SerialPort::Port::kUSB);
		 } catch (std::exception& ex ) {
			 std::string err_string = "Error instantiating navX MXP:  ";
		     err_string += ex.what();
		     DriverStation::ReportError(err_string.c_str());
		     GyroFound = false;
		 }
		 navxgyro->ZeroYaw();

		 /*
		  * Setup controllers
		  */
		 MainJoystick = new Joystick(0);
		 ArmJoystick = new Joystick(1);

		 ArmTalon = new TalonSRX(1);
		 ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,0);
		 ArmTalon->SetSensorPhase(true);

		 ArmTalon->Config_kF(kPIDLoopIdx, kF, kTimeoutMs);
		 ArmTalon->Config_kP(kPIDLoopIdx, kP, kTimeoutMs);
		 ArmTalon->Config_kI(kPIDLoopIdx, kI, kTimeoutMs);
		 ArmTalon->Config_kD(kPIDLoopIdx, kD, kTimeoutMs);
		 ArmTalon->ConfigPeakOutputForward(arm_power, kTimeoutMs);
		 ArmTalon->ConfigPeakOutputReverse(-arm_power, kTimeoutMs);
		 intake = new Talon(9);

		 // grabber setup
		 //talonGrabber = new TalonSRX(2);
		 //talonGrabber->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,0);
		 //talonGrabber->SetSensorPhase(false);


	}
	/*
	 * Drive function. This controls the drive train
	 */
	void drive(double x, double y) {
			x = x * driveLevel;
			y = y * driveLevel;
			m_drive.ArcadeDrive(x,y);
	}


	/*
	 * Function grabs values from joysticks and gamepads
	 */
	void pollControllers(){
		joystickX = MainJoystick->GetY();
		joystickY = MainJoystick->GetX();
		intakeSpeed = (MainJoystick->GetZ()-1)/2;

		/* Add joystick switches here
		 *
		 */

		// buttons for the drive train levels
		if(MainJoystick->GetRawButton(1)){
			driveLevel = driveNormal;
		} else if(MainJoystick->GetRawButton(2)){
			driveLevel = driveFull;
		} else if(MainJoystick->GetRawButton(3)) {
			driveLevel = driveSlow;
		}

		// buttons for the intake levels
		if(MainJoystick->GetRawButton(11)) {
			intakeForward = 1;
			intakeBackward = 0;
		} else if(MainJoystick->GetRawButton(10)){
			intakeForward = 0;
			intakeBackward = 1;
		} else {
			intakeForward = 0;
			intakeBackward = 0;
		}

		// buttons for the grabber levels
		if(MainJoystick->GetRawButton(8)) {
			grabberForward = 1;
			grabberBackward = 0;
		} else if(MainJoystick->GetRawButton(9)){
			grabberForward = 0;
			grabberBackward = 1;
		} else {
			grabberForward = 0;
			grabberBackward = 0;
		}

		if(MainJoystick->GetRawButton(4)) {
			grabberPneumaticsForward = true;
			grabberPneumaticsBackward = false;

		} else if(MainJoystick->GetRawButton(5)){
			grabberPneumaticsForward = false;
			grabberPneumaticsBackward = true;

		}else {
			grabberPneumaticsForward = false;
			grabberPneumaticsBackward = false;
		}

		// arm controller
		if(ArmJoystick->GetRawButton(2)){
			ArmButtons.low = true;
			ArmButtons.mid = false;
			ArmButtons.high = false;
		} else if(ArmJoystick->GetRawButton(3)){
			ArmButtons.low = false;
			ArmButtons.mid = true;
			ArmButtons.high = false;
		} else if(ArmJoystick->GetRawButton(1)){
			ArmButtons.low = false;
			ArmButtons.mid = false;
			ArmButtons.high = true;
		} else{
			ArmButtons.low = false;
			ArmButtons.mid = false;
			ArmButtons.high = false;
		}





	}

	/*
	 * Functions grabs values from gyros and other sensors
	 */
	void pollSensors(){


	}

	void runIntake() {
		if(intakeForward) {
			intake->Set(intakeSpeed);
		} else if(intakeBackward) {
			intake->Set(-intakeSpeed);
		}else {
			intake->Set(0);
		}
	}

/*
	void runGrabber() {
		if(grabberForward) {
			talonGrabber->Set(ControlMode::PercentOutput, 0.5);
		} else if(grabberBackward) {
			talonGrabber->Set(ControlMode::PercentOutput, -0.5);
		} else {
			talonGrabber->Set(ControlMode::PercentOutput, 0);
		}

	}
*/
	void runGrabberPneumatics() {
		if(grabberPneumaticsForward) {
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		} else if(grabberPneumaticsBackward) {
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		} else {
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kOff);
		}
	}

	void runArm() {
		if(ArmButtons.low) {
			arm_currentPos = 0;
		} else if(ArmButtons.mid) {
			arm_currentPos = 1000;
		} else if(ArmButtons.high){
			arm_currentPos = 4096;
		} else {
			arm_currentPos += ArmJoystick->GetY() * 100;
			//ArmTalon->Set(ControlMode::PercentOutput, ArmJoystick->GetY());
		}

		ArmTalon->Set(ControlMode::Position, arm_currentPos);

/*		if(ArmJoystick->GetRawButton(8)) {
			dashData1 = SmartDashboard::GetString("DB/String 0", "myDefaultData");
			dashData2 = SmartDashboard::GetString("DB/String 1", "myDefaultData");
			dashData3 = SmartDashboard::GetString("DB/String 2", "myDefaultData");
			dashData4 = SmartDashboard::GetString("DB/String 3", "myDefaultData");
			dashData5 = SmartDashboard::GetString("DB/String 4", "myDefaultData");

			ArmTalon->Config_kF(kPIDLoopIdx, atof(dashData1.c_str()), kTimeoutMs);
			ArmTalon->Config_kP(kPIDLoopIdx, atof(dashData2.c_str()), kTimeoutMs);
			ArmTalon->Config_kI(kPIDLoopIdx, atof(dashData3.c_str()), kTimeoutMs);
			ArmTalon->Config_kD(kPIDLoopIdx, atof(dashData4.c_str()), kTimeoutMs);
			ArmTalon->ConfigPeakOutputForward((double) atof(dashData5.c_str()), kTimeoutMs);
			ArmTalon->ConfigPeakOutputReverse(-(double) atof(dashData5.c_str()), kTimeoutMs);

			SmartDashboard::PutNumber("1", atof(dashData1.c_str()) );
			SmartDashboard::PutNumber("2", atof(dashData2.c_str()));
			SmartDashboard::PutNumber("3", atof(dashData3.c_str()));
			SmartDashboard::PutNumber("4", atof(dashData4.c_str()));


		}*/

//		ArmTalon->Set(ControlMode::Position, 1000);

	}
	/*
	 * This autonomous (along with the chooser code above) shows how to
	 * select between different autonomous modes using the dashboard. The
	 * sendable chooser code works with the Java SmartDashboard. If you
	 * prefer the LabVIEW Dashboard, remove all of the chooser code and
	 * uncomment the GetString line to get the auto name from the text box
	 * below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to
	 * the if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as
	 * well.
	 */
	void AutonomousInit() override {
		/*m_autoSelected = m_chooser.GetSelected();
		// m_autoSelected = SmartDashboard::GetString("Auto Selector",
		//		 kAutoNameDefault);
		std::cout << "Auto selected: " << m_autoSelected << std::endl;

		if (m_autoSelected == kAutoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}*/
	}

	void AutonomousPeriodic() {
		/*
		 * Auto mode works similar to a state machine.
		 */
		switch(autoState) {
			case(1):
				navxgyro->ZeroYaw(); // zero out gyro when auto starts
				autoState = 2;
				break;
			case(2):
				gyroAngle = navxgyro->GetAngle();
				m_drive.CurvatureDrive(0.5, -gyroAngle * kP,false); // get the angle of the gyro and drive straight
				break;
			default:
				break;
		}
	}


	void TeleopInit() {}


	void TeleopPeriodic() {
		pollControllers(); //
		pollSensors();

		runIntake();
		//runGrabber();
		runArm();
		runGrabberPneumatics();
		drive(joystickX,joystickY);
		//SmartDashboard::PutString("DB/String 0", "My 21 Char TestString");
		if(GyroFound) {
			SmartDashboard::PutNumber("GryoAngle", navxgyro->GetAngle());
		}
		SmartDashboard::PutNumber("Talon Sensor Velocity", ArmTalon->GetSelectedSensorVelocity(0));
		SmartDashboard::PutNumber("Talon Sensor Position", ArmTalon->GetSelectedSensorPosition(0));
	}

	void TestPeriodic() {}

private:
	frc::LiveWindow& m_lw = *LiveWindow::GetInstance();
	frc::SendableChooser<std::string> m_chooser;
	const std::string kAutoNameDefault = "Default";
	const std::string kAutoNameCustom = "My Auto";
	std::string m_autoSelected;

	/*
	 * Main program variables. Levels and stuff
	 */
	double driveSlow = 0.3, driveNormal = 0.6, driveFull = 1;
	double driveLevel = driveNormal;


	/*
	 * Controller setup. Joy-sticks, buttons etc.
	 */
	double joystickX = 0;
	double joystickY = 0;
	Joystick *MainJoystick;
	Joystick *ArmJoystick;


	/*
	 * Sensor Setup
	 */
	class AHRS *navxgyro;
	float gyroAngle;

	/* The following PID Controller coefficients will need to be tuned */
	/* to match the dynamics of your drive system.  Note that the      */
	/* SmartDashboard in Test mode has support for helping you tune    */
	/* controllers by displaying a form where you can enter new P, I,  */
	/* and D constants and test the mechanism.                         */

	double kP = 1;
	double kI = 0.00f;
	double kD = 0.00f;
	double kF = 0.00f;
	double arm_power = 0.5f;
	double arm_currentPos = 0;

	/* This tuning parameter indicates how close to "on target" the    */
	/* PID Controller will attempt to get.                             */

	double kToleranceDegrees = 2.0f;

	/* Function Variables
	 *
	 */
	bool GyroFound = true;
	int autoState = 1;

	/*
	 * Motor setup
	 */
	Talon *intake;
	TalonSRX *ArmTalon;

	//TalonSRX *talonGrabber;
	bool intakeForward = false;
	bool intakeBackward = false;
	double intakeSpeed = 0;

	bool grabberForward = false;
	bool grabberBackward = false;

	bool grabberPneumaticsForward = false;
	bool grabberPneumaticsBackward = false;

	frc::DoubleSolenoid grabberSolenoid {0,1};

	struct structArmButtons {
		bool low = false;
		bool mid = false;
		bool high = false;
	} ArmButtons;

	std::string dashData1;
	std::string dashData2;
	std::string dashData3;
	std::string dashData4;
	std::string dashData5;
};

START_ROBOT_CLASS(Robot)
