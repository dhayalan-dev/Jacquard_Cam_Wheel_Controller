#include "stepper_motor.h"
#include "machine_config.h"

bool stepperMotorInit()
{
    pinMode(STEP_PIN_1,   OUTPUT);
    pinMode(EN_PIN_1,     OUTPUT);
    
    stepperMotorEnable();
    digitalWrite(STEP_PIN_1,  LOW);

    delay(5); // Allow time for the motor to initialize

    return true;
}

bool stepperMotorStep(uint16_t microsteps)
{
    
    for(uint16_t i = 0; i < microsteps; i++)
    {
        // Generate one DRV8825 step pulse
        digitalWrite(STEP_PIN_1, HIGH);
        delayMicroseconds(STEP_PULSE_HIGH_US);


        digitalWrite(STEP_PIN_1, LOW);
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

bool stepperMotorEnable()
{
    digitalWrite(EN_PIN_1, LOW);
    Serial.println("Enabled");
    return true;
}

bool stepperMotorStop()
{
    digitalWrite(EN_PIN_1, HIGH);
    Serial.println("Aborted");
    return true;
}