#pragma once

#include <Arduino.h>

void lcdInit();

void lcdClear();

void lcdPrint(
    uint8_t col,
    uint8_t row,
    const char *text
);

void lcdShowMenu(
    const char *item1,
    const char *item2,
    uint8_t selectedRow
);