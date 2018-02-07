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

// autonomous config files
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <cstdlib>
using namespace std;
#define OPERATION_ELEMENTS 13

class Robot : public frc::TimedRobot {
public:
	/* setup the drive train
	 *
	 */
	 frc::Victor m_frontLeft{DriveM_FrontLeft};
	 frc::Victor m_midLeft{DriveM_MiddleLeft};
	 frc::Victor m_rearLeft{DriveM_RearLeft};
	 frc::SpeedControllerGroup m_left{m_frontLeft, m_midLeft, m_rearLeft};

	 frc::Victor m_frontRight{DriveM_FrontRight};
	 frc::Victor m_midRight{DriveM_MiddleRight};
	 frc::Victor m_rearRight{DriveM_RearRight};
	 frc::SpeedControllerGroup m_right{m_frontRight, m_midRight, m_rearRight};

	 frc::DifferentialDrive m_drive{m_left, m_right};



	 /***************************************************
	  * AUTONOMOUS CONFIG FILE LOADING CODE
	  */
	 struct node {
	 	double data[OPERATION_ELEMENTS];
	 	node *next;
	 };
	 class list {
	 	 	 private:
	 			int x;
	 		public:
	 			node *head, *tail;
	 			list() {  // setup class with null pointers
	 				head=NULL;
	 				tail=NULL;
	 				x=0;
	 			}
	 			void createnode(double value[]) {
	 				node *temp=new node;
	 				for(x = 0; x<OPERATION_ELEMENTS; x++) { // transfer data not a pointer
	 					temp->data[x] = value[x];
	 				}
	 				temp->next=NULL;
	 				if(head==NULL)
	 				{
	 					head=temp;
	 					tail=temp;
	 					temp=NULL;
	 				}
	 				else
	 				{
	 					tail->next=temp;
	 					tail=temp;
	 				}
	 			}
	 			void display() {
	 				node *temp=new node;
	 				temp=head;
	 				int z;
	 				while(temp!=NULL)
	 				{
	 					for(z=0; z<OPERATION_ELEMENTS; z++) {
	 						cout << temp->data[z] << "\n";
	 					}
	 					temp=temp->next;
	 				}
	 			}
	 };
	 //list autoMode1, autoMode2, autoMode3, autoMode4, autoMode5, autoMode6;

	 class AutoConfigLoader {
	 public:
		 list autoMode1, autoMode2, autoMode3, autoMode4, autoMode5, autoMode6;
		 bool FileNotFound = false;
		 void loadConfig() {
			 	 int y = 0;
			 	 ifstream myfile("/media/sda1/Config.txt");

			 	 string line;
			 	 string newString;

			 	 int x;
			 	 size_t charNumb;
			 	 size_t charNumb2;
			 	 size_t stringlen;


			 	 char CurrentModeName[20];


			 	 bool foundEndOfBlock = false;
			 	 double randomArray[OPERATION_ELEMENTS];

			 	 //myfile.open ("C:/Users/mainuser/Documents/Programming/Projects/ConfigFileGrabber/src/config.txt");
			 	 if (myfile.is_open()) {
			 			while (getline(myfile,line)) {
			 				if(line.length() != 0) {
			 					if(line.at(0) == '-') { // auto block found
			 						line.erase(line.begin()); 									// remove first '-'
			 						charNumb = line.find('-');                                  // find pos of last '-'
			 						stringlen = line.copy(CurrentModeName , charNumb , 0);   // copy up to the last '-'
			 						CurrentModeName[stringlen] = '\0';                       // put a null at the end of the string
			 						//cout << "NEW NAME:" << CurrentModeName << '\n';                  // print the value
			 						// CHECK INTO REMOVING SPACES HERE

			 						foundEndOfBlock = true;
			 						while(foundEndOfBlock) {

			 							getline(myfile,line);                   // grab the next line
			 							if(line.find("Operation") != std::string::npos) { // found the operation line line.compare(0,9,"Operation") == 0
			 								x = 0;
			 								while(x != OPERATION_ELEMENTS) {
			 									getline(myfile,line);
			 									charNumb2 = line.find(':');
			 									line.erase(0, charNumb2);
			 									for (string::iterator it = line.begin(); it != line.end(); ++it) { // remove all non digits
			 									        if (isdigit(*it)) newString.push_back(*it);
			 									}

			 									randomArray[x] = atoi(newString.c_str());
			 									newString.clear();
			 									x++;
			 								}
			 								if(!strcmp("AutoMode_1",CurrentModeName)) {
			 									autoMode1.createnode(randomArray);
			 									y++;
			 								}
			 								else if(!strcmp("AutoMode_2",CurrentModeName)) {
			 									autoMode2.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_3",CurrentModeName)) {
			 									autoMode3.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_4",CurrentModeName)) {
			 									autoMode4.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_5",CurrentModeName)) {
			 									autoMode5.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_6",CurrentModeName)) {
			 									autoMode6.createnode(randomArray);
			 								}

			 							} else if(line.find("-END-") != std::string::npos) {
			 								foundEndOfBlock = false;
			 							}

			 						}

			 					}
			 				}
			 			}
			 			myfile.close();

			 			frc::SmartDashboard::PutNumber("Mode 1 states loaded", y);

				 		frc::SmartDashboard::PutString("AutoLoaded", "FOUND");
			 	 }
			 	 else {
			 		 // PRINT ERROR MESSAGE OR SEARCH FOR FILE HERE
			 		frc::SmartDashboard::PutString("AutoLoaded", "NOT FOUND");
			 		FileNotFound= true;
			 	 }
		 }

	 };

