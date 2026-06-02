#include "machine_controller.h"

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"

static pattern_t currentPattern;

static machine_state_e currentState = MACHINE_STATE_IDLE;

static uint8_t currentWheel = 0;
static uint8_t currentRepeat = 0;
static uint8_t currentStep = 0;

bool machineControllerInit()
{
    currentState = MACHINE_STATE_IDLE;

    currentWheel = 0;
    currentRepeat = 0;
    currentStep = 0;

    return true;
}

bool machineControllerLoadPattern(const char *filename)
{
    if(currentState == MACHINE_STATE_RUNNING)
    {
        return false;
    }

    return loadPatternFromFile(
        filename,
        currentPattern
    );
}

bool machineControllerStart()
{
    if(currentState == MACHINE_STATE_RUNNING)
    {
        return false;
    }

    currentWheel = 0;
    currentRepeat = 0;
    currentStep = 0;

    currentState = MACHINE_STATE_RUNNING;

    return true;
}

bool machineControllerStop()
{
    currentState = MACHINE_STATE_IDLE;

    return true;
}

static void _machineActuateSolenoid(bool state)
{
    /*
     * TODO:
     * drive solenoid output
     */

    Serial.print(state ? "#" : ".");
}

void machineControllerUpdate()
{
    if(currentState != MACHINE_STATE_RUNNING)
    {
        return;
    }

    bool bitValue = getPatternBit(
            currentPattern,
            currentWheel,
            currentStep
        );

    _machineActuateSolenoid(bitValue);

    stepperMotorMoveOneCamPosition();

    currentStep++;

    if(currentStep >= LOGICAL_PATTERN_STEPS)
    {
        currentStep = 0;
        currentRepeat++;
        Serial.println();
    }

    if(currentRepeat >= REPEAT_COUNT)
    {
        currentRepeat = 0;
        currentState = MACHINE_STATE_WAIT_FOR_NEXT_WHEEL;
    }

    if(currentWheel >= NO_OF_WHEELS)
    {
        currentState = MACHINE_STATE_COMPLETED;
        Serial.println("\nPattern Complete");
    }
}

bool machineControllerNextWheel()
{
    if (currentState != MACHINE_STATE_WAIT_FOR_NEXT_WHEEL)
    {
        return false;
    }

    currentWheel++;

    if(currentWheel >= NO_OF_WHEELS)
    {
        currentState = MACHINE_STATE_COMPLETED;
        return true;
    }

    currentState = MACHINE_STATE_RUNNING;
    return true;
}

machine_state_e machineControllerGetState()
{
    return currentState;
}

uint8_t machineControllerGetWheelIndex()
{
    return currentWheel;
}

uint8_t machineControllerGetStepIndex()
{
    return currentStep;
}