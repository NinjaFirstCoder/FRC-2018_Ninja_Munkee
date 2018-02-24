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

#include "FieldColorLocations.h"

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

#include <PIDController.h>
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




class Robot : public frc::TimedRobot, public PIDSource, public PIDOutput {
public:
	/* setup the drive train
	 *
	 */
	 frc::Victor m_frontLeft{DriveM_FrontLeft};
	 frc::Victor m_midLeft{DriveM_MiddleLeft};
	 WPI_TalonSRX m_rearLeft{DriveM_RearLeft};
	 frc::SpeedControllerGroup m_left{m_frontLeft, m_midLeft, m_rearLeft};

	 frc::Victor m_frontRight{DriveM_FrontRight};
	 frc::Victor m_midRight{DriveM_MiddleRight};
	 WPI_TalonSRX m_rearRight{DriveM_RearRight};
	 frc::SpeedControllerGroup m_right{m_frontRight, m_midRight, m_rearRight};


	 frc::DifferentialDrive m_drive{m_left, m_right};

	 /***************************************************
	  * AUTONOMOUS CONFIG FILE LOADING CODE
	  */

	 FieldColorLocations fieldColorLocations;

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
		 list autoMode1, autoMode2, autoMode3, autoMode4, autoMode5, autoMode6, autoMode7, autoMode8, autoMode9, autoMode10, autoMode11, autoMode12, autoMode13, autoMode14, autoMode15, autoMode16, autoMode17;
		 bool FileNotFound = false;
		 void loadConfig() {
			 	 int y = 0;
			 	 ifstream myfile("/media/sda1/Config.txt");
			 	 //ifstream myfile("/home/lvuser/Config.txt");

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
			 									/*for (string::iterator it = line.begin(); it != line.end(); ++it) { // remove all non digits
			 									        if (isdigit(*it)) newString.push_back(*it);
			 									}*/
			 									for(int z = 0; z < (signed) line.length(); z++) {
			 										if(line[z] == '0' || line[z] == '1' || line[z] == '2' || line[z] == '3' || line[z] == '4' || line[z] == '5' || line[z] == '6' || line[z] == '7' || line[z] == '8' || line[z] == '9' || line[z] == '.' || line[z] == '-' ) {
			 											newString.push_back(line[z]);
			 										}
			 									}


			 									randomArray[x] = atof(newString.c_str());
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
			 								else if(!strcmp("AutoMode_7",CurrentModeName)) {
			 									autoMode7.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_8",CurrentModeName)) {
			 									autoMode8.createnode(randomArray);
			 								}
			 								else if(!strcmp("AutoMode_9",CurrentModeName)) {
			 									autoMode9.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_10",CurrentModeName)) {
			 									autoMode10.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_11",CurrentModeName)) {
			 									autoMode11.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_12",CurrentModeName)) {
			 									autoMode12.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_13",CurrentModeName)) {
			 									autoMode13.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_14",CurrentModeName)) {
			 									autoMode14.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_15",CurrentModeName)) {
			 									autoMode15.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_16",CurrentModeName)) {
			 									autoMode16.createnode(randomArray);

			 								}else if(!strcmp("AutoMode_17",CurrentModeName)) {
			 									autoMode17.createnode(randomArray);
			 								}
			 							} else if(line.find("-END-") != std::string::npos) {
			 								foundEndOfBlock = false;
			 							}
			 						}
			 					}
			 				}
			 			}
			 			myfile.close();
			 			frc::SmartDashboard::PutNumber("Mode_2_states_loaded", y);
				 		frc::SmartDashboard::PutString("Pulled_Auto_Modes_From_Config", "Yes");
			 	 }
			 	 else { // PRINT ERROR MESSAGE OR SEARCH FOR FILE HERE
			 		frc::SmartDashboard::PutString("Pulled_Auto_Modes_From_Config", "FAILED");
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
		 try {
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
		 GamepadButtons = new Joystick(GAMEPAD_PORT);

		 /****************************************
		  * Drive train motor encoder setup
		  */
		 m_rearLeft.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
		 m_rearLeft.SetSensorPhase(true);
		 m_left.SetInverted(false);

		 m_rearRight.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
		 m_rearRight.SetSensorPhase(false);

		 DrivePIDController = new PIDController(DRIVE_PIDC_Kp, DRIVE_PIDC_Ki, DRIVE_PIDC_Kd, this, this, DRIVE_PIDC_PERIOD);

		 TicksPerInch = (1/(DRIVE_WHEEL_DIA * 3.1415)) * 4096;


		 /***************************************
		  * Arm motor and encoder setup
		  */
		 ArmTalon = new TalonSRX(ARM_CONTROLLER_PORT);
		 ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,0);
		 ArmTalon->SetSensorPhase(true);

		 ArmTalon->Config_kF(kPIDLoopIdx, CONST_kF, kTimeoutMs);
		 ArmTalon->Config_kP(kPIDLoopIdx, CONST_kP, kTimeoutMs);
		 ArmTalon->Config_kI(kPIDLoopIdx, CONST_kI, kTimeoutMs);
		 ArmTalon->Config_kD(kPIDLoopIdx, CONST_kF, kTimeoutMs);
		 //ArmTalon->ConfigPeakOutputForward((double) ARM_POWER, kTimeoutMs);
		 //ArmTalon->ConfigPeakOutputReverse((double)-ARM_POWER, kTimeoutMs);

		 HallEffect = new DigitalInput(9);
		 IntakeSwitch = new DigitalInput(8);

		 /***************************************
		  * Intake motor and encoder setup
		  */
		 IntakeTalonLeft = new Talon(LEFT_INTAKE_PORT);
		 IntakeTalonRight = new Talon(RIGHT_INTAKE_PORT);

		 // Grabber setup
		 GrabberTalon = new TalonSRX(GRABBER_CONTROLLER_PORT);


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
			//x = x * driveLevel;
			//y = y * driveLevel;
			m_drive.ArcadeDrive(x,y);
	}


	/*******************************************************
	 * Function grabs values from joysticks and gamepads
	 */
	void pollControllers(){
		joystickX = -MainJoystick->GetX();
		joystickY = MainJoystick->GetY();
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

		/**********************************************************
		 * buttons for the intake levels
		 */
		if(MainJoystick->GetRawButton(2)) { // run the intake forward
			intakeForward = 1;
			intakeBackward = 0;
		} else if(MainJoystick->GetRawButton(3)){ // run the intake backwards
			intakeForward = 0;
			intakeBackward = 1;
		} else {
			intakeForward = 0;
			intakeBackward = 0;
		}
		if(MainJoystick->GetRawButton(1)) {
			intakeOpen = true;
			intakeClose = false;
		}else if(MainJoystick->GetRawButton(4)){
			intakeOpen = false;
			intakeClose = true;
		}

		if(MainJoystick->GetRawButton(8)) {
			intakeRotateRight = true;
			intakeRotateLeft = false;
		} else if(MainJoystick->GetRawButton(9)) {
			intakeRotateRight = false;
			intakeRotateLeft = true;
		} else {
			intakeRotateRight = false;
			intakeRotateLeft = false;
		}
		/**********************************************************
		 * buttons for the grabber levels
		 */
		if(GamepadButtons->GetRawButton(4)) {
			grabberForward = 1;
			grabberBackward = 0;
		} else if(GamepadButtons->GetRawButton(8)){
			grabberForward = 0;
			grabberBackward = 1;
		} else {
			grabberForward = 0;
			grabberBackward = 0;
		}

		if(GamepadButtons->GetRawButton(6)) {
			grabberOpen = true;
		} else {
			grabberOpen = false;
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
		/**********************************************************
		 * buttons for the arm levels
		 */
		if(GamepadButtons->GetRawButton(3)){
			ArmButtons.low = true;
			ArmButtons.mid = false;
			ArmButtons.high = false;
			ArmButtons.lowmid = false;
		} else if(GamepadButtons->GetRawButton(2)){
			ArmButtons.low = false;
			ArmButtons.mid = true;
			ArmButtons.high = false;
			ArmButtons.lowmid = false;
		} else if(GamepadButtons->GetRawButton(1)){
			ArmButtons.low = false;
			ArmButtons.mid = false;
			ArmButtons.high = true;
			ArmButtons.lowmid = false;
		} else if(GamepadButtons->GetRawButton(7)) {
			ArmButtons.low = false;
			ArmButtons.mid = false;
			ArmButtons.high = false;
			ArmButtons.lowmid = true;
		} else{
			ArmButtons.low = false;
			ArmButtons.mid = false;
			ArmButtons.high = false;
			ArmButtons.lowmid = false;
		}

		if(ArmJoystick->GetRawButton(9)) {
			armZeroDown = false;
			armZeroUp = true;
		} else if(ArmJoystick->GetRawButton(8)) {
			armZeroDown = true;
			armZeroUp = false;
		} else {
			armZeroDown = false;
			armZeroUp = false;
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
			IntakeTalonLeft->Set(-INTAKE_SPEED);
			IntakeTalonRight->Set(INTAKE_SPEED);

		} else if(intakeBackward) {
			IntakeTalonLeft->Set(INTAKE_SPEED);
			IntakeTalonRight->Set(-INTAKE_SPEED);
		} else if (intakeRotateRight) {
			IntakeTalonLeft->Set(-INTAKE_SPEED);
			IntakeTalonRight->Set(-INTAKE_SPEED);
		} else if (intakeRotateLeft) {
			IntakeTalonLeft->Set(INTAKE_SPEED);
			IntakeTalonRight->Set(INTAKE_SPEED);
		} else {

			IntakeTalonRight->Set(0);
			IntakeTalonLeft->Set(0);
		}

		if(intakeOpen) {
			intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		} else if(intakeClose) {
			intakeSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);

		}
		if(IntakeSwitch->Get() && ArmTalon->GetSelectedSensorPosition(0) < 200) {
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		}

	}

	/****************************************************
	 * Function runs the grabber. NOTE: the grabber is the thing on the end of the arm
	 */
	void runGrabber() {
		if(grabberForward) {
			GrabberTalon->Set(ControlMode::PercentOutput, GRABBER_POWER);
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		} else if(grabberBackward) {
			GrabberTalon->Set(ControlMode::PercentOutput, -GRABBER_POWER);
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		} else {
			GrabberTalon->Set(ControlMode::PercentOutput, 0);
		}

		if(grabberOpen) {
			grabberSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
			grabberOpen = false;
		}

	}

	/*****************************************************
	 * Function runs the arm motor
	 */
	void runArm() {
		int tmp;
		int lowerLimit = -10000;
		if(ArmButtons.low ) {
				arm_currentPos = lowerLimit;
				grabberSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		} else {
				tmp = -ArmJoystick->GetY();
				if (tmp < -0.1) {
						arm_currentPos += ((tmp + 0.1) * (-1 / (-1 + 0.1))) * 5800;
				} else {
						arm_currentPos += 0;
				}
				//arm_currentPos += -ArmJoystick->GetY() * 5800;
				//ArmTalon->Set(ControlMode::PercentOutput, ArmJoystick->GetY());
		}

		if(!HallEffect->Get()) {
			ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			arm_currentPos = 0;

			//arm_currentPos =  ArmTalon->GetSelectedSensorPosition(0);
		} else if(arm_currentPos < lowerLimit) {
			arm_currentPos = lowerLimit;
		}


		if(ArmButtons.mid) {
				arm_currentPos = (ARM_UPPER_LIMIT/2);
		} else if(ArmButtons.high){
				arm_currentPos = ARM_UPPER_LIMIT;
		} else if(ArmButtons.lowmid) {
				arm_currentPos = 40000;
		} else {
				tmp = -ArmJoystick->GetY();
				if (tmp > 0.1) {
						arm_currentPos += ((tmp - 0.1) * (1 / (1 - 0.1))) * 5800 ;
				} else {
						arm_currentPos += 0;
				}
				//arm_currentPos += -ArmJoystick->GetY() * 5800;
				//ArmTalon->Set(ControlMode::PercentOutput, ArmJoystick->GetY());
		}

		if(arm_currentPos > ARM_UPPER_LIMIT) {
			arm_currentPos = ARM_UPPER_LIMIT;
		}
		/*
		int tmp;
		if(!zeroingOperation) {
			if(!HallEffect->Get()) {
				ArmTalon->SetSelectedSensorPosition(-100, kPIDLoopIdx, kTimeoutMs);
			}
	/ *
			if(armZeroDown && arm_currentPos == 0 ) {
				ArmTalon->SetSelectedSensorPosition(+100, kPIDLoopIdx, kTimeoutMs);
			} else if(armZeroUp && arm_currentPos == 0) {
				ArmTalon->SetSelectedSensorPosition(-100, kPIDLoopIdx, kTimeoutMs);
			}* /
			if(ArmButtons.low ) {
				arm_currentPos = 0;
				grabberSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
			} else if(ArmButtons.mid) {
				arm_currentPos = (550000/2);
			} else if(ArmButtons.high){
				arm_currentPos = 550000;
			} else {
				tmp = -ArmJoystick->GetY();
				if (tmp > 0.1) {
					arm_currentPos += ((tmp - 0.1) * (1 / (1 - 0.1))) * 5800 ;
				} else if (tmp < -0.1) {
					arm_currentPos += ((tmp + 0.1) * (-1 / (-1 + 0.1))) * 5800;
				} else {
					arm_currentPos += 0;
				}
				//arm_currentPos += -ArmJoystick->GetY() * 5800;
				//ArmTalon->Set(ControlMode::PercentOutput, ArmJoystick->GetY());
			}
			if(arm_currentPos < 1 ) {
				arm_currentPos = 0;
			} if(arm_currentPos > 550000) {
				arm_currentPos = 550000;
			}
		}

		if(armZeroDown ) {
			arm_currentPos -= 500;
			zeroEdgeDetect = true;
			zeroingOperation = true;
		} else if(armZeroUp) {
			arm_currentPos += 500;
			zeroEdgeDetect = true;
			zeroingOperation = true;
		}

		if(!armZeroDown && !armZeroUp && zeroEdgeDetect) {
			ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			arm_currentPos = 0;
			zeroingOperation = false;
			zeroEdgeDetect = false;
		}*/
	/*	int tmp = ArmJoystick->GetY();
		int turn;
		if (tmp > 0.1) {
			turn = ((tmp - 0.1) * (1 / (1 - 0.1)));
		} else if (tmp < -0.1) {
			turn = ((tmp + 0.1) * (-1 / (-1 + 0.1)));
		} else {
			turn = 0;
		}*/
		ArmTalon->Set(ControlMode::Position, arm_currentPos);
		SmartDashboard::PutNumber("Arm Position", ArmTalon->GetSelectedSensorPosition(0));
		SmartDashboard::PutNumber("Arm Target Position", arm_currentPos);

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
			/*
			 * This resets all the variables for autonomous so that it can be run more than once
			 */
			autonomousVars.CompletingOperation = true;
			autonomousVars.DriveOperationDone = false;
			autonomousVars.ArmOperationDone = false;
			autonomousVars.GrabberOperationDone = false;
			autonomousVars.IntakeOperationDone = false;
			autonomousVars.TimeOperationDone = false;
			autonomousVars.foundList = false;
			autonomousVars.timeCount = 0;
			autonomousVars.grabberTimeCount = 0;
			autonomousVars.intakeTimeCount = 0;
			autonomousVars.startingConfigDone = false;
			autonomousVars.TurningOperationDone = false;

			//ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			m_rearLeft.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			m_rearRight.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			navxgyro->ZeroYaw();

			m_left.SetInverted(false);

			ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			ArmTalon->Set(ControlMode::Position, 0);

			SmartDashboard::PutNumber("arm waiting", 0);

			SmartDashboard::PutBoolean("graph_reset", true);
			/**
			 * Here, get the data from the DriverStation regarding the switches
			 * and scale on the field. Three characters will be sent over and
			 * stored into gameData. The first denotes the switch closest to the
			 * robot, the second denotes the scale, and the third denotes the switch
			 * furthest from the robot. The three characters each will be 'L' or 'R'
			 * and denote the side of the field that the alliance color is on, relative
			 * to the robot's alliance.
			 */
			string gameData = DriverStation::GetInstance().GetGameSpecificMessage();
			if(gameData.length() >= 3) {
				switch(gameData[0]) {
				case 'L':
				case 'l':
					fieldColorLocations.setNearestSwitchLeft();
					break;
				case 'R':
				case 'r':
					fieldColorLocations.setNearestSwitchRight();
				}

				switch(gameData[1]) {
				case 'L':
				case 'l':
					fieldColorLocations.setScaleLeft();
					break;
				case 'R':
				case 'r':
					fieldColorLocations.setScaleRight();
				}

				switch(gameData[2]) {
				case 'L':
				case 'l':
					fieldColorLocations.setFurthestSwitchLeft();
					break;
				case 'R':
				case 'r':
					fieldColorLocations.setFurthestSwitchRight();
				}
			}

			// enable the drive PID controller
			SmartDashboard::PutBoolean("graph_reset", true);
			DrivePIDController->Enable();

	}

	/*********************************************************************************
	 * This is the main autonomous function. It selects the proper auto mode to run
	 * from the config file stored on the robotRIO. The method for mode selection
	 * is determined on the custom tab of the driver station. The modes are as follows:
	 * 		00 - Prelim_Automatic_Right
	 * 		01 - Prelim_Automatic_Left
	 * 		02 - Finals_Automatic_Right
	 * 		03 - Finals_Automatic_Left
	 * 		04 - DriveForward
	 * 		05 - Left_Switch_Scale
	 * 		06 - Left_Switch
	 * 		07 - Left_Scale
	 * 		08 - Left_Scale_FINAL
	 * 		09 - Right_Switch_Scale
	 * 		10 - Right_Switch
	 * 		11 - Right_Scale
	 * 		12 - Right_Scale_FINAL
	 *
	 */
	void AutonomousPeriodic() {





		string nearestSwitch = fieldColorLocations.isNearestSwitchOnLeft() ? "LEFT" : "RIGHT";
		string scale = fieldColorLocations.isScaleOnLeft() ? "LEFT" : "RIGHT";
		string furthestSwitch = fieldColorLocations.isFurthestSwitchOnLeft() ? "LEFT" : "RIGHT";
		SmartDashboard::PutString("COLORS", nearestSwitch + " " + scale + " " + furthestSwitch);

		/*
		if(!fieldColorLocations.isNearestSwitchOnLeft()) {
			/ *
			 * TODO:
			 * 1. Drive next to switch.
			 * 2. Turn toward switch.
			 * 3. Deploy intake dump box.
			 * 4. Turn toward scale.
			 * 5. Drive over the line.
			 * /
		} else if(!fieldColorLocations.isScaleOnLeft()) {
			/ *
			 * TODO:
			 * 1. Drive to scale.
			 * 2. Deploy intake and dump box.
			 * /
		} else {
			/*
			 * TODO:
			 * 1. Drive past switch.
			 * 2. Turn to the left 90 degrees.
			 * 3. Drive to switch on left side.
			 * 4. Turn to the left 90 degrees.
			 * 5. Deploy intake and dump box.
			 * /
		} */

		/*
		 * Recommendation: utilize JSON for the configuration file. JSON has several libraries available,
		 * as it is a standard. An example might be as such.
		 *
		 * {
		 *   "nearSwitch": [
		 *     "FORWARD 100",
		 *     "LEFT 90",
		 *     "DEPLOY",
		 *     "RIGHT 90",
		 *     "DRIVE 50"
		 *   ],
		 *   "scale": [
		 *     "FORWARD 200",
		 *     "DEPLOY"
		 *   ],
		 *   "farSwitch": [
		 *     "FORWARD 120",
		 *     "LEFT 90",
		 *     "DRIVE 200",
		 *     "LEFT 90",
		 *     "DEPLOY"
		 *   ]
		 * }
		 */

		if(!autonomousVars.startingConfigDone) {

			drive(0,0);
			autonomousVars.startingConfigDone = true;
		} else if(!autonomousVars.foundList) {
			//SmartDashboard::PutString("CURRENTAUTOSTATE", "started");
			int AutoMode = SmartDashboard::GetNumber("Autonomous Mode", 404); // may need to be changed from 0
			SmartDashboard::PutNumber("FOUND", AutoMode);

			if(AutoConfig.FileNotFound) { // if the auto config wasn't loaded
				drive(0,0); // update drive so it doesn't error
				return;
			}
			SmartDashboard::PutString("AUTOMODELOAD", "GOT TO SWITCH");
			switch(AutoMode){
				case(0): // Prelim Automatic Right
						// PUT SELECTOR HERE
						CurrentAutoMode = &AutoConfig.autoMode1; // put the stuff from that list into the current list
						autonomousVars.foundList = true;
						break;
				case(1): // Prelim Automatic Left
						// PUT SELECTOR HERE
						CurrentAutoMode = &AutoConfig.autoMode1;
						autonomousVars.foundList = true;
						break;
				case(2): // Finals Automatic Right
						// PUT SELECTOR HERE
						CurrentAutoMode = &AutoConfig.autoMode1;
						autonomousVars.foundList = true;
						break;
				case(3): // Finals Automatic Left
						// PUT SELECTOR HERE
						CurrentAutoMode = &AutoConfig.autoMode1; // put the stuff from that list into the current list
						autonomousVars.foundList = true;
						break;
				case(4): // Drive Forward
						CurrentAutoMode = &AutoConfig.autoMode1;
						autonomousVars.foundList = true;
						break;
				case(5): // Left Switch Scale
						CurrentAutoMode = &AutoConfig.autoMode2;
						autonomousVars.foundList = true;
						break;
				case(6): // Left Switch
						CurrentAutoMode = &AutoConfig.autoMode3;
						autonomousVars.foundList = true;
						break;
				case(7): // Left Scale
						CurrentAutoMode = &AutoConfig.autoMode4;
						autonomousVars.foundList = true;
						break;
				case(8): // Left Scale Final
						CurrentAutoMode = &AutoConfig.autoMode5; // put the stuff from that list into the current list
						autonomousVars.foundList = true;
						break;
				case(9): // Right Switch Scale
						CurrentAutoMode = &AutoConfig.autoMode6;
						autonomousVars.foundList = true;
						break;
				case(10): // Right Switch
						CurrentAutoMode = &AutoConfig.autoMode7;
						autonomousVars.foundList = true;
						break;
				case(11): // Right Scale
						CurrentAutoMode = &AutoConfig.autoMode8;
						autonomousVars.foundList = true;
						break;
				case(12): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode9;
						autonomousVars.foundList = true;
						break;
				case(13): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode10;
						autonomousVars.foundList = true;
						break;
				case(14): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode11;
						autonomousVars.foundList = true;
						break;
				case(15): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode12;
						autonomousVars.foundList = true;
						break;
				case(16): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode13;
						autonomousVars.foundList = true;
						break;
				case(17): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode14;
						autonomousVars.foundList = true;
						break;
				case(18): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode15;
						autonomousVars.foundList = true;
						break;
				case(19): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode16;
						autonomousVars.foundList = true;
						break;
				case(20): // Right Scale Final
						CurrentAutoMode = &AutoConfig.autoMode17;
						autonomousVars.foundList = true;
						break;
				default:
					break;

			}
			autonomousVars.autoTemp = CurrentAutoMode->head;
			// zero out sensors
			//ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			m_rearLeft.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			m_rearRight.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
			navxgyro->ZeroYaw();
			SmartDashboard::PutNumber("YEET2", 0);
			drive(0,0);


			DrivePIDController->SetP((double) SmartDashboard::GetNumber("drive_P", 0));
					DrivePIDController->SetI((double) SmartDashboard::GetNumber("drive_I", 0));
					DrivePIDController->SetD((double) SmartDashboard::GetNumber("drive_D", 0));

					SmartDashboard::PutNumber("actual_P", DrivePIDController->GetP());
					SmartDashboard::PutNumber("actual_I", DrivePIDController->GetI());
					SmartDashboard::PutNumber("actual_D", DrivePIDController->GetD());
					DrivePIDController->Reset();
					DrivePIDController->Enable();
		} else {
			if(!autonomousVars.CompletingOperation) { // if not currently in a operation
				// grab next operation
				if(autonomousVars.autoTemp != NULL) {
					// grab next operation
					autonomousVars.autoTemp = autonomousVars.autoTemp->next; // grab next operation
					if(autonomousVars.autoTemp!=NULL) {
						autonomousVars.CompletingOperation = true;
						SmartDashboard::PutString("CURRENTAUTOSTATE", "grabbing next value");
						navxgyro->ZeroYaw();
					} else {
						autonomousVars.CompletingOperation = false;
						SmartDashboard::PutString("CURRENTAUTOSTATE", "FOUND NULL");
					}
				}
				else {
					SmartDashboard::PutString("CURRENTAUTOSTATE", "FOUND NULL");
					autonomousVars.CompletingOperation = false;
					drive(0,0);
				}
			} else {
				/**********************************************************************************
				 * Run arm operations
				 * /

				if(!autonomousVars.ArmOperationDone) {
					if(autonomousVars.autoTemp->data[0] > 0) { // initial state of doing nothing.
						if(autonomousVars.autoTemp->data[0] == 0) {
							// run tell you find the hall effect
							ArmTalon->Set(ControlMode::Position, -ARM_UPPER_LIMIT);

							if(!HallEffect->Get()) {
								ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
								ArmTalon->Set(ControlMode::Position, 0);
								autonomousVars.ArmOperationDone = true;
								SmartDashboard::PutNumber("arm waiting", 1);
								// fix the position
							}
						} else if(ArmTalon->GetSelectedSensorPosition(0) < (autonomousVars.autoTemp->data[0] - ARM_AUTO_ERROR) || ArmTalon->GetSelectedSensorPosition(0) > (autonomousVars.autoTemp->data[0] + ARM_AUTO_ERROR) ){
							ArmTalon->ConfigPeakOutputForward((double) autonomousVars.autoTemp->data[1], kTimeoutMs);
							ArmTalon->ConfigPeakOutputReverse((double)-autonomousVars.autoTemp->data[1], kTimeoutMs);
							SmartDashboard::PutNumber("MOTOR POWER", autonomousVars.autoTemp->data[1]);
							if(autonomousVars.autoTemp->data[0] > ARM_UPPER_LIMIT) {
								ArmTalon->Set(ControlMode::Position, ARM_UPPER_LIMIT);
							} else {
								ArmTalon->Set(ControlMode::Position, autonomousVars.autoTemp->data[0]);
							}
						} else {
							autonomousVars.ArmOperationDone = true;
							SmartDashboard::PutNumber("arm waiting", 1);
							// fix the position
						}
					} else {
						autonomousVars.ArmOperationDone = true;
						SmartDashboard::PutNumber("arm waiting", 1);
					}
				}
				SmartDashboard::PutNumber("Arm Motor Target Position", autonomousVars.autoTemp->data[0]);
				SmartDashboard::PutNumber("Arm Motor actual Position", ArmTalon->GetSelectedSensorPosition(0));
				*/
				autonomousVars.ArmOperationDone = true;



				/**********************************************************************************
				 * run time operations
				 */
				if(autonomousVars.autoTemp->data[12] != autonomousVars.timeCount) {
					autonomousVars.timeCount++;
					SmartDashboard::PutNumber("Time Ticks", autonomousVars.timeCount);
					SmartDashboard::PutNumber("Target Time", autonomousVars.autoTemp->data[12]);
				} else {
					autonomousVars.TimeOperationDone = true;
				}



				/**********************************************************************************
				 * run grabber operations
				 * /
				if(autonomousVars.autoTemp->data[6]) {
					grabberSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
				} else {
					grabberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
				}
				if(autonomousVars.autoTemp->data[8] != autonomousVars.grabberTimeCount) {
					autonomousVars.grabberTimeCount++;
					GrabberTalon->Set(ControlMode::PercentOutput, autonomousVars.autoTemp->data[7]);
				} else {
					GrabberTalon->Set(ControlMode::PercentOutput, 0);
					autonomousVars.GrabberOperationDone = true;
				} */

				autonomousVars.GrabberOperationDone = true;


				/**********************************************************************************
				 * run intake operations
				 * /
				if(autonomousVars.autoTemp->data[9]) { // if its a one open the intake
					intakeSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
				} else {
					intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
				}
				if(autonomousVars.autoTemp->data[11] != autonomousVars.intakeTimeCount) {
					autonomousVars.intakeTimeCount ++;
					IntakeTalonLeft->Set(autonomousVars.autoTemp->data[10]);
					IntakeTalonRight->Set(-autonomousVars.autoTemp->data[10]);
					// add intake movement here

				} else {
					IntakeTalonLeft->Set(0);
					IntakeTalonRight->Set(0);
					autonomousVars.IntakeOperationDone = true;
				} */
				autonomousVars.IntakeOperationDone = true;


				/**********************************************************************************
				 * run drive train operations
				 */
				if(!autonomousVars.DriveOperationDone) {
					DrivePIDController->SetSetpoint((TicksPerInch * (autonomousVars.autoTemp->data[2])-2));
					SmartDashboard::PutNumber("Drive Target Position", autonomousVars.autoTemp->data[2]);


				}
				/*//if(!autonomousVars.DriveOperationDone) {
					if(autonomousVars.autoTemp->data[2] < 1) {
						if(((int) ((m_rearLeft.GetSelectedSensorPosition(0) + m_rearRight.GetSelectedSensorPosition(0))/2)) > (autonomousVars.autoTemp->data[2] + DRIVE_AUTO_ERROR)) { // backward
							m_drive.CurvatureDrive(-autonomousVars.autoTemp->data[3], -navxgyro->GetAngle() * 0.014,false);

						} else {
							SmartDashboard::PutNumber("DRIVE MOTOR POS", autonomousVars.autoTemp->data[2]);
							autonomousVars.DriveOperationDone = true;
							navxgyro->ZeroYaw();
							SmartDashboard::PutNumber("yeet", 0);
							SmartDashboard::PutNumber("yeet", 1);
						}
					} else if(autonomousVars.autoTemp->data[2] > 1) {
						if(((int) ((m_rearLeft.GetSelectedSensorPosition(0) + m_rearRight.GetSelectedSensorPosition(0))/2)) < (autonomousVars.autoTemp->data[2] - DRIVE_AUTO_ERROR)) { // forward
							m_drive.CurvatureDrive(autonomousVars.autoTemp->data[3],-navxgyro->GetAngle() * 0.007,false);

						} else {
							SmartDashboard::PutNumber("DRIVE MOTOR POS", autonomousVars.autoTemp->data[2]);
							autonomousVars.DriveOperationDone = true;
							navxgyro->ZeroYaw();
							SmartDashboard::PutNumber("yeet", 0);
							SmartDashboard::PutNumber("yeet", 1);
						}
					}
					else {
						autonomousVars.DriveOperationDone = true;
					}
				//}*/
				SmartDashboard::PutNumber("Drive left Position", m_rearLeft.GetSelectedSensorPosition(0));
				SmartDashboard::PutNumber("Drive Right Position", m_rearRight.GetSelectedSensorPosition(0));
				// put data on the graph
				//SmartDashboard::PutNumber("graph_actual", (m_rearRight.GetSelectedSensorPosition(0)+m_rearLeft.GetSelectedSensorPosition(0))/2);
				//SmartDashboard::PutNumber("graph_target", autonomousVars.autoTemp->data[2]);
				//SmartDashboard::PutBoolean("graph_in", true);

				autonomousVars.TurningOperationDone = true;
				// turning operation
				/*if(autonomousVars.DriveOperationDone) {
					if(!autonomousVars.TurningOperationDone) {
						SmartDashboard::PutNumber("YEET2", 3);
	//					if(autonomousVars.autoTemp->data[4] < 0) {
								if( navxgyro->GetAngle() > (autonomousVars.autoTemp->data[4] + 2) || navxgyro->GetAngle() < (autonomousVars.autoTemp->data[4] - 2) ) { // backward
										if(navxgyro->GetAngle() < autonomousVars.autoTemp->data[4]) {
											m_drive.ArcadeDrive(0,autonomousVars.autoTemp->data[5]);
										} else if(navxgyro->GetAngle() > autonomousVars.autoTemp->data[4]) {
											m_drive.ArcadeDrive(0,-autonomousVars.autoTemp->data[5]);
										}

									    SmartDashboard::PutNumber("YEET2", 1);
								} else {
										m_drive.ArcadeDrive(0,0);
										navxgyro->ZeroYaw();
										autonomousVars.TurningOperationDone = true;
								}
						//} else if(autonomousVars.autoTemp->data[4] >= 0) {
								/*if(navxgyro->GetAngle() > (autonomousVars.autoTemp->data[4] + 5)) { // forward
										m_drive.ArcadeDrive(-1,0);
										SmartDashboard::PutNumber("YEET2", 1);
								} else {
										m_drive.ArcadeDrive(0,0);
										autonomousVars.TurningOperationDone = true;
								} /
//						}

					}
				}*/
				SmartDashboard::PutNumber("GYRO ANGLE", navxgyro->GetAngle());
				SmartDashboard::PutNumber("GYRO TARGET ANGLE", autonomousVars.autoTemp->data[4] );
				SmartDashboard::PutNumber("GYRO SPEED" , autonomousVars.autoTemp->data[5]);
				/*
				if(m_rearLeft.GetSelectedSensorPosition(0) < (autonomousVars.autoTemp->data[2] - DRIVE_AUTO_ERROR) || m_rearLeft.GetSelectedSensorPosition(0) > (autonomousVars.autoTemp->data[2] + DRIVE_AUTO_ERROR) ){
					if(autonomousVars.autoTemp->data[2]  < 1) {
						m_drive.CurvatureDrive(autonomousVars.autoTemp->data[3], navxgyro->GetAngle() * 0.014,false);
						//(0,autonomousVars.autoTemp->data[3]); // backward
					} else {
						m_drive.CurvatureDrive(autonomousVars.autoTemp->data[3], navxgyro->GetAngle() * 0.007,false);
					}

					SmartDashboard::PutNumber("DRIVE MOTOR POWER", autonomousVars.autoTemp->data[3]);
					SmartDashboard::PutNumber("Drive Position", m_rearLeft.GetSelectedSensorPosition(0));
				} else {
					SmartDashboard::PutNumber("DRIVE MOTOR POS", autonomousVars.autoTemp->data[2]);
					autonomousVars.DriveOperationDone = true;
				}
*/
				//drive(0,0);

				/**********************************************************************************
				 * Put all the modes current completion state on the driver station and test to see
				 * if we can move onto the next operation.
				 */
				SmartDashboard::PutNumber("TurningOperationDone", autonomousVars.TurningOperationDone);
				SmartDashboard::PutNumber("DriveOperationDone", autonomousVars.DriveOperationDone);
				SmartDashboard::PutNumber("ArmOperationDone", autonomousVars.ArmOperationDone);
				SmartDashboard::PutNumber("TimeOperationDone", autonomousVars.TimeOperationDone);
				SmartDashboard::PutNumber("IntakeOperationDone", autonomousVars.IntakeOperationDone );
				SmartDashboard::PutNumber("GrabberOperationDone", autonomousVars.GrabberOperationDone);
				if(autonomousVars.TurningOperationDone && autonomousVars.DriveOperationDone && autonomousVars.ArmOperationDone && autonomousVars.TimeOperationDone && autonomousVars.IntakeOperationDone && autonomousVars.GrabberOperationDone) {
					SmartDashboard::PutString("CURRENTAUTOSTATE", "Finished arm");

					autonomousVars.CompletingOperation = false; //reset operations and variables so they don't carry over to the next state
					autonomousVars.ArmOperationDone = false;
					autonomousVars.TimeOperationDone = false;
					autonomousVars.GrabberOperationDone = false;
					autonomousVars.IntakeOperationDone = false;
					autonomousVars.DriveOperationDone = false;
					autonomousVars.TurningOperationDone = false;

					autonomousVars.timeCount = 0;
					autonomousVars.grabberTimeCount = 0;
					autonomousVars.intakeTimeCount = 0;
					SmartDashboard::PutNumber("arm waiting", 0);
					m_rearLeft.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs); // this makes the drive train position relative
					m_rearRight.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
					navxgyro->ZeroYaw();

					SmartDashboard::PutNumber("yeet", 0);
					SmartDashboard::PutNumber("yeet", 1);
				}

			}
		}
		if(autonomousVars.DriveOperationDone && autonomousVars.TurningOperationDone) {
			drive(0,0);
		}





	}

	double PIDGet(){
		return ((m_rearLeft.GetSelectedSensorPosition(0) + m_rearRight.GetSelectedSensorPosition(0))/2);
	}

	void PIDWrite(double PIDPower){
		drive(PIDPower * SmartDashboard::GetNumber("drive_Undef", 0), 0);
		SmartDashboard::PutNumber("graph_actual", ((m_rearLeft.GetSelectedSensorPosition(0) + m_rearRight.GetSelectedSensorPosition(0))/2));
		SmartDashboard::PutNumber("graph_target", autonomousVars.autoTemp->data[2]);
		SmartDashboard::PutBoolean("graph_in", true);
	}

	/***********************************************************************
	 * This is the first thing that is ran when teleop starts. Make sure you
	 * have all the things from auto set back to normal in this function.
	 * Other wise you might get odd results
	 *
	 */
	void TeleopInit() {
		// RESET TALON SPEED HERE
		//ArmTalon->ConfigPeakOutputForward((double) ARM_POWER, kTimeoutMs);
		//ArmTalon->ConfigPeakOutputReverse((double)-ARM_POWER, kTimeoutMs);

		m_left.SetInverted(true);
		/*
		 * TODO:
		 * add a thing that doesn't change the zero if auto ran first
		 */
		ArmTalon->SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
		ArmTalon->Set(ControlMode::Position, 0);
		arm_currentPos = 0;
		intnumb++;
		SmartDashboard::PutNumber("intnumb", intnumb);
		zeroingOperation = false;
		zeroEdgeDetect = false;

		m_rearRight.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);
		m_rearLeft.SetSelectedSensorPosition(0, kPIDLoopIdx, kTimeoutMs);

		DrivePIDController->Disable(); // turn off the drive PID controller
	}

	/***********************************************************************
	 * This function is ran every 20ms in Teleop mode. It should call all the
	 * functions that run the different aspects of the robot.
	 *
	 */
	void TeleopPeriodic() {
		pollControllers(); //
		pollSensors();
		//runIntake();
	    //runGrabber();
		//runArm();
		drive(joystickX,joystickY);

		if(GyroFound) {
			SmartDashboard::PutNumber("GryoAngle", navxgyro->GetAngle());
		}
		SmartDashboard::PutNumber("Talon Sensor Velocity", m_rearLeft.GetSelectedSensorVelocity(0));
		SmartDashboard::PutNumber("Talon Sensor Position", m_rearLeft.GetSelectedSensorPosition(0));
		SmartDashboard::PutNumber("Talon Right Sensor Velocity", m_rearRight.GetSelectedSensorVelocity(0));
		SmartDashboard::PutNumber("Talon Right Sensor Position", m_rearRight.GetSelectedSensorPosition(0));
		SmartDashboard::PutNumber("Drive Target X", joystickX);


		//SmartDashboard::PutNumber("Hall Effect Value" , HallEffect->Get());
	}

	/***********************************************************************
	 * This function can be used for testing things outside of messing with
	 * the teleop or auto modes.
	 *
	 */

	void TestInit() {
		SmartDashboard::PutBoolean("graph_reset", true);

		DrivePIDController->Enable();


	}

	void TestPeriodic() {

		DrivePIDController->SetSetpoint(10000);
		drive(0,0);

	}



	/***********************************************************************
	 * This has all of the main program variables in it. Almost all of them
	 * are just declared here, not initialized. For initialization go to the
	 * RobotInit Function.
	 */