	 /********************************************************
	  * RobotInit is the first function that is run when the robot starts. Put
	  * all of the motor controller/joystick/etc setup inside.
	  */
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

		 // Joystick setup
		 MainJoystick = new Joystick(MAIN_JOYSTICK);
		 ArmJoystick = new Joystick(SECONDARY_JOYSTICK);

		 // Arm setup
		 ArmTalon = new TalonSRX(ARM_CONTROLLER_PORT);
		 ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,0);
		 ArmTalon->SetSensorPhase(true);

		 ArmTalon->Config_kF(kPIDLoopIdx, CONST_kF, kTimeoutMs);
		 ArmTalon->Config_kP(kPIDLoopIdx, CONST_kP, kTimeoutMs);
		 ArmTalon->Config_kI(kPIDLoopIdx, CONST_kI, kTimeoutMs);
		 ArmTalon->Config_kD(kPIDLoopIdx, CONST_kF, kTimeoutMs);
		 ArmTalon->ConfigPeakOutputForward((double) ARM_POWER, kTimeoutMs);
		 ArmTalon->ConfigPeakOutputReverse((double)-ARM_POWER, kTimeoutMs);

		 // Intake setup
		 IntakeTalon = new Talon(9);

		 // Grabber setup
		 GrabberTalon = new TalonSRX(2);
		 GrabberTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,0);
		 GrabberTalon->SetSensorPhase(false);

		 // auto config loader
		 AutoConfig.loadConfig();
		// chooser.AddDefault("Automatic", new AutoManual());
		 //chooser.AddObject("Manual 1", new AutoSelection());

