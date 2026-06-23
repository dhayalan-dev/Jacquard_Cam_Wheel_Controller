#pragma once

#include <Arduino.h>
#include "machine_config.h"

enum reset_state_e
{
    RESET_CONTROLLER_IDLE = 0,
    RESET_CONTROLLER_RUNNING,
    RESET_CONTROLLER_COMPLETE,
    RESET_CONTROLLER_ABORTED
};

/**
 * @brief Initializes the reset controller.
 */
bool resetControllerInit();

/**
 * @brief Starts the reset controller.
 */
bool resetControllerStart();

/**
 * @brief Updates the reset controller.
 */
void resetControllerUpdate();

/**
 * @brief Aborts the reset controller.
 */
void resetControllerAbort();

/**
 * @brief Gets the current state of the reset controller.
 */
reset_state_e resetControllerGetState();

/**
 * @brief Gets the current step of the reset controller.
 */
uint16_t resetControllerGetStep();

/**
 * @brief Checks if the reset controller is running.
 */
bool resetControllerIsRunning();