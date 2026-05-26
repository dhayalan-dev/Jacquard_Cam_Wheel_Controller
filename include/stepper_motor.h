/** 
 * @file stepper_motor.h
 * @brief Stepper motor control for Jacquard Cam Wheel project
 * @author Dhayalan R
*/
#pragma once

#include "machine_config.h"
#define STEP_PIN 18
#define SLEEP_PIN 15
#define RESET_PIN 14

static_assert(
    STEP_PIN == 18, "STEP_PIN must be defined as 18"
);
static_assert(
    SLEEP_PIN == 15, "SLEEP_PIN must be defined as 15"
);
static_assert(
    RESET_PIN == 14, "RESET_PIN must be defined as 14"
);

/**
 * @brief Initializes the stepper motor control pins and state
 * @return true if initialization was successful, false otherwise
 */
bool stepperMotorInit();

/**
 * @brief Sets the stepper motor state to sleep or awake
 * @param sleep true to put the motor to sleep, false to awake
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorSleep(bool sleep);

/**
 * @brief Moves the stepper motor by a specified number of steps
 * @param steps Number of steps to move
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorStep(uint32_t steps);

/**
 * @brief Moves the stepper motor by one cam position
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorMoveOneCamPosition();

/**
 * @brief Resets the stepper motor state
 * @return true if the operation was successful, false otherwise
 */
bool stepperMotorReset();