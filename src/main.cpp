#include <Arduino.h>

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"

pattern_t pattern;
uint8_t numberOfWheels = NO_OF_WHEELS;
uint8_t currentWheelIndex = 0;
uint8_t currentRepeatIndex = 0;
uint8_t currentStep = 0;

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

    // printPattern("heart", pattern);
}

void loop()
{
    // Nothing to do in the main loop for now
}