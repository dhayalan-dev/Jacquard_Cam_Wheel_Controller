#include <Arduino.h>

#include "machine_config.h"
#include "stepper_motor.h"
#include "patterns.h"
#include "machine_controller.h"
#include "pattern_registry.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "reset_controller.h"


void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Jacquard Cam Wheel");

    if(!wifiManagerInit())
    {
        Serial.println("Failed to initialize Wi-Fi");
        while (1);
    }

    if(!webServerInit())
    {
        Serial.println("Failed to initialize web server");
        while (1);
    }

    if(!initPatternFS())
    {
        Serial.println("Failed to initialize pattern file system");
        while(1);
    }

    if (!machineControllerInit())
    {
        Serial.println("Failed to initialize machine controller");
        while(1);
    }
    
    if (!stepperMotorInit())
    {
        Serial.println("Failed to initialize stepper motor");
        while(1);
    }

    if(!resetControllerInit())
    {
        Serial.println("Failed to initialize reset controller");
        while(1);
    }

}

void loop()
{
    webServerUpdate();
    machineControllerUpdate();
    resetControllerUpdate();
    
    delay(100);
}