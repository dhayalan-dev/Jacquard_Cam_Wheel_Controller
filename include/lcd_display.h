#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

/**
 * Pin definitions for the LCD display.
 */
#define LCD_RS_PIN 22
#define LCD_ENABLE_PIN 21
#define LCD_D4_PIN 19
#define LCD_D5_PIN 15
#define LCD_D6_PIN 25
#define LCD_D7_PIN 5

/**
 * LCD display configuration parameters
 */
#define LCD_COLS 8
#define LCD_ROWS 2

/**
 * Function declarations for LCD display operations
 */

/**
 * @brief Initializes the LCD display.
 * @param None
 * @return true if initialization is successful, false otherwise
 */
bool lcdDisplayInit(void);

// Clears the LCD display
void lcdDisplayClear(void);

/**
 * @brief Prints text to the LCD display at the specified column and row.
 * @param col The column number (0-based index)
 * @param row The row number (0-based index)
 * @param txt The text to be displayed
 * @return true if printing is successful, false otherwise
 */
bool lcdDisplayPrint(
    uint8_t col,
    uint8_t row,
    const char* txt
);