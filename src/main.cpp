#include <Arduino.h>

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"
#include "keypad_input.h"

pattern_t pattern;
uint8_t numberOfWheels = NO_OF_WHEELS;
uint8_t currentWheelIndex = 0;
uint8_t currentRepeatIndex = 0;
uint8_t currentStep = 0;

void printWithStepper(void)
{
    for(currentWheelIndex = 0; currentWheelIndex < numberOfWheels; currentWheelIndex++)
    {
        for(currentRepeatIndex = 0; currentRepeatIndex < REPEAT_COUNT; currentRepeatIndex++)
        {
            for(currentStep = 0; currentStep < STEPS_PER_WHEEL; currentStep++)
            {
                bool result = stepperMotorMoveOneCamPosition();
                if(!result)
                {
                    Serial.println("Failed to move stepper motor");
                    while(1);
                }
                
                bool bitValue = getPatternBit(pattern, currentWheelIndex, currentStep);
                Serial.print( bitValue ? "#" : " " );
            }
            Serial.println();
        }
    }
}

void matrixKeypad(keypad_action_e action)
{
    static keypad_action_e lastAction =
    KEYPAD_ACTION_NONE;

    if(action != lastAction)
    {
        lastAction = action;

        switch(action)
        {
            case KEYPAD_ACTION_UP:
                Serial.println("UP");
                break;

            case KEYPAD_ACTION_DOWN:
                Serial.println("DOWN");
                break;

            case KEYPAD_ACTION_LEFT:
                Serial.println("LEFT");
                break;

            case KEYPAD_ACTION_RIGHT:
                Serial.println("RIGHT");
                break;

            case KEYPAD_ACTION_SELECT:
                Serial.println("SELECT");
                break;

            default:
                break;
        }
    }
}
void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Jacquard Cam Wheel");

    if (!stepperMotorInit())
    {
        Serial.println("Failed to initialize stepper motor");
        while(1);
    }

    if(!initPatternFS())
    {
        Serial.println("Failed to initialize pattern file system");
        while(1);
    }

    if(!keypadInit())
    {
        Serial.println("Failed to initialize keypad");
        while(1);
    }

    // Load pattern from file system
    if (
        loadPatternFromFile(
            "/patterns/heart.bin",
            pattern
        )
    )
    {
        Serial.println("Pattern loaded successfully");
    }
    else
    {
        Serial.println("Failed to load pattern");
        while(1);
    }

    printPattern("heart", pattern);


}

void loop()
{
    keypad_action_e action = keypadRead();

    matrixKeypad(action);
    
}