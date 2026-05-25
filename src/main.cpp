#include <Arduino.h>

#include "config.h"
#include "patterns.h"

pattern_t pattern;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Jacquard Cam Wheel");

    if(!initPatternFS())
    {
        Serial.println("Failed to initialize pattern file system");
        while(1);
    }

    // Load pattern from file system
    if (
        loadPatternFromFile(
            "/patterns/diamond.bin",
            pattern
        )
    )
    {
        Serial.println("Pattern loaded successfully");
    }
    else
    {
        Serial.println("Failed to load pattern");
        while(1);
    }

    printPattern("diamond", pattern);
}

void loop()
{
    // Nothing to do in the main loop for now
}