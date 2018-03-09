#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

/**********************************
 * Joystick ports, make sure in the driver station that these numbers match
 */
#define MAIN_JOYSTICK 0
#define SECONDARY_JOYSTICK 1
#define GAMEPAD_PORT 2

/**********************************
 * This is the drive train setup and each constant is the motor port number. Currently all sparks but
 * may be changed to a combination of both talons and sparks later.
 */
#define DriveM_FrontLeft 3
#define DriveM_MiddleLeft 2
#define DriveM_RearLeft 2 // this is the talon srx

#define DriveM_FrontRight 0
#define DriveM_MiddleRight 1
#define DriveM_RearRight 1 // this is the talon srx

#define DRIVE_AUTO_ERROR 100

#define DRIVE_PIDC_Kp 0.0035    // for driving
#define DRIVE_PIDC_Ki 0
#define DRIVE_PIDC_Kd 0.002
#define DRIVE_PIDC_Kf 0

#define DRIVE_STRAIGHT_P 0.095 // for driving straight


#define DRIVE_PIDT_Kp 0.095   // for turning
#define DRIVE_PIDT_Ki 0
#define DRIVE_PIDT_Kd 0
#define DRIVE_PIDT_Kf 0


#define DRIVE_PIDC_PERIOD 0.02  // default maybe change to 20ms

#define DRIVE_WHEEL_DIA 7 // in inches

/**********************************
 * PID constants. These are for the arm PID and should be adjusted with care
 */
#define CONST_kP 1 // proportional gain
#define CONST_kI 0 // integral gain
#define CONST_kD 0 // derivative gain
#define CONST_kF 0 // feed - forward

/*********************************
 * Constants for the arm
 */
#define ARM_POWER 1         // this is the power the arm runs at.
#define ARM_CONTROLLER_PORT 3 // this is the CAN talon device ID

#define ARM_POS_1 1000
#define ARM_POS_2 2000
#define ARM_POS_3 4096

#define ARM_AUTO_ERROR 10000

#define ARM_UPPER_LIMIT 620000

/*********************************
 * Constants for the Grabber
 */
#define GRABBER_CONTROLLER_PORT 4 // this is the CAN talon device ID
#define GRABBER_POWER 1
#define GRABBER_SOL_ONE 6
#define GRABBER_SOL_TWO 7

/*********************************
 * Constants for the Intake
 */
#define INTAKE_SPEED 1
#define LEFT_INTAKE_PORT 9
#define RIGHT_INTAKE_PORT 8

#define INTAKE_SOL_ONE 4
#define INTAKE_SOL_TWO 5

// climber
#define CLIMBER_PORT 7


enum Constants {
	/**
	 * Which PID slot to pull gains from.  Starting 2018, you can choose
	 * from 0,1,2 or 3.  Only the first two (0,1) are visible in web-based configuration.
	 */
	kSlotIdx = 0,

	/* Talon SRX/ Victor SPX will supported multiple (cascaded) PID loops.
	 * For now we just want the primary one.
	 */
	kPIDLoopIdx = 0,

	/*
	 * set to zero to skip waiting for confirmation, set to nonzero to wait
	 * and report to DS if action fails.
	 */
	kTimeoutMs = 10
};

#endif
