#pragma once

#include <Arduino.h>

#define TOTAL_DESIGNS 2

typedef struct
{
    const char *name;

    const char *topPatternFile;
    const char *bottomPatternFile;

    uint8_t topRepeat;
    uint8_t bottomRepeat;
}design_t;

extern const design_t designs[TOTAL_DESIGNS];
