#pragma once

#include <Arduino.h>

/**
 * @brief Pattern registry entry.
 *
 * Associates a user-friendly pattern name
 * with the binary pattern file stored in SPIFFS.
 */
struct pattern_entry_t
{
    const char *name;
    const char *filename;
};

/**
 * @brief Returns total number of patterns.
 */
uint8_t patternRegistryGetCount();

/**
 * @brief Returns pattern information by index.
 *
 * @param index Pattern index.
 *
 * @return Pointer to pattern entry.
 *         nullptr if index is invalid.
 */
const pattern_entry_t* patternRegistryGet(uint8_t index);

/**
 * @brief Returns pattern name by index
 * @param index pattern index
 * @return return string pattern name
 */
const char* patternRegistryGetName(uint8_t index);

/**
 * @brief Returns pattern filename by index
 * @param index pattern index
 * @return return string pattern filename
 */
const char* patternRegistryGetFileName(uint8_t index);