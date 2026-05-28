#pragma once

#include <Arduino.h>

/**
 * @brief Enumeration for UI states
 * UI_STATE_PATTERN_SELECT: User is selecting a pattern
 * UI_STATE_RUNNING: Pattern is running
 * UI_STATE_ERROR: An error has occurred
 */
enum ui_state_e
{
    UI_STATE_PATTERN_SELECT = 0,
    UI_STATE_RUNNING,
    UI_STATE_ERROR,
};

// Function declarations
/**
 * @brief Initializes the UI manager.
 * @param None
 * @return true if initialization is successful, false otherwise
 */
bool uiManagerInit(void);

/**
 * @brief Updates the UI manager state and handles user input.
 * This function:
 * - Reads keypad input
 * - Updates menu selection
 * - Changes UI state
 * - Refreshes LCD
 *
 * Should be called continuously in loop().
 * @param None
 * @return None
 */
void uiManagerUpdate(void);

/**
 * @brief Gets the currently selected pattern index.
 * @param None
 * @return The index of the selected pattern (0-based)
 */
uint8_t uiManagerGetSelectedPattern(void);