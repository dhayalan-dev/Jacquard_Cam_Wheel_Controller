#pragma once

#include <Arduino.h>

enum key_t
{
    KEY_NONE,
    KEY_UP,
    KEY_DOWN,
    KEY_SELECT,
    KEY_BACK
};

void keypadInit();

key_t keypadRead();