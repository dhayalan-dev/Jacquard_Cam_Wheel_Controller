#pragma once

#include <Arduino.h>
#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"

enum machine_state_e
{
    MACHINE_STATE_IDLE = 0,
    MACHINE_STATE_RUNNING,
    MACHINE_STATE_READY,
    MACHINE_STATE_WAIT_FOR_NEXT_WHEEL,
    MACHINE_STATE_COMPLETED,
    MACHINE_STATE_ERROR,
    MACHINE_STATE_ABORTED
};


/**
 * @brief Initializes machine controller
 */
bool machineControllerInit();

/**
 * @brief Loads the selected pattern into memory
 */
bool machineControllerLoadPattern(const char *fileName);

/**
 * @brief Start loaded pattern execution
 */
bool machineControllerStart();

/**
 * @brief Stop current execution.
 */
bool machineControllerStop();

/**
 * @brief Abort the current execution
 */
void machineControllerAbort();

/**
 * @brief Machine state handler.
 */
void machineControllerUpdate();

/**
 * @brief Get current machine state.
 */
machine_state_e machineControllerGetState();

/**
 * @brief Get current wheel index.
 */
uint8_t machineControllerGetWheelIndex();

/**
 * @brief Get current step index.
 */
uint8_t machineControllerGetStepIndex();

/**
 * @brief Returns the current progress on the pattern
 */
uint8_t machineControllerGetProgress();

/**
 * @brief Returns the current pattern file
 */
const char* machineControllerGetCurrentPatternFile(void);