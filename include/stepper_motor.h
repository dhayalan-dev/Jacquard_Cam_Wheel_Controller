/** 
 * @file stepper_motor.h
 * @brief Stepper motor control for Jacquard Cam Wheel project
 * @author Dhayalan R
*/
#pragma once

#include "machine_config.h"

/**
 * @brief Initializes the stepper motor control pins and state
 * @return true if initialization was successful, false otherwise
 */
bool stepperMotorInit();

/**
 * @brief Moves the stepper motor by a specified number of steps
 * @param steps Number of steps to move
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorStep(uint16_t steps);

/**
 * @brief Moves the stepper motor by one cam position
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorMoveOneCamPosition();

/**
 * @brief Enables the stepper motor
 * @return True if operation successful, false otherwise
 */
bool stepperMotorEnable();

/**
 * @brief Stops the stepper motor
 * @return True if operation successful, false otherwise
 */
bool stepperMotorStop();