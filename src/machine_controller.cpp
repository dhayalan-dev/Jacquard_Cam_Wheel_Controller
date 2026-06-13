#include "machine_controller.h"

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"

static pattern_t currentPattern;

static String currentPatternFile = "";

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

    if(!loadPatternFromFile(
        filename,
        currentPattern))
    {
        return false;
    }

    currentWheel = 0;
    currentStep = 0;
    currentRepeat = 0;

    currentPatternFile = filename;
    currentState = MACHINE_STATE_READY;

    return true;
}

bool machineControllerStart()
{
    if(currentState != MACHINE_STATE_READY &&
       currentState != MACHINE_STATE_WAIT_FOR_NEXT_WHEEL)
    {
        return false;
    }

    currentState = MACHINE_STATE_RUNNING;

    return true;
}

bool machineControllerStop()
{
    stepperMotorStop();
    currentState = MACHINE_STATE_IDLE;

    return true;
}

void machineControllerAbort()
{
    stepperMotorStop();
    currentState = MACHINE_STATE_ABORTED;
    stepperMotorEnable();
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

uint8_t machineControllerGetProgress()
{
    uint32_t completedSteps = (
    (
        currentWheel * currentPattern.repeat
    ) + currentRepeat) * STEPS_PER_WHEEL + currentStep;

    uint32_t totalSteps =
        NO_OF_WHEELS * currentPattern.repeat * STEPS_PER_WHEEL;

    if(totalSteps == 0) return 0;

    return (uint8_t)((completedSteps * 100UL) / totalSteps);
}

const char* machineControllerGetCurrentPatternFile(void)
{
    return currentPatternFile.c_str();
}