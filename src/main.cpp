#include <Arduino.h>

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"
#include "machine_controller.h"

pattern_t pattern;
uint8_t numberOfWheels = NO_OF_WHEELS;
uint8_t currentWheelIndex = 0;
uint8_t currentRepeatIndex = 0;
uint8_t currentStep = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Jacquard Cam Wheel");

    if(!initPatternFS())
    {
        Serial.println("Failed to initialize pattern file system");
        while(1);
    }
    
    if (!stepperMotorInit())
    {
        Serial.println("Failed to initialize stepper motor");
        while(1);
    }

    machineControllerInit();

    machineControllerLoadPattern("/pattern/heart.bin");

    machineControllerStart();
}

void loop()
{
    machineControllerUpdate();
    delay(100);
}