//		 frc::SmartDashboard::PutData("Auto Modes", &chooser);

	}
	/********************************************************
	 * Drive function. This runs the drive train
	 */
	void drive(double x, double y) {
			x = x * driveLevel;
			y = y * driveLevel;
			m_drive.ArcadeDrive(x,y);
	}


	/*******************************************************
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

	/*****************************************************
	 * Functions grabs values from gyros and other sensors
	 */
	void pollSensors(){


	}

	/*****************************************************
	 * Function runs intake motors
	 */
	void runIntake() {
		if(intakeForward) {
			IntakeTalon->Set(intakeSpeed);
		} else if(intakeBackward) {
			IntakeTalon->Set(-intakeSpeed);
		}else {
			IntakeTalon->Set(0);
		}
	}

	/****************************************************
	 * Function runs the grabber. NOTE: the grabber is the thing on the end of the arm
	 */
	void runGrabber() {
		if(grabberForward) {
			GrabberTalon->Set(ControlMode::PercentOutput, 0.5);
		} else if(grabberBackward) {
			GrabberTalon->Set(ControlMode::PercentOutput, -0.5);
		} else {
			GrabberTalon->Set(ControlMode::PercentOutput, 0);
		}

	}

	/****************************************************
	 * Function runs the grabber pneumatics
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

	/*****************************************************
	 * Function runs the arm motor
	 */
	void runArm() {
		if(ArmButtons.low) {
			arm_currentPos = 0;
		} else if(ArmButtons.mid) {
			arm_currentPos = ARM_POS_1;
		} else if(ArmButtons.high){
			arm_currentPos = ARM_POS_2;
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
		//autonomousCommand.reset(chooser.GetSelected());
		//if(autonomousCommand.get() != nullptr) {
		//	autonomousCommand->Start();
		//}
	/*	m_autoSelected = m_chooser.GetSelected();
		 m_autoSelected = SmartDashboard::GetString("Auto Selector",
				 kAutoNameDefault);
		std::cout << "Auto selected: " << m_autoSelected << std::endl;

		if (m_autoSelected == kAutoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}*/
			autonomousVars.CompletingOperation = true;
			autonomousVars.DriveOperationDone = false;
			autonomousVars.ArmOperationDone = false;
			autonomousVars.GrabberOperationDone = false;
			autonomousVars.IntakeOperationDone = false;
			autonomousVars.TimeOperationDone = false;
			autonomousVars.foundList = false;

	}

	void AutonomousPeriodic() {
		if(!autonomousVars.foundList) {
			//SmartDashboard::PutString("CURRENTAUTOSTATE", "started");
			int AutoMode = SmartDashboard::GetNumber("Autonomous Mode", 404); // may need to be changed from 0
			SmartDashboard::PutNumber("FOUND", AutoMode);

			if(AutoConfig.FileNotFound) { // if the auto config wasn't loaded
				drive(0,0); // update drive so it doesn't error
				return;
			}
			switch(AutoMode){
				case(0): // Automatic selection
						CurrentAutoMode = &AutoConfig.autoMode1; // put the stuff from that list into the current list
						autonomousVars.foundList = true;
						break;
				case(1):
						CurrentAutoMode = &AutoConfig.autoMode2;
						autonomousVars.foundList = true;
						break;
				case(2):
						CurrentAutoMode = &AutoConfig.autoMode3;
						autonomousVars.foundList = true;
						break;
				case(3):
						CurrentAutoMode = &AutoConfig.autoMode4;
						autonomousVars.foundList = true;
						break;
				case(4):
						CurrentAutoMode = &AutoConfig.autoMode5;
						autonomousVars.foundList = true;
						break;
				default:
					break;

			}
			autonomousVars.autoTemp = CurrentAutoMode->head;
			// zero out sensors
			ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
		} else {
			if(!autonomousVars.CompletingOperation) { // if not currently in a operation
				// grab next operation
				if(autonomousVars.autoTemp != NULL) {
					// grab next operation
					autonomousVars.autoTemp = autonomousVars.autoTemp->next; // grab next operation
					if(autonomousVars.autoTemp!=NULL) {
						autonomousVars.CompletingOperation = true;
						SmartDashboard::PutString("CURRENTAUTOSTATE", "grabbing next value");
					} else {
						autonomousVars.CompletingOperation = false;
						SmartDashboard::PutString("CURRENTAUTOSTATE", "FOUND NULL");
					}
				}
				else {
					SmartDashboard::PutString("CURRENTAUTOSTATE", "FOUND NULL");
					autonomousVars.CompletingOperation = false;
				}
			} else {
				// run current operation

				/*
				 * Run arm operations
				 */
				if(ArmTalon->GetSelectedSensorPosition(0) < (autonomousVars.autoTemp->data[0] - ARM_AUTO_ERROR) || ArmTalon->GetSelectedSensorPosition(0) > (autonomousVars.autoTemp->data[0] + ARM_AUTO_ERROR) ){
					ArmTalon->ConfigPeakOutputForward((double) autonomousVars.autoTemp->data[1], kTimeoutMs);
					ArmTalon->ConfigPeakOutputReverse((double)-autonomousVars.autoTemp->data[1], kTimeoutMs);
					ArmTalon->Set(ControlMode::Position, autonomousVars.autoTemp->data[0]);
				} else {
					autonomousVars.ArmOperationDone = true;
				}
				/*
				 * run time operations
				 */
				if(autonomousVars.autoTemp->data[12] != autonomousVars.timeCount) {
					autonomousVars.timeCount++;
					SmartDashboard::PutNumber("Time Ticks", autonomousVars.timeCount);
					SmartDashboard::PutNumber("Target Time", autonomousVars.autoTemp->data[12]);
				} else {
					autonomousVars.TimeOperationDone = true;
				}


				if(autonomousVars.ArmOperationDone && autonomousVars.TimeOperationDone) {
					SmartDashboard::PutString("CURRENTAUTOSTATE", "Finished arm");
					autonomousVars.CompletingOperation = false;
					autonomousVars.ArmOperationDone = false;
					autonomousVars.TimeOperationDone = false;

					// reset time
					autonomousVars.timeCount = 0;
				}

			}
		}
		drive(0,0);
		SmartDashboard::PutNumber("Talon Sensor Position", ArmTalon->GetSelectedSensorPosition(0));
		/*
		node *temp = new node;
		temp = CurrentAutoMode->head;
		SmartDashboard::PutNumber("Autodata0", temp->data[0]);
		SmartDashboard::PutNumber("Autodata1", temp->data[1]);
		SmartDashboard::PutNumber("Autodata2", temp->data[2]);
		SmartDashboard::PutNumber("Autodata3", temp->data[3]);
		SmartDashboard::PutNumber("Autodata4", temp->data[4]);
		SmartDashboard::PutNumber("Autodata5", temp->data[5]);


		drive(0,0);*/
		//frc::Scheduler::GetInstance()->Run();
		/*
		 * Auto mode works similar to a state machine.
		 */
		/*
		double kP = 1;
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
		}*/
	}

	void AutoSelection() {
		SmartDashboard::PutString("FIRST", "RAN");
	}
	void AutoManual() {
		SmartDashboard::PutString("SECOND", "RAN");
	}

	void TeleopInit() {
		// RESET TALON SPEED HERE
		ArmTalon->ConfigPeakOutputForward((double) ARM_POWER, kTimeoutMs);
		ArmTalon->ConfigPeakOutputReverse((double)-ARM_POWER, kTimeoutMs);

	}


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

	double arm_currentPos = 0;

	/* This tuning parameter indicates how close to "on target" the    */
	/* PID Controller will attempt to get.                             */

//	double kToleranceDegrees = 2.0f;

	/* Function Variables
	 *
	 */
	bool GyroFound = true;
	int autoState = 1;

	/*
	 * Motor setup
	 */
	Talon *IntakeTalon;
	TalonSRX *ArmTalon;
	TalonSRX *GrabberTalon;


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

	struct autonomousThings {
		bool CompletingOperation = true;
		bool DriveOperationDone = false;
		bool ArmOperationDone = false;
		bool GrabberOperationDone = false;
		bool IntakeOperationDone = false;
		bool TimeOperationDone = false;
		bool foundList = false;
		double timeCount = 0;
		node *autoTemp = new node;
	} autonomousVars;

	std::string dashData1;
	std::string dashData2;
	std::string dashData3;
	std::string dashData4;
	std::string dashData5;

	AutoConfigLoader AutoConfig;
	list *CurrentAutoMode;
	//std::unique_ptr<frc::Command> autonomousCommand;
	//frc::SendableChooser<frc::Command*> chooser;

};

START_ROBOT_CLASS(Robot)
