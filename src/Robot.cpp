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
#include <Timer.h>
#include <CameraServer.h>

#include <Encoder.h>
#include <I2c.h>
#include <Victor.h>

#include <Timer.h>
#include <Servo.h>
#include <ctime>
#include <AnalogGyro.h>


class Robot : public frc::TimedRobot {
public:
	void RobotInit() {
		m_chooser.AddDefault(kAutoNameDefault, kAutoNameDefault);
		m_chooser.AddObject(kAutoNameCustom, kAutoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
		/*
		 * Drive Train setup
		 */
		frontLeft  = new Victor(1); // the number you pass the victor class is the pwm channel
		frontRight = new Victor(2);
		rearLeft   = new Victor(3);
		rearRight  = new Victor(4);

		myDrive = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight );
		/*
		 * Controller setup
		 */
		driveStick = new Joystick(1);

		/*
		 * Sensor setup
		 */
		gyro = new AnalogGyro(0);
		gyro->InitGyro();
		gyro->Calibrate();




	}
	/*
	 * Drive function. This controls the drive train
	 */
	void drive(double x, double y, double z, double gyro = 0) {
			/*switch (driveLevel) {
			case (driveSlow):
				x = (x / 1) * strafeSlow;
				y = (y / 1) * driveSlow;
				z = (z / 1) * turnSlow;
				break;
			case (driveNormal):
				x = (x / 1) * strafeNormal;
				y = (y / 1) * driveNormal;
				z = (z / 1) * turnNormal;
				break;
			case (driveFull):
				x = (x / 1) * strafeFull;
				y = (y / 1) * driveFull;
				z = (z / 1) * turnFull;
				break;
			default:
				x = 0;
				y = 0;
				z = 0;
			}*/

			myDrive->ArcadeDrive(driveStick);
			//robotDrive.MecanumDrive_Cartesian(x, y, z, gyro);
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
		m_autoSelected = m_chooser.GetSelected();
		// m_autoSelected = SmartDashboard::GetString("Auto Selector",
		//		 kAutoNameDefault);
		std::cout << "Auto selected: " << m_autoSelected << std::endl;

		if (m_autoSelected == kAutoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (m_autoSelected == kAutoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}


	void TeleopInit() {}


	void TeleopPeriodic() {

		drive(0,0,0,0);
		SmartDashboard::PutString("DB/String 0", "My 21 Char TestString");
		SmartDashboard::PutNumber("GryoAngle", gyro->GetAngle());
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
	 * Drive train class setup
	 */


	class RobotDrive *myDrive;
	class Victor *frontLeft, *frontRight, *rearLeft, *rearRight;

	/*
	 * Controller setup
	 */
	class Joystick *driveStick; // main drive train Joystick

	/*
	 * Sensor Setup
	 */
	class AnalogGyro *gyro;
	const float kP = 0.03;


};

START_ROBOT_CLASS(Robot)
