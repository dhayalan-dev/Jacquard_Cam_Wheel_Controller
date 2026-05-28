
#pragma once

#include <Arduino.h>

/*
 * ROW PINS 
 */

#define KEYPAD_ROW_1 32
#define KEYPAD_ROW_2 33
#define KEYPAD_ROW_3 25
#define KEYPAD_ROW_4 26

/*
 * COLUMN PINS 
 */
#define KEYPAD_COL_1 27
#define KEYPAD_COL_2 14
#define KEYPAD_COL_3 12
#define KEYPAD_COL_4 13

/*
 * KEYPAD ACTIONS
 */
typedef enum
{
    KEYPAD_ACTION_NONE = 0,
    KEYPAD_ACTION_UP,
    KEYPAD_ACTION_DOWN,
    KEYPAD_ACTION_LEFT,
    KEYPAD_ACTION_RIGHT,
    KEYPAD_ACTION_SELECT
} keypad_action_e;

/*----------------------------------------
 * FUNCTION DECLARATIONS
 *---------------------------------------
 */

/*
 * @brief Initialize the keypad pins
 * @return true if initialization is successful, false otherwise
 */
bool keypadInit();

/*
 * @brief Read the current keypad action
 * @return The detected keypad action
 */
keypad_action_e keypadRead();