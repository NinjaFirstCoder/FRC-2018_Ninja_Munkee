/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>


#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <TimedRobot.h>

#include <Joystick.h>
#include <RobotDrive.h>
#include <Drive/DifferentialDrive.h>
#include <Timer.h>
#include <CameraServer.h>

#include <Encoder.h>
#include <I2c.h>
#include <Victor.h>

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

	}
	/*
	 * Drive function. This controls the drive train
	 */
	void drive(double x, double y, double gyro = 0) {
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

		/* Add joystick switches here
		 *
		 */
		if(MainJoystick->GetRawButton(1)){
			driveLevel = driveNormal;
		} else if(MainJoystick->GetRawButton(2)){
			driveLevel = driveFull;
		} else if(MainJoystick->GetRawButton(3)) {
			driveLevel = driveSlow;
		}
	}

	/*
	 * Functions grabs values from gyros and other sensors
	 */
	void pollSensors(){


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
		if(test) {
			navxgyro->ZeroYaw();
			test = false;
		}
		float gyroangle = navxgyro->GetAngle();

		m_drive.CurvatureDrive(0.5, -gyroangle * kP,false);
		//drive(0.5,0,-navxgyro->GetAngle()*kP);

		/*
		if (m_autoSelected == kAutoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
		*/
	}


	void TeleopInit() {}


	void TeleopPeriodic() {
		pollControllers(); //
		pollSensors();

		drive(joystickX,joystickY,0);
		//SmartDashboard::PutString("DB/String 0", "My 21 Char TestString");
		if(GyroFound) {
			SmartDashboard::PutNumber("GryoAngle", navxgyro->GetAngle());
		}
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
	double joystickX = 0;
	double joystickY = 0;
	Joystick *MainJoystick;



	/*
	 * Sensor Setup
	 */
	class AHRS *navxgyro;

	/* The following PID Controller coefficients will need to be tuned */
	/* to match the dynamics of your drive system.  Note that the      */
	/* SmartDashboard in Test mode has support for helping you tune    */
	/* controllers by displaying a form where you can enter new P, I,  */
	/* and D constants and test the mechanism.                         */

	double kP = 0.03f;
	double kI = 0.00f;
	double kD = 0.00f;
	double kF = 0.00f;

	/* This tuning parameter indicates how close to "on target" the    */
	/* PID Controller will attempt to get.                             */

	double kToleranceDegrees = 2.0f;

	/* Function Variables
	 *
	 */
	bool GyroFound = true;
	bool test = true;



};

START_ROBOT_CLASS(Robot)
