#include "reset_controller.h"

static reset_state_e currentState = RESET_CONTROLLER_IDLE;

static uint16_t currentStep = 0;

// =------------------Private Functions Start------------------= //
static bool _machineActuateSolenoid()
{   
    digitalWrite(SOLENOID_PIN_2, LOW);
    delay(SOLENOID_ACTUATION_DELAY_MS);

    digitalWrite(SOLENOID_PIN_2, HIGH);
    delay(SOLENOID_ACTUATION_DELAY_MS);

    return true;
}

bool _stepperMotorStep(uint16_t microsteps)
{
    
    for(uint16_t i = 0; i < microsteps; i++)
    {
        // Generate one DRV8825 step pulse
        digitalWrite(STEP_PIN_2, HIGH);
        delayMicroseconds(STEP_PULSE_HIGH_US);


        digitalWrite(STEP_PIN_2, LOW);
        delayMicroseconds(STEP_PULSE_LOW_US);
    }

    return true;
}

bool _stepperMotorMoveOneCamPosition()
{
    static uint8_t camStepCounter = 0;
    uint16_t requiredMicrosteps = BASE_MICROSTEPS_PER_CAM_POSITION; // Fixed microsteps per cam position

    camStepCounter++;
    if (camStepCounter >= EXTRA_MICROSTEP_INTERVAL) 
    {
        requiredMicrosteps++;
        camStepCounter = 0;
    }

    return _stepperMotorStep(requiredMicrosteps);
}
// =------------------Private Functions End------------------= //

// =------------------Public Functions Start------------------= //
bool resetControllerInit()
{
    pinMode(STEP_PIN_2,     OUTPUT);
    pinMode(EN_PIN_2,       OUTPUT);
    pinMode(SOLENOID_PIN_2, OUTPUT);

    digitalWrite(STEP_PIN_2, LOW);
    digitalWrite(EN_PIN_2,   HIGH); // Written assuming driver is active low
    digitalWrite(SOLENOID_PIN_2, HIGH);

    return true;
}

bool resetControllerStart()
{
    if(currentState == RESET_CONTROLLER_RUNNING)
    {
        return false;
    }
    else if (currentState == RESET_CONTROLLER_IDLE     || 
             currentState == RESET_CONTROLLER_COMPLETE ||
             currentState == RESET_CONTROLLER_ABORTED)
    {
        digitalWrite(EN_PIN_2, LOW);
        currentStep = 0;

        currentState = RESET_CONTROLLER_RUNNING;
        return true;
    }   

    return false;
}

void resetControllerUpdate()
{
    if(currentState != RESET_CONTROLLER_RUNNING)
    {
        return;
    }

    if(!_machineActuateSolenoid())
    {
        return;
    }

    _stepperMotorMoveOneCamPosition();
    currentStep++;

    if(currentStep >= CAM_POSITIONS)
    {
        digitalWrite(EN_PIN_2, HIGH); // Stop the stepper motor

        currentState = RESET_CONTROLLER_COMPLETE;
        Serial.println("Reset complete");
    }

}

void resetControllerAbort()
{
    currentStep = 0;
    digitalWrite(SOLENOID_PIN_2, HIGH);
    digitalWrite(EN_PIN_2, HIGH); // Stop the stepper motor

    currentState = RESET_CONTROLLER_ABORTED;
}

reset_state_e resetControllerGetState()
{
    return currentState;
}

uint16_t resetControllerGetStep()
{
    return currentStep;
}

bool resetControllerIsRunning()
{
    return (currentState == RESET_CONTROLLER_RUNNING);
}
// =------------------Public Functions End------------------= //