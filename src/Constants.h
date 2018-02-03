#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

/**********************************
 * Joystick ports, make sure in the driver station that these numbers match
 */
#define MAIN_JOYSTICK 0
#define SECONDARY_JOYSTICK 1

/**********************************
 * This is the drive train setup and each constant is the motor port number. Currently all sparks but
 * may be changed to a combination of both talons and sparks later.
 */
#define DriveM_FrontLeft 1
#define DriveM_MiddleLeft 2
#define DriveM_RearLeft 3

#define DriveM_FrontRight 4
#define DriveM_MiddleRight 5
#define DriveM_RearRight 6

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
#define ARM_POWER 0.5         // this is the power the arm runs at.
#define ARM_CONTROLLER_PORT 1 // this is the CAN talon device ID

/*********************************
 * Constants for the Grabber
 */
#define GRABBER_CONTROLLER_PORT 2 // this is the CAN talon device ID



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
