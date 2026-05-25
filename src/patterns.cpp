#include "patterns.h"

#include <SPIFFS.h>

bool initPatternFS()
{
    if(!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return false;
    }

    Serial.println("SPIFFS Mounted");

    return true;
}

bool loadPatternFromFile(
    const char *filename,
    pattern_t &pattern
)
{
    File file = SPIFFS.open(filename, FILE_READ);
    if (!file)
    {
        Serial.println("Failed to open pattern file");
        return false;
    }

    if(file.size() != sizeof(pattern_t))
    {
        Serial.println("Pattern file size mismatch");
        file.close();
        return false;
    }

    size_t bytesRead = 
    file.read(
        (uint8_t *)&pattern,
        sizeof(pattern_t)
    );
    file.close();

    return (
        bytesRead == sizeof(pattern_t)
    );
}

bool getPatternBit(
    const pattern_t &pattern,
    uint8_t wheelIndex,
    uint8_t step
)
{
    if (step >= STEPS_PER_WHEEL) // Ensure step is within valid range
    {
        return false;
    }

    uint8_t byteIndex = step / 8; // Determine which byte contains the step
    uint8_t bitIndex = step % 8;  // Determine which bit within that byte

    if(wheelIndex >= NO_OF_WHEELS)
    {
        printf("Invalid wheel index: %d\n", wheelIndex);
        return false;
    }

    return (
        pattern.wheels[wheelIndex].data[byteIndex] 
        >> (7- bitIndex)
    ) & 0x01; // Shift the byte to the right and mask with 0x01 to get the bit value
}

void printPattern(
    const char *patternName,
    const pattern_t &pattern
)
{
    Serial.println();

    Serial.print("Pattern: "); Serial.println(patternName);

    Serial.println();

    for(uint8_t wheel = 0; wheel < NO_OF_WHEELS; wheel++)
    {
        Serial.print("W"); Serial.print(wheel); Serial.print(": ");

        for(uint8_t step = 0; step < STEPS_PER_WHEEL; step++)
        {
            bool bit = getPatternBit(
                pattern,
                wheel,
                step
            );
            Serial.print( bit ? "#" : " " );
        }
        Serial.println();
    }
}