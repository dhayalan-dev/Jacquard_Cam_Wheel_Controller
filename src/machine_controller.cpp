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
    pinMode(SOLENOID_PIN, OUTPUT);
    digitalWrite(SOLENOID_PIN, HIGH);

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

    Serial.printf("Loaded Pattern: %s\n", filename);

    currentPatternFile = filename;
    currentState = MACHINE_STATE_READY;

    return true;
}

bool machineControllerStart()
{
    if(currentState == MACHINE_STATE_IDLE)
    {
        currentState = MACHINE_STATE_RUNNING;
        Serial.printf(
            "Starting Wheel %d\n",
            currentWheel + 1
        );

        return true;
    }

    if(currentState == MACHINE_STATE_RUNNING)
    {
        return false;
    }

    if(currentState == MACHINE_STATE_READY)
    {
        currentWheel = 0;
        currentRepeat = 0;
        currentStep = 0;

        stepperMotorEnable();
        currentState = MACHINE_STATE_RUNNING;

        return true;
    }

    if(currentState == MACHINE_STATE_WAIT_FOR_NEXT_WHEEL)
    {
        currentWheel++;

        if(currentWheel >= NO_OF_WHEELS)
        {
            currentState =
                MACHINE_STATE_COMPLETED;

            return true;
        }

        currentRepeat = 0;
        currentStep = 0;

        currentState =
            MACHINE_STATE_RUNNING;

        stepperMotorEnable();

        Serial.printf(
            "Starting Wheel %d\n",
            currentWheel + 1
        );

        return true;
    }

    return false;
}

bool machineControllerStop()
{
    digitalWrite(SOLENOID_PIN, HIGH);

    stepperMotorStop();
    currentState = MACHINE_STATE_IDLE;

    return true;
}

void machineControllerAbort()
{
    digitalWrite(SOLENOID_PIN, HIGH);

    stepperMotorStop();

    currentWheel  = 0;
    currentRepeat = 0;
    currentStep   = 0;

    currentState = MACHINE_STATE_ABORTED;

}

static bool _machineActuateSolenoid(bool state)
{
    Serial.print(
        state ? "#" : "."
    );
    
    digitalWrite(
        SOLENOID_PIN,
        state ? LOW : HIGH
    );
    delay(10);

    return true;
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

    if(!_machineActuateSolenoid(bitValue))
    {
        return;
    }

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

        stepperMotorStop();

        Serial.println("Wheel pattern completed");

        Serial.println("Change next wheel and press Start");

        return;
    }
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
    return ( (currentRepeat == 1) ? (LOGICAL_PATTERN_STEPS + currentStep) : currentStep );
}

uint8_t machineControllerGetProgress()
{
    uint32_t completedSteps =
    (
        (
            currentWheel * currentPattern.repeat
        ) + currentRepeat
    ) * LOGICAL_PATTERN_STEPS + currentStep;

    uint32_t totalSteps =
        NO_OF_WHEELS *
        currentPattern.repeat *
        LOGICAL_PATTERN_STEPS;

    if(completedSteps == 0) return 0;

    return (completedSteps * 100) / totalSteps;
}

const char* machineControllerGetCurrentPatternFile(void)
{
    return currentPatternFile.c_str();
}