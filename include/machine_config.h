#pragma once

#include <Arduino.h>

/**
 * @brief DRV8825 microstepping configuration
 * 
 * @details
 * The cam wheel contains 144 physical pins and with the specific
 * rotation requirement of 2.5 degrees per step, to achieve this we are
 * using 1/32 microstepping mode. 
 * 
 * One full revolution:
 * 360 / 144 = 2.5 degrees per step
 * 
 * The stepper motor uses:
 * - 1.8 degrees per full step
 * - 1/32 microstepping mode configured in DRV8825
 * 
 * Therefore:
 * 200 full steps/revolution * 32 microsteps = 6400 microsteps/revolution
 * 
 * To achieve 2.5 degrees per step:
 * 360 / 6400 = 0.05625 degrees per microstep
 * 
 * Required microstep per cam position:
 * 2.5 / 0.05625 = 44.444 microsteps per cam position
*/

/* ============================================================
 * MACHINE PARAMETERS
 * ============================================================
 */

#define CAM_POSITIONS 144

#define LOGICAL_PATTERN_STEPS 72

#define NO_OF_WHEELS 18

#define REPEAT_COUNT 2

/* ============================================================
 * MOTOR PARAMETERS
 * ============================================================
 */

#define MOTOR_FULL_STEPS 200

#define MICROSTEP_DIVIDER 32

#define MOTOR_MICROSTEPS \
    (MOTOR_FULL_STEPS * MICROSTEP_DIVIDER)

/* ============================================================
 * DETERMINISTIC STEP COMPENSATION
 * ============================================================
 *
 * Movement sequence:
 *
 * 44, 44, 45
 *
 * Average:
 *
 * (44 + 44 + 45) / 3
 * = 44.333 microsteps
 *
 */

#define BASE_MICROSTEPS_PER_CAM_POSITION 44 // 44 = 2.5 degrees

#define EXTRA_MICROSTEP_INTERVAL 3

/* ============================================================
 * DRIVER TIMING
 * ============================================================
 */

#define STEP_PULSE_HIGH_US 500
#define STEP_PULSE_LOW_US 500

#define SOLENOID_ACTUATION_DELAY_MS 30 

/* PIN DEFINITIONS 
 * Pattern Setter */
#define STEP_PIN_1    26
#define EN_PIN_1      27

#define SOLENOID_PIN_1 14

/* PIN DEFINITIONS 
 * Pattern Resetter */
#define STEP_PIN_2    21
#define EN_PIN_2      19

#define SOLENOID_PIN_2 18