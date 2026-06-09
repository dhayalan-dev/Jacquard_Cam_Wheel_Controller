#include "stepper_motor.h"
#include "machine_config.h"

bool stepperMotorInit()
{
    pinMode(DIR_PIN,    OUTPUT);
    pinMode(STEP_PIN,   OUTPUT);
    pinMode(RST_PIN,    OUTPUT);
    pinMode(EN_PIN,     OUTPUT);
    
    digitalWrite(EN_PIN,    LOW);
    digitalWrite(DIR_PIN,   HIGH);
    digitalWrite(RST_PIN,   HIGH);
    digitalWrite(STEP_PIN,  LOW);

    delay(5); // Allow time for the motor to initialize

    return true;
}

bool stepperMotorStep(uint16_t microsteps)
{
    
    for(uint16_t i = 0; i < microsteps; i++)
    {
        // Generate one DRV8825 step pulse
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(STEP_PULSE_HIGH_US);


        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(STEP_PULSE_LOW_US);
    }

    return true;
}

bool stepperMotorMoveOneCamPosition()
{
    static uint8_t camStepCounter = 0;
    uint16_t requiredMicrosteps = BASE_MICROSTEPS_PER_CAM_POSITION; // Fixed microsteps per cam position

    camStepCounter++;
    if (camStepCounter >= EXTRA_MICROSTEP_INTERVAL) 
    {
        requiredMicrosteps++;
        camStepCounter = 0;
    }

    return stepperMotorStep(requiredMicrosteps);
}