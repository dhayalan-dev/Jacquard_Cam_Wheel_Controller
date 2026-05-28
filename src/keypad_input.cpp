#include "keypad_input.h"

// Key map
static const char keyMap[4][4] =
{
    { '1', '2', '3', 'A' },

    { '4', '5', '6', 'B' },

    { '7', '8', '9', 'C' },

    { '*', '0', '#', 'D' }
};

// Pin arrays
static const uint8_t rowPins[4] = 
{
    KEYPAD_ROW_1,
    KEYPAD_ROW_2,
    KEYPAD_ROW_3,
    KEYPAD_ROW_4
};

static const uint8_t colPins[4] = 
{
    KEYPAD_COL_1,
    KEYPAD_COL_2,
    KEYPAD_COL_3,
    KEYPAD_COL_4
};

bool keypadInit()
{
    // Configure rows a outputs
    for(uint8_t i = 0; i < 4; i++)
    {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH);
    }

    // Configure columns as input with pullups
    for(uint8_t i = 0; i < 4; i++)
    {
        pinMode(colPins[i], INPUT_PULLUP);
    }

    return true; // Initialization successful
}

keypad_action_e keypadRead()
{
    for(uint8_t row = 0; row < 4; row++)
    {
        // Set the current row LOW
        digitalWrite(rowPins[row], LOW);

        // Check each column for a LOW signal
        for(uint8_t col = 0; col < 4; col++)
        {
            if(digitalRead(colPins[col]) == LOW)
            {
                char key = keyMap[row][col];
                
                // Key is pressed, return corresponding action
                switch(key)
                {
                    case '2': return KEYPAD_ACTION_UP;
                    case '8': return KEYPAD_ACTION_DOWN;
                    case '4': return KEYPAD_ACTION_LEFT;
                    case '6': return KEYPAD_ACTION_RIGHT;
                    case '5': return KEYPAD_ACTION_SELECT;
                    default: return KEYPAD_ACTION_NONE; // Unmapped key
                }
            }
        }

        // Set the current row back HIGH
        digitalWrite(rowPins[row], HIGH);
    }

    return KEYPAD_ACTION_NONE;
}