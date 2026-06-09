/** 
 * @file stepper_motor.h
 * @brief Stepper motor control for Jacquard Cam Wheel project
 * @author Dhayalan R
*/
#pragma once

#include "machine_config.h"

#define DIR_PIN     25
#define STEP_PIN    26
#define RST_PIN     14
#define EN_PIN      13

static_assert(
    STEP_PIN == 26, "STEP_PIN must be defined as 26"
);

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
