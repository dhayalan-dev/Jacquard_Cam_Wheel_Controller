#include "stepper_motor.h"
#include "machine_config.h"
#include <Preferences.h>

uint16_t totalSteps = 0;

Preferences _pref;

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

    _pref.begin("stepper", false);

    totalSteps = _pref.getUInt("steps", 0);

    stepperMotorOrigin();

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

    /* Calculate the total steps moved, if total steps exceed 6400 reset back to zero */
    (totalSteps >= 6400 ? totalSteps = 0 : totalSteps += microsteps);

    _pref.putUInt("steps", totalSteps);

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

bool stepperMotorOrigin()
{
    if (totalSteps == 0) return true;

    digitalWrite(DIR_PIN, LOW);
    stepperMotorStep(totalSteps);

    digitalWrite(DIR_PIN, HIGH);
    return true;
}