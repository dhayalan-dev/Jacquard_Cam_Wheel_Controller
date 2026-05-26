#include "stepper_motor.h"
#include "machine_config.h"

bool stepperMotorInit()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(SLEEP_PIN, OUTPUT);
    pinMode(RESET_PIN, OUTPUT);

    digitalWrite(SLEEP_PIN, HIGH);
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);

    delay(5); // Allow time for the motor to initialize

    return true;
}

bool stepperMotorSleep(bool sleep)
{
    digitalWrite(SLEEP_PIN, sleep ? LOW : HIGH);
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

bool stepperMotorReset()
{
    digitalWrite(RESET_PIN, LOW);
    delayMicroseconds(STEP_PULSE_HIGH_US);

    digitalWrite(RESET_PIN, HIGH);
    delay(5); // Allow time for the motor to reset

    return true;
}