/**
 * @file patterns.h
 * @brief This header file defines the data structures and function prototypes for handling cam 
 * wheel patterns in the Jacquard Cam Wheel project. It includes definitions for representing cam wheels and patterns.
 * @author Dhayalan
 */
#pragma once

#include <Arduino.h>

#define STEPS_PER_WHEEL 72
#define NO_OF_WHEELS 18
#define BYTES_PER_WHEEL ((STEPS_PER_WHEEL + 7) / 8)
#define REPEAT_COUNT 2

static_assert(
    STEPS_PER_WHEEL > 0,
    "Invalid step count"
);

static_assert(
    NO_OF_WHEELS > 0,
    "Invalid wheel count"
);

static_assert(
    BYTES_PER_WHEEL > 0,
    "Invalid wheel storage size"
);

/**
 * @brief Represents a single cam wheel, which consists of 144 pins, 
 * First part contains 72 pins and second part have the same design as of the first pins.
 */
struct wheel_t
{
    uint8_t data[BYTES_PER_WHEEL];
};

/**
 * @brief Represents a complete pattern, consisting of multiple cam wheels
 * and a global repeat count.
 */
struct pattern_t
{
    wheel_t wheels[NO_OF_WHEELS];
    uint8_t repeat; // Number of times the pattern on this wheel should be repeated
};

/**
 * @brief Initializes the SPIFFS file system to read pattern files. 
 * This function should be called before attempting to read any pattern files.
 * @param None
 * @return true if the SPIFFS file system was successfully initialized, false otherwise.
 */
bool initPatternFS();

/**
 * @brief Loads a pattern from a specified file in the SPIFFS file system. The pattern is read into the provided pattern_t structure.
 * @param filename The name of the file from which to load the pattern. This should be a valid file path in the SPIFFS file system.
 * @param pattern A reference to a pattern_t structure where the loaded pattern will be stored.
 * @return true if the pattern was successfully loaded, false otherwise.
 */
bool loadPatternFromFile(
    const char *filename,
    pattern_t &pattern
);

/**
 * @brief Retrieves the bit value for a specific step from a given pattern. This function is used to 
 * determine whether a particular step in the pattern is active (1) or inactive (0).
 * @param pattern The pattern from which to retrieve the bit value.
 * @param wheelIndex The index of the wheel for which to retrieve the bit value.
 * @param step The step number (0-71) for which to retrieve the bit value
 * @return true if the bit value for the specified step is 1 (active), false if it is 0 (inactive).
 */
bool getPatternBit(
    const pattern_t &pattern,
    uint8_t wheelIndex,
    uint8_t step
);

/**
 * @brief Prints the pattern to the serial console for debugging purposes.
 * @param patternName File name of the pattern being printed.
 * @param pattern The pattern to be printed.
 * @return None
 */
void printPattern(
    const char *patternName,
    const pattern_t &pattern
);