#pragma once

#include <Arduino.h>

#define PATTERN_SIZE 18

typedef struct 
{
    uint32_t rows[PATTERN_SIZE];
}pattern_t;

bool initPatternFS();

bool loadPattern(
    const char *filename,
    pattern_t &pattern
);

bool getCell(
    const pattern_t &pattern,
    uint8_t row,
    uint8_t col
);