private:
	frc::LiveWindow& m_lw = *LiveWindow::GetInstance();
	frc::SendableChooser<std::string> m_chooser;
	const std::string kAutoNameDefault = "Default";
	const std::string kAutoNameCustom = "My Auto";
	std::string m_autoSelected;

	int intnumb = 0;

	/*
	 * Main program variables. Levels and stuff
	 */
	double driveSlow = 0.3, driveNormal = 0.6, driveFull = 1;
	double driveLevel = driveNormal;
	PIDController *DrivePIDController;

	double PIDEncoderCount = 0;


	double TicksPerInch =0;



	/*
	 * Controller setup. Joy-sticks, buttons etc.
	 */
	double joystickX = 0;
	double joystickY = 0;
	Joystick *MainJoystick;
	Joystick *ArmJoystick;
	Joystick *GamepadButtons;


	/*
	 * Sensor Setup
	 */
	class AHRS *navxgyro;
	float gyroAngle;
	DigitalInput *HallEffect;
	DigitalInput *IntakeSwitch;


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
	Talon *IntakeTalonLeft;
	Talon *IntakeTalonRight;
	TalonSRX *ArmTalon;
	TalonSRX *GrabberTalon;

	bool intakeForward = false;
	bool intakeBackward = false;
	bool intakeOpen = false;
	bool intakeClose = false;
	double intakeSpeed = 0;

	bool zeroingOperation = false;
	bool zeroEdgeDetect = false;

	bool intakeRotateRight = false;
	bool intakeRotateLeft = false;

	bool grabberForward = false;
	bool grabberBackward = false;
	bool grabberOpen = false;

	bool grabberPneumaticsForward = false;
	bool grabberPneumaticsBackward = false;

	bool armZeroDown = false;
	bool armZeroUp = false;
	frc::DoubleSolenoid grabberSolenoid {GRABBER_SOL_ONE,GRABBER_SOL_TWO};
	frc::DoubleSolenoid intakeSolenoid {INTAKE_SOL_ONE,INTAKE_SOL_TWO};
	//frc::DoubleSolenoid intakeSolenoid {2,3};

	struct structArmButtons {
		bool low = false;
		bool lowmid = false;
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
		bool TurningOperationDone = false;
		bool foundList = false;
		double timeCount = 0;
		double grabberTimeCount =0;
		double intakeTimeCount =0;
		node *autoTemp = new node;
		bool startingConfigDone = false;
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
