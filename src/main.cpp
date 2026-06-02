#include <Arduino.h>

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"
#include "machine_controller.h"
#include "pattern_registry.h"
#include "wifi_manager.h"
#include "web_server.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Jacquard Cam Wheel");

    if(!wifiManagerInit())
    {
        while (1);
    }

    if(!webServerInit())
    {
        while (1);
    }

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

}

void loop()
{
    webServerUpdate();
}